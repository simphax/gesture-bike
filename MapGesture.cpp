//
//  MapGesture.cpp
//  Skeleton
//
//  Created by Simon on 2014-12-14.
//  Copylright(c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "MapGesture.h"
#include <GLUT/GLUT.h>
#include <stdio.h>

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA 40


MapGesture::MapGesture()
{
    
}


bool MapGesture::gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker)
{
    nite::Status status;
    /** Detect Left arm gesture **/
    float leftHandX, leftHandY, headX, headY;
    status = userTracker->convertJointCoordinatesToDepth(
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().x,
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().y,
                                                     skeleton->getJoint(nite::JointType::JOINT_LEFT_HAND).getPosition().z,
                                                     &leftHandX, &leftHandY);
    if(status == nite::STATUS_OK) {
        //printf("Left elbow: %f\n",leftElbowY);
        
        status = userTracker->convertJointCoordinatesToDepth(
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().x,
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().y,
                                                         skeleton->getJoint(nite::JointType::JOINT_HEAD).getPosition().z,
                                                         &headX, &headY);
        
        if(status == nite::STATUS_OK) {
            //printf("Left shoulder: %f\n",leftShoulderY);
            
            float deltaY = std::abs(leftHandX - headX);
            float deltaX = std::abs(leftHandY - headY);
            
            //printf("DeltaY: %f\n", leftDeltaY);
            
            if(deltaY < GESTURE_DELTA && deltaX < GESTURE_DELTA) {
                MapGestureCount++;
                
                if(MapGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                    return true;
                }
            } else {
                MapGestureCount = 0;
            }
        }
    }
    return false;
}

void MapGesture::hudMessage(HUD *hud)
{
    hud->toggleMap();
}

void MapGesture::draw(){}
void MapGesture::resetDraw() {}





