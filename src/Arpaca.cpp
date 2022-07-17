#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "Arpaca.h"
#include <ESPmDNS.h>


extern const ErrorID             ALPACA_NO_ERROR_ERROR_ID        = 0;
extern const char*               ALPACA_NO_ERROR_ERROR_DESC      = "";
extern const ClientTransactionID ALPACA_NO_CLIENT_TRANSACTION_ID = -1;
extern const ServerTransactionID ALPACA_NO_SERVER_TRANSACTION_ID = -1;
extern const char*               ALPACA_EMPTY_RESPONSE_VALUE     = "";


// These string names are used to associate URIs with server actions
const char *device_types[] = {
  "camera",
  "covercalibrator",
  "dome",
  "filterwheel",
  "focuser",
  "observingconditions",
  "rotator",
  "safetymonitor",
  "switch",
  "telescope"
};


AlpacaAction CommonActions[] = {
  {HTTP_PUT, "action",                   actionUndefined},
  {HTTP_PUT, "commandblind",             actionUndefined},
  {HTTP_PUT, "commandbool",              actionUndefined},
  {HTTP_PUT, "commandstring",            actionUndefined},
  {HTTP_GET, "connected",                actionUndefined},
  {HTTP_PUT, "connected",                actionUndefined},
  {HTTP_GET, "description",              actionUndefined},
  {HTTP_GET, "driverinfo",               actionUndefined},
  {HTTP_GET, "driverversion",            actionUndefined},
  {HTTP_GET, "interfaceversion",         actionUndefined},
  {HTTP_GET, "name",                     actionUndefined},
  {HTTP_GET, "supportedactions",         actionUndefined},
};

AlpacaAction CameraActions[] = {
  {HTTP_GET, "bayeroffsetx",             actionUndefined},
  {HTTP_GET, "bayeroffsety",             actionUndefined},
  {HTTP_GET, "binx",                     actionUndefined},
  {HTTP_PUT, "binx",                     actionUndefined},
  {HTTP_GET, "biny",                     actionUndefined},
  {HTTP_PUT, "biny",                     actionUndefined},
  {HTTP_GET, "camerastate",              actionUndefined},
  {HTTP_GET, "cameraxsize",              actionUndefined},
  {HTTP_GET, "cameraysize",              actionUndefined},
  {HTTP_GET, "canabortexposure",         actionUndefined},
  {HTTP_GET, "canasymmetricbin",         actionUndefined},
  {HTTP_GET, "canfastreadout",           actionUndefined},
  {HTTP_GET, "cangetcoolerpower",        actionUndefined},
  {HTTP_GET, "canpulseguide",            actionUndefined},
  {HTTP_GET, "cansetccdtemperature",     actionUndefined},
  {HTTP_GET, "canstopexposure",          actionUndefined},
  {HTTP_GET, "ccdtemperature",           actionUndefined},
  {HTTP_GET, "cooleron",                 actionUndefined},
  {HTTP_PUT, "cooleron",                 actionUndefined},
  {HTTP_GET, "coolerpower",              actionUndefined},
  {HTTP_GET, "electronsperadu",          actionUndefined},
  {HTTP_GET, "exposuremax",              actionUndefined},
  {HTTP_GET, "exposuremin",              actionUndefined},
  {HTTP_GET, "exposureresolution",       actionUndefined},
  {HTTP_GET, "fastreadout",              actionUndefined},
  {HTTP_PUT, "fastreadout",              actionUndefined},
  {HTTP_GET, "fullwellcapacity",         actionUndefined},
  {HTTP_GET, "gain",                     actionUndefined},
  {HTTP_PUT, "gain",                     actionUndefined},
  {HTTP_GET, "gainmax",                  actionUndefined},
  {HTTP_GET, "gainmin",                  actionUndefined},
  {HTTP_GET, "gains",                    actionUndefined},
  {HTTP_GET, "hasshutter",               actionUndefined},
  {HTTP_GET, "heatsinktemperature",      actionUndefined},
  {HTTP_GET, "imagearray",               actionUndefined},
  {HTTP_GET, "imagearrayvariant",        actionUndefined},
  {HTTP_GET, "imageready",               actionUndefined},
  {HTTP_GET, "ispulseguiding",           actionUndefined},
  {HTTP_GET, "lastexposureduration",     actionUndefined},
  {HTTP_GET, "lastexposurestarttime",    actionUndefined},
  {HTTP_GET, "maxadu",                   actionUndefined},
  {HTTP_GET, "maxbinx",                  actionUndefined},
  {HTTP_GET, "maxbiny",                  actionUndefined},
  {HTTP_GET, "numx",                     actionUndefined},
  {HTTP_PUT, "numx",                     actionUndefined},
  {HTTP_GET, "numy",                     actionUndefined},
  {HTTP_PUT, "numy",                     actionUndefined},
  {HTTP_GET, "offset",                   actionUndefined},
  {HTTP_PUT, "offset",                   actionUndefined},
  {HTTP_GET, "offsetmax",                actionUndefined},
  {HTTP_GET, "offsetmin",                actionUndefined},
  {HTTP_GET, "offsets",                  actionUndefined},
  {HTTP_GET, "percentcompleted",         actionUndefined},
  {HTTP_GET, "pixelsizex",               actionUndefined},
  {HTTP_GET, "pixelsizey",               actionUndefined},
  {HTTP_GET, "readoutmode",              actionUndefined},
  {HTTP_PUT, "readoutmode",              actionUndefined},
  {HTTP_GET, "readoutmodes",             actionUndefined},
  {HTTP_GET, "sensorname",               actionUndefined},
  {HTTP_GET, "sensortype",               actionUndefined},
  {HTTP_GET, "setccdtemperature",        actionUndefined},
  {HTTP_PUT, "setccdtemperature",        actionUndefined},
  {HTTP_GET, "startx",                   actionUndefined},
  {HTTP_PUT, "startx",                   actionUndefined},
  {HTTP_GET, "starty",                   actionUndefined},
  {HTTP_PUT, "starty",                   actionUndefined},
  {HTTP_GET, "subexposureduration",      actionUndefined},
  {HTTP_PUT, "subexposureduration",      actionUndefined},
  {HTTP_PUT, "abortexposure",            actionUndefined},
  {HTTP_PUT, "pulseguide",               actionUndefined},
  {HTTP_PUT, "startexposure",            actionUndefined},
  {HTTP_PUT, "stopexposure",             actionUndefined}
};

AlpacaAction CoverCalibratorActions[] = {
  {HTTP_GET, "brightness",               actionUndefined},
  {HTTP_GET, "calibratorstate",          actionUndefined},
  {HTTP_GET, "coverstate",               actionUndefined},
  {HTTP_GET, "maxbrightness",            actionUndefined},
  {HTTP_PUT, "calibratoroff",            actionUndefined},
  {HTTP_PUT, "calibratoron",             actionUndefined},
  {HTTP_PUT, "closecover",               actionUndefined},
  {HTTP_PUT, "haltcover",                actionUndefined},
  {HTTP_PUT, "opencover",                actionUndefined},
};

AlpacaAction DomeActions[] = {
  {HTTP_GET, "altitude",                 actionUndefined},
  {HTTP_GET, "athome",                   actionUndefined},
  {HTTP_GET, "atpark",                   actionUndefined},
  {HTTP_GET, "azimuth",                  actionUndefined},
  {HTTP_GET, "canfindhome",              actionUndefined},
  {HTTP_GET, "canpark",                  actionUndefined},
  {HTTP_GET, "cansetaltitude",           actionUndefined},
  {HTTP_GET, "cansetazimuth",            actionUndefined},
  {HTTP_GET, "cansetpark",               actionUndefined},
  {HTTP_GET, "cansetshutter",            actionUndefined},
  {HTTP_GET, "canslave",                 actionUndefined},
  {HTTP_GET, "cansyncazimuth",           actionUndefined},
  {HTTP_GET, "shutterstatus",            actionUndefined},
  {HTTP_GET, "slaved",                   actionUndefined},
  {HTTP_PUT, "slaved",                   actionUndefined},
  {HTTP_GET, "slewing",                  actionUndefined},
  {HTTP_PUT, "abortslew",                actionUndefined},
  {HTTP_PUT, "closeshutter",             actionUndefined},
  {HTTP_PUT, "findhome",                 actionUndefined},
  {HTTP_PUT, "openshutter",              actionUndefined},
  {HTTP_PUT, "park",                     actionUndefined},
  {HTTP_PUT, "setpark",                  actionUndefined},
  {HTTP_PUT, "slewtoaltitude",           actionUndefined},
  {HTTP_PUT, "slewtoazimuth",            actionUndefined},
  {HTTP_PUT, "synctoazimuth",            actionUndefined},
};

AlpacaAction FilterWheelActions[] = {
  {HTTP_GET, "focusoffsets",             actionUndefined},
  {HTTP_GET, "names",                    actionUndefined},
  {HTTP_GET, "position",                 actionUndefined},
  {HTTP_PUT, "position",                 actionUndefined},
};

AlpacaAction FocuserActions[] = {
  {HTTP_GET, "absolute",                 actionUndefined},
  {HTTP_GET, "ismoving",                 actionUndefined},
  {HTTP_GET, "maxincrement",             actionUndefined},
  {HTTP_GET, "maxstep",                  actionUndefined},
  {HTTP_GET, "position",                 actionUndefined},
  {HTTP_GET, "stepsize",                 actionUndefined},
  {HTTP_GET, "tempcomp",                 actionUndefined},
  {HTTP_PUT, "tempcomp",                 actionUndefined},
  {HTTP_GET, "tempcompavailable",        actionUndefined},
  {HTTP_GET, "temperature",              actionUndefined},
  {HTTP_PUT, "halt",                     actionUndefined},
  {HTTP_PUT, "move",                     actionUndefined},
};

AlpacaAction ObservingConditionsActions[] = {
  {HTTP_GET, "averageperiod",            actionUndefined},
  {HTTP_PUT, "averageperiod",            actionUndefined},
  {HTTP_GET, "cloudcover",               actionUndefined},
  {HTTP_GET, "dewpoint",                 actionUndefined},
  {HTTP_GET, "humidity",                 actionUndefined},
  {HTTP_GET, "pressure",                 actionUndefined},
  {HTTP_GET, "rainrate",                 actionUndefined},
  {HTTP_GET, "skybrightness",            actionUndefined},
  {HTTP_GET, "skyquality",               actionUndefined},
  {HTTP_GET, "skytemperature",           actionUndefined},
  {HTTP_GET, "starfwhm",                 actionUndefined},
  {HTTP_GET, "temperature",              actionUndefined},
  {HTTP_GET, "winddirection",            actionUndefined},
  {HTTP_GET, "windgust",                 actionUndefined},
  {HTTP_GET, "windspeed",                actionUndefined},
  {HTTP_PUT, "refresh",                  actionUndefined},
  {HTTP_GET, "sensordescription",        actionUndefined},
  {HTTP_GET, "timesincelastupdate",      actionUndefined},
};

AlpacaAction RotatorActions[] = {
  {HTTP_GET, "canreverse",               actionUndefined},
  {HTTP_GET, "ismoving",                 actionUndefined},
  {HTTP_GET, "mechanicalposition",       actionUndefined},
  {HTTP_GET, "position",                 actionUndefined},
  {HTTP_GET, "reverse",                  actionUndefined},
  {HTTP_PUT, "reverse",                  actionUndefined},
  {HTTP_GET, "stepsize",                 actionUndefined},
  {HTTP_GET, "targetposition",           actionUndefined},
  {HTTP_PUT, "halt",                     actionUndefined},
  {HTTP_PUT, "move",                     actionUndefined},
  {HTTP_PUT, "moveabsolute",             actionUndefined},
  {HTTP_PUT, "movemechanical",           actionUndefined},
  {HTTP_PUT, "sync",                     actionUndefined},
};

AlpacaAction SafetyMonitorActions[] = {
  {HTTP_GET, "issafe",                   actionUndefined},
};

AlpacaAction SwitchActions[] = {
  {HTTP_GET, "maxswitch",                actionUndefined},
  {HTTP_GET, "canwrite",                 actionUndefined},
  {HTTP_GET, "getswitch",                actionUndefined},
  {HTTP_GET, "getswitchdescription",     actionUndefined},
  {HTTP_GET, "getswitchname",            actionUndefined},
  {HTTP_GET, "getswitchvalue",           actionUndefined},
  {HTTP_GET, "minswitchvalue",           actionUndefined},
  {HTTP_GET, "maxswitchvalue",           actionUndefined},
  {HTTP_PUT, "setswitch",                actionUndefined},
  {HTTP_PUT, "setswitchname",            actionUndefined},
  {HTTP_PUT, "setswitchvalue",           actionUndefined},
  {HTTP_GET, "switchstep",               actionUndefined},
};

AlpacaAction TelescopeActions[] = {
  {HTTP_GET, "alignmentmode",            actionUndefined},
  {HTTP_GET, "altitude",                 actionUndefined},
  {HTTP_GET, "aperturearea",             actionUndefined},
  {HTTP_GET, "aperturediameter",         actionUndefined},
  {HTTP_GET, "athome",                   actionUndefined},
  {HTTP_GET, "atpark",                   actionUndefined},
  {HTTP_GET, "azimuth",                  actionUndefined},
  {HTTP_GET, "canfindhome",              actionUndefined},
  {HTTP_GET, "canpark",                  actionUndefined},
  {HTTP_GET, "canpulseguide",            actionUndefined},
  {HTTP_GET, "cansetdeclinationrate",    actionUndefined},
  {HTTP_GET, "cansetguiderates",         actionUndefined},
  {HTTP_GET, "cansetpark",               actionUndefined},
  {HTTP_GET, "cansetpierside",           actionUndefined},
  {HTTP_GET, "cansetrightascensionrate", actionUndefined},
  {HTTP_GET, "cansettracking",           actionUndefined},
  {HTTP_GET, "canslew",                  actionUndefined},
  {HTTP_GET, "canslewaltaz",             actionUndefined},
  {HTTP_GET, "canslewaltazasync",        actionUndefined},
  {HTTP_GET, "canslewasync",             actionUndefined},
  {HTTP_GET, "cansync",                  actionUndefined},
  {HTTP_GET, "cansyncaltaz",             actionUndefined},
  {HTTP_GET, "canunpark",                actionUndefined},
  {HTTP_GET, "declination",              actionUndefined},
  {HTTP_GET, "declinationrate",          actionUndefined},
  {HTTP_PUT, "declinationrate",          actionUndefined},
  {HTTP_GET, "doesrefraction",           actionUndefined},
  {HTTP_PUT, "doesrefraction",           actionUndefined},
  {HTTP_GET, "equatorialsystem",         actionUndefined},
  {HTTP_GET, "focallength",              actionUndefined},
  {HTTP_GET, "guideratedeclination",     actionUndefined},
  {HTTP_PUT, "guideratedeclination",     actionUndefined},
  {HTTP_GET, "guideraterightascension",  actionUndefined},
  {HTTP_PUT, "guideraterightascension",  actionUndefined},
  {HTTP_GET, "ispulseguiding",           actionUndefined},
  {HTTP_GET, "rightascension",           actionUndefined},
  {HTTP_GET, "rightascensionrate",       actionUndefined},
  {HTTP_PUT, "rightascensionrate",       actionUndefined},
  {HTTP_GET, "sideofpier",               actionUndefined},
  {HTTP_PUT, "sideofpier",               actionUndefined},
  {HTTP_GET, "siderealtime",             actionUndefined},
  {HTTP_GET, "siteelevation",            actionUndefined},
  {HTTP_PUT, "siteelevation",            actionUndefined},
  {HTTP_GET, "sitelatitude",             actionUndefined},
  {HTTP_PUT, "sitelatitude",             actionUndefined},
  {HTTP_GET, "sitelongitude",            actionUndefined},
  {HTTP_PUT, "sitelongitude",            actionUndefined},
  {HTTP_GET, "slewing",                  actionUndefined},
  {HTTP_GET, "slewsettletime",           actionUndefined},
  {HTTP_PUT, "slewsettletime",           actionUndefined},
  {HTTP_GET, "targetdeclination",        actionUndefined},
  {HTTP_PUT, "targetdeclination",        actionUndefined},
  {HTTP_GET, "targetrightascension",     actionUndefined},
  {HTTP_PUT, "targetrightascension",     actionUndefined},
  {HTTP_GET, "tracking",                 actionUndefined},
  {HTTP_PUT, "tracking",                 actionUndefined},
  {HTTP_GET, "trackingrate",             actionUndefined},
  {HTTP_PUT, "trackingrate",             actionUndefined},
  {HTTP_GET, "trackingrates",            actionUndefined},
  {HTTP_GET, "utcdate",                  actionUndefined},
  {HTTP_PUT, "utcdate",                  actionUndefined},
  {HTTP_PUT, "abortslew",                actionUndefined},
  {HTTP_GET, "axisrates",                actionUndefined},
  {HTTP_GET, "canmoveaxis",              actionUndefined},
  {HTTP_GET, "destinationsideofpier",    actionUndefined},
  {HTTP_PUT, "findhome",                 actionUndefined},
  {HTTP_PUT, "moveaxis",                 actionUndefined},
  {HTTP_PUT, "park",                     actionUndefined},
  {HTTP_PUT, "pulseguide",               actionUndefined},
  {HTTP_PUT, "setpark",                  actionUndefined},
  {HTTP_PUT, "slewtoaltaz",              actionUndefined},
  {HTTP_PUT, "slewtoaltazasync",         actionUndefined},
  {HTTP_PUT, "slewtocoordinates",        actionUndefined},
  {HTTP_PUT, "slewtocoordinatesasync",   actionUndefined},
  {HTTP_PUT, "slewtotarget",             actionUndefined},
  {HTTP_PUT, "slewtotargetasync",        actionUndefined},
  {HTTP_PUT, "synctoaltaz",              actionUndefined},
  {HTTP_PUT, "synctocoordinates",        actionUndefined},
  {HTTP_PUT, "synctotarget",             actionUndefined},
  {HTTP_PUT, "unpark",                   actionUndefined},
};


AlpacaAction* ActionGroup[] = {
  CameraActions,
  CoverCalibratorActions,
  DomeActions,
  FilterWheelActions,
  FocuserActions,
  ObservingConditionsActions,
  RotatorActions,
  SafetyMonitorActions,
  SwitchActions,
  TelescopeActions
};


ErrorInfo AlpacaError[] = {
  {0x400, "Method or property not implemented"},
  {0x401, "Invalid value"},
  {0x402, "Value not set"}, 
  {0x407, "Not connected"},
  {0x408, "Invalid while parked"},
  {0x409, "Invalid while slaved"},  
  {0x40B, "Invalid operation"},
  {0x40C, "Action not implemented"}
};


enum ReturnStatus {
  SUCCESS = 0,
  FAILURE = -1
};


// Constructors:
AlpacaDevice::AlpacaDevice(AlpacaDeviceType device_type, int device_number, char *dns_name) {
  this->device_type   = device_type;
	this->device_number = device_number;
  this->actions       = ActionGroup[device_type];
  this->num_actions   = sizeof(this->actions)/sizeof(this->actions[0]);  
  strcpy(this->dns_name, dns_name);  
}
AlpacaDevice::~AlpacaDevice() {}


void AlpacaDevice::begin(int alpaca_port) {
  this->alpaca_port = alpaca_port;
  this->begin();
}

void AlpacaDevice::begin() {
  Serial.println('Starting Alpaca device');
  this->alpaca_port   = alpaca_port;

  sprintf(this->base_url, "/api/v%i/%s/i", ALPACA_API_VERSION, device_type, device_number);
  Serial.print("Base url: ");
  Serial.println(this->base_url);

  WebServer server(alpaca_port);
  this->server = &server;
    
  // Set up mDNS responder:
  if (MDNS.begin(this->dns_name)) {
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("http", "tcp", alpaca_port);    
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  
  this->server->begin();

}


void actionUndefined(void){
  Serial.println("Not implemented");
}

int AlpacaDevice::parseRequest(){
  this->requestParameters.clear();
  if (this->server->hasArg("plain") == false) {
    //handle error here
    return FAILURE;
  }
  String request = this->server->arg("plain");
  deserializeJson(this->requestParameters, request);
  return SUCCESS;
}

int AlpacaDevice::fetchRequestParameter(const char* parameterName, char* parameterValue){
  if (this->requestParameters.containsKey(parameterName) == false){
    return FAILURE;
  }
  strcpy(parameterValue, this->requestParameters[parameterName]);
  return SUCCESS;
}





// Send GET request NOMINAL response:
void AlpacaDevice::sendGetRequestNominalResponse(
  const char *strValue,
  ClientTransactionID clientTransactionID,  /* (optional in call;  default provided by library) */
  ServerTransactionID serverTransactionID   /* (optional in call;  default provided by library) */
){
  this->sendGetResponse({
    /* strValue             */ strValue,
    /* clientTransactionID  */ clientTransactionID,
    /* serverTransactionID  */ serverTransactionID,
    /* errorID              */ ALPACA_NO_ERROR_ERROR_ID,
    /* errorDesc            */ ALPACA_NO_ERROR_ERROR_DESC
  });
}


// Send GET request ERROR response:
void AlpacaDevice::sendGetRequestErrorResponse(
  ErrorID errorID,
  const char* errorDesc,
  const char* strValue,                      /* (optional in call;  default provided by library) */
  ClientTransactionID clientTransactionID,   /* (optional in call;  default provided by library) */
  ServerTransactionID serverTransactionID    /* (optional in call;  default provided by library) */
){
  this->sendGetResponse(AlpacaGetResponse {
    /* strValue             */ strValue,
    /* clientTransactionID  */ clientTransactionID,
    /* serverTransactionID  */ serverTransactionID,
    /* errorID              */ errorID,
    /* errorDesc            */ errorDesc
  });
}

// Send GET request ERROR response by error ID for pre-defined Alpaca errors:
void AlpacaDevice::sendGetRequestErrorResponse(
  AlpacaErrorIdx alpacaErrorIdx,
  const char* strValue,                      /* (optional in call;  default provided by library) */
  ClientTransactionID clientTransactionID,   /* (optional in call;  default provided by library) */
  ServerTransactionID serverTransactionID    /* (optional in call;  default provided by library) */
){
  this->sendGetResponse({
    /* strValue             */ strValue,
    /* clientTransactionID  */ clientTransactionID,
    /* serverTransactionID  */ serverTransactionID,
    /* errorID              */ AlpacaError[alpacaErrorIdx].errorID,
    /* errorDesc            */ AlpacaError[alpacaErrorIdx].errorDesc
  });
}


void AlpacaDevice::sendGetResponse(AlpacaGetResponse alpacaGetResponse){
  String response = "{";
  response += "\"Value\": " + String(alpacaGetResponse.strValue);
  if (alpacaGetResponse.clientTransactionID != ALPACA_NO_CLIENT_TRANSACTION_ID){
    response += ",\"ClientTransactionID\": " + String((int)alpacaGetResponse.clientTransactionID);    
  }
  if (alpacaGetResponse.serverTransactionID != ALPACA_NO_SERVER_TRANSACTION_ID){
    response += ",\"ServerTransactionID\": " + String((int)alpacaGetResponse.serverTransactionID);    
  }
  response += ",\"ErrorNumber\": " + String((int)alpacaGetResponse.errorID);
  response += ",\"ErrorMessage\": " + String((int)alpacaGetResponse.errorDesc);
  response += "}";
  strncpy(this->response_buffer, response.c_str(), ALPACA_RESPONSE_BUFFER_LEN);
  this->server->send(200, "application/json", this->response_buffer);
}



void AlpacaDevice::RegisterServerons(){
  for (int i=0; i<this->num_actions; i++){
    this->server->on(
      this->actions[i].name, 
      this->actions[i].http_method_type, 
      this->actions[i].fcn
    );
  }
  this->server->onNotFound([this](){ this->pageNotFound();});
}


void AlpacaDevice::pageNotFound() {
  this->server->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void AlpacaDevice::setAction(AlpacaCommonAction                action_idx,  void (*fcn)(void)) {
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaCameraAction                action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_CAMERA);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaCoverCalibratorAction       action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_COVER_CALIBRATOR);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaDomeAction                  action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_DOME);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaFilterWheelAction           action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_FILTER_WHEEL);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaFocuserAction               action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_FOCUSER);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaObservingConditionsAction   action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_OBSERVING_CONDITIONS);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaRotatorActions              action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_ROTATOR);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaSafetyMonitorAction         action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_SAFETY_MONITOR);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaSwitchAction                action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_SWITCH);
  this->setAction(action_idx, fcn);
}

void AlpacaDevice::setAction(AlpacaTelescopeAction             action_idx,  void (*fcn)(void)){
  assert (this->device_type == DEVICE_TYPE_TELESCOPE);
  this->setAction(action_idx, fcn);
}


void AlpacaDevice::setAction(int action_idx, void (*fcn)(void)){
  assert (action_idx < this->num_actions);
  this->actions[action_idx].fcn = fcn;
}