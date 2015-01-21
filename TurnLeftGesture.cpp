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

#define GESTURE_HOLD_FRAMES_THRESHOLD 30
#define GESTURE_DELTA_Y -30

#define ARROW_IMG_WIDTH 540
#define ARROW_IMG_HEIGHT 460

TurnLeftGesture::TurnLeftGesture() : TurnGesture(nite::JointType::JOINT_LEFT_HAND, nite::JointType::JOINT_LEFT_ELBOW, nite::JointType::JOINT_LEFT_SHOULDER)
{
    texture = GLHelper::LoadTexture("arrow.bmp", ARROW_IMG_WIDTH, ARROW_IMG_HEIGHT);
}

void TurnLeftGesture::draw()
{
    
    GLHelper::DrawTexture(texture, ARROW_IMG_WIDTH/2, ARROW_IMG_HEIGHT/2, 0, 0, opacityTotal, false, false);
    
    this->animate();
    
}

void TurnLeftGesture::resetDraw()
{
    xTranslation = 0;
    opacityDelta = 1;
    opacityTotal = 0;
    
}

void TurnLeftGesture::animate()
{
    if(lastFrameTime + FRAMETIME < glutGet(GLUT_ELAPSED_TIME) ) {
        
        
        if(opacityTotal <= 0.1) {
            opacityDelta = 1;
        }
        if(opacityTotal >= 0.9) {
            opacityDelta = -opacityDelta;
        }
        
        opacityTotal += opacityDelta * 0.1;
        
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}


void TurnLeftGesture::hudMessage(HUD *hud)
{
    
}

