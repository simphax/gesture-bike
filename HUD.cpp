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
#define MAP_IMG_WIDTH 480
#define MAP_IMG_HEIGHT 216
#define RECOGNIZING_IMG_WIDTH 244
#define RECOGNIZING_IMG_HEIGHT 25


//Constructor
HUD::HUD()
{
    currentMap = 1;
    loadingRotationAngle = 0;
    lastFrameTime = 0;
    
    //mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
    
    texture_map_1 = GLHelper::LoadTexture( "kuggen_route_01.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_map_2 = GLHelper::LoadTexture( "kuggen_route_02.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_loading = GLHelper::LoadTexture( "loading.bmp", LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT );
    texture_recognizing = GLHelper::LoadTexture( "recognizing.bmp", RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT );

}

void HUD::draw(bool userDetected)
{
    
    this->drawRecognizing();
    
    if(userDetected){
        this->drawMap();
    }
    
    
    
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
    if(currentMap == 1){
        GLHelper::DrawTexture(texture_map_1, MAP_IMG_WIDTH, MAP_IMG_HEIGHT, 200, 260, 1, false, false);
    }else{
        GLHelper::DrawTexture(texture_map_2, MAP_IMG_WIDTH, MAP_IMG_HEIGHT, 200, 260, 1, false, false);
    }

    
    
}



void HUD::drawRecognizing()
{
    
    
    
    //Rotation of loading graphic
    glPushMatrix(); //Save the current matrix.
    //Change the current matrix.
    glTranslatef(430, 350, 0);
    glRotatef(loadingRotationAngle, 0, 0, 1);
    
    GLHelper::DrawTexture(texture_loading, LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT, -90, -90, 1, false, false);
    
    //Reset the current matrix to the one that was saved.
    glPopMatrix();
    
    GLHelper::DrawTexture(texture_recognizing, RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT, 310, 440, 1, false, false);

    
    this->animate();
}



void HUD::switchMap()
{

    if (currentMap == 1){
        currentMap = 2;
    }else{
        currentMap = 1;
    }
    
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

void HUD::animate()
{
    if(lastFrameTime + 10 < glutGet(GLUT_ELAPSED_TIME) ) {
        
        
        loadingRotationAngle += 2;
        
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}
