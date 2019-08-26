// *** ConsoleShell ***

// This example shows how to use CmdMessenger as a shell, and communicate with it using the Serial Console
// This example is different from all others:
// - there is no PC counterpart 
// - it will only receive commands, instead of sending commands it will use Serial.Pring
//
// Below is an example of interacting with the sample:
// 
//   Available commands
//   0;                  - This command list
//   1,<led state>;      - Set led. 0 = off, 1 = on
//   2,<led brightness>; - Set led brighness. 0 - 1000
//   3;                  - Show led state
//  
// Command> 3;
//  
//  Led status: on
//  Led brightness: 500
//  
// Command> 2,1000;
//  
//   Led status: on
//   Led brightness: 1000
//  
// Command> 1,0;
//  
//   Led status: off
//   Led brightness: 1000

#include <CmdMessenger.h>  // CmdMessenger
#include "FastLED.h"
#include "DHT.h"

// This is the list of recognized commands.  
// In order to receive, attach a callback function to these events
enum
{
  // Received
  r_info      ,
  r_cmds      ,
  r_ping      ,
  r_set_pixel ,
  r_set_strip ,

  // Sent
  s_info      ,
  s_cmds      ,
  s_pong      ,
  s_ack       ,
  s_err       ,
  s_unknown   ,
  s_msg       ,
};
// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, ',', ';', '/');
//========== member_variables

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(s_unknown);
}

// Callback function that shows a list of connected sensors and lights
void OnInfo() {
  const char* info = ""
    " //========== info"
    "";
  cmdMessenger.sendCmd(s_info, info);
}

// Callback function that shows a list of commands
void OnCommandList() {
  const char* commandlist = "0;  | show device information\n"
    "1;  | show available commands, this list\n"
    "2;  | ping for heartbeat\n"
    "3, <strip>, <pixel>, <r>, <g>, <b>, <brightness>;  | set a pixels color\n"
    "4, <strip>, <r>, <g>, <b>, <brightness>;  | set an entire strips color\n";
  cmdMessenger.sendCmd(s_cmds, commandlist);
}

//Callback function for connection heartbeat
void OnPing() {
  cmdMessenger.sendCmd(s_pong);
}

// Callback function that sets led on or off
void OnSetPixel() {
  // Read led state argument, expects 0 or 1 and interprets as false or true 
  int strip = cmdMessenger.readInt16Arg();
  int pixel = cmdMessenger.readInt16Arg();
  int r = cmdMessenger.readInt16Arg();
  int g = cmdMessenger.readInt16Arg();
  int b = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg(); 
  //strips is the generated array of neopixels
  //striplen is the length of This
  //lengths is the generated array of strip lengths
  if(strip >= strip_amt) {
    cmdMessenger.sendCmd(s_err,"The requested strip does not exist");   
    return;
  }
  if(pixel >= lengths[strip]) {
    cmdMessenger.sendCmd(s_err,"The requested pixel does not exist");
    return;
  }
  if (r > 255 || g > 255 || b > 255 || brightness > 255){
    cmdMessenger.sendCmd(s_err,"Check your values");
    return;
  }
  strips[strip][pixel] = CRGB(r,g,b);
  FastLED.setBrightness(brightness);
  FastLED.show();
  cmdMessenger.sendCmd(s_ack);
}

void OnSetStrip() {
  int strip = cmdMessenger.readInt16Arg();
  int r = cmdMessenger.readInt16Arg();
  int g = cmdMessenger.readInt16Arg();
  int b = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg(); 

  if(strip >= strip_amt) {
    cmdMessenger.sendCmd(s_err,"The requested strip does not exist");   
    return;
  }
  if (r > 255 || g > 255 || b > 255 || brightness > 255){
    cmdMessenger.sendCmd(s_err,"Check your values");
    return;
  }

  for(int i = 0; i<lengths[strip]; i++){
    strips[strip][i] = CRGB(r,g,b);
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
  cmdMessenger.sendCmd(s_ack);
}

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  /*
    // Received
    r_info      ,
    r_ping      ,
    r_set_pixel ,
    r_set_strip ,

    // Sent
    s_info      ,
    s_ping      ,
    s_ack       ,
    s_err       ,
    s_msg       ,
   */
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(r_info, OnInfo);
  cmdMessenger.attach(r_cmds, OnCommandList);
  cmdMessenger.attach(r_ping, OnPing);
  cmdMessenger.attach(r_set_pixel, OnSetPixel);
  cmdMessenger.attach(r_set_strip, OnSetStrip);
}

// Setup function
void setup() 
{
  // Listen on serial connection for messages from the PC
  Serial.begin(115200); 
  
  // Adds newline to every command
  cmdMessenger.printLfCr();

  //========== setup

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();
  
  // Show command list
  OnCommandList();
}

// Loop function
void loop() 
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}