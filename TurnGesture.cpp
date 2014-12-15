//
//  TurnGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-15.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "TurnGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>

#define ELBOW_HOLD_FRAMES_THRESHOLD 10
#define HAND_HOLD_FRAMES_THRESHOLD 5
#define ELBOW_GESTURE_DELTA_Y 30
#define HAND_GESTURE_DELTA_Y 40


TurnGesture::TurnGesture(nite::JointType handJoint, nite::JointType elbowJoint, nite::JointType shoulderJoint)
{
    this->handJoint = handJoint;
    this->elbowJoint = elbowJoint;
    this->shoulderJoint = shoulderJoint;
    elbowDeltaYBuffer = new CircularBuffer(20);
    handDeltaYBuffer = new CircularBuffer(10);
}


bool TurnGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect right arm gesture **/
    float elbowX, elbowY, handX, handY, shoulderX, shoulderY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(handJoint).getPosition().x,
                                                         skeleton->getJoint(handJoint).getPosition().y,
                                                         skeleton->getJoint(handJoint).getPosition().z,
                                                         &handX, &handY);
    if(status == nite::STATUS_OK) {
        status = userTracker->convertJointCoordinatesToDepth(
                                                             skeleton->getJoint(elbowJoint).getPosition().x,
                                                             skeleton->getJoint(elbowJoint).getPosition().y,
                                                             skeleton->getJoint(elbowJoint).getPosition().z,
                                                             &elbowX, &elbowY);
        if(status == nite::STATUS_OK) {
            //printf("Left elbow: %f\n",leftElbowY);
            
            status = userTracker->convertJointCoordinatesToDepth(
                                                                 skeleton->getJoint(shoulderJoint).getPosition().x,
                                                                 skeleton->getJoint(shoulderJoint).getPosition().y,
                                                                 skeleton->getJoint(shoulderJoint).getPosition().z,
                                                                 &shoulderX, &shoulderY);
            
            if(status == nite::STATUS_OK) {
                //printf("Left shoulder: %f\n",leftShoulderY);
                
                elbowDeltaYBuffer->add(abs(shoulderY-elbowY));
                handDeltaYBuffer->add(abs(shoulderY-handY));
                
                
                float elbowDeltaY = elbowDeltaYBuffer->getAvg();
                float handDeltaY = handDeltaYBuffer->getAvg();
                //printf("DeltaY: %f\n", leftDeltaY);
                printf("elbowDeltaY: %f\n", elbowDeltaY);
                printf("handDeltaY: %f\n", handDeltaY);
                
                if(elbowDeltaY < ELBOW_GESTURE_DELTA_Y) {
                    elbowGestureCount++;
                    
                } else {
                    elbowGestureCount = 0;
                }
                
                if(handDeltaY < HAND_GESTURE_DELTA_Y) {
                    handGestureCount++;
                    
                } else {
                    handGestureCount = 0;
                }
                
                
                if(elbowGestureCount > ELBOW_HOLD_FRAMES_THRESHOLD && handGestureCount > HAND_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            }
        }
    }
    return false;
}