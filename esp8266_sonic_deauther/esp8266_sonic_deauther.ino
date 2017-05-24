/*
  ====================================================================
       Copyright (c) 2017 Stefan Kremser & Luigi Pizzolito
              github.com/spacehuhn https://github.com/Gangster45671
  ====================================================================
*/

#include <Arduino.h>
#include <IRremoteESP8266.h>

//IR Stuff
unsigned int tv[67] = {9000, 4600, 550, 650, 550, 600, 550, 650, 550, 650, 550, 600, 550, 650, 500, 1750, 550, 650, 550, 1750, 600, 1750, 550, 1750, 550, 1750, 550, 1750, 550, 1750, 500, 700, 550, 1750, 500, 700, 550, 1750, 550, 650, 550, 600, 550, 1750, 550, 600, 550, 650, 500, 700, 550, 1750, 550, 650, 550, 1750, 500, 1800, 550, 650, 500, 1750, 550, 1750, 550, 1750, 550}; // UNKNOWN 1A2EEC3B
int RECV_PIN = 14;
IRrecv irrecv(RECV_PIN);
IRsend irsend(4);
boolean recording = false;
decode_results results;
irparams_t save;        // A place to copy the interrupt state while decoding.
String IRccode;

//internet stuff
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

//I/O Stuff
#define spin 12
#define tpin 13
#define uvpin 15
bool ts = false;
bool tt = false;
bool tuv = false;

#define resetPin 0 /* <-- comment out or change if you need GPIO 4 for other purposes */
//#define USE_DISPLAY /* <-- uncomment that if you want to use the display */


String wifiMode = "";
String attackMode = "";
String scanMode = "SCAN";

bool warning = true;

extern "C" {
#include "user_interface.h"
}

ESP8266WebServer server(80);

#include <EEPROM.h>
#include "data.h"
#include "NameList.h"
#include "APScan.h"
#include "ClientScan.h"
#include "Attack.h"
#include "Settings.h"
#include "SSIDList.h"

/* ========== DEBUG ========== */
const bool debug = true;
/* ========== DEBUG ========== */

NameList nameList;

APScan apScan;
ClientScan clientScan;
Attack attack;
Settings settings;
SSIDList ssidList;

void sniffer(uint8_t *buf, uint16_t len) {
  clientScan.packetSniffer(buf, len);
}

void startWifi() {
  Serial.println("\nStarting WiFi AP:");
  WiFi.mode(WIFI_STA);
  wifi_set_promiscuous_rx_cb(sniffer);
  WiFi.softAP((const char*)settings.ssid.c_str(), (const char*)settings.password.c_str(), settings.apChannel, settings.ssidHidden); //for an open network without a password change to:  WiFi.softAP(ssid);
  Serial.println("SSID     : '" + settings.ssid + "'");
  Serial.println("Password : '" + settings.password + "'");
  Serial.println("-----------------------------------------------");
  if (settings.password.length() < 8) Serial.println("WARNING: password must have at least 8 characters!");
  if (settings.ssid.length() < 1 || settings.ssid.length() > 32) Serial.println("WARNING: SSID length must be between 1 and 32 characters!");
  wifiMode = "ON";
}

void stopWifi() {
  Serial.println("stopping WiFi AP");
  Serial.println("-----------------------------------------------");
  WiFi.disconnect();
  wifi_set_opmode(STATION_MODE);
  wifiMode = "OFF";
}

void loadIndexHTML() {
  if (warning) {
    sendFile(200, "text/html", data_indexHTML, sizeof(data_indexHTML));
  } else {
    sendFile(200, "text/html", data_apscanHTML, sizeof(data_apscanHTML));
  }
}

void loadAPScanHTML() {
  warning = false;
  sendFile(200, "text/html", data_apscanHTML, sizeof(data_apscanHTML));
}
void loadStationsHTML() {
  sendFile(200, "text/html", data_stationHTML, sizeof(data_stationHTML));
}
void loadAttackHTML() {
  sendFile(200, "text/html", data_attackHTML, sizeof(data_attackHTML));
}
void loadSettingsHTML() {
  sendFile(200, "text/html", data_settingsHTML, sizeof(data_settingsHTML));
}
void load404() {
  sendFile(200, "text/html", data_error404, sizeof(data_error404));
}
void loadInfoHTML() {
  sendFile(200, "text/html", data_infoHTML, sizeof(data_infoHTML));
}
void loadLicense() {
  sendFile(200, "text/plain", data_license, sizeof(data_license));
}

void loadFunctionsJS() {
  sendFile(200, "text/javascript", data_functionsJS, sizeof(data_functionsJS));
}
void loadAPScanJS() {
  sendFile(200, "text/javascript", data_apscanJS, sizeof(data_apscanJS));
}
void loadStationsJS() {
  sendFile(200, "text/javascript", data_stationsJS, sizeof(data_stationsJS));
}
void loadAttackJS() {
  sendFile(200, "text/javascript", data_attackJS, sizeof(data_attackJS));
}
void loadSettingsJS() {
  sendFile(200, "text/javascript", data_settingsJS, sizeof(data_settingsJS));
}

void loadStyle() {
  sendFile(200, "text/css;charset=UTF-8", data_styleCSS, sizeof(data_styleCSS));
}


void startWiFi(bool start) {
  if (start) startWifi();
  else stopWifi();
  clientScan.clearList();
}

//============IR=============

void IRmanage() {
  if (recording) {
    if (irrecv.decode(&results, &save)) {
      Serial.println("IR code recorded!");
      //irrecv.resume(); // Receive the next value
      Serial.print("Recorded ");
      Serial.print(results.rawlen);
      Serial.println(" intervals.");
      dumpCode(&results);
      recording = false;
    }
  }
}

void dumpCode(decode_results *results) {
  Serial.println(results->value, HEX);
  IRccode = (results->value);
}

void IRrecord() {
  recording = true;
  server.send ( 200, "text/json", "true");
}

void sendIR() {
  if (!recording) {
    Serial.println("Sending recorded IR signal!");
    irsend.begin();
    irsend.sendRaw((unsigned int*) results.rawbuf, results.rawlen, 38);
    server.send ( 200, "text/json", "true");
    irrecv.enableIRIn();
  }
}

void sendTV() {
  irsend.begin();
  irsend.sendRaw(tv, 67, 38);
  server.send ( 200, "text/json", "true");
  Serial.println("true");
  irrecv.enableIRIn();
}

//==========Sensors==========
//call function
void loadSensorsHTML() {
  sendFile(200, "text/html", data_SensorsHTML, sizeof(data_SensorsHTML));
}

void loadSensorsJS() {
  sendFile(200, "text/html", data_SensorsJS, sizeof(data_SensorsJS));
}

void updateSensors() {
  //get sensor data
  //dummy
  int temp = 20;
  int ligth = 100;
  int mot = 40;

  //vbat
  int reading = analogRead(A0);
  float vbat = reading * 3.3;
  vbat /= 1024.0;



  //parse
  String page = String(temp);
  page += ",";
  page += ligth;
  page += ",";
  page += mot;
  page += ",";
  page += vbat;
  page += ",";
  page += millis();
  //ir status
  page += ",";
  page += recording;
  page += ",";
  page += IRccode;
  server.send ( 200, "text/json", page);
}

void tgl1() {
  //do cool stuff
  ts = !ts;
  digitalWrite(spin, ts);
  server.send ( 200, "text/json", "true"+String(ts)); //tell server it worked.
  Serial.println("true"+String(ts));
}

void tgl2() {
  //do cool stuff
  tt = !tt;
  digitalWrite(tpin, tt);
  server.send ( 200, "text/json", "true"+String(tt)); //tell server it worked.
  Serial.println("true"+String(tt));
}

void tgl3() {
  //do cool stuff
  tuv = !tuv;
  digitalWrite(uvpin, tuv);
  server.send ( 200, "text/json", "true"+String(tuv)); //tell server it worked.
  Serial.println("true"+String(tuv));
}






//==========AP-Scan==========
void startAPScan() {
  scanMode = "scanning...";

  if (apScan.start()) {


    server.send ( 200, "text/json", "true");
    attack.stopAll();
    scanMode = "SCAN";
  }
}

void sendAPResults() {
  apScan.sendResults();
}

void selectAP() {
  if (server.hasArg("num")) {
    apScan.select(server.arg("num").toInt());
    server.send( 200, "text/json", "true");
    attack.stopAll();
  }
}

//==========Client-Scan==========
void startClientScan() {
  if (server.hasArg("time") && apScan.getFirstTarget() > -1 && !clientScan.sniffing) {
    server.send(200, "text/json", "true");
    clientScan.start(server.arg("time").toInt());
    attack.stopAll();
  } else server.send( 200, "text/json", "Error: no selected access point");
}

void sendClientResults() {
  clientScan.send();
}
void sendClientScanTime() {
  server.send( 200, "text/json", (String)settings.clientScanTime );
}

void selectClient() {
  if (server.hasArg("num")) {
    clientScan.select(server.arg("num").toInt());
    attack.stop(0);
    server.send( 200, "text/json", "true");
  }
}

void addClientFromList() {
  if (server.hasArg("num")) {
    int _num = server.arg("num").toInt();
    clientScan.add(nameList.getMac(_num));

    server.send( 200, "text/json", "true");
  } else server.send( 200, "text/json", "false");
}

void setClientName() {
  if (server.hasArg("id") && server.hasArg("name")) {
    if (server.arg("name").length() > 0) {
      nameList.add(clientScan.getClientMac(server.arg("id").toInt()), server.arg("name"));
      server.send( 200, "text/json", "true");
    }
    else server.send( 200, "text/json", "false");
  }
}

void deleteName() {
  if (server.hasArg("num")) {
    int _num = server.arg("num").toInt();
    nameList.remove(_num);
    server.send( 200, "text/json", "true");
  } else server.send( 200, "text/json", "false");
}

void clearNameList() {
  nameList.clear();
  server.send( 200, "text/json", "true" );
}

void editClientName() {
  if (server.hasArg("id") && server.hasArg("name")) {
    nameList.edit(server.arg("id").toInt(), server.arg("name"));
    server.send( 200, "text/json", "true");
  } else server.send( 200, "text/json", "false");
}

void addClient() {
  if (server.hasArg("mac") && server.hasArg("name")) {
    String macStr = server.arg("mac");
    macStr.replace(":", "");
    Serial.println("add " + macStr + " - " + server.arg("name"));
    if (macStr.length() < 12 || macStr.length() > 12) server.send( 200, "text/json", "false");
    else {
      Mac _newClient;
      for (int i = 0; i < 6; i++) {
        const char* val = macStr.substring(i * 2, i * 2 + 2).c_str();
        uint8_t valByte = strtoul(val, NULL, 16);
        Serial.print(valByte, HEX);
        Serial.print(":");
        _newClient.setAt(valByte, i);
      }
      Serial.println();
      nameList.add(_newClient, server.arg("name"));
      server.send( 200, "text/json", "true");
    }
  }
}

//==========Attack==========
void sendAttackInfo() {
  attack.sendResults();
}

void startAttack() {
  if (server.hasArg("num")) {
    int _attackNum = server.arg("num").toInt();
    if (apScan.getFirstTarget() > -1 || _attackNum == 1 || _attackNum == 2) {
      attack.start(server.arg("num").toInt());
      server.send ( 200, "text/json", "true");
    } else server.send( 200, "text/json", "false");
  }
}

void addSSID() {
  ssidList.add(server.arg("name"));
  server.send( 200, "text/json", "true");
}

void cloneSSID() {
  ssidList.addClone(server.arg("name"));
  server.send( 200, "text/json", "true");
}

void deleteSSID() {
  ssidList.remove(server.arg("num").toInt());
  server.send( 200, "text/json", "true");
}

void randomSSID() {
  ssidList._random();
  server.send( 200, "text/json", "true");
}

void clearSSID() {
  ssidList.clear();
  server.send( 200, "text/json", "true");
}

void resetSSID() {
  ssidList.load();
  server.send( 200, "text/json", "true");
}

void saveSSID() {
  ssidList.save();
  server.send( 200, "text/json", "true");
}

void restartESP() {
  server.send( 200, "text/json", "true");
  ESP.reset();
}

//==========Settings==========
void getSettings() {
  settings.send();
}

void saveSettings() {
  if (server.hasArg("ssid")) settings.ssid = server.arg("ssid");
  if (server.hasArg("ssidHidden")) {
    if (server.arg("ssidHidden") == "false") settings.ssidHidden = false;
    else settings.ssidHidden = true;
  }
  if (server.hasArg("password")) settings.password = server.arg("password");
  if (server.hasArg("apChannel")) {
    if (server.arg("apChannel").toInt() >= 1 && server.arg("apChannel").toInt() <= 14) {
      settings.apChannel = server.arg("apChannel").toInt();
    }
  }
  if (server.hasArg("ssidEnc")) {
    if (server.arg("ssidEnc") == "false") settings.attackEncrypted = false;
    else settings.attackEncrypted = true;
  }
  if (server.hasArg("scanTime")) settings.clientScanTime = server.arg("scanTime").toInt();
  if (server.hasArg("timeout")) settings.attackTimeout = server.arg("timeout").toInt();
  if (server.hasArg("deauthReason")) settings.deauthReason = server.arg("deauthReason").toInt();
  if (server.hasArg("packetRate")) settings.attackPacketRate = server.arg("packetRate").toInt();
  if (server.hasArg("apScanHidden")) {
    if (server.arg("apScanHidden") == "false") settings.apScanHidden = false;
    else settings.apScanHidden = true;
  }
  if (server.hasArg("useLed")) {
    if (server.arg("useLed") == "false") settings.useLed = false;
    else settings.useLed = true;
    attack.refreshLed();
  }
  if (server.hasArg("channelHop")) {
    if (server.arg("channelHop") == "false") settings.channelHop = false;
    else settings.channelHop = true;
  }
  if (server.hasArg("multiAPs")) {
    if (server.arg("multiAPs") == "false") settings.multiAPs = false;
    else settings.multiAPs = true;
  }

  settings.save();
  server.send( 200, "text/json", "true" );
}

void resetSettings() {
  settings.reset();
  server.send( 200, "text/json", "true" );
}


void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  pinMode(A0, INPUT);
  pinMode(spin, OUTPUT);
  pinMode(tpin, OUTPUT);
  pinMode(uvpin, OUTPUT);
  digitalWrite(spin, LOW);
  digitalWrite(tpin, LOW);
  digitalWrite(uvpin, LOW);
  if (debug) {
    delay(2000);
    Serial.println("\nStarting...\n");
  }

  attackMode = "START";
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  EEPROM.begin(4096);
  SPIFFS.begin();

  settings.load();
  if (debug) settings.info();
  nameList.load();
  ssidList.load();

#ifdef resetPin
  pinMode(resetPin, INPUT_PULLUP);
  if (digitalRead(resetPin) == LOW) settings.reset();
#endif

  startWifi();
  attack.stopAll();
  attack.generate();

  /* ========== Web Server ========== */

  /* HTML */
  server.onNotFound(load404);

  server.on("/", loadIndexHTML);
  server.on("/index.html", loadIndexHTML);
  server.on("/apscan.html", loadAPScanHTML);
  server.on("/stations.html", loadStationsHTML);
  server.on("/attack.html", loadAttackHTML);
  server.on("/settings.html", loadSettingsHTML);
  server.on("/info.html", loadInfoHTML);
  server.on("/license", loadLicense);

  /* Sensors */
  server.on("/sensors.html", loadSensorsHTML);
  server.on("/js/sensors.js", loadSensorsJS);
  server.on("/tgl1.json", tgl1);
  server.on("/tgl2.json", tgl2);
  server.on("/tgl3.json", tgl3);
  server.on("/update.json", updateSensors);
  /* IR Stuff */
  server.on("/ir.json", sendTV);
  server.on("/IRrcd.json", IRrecord);
  server.on("/IRsnd.json", sendIR);

  /* JS */
  server.on("/js/apscan.js", loadAPScanJS);
  server.on("/js/stations.js", loadStationsJS);
  server.on("/js/attack.js", loadAttackJS);
  server.on("/js/settings.js", loadSettingsJS);
  server.on("/js/functions.js", loadFunctionsJS);

  /* CSS */
  server.on ("/style.css", loadStyle);

  /* JSON */
  server.on("/APScanResults.json", sendAPResults);
  server.on("/APScan.json", startAPScan);
  server.on("/APSelect.json", selectAP);
  server.on("/ClientScan.json", startClientScan);
  server.on("/ClientScanResults.json", sendClientResults);
  server.on("/ClientScanTime.json", sendClientScanTime);
  server.on("/clientSelect.json", selectClient);
  server.on("/setName.json", setClientName);
  server.on("/addClientFromList.json", addClientFromList);
  server.on("/attackInfo.json", sendAttackInfo);
  server.on("/attackStart.json", startAttack);
  server.on("/settings.json", getSettings);
  server.on("/settingsSave.json", saveSettings);
  server.on("/settingsReset.json", resetSettings);
  server.on("/deleteName.json", deleteName);
  server.on("/clearNameList.json", clearNameList);
  server.on("/editNameList.json", editClientName);
  server.on("/addSSID.json", addSSID);
  server.on("/cloneSSID.json", cloneSSID);
  server.on("/deleteSSID.json", deleteSSID);
  server.on("/randomSSID.json", randomSSID);
  server.on("/clearSSID.json", clearSSID);
  server.on("/resetSSID.json", resetSSID);
  server.on("/saveSSID.json", saveSSID);
  server.on("/restartESP.json", restartESP);
  server.on("/addClient.json", addClient);

  server.begin();
  //irsend.begin();
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
  if (clientScan.sniffing) {
    if (clientScan.stop()) startWifi();
  } else {
    server.handleClient();
    attack.run();
    IRmanage();
  }

  if (Serial.available()) {
    String input = Serial.readString();
    if (input == "reset" || input == "reset\n" || input == "reset\r" || input == "reset\r\n") {
      settings.reset();
    }
  }


  //IRmanage();
}


