#include <IRremote.h>
#include <IRremoteInt.h>
#include <avr/pgmspace.h>

IRsend irsend;

const PROGMEM uint16_t sony_old[22] = {550, 1450, 550, 450, 600, 1450, 550, 450, 600, 1500, 550, 450, 550, 500, 550, 500, 550, 500, 550, 450, 600};
const PROGMEM uint16_t grundig1[22] = {550, 2600, 550, 450, 600, 450, 550, 500, 500, 500, 550, 500, 500, 500, 550, 500, 550, 500, 550, 450, 550};

void setup() {
}

void loop() {
  sendSony(0xA90, 12); //SONY, NEC
  sendNEC(0x20DF10EF); //LG, SAMSUNG, VIZIO, HISENSE, KENWOOD, PROSCAN, ZENITH
  sendRC5(0x80C); //PHILIPS, MAGNAVOX, GRUNDIG, MEDION, SILVERCREST
  sendRC5(0xC); //PHILIPS, GRUNDIG, MEDION, WATSON, AEG, SILVERCREST, THOMSON, LG
  sendRC5(0x84C); //TELEFUNKEN, SHARP, SANYO
  sendRC5(0x4C); //SHARP, TOSHIBA, MEDION
  irsend.sendSharpRaw(0x425D, 15); //SHARP
  pause();
  sendRC5(0x44C); //PANASONIC
  sendSony(0xF50, 12); //SONY
  for (int i = 0; i < 2; ++i) {
    sendNEC(0x4CB340BF); //OPTOMA PROJECTOR
    sendNEC(0xC1AA09F6); //EPSON PROJECTOR/TV
    sendNEC(0x189728D7); //NEC PROJECTOR
  }
  for (int i = 0; i < 2; ++i) {
    irsend.sendPanasonic(0x4004, 0x100BCBD); //PANASONIC TV1
    delay(40);
  }
  send(grundig1, 22);
  pause();
  irsend.sendJVC(0xC0E8, 16, 0); //JVC, THOMSON
  pause();
  sendNEC(0x2FD48B7); //TOSHIBA, HISENSE, SAMSUNG, APEX, AKAI
  sendSamsung(0x6060F00F); //AIKO
  sendNEC(0x86C6807F); //ACER
  sendSamsung(0xE0E040BF); //SAMSUNG TV1
  sendSamsung(0xE0E019E6); //SAMSUNG TV2
  sendSamsung(0x1010D02F); //HAIER
  sendNEC(0x6F900FF); //BENQ
  sendNEC(0x1FE41BE); //TELEFUNKEN
  irsend.sendRC6(0x1000C, 20); //PHILIPS RC6
  pause();
  irsend.sendRC6(0xC, 20); //PHILIPS RC6
  pause();
  irsend.sendSharpRaw(0x5A5D, 15); //SHARP
  pause();
  sendNEC(0x7B6B4FB0); //LG
  sendNEC(0x986718E7); //MEDION
  sendNEC(0x4B36E21D); //ONKYO
  sendNEC(0x4B36D32C); //ONKYO
  irsend.sendJVC(0xC5E8, 16, 0); //JVC A/V
  pause();
  sendNEC(0x189710EF); //NEC PROJECTOR ON
  sendNEC(0x1897639C); //NEC PROJECTOR [INPUT=SVIDEO] (No one uses S-video)
  sendNEC(0xAB500FF); //YAMAHA
  sendNEC(0x1FE48B7); //FUJITSU
  sendNEC(0x3E060FC0); //AIWA
  sendRC5(0xC3D); //GRUNDIG FINE ARTS
  sendNEC(0xFB38C7); //GRUNDIG, MEDION
  sendXSAT(0x14, 0x15); //GRUNDIG, XSAT
  sendRC5(0x301); //SHARP
  sendRC5(0xB01); //SHARP
  sendSamsung(0x909C837); //SAMSUNG TV3
  sendRC5(0x9CC); //PHILIPS ITV
  sendNEC(0x20DFBE41); //LG TV3
  sendSamsung(0x909040BF); //SAMSUNG
  sendNEC(0x55AA38C7); //PIONEER, SHARP
  irsend.sendJVC(0xF8EB, 16, 0); //SONY, JVC
  pause();
  send(sony_old, 22); //SONY
  sendNEC(0x1CE348B7); //SANYO
  sendNEC(0x1CE338C7); //HITACHI, SANYO, GRUNDIG
  sendNEC(0x10EFEB14); //AOC
  sendNEC(0x18E710EF); //NEC TV
  sendNEC(0xAF5FC03); //DENON
  sendNEC(0xBD807F); //PHILIPS LCD MONITOR
  sendNEC(0xC18F50AF); //VIEWSONIC
  sendNEC(0x8C73817E); //LENOVO
  sendNEC(0x38C7AC0A); //MALATA
  sendNEC(0xDE010FC0); //AIWA
  sendNEC(0xFD00FF); //TELEFUNKEN
  sendNEC(0x8E7152AD); //TOSHIBA
}

void pause() {
  delay(100);
}

void send(unsigned int const codes[], int len) {
  unsigned int buf[len];
  for (int i = 0; i < len; ++i) {
    buf[i] = pgm_read_word_near(codes + i);
  }
  irsend.sendRaw(buf, len, 38);
  pause();
}

void sendNEC(unsigned long code) {
  irsend.sendNEC(code, 32);
  pause();
}

void sendRC5(unsigned int code) {
  irsend.sendRC5(code, 12);
  pause();
}

void sendSony(unsigned int code, int bits) {
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(code, bits);
    delay(40);
  }
  pause();
}

void sendSamsung(unsigned long code) {
  irsend.sendSAMSUNG(code, 32);
  pause();
}

void sendXSAT(unsigned char adr, unsigned char cmd) {
  irsend.sendXSAT(adr, cmd);
  pause();
}

