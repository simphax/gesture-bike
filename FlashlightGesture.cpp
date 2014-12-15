//
//  FlashlightGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "FlashlightGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA 40


FlashlightGesture::FlashlightGesture()
{
    
}


bool FlashlightGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float rightHandX, rightHandY, headX, headY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().x,
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().y,
                                                     skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().z,
                                                     &rightHandX, &rightHandY);
    if(status == nite::STATUS_OK) {
        //printf("Left elbow: %f\n",leftElbowY);
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().z,
                                                         &headX, &headY);
        
        if(status == nite::STATUS_OK) {
            //printf("Left shoulder: %f\n",leftShoulderY);
            
            float deltaY = std::abs(rightHandX - headX);
            float deltaX = std::abs(rightHandY - headY);
            
            //printf("DeltaY: %f\n", leftDeltaY);
            
            if(deltaY < GESTURE_DELTA && deltaX < GESTURE_DELTA) {
                flashlightGestureCount++;
                
                if(flashlightGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            } else {
                flashlightGestureCount = 0;
            }
        }
    }
    return false;
}

void FlashlightGesture::hudMessage(HUD *hud)
{
    hud->toggleFlashlight();
}

void FlashlightGesture::draw(){}
void FlashlightGesture::resetDraw() {}





