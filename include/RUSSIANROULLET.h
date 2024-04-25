#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


LiquidCrystal_I2C ScoreBoard(0x27, 16,2);
LiquidCrystal_I2C Player1Menue(0x23, 16,2);// change address when you get the 2 lcd displays
LiquidCrystal_I2C Player2Menue(0x22, 16,2);// change address when you get the 2 lcd displays


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
    bool ResetHealth = true;
    int RoundPlays = 1;
    int TotalBulletsPerRoundPlay = 2;
    int BulletDamage = 1;

    int Turn = 1;// 1 = Player 1, 2 = player 2

    int ShootSelf_P1_pin = 2;
    int ShootOponent_P1_pin = 3;
    int Player1_Led = 6;
    int Player1_Round_Wins = 0;

    int ShootSelf_P2_pin = 4;
    int ShootOponent_P2_pin = 5;
    int Player2_Led = 7;
    int Player2_Round_Wins = 0;

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
    void ShowWinner();
    void ShowRound();

    void Reload();

    void FlickerLED();
    //Item Functions
    void Magnifying_Glass(int Player);
    void Saw(int Player);
    void HandCuffs(int Player);
    void Cigartte(int Player);
    void Beer(int Player);

    //Functions END
    

};

void RUSSIANROULLET::Start()
{
    randomSeed(analogRead(0));
    pinMode(ShootSelf_P1_pin, INPUT);
    pinMode(ShootOponent_P1_pin, INPUT);
    pinMode(ShootSelf_P2_pin, INPUT);
    pinMode(ShootOponent_P2_pin, INPUT);
    pinMode(Player1_Led, OUTPUT);
    pinMode(Player2_Led, OUTPUT);
    digitalWrite(Player1_Led, LOW);
    digitalWrite(Player2_Led, LOW);
    Serial.begin(9600);

    ScoreBoard.init();
    ScoreBoard.backlight();

    Player1Menue.init();
    Player2Menue.init();

    Player1Menue.backlight();    
    Player2Menue.backlight();
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
        if(ResetHealth)
        {
            Player1_Health = 4;
            Player2_Health = 4;
            ResetHealth = false;
        }
        TotalBullets = TotalBulletsPerRoundPlay * RoundPlays;
        LiveBullets = random(1, TotalBullets + 1);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        Turn = 1;
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets == TotalBullets){ blankBullets = random(1,TotalBullets - 1); LiveBullets -= blankBullets; ShowBullets(); } // if Livebullets is equal to total bullets 
    else if (blankBullets == TotalBullets){ LiveBullets = random(1,TotalBullets - 1); blankBullets -= LiveBullets; ShowBullets();} // if blank bullets is equal to live bullets
    if(LiveBullets < 0){LiveBullets = LiveBullets * -1; ShowBullets();} // if livebullets is in negative
    else if(blankBullets < 0 ){blankBullets = blankBullets * -1; ShowBullets();} // if blankbullets is in negative
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; // if blank bullets are finished
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0; // if livebullets are finished
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    while(true)
    {
        ShowHealth();
        if(Turn == 1)
        {
            digitalWrite(Player2_Led, 0);
            digitalWrite(Player1_Led, 1);
            if(digitalRead(ShootOponent_P1_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player2_Health-= BulletDamage;
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
                    Player1_Health-= BulletDamage;
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
            digitalWrite(Player2_Led, 1);
            digitalWrite(Player1_Led, 0);
            if(digitalRead(ShootOponent_P2_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player1_Health-= BulletDamage;
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
                    Player2_Health-= BulletDamage;
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
    BulletDamage = 1;
    ShowHealth();
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        Reload();
    }
    if(Player1_Health <= 0) // If First Player Losses All Health In First Round(Player 2 Wins Round)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player2_Round_Wins++;
        RoundPlays = 1;
    }
    else if(Player2_Health <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player1_Round_Wins++;
        RoundPlays = 1;
    }
    ProceedTurn();
    delay(500);
    
}

void RUSSIANROULLET::PlayRound_2()
{
    if(!Has_Chosen_Bullets)
    {

        if(ResetHealth)
        {
            Player1_Health = 6;
            Player2_Health = 6;
            ResetHealth = false;
        }
        TotalBullets = TotalBulletsPerRoundPlay * RoundPlays;
        LiveBullets = random(1, TotalBullets + 1);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        Turn = 1;
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    if(LiveBullets == TotalBullets){ blankBullets = random(1, TotalBullets - 1); LiveBullets -= blankBullets; ShowBullets(); }
    else if (blankBullets == TotalBullets){ LiveBullets = random(1, TotalBullets - 1); blankBullets -= LiveBullets; ShowBullets();}
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    while(true)
    {
        ShowHealth();
        if(Turn == 1)
        {
            digitalWrite(Player2_Led, 0);
            digitalWrite(Player1_Led, 1);
            if(digitalRead(ShootOponent_P1_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player2_Health-= BulletDamage;
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
                    Player1_Health-= BulletDamage;
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
            digitalWrite(Player2_Led, 1);
            digitalWrite(Player1_Led, 0);
            if(digitalRead(ShootOponent_P2_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player1_Health-= BulletDamage;
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
                    Player2_Health-= BulletDamage;
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
    BulletDamage = 1;
    ShowHealth();
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        Reload();
    }
    if(Player1_Health <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player2_Round_Wins++;
        RoundPlays = 1;
    }
    else if(Player2_Health <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player1_Round_Wins++;
        RoundPlays = 1;
    }
    ProceedTurn();
    delay(500);
    
}

void RUSSIANROULLET::PlayRound_3()
{ 
    if(!Has_Chosen_Bullets)
    {
        if(ResetHealth)
        {
            Player1_Health = 8;
            Player2_Health = 8;
        }
        TotalBullets = TotalBulletsPerRoundPlay * RoundPlays;
        LiveBullets = random(1, TotalBullets +  1);
        blankBullets = TotalBullets - LiveBullets; 
        CurrentBullet = random(0,2);
        Has_Chosen_Bullets = true;
        Turn = 1;
        Serial.println("Playing 3rd Round");
        ShowRound();
        ShowBullets();
    }
    if(LiveBullets > 0 && blankBullets <= 0) CurrentBullet = 1; 
    else if(blankBullets > 0 && LiveBullets <= 0) CurrentBullet = 0;
    if(LiveBullets == TotalBullets){ blankBullets = random(1, TotalBullets - 1); LiveBullets -= blankBullets; ShowBullets(); }
    else if (blankBullets == TotalBullets){ LiveBullets = random(1, TotalBullets - 1); blankBullets -= LiveBullets; ShowBullets();}
    Serial.print("Current bullet: ");
    Serial.println(CurrentBullet);
    Serial.print("Live Bullets: ");
  	Serial.println(LiveBullets);
  	Serial.print("Blank Bullets: ");
  	Serial.println(blankBullets);
    while(true)
    {
        ShowHealth();
        if(Turn == 1)
        {
            digitalWrite(Player2_Led, 0);
            digitalWrite(Player1_Led, 1);
            if(digitalRead(ShootOponent_P1_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player2_Health-= BulletDamage;
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
                    Player1_Health-= BulletDamage;
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
            digitalWrite(Player2_Led, 1);
            digitalWrite(Player1_Led, 0);
            if(digitalRead(ShootOponent_P2_pin) == 1)
            {
                Serial.println("Shooting Oponent");
                delay(100);
                if(CurrentBullet == 1)
                {
                    Serial.println("Shot Oponent");
                    LiveBullets--;
                    Player1_Health-= BulletDamage;
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
                    Player2_Health-= BulletDamage;
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
    BulletDamage = 1;
    ShowHealth();
    if(LiveBullets <= 0 && blankBullets <= 0)
    {
        Reload();
    }
    if(Player1_Health <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player2_Round_Wins++;
        RoundPlays = 1;
    }
    else if(Player2_Health <= 0)
    {
        Serial.println("Round Over");
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Round Over!");
        Round++;
        delay(2000);
        ScoreBoard.clear();
        Has_Chosen_Bullets = false;
        ResetHealth = true;
        Player1_Round_Wins++;
        RoundPlays = 1;
    }
    ProceedTurn();
    delay(500);
    
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
    ScoreBoard.clear();
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

void RUSSIANROULLET::ShowWinner()
{
    if(Player1_Round_Wins < Player2_Round_Wins)
    {
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 2 Won");
    }
    else
    {
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 1 Won!");
    }
    FlickerLED();
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

void RUSSIANROULLET::Reload()
{
    Serial.println("Round Over");
    ScoreBoard.clear();
    ScoreBoard.setCursor(0,0);
    ScoreBoard.print("Reloading");
    delay(1000);
    ScoreBoard.clear();
    Has_Chosen_Bullets = false;
    ResetHealth = false;
    RoundPlays++;
}

void RUSSIANROULLET::FlickerLED()
{
    digitalWrite(Player1_Led, 1);
    digitalWrite(Player2_Led, 0);
    delay(70);
    digitalWrite(Player1_Led, 0);
    digitalWrite(Player2_Led, 1);
    delay(70);
}

void RUSSIANROULLET::Magnifying_Glass(int Player)
{
    if(Player == 1)
    {    
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 1 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Magnifying Glass");
        if(CurrentBullet == 1)
        {
            Player1Menue.clear();
            Player1Menue.setCursor(0,0);
            Player1Menue.print("Live Bullet");
            delay(2000);
            Player1Menue.clear();
        }
        else if(CurrentBullet == 0)
        {
            Player1Menue.clear();
            Player1Menue.setCursor(0,0);
            Player1Menue.print("Blank Bullet");
            delay(2000);
            Player1Menue.clear();
        }
    }
    else if(Player == 2)
    {
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 2 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Magnifying Glass");
        if(CurrentBullet == 1)
        {
            Player2Menue.clear();
            Player2Menue.setCursor(0,0);
            Player2Menue.print("Live Bullet");
            delay(2000);
            Player2Menue.clear();
        }
        else if(CurrentBullet == 0)
        {
            Player2Menue.clear();
            Player2Menue.setCursor(0,0);
            Player2Menue.print("Blank Bullet");
            delay(2000);
            Player2Menue.clear();
        }
    }
    ScoreBoard.clear();
}

void RUSSIANROULLET::Saw(int Player)
{
    if(Player == 1)
    {
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 1 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Double Damage");
        Player1Menue.clear();
        Player1Menue.setCursor(0,0);
        Player1Menue.print("Using:");
        Player1Menue.setCursor(0,1);
        Player1Menue.print("Double Damage");
        delay(2000);
    }
    else if(Player == 2)
    {
        ScoreBoard.clear();
        ScoreBoard.setCursor(0,0);
        ScoreBoard.print("Player 2 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Double Damage");
        Player2Menue.clear();
        Player2Menue.setCursor(0,0);
        Player2Menue.print("Using:");
        Player2Menue.setCursor(0,1);
        Player2Menue.print("Double Damage");
        delay(2000);  
    }
    BulletDamage = 2;
    ScoreBoard.clear();
}

void RUSSIANROULLET::HandCuffs(int Player)
{
    ScoreBoard.clear();
    if(Player == 1)
    {
        ScoreBoard.print("Player 1 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("HandCuffs");
        Player1Menue.clear();
        Player1Menue.setCursor(0,0);
        Player1Menue.print("You Now Have An");
        Player1Menue.setCursor(0,1);;
        Player1Menue.print("Extra Turn");
        delay(2000);
        P1_Has_2_Turns = true;
    }
    else if(Player == 2)
    {
        ScoreBoard.print("Player 2 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("HandCuffs");
        Player2Menue.clear();
        Player2Menue.setCursor(0,0);
        Player2Menue.print("You Now Have An");
        Player2Menue.setCursor(0,1);;
        Player2Menue.print("Extra Turn");
        delay(2000);
        P2_Has_2_Turns = true;
    }
    ScoreBoard.clear();
    Player1Menue.clear();
    Player2Menue.clear();
}

void RUSSIANROULLET::Cigartte(int Player)
{
    ScoreBoard.clear();
    if(Player == 1)
    {
        ScoreBoard.print("Player 1 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Cigarette");
        Player1Menue.clear();
        Player1Menue.setCursor(0,0);
        Player1Menue.print("You Now Have");
        Player1Menue.setCursor(0,1);;
        Player1Menue.print("+1 Health Point");
        delay(2000);
        Player1_Health++;
    }
    else if(Player == 2)
    {
        ScoreBoard.print("Player 2 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Cigarette");
        Player2Menue.clear();
        Player2Menue.setCursor(0,0);
        Player2Menue.print("You Now Have An");
        Player2Menue.setCursor(0,1);;
        Player2Menue.print("+1 Health Point");
        delay(2000);
        Player2_Health++;
    }
    ScoreBoard.clear();
    Player1Menue.clear();
    Player2Menue.clear();
}

void RUSSIANROULLET::Beer(int Player)
{
    ScoreBoard.clear();
    if(Player == 1)
    {
        ScoreBoard.print("Player 1 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Beer Can");
        Player1Menue.clear();
        Player1Menue.setCursor(0,0);
        Player1Menue.print("Using:");
        Player1Menue.setCursor(0,1);
        Player1Menue.print("Beer Can");
        if(CurrentBullet == 1)
        {
            ScoreBoard.clear();
            ScoreBoard.setCursor(0,0);
            ScoreBoard.print("The Bullet Was:");
            ScoreBoard.setCursor(0,1);
            ScoreBoard.print("Live");
            LiveBullets--;
        }
        else if(CurrentBullet == 0)
        {
            ScoreBoard.clear();
            ScoreBoard.setCursor(0,0);
            ScoreBoard.print("The Bullet Was:");
            ScoreBoard.setCursor(0,1);
            ScoreBoard.print("Blank");
            blankBullets--;
        }
    }
    else if(Player == 2)
    {
        ScoreBoard.print("Player 2 Used:");
        ScoreBoard.setCursor(0,1);
        ScoreBoard.print("Beer Can");
        Player2Menue.clear();
        Player2Menue.setCursor(0,0);
        Player2Menue.print("Using:");
        Player2Menue.setCursor(0,1);
        Player2Menue.print("Beer Can");
        if(CurrentBullet == 1)
        {
            ScoreBoard.clear();
            ScoreBoard.setCursor(0,0);
            ScoreBoard.print("The Bullet Was:");
            ScoreBoard.setCursor(0,1);
            ScoreBoard.print("Live");
            LiveBullets--;
        }
        else if(CurrentBullet == 0)
        {
            ScoreBoard.clear();
            ScoreBoard.setCursor(0,0);
            ScoreBoard.print("The Bullet Was:");
            ScoreBoard.setCursor(0,1);
            ScoreBoard.print("Blank");
            blankBullets--;
        }
    }
    delay(2000);
    ScoreBoard.clear();
    Player1Menue.clear();
    Player2Menue.clear();
}
