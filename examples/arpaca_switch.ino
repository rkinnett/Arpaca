#include <Arduino.h>
#include <WiFi.h>
#include <Arpaca.h>

#if __has_include("my_wifi.h") 
#include "my_wifi.h"
#endif
#ifndef MY_WIFI_CREDENTIALS
const char wifi_ssid[]     = "my_wifi_ssid";
const char wifi_password[] = "my_wifi_password";
#endif

// TO-DO:
// make port 80 webpage to include user-configurable alpaca http port
// make filter names and offsets configurable by port 80 page

typedef struct {
  char name[32];
  int focus_offset;
} Filter;

#define NUM_FILTERS 7

Filter filters[] = {
  {"Luminance", 0},
  {"Red",       0},
  {"Green",     0},
  {"Blue",      0},
  {"Sii",       0},
  {"Ha",        0},
  {"Oiii",      0}
};

int filter_wheel_position = 0;

// To-Do:  Add local error definitions:
ErrorInfo LocalError[] = {
  {0x500, "Generic firmware error"},
  {0x501, "Unable to parse request."},
};

typedef enum LocalErrorIdx {
  LOCAL_ERROR_GENERIC,
  LOCAL_ERROR_FAILRED_TO_PARSE_REQUEST,
} LocalErrorIdx;



AlpacaDevice Device(DEVICE_TYPE_FILTER_WHEEL, 0);

// Prototypes for Alpaca common functions:
void doOtherAction(void);
void doCommandBlind(void);
void doCommandBool(void);
void doCommandString(void);
void getConnectionState(void);
void setConnectionState(void);
void getDescription(void);
void getDriverInfo(void);
void getDriverVersion(void);
void getInterfaceVersion(void);
void getName(void);
void getOtherActions(void);

// Prototypes for Alpaca device type-specific functions:
void getFilterFocusOffsets(void);
void getFilterNames(void);
void getFilterWheelPosition(void);
void setFilterWheelPosition(void);



void setup() {

  // Set Alpaca common functions:
  Device.setAction(ACTION_COMMON_PUT_ACTION,               doOtherAction);
  Device.setAction(ACTION_COMMON_PUT_COMMAND_BLIND,        doCommandBlind);
  Device.setAction(ACTION_COMMON_PUT_COMMAND_BOOL,         doCommandBool);
  Device.setAction(ACTION_COMMON_PUT_COMMAND_STRING,       doCommandString);
  Device.setAction(ACTION_COMMON_GET_CONNECTED,            getConnectionState);
  Device.setAction(ACTION_COMMON_PUT_CONNECTED,            setConnectionState);
  Device.setAction(ACTION_COMMON_GET_DESCRIPTION,          getDescription);
  Device.setAction(ACTION_COMMON_GET_DRIVER_INFO,          getDriverInfo);
  Device.setAction(ACTION_COMMON_GET_DRIVER_VERSION,       getDriverVersion);
  Device.setAction(ACTION_COMMON_GET_INTERFACE_VERSION,    getInterfaceVersion);
  Device.setAction(ACTION_COMMON_GET_NAME,                 getName);
  Device.setAction(ACTION_COMMON_GET_SUPPORTED_ACTIONS,    getOtherActions);

  // Set Alpaca device type-specific functions:
  Device.setAction(ACTION_FILTER_WHEEL_GET_FOCUS_OFFSETS,  getFilterFocusOffsets);
  Device.setAction(ACTION_FILTER_WHEEL_GET_NAMES,          getFilterNames);
  Device.setAction(ACTION_FILTER_WHEEL_GET_POSITION,       getFilterWheelPosition);
  Device.setAction(ACTION_FILTER_WHEEL_PUT_POSITION,       setFilterWheelPosition);

  // Start Alpaca device server:
  Device.begin(11111);

}

void loop() {
  // put your main code here, to run repeatedly:

}


void getFilterFocusOffsets(void){
  char* strServerTransactionID[PARAMETER_BUFF_LEN + 1];
  snprintf("%i", Device.serverTransactionCount++, PARAMETER_BUFF_LEN);
  Serial.print(strServerTransactionID); Serial.print(" "); Serial.print(__func__);
  char* strClientTransactionID[PARAMETER_BUFF_LEN];
  int gotClientTransactionID = Device.parseRequest() && Device.fetchRequestParameter("ClientTransactionID", strClientTransactionID);

  String strFilterOffsets = "[";
  for (int filter_idx=0; filter_idx<NUM_FILTERS; filter_idx++){
    strFilterOffsets += String(filters[filter_idx].focus_offset);
    strFilterOffsets += ",";
  }
  strFilterOffsets += "]";
  Serial.print("Filter focus offsets array: ");
  Serial.println(strFilterOffsets);
  if (gotClientTransactionID){
    Device.sendGetRequestNominalResponse(strFilterOffsets.c_str());
  } else {
    
  }
}

void getFilterNames(void){
  int serverTransactionID = Device.serverTransactionCount++;
  Serial.print(serverTransactionID); Serial.print(" "); Serial.print(__func__);
  
  String strFilterNames = "[";
  for (int filter_idx=0; filter_idx<NUM_FILTERS; filter_idx++){
    strFilterNames += filters[filter_idx].name;
    strFilterNames += ",";
  }
  strFilterNames += "]";
  Serial.print("Filter names array: ");
  Serial.println(strFilterNames);
  Device.sendGetRequestNominalResponse(strFilterNames.c_str());
}

void getFilterWheelPosition(void){
  int serverTransactionID = Device.serverTransactionCount++;
  Serial.print(serverTransactionID); Serial.print(" "); Serial.print(__func__);

  String strFilterWheelPosition = String(filter_wheel_position);
  Device.sendGetRequestNominalResponse(strFilterWheelPosition.c_str());
}

void setFilterWheelPosition(void){
  int serverTransactionID = Device.serverTransactionCount++;
  Serial.print(serverTransactionID); Serial.print(" "); Serial.print(__func__);

  // Require request parameters:
  if (Device.parseRequest() == FAILURE){
    Device.sendGetRequestErrorResponse(
      LocalError[LOCAL_ERROR_FAILRED_TO_PARSE_REQUEST].errorID, 
      LocalError[LOCAL_ERROR_FAILRED_TO_PARSE_REQUEST].errorDesc
    );
  }
  // Require 


  // To-do:  Require connection
  // To-do:  access commanded filter position from PUT request
  // filter_wheel_position = 
  //Device.sendPutRequestNominalResponse();
}

void doOtherAction(void) {}
void doCommandBlind(void) {}
void doCommandBool(void) {}
void doCommandString(void) {}
void getConnectionState(void) {}
void setConnectionState(void) {}
void getDescription(void) {}
void getDriverInfo(void) {}
void getDriverVersion(void) {}
void getInterfaceVersion(void) {}
void getName(void) {}
void getOtherActions(void) {}
