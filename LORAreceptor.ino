#define ledpin 2
String incomingString;
void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  Serial.println("AT+ADDRESS=155");  //0~65535
  delay(500);
  Serial.println("AT+NETWORKID=3");  //=3~15,18(default18)
  delay(500);
  Serial.println("AT+BAND=868000000");  //868Mh
  delay(1500);
}
void loop() {
  if (Serial.available()) {
    incomingString = Serial.readString();
    if (incomingString.indexOf("PREND") > 0) {
      digitalWrite(ledpin, HIGH);
      delay(500);
      digitalWrite(ledpin, LOW);
    } else {
      digitalWrite(ledpin, LOW);
    }
  }
}
