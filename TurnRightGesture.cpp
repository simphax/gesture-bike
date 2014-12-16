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


TurnRightGesture::TurnRightGesture() : TurnGesture(nite::JointType::JOINT_RIGHT_HAND, nite::JointType::JOINT_RIGHT_ELBOW, nite::JointType::JOINT_RIGHT_SHOULDER)
{
}


void TurnRightGesture::draw()
{
    /* Right arrow */
    glBegin( GL_POLYGON );
    glColor3f(1.0 * opacityTotal, 1 * opacityTotal, 0 * opacityTotal);
    glVertex3f(640.0f, 0.0f, 0.0f);
    glVertex3f(640.0f, 230.0f, 0.0f);
    glVertex3f(340.0f, 0.0f, 0.0f);
    glEnd();
    
    
    /* HUD square signal */
    glBegin( GL_POLYGON );
    glColor3f(1.0 * opacityTotal, 1 * opacityTotal, 0 * opacityTotal);
    glVertex3f(560.0f, 320.0f, 0.0f);
    glVertex3f(640.0f, 320.0f, 0.0f);
    glVertex3f(640.0f, 480.0f, 0.0f);
    glVertex3f(560.0f, 480.0f, 0.0f);
    glEnd();
    
    
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

