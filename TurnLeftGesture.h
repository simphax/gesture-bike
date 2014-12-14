//
//  TurnLeftGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__TurnLeftGesture__
#define __Skeleton__TurnLeftGesture__

#include <stdio.h>
#include "IGesture.h"

class TurnLeftGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds up the left arm
    int leftGestureCount = 0;
    
    //Priority - higher priority gestures get triggered first, lower priority gestures cannot interrupt higher ones
    int priority = 1;
    
    //Animation Properties
    float xTranslation = 0;
    float opacityTotal = 0;
    signed int opacityDelta = 1;
    float lastFrameTime = 0;
    
    void animate();
    
public:
    TurnLeftGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    int getPriority();
    bool getActiveState();
    void setActiveState(bool state);
    
};

#endif /* defined(__Skeleton__TurnLeftGesture__) */
