//
//  HUD.cpp
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "HUD.h"
#include <GLUT/GLUT.h>

//Constructor
HUD::HUD()
{
    isFlashlightOn = true;
    isUserDetected = false;
    flashLightToggleTime = glutGet(GLUT_ELAPSED_TIME);
}

void HUD::draw()
{
    this->drawFlashlight();
    this->drawUserStatus();
    
}

//Displays message in the HUD
void HUD::displayMessage(const char *string)
{
    
    glColor3f(0.0f, 0.3f, 1.0f);
    glRasterPos3f(280.0f, 450.0f, 0.0f);
    
    int j = strlen( string );
    
    for( int i = 0; i < j; i++ ) {
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, string[i] );
    }
    
}

void HUD::drawFlashlight()
{
 
    if(isFlashlightOn)
    {
        glBegin( GL_POLYGON );
        glColor3f(1.0 , 1.0 , 1.0);
        glVertex3f(0.0f, 320.0f, 0.0f);
        glVertex3f(640.0f, 320.0f, 0.0f);
        glVertex3f(640.0f, 480.0f, 0.0f);
        glVertex3f(0.0f, 480.0f, 0.0f);
        glEnd();
    }

    
}

void HUD::drawUserStatus()
{
    if(isUserDetected)
    {
        //Checkmark
        glBegin( GL_LINE_STRIP );
        glColor3f(0.0 , 1.0 , 0.0);
        glVertex3f(260.0f, 420.0f, 0.0f);
        glVertex3f(300.0f, 460.0f, 0.0f);
        glVertex3f(340.0f, 380.0f, 0.0f);
        glEnd();
    }
    else
    {
        //Circle
        this->drawCircle(320.0, 400.0, 60.0, 50);
    }
    
}

void HUD::toggleFlashlight(){
    
    //Flashlight is off - turn it on
    if(!isFlashlightOn)
    {
        //Don't turn on if it was turned off within the last 1 second
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - flashLightToggleTime;
        
        
        
        if(elapsedTime > FLASHLIGHTLOCKTIMEMS)
        {
            isFlashlightOn = !isFlashlightOn;
            flashLightToggleTime = glutGet(GLUT_ELAPSED_TIME);
        }
    }
     //Flashlight is on - turn it off
    else
    {
        //Don't turn off if it was turned on within the last 2 seconds
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - flashLightToggleTime;
        
        
        
        if(elapsedTime > FLASHLIGHTLOCKTIMEMS){
            
            
            //Turn on flashlight
            isFlashlightOn = !isFlashlightOn;
            flashLightToggleTime = glutGet(GLUT_ELAPSED_TIME);
        }
    }
    
    
}

void HUD::toggleUserStatus(bool status)
{
    isUserDetected = status;
}



void HUD::drawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0 , 0.3f , 1.0);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        
        float x = r * cosf(theta); //calculate the x component
        float y = r * sinf(theta); //calculate the y component
        
        glVertex3f(x + cx, y + cy, 0.0f);//output vertex
        
    } 
    glEnd(); 
}


