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

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA_Y -30


TurnLeftGesture::TurnLeftGesture()
{
    
}


bool TurnLeftGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float leftElbowX, leftElbowY, leftShoulderX, leftShoulderY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().x,
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().y,
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().z,
                                                     &leftElbowX, &leftElbowY);
    if(status == nite::STATUS_OK) {
        //printf("Left elbow: %f\n",leftElbowY);
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().z,
                                                         &leftShoulderX, &leftShoulderY);
        
        if(status == nite::STATUS_OK) {
            //printf("Left shoulder: %f\n",leftShoulderY);
            
            float leftDeltaY = leftShoulderY-leftElbowY;
            //printf("DeltaY: %f\n", leftDeltaY);
            
            if(leftDeltaY > GESTURE_DELTA_Y) {
                leftGestureCount++;
                
                if(leftGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            } else {
                leftGestureCount = 0;
            }
        }
    }
    return false;
}



void TurnLeftGesture::draw()
{
    
    /* Left arrow */
    glBegin( GL_POLYGON );
    glColor3f(1, 1, 0);
    glVertex3f(100.0f + xTranslation, 245.0f, 0.0f);
    glVertex3f(0.0f + xTranslation, 150.0f, 0.0f);
    glVertex3f(0.0f + xTranslation, 350.0f, 0.0f);
    glEnd();
    
    this->animate();
    
}

void TurnLeftGesture::resetDraw()
{
    xTranslation = 0;
}

void TurnLeftGesture::animate()
{
    if(lastFrameTime + FRAMERATE < glutGet(GLUT_ELAPSED_TIME) ) {
        xTranslation ++;
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}



