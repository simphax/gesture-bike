//
//  CircularBuffer.h
//  Skeleton
//
//  Created by Simon on 2014-12-15.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__CircularBuffer__
#define __Skeleton__CircularBuffer__

#include <stdio.h>

class CircularBuffer
{
private:
    //Counts the amount of frames that the user holds up the left arm
    float *buffer;
    int count;
    int max;
    
public:
    CircularBuffer(int size);
    void add(float value);
    float getAvg();
};

#endif /* defined(__Skeleton__CircularBuffer__) */
