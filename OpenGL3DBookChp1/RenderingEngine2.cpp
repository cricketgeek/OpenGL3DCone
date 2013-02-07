//
//  RenderingEngine1.cpp
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 2/2/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//


#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <cmath>
#include <iostream>
#include "IRenderingEngine.hpp"

#define STRINGIFY(A) #A
#include "Simple.vert"
#include "Simple.frag"

static const float RevolutionsPerSecond = 1;

class RenderingEngine2 : public IRenderingEngine
{
public:
    RenderingEngine2();
    void Initialize(int width, int height);
    void Render() const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation newOrientation);
private:
    float  RotationDirection() const;
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vShader, const char *fShader) const;
    void ApplyOrtho(float maxX, float maxY) const;
    void ApplyRotation(float degrees) const;
    float _desiredAngle;
    float  _currentAngle;
    GLuint _simpleProgram;
    GLuint _frameBuffer;
    GLuint _renderBuffer;
};

IRenderingEngine *CreateRenderer2()
{
    return new RenderingEngine2();
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

RenderingEngine2::RenderingEngine2()
{
    glGenRenderbuffers(1, &_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
}

void RenderingEngine2::Initialize(int width, int height)
{
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    glViewport(0, 0, width, height);
    
    _simpleProgram = BuildProgram(SimpleVertexShader, SimpleFragmentShader);
    glUseProgram(_simpleProgram);
    
    ApplyOrtho(2, 3);
    
    OnRotate(DeviceOrientationPortrait);
    _currentAngle = _desiredAngle;
}

void RenderingEngine2::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ApplyRotation(_currentAngle);
    
    GLuint positionSlot = glGetAttribLocation(_simpleProgram, "Position");
    GLuint colorSlot = glGetAttribLocation(_simpleProgram, "SourceColor");
    
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(colorSlot);
    
    GLsizei stride = sizeof(Vertex);
    const GLvoid *pCoords = &Vertices[0].Position[0];
    const GLvoid *pColors = &Vertices[0].Color[0];
    
    glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);
    glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glDisableVertexAttribArray(positionSlot);
    glDisableVertexAttribArray(colorSlot);
    
}

void RenderingEngine2::OnRotate(DeviceOrientation newOrientation)
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

float RenderingEngine2::RotationDirection() const
{
    float delta = _desiredAngle - _currentAngle;
    if (delta == 0) {
        return 0;
    }
    
    bool counterClockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterClockwise ? +1 : -1;
}

void RenderingEngine2::UpdateAnimation(float timeStep)
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

GLuint RenderingEngine2::BuildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) const
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(3);
    }
    
    return programHandle;
}

GLuint RenderingEngine2::BuildShader(const char *source, GLenum shaderType) const
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }
    
    return shaderHandle;
}

void RenderingEngine2::ApplyOrtho(float maxX, float maxY) const
{
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(_simpleProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
}

void RenderingEngine2::ApplyRotation(float degrees) const
{
    float radians = degrees * 3.14159f / 180.0f;
    float s = std::sin(radians);
    float c = std::cos(radians);
    
    float zRotation[16] = {
      c, s, 0, 0,
     -s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
    
    GLint modelviewUniform = glGetUniformLocation(_simpleProgram, "Modelview");
    glUniformMatrix4fv(modelviewUniform, 1, 0, &zRotation[0]);
}



