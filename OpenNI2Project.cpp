// OpenNI2Project.cpp : Defines the entry point for the console application.
//


/*
 Input Experiment
 --------------------
 1. Butons
 - ENVELOPE OFF
 - GESTURES OFF
 - SPLITVIEW ON
 
 2. Gestures
 - ENVELOPE ON
 - GESTURES ON
 - SPLITVIEW ON
 
 
 Output Experiment
 --------------------
 1. Projector
 - ENVELOPE ON
 - GESTURES OFF
 - SPLITVIEW ON
 2. HUD
 - ENVELOPE OFF
 - GESTURES OFF
 - SPLIT SCREEN OFF
 
 
 Visibility Study
 --------------------
 - GESTURES ON
 - SPLITVIEW ON
 - ENVELOPE ON
 
 */


// EXPERIMENT TOGGLES
bool envelopeEnabled = false;
bool gesturesEnabled = false;
bool splitviewEnabled = false;



//DEBUG TOGGLES
#define FULLSCREEN 0
#define DEPTHCAMERA 0
#define DEBUG 0
#define ENABLEGPS 1
#define TESTGPS 1


// General headers
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <list>

// OpenNI2 headers
#include "OpenNI.h"
#include "NiTE.h"
using namespace openni;

#include "nmea/nmea.h"


// GLUT headers
#include <GLUT/GLUT.h>
#include <algorithm>

// Gestures
#include "IGesture.h"
#include "TurnLeftGesture.h"
#include "TurnRightGesture.h"
#include "StopGesture.h"
#include "RightStopGesture.h"
#include "HazardGesture.h"
#include "PassingGesture.h"


#include "HUD.h"


//16:10 displays
//int window_w = 854;
//int window_h = 534;

//16:9 displays
int window_w = 854;
int window_h = 480;

int camera_w = 640;
int camera_h = 480;

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
bool debugSkeleton = false;
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
HUD *hud;


//GPS and NMEA Lib
struct termios tio;
struct termios stdio;
struct termios old_stdio;
int tty_fd;
char c[2048];
int sizet;
nmeaINFO info;
nmeaPARSER parser;
nmeaPOS dpos;

float GPSSpeed = 0;
float GPSLatitude = 0;
float GPSLongitude = 0;


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
    
    if(key == 'r')
    {
        isUserDetected = !isUserDetected;
    }
    
    
    if(key == 'm')
    {
        hud->switchMap();
    }
    
    if(key == 's')
    {
        hud->switchView();
        envelopeEnabled = !envelopeEnabled;
    }
    
    if(key == ']')
    {
        GPSSpeed ++;
    }
    
    if(key == '[')
    {
        GPSSpeed --;
    }

    
}

void gl_IdleCallback()
{
    glutPostRedisplay();
}

void drawSkeleton(nite::Skeleton user_skel){
    
    glBindTexture(GL_TEXTURE_2D, 1);
    
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
                            (camera_w / (double)depthFrame.getWidth()),
                            (camera_h / (double)depthFrame.getHeight()));
    texture_x = (unsigned int)(camera_w -
                               (resizeFactor * depthFrame.getWidth())) / 2;
    texture_y = (unsigned int)(camera_h -
                               (resizeFactor * depthFrame.getHeight())) / 2;
    
    nite::UserMap usersMap = usersFrame.getUserMap();
    
    for	(unsigned int y = 0;
         y < (camera_h - 2 * texture_y); ++y)
    {
        OniRGB888Pixel* texturePixel = gl_texture +
        ((y + texture_y) * camera_w) + texture_x;
        for	(unsigned int x = 0;
             x < (camera_w - 2 * texture_x);
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
    
    // Create the OpenGL texture map
    glTexParameteri(GL_TEXTURE_2D,
                    0x8191, GL_TRUE); // 0x8191 = GL_GENERATE_MIPMAP
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 camera_w, camera_h,	0, GL_RGB,
                 GL_UNSIGNED_BYTE, gl_texture);
    
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, (float)camera_w, 0.0f);
    
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((float)camera_w,(float)camera_h, 0.0f);
    
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((float)camera_w, 0.0f, 0.0f);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 1);
}


void drawSafetyLines(float xOffset, float yOffset, float skew, bool mirrorDrawDirection)
{
    /* Left square */
    //save matrix
    glPushMatrix();
    
    //glRotated(100,135,30,0);
    
    float width = 376;
    float maxWidth = 376;
    float height = 30;
    
    width = GPSSpeed * 10 + 100;
    
    
    glBegin( GL_POLYGON );
    glColor3f(0, 1.0, 0);
    
    if(mirrorDrawDirection)
    {
        glVertex3f(maxWidth, yOffset, 0.0f);
        
        glColor3f(1.0, 0, 0);
        glVertex3f(maxWidth-width, yOffset - skew, 0.0f);
        
        
        glVertex3f(maxWidth-width, yOffset - skew + height, 0.0f);
        
        glColor3f(0, 1.0, 0);
        glVertex3f(maxWidth, height + yOffset, 0.0f);
    }else
    {
        glVertex3f(xOffset, yOffset, 0.0f);
        
        glColor3f(1.0, 0, 0);
        glVertex3f(xOffset+width, yOffset + skew, 0.0f);
        glVertex3f(xOffset+width, yOffset + skew + height, 0.0f);
        
        glColor3f(0, 1.0, 0);
        glVertex3f(xOffset, height + yOffset, 0.0f);
    }
    
    

    

    
    glEnd();
    glPopMatrix();
}

int frame=0;

void drawSafetyEnvelope()
{
    int animationOffset = frame * 4 % 120;
    
    for(int i=0; i<10; i++) {
        drawSafetyLines(0,i*60-animationOffset+20,-30.0f, true);
    }
    
    for(int i=0; i<10; i++) {
        drawSafetyLines(478,i*60-animationOffset+20,30.0f, false);
    }
    
    frame++;
    
    //Black box to cover up the lines
    glBegin( GL_POLYGON );
    glColor3f(0, 0, 0);
    glVertex3f(0.0f, 190.0f, 0.0f);
    glVertex3f(854, 190.0f, 0.0f);
    glVertex3f(854, 480.0f, 0.0f);
    glVertex3f(0.0f, 480.0f, 0.0f);
    glEnd();
    
}



void trace(const char *str, int str_size)
{
    //printf("Trace: ");
    //write(1, str, str_size);
    //printf("\n");
}
void error(const char *str, int str_size)
{
    printf("Error: ");
    write(1, str, str_size);
    printf("\n");
}



void gpsStartup()
{
    
    //NMEA Library setup
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
    
    
    //USB Serial Read
    tcgetattr(STDOUT_FILENO,&old_stdio);
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
    
    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;
    
    tty_fd=open("/dev/tty.usbserial", O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio,B9600);            // 9600 baud
    cfsetispeed(&tio,B9600);            // 9600 baud
    
    tcsetattr(tty_fd,TCSANOW,&tio);
    
    
    
}


/*
 * Convert a NMEA decimal-decimal degree value into degrees/minutes/seconds
 * First. convert the decimal-decimal value to a decimal:
 * 5144.3855 (ddmm.mmmm) = 51 44.3855 = 51 + 44.3855/60 = 51.7397583 degrees
 *
 * Then convert the decimal to degrees, minutes seconds:
 * 51 degress + .7397583 * 60 = 44.385498 = 44 minutes
 * .385498 = 23.1 seconds
 * Result: 51 44' 23.1"
 *
 * @return String value of the decimal degree, using the proper units
 */
double decimalToDecimalDegrees(double value) {
    double degValue = value / 100;
    int degrees = (int) degValue;
    double decMinutesSeconds = ((degValue - degrees)) / .60;
    double decimal = degrees +decMinutesSeconds;

    return decimal;
}


void gpsRead()
{
    // if new data is available on the serial port, print it out
    sizet =  (int)read(tty_fd,c,100);
    
    if(sizet > 0)
    {
        
        nmea_parse(&parser, &c[0], sizet, &info);
        nmea_info2pos(&info, &dpos);
        
        if(!TESTGPS)
        GPSSpeed = info.speed;
        
        GPSLongitude = decimalToDecimalDegrees(info.lon);
        GPSLatitude = decimalToDecimalDegrees(info.lat);

        
        
        /*printf(
               "Speed: %f, LonD: %f, , LonI: %f, Sig: %d, Fix: %d\n",
               info.speed, decimalToDecimalDegrees(info.lon), decimalToDecimalDegrees(info.lat), info.sig, info.fix
               );
        */
        
        
    }
}


void gpsShutdown()
{
    close(tty_fd);
    tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);
    nmea_parser_destroy(&parser);
}







void gl_DisplayCallback()
{
    
    if(ENABLEGPS)
    gpsRead();
    
    // Clear the OpenGL buffers
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 1);
    
    
    
    //START USER DETECT
    if (uTracker.isValid())
    {
        
        nite::UserTrackerFrameRef usersFrame;
        status = uTracker.readFrame(&usersFrame);
        if (status == nite::STATUS_OK && usersFrame.isValid())
        {
            gl_depthTextureSetup(usersFrame);
            
            if(debugSkeleton) {
                drawDepthTexture();
            }
            
            const nite::Array<nite::UserData>& users = usersFrame.getUsers();
            //Loop through all detected users
            for (int i = 0; i < users.getSize(); ++i)
            {
                if (users[i].isNew())
                {
                    uTracker.startSkeletonTracking(users[i].getId());
                    uTracker.setSkeletonSmoothingFactor(0.95f);
                    
                }
                
                nite::Skeleton user_skel = users[i].getSkeleton();
                if (user_skel.getState() == nite::SKELETON_TRACKED)
                {
                    
                    //User detected
                    isUserDetected = true;
                    
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
                    

                    
                    if(debugSkeleton) {
                        drawSkeleton(user_skel);
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
    //END USER DETECT
    
    //Draw Safety Envelope
    if(envelopeEnabled && activeGesture == NULL && splitviewEnabled)
    drawSafetyEnvelope();
    
    //Draw HUD
    if(TESTGPS){
        GPSLatitude = 57.6871977;
        GPSLongitude = 11.9930511;
    }
    
    hud->draw(isUserDetected  ||  !gesturesEnabled, GPSSpeed, GPSLatitude, GPSLongitude);
        
   

    
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
    glutGameModeString("1280x720:32@60");
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
    
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);// you enable blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Setup the OpenGL viewpoint
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, window_w, window_h, 0, -1.0, 1.0);
    
    glLineWidth(10.0);
    
    glBindTexture(GL_TEXTURE_2D, 1);
}


int main(int argc, char* argv[])
{
    
    
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
    isUserDetected = false;
    printf("\r\n---------------------- OpenGL -------------------------\r\n");
    printf("Initializing OpenGL ...\r\n");
    
    glutInit(&argc, (char**)argv);
    gl_Setup();
    
    if(gesturesEnabled)
    {
        //Add Gestures in order of priority
        gestures.push_back(new StopGesture());
        gestures.push_back(new RightStopGesture());
        //gestures.push_back(new HazardGesture());
        //gestures.push_back(new PassingGesture());
        gestures.push_back(new TurnLeftGesture());
        gestures.push_back(new TurnRightGesture());
        
        
    
    }
    
    
    hud = new HUD(window_w, window_h, splitviewEnabled);
    
    
    printf("Starting OpenGL rendering process ...\r\n");

    if(ENABLEGPS)
    gpsStartup();

    
    glutMainLoop();
    
    
    if(ENABLEGPS)
    gpsShutdown();
    
    
    
    
    
    return 0;
}
