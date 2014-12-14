//
//  IGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef Skeleton_IGesture_h
#define Skeleton_IGesture_h

#include "NiTE.h"

class IGesture
{
public:
    virtual ~IGesture() {}
    /**
     @brief Process skeleton to detect gesture.
     @return True if gesture was recognized. False if gesture ended.
     */
    virtual bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker) = 0;
    /**
     @brief Draws the graphics that indicate the gesture function 
     */
    virtual void draw() = 0;
};

#endif

