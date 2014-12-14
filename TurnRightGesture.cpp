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

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
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
    glColor3f(1, 1, 0);
    glVertex3f(640.0f, 245.0f, 0.0f);
    glVertex3f(540.0f, 150.0f, 0.0f);
    glVertex3f(540.0f, 350.0f, 0.0f);
    glEnd();
}