//
//  HUD.cpp
//  Skeleton
//
//  Created by Velko Vechev on 2014-12-14.
//  Copyright (c) 2014 Gabriele Gambotto. All rights reserved.
//

#include "HUD.h"


//Constructor
HUD::HUD()
{
    isFlashlightOn = true;
    isMapOn = true;
    flashLightToggleTime = glutGet(GLUT_ELAPSED_TIME);
    mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
}

void HUD::draw()
{
    this->drawFlashlight();
    this->drawMap();
    
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


void HUD::drawFlashlight()
{
 
    if(isFlashlightOn)
    {
        glBegin( GL_POLYGON );
        glColor3f(1.0 , 1.0 , 1.0);
        glVertex3f(0.0f, 480.0f - (float)HUDHEIGHT, 0.0f);
        glVertex3f(640.0f, 480.0f - (float)HUDHEIGHT, 0.0f);
        glVertex3f(640.0f, 480.0f, 0.0f);
        glVertex3f(0.0f, 480.0f, 0.0f);
        glEnd();
    }

    
}

void HUD::drawMap()
{
    glEnable(GL_TEXTURE_2D);
    
    GLuint texture;
    texture= LoadTexture( "map_lindholmen.bmp", HUDWIDTH, HUDHEIGHT );
    

    glColor3f(1.0, 1.0, 1.0);
    
    glBindTexture (GL_TEXTURE_2D, texture);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 480.0f, 0.0f);
   
    
    glTexCoord2f(0.0f, 1.0f);
     glVertex3f(0.0f, 480.0f - (float)HUDHEIGHT, 0.0f);
    
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(640.0f, 480.0f - (float)HUDHEIGHT, 0.0f);
    
    
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(640.0f,480.0f, 0.0f);
    glEnd();
    
    
    
    glDisable(GL_TEXTURE_2D);
    
    this->drawCircle(320.0, 400.0, 40.0, 50);
    
}


void HUD::toggleFlashlight(){
    
    float elapsedTime = 0;
    
    //Flashlight is off - turn it on
    if(!isFlashlightOn)
    {
        //Don't turn on if it was turned off within the last 1 second
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - flashLightToggleTime;
        
        if(elapsedTime > GESTURETIMEOUT)
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

        if(elapsedTime > GESTURETIMEOUT){

            //Turn on flashlight
            isFlashlightOn = !isFlashlightOn;
            flashLightToggleTime = glutGet(GLUT_ELAPSED_TIME);
        }
    }
}

void HUD::toggleMap(){
    
    float elapsedTime = 0;
    
    //Flashlight is off - turn it on
    if(!isFlashlightOn)
    {
        //Don't turn on if it was turned off within the last 1 second
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - mapToggleTime;
        
        if(elapsedTime > GESTURETIMEOUT)
        {
            isMapOn = !isMapOn;
            mapToggleTime= glutGet(GLUT_ELAPSED_TIME);
        }
    }
    //Flashlight is on - turn it off
    else
    {
        //Don't turn off if it was turned on within the last 2 seconds
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - mapToggleTime;
        
        if(elapsedTime > GESTURETIMEOUT){
            
            //Turn on flashlight
            isMapOn = !isMapOn;
            mapToggleTime = glutGet(GLUT_ELAPSED_TIME);
        }
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


//Simple Texture loading (BMP only)
//SRC: http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture

GLuint HUD::LoadTexture( const char * filename, int width, int height )
{
    
    
    GLuint texture;
    
    unsigned char * data;
    
    FILE * file;
    
    file = fopen( filename, "rb" );
    
    
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );
    
    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        
        data[index] = R;
        data[index+2] = B;
        
    }
    
    
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );
    
    return texture;
}