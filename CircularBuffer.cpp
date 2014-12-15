//
//  CircularBuffer.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-15.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(int size)
{
    count = 0;
    max = size;
    buffer = new float[size];
}

void CircularBuffer::add(float value)
{
    buffer[count++ % max] = value;
}

float CircularBuffer::getAvg()
{
    float sum = 0.0f;
    int size = (count<max)?count:max;
    for(int i=0; i<size; i++)
    {
        sum += buffer[i];
    }
    return sum/size;
}