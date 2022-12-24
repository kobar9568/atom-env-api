#include <M5Atom.h>
#include <Adafruit_SGP30.h>

Adafruit_SGP30 sgp30;
long last_millis = 0;

void setup() {
    M5.begin(true, false, true);
    delay(50);

    Serial.begin(9600);
    delay(50);

    if (!sgp30.begin()) {
        Serial.print("Failed connect to SGP30.\n");
        while(1);
    }

    Serial.print("Startup complete.\n");
}

void loop() {
    // Calibrete SGP30.
    static int i = 15;
    while (i > 0) {
        if (millis() - last_millis > 1000) {
            last_millis = millis();
            i--;
        }
    }

    if (!sgp30.IAQmeasure()) {
        Serial.println("Measurement failed.\n");
        return;
    }

    Serial.printf("TVOC:%d ppb\n",sgp30.TVOC);
    Serial.printf("eCO2:%d ppm\n",sgp30.eCO2);
    delay(500);
}
