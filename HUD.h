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


#define HUDHEIGHT 240
#define HUDWIDTH 854
#define LOADINGWH 178

class HUD
{
private:

    
    int currentMap;
    
    void drawCircle(float cx, float cy, float r, int num_segments);
        
    GLuint texture_map_1, texture_map_2, texture_loading,  texture_recognizing;

    
public:
    HUD();
    void draw();
    void drawMap();
    void drawRecognizing();
    void drawMessage(const char* string);
    void switchMap();
    
};

#endif /* defined(__Skeleton__HUD__) */
