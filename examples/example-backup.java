//back up
console.log("test");
//events
  server.on("/sensors.html", loadSensors);
  server.on("/tgls.html", tgl);
  server.on("/tglt.html", tglt);
  server.on("/tgluv.html", tgluv);
  server.on("/ir.html", sendIR);

//loop
    digitalWrite(spin, ts);
    digitalWrite(tpin, tt);
    digitalWrite(uvpin, tuv);


//functions
void sendIR() {
  String o = "<script>window.open(\"sensors.html\",\"_self\")</script>";
  sendFile(200, "text/html", string2char(o), o.length());
  irsend.sendRaw(tv, 67, 38);
  Serial.println("Sent ir code");
}

void loadSensors() {
/*
  String s = F("<!Doctype html> <html> <head> <meta charset=\"utf-8\"> <title>Sonic Deauther - Sensors</title> <meta name=\"description\" content=\"Sonic Deauthenticator\"> <meta name=\"author\" content=\"Luigi Pizzolito & Stefan Kremser\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel=\"stylesheet\" href=\"style.css\"> </head> <body> <nav> <a href=\"apscan.html\">APs</a> <a href=\"stations.html\">Stations</a> <a href=\"attack.html\">Attacks</a> <a href=\"settings.html\">Settings</a> <a class=\"right\" href=\"info.html\">Info</a> <a class=\"right\" href=\"sensors.html\">Sensors</a> </nav> <div id=\"error\"></div> <div> <h2>Controls</h2> <br /> <center>");

    s += "<button class=\"button";
    s += ts;
    s += "\" onclick=\"tgl1()\">Toggle Sonic</button>";
    s += "<button class=\"button";
    s += tt;
    s += "\" onclick=\"tgl2()\">Toggle Torch</button>";
    s += "<button class=\"button";
    s += tuv;
    s += "\" onclick=\"tgl3()\">Toggle UV Torch</button>";

  s += F(" <br /> <br /> </center> </div> <div> <h2>Sensors</h2> <br /> <h1 class=\"header\">Temperature</h1> <p>");
  s += "20" ;
  s += F(" *C</p> <h1 class=\"header\">Light</h1> <p>");
  s += "light reading";
  s += F("</p> <h1 class=\"header\">Motion</h1> <p>");
  s += "motion reading";
  s += F(" mov/min</p> <h1 class=\"header\">Battery Voltage</h1> <p>");
  int reading = analogRead(A0);
  float voltage = reading * 3.3;
  voltage /= 1024.0;
  s += voltage;
  s += F(" V</p><h1 class=\"header\">Uptime</h1></p>");
  s += millis() / 1000;
  s += F(" Seconds</p> </div> <script> function tgl1() {window.open(\"tgls.html\", \"_self\");} function tgl2() {window.open(\"tglt.html\", \"_self\");} function tgl3() {window.open(\"tgluv.html\", \"_self\");} </script> <div id=\"copyright\"> version 1.4<br /> Luigi Pizzolito & Stefan Kremser<br /> <a href=\"https://github.com/spacehuhn\" target=\"_blank\">github.com/spacehuhn</a><br /> <a href=\"http://luigiptardis.site90.com/\">Luigi's Website</a> </div> </body> </html>");
  Serial.println(s);
  char sf[s.length()+1];
  s.toCharArray(sf, s.length()+1);
  Serial.println(sf);
  //error somewhere here
  sendFile(200, "text/html", sf, sizeof(sf));
  */
  /*
    sendFile(200, "text/html", sensors_HTML, sizeof(sensors_HTML));
  */

  Serial.println("sensor opened");
}


char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

void tgl() {
  Serial.println("tgls!");
  ts = !ts;
  String o = "<script>window.open(\"sensors.html\",\"_self\")</script>";
  sendFile(200, "text/html", string2char(o), o.length());
}
void tglt() {
  Serial.println("tglt!");
  tt = !tt;
  String o = "<script>window.open(\"sensors.html\",\"_self\")</script>";
  sendFile(200, "text/html", string2char(o), o.length());
}
void tgluv() {
  Serial.println("tgluv!");
  tuv = !tuv;
  String o = "<script>window.open(\"sensors.html\",\"_self\")</script>";
  sendFile(200, "text/html", string2char(o), o.length());
}
