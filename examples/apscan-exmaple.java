//initializers
#include "APScan.h"
APScan apScan;

//on data.h
const char data_apscanHTML[] PROGMEM = {};
const char data_apscanJS[] PROGMEM = {};

//on setup
server.on("/apscan.html", loadAPScanHTML);
server.on("/js/apscan.js", loadAPScanJS);
server.on("/APScan.json", startAPScan);
server.on("/APScanResults.json", sendAPResults);



//call function
void loadAPScanHTML() {
  sendFile(200, "text/html", data_apscanHTML, sizeof(data_apscanHTML));
}

void loadAPScanJS() {
  sendFile(200, "text/javascript", data_apscanJS, sizeof(data_apscanJS));
}


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