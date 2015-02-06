//
//  HazardGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__HazardGesture__
#define __Skeleton__HazardGesture__

#include <stdio.h>
#include "IGesture.h"

class HazardGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds down the arm
    int HazardGestureCount = 0;
    
    CircularBuffer *handElbowDeltaXBuffer;
    CircularBuffer *handElbowDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaXBuffer;
    
    
    //Animation Properties
    float opacityTotal = 0;
    signed int opacityDelta = 1;
    float lastFrameTime = 0;
    GLuint texture;
    GLuint textureHUD;
    
    void animate();
    
public:
    HazardGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
    
};

#endif /* defined(__Skeleton__HazardGesture__) */
