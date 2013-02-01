//
//  GLView.h
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 1/31/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface GLView : UIView

@property EAGLContext *context;

@end
