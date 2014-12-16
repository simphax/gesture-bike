//
//  StopGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__RightStopGesture__
#define __Skeleton__RightStopGesture__

#include <stdio.h>
#include "IGesture.h"

class RightStopGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds down the arm
    int stopGestureCount = 0;
    
    CircularBuffer *handElbowDeltaXBuffer;
    CircularBuffer *handElbowDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaXBuffer;
    
    
    //Animation Properties
    float opacityTotal = 0;
    signed int opacityDelta = 1;
    float lastFrameTime = 0;
    
    void animate();
    
public:
    RightStopGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
    
};

#endif /* defined(__Skeleton__StopGesture__) */
