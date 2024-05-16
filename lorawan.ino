#define ledpin 2
int toggleState_1 = 0;
void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  Serial.println("AT+ADDRESS=0");  //0~65535
  delay(500);
  Serial.println("AT+NETWORKID=3");  //=3~15,18(default18)
  delay(500);
  Serial.println("AT+BAND=868000000");  //868Mh
  delay(1000);
}
void loop() {
  if (toggleState_1 == 0) {
    Serial.println("AT+SEND=155,5,PREND");  // Direccion, Longitud, Mensage
    digitalWrite(ledpin, HIGH);
    toggleState_1 = 1;
    delay(1500);
  } else {
    digitalWrite(ledpin, LOW);
    toggleState_1 = 0;
    delay(1500);
  }
}
