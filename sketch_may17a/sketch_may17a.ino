//float r = 0.01;
const int tempo = 10000; // us
float b = 0.5;
const float ths[2] = {0.05, 0.95};

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  static int out = 1;
  static float y=1, th=ths[!out];
  static float prev_micros = micros();
  float x;
  x = digitalRead(2);
  /*
  Serial.print(micros() - prev_micros);
  Serial.print(" ");
  */
  Serial.print(x);
  Serial.print(" ");
  y += /* r * */ (micros() - prev_micros) / tempo * b * (x - y);
  y = min(1, y);
  y = max(0, y);
  if(y > th && th == ths[1]) {
    out = 1;
  }
  else if(y < th && th == ths[0]) {
    out = 0;
  }
  th = ths[!out];
  prev_micros = micros();
  Serial.print(y);
  Serial.print(" ");
  Serial.println(out);
//  delay(1/r - 10 + random(20));
}
