#include <fuelgauge.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

Gauge gauge(200, -200, 8);

float count = 0;
int energy;
int fuel;

void setup() {
  gauge.setup(lcd);
  Serial.begin(9600);
  
}

void loop() {
  count += 0.65;
  energy = (sin(count) * 200);
  fuel = (sin(count) * 50) + 50;
  gauge.setEnergy(energy);
  gauge.setFuel(fuel);
  gauge.displayGauge();
  delay(500);
}
