void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

int blinking = 0;
int blink_cnt = 0;
millis;

void loop() {
  if (Serial.available()>0)
  {
    int sentByte = Serial.read();

    if(sentByte == 49){
      blinking = 0;
      Serial.print("char 1: led on\n");
      digitalWrite(13, HIGH);
    }
    else if(sentByte == 50){
      blinking = 0;
      Serial.print("char 2: led off\n");
      digitalWrite(13, LOW);
    }
    else if(sentByte == 51){
      Serial.print("char 3: blink\n");
      blinking = 1;
      blink_cnt = 0;

    }
    else Serial.print("Nu am receptionat niciuna dintre optiuni.\n");
  }
}

