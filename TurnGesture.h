//
//  TurnGesture.h
//  Skeleton
//
//  Created by Simon on 2014-12-15.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__TurnGesture__
#define __Skeleton__TurnGesture__

#include <stdio.h>
#include "IGesture.h"
#include "CircularBuffer.h"
#include "NiTE.h"

class TurnGesture : public IGesture
{
private:
    //Counts the amount of frames that the user holds up the left arm
    int handGestureCount = 0;
    int elbowGestureCount = 0;
    
    CircularBuffer *elbowDeltaYBuffer;
    CircularBuffer *handDeltaYBuffer;
    
    nite::JointType handJoint, elbowJoint, shoulderJoint;
    
public:
    TurnGesture(nite::JointType handJoint, nite::JointType elbowJoint, nite::JointType shoulderJoint);
    bool gestureDetect(nite::Skeleton *skeleton, nite::UserTracker *userTracker);
};

#endif /* defined(__Skeleton__TurnGesture__) */
