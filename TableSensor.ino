const int sensor1Pin = A0; // Sensor output connected to A0
const int sensor2Pin = A1; // Sensor output connected to A1
const int sensor3Pin = A2; // Sensor output connected to A2

const int ledPin = 9;     // LED connected to pin 9 through 220-ohm resistor
const int signalOutPin;  // Pin for outputing the signal to the display board

// Declare and initialize adjusttable threshold variables
const int lowThreshold = 10; 
const int highThreshold = 1000;

int sensor1Value = 0;  // Variable to store the sensor 1A value
int sensor2Value = 0;  // Variable to store the sensor value
int sensor3Value = 0;  // Variable to store the sensor value

bool isDetectedValue = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(signalOutPin, OUTPUT);

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);

  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  sensor1Value = analogRead(sensor1Pin); // Read the sensor value
  sensor2Value = analogRead(sensor2Pin); // Read the sensor value
  sensor3Value = analogRead(sensor3Pin); // Read the sensor value

  isDetectedValue = isDetected();

  Serial.println(isDetectedValue); 
  Serial.println(sensor1Value); 
  Serial.println(sensor2Value); 
  Serial.println(sensor3Value); 

  int ledBrightness = map(sensor1Value, 0, 1023, 0, 255); // Map sensor value to LED brightness

  // Check if the sensor value is within a specific range
  if (isDetectedValue) {
    analogWrite(ledPin, 1); // Adjust LED brightness based on the sensor value
    delay(100); // Short delay for stability
  } else {
    analogWrite(ledPin, 0); // Turn off LED
  }
}

bool isDetected()
{
  return (isSignalInRange(sensor1Value) || isSignalInRange(sensor2Value) || isSignalInRange(sensor3Value));
}

bool isSignalInRange(int sensorValue)
{
  return (sensorValue > lowThreshold && sensorValue < highThreshold);
}
