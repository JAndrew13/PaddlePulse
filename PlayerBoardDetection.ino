// INNOVATIVE PADDLE PULSE TECHNOLOGY
// PLAYER BOARD SENSOR CONTROLLER
// @JAndrew13 on Github

// ------------------ Hardware Setup -------------------- //
  // Ball detection pins
  #define VIBE_PIN_1 A0 // Sensor output connected to A0
  #define VIBE_PIN_2 A1 // Sensor output connected to A1
  #define VIBE_PIN_3 A2 // Sensor output connected to A2

  // ultrasonic Sensor detection pins 
  #define TRIG_PIN 9 // sends signal to ultrasonic sensor
  #define ECHO_PIN 10 // listens to ultrasonic sensor response

  // Signal response pins
  #define SERVE_REQUEST_PIN 11 // ServeRequest pin with connection to scoreboard
  #define BALL_OUT_PIN 12  // HitDetection pin with connection to scoreboard

// ------------- Constants/Variables Setup -------------- //
  // Declare and initialize adjusttable threshold variables
  const int hitThresholdLow = 10; 
  const int hitThresholdHigh = 1000;

  // Define Serve-Start Sensor related constants
  const int serveRequestMaxRange = 2; // detection range in cm
  unsigned long serveStartTime;
  
  bool isServeRequested = false;

// -------------- Hardware Initialization --------------- //
  void setup() {
    pinMode(VIBE_PIN_1, INPUT);
    pinMode(VIBE_PIN_2, INPUT);
    pinMode(VIBE_PIN_3, INPUT);
    pinMode(TRIG_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT);
    pinMode(SERVE_REQUEST_PIN, OUTPUT);
    pinMode(BALL_OUT_PIN, OUTPUT);

    // Interrupt declaration for vibration sensors
    attachInterrupt(digitalPinToInterrupt(VIBE_PIN_1), hitDetected, CHANGE);
    attachInterrupt(digitalPinToInterrupt(VIBE_PIN_2), hitDetected, CHANGE);
    attachInterrupt(digitalPinToInterrupt(VIBE_PIN_3), hitDetected, CHANGE);

    // Initialize sensors LOW
    digitalWrite(SERVE_REQUEST_PIN, LOW);
    digitalWrite(VIBE_PIN_1, LOW);
    digitalWrite(VIBE_PIN_2, LOW);
    digitalWrite(VIBE_PIN_3, LOW);
    
    Serial.begin(9600); // Start serial communication at 9600 baud rate
  }

// --------------------- Main Loop ---------------------- //
  void loop() {
    checkServeSignal();
  }

// -------------------- Hit Detection ------------------- //
  void hitDetected() {
    verifyHitSignal(VIBE_PIN_1);
    verifyHitSignal(VIBE_PIN_2);
    verifyHitSignal(VIBE_PIN_3);
  }

  void verifyHitSignal(int pin) {
    int sensorValue = analogRead(pin);
    if (isHitSignalInRange(sensorValue)) {

      //-- Hit detection logic --//
        digitalWrite(BALL_OUT_PIN, HIGH);
        delay(10); // short pulse to signal hit
        digitalWrite(BALL_OUT_PIN, LOW);

        Serial.print("Valid hit detected on pin: ");
        Serial.print(pin);
    }
  }

  bool isHitSignalInRange(int sensorValue) {
    return (sensorValue > hitThresholdLow && sensorValue < hitThresholdHigh);
  }

// -------------------- Serve Request ------------------- //
  void checkServeSignal() //Check the serve sensor and returns the distance in cm
  {
    digitalWrite(TRIG_PIN, LOW); // Clear the trigger pin
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH); // Activate the sensor for 10 microseconds
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW); // Activate the sensor for 10 microseconds

    long duration = pulseIn(ECHO_PIN, HIGH); // Read the echo pin; pulseIn returns the duration in microseconds
    int distance = duration * 0.034 / 2; // Calculate the distance (Speed of sound wave divided by 2 (go and return))

    if (distance > 0 && distance < serveRequestMaxRange) { // check if the detection signal is in range
      digitalWrite(SERVE_REQUEST_PIN, HIGH);
    }
    digitalWrite(SERVE_REQUEST_PIN, LOW);
  }
