#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class RUSSIANROULLET
{
    public:
    // Variables Start
    int Round = 1; // Number of rounds, TotalRounds 3
    int LiveBullets; // Random Ammout Of Live Bullets From 1 - 6, Total Bullets 8
    int blankBullets; // totalbullets - live bullets = blank bullets
    int CurrentBullet; // 1 = Live, 0 = blank
    int TotalBullets = 8; // Total Number of Bullets

    bool P1_Has_2_Turns = false;
    bool P2_Has_2_Turns = false;
    bool Has_round_started = false;
    bool Should_Play_Round = true;
    int Turn = 1;// 1 = Player 1, 2 = player 2

    int ShootSelf_P1_pin = 2;
    int ShootOponent_P1_pin = 3;

    int ShootSelf_P2_pin = 4;
    int ShootOponent_P2_pin = 5;

    //Variables End

    //Functions Start
    // Initiates thing, idk much(till now)
    void Start();
    void ProceedTurn();
    void PlayRound_1();

    //Functions END
    

};

void RUSSIANROULLET::Start()
{
    randomSeed(analogRead(0));
    pinMode(ShootSelf_P1_pin, INPUT);
    pinMode(ShootOponent_P1_pin, INPUT);
    pinMode(ShootSelf_P2_pin, INPUT);
    pinMode(ShootOponent_P2_pin, INPUT);
    Serial.begin(9600);

}

void RUSSIANROULLET::ProceedTurn()
{
    Serial.println("Next Turn");
    Turn++;
    if (Turn > 2)
    {
        Turn = 1;
    }
    if(P1_Has_2_Turns == true)
    {
        Turn = 1;
        P1_Has_2_Turns = false;
    }
    else if(P2_Has_2_Turns == true)
    {
        Turn = 2;
        P2_Has_2_Turns = false;
    }
}

void RUSSIANROULLET::PlayRound_1()
{
    if (Should_Play_Round)
    {
    if(!Has_round_started)
    {
        LiveBullets = random(1,6);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_round_started = true;
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    while(true)
    {
        if(Turn == 1)
        {
            if(digitalRead(ShootOponent_P1_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    break;
                }
                else if(CurrentBullet == 0)
                {
                    Serial.println("Blank Bullet");
                    blankBullets--;
                    break;
                }
            }
            else if (digitalRead(ShootSelf_P1_pin) == 1)
            {
                Serial.println("Shooting Self");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Self");
                    LiveBullets--;
                    break;
                }
                else if(CurrentBullet == 0)
                {
                    Serial.println("Blank Bullet");
                    blankBullets--;
                    P1_Has_2_Turns = true;
                    break;
                }
            }
        }
        else if(Turn == 2)
        {
            if(digitalRead(ShootOponent_P2_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    break;
                }
                else if(CurrentBullet == 0)
                {
                    Serial.println("Blank Bullet");
                    blankBullets--;
                    break;
                }
            }
            else if (digitalRead(ShootSelf_P2_pin) == 1)
            {
                Serial.println("Shooting Self");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Self");
                    LiveBullets--;
                    break;
                }
                else if(CurrentBullet == 0)
                {
                    Serial.println("Blank Bullet");
                    blankBullets--;
                    P2_Has_2_Turns = true;
                    break;
                }
            }
        }
        
    }
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        Serial.println("Round Over");
        Should_Play_Round = false;
    } 
    ProceedTurn();
    delay(2000);
    }
}
