#include <fuelgauge.h>
#include <Adafruit_RGBLCDShield.h>
#include <math.h>
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2

Gauge::Gauge(int MaxEnergy, int MinEnergy, int LineIndex)
{
  indoff = LineIndex;
  
  minEnergy = MinEnergy;
  maxEnergy = MaxEnergy;

}

void Gauge::setup(Adafruit_RGBLCDShield Lcd){
  lcd = Lcd;

  lcd.begin(16, 2);
  lcd.setBacklight(0x1);    

  uint8_t prog0[8] = {                           //Custom Characters
 	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
  };
  uint8_t prog1[8] = {
	0x0,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x0
  };
  uint8_t prog2[8] = {
	0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x0
  };
  uint8_t prog3[8] = {
	0x0,0x7,0x7,0x7,0x7,0x7,0x7,0x0
  };
  uint8_t prog4[8] = {
  	0x4,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x4
  };
  uint8_t prog5[8] = {
	0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4
  };
  uint8_t prog6[8] = {
	0x4,0x7,0x7,0x7,0x7,0x7,0x7,0x4
  };
  uint8_t prog7[8] = {
 	0x0,0xf,0xf,0xf,0xf,0xf,0xf,0x0
  };
  			              //Custom characters
  lcd.createChar(0, prog0);           //Blank
  lcd.createChar(1, prog1);           //Positive half
  lcd.createChar(2, prog2);           //Full
  lcd.createChar(3, prog3);           //Negative half
  lcd.createChar(4, prog4);           //Negative half indictor
  lcd.createChar(5, prog5);           //Indictor
  lcd.createChar(6, prog6);           //Positive half indictor
  lcd.createChar(7, prog7);           //Unknown
}

void Gauge::displayFuel(int fuel){                                 

  lcd.setCursor(0,1);
  
  int fuelConv = fuel * 0.55;
  int target = (fuelConv / 5) + 5;  
  int remainder = (fuelConv % 5);
  lcd.print("Fuel:");

  for(int i = 5; i <= 16; ++i){
	lcd.setCursor(i,1);
				
		if((i > 4)&&(i <= target)){
			if(i == target){			
				printRemainder(remainder);					
			}
			else{
				lcd.write(2);
			}
		}
		else
		{
			lcd.print(" ");
		}
		
	}
} 



void Gauge::displayEnergy(int energy){                   //Display Label and energy meter
  lcd.setCursor(0,0);
  lcd.print("Kw:"); 
  
  int multiplyer;
  if(energy >= 0){
	multiplyer = maxEnergy / (16 - indoff);
  }
  else{
    multiplyer = (minEnergy / (indoff -  2)) * -1;
  }

  if((energy <= 0)&&(energy >= minEnergy)){           //Colour indicator
      lcd.setBacklight(GREEN);
  }
  else if((energy <= (maxEnergy / 2))&&(energy > 0)){
      lcd.setBacklight(YELLOW);
  }
  else if((energy <= maxEnergy)&&(energy > (maxEnergy / 2))){
      lcd.setBacklight(RED);
  }
  
  int target = (energy / multiplyer) + indoff;
  int remainder = (energy % multiplyer)/(multiplyer/5);

  

	for(int i = 3; i <= 16; ++i){
		lcd.setCursor(i,0);
			
		if(i == indoff){
			if((energy >= -2)&&(energy <= 2)){
				lcd.write(5);
  			}
  			else if(energy < -2){
   				lcd.write(4);
			}
			else if(energy > 2){
				lcd.write(6);
			}
		}
		else
		{		
			if((i > indoff)&&(i <= target)){
				if(i == target){			
					printRemainder(remainder);					
				}
				else{
					lcd.write(2);
				}
			}
			else if((i < indoff)&&(i >= target)){
				if(i == target){			
					printRemainder(remainder);					
				}
				else{
					lcd.write(2);
				}
			}
			else
			{
				lcd.print(" ");
			}
		}
		
	}
  
    
}

void Gauge::printRemainder(int remainder){           //Determines what character to use for each remainder
  if (remainder >= 0){
     switch(remainder){
     case(0):
      lcd.write(0);
     case(1):
     case(2):
     case(3):
      lcd.write(1);
     break;
     case(4):
     case(5):
      lcd.write(2);
     break;
     }
   }
   else if(remainder < 0){
     switch(remainder){
     case(-1):
     case(-2):
     case(-3):
      lcd.write(3);
     break;
     case(-4):
     case(-5):
      lcd.write(2);
     break;
     }
   }
}

