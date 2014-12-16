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

#define X_HOLD_FRAMES_THRESHOLD 5
#define Y_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA_X 30
#define GESTURE_DELTA_Y 30

FlashlightGesture::FlashlightGesture()
{
    handJoint = nite::JointType::JOINT_RIGHT_HAND;
    headJoint = nite::JointType::JOINT_HEAD;

    
    deltaBufferX = new CircularBuffer(20);
    deltaBufferY = new CircularBuffer(10);
}


bool FlashlightGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    
    /** Detect right arm gesture **/
    float headX, headY, handX, handY;

    
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(headJoint).getPosition().x,
                                                         skeleton->getJoint(headJoint).getPosition().y,
                                                         skeleton->getJoint(headJoint).getPosition().z,
                                                         &headX, &headY);
    if(status == nite::STATUS_OK) {
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                             skeleton->getJoint(handJoint).getPosition().x,
                                                             skeleton->getJoint(handJoint).getPosition().y,
                                                             skeleton->getJoint(handJoint).getPosition().z,
                                                             &handX, &handY);
        
        if(status == nite::STATUS_OK) {
            //printf("Left shoulder: %f\n",leftShoulderY);
            
            deltaBufferX->add(abs(headX-handX));
            deltaBufferY->add(abs(headY-handY));
            
            
            float deltaX = deltaBufferX->getAvg();
            float deltaY = deltaBufferY->getAvg();
            //printf("DeltaY: %f\n", leftDeltaY);

            
            if(deltaX < GESTURE_DELTA_X) {
                XGestureCount++;
                
            } else {
                XGestureCount = 0;
            }
            
            if(deltaY < GESTURE_DELTA_Y) {
                YGestureCount++;
                
            } else {
                YGestureCount = 0;
            }
            
            
            if(XGestureCount > X_HOLD_FRAMES_THRESHOLD && YGestureCount > Y_HOLD_FRAMES_THRESHOLD) {
                return true;
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





