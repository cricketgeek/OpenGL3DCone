//
//  RenderingEngine1.cpp
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 2/2/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//


#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "IRenderingEngine.hpp"

static const float RevolutionsPerSecond = 1;

class RenderingEngine1 : public IRenderingEngine
{
public:
    RenderingEngine1();
    void Initialize(int width, int height);
    void Render() const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation newOrientation);
private:
    float  RotationDirection() const;
    float _desiredAngle;
    float  _currentAngle;
    GLuint _frameBuffer;
    GLuint _renderBuffer;
};

IRenderingEngine *CreateRenderer1()
{
    return new RenderingEngine1();
}

struct Vertex {
    float Position[2];
    float Color[4];
};

const Vertex Vertices[] = {
    {{-0.5f, -0.866f}, {1, 1, 0.5f, 1}},
    {{0.5f, -0.866f},  {1, 1, 0.5f, 1}},
    {{0, 1},           {1, 1, 0.5f, 1}},
    {{-0.5f, -0.866f}, {0.5f, 0.5f, 0.5f}},
    {{0.5f, -0.866f},  {0.5f, 0.5f, 0.5f}},
    {{0, -0.4f},       {0.5f, 0.5f, 0.5f}},
};

RenderingEngine1::RenderingEngine1()
{
    glGenRenderbuffersOES(1, &_renderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderBuffer);
}

void RenderingEngine1::Initialize(int width, int height)
{
    glGenFramebuffersOES(1, &_frameBuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frameBuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _renderBuffer);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    
    const float maxX = 2;
    const float maxY = 3;
    
    glOrthof(-maxX, +maxX, -maxY, +maxY, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    OnRotate(DeviceOrientationPortrait);
    _currentAngle = _desiredAngle;
}

void RenderingEngine1::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glRotatef(_currentAngle, 0, 0, 1);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Vertices[0].Position[0]);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), &Vertices[0].Color[0]);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}

void RenderingEngine1::OnRotate(DeviceOrientation newOrientation)
{
    float angle = 0;
    
    switch (newOrientation) {
        case DeviceOrientationLandscapeLeft:
            angle = 270;
            break;
        case DeviceOrientationPortraitUpsideDown:
            angle = 180;
            break;
        case DeviceOrientationLandscapeRight:
            angle = 90;
            break;
        default:
            break;
    }
    
    _desiredAngle = angle;
}

float RenderingEngine1::RotationDirection() const
{
    float delta = _desiredAngle - _currentAngle;
    if (delta == 0) {
        return 0;
    }
    
    bool counterClockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterClockwise ? +1 : -1;
}

void RenderingEngine1::UpdateAnimation(float timeStep)
{
    float direction = RotationDirection();
    if (direction == 0){
        return;
    }
    
    float degrees = timeStep * 360 * RevolutionsPerSecond;
    _currentAngle += degrees * direction;
    
    if (_currentAngle >= 360) {
        _currentAngle -= 360;
    }
    else if (_currentAngle < 0)
    {
        _currentAngle += 360;
    }
    
    if (RotationDirection() != direction) {
        _currentAngle = _desiredAngle;
    }
    
}





