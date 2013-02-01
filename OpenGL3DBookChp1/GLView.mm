//
//  GLView.m
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 1/31/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//

#import "GLView.h"

@implementation GLView

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)drawView
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    [self.context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)super.layer;
        eaglLayer.opaque = YES;
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            return nil;
        }
        
        GLuint frameBuffer;
        GLuint renderBuffer;

        glGenFramebuffersOES(1, &frameBuffer);
        glGenRenderbuffersOES(1, &renderBuffer);

        glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderBuffer);

        [_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderBuffer);

        glViewport(0, 0, CGRectGetWidth(frame), CGRectGetHeight(frame));

        [self drawView];
    }
    return self;
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

@end
