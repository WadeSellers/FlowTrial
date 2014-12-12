//
//  UserHomeScreenVC.m
//  FlowTrial
//
//  Created by Wade Sellers on 12/11/14.
//  Copyright (c) 2014 Wade Sellers. All rights reserved.
//

#import "UserHomeScreenVC.h"

@interface UserHomeScreenVC () <UISearchBarDelegate>                                                                                                                                                                                                                                                            
@property (weak, nonatomic) IBOutlet UISearchBar *searchBar;

@property (weak, nonatomic) IBOutlet UITextField *nameTextField;
@property (weak, nonatomic) IBOutlet UITextField *ageTextField;
@property (weak, nonatomic) IBOutlet UITextField *phaseTextField;
@property (weak, nonatomic) IBOutlet UITextField *cloneTextField;
@property (weak, nonatomic) IBOutlet UITextField *vegetativeTextField;
@property (weak, nonatomic) IBOutlet UITextField *floweringTextField;

@property NSArray *parsedItems;
@property PFObject *displayedItem;

@end

@implementation UserHomeScreenVC

- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"%@", self.user);
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
    [self parseForResults];

}

- (void)parseForResults {
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    [formatter setNumberStyle:NSNumberFormatterDecimalStyle];
    NSNumber *barcode = [formatter numberFromString:self.searchBar.text];

    PFQuery *query = [PFQuery queryWithClassName:@"Inventory"];
    [query whereKey:@"barcode" equalTo:barcode];
    [query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
        if (error) {
            NSLog(@"Error: %@", error.userInfo);
        }
        else {
            self.parsedItems =  [[NSArray alloc] initWithArray:objects];
            NSLog(@"list of items: %@", self.parsedItems);
            self.displayedItem = [[PFObject alloc] initWithClassName:@"Inventory"];
            self.displayedItem = [objects firstObject];

            [self displayItemInformation];
        }
    }];
}

- (void)displayItemInformation {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"MM/dd/yyyy"];
    NSString *cloneDate = [dateFormatter stringFromDate:self.displayedItem[@"clone"]];
    NSString *vegetativeDate = [dateFormatter stringFromDate:self.displayedItem[@"vegetative"]];
    NSString *floweringDate = [dateFormatter stringFromDate:self.displayedItem[@"flowering"]];

    self.nameTextField.text = self.displayedItem[@"name"];
    self.ageTextField.text = [self.displayedItem[@"age"] stringValue];
    self.phaseTextField.text = self.displayedItem[@"phase"];
    self.cloneTextField.text = cloneDate;
    self.vegetativeTextField.text = vegetativeDate;
    self.floweringTextField.text = floweringDate;
}


#pragma mark - UGrokIt delegate methods
- (id) init {
    self = [super init];
    [Ugi singleton].loggingStatus = UGI_LOGGING_STATE | UGI_LOGGING_INVENTORY;
    return self;
}






@end
