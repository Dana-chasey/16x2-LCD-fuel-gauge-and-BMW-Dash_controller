/*
	Notes:
	Constructor (Gauge::Gauge(int,int,int)) should be initialized globally and Gauge::setup(lcd) should be called in setup
	
	Gauge::displayGauge() is used to update lcd after any change is made to either energy or fuel
*/


#ifndef fuelgauge_h
#define fuelgauge_h
#include <Adafruit_RGBLCDShield.h>

class Gauge
{
	public:
		Gauge(int MaxEnergy, int MinEnergy, int LineIndex); //Constructor sets Max/Min power draw and where... 
								    //along the indicator line sits		
		void setup(Adafruit_RGBLCDShield Lcd);		    //Sets up gauge and selects lcd		
		void displayFuel(int fuel);			    //can be used to update fuel or energy(displays) seperatly
		void displayEnergy(int energy); 	
	private:	
		int indoff;		  			    //index offset	 
		int minEnergy;	
		int maxEnergy;
		Adafruit_RGBLCDShield lcd;  
		void printRemainder(int remainder);  		              
};

#endif
