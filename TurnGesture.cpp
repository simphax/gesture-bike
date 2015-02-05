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

/* With Stop Gesture
#define ELBOW_HOLD_FRAMES_THRESHOLD 10
#define HAND_HOLD_FRAMES_THRESHOLD 8
#define ELBOW_GESTURE_DELTA_Y 40
#define HAND_GESTURE_DELTA_Y 40
#define Z_MIN 800
*/

#define ELBOW_HOLD_FRAMES_THRESHOLD 3
#define HAND_HOLD_FRAMES_THRESHOLD 0
#define ELBOW_GESTURE_DELTA_X 40
#define ELBOW_GESTURE_DELTA_Y 60
#define HAND_GESTURE_DELTA_Y 200

#define Z_MIN 800

TurnGesture::TurnGesture(nite::JointType handJoint, nite::JointType elbowJoint, nite::JointType shoulderJoint, bool isLeft)
{
    this->handJoint = handJoint;
    this->elbowJoint = elbowJoint;
    this->shoulderJoint = shoulderJoint;
    
    this->isLeft = isLeft;
    
    elbowDeltaXBuffer = new CircularBuffer(10);
    
    elbowDeltaYBuffer = new CircularBuffer(20);
    handDeltaYBuffer = new CircularBuffer(10);
}


bool TurnGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect right arm gesture **/
    float elbowX, elbowY, handX, handY, handZ, shoulderX, shoulderY, elbowZ;
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(handJoint).getPosition().x,
                                                         skeleton->getJoint(handJoint).getPosition().y,
                                                         skeleton->getJoint(handJoint).getPosition().z,
                                                         &handX, &handY);
    handZ =skeleton->getJoint(handJoint).getPosition().z;
    if(status == nite::STATUS_OK) {
        status = userTracker->convertJointCoordinatesToDepth(
                                                             skeleton->getJoint(elbowJoint).getPosition().x,
                                                             skeleton->getJoint(elbowJoint).getPosition().y,
                                                             skeleton->getJoint(elbowJoint).getPosition().z,
                                                             &elbowX, &elbowY);
        elbowZ = skeleton->getJoint(elbowJoint).getPosition().z;
        if(status == nite::STATUS_OK) {
            //printf("Left elbow: %f\n",leftElbowY);
            
            status = userTracker->convertJointCoordinatesToDepth(
                                                                 skeleton->getJoint(shoulderJoint).getPosition().x,
                                                                 skeleton->getJoint(shoulderJoint).getPosition().y,
                                                                 skeleton->getJoint(shoulderJoint).getPosition().z,
                                                                 &shoulderX, &shoulderY);
            
            if(status == nite::STATUS_OK) {
                //printf("Left shoulder: %f\n",leftShoulderY);
                
                
                //Fill Buffers
                if(this->isLeft){
                    elbowDeltaXBuffer->add(shoulderX-elbowX);
                }else{
                    elbowDeltaXBuffer->add(elbowX-shoulderX);
                }
                
                elbowDeltaYBuffer->add(abs(shoulderY-elbowY));
                handDeltaYBuffer->add(abs(shoulderY-handY));
                
            
                //Get Deltas
                float elbowDeltaX = elbowDeltaXBuffer->getAvg();
                
                float elbowDeltaY = elbowDeltaYBuffer->getAvg();
                float handDeltaY = handDeltaYBuffer->getAvg();
                
                
                //if (this->isLeft)
                //printf("Should DeltaX: %f\n", elbowDeltaX);
                //printf("elbowDeltaY: %f\n", elbowDeltaY);
                //printf("handDeltaY: %f\n", handDeltaY);
                
                
                if(elbowDeltaY < ELBOW_GESTURE_DELTA_Y && elbowDeltaX > ELBOW_GESTURE_DELTA_X) {
                    elbowGestureCount++;
                    
                } else {
                    elbowGestureCount = 0;
                }
                
                if(handDeltaY < HAND_GESTURE_DELTA_Y) {
                    handGestureCount++;
                    
                } else {
                    handGestureCount = 0;
                }
                
                
                if(elbowGestureCount > ELBOW_HOLD_FRAMES_THRESHOLD && handGestureCount > HAND_HOLD_FRAMES_THRESHOLD && elbowZ > Z_MIN && handZ > Z_MIN) {
                    return true;
                }
            }
        }
    }
    return false;
}