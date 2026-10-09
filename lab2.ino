// int count = 0;
// unsigned long last_ms = 0;
//
// void setup() {
//   for (int i=8; i<=13; i++) {
//     pinMode(i, OUTPUT);
//   }
// }
//
// void loop() {
//   unsigned long now_ms = millis();
//   if (now_ms - last_ms >= 500) {
//     last_ms = now_ms;
//     for (int i=0; i<6; i++) {
//       digitalWrite(8 + i, (count >> bit) & 1);
//     }
//     count = (count + 1) % 64;
//   }
// }


// void setup() {
//   for (int i = 8; i <= 13; i++) {
//     pinMode(i, OUTPUT);
//   }
//   Serial.begin(9600);
// }
//
// void loop() {
//   if (Serial.available() > 0) {
//     int num = Serial.parseInt();

//     if (num >= 0 && num <= 63) {
//       for (int i=0; i<6; i++) {
//         digitalWrite(8 + i, bitRead(num, i));
//       }
//       Serial.println(num);
//     } else {
//       Serial.println("valoare invalida (0-63)");
//     }
//   }
// }

//fade
int led_pin = 9;
int brightness = 0;
int step = 5;
unsigned long last_ms = 0;

void setup() {
  pinMode(led_pin, OUTPUT);
}

void loop() {
  unsigned long now_ms = millis();
  if (now_ms - last_ms >= 30) {
    last_ms = now_ms;
    analogWrite(led_pin, brightness);
    brightness += step;
    if (brightness <= 0 || brightness >= 255) {
      step = -step;
    }
  }
}

// int out_pin = 12;
// unsigned long high_us = 0;
// unsigned long start_us = 0;
//
// void setup() {
//   pinMode(out_pin, OUTPUT);
//   Serial.begin(9600);
// }
//
// void loop() {
//   if (Serial.available() > 0) {
//     float volts = Serial.parseFloat();
//     if (volts >= 0 && volts <= 5) {
//       high_us = volts / 5.0 * 2000;
//       Serial.println(volts);
//     }
//   }
//
//   unsigned long now_us = micros();
//   if (now_us - start_us >= 2000) {
//     start_us = now_us;
//   }
//
//   if (now_us - start_us < high_us) {
//     digitalWrite(out_pin, HIGH);
//   } else {
//     digitalWrite(out_pin, LOW);
//   }
// }