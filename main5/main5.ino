#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <MPU6050.h>
// #include <SoftwareSerial.h>


// Pin definitions
#define MPU_INT_PIN 2 // Pin where the MPU6050 interrupt is connected
#define SWITCH_PIN 3 // pin for manual switch
#define PIR_PIN  5 // pin for reading motion sensor 
#define RELAY_PIN 7 // pin for relay switch
#define TRIGGER_PIN  10  // Pin for the trigger of the ultrasonic sensor
#define ECHO_PIN     11 // Pin for the echo of the ultrasonic sensor
#define SERVO_PIN    12 // Pin for the servo motor
#define RX 4 // TX of esp8266 in connected with Arduino pin 4
#define TX 6 // RX of esp8266 in connected with Arduino pin 6
#define LM35_PIN A2 // analog pin for LM35 output

// distance sensor constant
#define MAX_DISTANCE 300 // Maximum distance to measure (in cm)

//servo motor constants
#define SERVO_ANGLE_START 0  // Starting angle for the servo motor
#define SERVO_ANGLE_END 180   // Ending angle for the servo motor
#define NUM_SAMPLES 15  // Number of samples (120 degrees / 20 degrees = 7 samples)

// Constants
const unsigned long motionTimeout = 5000; // 5 seconds
unsigned long lcdUpdateInterval = 5000; // Time of the last LCD update
// String WIFI_SSID = "abcd";// WIFI NAME
// String WIFI_PASS = "esp12345"; // WIFI PASSWORD
// String API = "JXM1FZGSLOJPNBB4";// Write API KEY
// String HOST = "api.thingspeak.com";
// String PORT = "80";

// Variables
unsigned long lastMotionTime = 0;   // Time when motion was last detected
bool distanceSensorActive = false;  // Distance sensor active flag
unsigned long lastLCDUpdateTime = 0;   // Time when motion was last detected
int referenceDistances[NUM_SAMPLES]; // where initial mapping is stored
int countTrueCommand;
int countTimeCommand; 
boolean found = false;
float ambientTemperatureC;
bool isAmbientTemperatureSet = false;   
const int numSamples = 10;
float temperatureHistory[numSamples];
int currentIndex = 0;
float temperatureSum = 0;
float current;
float power;
float MPUsteadystate[numSamples];
unsigned long lastEspUpdateTime;
int16_t ax, ay, az;
volatile bool relay_updateLCD = false;
volatile bool earthquakeDetected = false;

LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD setup
Servo servo; // Servo object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // distance sensor object
MPU6050 mpu; // Create an object for the MPU6050
// SoftwareSerial esp8266(RX,TX); 

void setup() {
  Serial.begin(9600);
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCD Initialized");
  delay(1000);
  lcd.clear();
  
  // initialize ESP
  // esp8266.begin(115200);
  // sendCommand("AT",5,"OK");
  // sendCommand("AT+CWMODE=1",5,"OK");
  // sendCommand("AT+CWJAP=\""+ WIFI_SSID +"\",\""+ WIFI_PASS +"\"",20,"OK");
  // Serial.println("initialized ESP");

  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);  // Set the relay pin as an output
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Set switch pin as input with internal pull-up resistor
  pinMode(MPU_INT_PIN, INPUT);  // Set the MPU6050 interrupt pin as input

  // Initialize the relay to the on state (HIGH for active-low relay)
  digitalWrite(RELAY_PIN, LOW);  

  // interrupts
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), toggleRelay, CHANGE); // Attach interrupt to switch pin on changing state
  attachInterrupt(digitalPinToInterrupt(MPU_INT_PIN), earthquakeDetect, RISING); // Attach interrupt to the MPU6050 interrupt pin

  // Attach the servo to the specified pin
  servo.attach(SERVO_PIN);

  // Initialize distance, motion, servo, and accelerometer
  initializeSensors();
}

void loop() {
  // Read PIR sensor state
  int pirState = digitalRead(PIR_PIN);
  Serial.println("pirState: ");
  Serial.print(pirState);

  if (pirState == HIGH) {
    // Motion detected
    lastMotionTime = millis();  // Update the last motion time
    distanceSensorActive = false; // Deactivate distance sensor

    // Automatically turn on relay if motion is detected and switch is on
    if (millis() - lastLCDUpdateTime > lcdUpdateInterval) {
      int relayState = digitalRead(RELAY_PIN);
      int switchState = digitalRead(SWITCH_PIN);
      if (relayState && switchState) {
        digitalWrite(RELAY_PIN, LOW);  // turn on relay pin
        lcd.backlight(); // turn on LCD
      }
    }
  }

  // Check if no motion has been detected for 10 seconds
  if (millis() - lastMotionTime > motionTimeout) {
    int relayState = digitalRead(RELAY_PIN);
    if (!distanceSensorActive && !relayState) {
      distanceSensorActive = true;  // Activate distance sensor
      lcd.setCursor(0, 0);
      lcd.print("No motion for 20s");
      lcd.setCursor(0, 1);
      lcd.print("Comparing map...");
      delay(3000);
      lcd.clear();
      compareDistanceMap();
    }
  }

  //Update ESP every 15 s
  if (millis() - lastEspUpdateTime > 15000) {
    powerESPupdate();
    lastEspUpdateTime = millis();
  }

  if (relay_updateLCD){
    volatile bool switchState = digitalRead(SWITCH_PIN);
    if (switchState) {
      lcd.backlight(); // turn on backlight if switch is on 
    } else {
      lcd.noBacklight(); // turn off backlight if switch is off
    }
    relay_updateLCD = false;
  }

  if (earthquakeDetected) {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Earthquake detected!");
    delay(2000);
    lcd.clear();
    earthquakeDetected = false;
  }
}





