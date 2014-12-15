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
    
}

void HUD::displayMessage(const char *string)
{
    
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(280.0f, 450.0f, 0.0f);
    
    int j = strlen( string );
    
    for( int i = 0; i < j; i++ ) {
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, string[i] );
    }
    
    //glutBitmapString(GLUT_BITMAP_HELVETICA_18, string);
    
}

void HUD::flashlight(bool isOn)
{
 
    glBegin( GL_POLYGON );
    glColor3f(1.0 , 1.0 , 1.0);
    glVertex3f(0.0f, 360.0f, 0.0f);
    glVertex3f(640.0f, 360.0f, 0.0f);
    glVertex3f(640.0f, 480.0f, 0.0f);
    glVertex3f(0.0f, 480.0f, 0.0f);
    
    glEnd();
    
}



