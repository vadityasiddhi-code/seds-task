#include <LiquidCrystal.h>

// Pin's allocation 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Defining constants
const int pingPin = 9; 
const int ldrPin = A0;
const int buttonPin = 8;
const int ledPin = 7;
const int buzzerPin = 6;


const int OPEN_SEA = 0;
const int ANCHOR_DROPPED = 1;
const int STORM = 2;
const int CHARYBDIS = 3;
const int WRECKED = 4;

int currentState = OPEN_SEA;


unsigned long dangerStartTime = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;


bool anchorActive = false;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  lcd.begin(16, 2);   // we have told arduino that screen has 16 rows and 2 columns
  
  pinMode(buttonPin, INPUT_PULLUP); // we are using the assigned pin 8 to check if the button is pressed or not using high and low voltage
  pinMode(ledPin, OUTPUT);// sending output to ledpin
  pinMode(buzzerPin, OUTPUT); // sending output of buzzer pin
  
  updateLCD("OPEN SEA"); // calling update lcd function
}

void loop() {
  if (currentState == WRECKED) {
    return; // stop all processing until simulation restarts
  }

  handleButton();
  updateStateMachine();
  executeStateActions();
}


void handleButton() {
  int reading = digitalRead(buttonPin);
  
  // Checking if button was just pressed
  if (reading == LOW && lastButtonState == HIGH) {
    // Check if enough time has passed 
    if (millis() - lastDebounceTime > debounceDelay) {
      anchorActive = !anchorActive; // Toggle anchor
      lastDebounceTime = millis();  // Reset timer
    }
  }
  
  lastButtonState = reading; // Update last state
}


void updateStateMachine() {
  if (anchorActive) {
    changeState(ANCHOR_DROPPED); 
    return;
  }
  
  // Read Sensors
  int lightLevel = analogRead(ldrPin); 
  bool isStorm = (lightLevel < 512); // Less than half of 1024 for storm
  bool isCharybdis = (getDistance() < 100); // Less than 100cm for chadrybis

  
  switch (currentState) {
    case OPEN_SEA:
    case ANCHOR_DROPPED:
      if (isStorm) {
        dangerStartTime = millis();
        changeState(STORM);// creating different scenarios
      } else if (isCharybdis) {
        dangerStartTime = millis();
        changeState(CHARYBDIS);
      } else {
        changeState(OPEN_SEA);
      }
      break;

    case STORM:
      if (!isStorm) {
        changeState(OPEN_SEA);
      } else if (millis() - dangerStartTime >= 5000) {
        changeState(WRECKED);
      }
      break;

    case CHARYBDIS:
      if (!isCharybdis) {
        changeState(OPEN_SEA);
      } else if (millis() - dangerStartTime >= 5000) {
        changeState(WRECKED);
      }
      break;
  }
}

void executeStateActions() {
  // LED Logic for Storm
  if (currentState == STORM) {
    if (millis() - lastBlinkTime >= 250) {
      lastBlinkTime = millis(); // blinking for storm
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Buzzer Logic for Charybdis
  if (currentState == CHARYBDIS) {
    tone(buzzerPin, 1000); // buzzer for storm
  } else {
    noTone(buzzerPin);
  }
}


void changeState(int newState) {
  if (currentState == newState) return;
  
  currentState = newState;
  
  switch (currentState) {
    case OPEN_SEA:      updateLCD("OPEN SEA"); break;
    case ANCHOR_DROPPED:updateLCD("ANCHOR DROPPED"); break;
    case STORM:         updateLCD("STORM"); break;
    case CHARYBDIS:     updateLCD("CHARYBDIS"); break;   // configuring the screen
    case WRECKED:       
      updateLCD("WRECKED"); 
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      break;
  }
}

void updateLCD(const char* text) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
}


long getDistance() {
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  
  pinMode(pingPin, INPUT);
  long duration = pulseIn(pingPin, HIGH, 30000); 
  
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}
