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
#include "TurnGesture.h"
#include "CircularBuffer.h"

class TurnRightGesture : public TurnGesture
{
private:
    //Animation Properties
    float xTranslation = 0;
    float opacityTotal = 0;
    signed int opacityDelta = 1;
    float lastFrameTime = 0;
    
    void animate();
    
    GLuint texture;
    GLuint textureNormal;
    
public:
    TurnRightGesture();
    
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
};

#endif /* defined(__Skeleton__TurnRightGesture__) */
