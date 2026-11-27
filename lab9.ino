#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lastButton = 0;
unsigned long lastDebounceT = 0;
const unsigned long debounceDelay = 50;

int readLCDButtons() {
  int adc = analogRead(A0);
  if (adc > 1000) return 0; // none
  if (adc < 60)   return 1; // right
  if (adc > 110 && adc < 200) return 2; // up
  if (adc > 200 && adc < 270) return 3; // down
  if (adc > 390 && adc < 480) return 4; // left
  if (adc > 600 && adc < 650) return 5; // select
  return 0;
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Astept python...");
}

void loop() {
  int currentButton = readLCDButtons();

  if (currentButton != lastButton && (millis() - lastDebounceT) > debounceDelay) {
    lastDebounceT = millis();
    if (currentButton != 0) {
      switch (currentButton) {
        case 1: Serial.println("RIGHT"); break;
        case 2: Serial.println("UP"); break;
        case 3: Serial.println("DOWN"); break;
        case 4: Serial.println("LEFT"); break;
        case 5: Serial.println("SELECT"); break;
      }
    }
    lastButton = currentButton;
  }

  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("L1:")) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print(cmd.substring(3));
    } 
    else if (cmd.startsWith("L2:")) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(cmd.substring(3));
    }
  }
}