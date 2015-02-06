//
//  PassingGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "PassingGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>
#include "GLHelper.h"

#define GESTURE_HOLD_FRAMES_THRESHOLD 15
#define ELBOW_SHOULDER_DELTA_Y 60
#define ELBOW_SHOULDER_DELTA_X 30
#define HAND_ELBOW_DELTA_X 50
#define HAND_ELBOW_DELTA_Y 20
#define HAND_Z_MIN 700

#define Passing_IMG_WIDTH 808
#define Passing_IMG_HEIGHT 460

#define Passing_IMG_HUD_WIDTH 540
#define Passing_IMG_HUD_HEIGHT 460

PassingGesture::PassingGesture()
{
    handElbowDeltaXBuffer = new CircularBuffer(20);
    handElbowDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaYBuffer = new CircularBuffer(20);
    elbowShoulderDeltaXBuffer = new CircularBuffer(20);
    
    this->texture = GLHelper::LoadTexture( "passing.bmp", Passing_IMG_WIDTH, Passing_IMG_HEIGHT );
    this->textureHUD = GLHelper::LoadTexture( "passing-hud.bmp", Passing_IMG_HUD_WIDTH, Passing_IMG_HUD_HEIGHT );
}

bool PassingGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
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
                
                if(handElbowDeltaX < HAND_ELBOW_DELTA_X && elbowShoulderDeltaY < ELBOW_SHOULDER_DELTA_Y && elbowShoulderDeltaX > ELBOW_SHOULDER_DELTA_X && handElbowDeltaY > HAND_ELBOW_DELTA_Y && leftHandZ > HAND_Z_MIN) {
                    PassingGestureCount++;
                    
                    if(PassingGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                        return true;
                    }
                } else {
                    PassingGestureCount = 0;
                }
            }
        }
    }
    return false;
}

void PassingGesture::draw()
{
    
    GLHelper::DrawTexture(this->texture, Passing_IMG_WIDTH/2, Passing_IMG_HEIGHT/2, 0, 0, 1, false, false);

    GLHelper::DrawBlackBox(Passing_IMG_WIDTH/2, Passing_IMG_HEIGHT/2, yDisplace, 0);
    
    //HUD small Passing signs
    GLHelper::DrawTexture(this->textureHUD, Passing_IMG_HUD_WIDTH/3, Passing_IMG_HUD_HEIGHT/3, 0, 300, 1, false, false);
    
    
    this->animate();
    
}

void PassingGesture::resetDraw()
{
    this->yDisplace = 0;
}


void PassingGesture::animate()
{
    if(lastFrameTime + FRAMETIME < glutGet(GLUT_ELAPSED_TIME) ) {
        
        yDisplace -= 3;
        
        
        if(yDisplace <= -300) {
            yDisplace = 0;
        }
        
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}

void PassingGesture::hudMessage(HUD *hud)
{
    
}