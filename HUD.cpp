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
    
    currentMap = 3;
    loadingRotationAngle = 0;
    radiusMultiplier = 1;
    lastFrameTime = 0;


    //route_1_proj
    mapData[0][0] = 11.9641433; //West Longitude
    mapData[0][1] = 11.9710527; //East Longitude
    mapData[0][2] = 57.7010824; //North Latitude
    mapData[0][3] = 57.6973903; //South Latitude
    mapData[0][4] = 220; //Width
    mapData[0][5] = 220; //Height
    
    
    //route_2_proj
    mapData[1][0] = 11.9674156; //West Longitude
    mapData[1][1] = 11.9737778; //East Longitude
    mapData[1][2] = 57.7019137; //North Latitude
    mapData[1][3] = 57.6985083; //South Latitude
    mapData[1][4] = 220; //Width
    mapData[1][5] = 220; //Height
    
    //route_1_hud
    mapData[2][0] = 11.9641433; //West Longitude
    mapData[2][1] = 11.9710527; //East Longitude
    mapData[2][2] = 57.7010824; //North Latitude
    mapData[2][3] = 57.6973903; //South Latitude
    mapData[2][4] = 480; //Width
    mapData[2][5] = 480; //Height
    
    //route_2_hud
    mapData[3][0] = 11.9674156; //West Longitude
    mapData[3][1] = 11.9737778; //East Longitude
    mapData[3][2] = 57.7019137; //North Latitude
    mapData[3][3] = 57.6985083; //South Latitude
    mapData[3][4] = 480; //Width
    mapData[3][5] = 480; //Height
    
    
    //testmap
    mapData[4][0] = 11.9915276; //West Longitude
    mapData[4][1] = 11.9967204; //East Longitude
    mapData[4][2] = 57.6886429; //North Latitude
    mapData[4][3] = 57.6857983; //South Latitude
    mapData[4][4] = 480; //Width
    mapData[4][5] = 480; //Height
    
    //Load Map textures
    mapTextures[0] = GLHelper::LoadTexture( "route1_proj.bmp", mapData[0][4], mapData[0][5] );
    mapTextures[1] = GLHelper::LoadTexture( "route2_proj.bmp", mapData[1][4], mapData[1][5] );
    mapTextures[2] = GLHelper::LoadTexture( "route1_hud.bmp", mapData[2][4], mapData[2][5] );
    mapTextures[3] = GLHelper::LoadTexture( "route2_hud.bmp", mapData[3][4], mapData[3][5] );
    mapTextures[4] = GLHelper::LoadTexture( "route_test.bmp", mapData[4][4], mapData[4][5] );

    texture_loading = GLHelper::LoadTexture( "loading.bmp", LOADING_IMG_WIDTH, LOADING_IMG_HEIGHT );
    texture_recognizing = GLHelper::LoadTexture( "recognizing.bmp", RECOGNIZING_IMG_WIDTH, RECOGNIZING_IMG_HEIGHT );
    
}

void HUD::draw(bool userDetected, float speed, float latitude, float longitude)
{
    
    
    
    
    if(userDetected){
        this->drawFlashligh();
        this->drawMap();
        this->drawGPSlocation(latitude, longitude);
    }else{
        this->drawRecognizing();
    }
    
    this->drawStoppingLines(speed);
    
    
    

    //char message[25];
    
    // convert 123 to string [buf]
    //itoa(speed,message, 10);
    
    //this->drawMessage("30 km/h");
    
    this->animate();
    
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

//Displays message in the HUD
void HUD::drawGPSlocation(float latitude, float longitude)
{
    
    //Testing
    //latitude = 57.6871977;
    //longitude = 11.9930511;
    
    //Get x and y from lon and lat
    int x = (int)((mapData[currentMap][1] - longitude) / (mapData[currentMap][1] - mapData[currentMap][0]) * mapData[currentMap][4]);
    int y = (int)((mapData[currentMap][2] - latitude) / (mapData[currentMap][2] - mapData[currentMap][3]) * mapData[currentMap][5]);

    //printf("X position: %d, Y position: %d \n",x, y);
    
    //Adjust for coordinate system
    int offset = (int)(this->HUDwidth - mapData[currentMap][4])/2;
    x = mapData[currentMap][4] - x + offset;
    y = y + this->HUDheight -  mapData[currentMap][5];
    
    float radius = mapData[currentMap][4] / 40;
    
    //printf("X position: %f: \n",radiusMultiplier);

    this->drawCircle(x, y,radius);
    this->drawCircle(x, y,radius/2);
    
    

}


void HUD::drawCircle(float cx, float cy, float r) {
    glBegin(GL_LINE_LOOP);
    glColor3f(0 , 0 , 1.0);
    for(int ii = 0; ii < 10; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(10);//get the current angle
        
        float x = r * cosf(theta); //calculate the x component
        float y = r * sinf(theta); //calculate the y component
        
        glVertex3f(x + cx, y + cy, 0.0f);//output vertex
        
    }
    glEnd();
}


//Draw lines that grow according to the speed (taller line = longer stopping distance)
void HUD::drawStoppingLines(float speed)
{
    
    float heightStart = this->HUDheight;
    
    if(splitView){
        heightStart = this->HUDheight/2;
    }
    
    float width = 80;
    
    //Saturate at 20km/h
    
    float height = heightStart - speed * heightStart/20;
    
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
    glVertex3f(0+width, 480.0f  - heightStart, 0.0f);
    glVertex3f(0, 480.0f - heightStart , 0.0f);
    glEnd();
    
    glBegin( GL_POLYGON );
    glColor3f(0, 0.0, 0);
    glVertex3f(0, 480.0f  - heightStart + height , 0.0f);
    glVertex3f(0+width, 480.0f  - heightStart + height , 0.0f);
    glVertex3f(0+width, 480.0f - heightStart, 0.0f);
    glVertex3f(0, 480.0f  - heightStart , 0.0f);
    glEnd();
    
    
    //Right
    glBegin( GL_POLYGON );
    glColor3f(0, 1.0, 0);
    glVertex3f(this->HUDwidth, 480.0f, 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f , 0.0f);
    glColor3f(1.0, 0, 0);
    glVertex3f(this->HUDwidth-width, 480.0f  - heightStart, 0.0f);
    glVertex3f(this->HUDwidth, 480.0f - heightStart , 0.0f);
    glEnd();
    
    glBegin( GL_POLYGON );
    glColor3f(0, 0.0, 0);
    glVertex3f(this->HUDwidth, 480.0f  - heightStart + height , 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f  - heightStart + height , 0.0f);
    glVertex3f(this->HUDwidth-width, 480.0f - heightStart, 0.0f);
    glVertex3f(this->HUDwidth, 480.0f  - heightStart , 0.0f);
    glEnd();
    
}





void HUD::drawMap()
{
    
    //Center map horizontally
    int offset = (int)(this->HUDwidth - mapData[currentMap][4])/2;
    
    //Draw current map
    GLHelper::DrawTexture(mapTextures[currentMap], mapData[currentMap][4], mapData[currentMap][5], offset, this->HUDheight - mapData[currentMap][5], 1, false, false);
    

    
}

void HUD::drawFlashligh()
{
    
    //Draw flashlight
    glBegin( GL_POLYGON );
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(187, this->HUDheight , 0.0f);
    glVertex3f(187+480, this->HUDheight , 0.0f);
    glVertex3f(187+480, this->HUDheight - 220, 0.0f);
    glVertex3f(187,  this->HUDheight-220 , 0.0f);
    glEnd();

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


}



void HUD::switchMap()
{

    currentMap ++;
    
    if (currentMap > 4){
        currentMap = 0;
    }
    
}

void HUD::switchView()
{
    splitView = !splitView;
}



void HUD::animate()
{
     if(lastFrameTime + FRAMETIME < glutGet(GLUT_ELAPSED_TIME) ) {
        
        
        loadingRotationAngle += 2;
         
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    }
}
