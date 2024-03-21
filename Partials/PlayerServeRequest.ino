// Define the ultrasonic sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 10
#define OUTPUT_PIN 7

const int detectionRange = 3 // detection range in cm

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  // Ensure the trigger pin is set to LOW initially
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by setting the TRIG_PIN high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin; pulseIn returns the duration in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  long distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and return)

  // Check if the distance is less than 4cm
  if (distance > 0 && distance < detectionRange) {
    // Send digital out signal
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(50); // Short delay to ensure signal is received
    digitalWrite(OUTPUT_PIN, LOW);

    // Stop detecting for 5 seconds
    delay(5000);
  }

  delay(10); // Short delay before next measurement
}
