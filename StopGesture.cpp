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

#define GESTURE_HOLD_FRAMES_THRESHOLD 15
#define ELBOW_SHOULDER_DELTA_Y 60
#define ELBOW_SHOULDER_DELTA_X 20
#define HAND_ELBOW_DELTA_X 30
#define HAND_ELBOW_DELTA_Y 20

StopGesture::StopGesture()
{
    handElbowDeltaXBuffer = new CircularBuffer(20);
    handElbowDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaXBuffer = new CircularBuffer(20);
    
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
            //printf("Left elbow: %f\n",leftElbowX);
            
            status = userTracker->convertJointCoordinatesToDepth(
                                                                 skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().x,
                                                                 skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().y,
                                                                 skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().z,
                                                                 &leftHandX, &leftHandY);
            leftHandZ = skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().z;
            
            if(status == nite::STATUS_OK) {
                //printf("Left hand: %f\n",leftHandX);
                //printf("Left hand Z: %f\n",leftHandZ);
                //>7-800
                
                handElbowDeltaXBuffer->add(abs(leftHandX - leftElbowX));
                handElbowDeltaYBuffer->add(leftHandY - leftElbowY);
                elbowShoulderDeltaYBuffer->add(leftElbowY - leftShoulderY);
                elbowShoulderDeltaXBuffer->add(leftShoulderX - leftElbowX);
               
                float handElbowDeltaX = handElbowDeltaXBuffer->getAvg();
                float handElbowDeltaY = handElbowDeltaYBuffer->getAvg();
                float elbowShoulderDeltaX = elbowShoulderDeltaXBuffer->getAvg();
                float elbowShoulderDeltaY = elbowShoulderDeltaYBuffer->getAvg();
                //printf("DeltaX: %f\n", elbowShoulderDeltaX);
                
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
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(300.0f, 0.0f, 0.0f);
    glVertex3f(300.0f, 230.0f, 0.0f);
    glVertex3f(0.0f, 230.0f, 0.0f);
    glEnd();
    
    /* Right square */
    glBegin( GL_POLYGON );
    glColor3f(1, 0, 0);
    glVertex3f(340.0f, 0.0f, 0.0f);
    glVertex3f(640.0f, 0.0f, 0.0f);
    glVertex3f(640.0f, 230.0f, 0.0f);
    glVertex3f(340.0f, 230.0f, 0.0f);
    glEnd();
    
    /* HUD squares */
    glBegin( GL_POLYGON );
    glColor3f(1.0, 0, 0);
    glVertex3f(0.0f, 320.0f, 0.0f);
    glVertex3f(80.0f, 320.0f, 0.0f);
    glVertex3f(80.0f, 480.0f, 0.0f);
    glVertex3f(0.0f, 480.0f, 0.0f);
    glEnd();
    
    glBegin( GL_POLYGON );
    glColor3f(1.0, 0, 0);
    glVertex3f(560.0f, 320.0f, 0.0f);
    glVertex3f(640.0f, 320.0f, 0.0f);
    glVertex3f(640.0f, 480.0f, 0.0f);
    glVertex3f(560.0f, 480.0f, 0.0f);
    glEnd();
    
    
}

void StopGesture::resetDraw()
{
    
}

void StopGesture::hudMessage(HUD *hud)
{
    
}