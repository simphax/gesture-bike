//
//  FlashlightGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__FlashlightGesture__
#define __Skeleton__FlashlightGesture__

#include <stdio.h>
#include "IGesture.h"
#include <cmath>        // std::abs

class FlashlightGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds up the left arm
    int flashlightGestureCount = 0;
    
    
public:
    FlashlightGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    
};

#endif /* defined(__Skeleton__FlashlightGesture__) */
