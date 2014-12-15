//
//  MapGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__MapGesture__
#define __Skeleton__MapGesture__

#include <stdio.h>
#include "IGesture.h"
#include <cmath>        // std::abs

class MapGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds up the left arm
    int MapGestureCount = 0;
    
    
public:
    MapGesture();
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
    
};

#endif /* defined(__Skeleton__MapGesture__) */
