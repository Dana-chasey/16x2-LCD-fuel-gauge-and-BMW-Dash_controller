#include <mcp2515_defs.h>
#include <Canbus.h>
#include <mcp2515.h>
#include <defaults.h>
#include <global.h>

#include <fuelgauge.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Gauge gauge(3251, -3251, 8);

int energy;
int negEnergy;
int fuel;
void setup() {
  gauge.setup(lcd);
  Serial.begin(9600);
  lcd.display();
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

void loop() {
    tCAN message;
    long compare;
if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
    {
         Serial.println(message.id,HEX);
         switch(message.id){
          case(0x696):
              
              compare = message.data[6];
              compare *= 256;
              negEnergy = (compare + message.data[7] - 500) / -20;
              
              compare = message.data[2];
              compare *= 256;
              energy = (compare + message.data[3] - 500) / 20;

              if(negEnergy*-1>energy){
                energy = negEnergy;
              }

              Serial.print("Energy consumption: ");
              Serial.println(energy);
              Serial.println("--------------------");
              gauge.displayEnergy(energy);
          break;
          case(0x374):
               fuel = (message.data[1])/2.56;
               Serial.print("Fuel percentage: ");
               Serial.println(fuel);
               Serial.println("--------------------");
               gauge.displayFuel(fuel);
          break;
        }  
    }
  }
  //Serial.print(" Energy:");
  //Serial.println(energy);
  //Serial.print(" Fuel:");
  //Serial.println(fuel);
  delay(2000);
}
