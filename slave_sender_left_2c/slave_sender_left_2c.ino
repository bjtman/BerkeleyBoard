//---------------
// Brian Tice
// 5-2-2013
//---------------
// Slave-Left Version 2c

// Summary: expanded to all 48 muxes. Changed to compressed bit array for sensor data.

// Code for and i2c write by Nick Gammon
// http://arduino.cc/forum/index.php/topic,104732.0.html

#include <Wire.h>

// I2C ADDRESS CONSTANTS
#define MASTER 2
#define SLAVELEFT 3
#define SLAVERIGHT 4

// Slave origin byte constant. 
#define LEFTSLAVEADDRESSBYTE 0

// Threshhold Constants for on/off of buttons
#define LOWTHRESH    50
#define HIGHTHRESH   90

//Give convenient names to the control pins
#define CONTROL0 5    
#define CONTROL1 4
#define CONTROL2 3
#define CONTROL3 2

//Create arrays for data from the the MUXs
int      mux0array[16];
int      mux1array[16];
int      mux2array[16];
//boolean  Stateof_Leftswitches[16];


struct 
{
   boolean SlaveOrigin;               // 0 means left. 1 means right
   long    leftSwitchOnOffArray[2];   // Two long's which house compressed on/off states of all 48 switches
                                      // only first 24 bits of each long are used. The last 8 bits are unused (don't care) values. 
} LeftSensorData;

void setup()
{
  
  // This is a boolean value in the struct that acts as header information to the master. This byte tells which slave the data packet is coming from.
  LeftSensorData.SlaveOrigin = LEFTSLAVEADDRESSBYTE;
  
  //Set MUX control pins to output
  pinMode(CONTROL0, OUTPUT);
  pinMode(CONTROL1, OUTPUT);
  pinMode(CONTROL2, OUTPUT);
  pinMode(CONTROL3, OUTPUT);
  Wire.begin(SLAVELEFT);                // join i2c bus with address #2  
}

void loop()
{
 
  //This for loop is used to scroll through and store the 16 inputs on the FIRST multiplexer
  for (int i=0; i<16; i++)
  {
    //The following 4 commands set the correct logic for the control pins to select the desired input
    digitalWrite(CONTROL0, (i&15)>>3); 
    digitalWrite(CONTROL1, (i&7)>>2);  
    digitalWrite(CONTROL2, (i&3)>>1);  
    digitalWrite(CONTROL3, (i&1));     
    
    //Read and store the input value at a location in the array
    mux0array[i] = analogRead(0);
    delay(40);
  }
  
  //This for loop is used to scroll through and store the 16 inputs on the SECOND multiplexer
  for (int i=0; i<16; i++)
  {
    //The following 4 commands set the correct logic for the control pins to select the desired input
    digitalWrite(CONTROL0, (i&15)>>3); 
    digitalWrite(CONTROL1, (i&7)>>2);  
    digitalWrite(CONTROL2, (i&3)>>1);  
    digitalWrite(CONTROL3, (i&1));     
    
    //Read and store the input value at a location in the array
    mux1array[i] = analogRead(1);
    delay(40);
  }
  
  //This for loop is used to scroll through and store the 16 inputs on the FIRST multiplexer
  for (int i=0; i<16; i++)
  {
    //The following 4 commands set the correct logic for the control pins to select the desired input
    digitalWrite(CONTROL0, (i&15)>>3); 
    digitalWrite(CONTROL1, (i&7)>>2);  
    digitalWrite(CONTROL2, (i&3)>>1);  
    digitalWrite(CONTROL3, (i&1));     
    
    //Read and store the input value at a location in the array
    mux2array[i] = analogRead(2);
    delay(40);
  }
  
 
  //populate our two long variables with compressed binary sensor data:
  for(int index=0;index<16;index++)
  {
    //We want to clear the bit that
    if(mux0array[index] < LOWTHRESH) 
    {
      LeftSensorData.leftSwitchOnOffArray[0] &= ~(1 << index);
    }
    if(mux0array[index] > HIGHTHRESH)
    {
      LeftSensorData.leftSwitchOnOffArray[0] |= (1 << index);
    }
  }
  
  for(int indexTwo=0;indexTwo<16;indexTwo++)
  {
    //We want to clear the bit that
    if(mux1array[indexTwo] < LOWTHRESH) 
    {
      LeftSensorData.leftSwitchOnOffArray[0] &= ~(1 << (indexTwo + 16));
    }
    if(mux1array[indexTwo] > HIGHTHRESH)
    {
      
      LeftSensorData.leftSwitchOnOffArray[0] |= (1 << (indexTwo + 16));
        
    }
      
  }
  
  
  //populate our two long variables with compressed binary sensor data:
  for(int indexThree=0;indexThree<16;indexThree++)
  {
    //We want to clear the bit that
    if(mux2array[indexThree] < LOWTHRESH) 
    {
      LeftSensorData.leftSwitchOnOffArray[1] &= ~(1 << indexThree);
    }
    if(mux2array[indexThree] > HIGHTHRESH)
    {
      LeftSensorData.leftSwitchOnOffArray[1] |= (1 << indexThree);
    }
  }
  
     
  Wire.beginTransmission (MASTER);
  Wire.write ((byte *) &LeftSensorData, sizeof LeftSensorData);
  Wire.endTransmission ();
  delay(10);
  
}


