// OpenNI2Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// General headers
#include <stdio.h>
// OpenNI2 headers
#include "OpenNI.h"
#include "NiTE.h"
using namespace openni;
// GLUT headers
#include <GLUT/GLUT.h>
#include <algorithm>
int window_w = 640;
int window_h = 480;
OniRGB888Pixel* gl_texture;
nite::UserTracker uTracker;


//Counts the amount of frames that the user holds up the left arm
int leftGestureCount = 0;
//Right arm
int rightGestureCount = 0;

//blinkLeft: the gesture is recognized. blinkedLeft: the gesture function has been fired, false when function is finished
bool blinkLeft, blinkRight;

int gestureStartTime = 0;

bool showDebug = false;

#define GESTURE_HOLD_FRAMES_THRESHOLD 10
#define GESTURE_DELTA_Y -30

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
        //printf("esc key pressed");
		uTracker.destroy();
		nite::NiTE::shutdown();
		exit(0);
	}
    if (key == 68) {
        //printf("debug key pressed");
        showDebug = !showDebug;
    }
}

void gl_IdleCallback()
{
	glutPostRedisplay();
}

void blinkLeftEnd(int value) {
    blinkLeft = false;
}

void blinkRightEnd(int value) {
    blinkRight = false;
}

void gl_DisplayCallback()
{
	if (uTracker.isValid())
	{
		//Status status = STATUS_OK;
		nite::Status status = nite::STATUS_OK;
		nite::UserTrackerFrameRef usersFrame;
		status = uTracker.readFrame(&usersFrame);
		if (status == nite::STATUS_OK && usersFrame.isValid())
		{
			// Clear the OpenGL buffers
			glClear (
				GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
			// Setup the OpenGL viewpoint
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, window_w, window_h, 0, -1.0, 1.0);

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
			double resizeFactor = std::min(
				(window_w / (double)depthFrame.getWidth()),
				(window_h / (double)depthFrame.getHeight()));
			unsigned int texture_x = (unsigned int)(window_w - 
				(resizeFactor * depthFrame.getWidth())) / 2;
			unsigned int texture_y = (unsigned int)(window_h - 
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

            if(showDebug) {
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
                glVertex3f((float)window_w,
                    (float)window_h, 0.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f((float)window_w, 0.0f, 0.0f);
                glEnd();
            }
            
            
            /* Right arrow */
            if(blinkRight) {
                glBegin( GL_POLYGON );
                glColor3f(1, 1, 0);
                glVertex3f(640.0f, 245.0f, 0.0f);
                glVertex3f(540.0f, 150.0f, 0.0f);
                glVertex3f(540.0f, 350.0f, 0.0f);
                glEnd();
            }
            /* Left arrow */
            if(blinkLeft) {
                glBegin( GL_POLYGON );
                glColor3f(1, 1, 0);
                glVertex3f(100.0f, 245.0f, 0.0f);
                glVertex3f(0.0f, 150.0f, 0.0f);
                glVertex3f(0.0f, 350.0f, 0.0f);
                glEnd();
            }
			const nite::Array<nite::UserData>& users =
				usersFrame.getUsers();

			glBegin( GL_POINTS );
			glColor3f( 1.f, 0.f, 0.f );
			for (int i = 0; i < users.getSize(); ++i)
			{
				if (users[i].isNew())
				{
					uTracker.startSkeletonTracking(
						users[i].getId());
				}
				nite::Skeleton user_skel = users[i].getSkeleton();
				if (user_skel.getState() == 
					nite::SKELETON_TRACKED)
				{
                    if(showDebug) {
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
                    }
                    
                    
                    /** Detect Left arm gesture **/
                    float leftElbowX, leftElbowY, leftShoulderX, leftShoulderY;
                    status = uTracker.convertJointCoordinatesToDepth(
                                                                     user_skel.getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().x,
                                                                     user_skel.getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().y,
                                                                     user_skel.getJoint(nite::JointType::JOINT_LEFT_ELBOW).getPosition().z,
                                                                     &leftElbowX, &leftElbowY);
                    if(HandleStatus(status)) {
                        //printf("Left elbow: %f\n",leftElbowY);
                        
                        status = uTracker.convertJointCoordinatesToDepth(
                                                                         user_skel.getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().x,
                                                                         user_skel.getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().y,
                                                                         user_skel.getJoint(nite::JointType::JOINT_LEFT_SHOULDER).getPosition().z,
                                                                         &leftShoulderX, &leftShoulderY);
                        
                        if(HandleStatus(status)) {
                            //printf("Left shoulder: %f\n",leftShoulderY);
                            
                            float leftDeltaY = leftShoulderY-leftElbowY;
                            //printf("DeltaY: %f\n", leftDeltaY);
                            
                            if(leftDeltaY > GESTURE_DELTA_Y) {
                                leftGestureCount++;
                                
                                if(leftGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                                    blinkLeft = true;
                                    glutTimerFunc(2000,blinkLeftEnd,0);
                                }
                            } else {
                                leftGestureCount = 0;
                            }
                        }
                    }
                    /** Detect right arm gesture **/
                    float rightElbowX, rightElbowY, rightShoulderX, rightShoulderY;
                    status = uTracker.convertJointCoordinatesToDepth(
                                                                     user_skel.getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().x,
                                                                     user_skel.getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().y,
                                                                     user_skel.getJoint(nite::JointType::JOINT_RIGHT_ELBOW).getPosition().z,
                                                                     &rightElbowX, &rightElbowY);
                    if(HandleStatus(status)) {
                        //printf("Left elbow: %f\n",leftElbowY);
                        
                        status = uTracker.convertJointCoordinatesToDepth(
                                                                         user_skel.getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().x,
                                                                         user_skel.getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().y,
                                                                         user_skel.getJoint(nite::JointType::JOINT_RIGHT_SHOULDER).getPosition().z,
                                                                         &rightShoulderX, &rightShoulderY);
                        
                        if(HandleStatus(status)) {
                            //printf("Left shoulder: %f\n",leftShoulderY);
                            
                            float rightDeltaY = rightShoulderY-rightElbowY;
                            //printf("DeltaY: %f\n", leftDeltaY);
                            
                            if(rightDeltaY > GESTURE_DELTA_Y) {
                                rightGestureCount++;
                                
                                if(rightGestureCount > GESTURE_HOLD_FRAMES_THRESHOLD) {
                                    blinkRight = true;
                                    glutTimerFunc(2000,blinkRightEnd,0);
                                }
                            } else {
                                rightGestureCount = 0;
                            }
                        }
                    }
				}
			}
			glEnd();
			glColor3f( 1.f, 1.f, 1.f );
			glutSwapBuffers();
		}
	}
}

int main(int argc, char* argv[])
{
	printf("\r\n----------------- NiTE & User Tracker -------------------\r\n");
	nite::Status status = nite::STATUS_OK;
	printf("Initializing NiTE ...\r\n");
	status = nite::NiTE::initialize();
	if (!HandleStatus(status)) return 1;

	printf("Creating a user tracker object ...\r\n");
	status = uTracker.create();
	if (!HandleStatus(status)) return 1;
	
	printf("\r\n---------------------- OpenGL -------------------------\r\n");
	printf("Initializing OpenGL ...\r\n");
	gl_texture = (OniRGB888Pixel*)malloc(
		window_w * window_h * sizeof(OniRGB888Pixel));
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow ("OpenGL | OpenNI 2.x Skeleton");
	glPointSize(10.0);
	glutKeyboardFunc(gl_KeyboardCallback);
	glutDisplayFunc(gl_DisplayCallback);
	glutIdleFunc(gl_IdleCallback);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	printf("Starting OpenGL rendering process ...\r\n");
	glutMainLoop();

	return 0;
}

