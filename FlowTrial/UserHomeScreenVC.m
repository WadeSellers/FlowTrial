//
//  UserHomeScreenVC.m
//  FlowTrial
//
//  Created by Wade Sellers on 12/11/14.
//  Copyright (c) 2014 Wade Sellers. All rights reserved.
//

#import "UserHomeScreenVC.h"

@interface UserHomeScreenVC ()

@end

@implementation UserHomeScreenVC

- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"Made it to the next page");
    NSLog(@"%@", self.user);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
