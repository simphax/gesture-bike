//
//  HUD.h
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__HUD__
#define __Skeleton__HUD__

#define FRAMETIME 10


#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // pulls in declaration for strlen.
#include <cmath>        // std::abs




class HUD
{
private:

    int HUDwidth;
    int HUDheight;
    
    bool splitView;
    
    int currentMap;
    
    int loadingRotationAngle;
    float opacityMultiplier;
    int opacityDirection;
    
    float lastFrameTime;
    
    //void drawCircle(float cx, float cy, float r, int num_segments);
    
    GLuint mapTextures [5];
    GLuint texture_loading,  texture_recognizing;

    //4 maps 4 values: West/East Longitude + North/South Latitude, Width, and Height of Map (in that order)
    float mapData [5][6];

    void drawCircle(float x, float y, float radius);
    void drawFlashligh();
    
    
    
public:
    HUD(int width, int height, bool splitView);
    void draw(bool userDetected, float speed, float latitude, float longitude);
    void drawMap();
    void drawStoppingLines(float speed);
    void drawGPSlocation(float latitude, float longitude, float speed);
    void drawRecognizing();
    void drawMessage(const char* string);
    void switchMap();
    void switchView();
    void animate();
    
};

#endif /* defined(__Skeleton__HUD__) */
