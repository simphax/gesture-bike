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
#define ELBOW_SHOULDER_DELTA_Y 60
#define ELBOW_SHOULDER_DELTA_X 20
#define HAND_ELBOW_DELTA_X 30
#define HAND_ELBOW_DELTA_Y 20

StopGesture::StopGesture()
{
    
}

bool StopGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float leftElbowX, leftElbowY, leftHandX, leftHandY, leftHandZ, leftShoulderX, leftShoulderY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().z,
                                                         &leftShoulderX, &leftShoulderY);
    if(status == nite::STATUS_OK) {
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
               
                float handElbowDeltaX = abs(leftHandX-leftElbowX);
                float elbowShoulderDeltaY = leftElbowY - leftShoulderY;
                float elbowShoulderDeltaX = leftShoulderX - leftElbowX;
                float handElbowDeltaY = leftHandY - leftElbowY;
                printf("DeltaX: %f\n", elbowShoulderDeltaX);
                
                if(handElbowDeltaX < HAND_ELBOW_DELTA_X && elbowShoulderDeltaY < ELBOW_SHOULDER_DELTA_Y && elbowShoulderDeltaX > ELBOW_SHOULDER_DELTA_X && leftElbowX < 100 && handElbowDeltaY > HAND_ELBOW_DELTA_Y && leftHandZ > 700) {
                    stopGestureCount++;
                    
                    if(stopGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                        return true;
                    }
                } else {
                    stopGestureCount = 0;
                }
            }
        }
    }
    return false;
}

void StopGesture::draw()
{
    /* Left square */
    glBegin( GL_POLYGON );
    glColor3f(1, 0, 0);
    glVertex3f(0.0f, 150.0f, 0.0f);
    glVertex3f(200.0f, 150.0f, 0.0f);
    glVertex3f(200.0f, 350.0f, 0.0f);
    glVertex3f(0.0f, 350.0f, 0.0f);
    glEnd();
    
    /* Right square */
    glBegin( GL_POLYGON );
    glColor3f(1, 0, 0);
    glVertex3f(440.0f, 150.0f, 0.0f);
    glVertex3f(640.0f, 150.0f, 0.0f);
    glVertex3f(640.0f, 350.0f, 0.0f);
    glVertex3f(440.0f, 350.0f, 0.0f);
    glEnd();
}

void StopGesture::resetDraw()
{
    
}