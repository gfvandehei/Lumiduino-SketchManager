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

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);
//========== member_variables


// This is the list of recognized commands.  
// In order to receive, attach a callback function to these events
enum
{
  kCommandList         , // Command to request list of available commands              , // Command to request led to be set in specific state  
  kSetPixel            , // Command to request led to be set in to specific brightness  
  kStatus              , // Command to request led status
};

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kCommandList, OnCommandList);
  cmdMessenger.attach(kSetPixel, OnSetLed);
  cmdMessenger.attach(kStatus, OnStatus);
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  Serial.println("This command is unknown!");
  ShowCommands();
}

// Callback function that shows a list of commands
void OnCommandList()
{
  ShowCommands();
}

// Callback function that sets led on or off
void OnSetLed()
{
  // Read led state argument, expects 0 or 1 and interprets as false or true 
  bool ledState = cmdMessenger.readBoolArg(); 
  ShowLedState();  
}

// Callback function that shows led status
void OnStatus()
{
  // Send back status that describes the led state
  ShowLedState();  
}

// Show available commands
void ShowCommands() 
{
  Serial.println("Available commands");
  Serial.println(" 0;                 - This command list");
  Serial.println(" 1,<strip>, <pixel>, <r>, <g>, <b>, <brightness>;");
  Serial.println(" 2;                  - Show led state");
}

// Show led state
void ShowLedState() 
{
    Serial.print("STATE");
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
  ShowCommands();
}

// Loop function
void loop() 
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}