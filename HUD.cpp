//
//  HUD.cpp
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "HUD.h"
#include "GLHelper.h"

#define LOADING_IMG_WIDTH 180
#define LOADING_IMG_HEIGHT 180
#define MAP_IMG_WIDTH 480
#define MAP_IMG_HEIGHT 216
#define MAP_IMG_HEIGHT_HUD 480
#define RECOGNIZING_IMG_WIDTH 244
#define RECOGNIZING_IMG_HEIGHT 25


//Constructor
HUD::HUD(int width, int height, bool splitView)
{
    this->HUDwidth = width;
    this->HUDheight = height;
    this->splitView = splitView;
    
    currentMap = 1;
    loadingRotationAngle = 0;
    lastFrameTime = 0;
    
    //mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
    
    texture_map_1 = GLHelper::LoadTexture( "route1_proj.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_map_2 = GLHelper::LoadTexture( "route2_proj.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT );
    texture_map_3 = GLHelper::LoadTexture( "route1_hud.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT_HUD );
    texture_map_4 = GLHelper::LoadTexture( "route2_hud.bmp", MAP_IMG_WIDTH, MAP_IMG_HEIGHT_HUD );
    texture_loading = GLHelper::LoadTexture( "loading.bmp", LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT );
    texture_recognizing = GLHelper::LoadTexture( "recognizing.bmp", RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT );

}

void HUD::draw(bool userDetected, float speed)
{
    
    
    if(userDetected){
        this->drawMap();
    }else{
        this->drawRecognizing();
    }
    
    this->drawStoppingLines(speed);
    

    //char message[25];
    
    // convert 123 to string [buf]
    //itoa(speed,message, 10);
    
    //this->drawMessage("30 km/h");
    
    
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


//Draw lines that grow according to the speed (taller line = longer stopping distance)
void HUD::drawStoppingLines(float speed)
{
    

    
    float width = 80;
    
    //Saturate at 20km/h
    
    float height = this->HUDheight - speed * this->HUDheight/20;
    
    if (height < 0 )
    {
        height = 0;
    }
    
    //Left
    glBegin( GL_POLYGON );
    glColor3f(0, 1.0, 0);
    glVertex3f(0, 480.0f, 0.0f);
    glVertex3f(0+width, 480.0f , 0.0f);
    glColor3f(1.0, 0, 0);
    glVertex3f(0+width, 480.0f  - this->HUDheight, 0.0f);
    glVertex3f(0, 480.0f - this->HUDheight , 0.0f);
    glEnd();
    
    glBegin( GL_POLYGON );
    glColor3f(0, 0.0, 0);
    glVertex3f(0, 480.0f  - this->HUDheight + height , 0.0f);
    glVertex3f(0+width, 480.0f  - this->HUDheight + height , 0.0f);
    glVertex3f(0+width, 480.0f - this->HUDheight, 0.0f);
    glVertex3f(0, 480.0f  - this->HUDheight , 0.0f);
    glEnd();
    
    
    //Right
    glBegin( GL_POLYGON );
    glColor3f(0, 1.0, 0);
    glVertex3f(this->HUDwidth, 480.0f, 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f , 0.0f);
    glColor3f(1.0, 0, 0);
    glVertex3f(this->HUDwidth-width, 480.0f  - this->HUDheight, 0.0f);
    glVertex3f(this->HUDwidth, 480.0f - this->HUDheight , 0.0f);
    glEnd();
    
    glBegin( GL_POLYGON );
    glColor3f(0, 0.0, 0);
    glVertex3f(this->HUDwidth, 480.0f  - this->HUDheight + height , 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f  - this->HUDheight + height , 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f - this->HUDheight, 0.0f);
    glVertex3f(this->HUDwidth, 480.0f  - this->HUDheight , 0.0f);
    glEnd();
    
}





void HUD::drawMap()
{
    
    switch(currentMap){
        case 2  :
           GLHelper::DrawTexture(texture_map_2, MAP_IMG_WIDTH, MAP_IMG_HEIGHT, 200, this->HUDheight - MAP_IMG_HEIGHT, 1, false, false);
            break;
        case 3  :
            GLHelper::DrawTexture(texture_map_3, MAP_IMG_WIDTH, MAP_IMG_HEIGHT_HUD, 200, this->HUDheight - MAP_IMG_HEIGHT_HUD, 1, false, false);
            break;
        case 4  :
            GLHelper::DrawTexture(texture_map_4, MAP_IMG_WIDTH, MAP_IMG_HEIGHT_HUD, 200, this->HUDheight - MAP_IMG_HEIGHT_HUD, 1, false, false);
            break;
        default : //1
            GLHelper::DrawTexture(texture_map_1, MAP_IMG_WIDTH, MAP_IMG_HEIGHT, 200, this->HUDheight - MAP_IMG_HEIGHT, 1, false, false);
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

    currentMap ++;
    
    if (currentMap > 4){
        currentMap = 1;
    }
    
}

void HUD::switchView()
{
    splitView = !splitView;
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
