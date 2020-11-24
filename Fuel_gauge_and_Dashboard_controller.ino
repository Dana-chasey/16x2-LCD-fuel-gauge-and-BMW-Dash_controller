#include <mcp2515_defs.h>
#include <Canbus.h>
#include <mcp2515.h>
#include <defaults.h>
#include <global.h>
#include <Tone.h>  // "Tone by Brett Hagman" in Library Manager

#include <fuelgauge.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Gauge gauge(14000, -5000, 5);   //Power USING

Tone E36TachTone;

int switches = 0;
int kwPlays = 0;
int rpmPlays = 0;    
int RPMt;
int lastMsg = 3;
int energy;
int negEnergy;
int fuel;
void setup() {
  gauge.setup(lcd);
  Serial.begin(9600);
  lcd.display();
  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");

   //#####TACH#####
    E36TachTone.begin(2);
  //35=1k, 129=4k, 222=7k
  //28.5 31 31.53
   
    delay(1000);
   
  //RPM sweep
    for(int t=31; t<222; t++){
      E36TachTone.play(t);
      delay(2);
    }
    delay(500);
    for(int t=222; t>31; t--){
      E36TachTone.play(t);
      delay(2);
    }
    E36TachTone.play(31);
    
  
}

void loop() {
    tCAN message;
    long compare;
    int cRPM;           
    
    E36TachTone.play(RPMt);
if(lastMsg == 3){
  if (mcp2515_check_message()){
    if (mcp2515_get_message(&message)){ 
         //Serial.println(message.id,HEX);
         if(message.id == 0x298){
        
        
           cRPM = message.data[6] * 256;
           cRPM += message.data[7];
           cRPM -= 10000;
           //gauge.displayRPM(compare);
           Serial.print("RPM: ");
           Serial.print(cRPM);
           Serial.print(" ");
              
                //#####TACH LOOP#####
           Tone E36TachTone;
           // E36TachTone.begin(2);
           int RPM = cRPM;  //input RPM read from CAN here IF car is in ready state else stop RPM gauge
           
  
           RPMt = RPM/30.5;
  
           if (RPMt < 31){ //stops rpm gauge signal going below 31 Hz for idle(causes issues)
              RPMt = 31;     
           }
           
           Serial.print(" tone: ");
           Serial.print(RPMt);
           Serial.print(" ");
           //delay(200);

           int temp = message.data[3]-45;
              temp = (80 - temp);
              Serial.print(temp);
              Serial.print(" ");
              
              Serial.print(temp);
              Serial.print(" ");
              temp *= 1.5;
              Serial.print(temp);
              Serial.print(" ");
              
              temp += 10;
              Serial.print(temp);
              
              
              //#####TEMP#####  
            analogWrite(6, temp); //sets temp to middle pos
            
            // 6=HOTFlashing 10=HighesTemp 15=HOTLightOn, 50=MID, 100=COLD
            // Use 10 - 100 for temp range, and 6 as a set value for flashing HOT
           Serial.print(" RPM Plays: ");
            Serial.print(rpmPlays);
            rpmPlays += 1;
            if(rpmPlays > 1){
              lastMsg = 1;
              Serial.println(" ");
            }
           
          
        }  
    }
}
}          

if(lastMsg == 2){
   if (mcp2515_get_message(&message)){ 
   if(message.id == 0x374){
      fuel = (message.data[1])/2.56;
      Serial.print("Fuel percentage: ");
      Serial.print(fuel);
      Serial.println(" ");
      //Serial.println("--------------------");
      gauge.displayFuel(fuel);
      switches = 0;
      lastMsg = 3;   
   }
   }
}

if(lastMsg == 1){
   if (mcp2515_get_message(&message)){ 
   if(message.id == 0x373){
      compare = message.data[2] * 256;
      compare += (message.data[3] - 32700);
      compare *= -1;
             
      gauge.displayEnergy(compare);
      Serial.println("energy: ");
      Serial.print(compare);
      Serial.println("  ");
              
      //Serial.println(highest);
      //delay(200);
      //-16225
      rpmPlays = 0;
      kwPlays += 1;
      if(kwPlays > 0){
        if(switches > 4){
          lastMsg = 2; 
        }
        kwPlays = 0;
        switches += 1;
        if(lastMsg != 2){
          lastMsg = 3;
        }
      } 
   }   
   }
   
}   
}

