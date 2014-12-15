//
//  StopGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__StopGesture__
#define __Skeleton__StopGesture__

#include <stdio.h>
#include "IGesture.h"

class StopGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds down the arm
    int stopGestureCount = 0;
    
public:
    StopGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
};

#endif /* defined(__Skeleton__StopGesture__) */
