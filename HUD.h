//
//  HUD.h
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__HUD__
#define __Skeleton__HUD__

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // pulls in declaration for strlen.
#include <cmath>        // std::abs


#define GESTURETIMEOUT 3500
#define HUDHEIGHT 160
#define HUDWIDTH 640

class HUD
{
private:
    
    //Remember flashlight state
    bool isFlashlightOn;
    float flashLightToggleTime;
    
    //Remember flashlight state
    bool isMapOn;
    float mapToggleTime;
    
    
    void drawCircle(float cx, float cy, float r, int num_segments);
    
    GLuint LoadTexture( const char * filename,  int width, int height);

    
public:
    HUD();
    void draw();
    void drawFlashlight();
    void drawMap();
    void drawMessage(const char* string);
    void toggleFlashlight();
    void toggleMap();
    
};

#endif /* defined(__Skeleton__HUD__) */
