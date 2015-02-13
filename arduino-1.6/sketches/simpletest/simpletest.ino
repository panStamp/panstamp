void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, !digitalRead(LED));
  
  delay(500);
}
