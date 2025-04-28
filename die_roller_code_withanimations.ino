#include <LiquidCrystal_I2C.h>
#include <stdlib.h>

// --- LCD Configuration ---
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS    4

// --- Button Configuration ---
#define BUTTON_PIN 8

// --- Buzzer Configuration ---
#define BUZZER_PIN 6

// --- Debounce Delay ---
#define DEBOUNCE_DELAY 100

// --- Animation Constants ---
#define ANIMATION_DELAY 80
#define ANIMATION_FRAMES 16

// --- Global Variables ---
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
int buttonState = LOW;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
int diceValue = 1;
bool gameStarted = false;

// --- Function Declarations ---
void displayDiceValue(int value);
void playTone(unsigned int frequency, unsigned long duration);
void displayFancyDiceAnimation();
void displayLargeDiceValue(int value);
void displayStartScreen(); //remove
void playStartScreenAnimation(); //remove

void setup() {
  // --- Serial Initialization ---
  Serial.begin(9600);
  Serial.println("Dice Roller Setup...");

  // --- LCD Initialization ---
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // --- Button Initialization ---
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonState = digitalRead(BUTTON_PIN);

  // --- Buzzer Initialization ---
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // --- Seed the random number generator ---
  randomSeed(analogRead(0));

  // --- Initial Display ---
  displayStartScreen(); //show
  Serial.println("Setup Complete.");
}

void loop() {
  // --- Read button with debouncing ---
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      // --- Check for button press (LOW because of INPUT_PULLUP) ---
      if (buttonState == LOW) {
        gameStarted = true; //set
        lcd.clear();
        displayFancyDiceAnimation();
        diceValue = random(1, 7);
        Serial.print("You rolled: ");
        Serial.println(diceValue);
        displayDiceValue(diceValue); // Use the function to display dot pattern
        playTone(1000, 200);
        gameStarted = false;
      }
    }
  }

  lastButtonState = reading;
}

// --- Function to display dice value ---
void displayDiceValue(int value) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You rolled:");
  displayLargeDiceValue(value);
}

// --- Function to play tone ---
void playTone(unsigned int frequency, unsigned long duration) {
  tone(BUZZER_PIN, frequency);
  delay(duration);
  noTone(BUZZER_PIN);
}

// --- Function to display fancy dice animation ---
void displayFancyDiceAnimation() {
  const char animFrames[ANIMATION_FRAMES][LCD_COLUMNS] = {
    "|o       o|       ",
    "|  o   o  |       ",
    "|o   o   o|       ",
    "|    o    |       ",
    "|o       o|       ",
    "|  o   o  |       ",
    "|o   o   o|       ",
    "|o  o o  o|       ",
    "|o  o o  o|       ",
    "| ooooooo |       ",
    "|ooooooo  |       ",
    "|  oooo   |       ",
    "|   ooo   |       ",
    "|    o    |       ",
    "|         |       ",
    "|         |       "
  };

  for (int i = 0; i < ANIMATION_FRAMES; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rolling...");
    lcd.setCursor(0, 1);
    lcd.print(animFrames[i]);
    delay(ANIMATION_DELAY);
  }
}

// --- Function to display large dice value ---
void displayLargeDiceValue(int value) {
  lcd.clear();
  switch (value) {
    case 1:
      lcd.setCursor(8, 0); lcd.print(" ");
      lcd.setCursor(8, 1); lcd.print("o");
      lcd.setCursor(8, 2); lcd.print(" ");
      lcd.setCursor(8, 3); lcd.print(" ");
      break;
    case 2:
      lcd.setCursor(12, 0); lcd.print("o");
      lcd.setCursor(8, 1); lcd.print(" ");
      lcd.setCursor(8, 2); lcd.print(" ");
      lcd.setCursor(4, 3); lcd.print("o");
      break;
    case 3:
      lcd.setCursor(12, 0); lcd.print("o");
      lcd.setCursor(8, 1); lcd.print("o");
      lcd.setCursor(8, 2); lcd.print(" ");
      lcd.setCursor(4, 3); lcd.print("o");
      break;
    case 4:
      lcd.setCursor(4, 0); lcd.print("o"); lcd.setCursor(12, 0); lcd.print("o");
      lcd.setCursor(8, 1); lcd.print(" ");
      lcd.setCursor(4, 3); lcd.print("o"); lcd.setCursor(12, 3); lcd.print("o");
      break;
    case 5:
      lcd.setCursor(4, 0); lcd.print("o"); lcd.setCursor(12, 0); lcd.print("o");
      lcd.setCursor(8, 1); lcd.print("o");
      lcd.setCursor(4, 3); lcd.print("o"); lcd.setCursor(12, 3); lcd.print("o");
      break;
    case 6:
      lcd.setCursor(4, 0); lcd.print("o"); lcd.setCursor(12, 0); lcd.print("o");
      lcd.setCursor(4, 2); lcd.print("o"); lcd.setCursor(12, 2); lcd.print("o");
      lcd.setCursor(4, 3); lcd.print("o"); lcd.setCursor(12, 3); lcd.print("o");
      break;
  }
}

// --- Function to display the starting screen ---
void displayStartScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to Roll");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");
}

// --- Function to play start screen animation ---
void playStartScreenAnimation() {
  const char animFrames[ANIMATION_FRAMES][LCD_COLUMNS] = {
    "->                ",
    "  ->              ",
    "    ->            ",
    "      ->          ",
  };
  for (int i = 0; i < ANIMATION_FRAMES; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready to Roll");
    lcd.setCursor(0, 1);
    lcd.print("Press Button");
    lcd.setCursor(0, 2);
    lcd.print(animFrames[i]);
    delay(ANIMATION_DELAY);
  }
}
