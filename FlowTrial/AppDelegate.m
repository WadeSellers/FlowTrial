//
//  AppDelegate.m
//  FlowTrial
//
//  Created by Wade Sellers on 12/10/14.
//  Copyright (c) 2014 Wade Sellers. All rights reserved.
//

#import "AppDelegate.h"
#import <Parse/Parse.h>
#import "Ugi.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [Parse setApplicationId:@"P62Sg9BYv7GhUug21My5nFJennZS1UwFcLzA4Bwn"
                  clientKey:@"B5onwj3iXXduPjS6sD7GtoiHkV8oNiteK2AWtrOX"];
    [PFAnalytics trackAppOpenedWithLaunchOptions:launchOptions];

    [Ugi singleton].loggingStatus = UGI_LOGGING_STATE | UGI_LOGGING_INTERNAL_CONNECTION_ERRORS;
    //
    // Add an observer to get connection state changes
    //
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(connectionStateChanged:)
                                                 name:[Ugi singleton].NOTIFICAION_NAME_CONNECTION_STATE_CHANGED
                                               object:nil];

    [[Ugi singleton] openConnection];

    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [[Ugi singleton] closeConnection];
    [Ugi releaseSingleton];
}

@end
