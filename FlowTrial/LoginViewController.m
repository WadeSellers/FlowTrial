//
//  ViewController.m
//  FlowTrial
//
//  Created by Wade Sellers on 12/10/14.
//  Copyright (c) 2014 Wade Sellers. All rights reserved.
//

#import "LoginViewController.h"
#import "User.h"
#import "UserHomeScreenVC.h"

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

@property NSArray *userArray;
@property User *user;

@end

@implementation LoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.user = [[User alloc] init];
}

- (void)viewDidAppear:(BOOL)animated {
    self.confirmPasswordLabel.hidden = YES;
    self.confirmPasswordTextField.hidden = YES;
    self.finishButton.hidden = YES;
}

- (IBAction)onPressedLoginButton:(id)sender {
    self.user.username = self.usernameTextField.text;
    self.user.password = self.passwordTextField.text;

    PFQuery *userQuery = [PFQuery queryWithClassName:@"User"];
    [userQuery whereKey:@"username" equalTo:self.user.username];
    [userQuery findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
        if (error)
        {
            NSLog(@"Error: %@", error.userInfo);
            self.userArray = [NSArray array];
        }
        else
        {
            self.userArray = [[NSArray alloc] initWithArray: objects];

            if ([[[self.userArray objectAtIndex:0] valueForKey:@"password"] isEqualToString:self.user.password])
            {
                [self performSegueWithIdentifier:@"intoHomeScreenSegue" sender:sender];
            }
            else
            {
                UIAlertView *badLoginAlert = [[UIAlertView alloc] initWithTitle:@"Incorrect Login" message:@"Password doesn't match for Username, try again." delegate:self cancelButtonTitle:@"Okay" otherButtonTitles: nil];
                [badLoginAlert show];
            }
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
    newUser.username = self.usernameTextField.text;
    newUser.password = self.passwordTextField.text;

    PFObject *newUserObject = [PFObject objectWithClassName:@"User"];
    newUserObject[@"username"] = newUser.username;
    newUserObject[@"password"] = newUser.password;
    [newUserObject saveInBackground];

}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    UserHomeScreenVC *userHomeScreenVC = [segue destinationViewController];
    userHomeScreenVC.user = [self.userArray objectAtIndex:0];
}



@end
