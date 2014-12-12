//
//  Ugi.h
//
//  Copyright (c) 2012 U Grok It. All rights reserved.
//

#define __UGI_H

#import <UIKit/UIKit.h>

#import "UgiInventory.h"
#import "UgiRfidConfiguration.h"
#import "UgiEpc.h"
#import "UgiTag.h"
#import "UgiTagReadState.h"
#import "UgiInventoryDelegate.h"

/**
 Singleton class that implements the U Grok It API.
 
 The Ugi class is used as a singleton - only one instance of the class exists.
 
 The singleton object is automatically created the first time it is referenced so no
 explicit initialization is needed.
 */
@interface Ugi : NSObject

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Lifecycle
///////////////////////////////////////////////////////////////////////////////////////

/**
 Get the singleton object.
 
 @return The one and only Ugi object, through which the application accesses the API.
 */
+ (Ugi *) singleton;

/**
 Release the singleton object.
 
 Normally called in applicationWillTerminate
 */
+ (void) releaseSingleton;

/**
 See if the application has microphone permission.
 This always returns YES prior to iOS7
 @param callback Callback called with permission status
 */
- (void) checkMicPermission:(void(^)(BOOL havePermission))callback;

/**
 Get the desired orientation (pre-iOS6)
 */
- (UIInterfaceOrientation) desiredInterfaceOrientation;
/**
 Get the preferred orientation (iOS6)
 */
- (UIInterfaceOrientationMask) desiredInterfaceOrientationMask;

///@name Connection

/**
 Try to open a connection to the reader.
 
 openConnection returns immediately, it does not wait for a connection to the reader
 to actually be established. If a reader is connected, the connection takes 400-500ms
 (just under half a second) for the connection sequence. Your app can get notification
 of connection state changes by registering with the default NotificationCenter with
 the name [Ugi singleton].NOTIFICAION_NAME_CONNECTION_STATE_CHANGED:
 @code
 [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(connectionStateChanged:)
                                              name:[Ugi singleton].NOTIFICAION_NAME_CONNECTION_STATE_CHANGED
                                            object:nil];
 @endcode
 
 This method is normally called in applicationDidBecomeActive but may be called
 elsewhere if the app does not want to always be connected to the reader.
 */
- (void) openConnection;

/**
 Close connection to the reader.
 
 This method is normally called in applicationWillTerminate.
 */
- (void) closeConnection;

///@}

///@name Connection Properties

//! Has openConnection has been called (without a corresponding call to closeConnecion)
@property (readonly, nonatomic) BOOL inOpenConnection;

//! Is anything is plugged into the audio jack (as best we can determine)
@property (readonly, nonatomic) BOOL isAnythingPluggedIntoAudioJack;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Connection state
///////////////////////////////////////////////////////////////////////////////////////

///@name Connection Types

/**
 States for the connection, sent with [Ugi singleton].NOTIFICAION_NAME_CONNECTION_STATE_CHANGED
 */
typedef enum {
  UGI_CONNECTION_STATE_NOT_CONNECTED,        //!< Nothing connected to audio port
  UGI_CONNECTION_STATE_CONNECTING,           //!< Something connected to audio port, trying to connect
  UGI_CONNECTION_STATE_INCOMPATIBLE_READER,  //!< Connected to an reader with incompatible firmware
  UGI_CONNECTION_STATE_CONNECTED             //!< Connected to reader
} UgiConnectionStates;

///@}

///@name Connection Properties

/**
 Notification of connection state changed is sent to default NSNotificationCenter. The object
 sent with the notification is an NSNumber containing the connection state.
 */
@property (readonly, nonatomic) NSString *NOTIFICAION_NAME_CONNECTION_STATE_CHANGED;
/**
 Notification of connection state changed is sent to default NSNotificationCenter. The object
 sent with the notification is an NSNumber containing the connection state.
 */
@property (readonly, nonatomic) NSString *NOTIFICAION_NAME_INVENTORY_STATE_CHANGED;

//! The current connection state
@property (readonly, nonatomic) UgiConnectionStates connectionState;

//! Whether the reader is connected (returns YES if connectionState == UGI_CONNECTION_STATE_CONNECTED)
@property (readonly, nonatomic) BOOL isConnected;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Inventory
///////////////////////////////////////////////////////////////////////////////////////

///@name Inventory

/**
 Start running inventory (if a reader is connected).
 
 If one or more EPCs are passed in, only they will be reported back to the delegate
 If no EPCs are passed (epcs=nil, numEpcs=0) then all EPCs will be reported back to the delegate

 If a small number of EPCs are passed (<=maxEpcsSentToReader), filtering is done on the
 reader and the reader plays sounds immediately (with no host interaction).
 Otherwise filtering is done on the host, and the host tells the reder when to play sounds
 which is slower.

 The inventory code keeps a history for each tag. This history is the number of finds for each
 time interval. The default is to store history for 20 intervals of 500ms each. This default can
 be modified via properties: historyIntervalMSec and historyDepth.
 
 @param delegate       Delegate object to report back to
 @param configuration  Configuration to use
 @param epcs           EPCs to find, all other EPCs are ignored (or nil to find all EPCs)
 @return               UgiInventory object that will hold the results of this inventory
 */
- (UgiInventory *) startInventory:(id<UgiInventoryDelegate>)delegate
                withConfiguration:(UgiRfidConfiguration*)configuration
                         withEpcs:(NSArray *)epcs;

/**
 Start running inventory (if a reader is connected).
 
 If one or more EPCs are passed in, ignore these EPCs
 
 @param delegate       Delegate object to report back to
 @param configuration  Configuration to use
 @param epcsToIgnore   EPCs to ignore
 @return               UgiInventory object that will hold the results of this inventory
 */
- (UgiInventory *) startInventoryIgnoringEpcs:(id<UgiInventoryDelegate>)delegate
                            withConfiguration:(UgiRfidConfiguration*)configuration
                             withEpcsToIgnore:(NSArray *)epcsToIgnore;

/**
 Start running inventory to find any tags
 
 @param delegate   Delegate object to report back to
 @param configuration  Configuration to use
 @return               UgiInventory object that will hold the results of this inventory
 */
- (UgiInventory *) startInventory:(id<UgiInventoryDelegate>)delegate
                withConfiguration:(UgiRfidConfiguration*)configuration;

/**
 Start running inventory to find one specific tag
 
 @param delegate   Delegate object to report back to
 @param configuration  Configuration to use
 @param epc            EPC to find, all other EPCs are ignored
 @return               UgiInventory object that will hold the results of this inventory
 */
- (UgiInventory *) startInventory:(id<UgiInventoryDelegate>)delegate
                withConfiguration:(UgiRfidConfiguration*)configuration
                          withEpc:(UgiEpc *)epc;

//! Get the currently active inventory object (if any)
@property (readonly, nonatomic) UgiInventory *activeInventory;

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Reader info
///////////////////////////////////////////////////////////////////////////////////////

///@name Reader Information Properties

//! Protocol version that the host requires
@property (readonly, nonatomic) int requiredProtocolVersion;

//! Protocol version that the host supports
@property (readonly, nonatomic) int supportedProtocolVersion;

//////////////

//
// These values are not valid until the reader is connected
//

//! Reader protocol version
@property (readonly, nonatomic) int readerProtocolVersion;

///@}

///@name Reader Information Types

/**
 Reader hardware type and version
 */
typedef enum {
  UGI_READER_HARDWARE_UNKNOWN,                  //!< Unknown
  UGI_READER_HARDWARE_GROKKER_1=5               //!< The original Grokker
} UgiReaderHardwareTypes;

///@}

///@name Reader Information Properties

//! Reader's model
@property (readonly, nonatomic) NSString *readerHardwareModel;

//! Reader's hardware type
@property (readonly, nonatomic) UgiReaderHardwareTypes readerHardwareType;
//! Reader's hardware type name
@property (readonly, nonatomic) NSString *readerHardwareTypeName;
//! Reader's hardware version
@property (readonly, nonatomic) int readerHardwareRevision;

//! Firmware version in the reader, major
@property (readonly, nonatomic) int firmwareVersionMajor;
//! Firmware version in the reader, minor
@property (readonly, nonatomic) int firmwareVersionMinor;
//! Firmware version in the reader, build
@property (readonly, nonatomic) int firmwareVersionBuild;

//! Reader's serial number
@property (readonly, nonatomic) int readerSerialNumber;

///@}

///@name Reader Information Types

//! Regions of the world
typedef enum {
  UGI_REGION_FCC,               //!< UGI_ANTENNA_TYPE_915: USA and others
  UGI_REGION_ETSI,              //!< UGI_ANTENNA_TYPE_866: EU and others
  UGI_REGION_TAIWAN_INDOOR,     //!< UGI_ANTENNA_TYPE_915: Taiwan, indoor use
  UGI_REGION_TAIWAN_OUTDOOR,    //!< UGI_ANTENNA_TYPE_915: Taiwan, outdoor use
  UGI_REGION_CHINA,             //!< UGI_ANTENNA_TYPE_915: China
  UGI_REGION_JAPAN_UNLICENSED,  //!< UGI_ANTENNA_TYPE_915: Japan, unlicensed
  UGI_REGION_JAPAN_LICENSED,    //!< UGI_ANTENNA_TYPE_915: Japan, licensed
  UGI_REGION_AUSTRALIA,         //!< UGI_ANTENNA_TYPE_915: Australia
  UGI_REGION_BRAZIL,            //!< UGI_ANTENNA_TYPE_915: Brazil
  UGI_REGION_HONG_KONG,         //!< UGI_ANTENNA_TYPE_915: Hong Kong, Singapore, Thailand, Vietnam
  UGI_REGION_INDONESIA,         //!< UGI_ANTENNA_TYPE_915: Indonesia
  UGI_REGION_ISRAEL,            //!< UGI_ANTENNA_TYPE_915: Israel
  UGI_REGION_SOUTH_KOREA,       //!< UGI_ANTENNA_TYPE_915: South Korea
  UGI_REGION_LATVIA,            //!< UGI_ANTENNA_TYPE_866: Latvia
  UGI_REGION_MALAYSIA,          //!< UGI_ANTENNA_TYPE_915: Malaysia
  UGI_REGION_NEW_ZEALAND,       //!< UGI_ANTENNA_TYPE_915: New Zealand
  UGI_REGION_PERU,              //!< UGI_ANTENNA_TYPE_915: Peru
  UGI_REGION_RUSSIA,            //!< UGI_ANTENNA_TYPE_915: Russian Federation
  UGI_REGION_SOUTH_AFRICA,      //!< UGI_ANTENNA_TYPE_915: South Africa
} UgiRegion;

//! Antenna types
typedef enum {
  UGI_ANTENNA_TYPE_915,    //!< 905-925MHz (FCC and dirivitives)
  UGI_ANTENNA_TYPE_866     //!< 865-868 MHz (ETSI and dirivitives)
} UgiAntennaType;

///@}

///@name Reader Information Properties

//! Region of the world
@property (readonly, nonatomic) UgiRegion region;

//! name of region of the world
@property (readonly, nonatomic) NSString *regionName;

//! Type of antenna
@property (readonly, nonatomic) UgiAntennaType antennaType;

//! Maximum number of tones in a sound
@property (readonly, nonatomic) int maxTonesInSound;

//! Maximum power that the reader can use
@property (readonly, nonatomic) double maxPower;

//! Maximum sensitivity
@property (readonly, nonatomic) int maxSensitivity;

//! Maximum number of volume levels
@property (readonly, nonatomic) int numVolumeLevels;

//! YES if the reader powers itself on and off automatically
@property (readonly, nonatomic) BOOL canAutoPower;

//! YES if the reader can sense the battery level
@property (readonly, nonatomic) BOOL canReadBatteryLevel;

//! YES if the reader can tune its antenna
@property (readonly, nonatomic) BOOL canTuneAntenna;

//! YES if the reader has battery power
@property (readonly, nonatomic) BOOL hasBattery;

//! YES if the reader has external power
@property (readonly, nonatomic) BOOL hasExternalPower;

//! Battery capacity in minutes
@property (readonly, nonatomic) int batteryCapacity;

//! Battery capacity in mAh
@property (readonly, nonatomic) int batteryCapacity_mAh;

//! YES if device initialized successfully
@property (readonly, nonatomic) BOOL deviceInitializedSuccessfully;


//! Description of the reader, generally used for debugging
@property (readonly, nonatomic) NSString *readerDescription;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Reader Configuration: Sounds
///////////////////////////////////////////////////////////////////////////////////////

/**
 @public
 Geiger counter sound configuration, used by getGeigerCounterSound and setGeigerCounterSound
 */
typedef struct {
  int frequency;          //!< @public Frequency for each click (Hz)
  int durationMsec;       //!< @public Duration of each click, in milliseconds
  double clickRate;        //!< @public Ratio used for translating finds/second into clicks/second
  int maxClicksPerSecond; //!< @public Maximum number of clicks per second
  int historyDepthMsec;   //!< @public Number of history periods to consider for determining click rate
} UgiGeigerCounterSound;

///@name Configuration

/**
 Get the current geiger counter sound configuration
 
 This configuration is used if UGI_INVENTORY_SOUNDS_GEIGER_COUNTER is
 passed to startInventory

 @param config  Buffer to fill
 @return        YES if successful, NO if reader has never been connected)
 */
- (BOOL) getGeigerCounterSound:(UgiGeigerCounterSound *)config;

/**
 Set the geiger counter sound configuration
 
 This configuration is used if UGI_INVENTORY_SOUNDS_GEIGER_COUNTER is
 passed to startInventory
 
 If no reader is connected, the reader will be configured with these parameters
 after a connection is established. Similiarly, if the reader is disconnected and
 reconncted, these parameters will be configured with these parameters.
 
 @param config    Configuration parameters to set
 */
- (void) setGeigerCounterSound:(UgiGeigerCounterSound *)config;

///@}

/**
 @public
 A single tone, used by setFoundItemSound and setFoundLastItemSound
 */
typedef struct {
  int frequency;      //!< @public Frequency of tone (Hz)
  int durationMsec;   //!< @public Duration of tone, in milliseconds
} UgiSpeakerTone;

///@name Configuration

/**
 Get the current set of tones played when an item is found
 
 This sound is used if UGI_INVENTORY_SOUNDS_FIRST_FIND or
 UGI_INVENTORY_SOUNDS_FIRST_FIND_AND_LAST is passed to startInventory
 
 @return        A memory buffer containing an array of UgiSpeakerTone
                structures, ending in a structure with durationMsec==0.
                The caller must free() this bufffer.
                Returns NULL if a reader has never been connected
 */
- (UgiSpeakerTone *) getFoundItemSound;

/**
 Set the set of tones played when an item is found
 
 This sound is used if UGI_INVENTORY_SOUNDS_FIRST_FIND or
 UGI_INVENTORY_SOUNDS_FIRST_FIND_AND_LAST is passed to startInventory
 
 If no reader is connected, the reader will be configured with these parameters
 after a connection is established. Similiarly, if the reader is disconnected and
 reconncted, these parameters will be configured with these parameters.
 
 @param sound     Array of UgiSpeakerTone structures, ending in a
                  structure with durationMsec==0.
 */
- (void) setFoundItemSound:(UgiSpeakerTone *)sound;

/**
 Get the current set of tones played when the last item is found
 
 This sound is used if UGI_INVENTORY_SOUNDS_FIRST_FIND_AND_LAST
 is passed to startInventory
 
 @return        A memory buffer containing an array of UgiSpeakerTone
                structures, ending in a structure with durationMsec==0.
                The caller must free() this bufffer.
                Returns NULL if a reader has never been connected
 */
- (UgiSpeakerTone *) getFoundLastItemSound;

/**
 Set the set of tones played when the last item is found
 
 This sound is used if UGI_INVENTORY_SOUNDS_FIRST_FIND_AND_LAST
 is passed to startInventory
 
 If no reader is connected, the reader will be configured with these parameters
 after a connection is established. Similiarly, if the reader is disconnected and
 reconncted, these parameters will be configured with these parameters.
 
 @param sound     Array of UgiSpeakerTone structures, ending in a
                  structure with durationMsec==0.
 */
- (void) setFoundLastItemSound:(UgiSpeakerTone *)sound;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Diagnostic data
///////////////////////////////////////////////////////////////////////////////////////

/**
 @public
 Diagnostic data
 */
typedef struct {
	double byteProtocolSkewFactor;           //!< @public Factor the the reader's clock is off by
	int byteProtocolBytesSent;               //!< @public Bytes sent by reader
	int byteProtocolBytesReceived;           //!< @public Bytes received by reader
	int byteProtocolSubsequentReadTimeouts;  //!< @public Reader timeouts waiting for a next byte in a packet
	int packetProtocolPacketsSent;           //!< @public Packets sent by reader
	int packetProtocolPacketsReceived;       //!< @public Bytes received by reader
	int packetProtocolSendFailures;          //!< @public Reader failures sending a packet
	int packetProtocolSendRetries;           //!< @public Reader retries sending packets
	int packetProtocolSendTimeouts;          //!< @public Reader timeouts sending packets
	int packetProtocolInvalidPackets;        //!< @public Reader invalid packets received
	int packetProtocolInternalCrcMismatches; //!< @public Reader responses received with illegal enbedded CRCs
	int packetProtocolCrcMismatches;         //!< @public Reader packets received with wrong CRC
	int rawInventoryRounds;                  //!< @public Number of inventory rounds run
	int rawTagFinds;                         //!< @public Number of tag finds
	int inventoryUnique;                     //!< @public Number of unique tags found
	int inventoryForgotten;                  //!< @public Number of forgotten tags
	int inventoryForgottenNotAcknowledged;   //!< @public Number of forgotten tags not acknowledged by the host
	int inventoryForgottenNotSent;           //!< @public Number of forgotten tags not sent to the host
} UgiDiagnosticData;

///@name Diagnostic information

/**
 Get diagnostic data
 
 @param data            Buffer to fill
 @param reset           YES to reset counters
 @return        YES if successful, NO if reader has never been connected)
 */
- (BOOL) getDiagnosticData:(UgiDiagnosticData *)data
             resetCounters:(BOOL)reset;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Battery
///////////////////////////////////////////////////////////////////////////////////////

/**
 @public
 Battery data
 */
typedef struct {
  BOOL canScan;                     //!< @public YES if battery is capable of scanning
  BOOL externalPowerIsConnected;    //!< @public YES if external power is connected
  BOOL isCharging;                  //!< @public YES if battery is charging
	int minutesRemaining;             //!< @public Minutes of scanning remaining
  int percentRemaining;             //!< @public Percent of scanning time remaining. This is not very accurate while charging
  double voltage;                   //!< @public Battery voltage
} UgiBatteryInfo;

/**
 Get battery information
 
 @param retBatteryInfo            Structure to return information in
 @return                    YES if successful, NO if reader has never been connected)
 */
- (BOOL) getBatteryInfo:(UgiBatteryInfo *)retBatteryInfo;

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Misc
///////////////////////////////////////////////////////////////////////////////////////

// This is not part of the external documentation yet because it is likely to change

//! @cond
/**
 Sounds playable on the reader via playSound
 */
typedef enum {
  UGI_PLAY_SOUND_FOUND_LAST     //!< The "last item found" sound
} UgiPlaySoundSoundTypes;

/**
 Play a sound on the reader
 
 @param sound   Sound to play
 */
- (void) playSound:(UgiPlaySoundSoundTypes)sound;
//! @endcond

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Logging
///////////////////////////////////////////////////////////////////////////////////////

///@name Logging Types

/**
 Types of logging.
 
 The default is no logging. The internal logging types are primarily for debugging
 of the API itself.
 */
typedef enum {
  UGI_LOGGING_INTERNAL_BYTE_PROTOCOL = 0x1,     //!< Lowest level communication protocol:
                                                //!< connection handshaking and byte send/receive
  UGI_LOGGING_INTERNAL_CONNECTION_ERRORS = 0x2, //!< Low level communication errors
  UGI_LOGGING_INTERNAL_CONNECTION_STATE = 0x4,  //!< Low level connection state changes 
  UGI_LOGGING_INTERNAL_PACKET_PROTOCOL = 0x8,   //!< Packet send/receive
  UGI_LOGGING_INTERNAL_COMMAND = 0x10,          //!< Command send/receive
  UGI_LOGGING_INTERNAL_INVENTORY = 0x20,        //!< Low-level inventory
  UGI_LOGGING_INTERNAL_FIRMWARE_UPDATE = 0x40,  //!< Low-level firmware update
  
  UGI_LOGGING_STATE = 0x1000,             //!< Connection and inventory state
  UGI_LOGGING_INVENTORY = 0x2000,         //!< Inventory activity
  UGI_LOGGING_INVENTORY_DETAIL = 0x4000   //!< Inventory details
} UgiLoggingTypes;

//! Function prototype for custom logging destination
typedef void LoggingDestination(NSString *s, NSObject *param);

///@}

///@name Logging Properties

/**
 The current logging status
 */
@property (nonatomic) UgiLoggingTypes loggingStatus;

/**
 If YES, add a timestamp to each logging line (default = NO)
 */
@property (nonatomic) BOOL loggingTimestamp;

/**
 Set the logging destination

 By default logging goes to NSLog()
 @param loggingDestination   Desitination (fuction) to send logging output to
 @param param                Opaque callback parameter
 */
- (void) setLoggingDestination:(LoggingDestination *)loggingDestination
                     withParam:(NSObject *)param;

///@}

///////////////////////////////////////////////////////////////////////////////////////
#pragma mark - SDK version
///////////////////////////////////////////////////////////////////////////////////////

///@name SDK Version Properties

/**
 SDK Version, major
 
 <br>
 Version 1.0.2 - March 17, 2014 - Reader protocol 11<br>
   Minor bug fixes<br>
   Custom commands<br>
 <br>
 Version 1.1.1 - April 25, 2014 - Reader protocol 13<br>
   Change UGI_MIN_EPC_LENGTH from 8 to 4<br>
   Tweak settings for INVENTORY_DISTANCE<br>
 <br>
 Version 1.2.1 - July 7, 2014 - Reader protocol 14<br>
   Production Grokker compatibility<br>
   Changing power during inventory<br>
 <br>
 Version 1.3.1 - August 2, 2014 - Reader protocol 16<br>
   More options for SELECT before inventory<br>
   Improved device compatibility<br>
   Optional data reading on every tag read<br>
 <br>
 Version 1.3.2 - August 26, 2014 - Reader protocol 17<br>
   XCode 6 and iOS 8 compatibility<br>
 <br>
 Version 1.4.1 - September 10, 2014 - Reader protocol 17<br>
   RFID configuration changes<br>
   Bug fixes<br>
 */
@property (readonly, nonatomic) int sdkVersionMajor;
//! SDK Version, minor
@property (readonly, nonatomic) int sdkVersionMinor;
//! SDK Version, minor
@property (readonly, nonatomic) int sdkVersionBuild;
//! SDK Version, date/time
@property (readonly, nonatomic) NSDate *sdkVersionDateTime;

///@}

@end
