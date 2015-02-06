//
//  PassingGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__PassingGesture__
#define __Skeleton__PassingGesture__

#include <stdio.h>
#include "IGesture.h"

class PassingGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds down the arm
    int PassingGestureCount = 0;
    
    CircularBuffer *handElbowDeltaXBuffer;
    CircularBuffer *handElbowDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaYBuffer;
    CircularBuffer *elbowShoulderDeltaXBuffer;
    
    
    //Animation Properties
    float yDisplace = 0;
    float lastFrameTime = 0;
    GLuint texture;
    GLuint textureHUD;
    
    void animate();
    
public:
    PassingGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
    
};

#endif /* defined(__Skeleton__PassingGesture__) */
