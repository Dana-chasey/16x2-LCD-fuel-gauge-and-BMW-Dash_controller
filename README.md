# 16x2-LCD-fuel-gauge
This is an aurduino library I created to display the energy level and current power draw of an electric car using a 16x2 RGB LCD display from adafruit.

Notes:
	Constructor (Gauge::Gauge(int,int,int)) should be initialized globally and Gauge::setup(lcd) should be called in setup
	
	Gauge::displayFuel(fuel) or Gauge::displayEnergy(energy) are used to update the lcd
