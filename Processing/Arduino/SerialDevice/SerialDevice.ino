/*
  Sketch SerialDevice

  This sketch is used to communicate to Processing sketch running on PC.
  The Processing sketch will automatically detect and connect to this board which
  use the same trans format.

  modified 2016/8/7
  by http://www.freenove.com
*/

#include "SerialCommand.h"

int analogPins[] = {A0, A1, A2, A3, A4, A5};
int ledPin = 13;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Do not put code here that will make each loop execute more than 20ms
  unsigned long msNow = millis();
  if (msNow % 2000 < 1000)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
}

void serialEvent() {
  static byte inData[64];
  static byte inDataNum = 0;

  while (Serial.available() > 0)
  {
    byte inByte = Serial.read();
    if (inByte == SerialCommand.transStart)
      inDataNum = 0;
    inData[inDataNum++] = inByte;
    if (inByte == SerialCommand.transEnd)
      break;
  }

  if (inData[0] == SerialCommand.transStart && inData[inDataNum - 1] == SerialCommand.transEnd)
  {
    Serial.write(SerialCommand.transStart);
    if (inData[1] == SerialCommand.requestEcho)
    {
      Serial.write(SerialCommand.echo);
    }
    else if (inData[1] == SerialCommand.requestAnalog)
    {
      int analog = analogRead(analogPins[0]);
      Serial.write(SerialCommand.Analog);
      Serial.write(analog / 100);
      Serial.write(analog % 100);
    }
    else if (inData[1] == SerialCommand.requestAnalogs)
    {
      int analogs[inData[2]];
      for (int i = 0; i < inData[2]; i++)
        analogs[i] = analogRead(analogPins[i]);
      Serial.write(SerialCommand.Analogs);
      for (int i = 0; i < inData[2]; i++)
      {
        Serial.write(analogs[i] / 100);
        Serial.write(analogs[i] % 100);
      }
    }
    Serial.write(SerialCommand.transEnd);
  }
}
