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
#include "TurnGesture.h"

class TurnLeftGesture : public TurnGesture
{
private:
    
    //Animation Properties
    float xTranslation = 0;
    float opacityTotal = 0;
    float opacity1 = 0;
    float opacity2 = 0;
    float opacity3 = 0;
    signed int opacityDelta = 1;
    float lastFrameTime = 0;
    
    void animate();
    
    GLuint arrow1, arrow2, arrow3, arrowHUD1, arrowHUD2, arrowHUD3;
    
public:
    TurnLeftGesture();
    void draw();
    void resetDraw();
    void hudMessage(HUD *hud);
};

#endif /* defined(__Skeleton__TurnLeftGesture__) */
