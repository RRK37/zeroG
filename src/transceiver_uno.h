
#pragma once

void zeroG_setup();

void pulse(bool x);

bool sendPacket(int ADR, int PID, int DAT);

bool sample();

bool passiveListen(int* listenBit, int* ADR, int* PID, int* DAT);

bool sendMessage(int* ADR, char message[]);
