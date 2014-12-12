//
//  Ugi_antennaTuning.h
//
//  Copyright (c) 2012 U Grok It. All rights reserved.
//

#define __UGI_ANTENNA_UPDATE_H

#import <Foundation/Foundation.h>

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - UgiTuneAntennaDelegate
///////////////////////////////////////////////////////////////////////////////////////

/**
 A UgiTuneAntennaDelegate object is passed to the tuneAntenna method
 of the Ugi singleton.
 
 All of the protocol methods are optional, the delegate only implements the
 methods it needs.
 */
@protocol UgiTuneAntennaDelegate <NSObject>

@optional

/**
 Callback for each frequency tuned
 
 @param frequency         Frequenecy tuned
 @param reflectedPower    Reflected power at this frequency
 @param cin               Value for capacitor Cin
 @param clen              Value for capacitor Clen
 @param cout              Value for capacitor Cout
 @param numTuned          Number of frequencies tuned
 @param numTotal          Number of total frequencies
 @return YES to cancel
 */
- (BOOL) tuneAntennaOneFrequencyTuned:(int)frequency
                                   rp:(int)reflectedPower
                                   ci:(int)cin
                                   cl:(int)clen
                                   co:(int)cout
                                tuned:(int)numTuned
                                total:(int)numTotal;

/**
 Antenna tuning complete
 
 @param success  YES is successful
 */
- (void) tuneAntennaCompleted:(BOOL)success;

///@}

@end

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Antenna Tuning
///////////////////////////////////////////////////////////////////////////////////////

/**
 Antenna tuning portion of Ugi interfacer
 */
@interface Ugi ()

/**
 Tune the antenna
 
 @param delegate    Object to send notifications to
 */
- (void) tuneAntenna:(id<UgiTuneAntennaDelegate>)delegate;

/**
 Get antenna tuning information
 
 @param staticAverage   Average tuning value from last saved tuning
 @param dynamicAverage  Average tuning value currently
 @return        YES if successful
 */
- (BOOL) getAntennaTuning:(int*)staticAverage dynamicAverage:(int*)dynamicAverage;

@end
