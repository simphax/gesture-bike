//
//  HUD.cpp
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "HUD.h"
#include "GLHelper.h"

#define HUD_HEIGHT 240
#define HUD_WIDTH 854
#define LOADING_IMG_WIDTH 180
#define LOADING_IMG_HEIGHT 180
#define MAP_IMG_WIDTH 472
#define MAP_IMG_HEIGHT 220
#define RECOGNIZING_IMG_WIDTH 244
#define RECOGNIZING_IMG_HEIGHT 25


//Constructor
HUD::HUD()
{
    currentMap = 1;

    //mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
    
    texture_map_1 = GLHelper::LoadTexture( "route_01.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_map_2 = GLHelper::LoadTexture( "route_01.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_loading = GLHelper::LoadTexture( "loading.bmp", LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT );
    texture_recognizing = GLHelper::LoadTexture( "recognizing.bmp", RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT );

}

void HUD::draw()
{
    this->drawRecognizing();
    //this->drawMap();
    
}

//Displays message in the HUD
void HUD::drawMessage(const char *string)
{
    
    glColor3f(0.0f, 0.3f, 1.0f);
    glRasterPos3f(280.0f, 450.0f, 0.0f);
    
    int j = strlen( string );
    
    for( int i = 0; i < j; i++ ) {
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, string[i] );
    }
    
}




void HUD::drawMap()
{

    GLHelper::DrawTexture(texture_map_1, MAP_IMG_WIDTH, MAP_IMG_HEIGHT, 0, 200, 1, false, false);
    
}



void HUD::drawRecognizing()
{
    
    GLHelper::DrawTexture(texture_loading, LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT, 0, 0, 1, false, false);
    GLHelper::DrawTexture(texture_recognizing, RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT, 0, 200, 1, false, false);
    
}



void HUD::switchMap(){

        //Todo - switch maps on key press or re-regonition
}



void HUD::drawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0 , 0.0f , 1.0);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        
        float x = r * cosf(theta); //calculate the x component
        float y = r * sinf(theta); //calculate the y component
        
        glVertex3f(x + cx, y + cy, 0.0f);//output vertex
        
    } 
    glEnd(); 
}
