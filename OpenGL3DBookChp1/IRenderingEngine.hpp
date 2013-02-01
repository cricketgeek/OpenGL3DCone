//
//  IRenderingEngine.hpp
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 1/31/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//

#ifndef OpenGL3DBookChp1_IRenderingEngine_hpp
#define OpenGL3DBookChp1_IRenderingEngine_hpp

enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight,
    DeviceOrientationFaceUp,
    DeviceOrientationFaceDown,
};

struct IRenderingEngine *CreateRenderer1();

struct IRenderingEngine {
    virtual void Initialize(int width, int height) = 0;
    virtual void Render() const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation newOrientation) = 0;
    virtual ~IRenderingEngine() {}
};

#endif
