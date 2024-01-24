#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C ScoreBoard(0x27, 16,2);

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
    bool Has_Chosen_Bullets = false;
    bool Should_Play_Round = true;


    int Turn = 1;// 1 = Player 1, 2 = player 2

    int ShootSelf_P1_pin = 2;
    int ShootOponent_P1_pin = 3;

    int ShootSelf_P2_pin = 4;
    int ShootOponent_P2_pin = 5;

    int Player1_Health = 5;
    int Player2_Health = 5;

    //Variables End

    //Functions Start
    // Initiates thing, idk much(till now)
    void Start();

    void ProceedTurn();

    void PlayRound_1();
    void PlayRound_2();
    void PlayRound_3();

    void ShowHealth();
    void ShowBullets();
    void Intro();
    void ShowRound();

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
    ScoreBoard.init();
    ScoreBoard.backlight();
    Intro();

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
    if(!Has_Chosen_Bullets)
    {
        TotalBullets = 4;
        LiveBullets = random(1,5);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    if(LiveBullets == TotalBullets){ blankBullets = random(1,5); LiveBullets -= blankBullets; ShowBullets(); }
    else if (blankBullets == TotalBullets){ LiveBullets = random(1,5); blankBullets -= LiveBullets; ShowBullets();}
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    ShowHealth();
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
                    Player2_Health--;
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
                    Player1_Health--;
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
                    Player1_Health--;
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
                    Player2_Health--;
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
    ShowHealth();
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        delay(1000);
        ScoreBoard.clear();
        Round++;
        Has_Chosen_Bullets = false;
    } 
    ProceedTurn();
    delay(2000);
    
}

void RUSSIANROULLET::PlayRound_2()
{
    
    if(!Has_Chosen_Bullets)
    {
        TotalBullets = 6;
        LiveBullets = random(1,7);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        Player1_Health = 5;
        Player2_Health = 5;
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    if(LiveBullets == TotalBullets){ blankBullets = random(1,5); LiveBullets -= blankBullets; ShowBullets(); }
    else if (blankBullets == TotalBullets){ LiveBullets = random(1,5); blankBullets -= LiveBullets; ShowBullets();}
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    ShowHealth();
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
                    Player2_Health--;
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
                    Player1_Health--;
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
                    Player1_Health--;
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
                    Player2_Health--;
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
    ShowHealth();
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        ScoreBoard.clear();
        Serial.println("Round Over");
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        delay(1000);
        ScoreBoard.clear();
        Should_Play_Round = false;
        Round++;
        Has_Chosen_Bullets = false;
    } 
    ProceedTurn();
    delay(2000);
    
}

void RUSSIANROULLET::PlayRound_3()
{ 
    if(!Has_Chosen_Bullets)
    {
        TotalBullets = 8;
        LiveBullets = random(1,9);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        Player1_Health = 5;
        Player2_Health = 5;
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    if(LiveBullets == TotalBullets){ blankBullets = random(1,5); LiveBullets -= blankBullets; ShowBullets(); }
    else if (blankBullets == TotalBullets){ LiveBullets = random(1,5); blankBullets -= LiveBullets; ShowBullets();}
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    ShowHealth();
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
                    Player2_Health--;
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
                    Player1_Health--;
                    break;
                }
                else if(CurrentBullet == 0)
                {
                    Serial.println("Blank Bullet");
                    Serial.println("You Will Get An Extra Chance");
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
                    Player1_Health--;
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
                    Player2_Health--;
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
        ScoreBoard.clear();
        Serial.println("Round Over");
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        delay(1000);
        ScoreBoard.clear();
        Round++;
        Has_Chosen_Bullets = false;
    } 
    ProceedTurn();
    delay(2000);
    
}

void RUSSIANROULLET::ShowHealth()
{
    ScoreBoard.setCursor(0,0);
    ScoreBoard.print("PLAYER1");
    ScoreBoard.setCursor(9,0);
    ScoreBoard.print("PLAYER2");
    ScoreBoard.setCursor(4,1);
    ScoreBoard.print(Player1_Health);
    ScoreBoard.setCursor(13,1);
    ScoreBoard.print(Player2_Health);
}

void RUSSIANROULLET::ShowBullets()
{
    ScoreBoard.setCursor(0,0);
    ScoreBoard.print("Lives");
    ScoreBoard.setCursor(11,0);
    ScoreBoard.print("Blank");
    ScoreBoard.setCursor(3,1);
    ScoreBoard.print(LiveBullets);
    ScoreBoard.setCursor(14,1);
    ScoreBoard.print(blankBullets);
    delay(2000);
    ScoreBoard.clear();
}

void RUSSIANROULLET::Intro()
{
    ScoreBoard.setCursor(0,0);
    ScoreBoard.print("Russian Roullet");
    delay(1500);
    ScoreBoard.clear();
    ScoreBoard.print("3 Rounds");
    delay(1000);
    ScoreBoard.clear();
    ScoreBoard.print("Round 1:");
    ScoreBoard.setCursor(0,1);
    ScoreBoard.print("4 Bullets");
    delay(1000);
    ScoreBoard.clear();
    ScoreBoard.print("Round 2:");
    ScoreBoard.setCursor(0,1);
    ScoreBoard.print("6 Bullets");
    delay(1000);
    ScoreBoard.clear();
    ScoreBoard.print("Round 3:");
    ScoreBoard.setCursor(0,1);
    ScoreBoard.print("8 Bullets");
    delay(1000);
    ScoreBoard.clear();
}

void RUSSIANROULLET::ShowRound()
{
    ScoreBoard.setCursor(0,0);
    ScoreBoard.print("Round:");
    ScoreBoard.setCursor(8,1);
    ScoreBoard.print(Round);
    ScoreBoard.setCursor(0,0);
    delay(1000);
    ScoreBoard.clear();
}
