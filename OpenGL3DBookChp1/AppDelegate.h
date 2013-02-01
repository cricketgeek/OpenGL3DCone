//
//  AppDelegate.h
//  OpenGL3DBookChp1
//
//  Created by Mark Jones on 1/31/13.
//  Copyright (c) 2013 Mark Jones. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLView.h"

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) GLView *view;
@property (strong, nonatomic) ViewController *viewController;

@end
