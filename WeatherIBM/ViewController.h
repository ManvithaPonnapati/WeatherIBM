//
//  ViewController.h
//  WeatherIBM
//
//  Created by Manu on 12/31/14.
//  Copyright (c) 2014 manu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

- (IBAction)loginfb:(id)sender;

@property (weak, nonatomic) IBOutlet UITextView *datafield;
- (IBAction)gotdata:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *newfetch;
- (IBAction)fetchact:(id)sender;

@end

