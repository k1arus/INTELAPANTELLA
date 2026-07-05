// If light is shone on the light sensor for 10 seconds, send “Success.”

#include <VendorUtilities.h> // Modify properly with SchooMy Block Editor (https://fox.schoomy.com/boards/editor)
VendorUtilities vendorUtils = VendorUtilities(); // Modify properly with SchooMy Block Editor (https://fox.schoomy.com/boards/editor)

float brightness = 0;

bool highActive = false;
bool triggered = false;
unsigned long highStartTime = 0;
const int THRESHOLD = 100;

float _sbeGetBrightness(int pinNumber, int res, float vol) {
    pinMode(pinNumber, INPUT);
    float cds_ad = analogRead(pinNumber);
    float cds_v = cds_ad * vol / res;
    float v_res = vol - cds_v;
    if (v_res < 0.01) {
        v_res = 0.01;
    }
    return 10000 * cds_v / v_res / 1000;
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    brightness = _sbeGetBrightness(A5, 1023, 5);

    if (brightness >= THRESHOLD) {

        if (!highActive) {
            highActive = true;
            highStartTime = millis();
            triggered = false;
        }

        if (!triggered && (millis() - highStartTime >= 10000)) {
            Serial.println("Success");
            Serial.println("Success");
            Serial.println("Success");
            triggered = true;
        }

    } else {
        highActive = false;
        triggered = false;
    }
}
