//
//  StopGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "RightStopGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>
#include "GLHelper.h"

#define GESTURE_HOLD_FRAMES_THRESHOLD 15
#define ELBOW_SHOULDER_DELTA_Y 60
#define ELBOW_SHOULDER_DELTA_X 30
#define HAND_ELBOW_DELTA_X 50
#define HAND_ELBOW_DELTA_Y 20
#define HAND_Z_MIN 700

#define STOP_IMG_WIDTH 808
#define STOP_IMG_HEIGHT 440

#define STOP_IMG_NORMAL_WIDTH 540
#define STOP_IMG_NORMAL_HEIGHT 440

RightStopGesture::RightStopGesture()
{
    handElbowDeltaXBuffer = new CircularBuffer(20);
    handElbowDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaXBuffer = new CircularBuffer(20);
    
    this->textureLeft = GLHelper::LoadTexture( "stop-left.bmp", STOP_IMG_WIDTH, STOP_IMG_HEIGHT );
    this->textureRight = GLHelper::LoadTexture( "stop-right.bmp", STOP_IMG_WIDTH, STOP_IMG_HEIGHT );
    this->textureNormal = GLHelper::LoadTexture( "stop.bmp", STOP_IMG_NORMAL_WIDTH, STOP_IMG_NORMAL_HEIGHT );
}

bool RightStopGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float leftElbowX, leftElbowY, leftHandX, leftHandY, leftHandZ, leftShoulderX, leftShoulderY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().z,
                                                         &leftShoulderX, &leftShoulderY);
    if(status == nite::STATUS_OK) {
        status = userTracker->convertJointCoordinatesToDepth(
                                                             skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().x,
                                                             skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().y,
                                                             skeleton->getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().z,
                                                             &leftElbowX, &leftElbowY);
        if(status == nite::STATUS_OK) {
            //printf("Left elbow: %f\n",leftElbowX);
            
            status = userTracker->convertJointCoordinatesToDepth(
                                                                 skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().x,
                                                                 skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().y,
                                                                 skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().z,
                                                                 &leftHandX, &leftHandY);
            leftHandZ = skeleton->getJoint(nite::JointType::JOINT_RIGHT_HAND).getPosition().z;
            
            if(status == nite::STATUS_OK) {
                //printf("Left hand: %f\n",leftHandX);
                //printf("Left hand Z: %f\n",leftHandZ);
                //>7-800
                
                handElbowDeltaXBuffer->add(abs(leftHandX - leftElbowX));
                handElbowDeltaYBuffer->add(leftHandY - leftElbowY);
                elbowShoulderDeltaYBuffer->add(leftElbowY - leftShoulderY);
                elbowShoulderDeltaXBuffer->add(leftElbowX - leftShoulderX);
               
                float handElbowDeltaX = handElbowDeltaXBuffer->getAvg();
                float handElbowDeltaY = handElbowDeltaYBuffer->getAvg();
                float elbowShoulderDeltaX = elbowShoulderDeltaXBuffer->getAvg();
                float elbowShoulderDeltaY = elbowShoulderDeltaYBuffer->getAvg();
                printf("DeltaX: %f\n", elbowShoulderDeltaX);
                
                if(handElbowDeltaX < HAND_ELBOW_DELTA_X && elbowShoulderDeltaY < ELBOW_SHOULDER_DELTA_Y && elbowShoulderDeltaX > ELBOW_SHOULDER_DELTA_X && handElbowDeltaY > HAND_ELBOW_DELTA_Y && leftHandZ > HAND_Z_MIN) {
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

void RightStopGesture::draw()
{
    
    GLHelper::DrawTexture(this->textureLeft, STOP_IMG_WIDTH/2, STOP_IMG_HEIGHT/2, 0, 0, opacityTotal, false, true);
    GLHelper::DrawTexture(this->textureRight, STOP_IMG_WIDTH/2, STOP_IMG_HEIGHT/2, 854 - STOP_IMG_WIDTH/2, 0, opacityTotal, false, true);
    
    //HUD small stop signs
    GLHelper::DrawTexture(this->textureNormal, STOP_IMG_NORMAL_WIDTH/3, STOP_IMG_NORMAL_HEIGHT/3, 80, 300, opacityTotal, false, false);
    
    GLHelper::DrawTexture(this->textureNormal, STOP_IMG_NORMAL_WIDTH/3, STOP_IMG_NORMAL_HEIGHT/3, 594, 300, opacityTotal, false, false);
    
    this->animate();
}

void RightStopGesture::resetDraw()
{
    
}


void RightStopGesture::animate()
{
    if(lastFrameTime + FRAMETIME < glutGet(GLUT_ELAPSED_TIME) ) {
        if(opacityTotal <= 0.5) {
            opacityDelta = 1;
        }
        if(opacityTotal >= 0.9) {
            opacityDelta = -opacityDelta;
        }
        
        opacityTotal += opacityDelta * 0.025;
        
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}

void RightStopGesture::hudMessage(HUD *hud)
{
    
}