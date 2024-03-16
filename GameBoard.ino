#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Create the display objects for a 4-digit alphanumeric display
Adafruit_AlphaNum4 display1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 display2 = Adafruit_AlphaNum4();

const int player1SignalPin = 2; // Input pin for player 1 signal
const int player2SignalPin = 3; // Input pin for player 2 signal

const int player1LedPin = 4; // LED for player 1
const int player2LedPin = 5; // LED for player 2

bool gameStart = false;
const int winCondition = 11;
const int leadCondition = 2; 

int player1Score = 0;
int player2Score = 0;

bool player1Returning = false;
bool player2Returning = false;

bool player1Serving = false;
bool player2Serving = false;

const float elapsedTimeLimit = 3; // max length of hangtime in seconds before failed return
unsigned long elapsedTime; // current time since player 
unsigned long startTime; 

void setup() {
  pinMode(player1SignalPin, INPUT);
  pinMode(player2SignalPin, INPUT);
  pinMode(player1LedPin, OUTPUT);
  pinMode(player2LedPin, OUTPUT);

  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize the alphanumeric display
  display1.begin(0x70); // Pass in the Display1 address
  display2.begin(0x71); // Pass in the Display2 address

  display1.clear(); // Clear the display buffer
  display2.clear(); // Clear the display buffer

  display1.writeDisplay(); // Write the buffer to the display
  display2.writeDisplay(); // Write the buffer to the display
}

void loop() {
  int player1Signal = digitalRead(player1SignalPin);
  int player2Signal = digitalRead(player2SignalPin);

  if (player1Signal == HIGH || player2Signal == HIGH)
  {
    Serial.println("Detection!");
  }
  
  if (gameStart == false) // On Serve
  {
    if (player1Signal == HIGH) {
      player1Serving = true;
      startTime = millis();
      Serial.println("p1 serve!");
    }

    if (player2Signal == HIGH) {
      player2Serving = true;
      startTime = millis();
      Serial.println("p2 serve!");
    }
    gameStart = true;
  } 

  else // if gameStart is true
  {
    if (player1Signal == HIGH) {
// serve hit net 
      
      if(player1Serving == true && ! player1Returning)
      {
        player2Scored();
      }

      if(player2Returning == true)
      {
        player1Returning = true;
        player2Returning = false;
        startTime = millis();
        Serial.println("switch");
      }
      else
      {
        player1Scored();
      }
    }
  
    if (player2Signal == HIGH) {
      if(player2Serving == true && !player2Returning)
      {
        player1Scored();
      }

      if(player1Returning == true)
      {
        player1Returning = false;
        player2Returning = true;
        startTime = millis();
        Serial.println("switch");
      }
      else
      {
        player2Scored();
      }
    }
  } 

  checkTimer();
  checkScores();
}

void checkTimer()
{
  elapsedTime = millis() - startTime; 
  if (elapsedTime >= elapsedTimeLimit)
  {
    if(player1Serving || player1Returning)
    {
      player2Scored();
    }

    else if(player2Serving || player2Returning)
    {
      player1Scored();
    }
  }
}

void checkScores()
{
  if (player1Score >= winCondition && player1Score - leadCondition >= player2Score)
  { 
    gameStart = false;
    displayWinner(1);
  } 

  if (player2Score >= winCondition && player2Score - leadCondition >= player1Score)
  { 
    gameStart = false;
    displayWinner(2);
  } 
}

void displayPlayerScores() {
  display1.clear();
  display2.clear();

  display1.writeDigitAscii(0, player1Score / 10 + '0', false); // Tens place
  display1.writeDigitAscii(1, player1Score % 10 + '0', false); // Ones place
  display1.writeDigitAscii(3, player2Score / 10 + '0', false); // Tens place
  display1.writeDigitAscii(4, player2Score % 10 + '0', false); // Ones place

  display2.writeDigitAscii(0, player1Score / 10 + '0', false); // Tens place
  display2.writeDigitAscii(1, player1Score % 10 + '0', false); // Ones place
  display2.writeDigitAscii(3, player2Score / 10 + '0', false); // Tens place
  display2.writeDigitAscii(4, player2Score % 10 + '0', false); // Ones place

  display1.writeDisplay();
  display2.writeDisplay();
}

void displayWinner(int player) {
  display1.clear();
  display2.clear();

  display1.writeDigitAscii(0, player, false); // Tens place
  display1.writeDigitAscii(1, player, false); // Ones place
  display1.writeDigitAscii(4, player, false); // Ones place

  display2.writeDigitAscii(0, player, false); // Tens place
  display2.writeDigitAscii(1, player, false); // Ones place
  display2.writeDigitAscii(4, player, false); // Ones place

  display1.writeDisplay();
  display2.writeDisplay();

  // after 10 seconds -> reset()
}

void resetGame()
{
  player1Score = 0;
  player2Score = 0;
  resetRound();
}

void resetRound()
{
  
  player1Returning = false;
  player1Serving = false;


  player2Returning = false;
  player2Serving = false;

  gameStart = false;
}

void player1Scored()
{
    player1Score = min(player1Score + 1, 99); // Increment score with a max of 99
    
    Serial.println("p1 score");

score();
}

void score(){
  
  resetRound();
  displayPlayerScores();
    elapsedTime = 0;
    delay(200);
}

void player2Scored()
{
    player2Score = min(player2Score + 1, 99); // Increment score with a max of 99
    
    Serial.println("p2 score");
  score();
}
