#include "RUSSIANROULLET.h"
void setup()
{
  
}

void loop()
{
  int LiveBullets = random(1,7);
  int blankBullets = 6 - LiveBullets;
  int CurrentBullet = random(0,2); // 1 = Live, 0 = blank
  Serial.print("Current bullet: ");
  Serial.println(CurrentBullet);
  while(true)
  {
    if(digitalRead(6) == 1)
    {
      if(CurrentBullet == 1)  
      {
        Serial.println("Shots Fired");
        break;
      }
      else if(CurrentBullet == 0)
      {
        Serial.println("Blank Bullet");
        break;
      }
    }
  }
  //Serial.print("Live Bullets: ");
  //Serial.println(LiveBullets);
  //Serial.print("Blank Bullets: ");
  //Serial.println(blankBullets);
}