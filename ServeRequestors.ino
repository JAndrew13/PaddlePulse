// Define the ultrasonic sensor pins

#define TRIG_PIN_1 9
#define ECHO_PIN_1 10
#define TRIG_PIN_2 11
#define ECHO_PIN_2 12

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN_1, OUTPUT); 
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_3, INPUT);
}

void loop() {
  bool p1_serve_requested = ScanForSignal(TRIG_PIN_1, ECHO_PIN_1);
  bool p2_serve_requested = ScanForSignal(TRIG_PIN_2, ECHO_PIN_2);
  if (p1_serve_requested)
  {
    StartRound(1);
  }
  if (p2_serve_requested)
  {
    StartRound(2);
  }
  delay(10); // Short delay before next measurement
}

bool ScanForServeSignal(int TRIG_PIN, int ECHO_PIN) //Check the serve sensor and returns the distance in cm
{
  digitalWrite(TRIG_PIN, LOW); // Clear the trigger pin
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); // Activate the sensor for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); // Activate the sensor for 10 microseconds

  long duration = pulseIn(ECHO_PIN, HIGH); // Read the echo pin; pulseIn returns the duration in microseconds
  int distance = duration * 0.034 / 2; // Calculate the distance (Speed of sound wave divided by 2 (go and return))

  if (distance > 0 && distance < serveRequestMaxRange) { // check if the detection signal is in range
    startTime = micros();
    Serial.println("Serve Request Detected!");
    return true;
  }
  return false;
}
