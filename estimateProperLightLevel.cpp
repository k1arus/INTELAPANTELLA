// Read realtime light level and display it via serial port.

#include <VendorUtilities.h> // Modify properly with SchooMy Block Editor (https://fox.schoomy.com/boards/editor)
VendorUtilities vendorUtils = VendorUtilities(); // Modify properly with SchooMy Block Editor (https://fox.schoomy.com/boards/editor)

float brightness = 0;

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
  if (true) {
    brightness = _sbeGetBrightness(A5, 1023, 5);
    Serial.println(brightness);
  }
}

