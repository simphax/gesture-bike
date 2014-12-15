//
//  HUD.h
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__HUD__
#define __Skeleton__HUD__

#include <stdio.h>
#include <string.h> // pulls in declaration for strlen.

#define FLASHLIGHTLOCKTIMEMS 5000

class HUD
{
private:
    
    //Remember flashlight state
    bool isFlashlightOn;
    float flashLightToggleTime;
    float elapsedTime;
    
public:
    HUD();
    void draw();
    void drawFlashlight();
    void displayMessage(const char* string);
    void toggleFlashlight();
    
};

#endif /* defined(__Skeleton__HUD__) */
