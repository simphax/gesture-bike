//
//  TurnRightGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__TurnRightGesture__
#define __Skeleton__TurnRightGesture__

#include <stdio.h>
#include "IGesture.h"

class TurnRightGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds up the left arm
    int rightGestureCount = 0;
    float xTranslation = 0;
    float lastFrameTime = 0;
    
    void animate();
    
public:
    TurnRightGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
};

#endif /* defined(__Skeleton__TurnRightGesture__) */
