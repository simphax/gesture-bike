//
//  TurnLeftGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "TurnLeftGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>
#include "GLHelper.h"

#define ARROW_IMG_WIDTH 808
#define ARROW_IMG_HEIGHT 440

#define ARROW_HUD_IMG_WIDTH 540
#define ARROW_HUD_IMG_HEIGHT 440


TurnLeftGesture::TurnLeftGesture() : TurnGesture(nite::JointType::JOINT_LEFT_HAND, nite::JointType::JOINT_LEFT_ELBOW, nite::JointType::JOINT_LEFT_SHOULDER, true)
{
    arrow1 = GLHelper::LoadTexture("arrow-1.bmp", ARROW_IMG_WIDTH, ARROW_IMG_HEIGHT);
    arrow2 = GLHelper::LoadTexture("arrow-2.bmp", ARROW_IMG_WIDTH, ARROW_IMG_HEIGHT);
    arrow3 = GLHelper::LoadTexture("arrow-3.bmp", ARROW_IMG_WIDTH, ARROW_IMG_HEIGHT);
    
    arrowHUD1 = GLHelper::LoadTexture("arrow-hud-1.bmp", ARROW_HUD_IMG_WIDTH, ARROW_HUD_IMG_HEIGHT);
    arrowHUD2 = GLHelper::LoadTexture("arrow-hud-2.bmp", ARROW_HUD_IMG_WIDTH, ARROW_HUD_IMG_HEIGHT);
    arrowHUD3 = GLHelper::LoadTexture("arrow-hud-3.bmp", ARROW_HUD_IMG_WIDTH, ARROW_HUD_IMG_HEIGHT);
}

void TurnLeftGesture::draw()
{
    
    GLHelper::DrawTexture(arrow1, ARROW_IMG_WIDTH/2, ARROW_IMG_HEIGHT/2, 0, 0, opacity1, false, false);
    GLHelper::DrawTexture(arrow2, ARROW_IMG_WIDTH/2, ARROW_IMG_HEIGHT/2, 0, 0, opacity2, false, false);
    GLHelper::DrawTexture(arrow3, ARROW_IMG_WIDTH/2, ARROW_IMG_HEIGHT/2, 0, 0, opacity3, false, false);
    
    //HUD Small version
    GLHelper::DrawTexture(arrowHUD1, ARROW_HUD_IMG_WIDTH/3, ARROW_HUD_IMG_HEIGHT/3, 80, 300, opacity1, false, false);
    GLHelper::DrawTexture(arrowHUD2, ARROW_HUD_IMG_WIDTH/3, ARROW_HUD_IMG_HEIGHT/3, 80, 300, opacity2, false, false);
    GLHelper::DrawTexture(arrowHUD3, ARROW_HUD_IMG_WIDTH/3, ARROW_HUD_IMG_HEIGHT/3, 80, 300, opacity3, false, false);
    
    this->animate();
    
}

void TurnLeftGesture::resetDraw()
{
    xTranslation = 0;
    opacityDelta = 1;
    opacityTotal = 0;
    opacity1 = 0;
    opacity2 = 0;
    opacity3 = 0;
    
}

void TurnLeftGesture::animate()
{
    if(lastFrameTime + FRAMETIME < glutGet(GLUT_ELAPSED_TIME) ) {
        
        
        if(opacityTotal >= 0.05 && opacityTotal < 0.4) {
            opacity1 += 0.1;
        }
        if(opacityTotal >= 0.4 && opacityTotal < 0.7) {
            opacity2 += 0.1;
        }
        if(opacityTotal >= 0.7) {
            opacity3 += 0.1;
        }
        if(opacityTotal >= 0.99) {
            opacityTotal = 0;
            opacity3 = 0;
            opacity2 = 0;
            opacity1 = 0;
        }
        
        opacityTotal += opacityDelta * 0.02;
                
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}


void TurnLeftGesture::hudMessage(HUD *hud)
{
    
}

