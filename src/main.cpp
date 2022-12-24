#include <M5Atom.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SGP30.h>
#include <ssid_define.h>

Adafruit_SGP30 sgp30;
long last_millis = 0;
// char ssid[33] = "INSERT_SSID_HERE";
// char psk[65] = "INSERT_PSK_HERE";
WebServer webServer(80);

void handleRequest(void);

void setup() {
    M5.begin(true, false, true);
    delay(50);

    Serial.begin(9600);
    delay(50);

    if (!sgp30.begin()) {
        Serial.print("Failed connect to SGP30.\n");
        while(1);
    }

    // WiFi.begin(ssid, psk);
    WiFi.begin(MY_SSID, MY_PSK);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("IPv4: ");
    Serial.println(WiFi.localIP());

    webServer.on("/", HTTP_GET, handleRequest);
    webServer.begin();

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

    webServer.handleClient();

    delay(500);
}

void handleRequest(void) {
    String buf = "";

    buf += "{\"TVOC\":";
    buf += sgp30.TVOC;
    buf += ",";
    buf += "\"eCO2\":";
    buf += sgp30.eCO2;
    buf += "}";

    webServer.send(200, "application/json", buf);
}
