//
//  TurnRightGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "TurnRightGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>
#include "GLHelper.h"

#define ARROW_IMG_WIDTH 808
#define ARROW_IMG_HEIGHT 460


TurnRightGesture::TurnRightGesture() : TurnGesture(nite::JointType::JOINT_RIGHT_HAND, nite::JointType::JOINT_RIGHT_ELBOW, nite::JointType::JOINT_RIGHT_SHOULDER)
{
    texture = GLHelper::LoadTexture("arrow-right.bmp", ARROW_IMG_WIDTH, ARROW_IMG_HEIGHT);
}


void TurnRightGesture::draw()
{
    GLHelper::DrawBlackBox(854, 240,0, 0);
    GLHelper::DrawTexture(texture, ARROW_IMG_WIDTH/2, ARROW_IMG_HEIGHT/2, 854-ARROW_IMG_WIDTH/2, 0, opacityTotal, false, false);
    
    this->animate();
}

void TurnRightGesture::resetDraw()
{
    xTranslation = 0;
    opacityDelta = 1;
    opacityTotal = 0;
    
}

void TurnRightGesture::animate()
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

void TurnRightGesture::hudMessage(HUD *hud)
{
    
}

