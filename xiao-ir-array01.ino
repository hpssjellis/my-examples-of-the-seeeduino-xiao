/***************************************************************************
 * 
 * my14_8x8_thermal_Array.ino
 * Connections on Portenta H7
 * GND, 3V3, SCL, SDA connections from Portenta to Thermal array. 
 * Portenta ---> Thermal Array
 *    GND   --->   GND
 *    3V3   --->   VIN
 *    SCL   --->   SCL
 *    SDA   --->   SDA
 * 
 * Using the AMG8833 IR 88 Thermal Imager Array Temperature Sensor Module 8x8 Infrared Camera Sensor 
 * From Amazon.com about $52 
 * 
 * Sketch by Jeremy Ellis
 * Twitter @rocksetta
 * 
 * 
 * 
 * 
  This is a library for the AMG88xx GridEYE 8x8 IR camera

  This sketch tries to read the pixels from the sensor

  Designed specifically to work with the Adafruit AMG88 breakout
  ----> http://www.adafruit.com/products/3538

  These sensors use I2C to communicate. The device's I2C address is 0x69

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
bool myFever = false;

//Calibrate with ICe and hot water that you know the temperature of. Might change with time.
float myCalibration = 9.4;   // +- Degrees to calibrate. This might totally depend on your device, may change with time.
float myError = 0.3;         // +- degrees different to print
float myTempConcern = 39;    // +- Temperature to check. Depends on myCalibration being correct
float myOldTotal;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    bool status;    
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }  

    delay(100); // let sensor boot up
}


void loop() { 
    //read all the pixels
    amg.readPixels(pixels);
    digitalWrite(LED_BUILTIN, HIGH);// sets it off
    
    float myTemp1 = 0;     
    float myTemp2 = 0;  
    float myTotal = 0;
    float myHighest = 0;

    for(int i=1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++){
        float myIncoming = pixels[i-1];
        myTotal += myIncoming;
        if (myIncoming > myHighest){myHighest = myIncoming;} // grab the largest
        if (myIncoming +myCalibration > myTempConcern){  // raw score for generic uncalibrated face temperature
           if (myTemp1 == 0){ 
             myTemp1 = myIncoming; // allow one false reading
           } else {
              myTemp2 = myIncoming;
            }
        }   
        
    }
    if (myTemp2 != 0){
       myFever = true;   // once set don't unset.
      }

    if ( myTotal > myOldTotal + (1*myError*64) || myTotal < myOldTotal + (-1*myError*64) ){  // just checking for change, default myError = 0.3    
         digitalWrite(LED_BUILTIN, LOW);
         delay(1000);      
         digitalWrite(LED_BUILTIN, HIGH);
         delay(200);      
         Serial.println("8x8 IR array Average Temperature: "+ String(myCalibration+myTotal/64)+ ", Highest Temp: "+String(myCalibration+myHighest));    
        }
  
    myOldTotal = myTotal;

    if (myFever){
      for (int j=0; j<= 60; j++){
         digitalWrite(LED_BUILTIN, LOW);
         delay(50);
         digitalWrite(LED_BUILTIN, HIGH);
         delay(50);
       }
       myFever = false; // reset fever        
       //digitalWrite(LED_BUILTIN, LOW);   /// why this
      }

    //delay a second
    delay(5);
}
