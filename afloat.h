//
//  afloat.h
//  AFloat
//
//  Created by Chris Nash on 16/01/2022.
//  Copyright © 2022 Chris Nash. All rights reserved.
//

#pragma once

#include <math.h>
#include <iostream>

template<class TYPE>
union afloat_t {
    float f;
    struct fp {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
    TYPE pcm;
    
    afloat_t() : f(_zero) {}
    
    static float encode(TYPE pcm){
        unsigned int i = pcm | ((sizeof(TYPE) == 2 ? 135 : 143) << 23);
        return *(float*)&i;
    }
    
    operator float() const {
        return f - _bias;
    }
    
    afloat_t& operator=(signed char x){
        pcm = (TYPE)((x << (sizeof(TYPE) == 2 ? 8 : 0)) + (1<<(sizeof(TYPE)*8-1)));
        return *this;
    }
    
    afloat_t& operator=(unsigned char x){
        pcm = (x << (sizeof(TYPE) == 2 ? 8 : 0));
        return *this;
    }
    
    afloat_t& operator=(signed short x){
        pcm = (TYPE)(x + (1<<(sizeof(TYPE)*8-1)));
        return *this;
    }
    
    afloat_t& operator=(unsigned short x){
        pcm = x;
        return *this;
    }
    
    afloat_t& operator=(float x){
        f = x + _bias;
        return *this;
    }
    
    afloat_t& operator=(double x){
        f = x + _bias;
        return *this;
    }

#define AFLOAT_BUFFER_SIZE 17
    static void info() {
        float buffer[AFLOAT_BUFFER_SIZE] = { 0 };
        TYPE ibuffer[AFLOAT_BUFFER_SIZE] = { 0 };
        
        printf("Memory: %d\n", (int)sizeof(afloat_t));
        printf("Saving: %d > %d\n", (int)sizeof(buffer), (int)sizeof(ibuffer));
        printf("bias = %f\n", afloat_t::_bias);
    }
    
    static bool check() {
        bool status = true;
        
        const int RANGE = 1<<(sizeof(TYPE)*8);
        float buffer[AFLOAT_BUFFER_SIZE] = { 0 };
        unsigned short ibuffer[AFLOAT_BUFFER_SIZE] = { 0 };
        for(int x=0; x<AFLOAT_BUFFER_SIZE; x++){
            buffer[x] = x / (float)(AFLOAT_BUFFER_SIZE - 1);
            ibuffer[x] = std::min((int)(buffer[x] * RANGE), RANGE - 1);
        }
        
        const float TOLERANCE = sizeof(TYPE) == 2 ? 0.0001f : 0.01f;
        
        afloat_t fl;
        for(int x=0; x<AFLOAT_BUFFER_SIZE; x++){
            fl.pcm = ibuffer[x];
            float correct = buffer[x] * 2 - 1;
            float encoded = fl;
            if(correct != encoded && fabs(correct-encoded) > TOLERANCE){
                if(status)
                    printf("afloat integrity check: failed\n");
                status = false;
                printf("  %f >> [%d|%d|%d] = %f", correct, fl.parts.mantissa, fl.parts.exponent, fl.parts.sign, encoded);
                printf(" (+%f)\n", fabs(correct-encoded));
            }else{
                //printf("  %f >> [%d|%d|%d] = %f\n", correct, fl.parts.mantissa, fl.parts.exponent, fl.parts.sign, encoded);
            }
        }
        if(status)
            printf("afloat integrity check: passed\n");
        
        return status;
    }
    
private:
    const static float _zero;
    const static float _bias;
};

template<class TYPE>
const float afloat_t<TYPE>::_zero = afloat_t::encode(0);
template<class TYPE>
const float afloat_t<TYPE>::_bias = afloat_t::encode(1<<(sizeof(TYPE)*8-1));

typedef afloat_t<unsigned short> afloat;
typedef afloat_t<unsigned char> afloat8;
