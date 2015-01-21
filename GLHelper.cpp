//
//  GLHelper.cpp
//  Skeleton
//
//  Created by Simon on 2015-01-21.
//  Copyright (c) 2015 Gabriele Gambotto. All rights reserved.
//

#include "GLHelper.h"
#include "stdlib.h"


//Simple Texture loading (BMP only)
//SRC: http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture

GLuint GLHelper::LoadTexture( const char * filename, int width, int height )
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

void GLHelper::DrawTexture( GLuint texture, int width, int height, int x, int y, float opacity , bool flipx, bool flipy)
{
    glColor3f(1.0 * opacity, 1.0 * opacity, 1.0 * opacity);
    
    glBindTexture (GL_TEXTURE_2D, texture);
    
    glBegin(GL_QUADS);
    
    if(flipx)
        glTexCoord2f(1.0f, 0.0f);
    else
        glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, 0.0f);
    
    
    if(flipx)
        glTexCoord2f(1.0f, 1.0f);
    else
        glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x, height + y, 0.0f);
    
    if(flipx)
        glTexCoord2f(0.0f, 1.0f);
    else
        glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + width, height + y, 0.0f);
    
    
    if(flipx)
        glTexCoord2f(0.0f, 0.0f);
    else
        glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + width, y, 0.0f);
    glEnd();
    
    
    glBindTexture (GL_TEXTURE_2D, 1);
    
}