//
//  StopGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "StopGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA_Y -30

StopGesture::StopGesture()
{
    
}

bool StopGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float leftElbowX, leftElbowY, leftHandX, leftHandY, leftHandZ;
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().z,
                                                         &leftElbowX, &leftElbowY);
    if(status == nite::STATUS_OK) {
        printf("Left elbow: %f\n",leftElbowX);
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                             skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().x,
                                                             skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().y,
                                                             skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().z,
                                                             &leftHandX, &leftHandY);
        leftHandZ = skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().z;
        
        if(status == nite::STATUS_OK) {
            printf("Left hand: %f\n",leftHandX);
            printf("Left hand Z: %f\n",leftHandZ);
            //>7-800
           
            float leftDeltaX = abs(leftHandX-leftElbowX);
            //printf("DeltaY: %f\n", leftDeltaY);
            
             
             
             
            if(leftDeltaX > GESTURE_DELTA_Y && leftElbowX < 100 && leftHandX < leftElbowX+20 && leftHandZ > 700) {
                stopGestureCount++;
                
                if(stopGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            } else {
                stopGestureCount = 0;
            }
        }
    }
    return false;
}

void StopGesture::draw()
{
    /* Left arrow */
    glBegin( GL_POLYGON );
    glColor3f(1, 0, 0);
    glVertex3f(100.0f, 245.0f, 0.0f);
    glVertex3f(0.0f, 150.0f, 0.0f);
    glVertex3f(0.0f, 350.0f, 0.0f);
    glEnd();
}

void StopGesture::resetDraw()
{
    
}