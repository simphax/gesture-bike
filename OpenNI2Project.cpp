// OpenNI2Project.cpp : Defines the entry point for the console application.
//

#define FULLSCREEN 0
#define DEBUG 0
#define DEPTHCAMERA 1

#include "stdafx.h"
// General headers
#include <stdio.h>
#include <list>

// OpenNI2 headers
#include "OpenNI.h"
#include "NiTE.h"
using namespace openni;

// GLUT headers
#include <GLUT/GLUT.h>
#include <algorithm>

// Gestures
#include "IGesture.h"
#include "TurnLeftGesture.h"
#include "TurnRightGesture.h"
#include "StopGesture.h"
#include "RightStopGesture.h"
#include "FlashlightGesture.h"
#include "MapGesture.h"


#include "HUD.h"

int window_w = 640;
int window_h = 480;

OniRGB888Pixel* gl_texture;

//NiTE vars
nite::UserTracker uTracker;
nite::Status status;


//Depth Map Vars
double resizeFactor;
unsigned int texture_x;
unsigned int texture_y;

long double gestureStartTime = 0;

//Debug help
#if DEBUG
bool debugSkeleton = true;
bool debugGestures = false;
bool debugGrid = false;
#else
bool debugSkeleton = false;
bool debugGestures = false;
bool debugGrid = false;
#endif

//User Detected
bool isUserDetected = false;

//Time to consider a gesture active
#define GESTURE_DRAW_TIME 3

//List of all available gestures
std::list<IGesture*> gestures;

//The last detected gesture
IGesture *activeGesture;

//Heads up display class
HUD *hud = new HUD();


char ReadLastCharOfLine()
{
	int newChar = 0;
	int lastChar;
	fflush(stdout);
	do 
	{
		lastChar = newChar;
		newChar = getchar();
	}
	while ((newChar != '\n') 
		&& (newChar != EOF));
	return (char)lastChar;
}

bool HandleStatus(nite::Status status)
{
	if (status == nite::STATUS_OK)
		return true;
	printf("ERROR: #%d, %s", status,
		OpenNI::getExtendedError());
	ReadLastCharOfLine();
	return false;
}

void gl_KeyboardCallback(unsigned char key, int x, int y)
{
	if (key == 27) // ESC Key
	{
		uTracker.destroy();
		nite::NiTE::shutdown();
		exit(0);
	}
    if (key == 'd')
    {
        debugSkeleton = !debugSkeleton;
    }
    if (key == 'g')
    {
        for(IGesture *gesture : gestures)
        {
            gesture->resetDraw();
        }
        
        debugGestures = !debugGestures;
    }
    if(key == 'h')
    {
        debugGrid = !debugGrid;
    }
    
    if(key == 'f')
    {
        hud->toggleFlashlight();
    }
    
    if(key == 'm')
    {
        hud->toggleMap();
    }
    
}

void gl_IdleCallback()
{
	glutPostRedisplay();
}

void drawSkeleton(nite::Skeleton user_skel){
    
    glBegin( GL_POINTS );
    glColor3f( 1.f, 0.f, 0.f );
    
    //Draw all joints
    for (int joint_Id = 0; joint_Id < 15;
         ++joint_Id)
    {
        float posX, posY;
        status =
        uTracker.convertJointCoordinatesToDepth(
                                                user_skel.getJoint((nite::JointType)
                                                                   joint_Id).getPosition().x,
                                                user_skel.getJoint((nite::JointType)
                                                                   joint_Id).getPosition().y,
                                                user_skel.getJoint((nite::JointType)
                                                                   joint_Id).getPosition().z,
                                                &posX, &posY);
        if (HandleStatus(status)){
            glVertex2f(
                       (posX * resizeFactor) + texture_x,
                       (posY * resizeFactor) + texture_y);
        }
    }
    glEnd();
    
}


//Creates an OpenGL texture from the depth data
void gl_depthTextureSetup(nite::UserTrackerFrameRef &usersFrame)
{
    // UPDATING TEXTURE (DEPTH 1MM TO RGB888)
    VideoFrameRef depthFrame = usersFrame.getDepthFrame();
    
    int depthHistogram[65536];
    int numberOfPoints = 0;
    memset(depthHistogram, 0,
           sizeof(depthHistogram));
    for	(int y = 0;
         y < depthFrame.getHeight(); ++y)
    {
        DepthPixel* depthCell = (DepthPixel*)(
                                              (char*)depthFrame.getData() +
                                              (y * depthFrame.getStrideInBytes())
                                              );
        for	(int x = 0; x < depthFrame.getWidth();
             ++x, ++depthCell)
        {
            if (*depthCell != 0)
            {
                depthHistogram[*depthCell]++;
                numberOfPoints++;
            }
        }
    }
    
    for (int nIndex=1;
         nIndex < sizeof(depthHistogram) / sizeof(int);
         nIndex++)
    {
        depthHistogram[nIndex] +=
        depthHistogram[nIndex-1];
    }
    
    int colors[] = {16777215,
        14565387, 32255, 7996159, 16530175, 8373026, 14590399, 7062435, 13951499, 55807};
    resizeFactor = std::min(
                            (window_w / (double)depthFrame.getWidth()),
                            (window_h / (double)depthFrame.getHeight()));
    texture_x = (unsigned int)(window_w -
                               (resizeFactor * depthFrame.getWidth())) / 2;
    texture_y = (unsigned int)(window_h -
                               (resizeFactor * depthFrame.getHeight())) / 2;
    
    nite::UserMap usersMap = usersFrame.getUserMap();
    
    for	(unsigned int y = 0;
         y < (window_h - 2 * texture_y); ++y)
    {
        OniRGB888Pixel* texturePixel = gl_texture +
        ((y + texture_y) * window_w) + texture_x;
        for	(unsigned int x = 0;
             x < (window_w - 2 * texture_x);
             ++x, ++texturePixel)
        {
            DepthPixel* depthPixel =
            (DepthPixel*)(
                          (char*)depthFrame.getData() +
                          ((int)(y / resizeFactor) *
                           depthFrame.getStrideInBytes())
                          ) +	(int)(x / resizeFactor);
            nite::UserId* userPixel =
            (nite::UserId*)(
                            (char*)usersMap.getPixels() +
                            ((int)(y / resizeFactor) *
                             usersMap.getStride())
                            ) +	(int)(x / resizeFactor);
            if (*depthPixel != 0)
            {
                float depthValue = (1 - ((float)depthHistogram[*depthPixel]  / numberOfPoints));
                int userColor = colors[(int)*userPixel % 10];
                texturePixel->b = ((userColor / 65536) % 256) * depthValue;
                texturePixel->g = ((userColor / 256) % 256) * depthValue;
                texturePixel->r = ((userColor / 1) % 256) * depthValue;
            }
            else
            {
                texturePixel->b = 0;
                texturePixel->g = 0;
                texturePixel->r = 0;
            }
        }
    }
}


//Draw the depth texture (map the texture corners to window corners)
void drawDepthTexture()
{
    glEnable(GL_TEXTURE_2D);
    
    // Create the OpenGL texture map
    glTexParameteri(GL_TEXTURE_2D,
                    0x8191, GL_TRUE); // 0x8191 = GL_GENERATE_MIPMAP
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 window_w, window_h,	0, GL_RGB,
                 GL_UNSIGNED_BYTE, gl_texture);
    
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, (float)window_h, 0.0f);
    
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((float)window_w,(float)window_h, 0.0f);
    
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((float)window_w, 0.0f, 0.0f);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


void drawAwarenessMarkers()
{
    
    if (!isUserDetected) return;
    
    float cx = 110;
    float cy = 110;
    float r = 100;
    int num_segments = 50;
    
    glColor3f(0.0, 1.0, 0.0 );
    
    //Left Half Circle
    
    for(int circles = 0; circles < 3; circles++)
    {
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0, 1.0, 0.0 );
        for(int ii = 25; ii < num_segments; ii++)
        {

            //Radius gets smaller with each circle
            float iR = r - circles * 20;
            
            float theta = 1.8f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            
            float x = iR * cosf(theta); //calculate the x component
            float y = iR * sinf(theta); //calculate the y component
            
            glVertex3f(x + cx, y + cy, 0.0f);//output vertex
            
        }
        glEnd();

    }
    
        
    cx = window_w - cx;
    
    for(int circles = 0; circles < 3; circles++)
    {
        
        //Right Half Circle
        glBegin(GL_LINE_STRIP);
        
        for(int ii = 22; ii < num_segments; ii++)
        {
            //Radius gets smaller with each circle
            float iR = r - circles * 20;
            
            float theta = -1.7f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            
            float x = iR * sinf(theta); //calculate the x component
            float y = iR * cosf(theta); //calculate the y component
            
            glVertex3f(x + cx, y + cy, 0.0f);//output vertex
            
        }
        glEnd();
    }
}


void gl_DisplayCallback()
{
    // Clear the OpenGL buffers
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    

	if (!uTracker.isValid())
    {
        //For debug purposes
        //Draw HUD
        hud->draw();
        //Draw Awareness Markers
        drawAwarenessMarkers();

    }
    else
	{
        
		nite::UserTrackerFrameRef usersFrame;
		status = uTracker.readFrame(&usersFrame);
		if (status == nite::STATUS_OK && usersFrame.isValid())
		{
			
            gl_depthTextureSetup(usersFrame);

            if(debugSkeleton) {
                drawDepthTexture();
            }
            
            //Draw HUD
            hud->draw();
            //Draw Awareness Markers
            drawAwarenessMarkers();
            
			const nite::Array<nite::UserData>& users = usersFrame.getUsers();

            //Loop through all detected users
			for (int i = 0; i < users.getSize(); ++i)
			{
				if (users[i].isNew())
				{
					uTracker.startSkeletonTracking(users[i].getId());
				}
				nite::Skeleton user_skel = users[i].getSkeleton();
				if (user_skel.getState() == nite::SKELETON_TRACKED)
				{
                    //First time user is detected
                    if(!isUserDetected)
                    {
                        hud->toggleFlashlight();
                    }
                    
                    //User detected
                    isUserDetected = true;

                    
                    
                    //showDetectionMessage();
                    
                    if(debugSkeleton) {
                        drawSkeleton(user_skel);
                    }
                    
                    //Loop through all available gestures
                    for(IGesture *gesture : gestures)
                    {
                        
                        if(gesture->gestureDetect(&user_skel, &uTracker)) {
                            
                            //Reset draw parameters when switching gestures
                            if(gesture != activeGesture){
                                gesture->resetDraw();
                            }
                            
                            //Activate gesture
                            activeGesture = gesture;
                            gestureStartTime = time(0);
                            
                            break;

                        }
                    }
                    
                    //Show the graphics for 2 seconds
                    if(time(0) - gestureStartTime > GESTURE_DRAW_TIME) {
                        activeGesture = NULL;
                    }
                    
                    //Only draw graphics for the last detected gesture
                    if(activeGesture) {
                        activeGesture->draw();
                        activeGesture->hudMessage(hud);
                    }
                    
                    break;
                    
				}
                else
                {
                    isUserDetected = false;
                }
            }
            
        }
        

	}
    

    
    
    if(debugGestures){
        for(IGesture *gesture : gestures)
        {
            gesture->draw();
        }
    }
    
    if(debugGrid) {
        
        glLineWidth(1.0);
        
        glColor3f( 1.f, 1.f, 1.f );
        
        for(int x = 0; x < window_w; x += 10){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x,0,0);
            glVertex3f(x,window_h,0);
            glEnd();
        };
        
        for(int y = 0; y < window_h; y += 10){
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,y,0);
            glVertex3f(window_w,y,0);
            glEnd();
        };
        
        glLineWidth(10.0);
        
    }
    
    glColor3f( 1.f, 1.f, 1.f );
    glutSwapBuffers();

}






void gl_Setup(void) {
    
    //Allocate memory for depth sensor texture
    gl_texture = (OniRGB888Pixel*)malloc(window_w * window_h * sizeof(OniRGB888Pixel));
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
 
    #if FULLSCREEN
        glutGameModeString("640x480:32@60");
        glutEnterGameMode();
    #else
        glutInitWindowSize(window_w, window_h);
        glutCreateWindow ("Gesture Bike App");
    #endif
    
    
    //Skeleton tracking point sizes
    glPointSize(10.0);
    glutKeyboardFunc(gl_KeyboardCallback);
    glutDisplayFunc(gl_DisplayCallback);
    glutIdleFunc(gl_IdleCallback);
    
    //Z object tracking. We are not usign this
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    
    // Setup the OpenGL viewpoint
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, window_w, window_h, 0, -1.0, 1.0);
    
    glLineWidth(10.0);
}


int main(int argc, char* argv[])
{
    //Add Gestures in order of priority
    gestures.push_back(new StopGesture());
    gestures.push_back(new RightStopGesture());
    gestures.push_back(new FlashlightGesture());
    gestures.push_back(new MapGesture());
    gestures.push_back(new TurnLeftGesture());
    gestures.push_back(new TurnRightGesture());


    if(DEPTHCAMERA)
    {
        printf("\r\n----------------- NiTE & User Tracker -------------------\r\n");
        nite::Status status = nite::STATUS_OK;
        printf("Initializing NiTE ...\r\n");
        
        
        status = nite::NiTE::initialize();
        while (!HandleStatus(status)){
            sleep(1);
            status = nite::NiTE::initialize();

        }
        
        printf("Creating a user tracker object ...\r\n");
        status = uTracker.create();
        while (!HandleStatus(status)){
            sleep(1);
            status = uTracker.create();
        }
    }
	
	
	printf("\r\n---------------------- OpenGL -------------------------\r\n");
	printf("Initializing OpenGL ...\r\n");
    
    glutInit(&argc, (char**)argv);
    gl_Setup();
    
	printf("Starting OpenGL rendering process ...\r\n");
	glutMainLoop();

	return 0;
}

