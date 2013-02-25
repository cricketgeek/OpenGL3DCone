//
//  GLView.m
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 1/31/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//
#import <OpenGLES/EAGLDrawable.h>
#import "GLView.h"
#import "mach/mach_time.h"
#import <OpenGLES/ES2/gl.h>

@implementation GLView

const bool ForceES1 = true;

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)didRotate:(NSNotification *)notification
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    _renderingEngine->OnRotate((DeviceOrientation)orientation);
    [self drawView:nil];
}

- (void)drawView:(CADisplayLink *)displayLink
{
    if (displayLink != nil) {
        float elapsedSeconds = displayLink.timestamp - _timestamp;
        _timestamp = displayLink.timestamp;
        _renderingEngine->UpdateAnimation(elapsedSeconds);
    }
    
    _renderingEngine->Render();
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)super.layer;
        eaglLayer.opaque = YES;
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        _context = [[EAGLContext alloc] initWithAPI:api];
        
        //_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
 
        if (!_context || ForceES1) {
            api = kEAGLRenderingAPIOpenGLES1;
        }
        
        
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            return nil;
        }
        
 
        if (api == kEAGLRenderingAPIOpenGLES1) {
            _renderingEngine = CreateRenderer1();
        }
        else
        {
            NSLog(@"using openGL ES 2.0");
            _renderingEngine = CreateRenderer2();
        }
        
//        _renderingEngine = CreateRenderer1();
        
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        _renderingEngine->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView:nil];

        _timestamp = CACurrentMediaTime();
        
        CADisplayLink *displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didRotate:) name:UIDeviceOrientationDidChangeNotification object:nil];
    }
    return self;
}


@end
