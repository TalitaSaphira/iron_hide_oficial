void setup() {
    Serial.begin(115200);
    while (!Serial) {}
}

void loop () {
//    Serial.println("Hello from Arduino");
//    delay(1000);
    if(Serial.available() > 0){
        String message = Serial.readStringUntil('\n');

        //Use isso apenas ver se o código funciona e depois exclua
        Serial.println(message);
    }
}