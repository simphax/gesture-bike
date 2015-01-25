//
//  HUD.cpp
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "HUD.h"
#include "GLHelper.h"


//Constructor
HUD::HUD()
{
    currentMap = 1;

    //mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
    
    texture_map_1 = GLHelper::LoadTexture( "route_01.bmp", HUDWIDTH, HUDHEIGHT );
    texture_map_2 = GLHelper::LoadTexture( "route_01.bmp", HUDWIDTH, HUDHEIGHT );
    texture_loading = GLHelper::LoadTexture( "img_test.bmp", 256, 256 );
    texture_recognizing = GLHelper::LoadTexture( "recognizing.bmp", HUDWIDTH, HUDHEIGHT );

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

    GLHelper::DrawTexture(texture_map_1, HUDWIDTH, HUDHEIGHT, 0, 200, 1, false, false);
    
}



void HUD::drawRecognizing()
{
    
    GLHelper::DrawTexture(texture_loading, 256, 256, 0, 200, 1, false, false);
   
    //GLHelper::DrawTexture(texture_recognizing, HUDWIDTH, HUDHEIGHT, 0, 200, 1, false, false);
    
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
