//
//  GLHelper.h
//  Skeleton
//
//  Created by Simon on 2015-01-21.
//  Copyright (c) 2015 Gabriele Gambotto. All rights reserved.
//

#ifndef __Skeleton__GLHelper__
#define __Skeleton__GLHelper__

#include <stdio.h>
#include <GLUT/GLUT.h>

class GLHelper
{
public:
    static GLuint LoadTexture( const char * filename, int width, int height );
    static void DrawTexture( GLuint texture, int width, int height, int x, int y, float opacity , bool flipx, bool flipy);
    static void DrawBlackBox(int width, int height, int y, int x);
};

#endif /* defined(__Skeleton__GLHelper__) */
