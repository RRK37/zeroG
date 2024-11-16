// This will have functions for the transceiver.
#include <Arduino.h>
#include <string.h>
#include "transceiver_uno.h"


// Constant values defining machine:
const int myADR = 3;

int dt = 40;

void pulse(bool x) {

    delay(0.45*dt);
    pinMode(11, INPUT);
    if (x) pinMode(11, OUTPUT);
    else pinMode(11, INPUT);
    delay(0.1*dt);
    pinMode(11, INPUT);
    delay(0.45*dt);

}

void zeroG_setup() {
    // Set up PWM stuff
    // set up Timer 2
    pinMode(11, OUTPUT);
    TCCR2A = _BV (COM2A0) | _BV(WGM21);  
    TCCR2B = _BV (CS20);   
    OCR2A =  209;  
    pinMode(11, INPUT);
    // Input pin set up.
    pinMode(A4, INPUT);
}

bool receiveACK(int ADR) {
  int tADR = 0;
  
  for (int x = 0; x < 5; x++){
    if (sample()) {
      delay(dt);
      if (sample()) {
        while(sample() == true) delay(0.001*dt);
          // Sense the ACK address, should be 000.
          if (sample()) tADR += 4; delay(dt); if (sample()) tADR += 2; delay(dt); if (sample()) tADR += 1; delay(dt);
          if (tADR != 0) return false;
          delay(dt);
          // Check if the correct address is sending the ACK
          if (sample()) tADR += 4; delay(dt); if (sample()) tADR += 2; delay(dt); if (sample()) tADR += 1; delay(dt);
          if (tADR == ADR) return true;
          else return false;
      }
    }
    delay(dt);
  }
  return false;
}

bool sendPacket(int ADR, int PID, int DAT) {

    // Do "helloo" sequence.
    pinMode(11, OUTPUT);
    delay(2.5*dt);
    pinMode(11, INPUT);
    delay(0.5*dt);

    // Send data.
    pulse(ADR & 4);
    pulse(ADR & 2);
    pulse(ADR & 1);
    pulse(PID & 8);
    pulse(PID & 4);
    pulse(PID & 2);
    pulse(PID & 1);
    pulse((ADR & 1)^(ADR & 2)^(ADR & 4)^(PID & 1)^(PID & 2)^(PID & 4)^(PID & 8)^(DAT & 1)^(DAT & 2)^(DAT & 4)^(DAT & 8)^(DAT & 16)^(DAT & 32)^(DAT & 64)^(DAT & 128)); // Calculate parity.
    pulse(DAT & 128);
    pulse(DAT & 64);
    pulse(DAT & 32);
    pulse(DAT & 16);
    pulse(DAT & 8);
    pulse(DAT & 4);
    pulse(DAT & 2);
    pulse(DAT & 1);
    pinMode(11, INPUT);

    return receiveACK(ADR);
}

bool sample() {
    if (analogRead(A4) < 500) return true;
    else return false;
}

bool activeListen(int* ADR, int* PID, int* DAT) {

    while(sample() == true) delay(0.001*dt);

    // Initialise the temporary values to be store.
    int tADR = 0;
    int tPID = 0;
    int tDAT = 0;
    int parity;

    delay(dt);
    // sample() ADR values.
    if (sample()) tADR += 4; delay(dt); if (sample()) tADR += 2; delay(dt); if (sample()) tADR += 1; delay(dt);
    
    // If the address is ACK address then ignore the rest of the packet. (ACK address == 000)
    if (tADR == 0) {
        delay(dt);delay(dt);delay(dt);
        return false; }

    // sample() PID values.
    if (sample()) tPID += 8; delay(dt); if (sample()) tPID += 4; delay(dt); if (sample()) tPID += 2; delay(dt); if (sample()) tPID += 1; delay(dt);
    // sample() parity bit.
    if (sample()) parity = 1; 
    else parity = 0;
    delay(dt);
    // sample() DAT values.
    if (sample()) tDAT += 128; delay(dt); if (sample()) tDAT += 64; delay(dt); if (sample()) tDAT += 32; delay(dt); if (sample()) tDAT += 16; delay(dt); 
    if (sample()) tDAT += 8; delay(dt); if (sample()) tDAT += 4; delay(dt); if (sample()) tDAT += 2; delay(dt); if (sample()) tDAT += 1; delay(dt);
    // Check parity bit.
    if (((tADR & 1)^(tADR & 2)^(tADR & 4)^(tPID & 1)^(tPID & 2)^(tPID & 4)^(tPID & 8)^(tDAT & 1)^(tDAT & 2)^(tDAT & 4)^(tDAT & 8)^(tDAT & 16)^(tDAT & 32)^(tDAT & 64)^(tDAT & 128)) == parity)  {return false;}
    // Check that the address matches.
    if (tADR != myADR) return false;
    // Now that the checks are passed, save the values to the memory adresses.
    *ADR = tADR; *PID = tPID; *DAT = tDAT;
    return true;
}

void sendACK(int* ADR) {
    // Do "helloo" sequence.
    pinMode(11, OUTPUT);
    delay(2.5*dt);
    pinMode(11, INPUT);
    delay(0.5*dt);
    // SEND ACK address "000".
    pulse(0);pulse(0);pulse(0);
    // Sen ADR of recipient.
    pulse(*ADR & 4);
    pulse(*ADR & 2);
    pulse(*ADR & 1);

    pinMode(11, INPUT);
}

bool passiveListen(int* listenBit, int* ADR, int* PID, int* DAT) {
    if (sample()) (*listenBit)++;
    else *listenBit = 0;
    
    // Check if two sample()s have been High in a row. If so enter activeListen and save values into ADR,PID,DAT
    if (*listenBit == 2) {
        if (activeListen(ADR, PID, DAT)) {sendACK(ADR);}
        *listenBit = 0;
        return true;
    }
    else return false;
}


bool sendMessage(int* ADR, char message[]) {

    for (int i = 0; i < 16; i++) {
        if (sendPacket(ADR,i,int(message[i])) == 0) return false;
        Serial.println(int(message[i]));
    }
    return true;
}