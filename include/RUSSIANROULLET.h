#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class RUSSIANROULLET
{
    // Initiates thing, idk much(till now)
    void Initiate();
    

};

void RUSSIANROULLET::Initiate()
{
    pinMode(6, INPUT);
  Serial.begin(9600);

}