#include <Arduino.h>
#include "transceiver_uno.h"

// For LCD:
#include <LiquidCrystal.h>
const int rs = 12, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long previousMillis = 0;
int xyz = 0;

int x;
int y;
int z;
int w;
int count = 0;

void setup() {
  
  zeroG_setup();
  Serial.begin(9600);
  pinMode(A4,INPUT);

  lcd.begin(16, 2); // LCD Setup.
  lcd.print("Message:");
}

void loop() {


//  RECEIVER PART
if (passiveListen(&w,&x,&y,&z)) {
  Serial.println("ADR: " + String(x) + " PID: " + String(y) + " DAT: " + String(z) + "\n");

  // Display the character on the LCD. 
  lcd.setCursor(y,1);
  lcd.print(char(z));
}
delay(30);


lcd.setCursor(2,1);
z = 82;
lcd.print(char(z));

//  SENDING MESSAGE
  // Serial.println(String(sendMessage(3,"ParallelWireless")));
  // while(true);

// INPUT MESSAGE
  // Serial.println(String(sendPacket(4,5,121)));

  // while(true){delay(5);}
  // Serial.println("sds");

  // // Check if data is available to read from the serial port
  // while (Serial.available()) {
  //   if (charNum == 0) {
  //     for (int i = 0; i < 16; i++) {inputString[i] = 0; }
  //   }
  //   // Read the incoming character
  //   char inChar = Serial.read();
  //   // Save the incoming character to the correct location in the string.
  //   if (charNum < 16) { inputString[charNum] = inChar; }
  //   // Set flag. Increment char counter.
  //   messageReceived = true;
  //   charNum++; }
  // if (messageReceived) {
  //   Serial.println("Received: " + String(inputString) + ". This message is " + sizeof(inputString)+ + " long." + int(inputString[15]));
  //   messageReceived = false;
  //   charNum = 0; }


}

