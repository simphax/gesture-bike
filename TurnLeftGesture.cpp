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

#define GESTURE_HOLD_FRAMES_THRESHOLD 30
#define GESTURE_DELTA_Y -30

TurnLeftGesture::TurnLeftGesture() : TurnGesture(nite::JointType::JOINT_LEFT_HAND, nite::JointType::JOINT_LEFT_ELBOW, nite::JointType::JOINT_LEFT_SHOULDER)
{
}

void TurnLeftGesture::draw()
{
    
    /* Left arrow */
    glBegin( GL_POLYGON );
    glColor3f(1.0 * opacityTotal, 1 * opacityTotal, 0 * opacityTotal);
    glVertex3f(0.0f, 230.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(300.0f, 0.0f, 0.0f);
    glEnd();
   
    /* HUD square signal */
    glBegin( GL_POLYGON );
    glColor3f(1.0 * opacityTotal, 1 * opacityTotal, 0 * opacityTotal);
    glVertex3f(0.0f, 320.0f, 0.0f);
    glVertex3f(80.0f, 320.0f, 0.0f);
    glVertex3f(80.0f, 480.0f, 0.0f);
    glVertex3f(0.0f, 480.0f, 0.0f);
    glEnd();
    
    
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

