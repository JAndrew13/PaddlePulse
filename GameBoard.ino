#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Create the display objects for a 4-digit alphanumeric display
Adafruit_AlphaNum4 display1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 display2 = Adafruit_AlphaNum4();

const int player1SignalPin = 2;  // Input pin for player 1 signal
const int player2SignalPin = 3;  // Input pin for player 2 signal

const int player1LedPin = 4;  // LED for player 1
const int player2LedPin = 5;  // LED for player 2

bool roundStarted = false;
const int winCondition = 11;
const int leadCondition = 2;
const int debounceDelay = 100;

int player1Score = 0;
int player2Score = 0;

bool player1Returning = false;
bool player2Returning = false;

bool player1Serving = false;
bool player2Serving = false;

const float elapsedTimeLimit = 3000;  // max length of hangtime in seconds before failed return
unsigned long elapsedTime;            // current time since player
unsigned long startTime;

void setup() {
  pinMode(player1SignalPin, INPUT);
  pinMode(player2SignalPin, INPUT);
  pinMode(player1LedPin, OUTPUT);
  pinMode(player2LedPin, OUTPUT);

  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize the alphanumeric display
  display1.begin(0x70);  // Pass in the Display1 address
  display2.begin(0x71);  // Pass in the Display2 address

  display1.clear();  // Clear the display buffer
  display2.clear();  // Clear the display buffer

  displayStartup();

  display1.writeDisplay();  // Write the buffer to the display
  display2.writeDisplay();  // Write the buffer to the display
}

void loop() {
  int player1Signal = digitalRead(player1SignalPin);
  int player2Signal = digitalRead(player2SignalPin);

  
  if (player1Signal == HIGH || player2Signal == HIGH) {
    delay(debounceDelay);
    startTime = millis();
    if(player1Signal == HIGH)
    {
      Serial.println("Player 1 Board hit!");
    } 
    else
    {
      Serial.println("Player 2 Board hit!");
    }
  }

  if (roundStarted == false)  // On Player Serve
  {
    // If Player 1 has started serving
    if (player1Signal == HIGH) { 
      player1Serving = true;
      Serial.print("round has started.");
      roundStarted = true;
      Serial.println("p1 serving");
      return;
    }

    // If Player 2 has started serving
    if (player2Signal == HIGH) {
      player2Serving = true;
      Serial.print("round has started.");
      roundStarted = true;
      Serial.println("p2 serving");
      return;
    }
  }

  else  // if roundStarted is true
  {
    if (player1Signal == HIGH) {

      // Player 2 completed serve
      if (player2Serving == true && !player2Returning) {
        player2Serving = false;
        player1Returning = true;
        Serial.println("p2 serve complete");
        return;
      }

      // Player 1 double hit
      if (player1Serving == true || player1Returning == true)
      {
        Serial.println("P1 double hit.");
        player2Scored();
        return;
      }

      // Player 2 completed return 
      if (player2Returning == true) {
        Serial.println("p2 returned");
        player2Returning = false;
        player1Returning = true;
        return;
      }
    }

    
    // Player 1 completed serve
    if (player2Signal == HIGH) {
      if (player1Serving == true && !player2Returning) {
        player1Serving = false;
        player2Returning = true;
        Serial.println("p1 serve complete");
        return;
      }

      // Player 2 double hit
      if (player2Serving == true || player2Returning == true) {
        Serial.println("P2 double hit.");
        player1Scored();
        return;
      }

      // Player 1 completed return 
      if (player1Returning == true) {
        Serial.println("p1 returned");
        player1Returning = false;
        player2Returning = true;
        return;
      }
    }
  }

  checkTimer();
  checkScores();
}

void checkTimer() {
  unsigned long tempTimeLimit = elapsedTimeLimit;
  if (player1Serving == true || player2Serving == true)
  {
    tempTimeLimit = tempTimeLimit / 2;
  }

  elapsedTime = millis() - startTime;
  if (elapsedTime >= tempTimeLimit) {
    if (player1Serving || player1Returning) {
      Serial.println("p1 timed out");  
      player2Scored();
    }
    else if (player2Serving || player2Returning) {
      Serial.println("p2 timed out");  
      player1Scored();
    }
  }
}

void checkScores() {
  // If player 1 has won
  if (player1Score >= winCondition && player1Score - leadCondition >= player2Score) {
    Serial.println("p1 wins");  
    displayWinner(1);
    delay(10000);
    resetGame();
  }
    // If player 2 has won
  if (player2Score >= winCondition && player2Score - leadCondition >= player1Score) {
    Serial.println("p2 wins");  
    displayWinner(2);
    delay(10000);
    resetGame();
  }
}

// Refresh the display wiht updated score
void displayPlayerScores() {
  display1.clear();
  display2.clear();

  display1.writeDigitAscii(0, player1Score / 10 + '0', false);  // Tens place
  display1.writeDigitAscii(1, player1Score % 10 + '0', false);  // Ones place
  display1.writeDigitAscii(2, player2Score / 10 + '0', false);  // Tens place
  display1.writeDigitAscii(3, player2Score % 10 + '0', false);  // Ones place

  display2.writeDigitAscii(0, player1Score / 10 + '0', false);  // Tens place
  display2.writeDigitAscii(1, player1Score % 10 + '0', false);  // Ones place
  display2.writeDigitAscii(2, player2Score / 10 + '0', false);  // Tens place
  display2.writeDigitAscii(3, player2Score % 10 + '0', false);  // Ones place

  display1.writeDisplay();
  display2.writeDisplay();
}

// Display the winner 
void displayWinner(int player) {
  // Determine the ASCII character to display based on the player number
  char winnerChar = (player == 1) ? '1' : '2';

  // Animation loop to flash the winner's number
  for(int i = 0; i < 6; i++) { // Flash 6 times
    display1.clear();
    display2.clear();
    
    if (i % 2 == 0) { // On even iterations, display the winner
      // Show the winning player's number on both displays
      display1.writeDigitAscii(0, winnerChar, false);
      display1.writeDigitAscii(1, winnerChar, false);
      display1.writeDigitAscii(2, winnerChar, false);
      display1.writeDigitAscii(3, winnerChar, false);
      
      display2.writeDigitAscii(0, winnerChar, false);
      display2.writeDigitAscii(1, winnerChar, false);
      display2.writeDigitAscii(2, winnerChar, false);
      display2.writeDigitAscii(3, winnerChar, false);
    }

    // Write changes to the displays
    display1.writeDisplay();
    display2.writeDisplay();
    
    // Wait for 500 milliseconds
    delay(500);
  }

  // Optionally, clear the displays or set them to a "Game Over" message
  display1.clear();
  display2.clear();
  // Example for "Game Over" message, adjust as per your requirement
  // This part assumes display1 is sufficient for "P1" or "P2" and "WIN!"
  if(player == 1) {
    display1.writeDigitAscii(0, 'P');
    display1.writeDigitAscii(1, '1');
    display1.writeDigitAscii(2, 'W');
    display1.writeDigitAscii(3, 'N');
  } else {
    display1.writeDigitAscii(0, 'P');
    display1.writeDigitAscii(1, '2');
    display1.writeDigitAscii(2, 'W');
    display1.writeDigitAscii(3, 'N');
  }

  display1.writeDisplay();
  display2.writeDisplay();

  delay(10000); // Display "Game Over" for 10 seconds before your next action
}

// Display zeros until player serves
void displayStartup() {
  display1.clear();
  display2.clear();

 char zero = '0'; // ASCII character for 0
  display1.writeDigitAscii(0, zero, false);  // Tens place for Player 1
  display1.writeDigitAscii(1, zero, false);  // Ones place for Player 1
  display1.writeDigitAscii(2, zero, false);  // Tens place for Player 2
  display1.writeDigitAscii(3, zero, false);  // Ones place for Player 2

  display2.writeDigitAscii(0, zero, false);  // Tens place for Player 1
  display2.writeDigitAscii(1, zero, false);  // Ones place for Player 1
  display2.writeDigitAscii(2, zero, false);  // Tens place for Player 2
  display2.writeDigitAscii(3, zero, false);  // Ones place for Player 2

  display1.writeDisplay();
  display2.writeDisplay();

  // after 10 seconds -> reset()
}

void resetGame() {
  player1Score = 0;
  player2Score = 0;
  Serial.println("game is resetting");  
  resetRound();
}

void resetRound() {
  player1Returning = false;
  player1Serving = false;

  player2Returning = false;
  player2Serving = false;

  roundStarted = false;
  Serial.println("round is resetting");  
}

void player1Scored() {
  player1Score = min(player1Score + 1, 99);  // Increment score with a max of 99
  Serial.println("p1 score");
  score();
}

void score() {
  resetRound();
  displayPlayerScores();
  elapsedTime = 0;
  delay(debounceDelay);
}

void player2Scored() {
  player2Score = min(player2Score + 1, 99);  // Increment score with a max of 99

  Serial.println("p2 score");
  score();
}
