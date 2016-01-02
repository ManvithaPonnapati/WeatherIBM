//
//  ViewController.m
//  WeatherIBM
//
//  Created by Manu on 12/31/14.
//  Copyright (c) 2014 manu. All rights reserved.
//

#import "ViewController.h"
#import <IMFPush/IMFPush.h>
#import "IMFURLProtocol.h"
#import <FacebookSDK/FacebookSDK.h>
#import <IMFCore/IMFCore.h>
#import <IMFFacebookAuthentication/IMFFacebookAuthenticationHandler.h>
@interface ViewController ()

@end

@implementation ViewController

@synthesize datafield;
- (void)viewDidLoad {
    [[IMFFacebookAuthenticationHandler sharedInstance] registerWithDefaultDelegate];
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)loginfb:(id)sender {
    IMFResourceRequest* request = [IMFResourceRequest requestWithPath:@"http://WeatherIBM.mybluemix.net/protected"];
    [request setHTTPMethod:@"GET"];
    [request sendWithCompletionHandler:^(IMFResponse *response, NSError *error) {
        if (error != nil) {
            // process the error
        } else {
            // process success
            
        }
    }];
}


- (IBAction)fetchact:(id)sender {
    NSURLRequest *Request = [NSURLRequest requestWithURL:[NSURL URLWithString: @"https://febc5de7-fd2d-4f0d-a024-d9c0717939a9:UULI6N8mFz@twcservice.mybluemix.net:443/api/weather/v2/forecast/daily/10day?units=m&geocode=45.42%2C75.69&language=en-US"]];
    NSURLResponse *resp = nil;
    NSError *error = nil;
    NSData *response = [NSURLConnection sendSynchronousRequest: Request returningResponse: &resp error: &error];
    NSString *responseString = [[NSString alloc] initWithData:response encoding:NSUTF8StringEncoding];
    NSLog(@"%@",responseString);
    datafield.text=responseString;
    
}
@end
