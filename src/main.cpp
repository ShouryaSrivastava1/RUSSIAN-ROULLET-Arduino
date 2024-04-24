#include "RUSSIANROULLET.h"

RUSSIANROULLET Roullet;
void setup()
{
  Roullet.Start();
}

void loop()
{
  if(Roullet.Round == 1)
  {
    Roullet.PlayRound_1();
  }
  else if(Roullet.Round == 2)
  {
    Roullet.PlayRound_2();
  }
  else if(Roullet.Round == 3)
  {
    Roullet.PlayRound_3();
  }
  else
  {
    Roullet.ShowWinner();
  }
}