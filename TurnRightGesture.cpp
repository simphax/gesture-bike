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

#define GESTURE_HOLD_FRAMES_THRESHOLD 30
#define GESTURE_DELTA_Y -30


TurnRightGesture::TurnRightGesture()
{
    
}


bool TurnRightGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect right arm gesture **/
    float rightElbowX, rightElbowY, rightShoulderX, rightShoulderY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().x,
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().y,
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().z,
                                                     &rightElbowX, &rightElbowY);
    if(status == nite::STATUS_OK) {
        //printf("Left elbow: %f\n",leftElbowY);
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().z,
                                                         &rightShoulderX, &rightShoulderY);
        
        if(status == nite::STATUS_OK) {
            //printf("Left shoulder: %f\n",leftShoulderY);
            
            float rightDeltaY = rightShoulderY-rightElbowY;
            //printf("DeltaY: %f\n", leftDeltaY);
            
            if(rightDeltaY > GESTURE_DELTA_Y) {
                rightGestureCount++;
                
                if(rightGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            } else {
                rightGestureCount = 0;
            }
        }
    }    return false;
}

void TurnRightGesture::draw()
{
    /* Right arrow */
    glBegin( GL_POLYGON );
    glColor3f(1.0 * opacityTotal, 1 * opacityTotal, 0 * opacityTotal);
    glVertex3f(640.0f, 0.0f, 0.0f);
    glVertex3f(640.0f, 150.0f, 0.0f);
    glVertex3f(490.0f, 0.0f, 0.0f);
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

