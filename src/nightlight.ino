const int LDR_PIN = A0;     // the input pin of the LDR to detect room brightness
const int LED_PIN = 2;      // the output pin to control the LED
const int TRIG_PIN = 3;     // the trigger pin of the ultrasonic sensor to signal it
const int ECHO_PIN = 4;     // the echo pin of the ultrasonic sensor to get the time duration

const double DIST_THRESH = 5.0;   // the maxmimize distance to activate/deactivate the night light
const int LIGHT_THRESH = 300;     // the photoresistor threshold to turn on the night light

/** Whether the night light is currently active. */
static int activated = 1;

/** Sets up the initial state of the program. */
void setup() {
    Serial.begin(9600);
    pinMode(LDR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

/** Handles the operations of the ultrasonic sensor. */
void handle_sensor() {
    // clear trigger pin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // set trigger pin to high for 10 microseconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // read echo pin and calculate distance
    long dur = pulseIn(ECHO_PIN, HIGH);
    double dist = dur / 29.0 / 2.0;
    // log distance
    Serial.println("Object Distance: " + String(dist, DEC) + " cm");
    // check proximity (5 cm)
    if(dist <= DIST_THRESH) {
        // confirm detected proximity by blinking
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        if(activated) {
            // turn light off
            Serial.println("Turned night light off");
            activated = 0;
            // turn LED off
            digitalWrite(LED_PIN, LOW);
        } else {
            // turn light on
            Serial.println("Turned night light on");
            activated = 1;
            // turn LED on if dark
            update();
        }
        delay(3000);
    }
}

/** Reads a value from the LDR and updates the LED. */
void update() {
    int val = analogRead(LDR_PIN);
    Serial.println("Photoresistor Value: " + String(val, DEC));
    if(val < LIGHT_THRESH) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
}

/** The main loop of the program. */
void loop() {
    handle_sensor();
    if(activated) {
        update();
    }
    delay(100);
}
