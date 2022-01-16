//
//  AFloat - low overhead +/-1.0f float support wrapper using 8-bit or 16-bit integer data for audio applications
//
//  Created by Chris Nash on 5/20/20.
//  Copyright © 2020 Chris Nash. All rights reserved.
//

#include "afloat.h"

int main(int argc, const char * argv[]) {
    afloat8::info();
    afloat8::check();
    
    signed short pcm = 0;
    afloat af;
    af = pcm;
    float f = -1.0f + af;
    printf("af = %f / f = %f\n", (double)af, (double)f);
    
    return 0;
}
