#include <IRremoteESP8266.h>

int RECV_PIN = 14;

IRrecv irrecv(RECV_PIN);
IRsend irsend(4);

boolean recording = true;
decode_results results;

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (recording) {
    if (irrecv.decode(&results)) {
      Serial.println("IR code recorded!");
      //irrecv.resume(); // Receive the next value
      Serial.print("Recorded ");
      Serial.print(results.rawlen);
      Serial.println(" intervals.");
      recording = false;
    }
  } else {
    // replay mode
    Serial.println("Sending recorded IR signal!");
    irsend.begin();
    irsend.sendRaw((unsigned int*) results.rawbuf, results.rawlen, 38);
    delay(2000);
  }
}
