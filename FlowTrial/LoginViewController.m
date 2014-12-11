//
//  ViewController.m
//  FlowTrial
//
//  Created by Wade Sellers on 12/10/14.
//  Copyright (c) 2014 Wade Sellers. All rights reserved.
//

#import "LoginViewController.h"
#import "User.h"

@interface LoginViewController () <UIAlertViewDelegate>

@property (weak, nonatomic) IBOutlet UILabel *usernameLabel;
@property (weak, nonatomic) IBOutlet UILabel *passwordLabel;
@property (weak, nonatomic) IBOutlet UILabel *confirmPasswordLabel;

@property (weak, nonatomic) IBOutlet UITextField *usernameTextField;
@property (weak, nonatomic) IBOutlet UITextField *passwordTextField;
@property (weak, nonatomic) IBOutlet UITextField *confirmPasswordTextField;

@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UIButton *signUpButton;
@property (weak, nonatomic) IBOutlet UIButton *finishButton;

@property NSArray *user;

@end

@implementation LoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewDidAppear:(BOOL)animated {
    self.confirmPasswordLabel.hidden = YES;
    self.confirmPasswordTextField.hidden = YES;
    self.finishButton.hidden = YES;
}

- (IBAction)onPressedLoginButton:(id)sender {
    PFQuery *userQuery = [PFQuery queryWithClassName:@"User"];
    [userQuery whereKey:@"username" equalTo:self.usernameTextField.text];
    [userQuery findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
        if (error)
        {
            NSLog(@"Error: %@", error.userInfo);
            self.user = [NSArray array];
        }
        else
        {
            self.user = [[NSArray alloc] initWithArray: objects];
        }
    }];

}



- (IBAction)onPressedSignUpButton:(id)sender {
    self.loginButton.hidden = YES;
    self.signUpButton.hidden = YES;
    self.confirmPasswordLabel.hidden = NO;
    self.confirmPasswordTextField.hidden = NO;
    self.finishButton.hidden = NO;
    self.usernameLabel.text = @"Enter a desired username";
    self.passwordLabel.text = @"Enter a password";
}

- (IBAction)onFinishButtonPressed:(id)sender {
    if (![self.passwordTextField.text isEqualToString:self.confirmPasswordTextField.text]) {
        UIAlertView *badConfirmPasswordAlert = [[UIAlertView alloc] initWithTitle:@"Password Mismatch" message:@"Please Try Again" delegate:self cancelButtonTitle:@"Okay" otherButtonTitles: nil];
        [badConfirmPasswordAlert show];
    }
    User *newUser = [[User alloc] init];
    newUser.username = self.usernameLabel.text;
    newUser.password = self.passwordTextField.text;

    PFObject *newUserObject = [PFObject objectWithClassName:@"User"];
    newUserObject[@"username"] = newUser.username;
    newUserObject[@"password"] = newUser.password;
    [newUserObject saveInBackground];

}



@end
