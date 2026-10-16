const int led_pins[6] = {8, 9, 10, 11, 12, 13};

int mode[6] = {0, 0, 0, 0, 0, 0};
unsigned long period_ms[6] = {1000, 1000, 1000, 1000, 1000, 1000};
unsigned long start_ms[6] = {0, 0, 0, 0, 0, 0};
byte level[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  for (int i=0; i<6; i++) {
    pinMode(led_pins[i], OUTPUT);
  }
}

void loop() {
  //parse "<pin>,<mode>,<duration>" commands from serial
  if (Serial.available()) {
    int pin_num = Serial.parseInt();
    Serial.read();

    String cmd = Serial.readStringUntil(',');
    cmd.trim();

    long dur = Serial.parseInt();
    if (dur <= 0) {
      dur = 1000;
    }

    int ch = pin_num - 8;
    if (ch >= 0 && ch < 6) {
      period_ms[ch] = dur;
      start_ms[ch] = millis();

      if (cmd == "off") {
        mode[ch] = 0;
      } else if (cmd == "fadein") {
        mode[ch] = 1;
      } else if (cmd == "fadeout") {
        mode[ch] = 2;
      } else if (cmd == "fadeinout") {
        mode[ch] = 3;
      } else if (cmd == "blink") {
        mode[ch] = 4;
      }
    }
  }

  // update target brightness for each channel
  unsigned long now_ms = millis();
  for (int ch = 0; ch < 6; ch++) {
    if (mode[ch] == 0) {
      level[ch] = 0;
      continue;
    }

    unsigned long phase_ms = (now_ms - start_ms[ch]) % period_ms[ch];

    if (mode[ch] == 1) {
      level[ch] = (phase_ms * 255UL) / period_ms[ch];
    } else if (mode[ch] == 2) {
      level[ch] = 255 - ((phase_ms * 255UL) / period_ms[ch]);
    } else if (mode[ch] == 3) {
      unsigned long half_ms = period_ms[ch] / 2;
      if (phase_ms < half_ms) {
        level[ch] = (phase_ms * 255UL) / half_ms;
      } else {
        level[ch] = 255 - (((phase_ms - half_ms) * 255UL) / half_ms);
      }
    } else if (mode[ch] == 4) {
      level[ch] = (phase_ms < period_ms[ch] / 2) ? 255 : 0;
    }
  }

  static byte pwm_tick = 0;
  pwm_tick++;
  for (int ch = 0; ch < 6; ch++) {
    digitalWrite(led_pins[ch], level[ch] > pwm_tick ? HIGH : LOW);
  }
}