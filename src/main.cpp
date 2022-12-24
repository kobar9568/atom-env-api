#include <M5Atom.h>

void setup() {
    M5.begin(true, false, true);
    delay(50);
    Serial.begin(9600);
    Serial.print("Startup complete.\n");
}

void loop() {
    Serial.print("hello, world\n");
    delay(1000);
}
