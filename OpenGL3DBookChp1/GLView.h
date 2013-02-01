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
#import "IRenderingEngine.hpp"

@interface GLView : UIView
{
@private
    EAGLContext *_context;
    IRenderingEngine *_renderingEngine;
    float _timestamp;
}

- (void)drawView:(CADisplayLink *)displayLink;
- (void)didRotate:(NSNotification *)notification;


@end
