
#define BUTTON_2    37
#define BUTTON_1    38
#define BUTTON_3    39
#define BUTTON_4    0
#define CUR_SENS_PIN  35
#define PAUZA_RELE_PIN    26
#define SPINDLE_RELE_PIN    19
#include <EEPROM.h>

bool active=false;
int page=1;
#include <Wire.h>

#include "SPI.h"

//#define SerialMon Serial
#include "TFT_eSPI.h"

String duvod ;
float maxcur;
float mincur= 10;
int interval;


 

bool spindle;
const int ledPin = 4; 
int podsvit = 50;
float reading, current, maxcurrent, mincurrent, voltage, voltage_raw;
float voltage_null=1.62;

bool paused;
#define TFT_GREY 0x7BEF
int tlac1, tlac2, tlac3;
TFT_eSPI myGLCD = TFT_eSPI();       // Invoke custom library

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
 

String stav = "zacatek";










void firstpage() {

 

  myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
  myGLCD.setTextSize(1);
  myGLCD.drawLine(0, 15, 240, 15,TFT_CYAN);
  myGLCD.drawString("Current limits :", 40, 22, 2);
  myGLCD.drawString("Imax : " + String(maxcurrent) + " A", 40, 42, 2);
  myGLCD.drawString("Imin : " + String(mincurrent) + " A", 40, 62, 2);
  myGLCD.drawString("Actual current:" , 40, 105, 2);
  myGLCD.setTextSize(2);
  myGLCD.drawLine(0, 90, 240, 90,TFT_CYAN);
  myGLCD.drawString(String(current, 2) + " A ", 40, 130, 2);
  myGLCD.drawLine(0, 180, 240, 180,TFT_CYAN);
  myGLCD.setTextColor(TFT_CYAN );

if (active){
    myGLCD.setTextSize(1);
    myGLCD.drawString("AKTIVNI" , 185, 5, 1);
    if (paused){
      //warning message
      myGLCD.setTextColor(TFT_CYAN,TFT_RED );
     if (spindle){
        myGLCD.drawString("Tool enebled", 40, 219, 2);
        }else{
        myGLCD.drawString("Tool disabled" , 40, 219, 2);   
        }
     // myGLCD.drawString(String(spindle) , 220, 190, 2);
      myGLCD.drawString("I max (so far) : " + String(maxcur) + " A" , 40, 185, 2);    
      myGLCD.drawString("I min (so far) : " + String(mincur) + " A" , 40, 202, 2);
      myGLCD.setTextSize(2);
     
      myGLCD.drawString(String(duvod), 5, 240, 2);
      
      } else {
        //myGLCD.fillRect(2, 220, 236,220,TFT_CYAN);
      myGLCD.setTextSize(2);
      myGLCD.setTextColor(TFT_CYAN, TFT_BLACK );
      myGLCD.drawString(String("So far so good!"), 20, 240, 2);
      myGLCD.setTextSize(1);
      if (spindle){
        myGLCD.drawString("Tool enable  ", 40, 219, 2);
        }else{
        myGLCD.drawString("Tool disable" , 40, 219, 2);   
        }
     // myGLCD.drawString(String(spindle) , 220, 190, 2);
      myGLCD.drawString("I max (so far) : " + String(maxcur) + " A" , 40, 185, 2);    
      myGLCD.drawString("I min (so far) : " + String(mincur) + " A" , 40, 202, 2);
      myGLCD.setTextSize(2); 
      }} else{
      myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
      myGLCD.drawString(String("Ready to start!"), 25, 240, 2);
      myGLCD.setTextSize(1);
      myGLCD.drawString("NEAKTIVNI" , 185, 5, 1); 
      if (spindle){
        myGLCD.drawString("Tool enable", 40, 219, 2);
        }else{
        myGLCD.drawString("Tool disable" , 40, 219, 2);   
        }
     // myGLCD.drawString(String(spindle) , 220, 190, 2);
      myGLCD.drawString("I max (so far) : " + String(maxcur) + " A" , 40, 185, 2);    
      myGLCD.drawString("I min (so far) : " + String(mincur) + " A" , 40, 202, 2);
      }
  
myGLCD.setTextColor(TFT_CYAN, TFT_BLACK );

  tlac3 = digitalRead(BUTTON_1);
  tlac2 = digitalRead(BUTTON_2);
  tlac1 = digitalRead(BUTTON_3);

if (!tlac3)
    {
    page = 2;
    myGLCD.fillScreen(0x000000);
    delay(200);
    
    }

if (!tlac2)
    {
   // digitalWrite(RELE_PIN_PAUZA, HIGH);
  // pausedd();
  if (active){
    active=false;
    myGLCD.fillScreen(0x000000);
    delay(200);
    maxcur=0;
    pausedd();
    digitalWrite(SPINDLE_RELE_PIN,HIGH);
    spindle = false; 
    
  } else {
  active = true;}
  myGLCD.fillScreen(0x000000);
  delay(200);
  digitalWrite(SPINDLE_RELE_PIN,LOW);
  spindle = true; 
  paused = false;  
   //pauzastart == millis();
     } 
     
if (!tlac1)
    {
    digitalWrite(SPINDLE_RELE_PIN, HIGH);
    spindle = false;
    
     }
  
}

void secondpage() {
 myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
  myGLCD.setTextSize(1);
  myGLCD.drawLine(0, 15, 240, 15,TFT_CYAN);
  myGLCD.drawString("Current limits :", 40, 22, 2);
  myGLCD.drawString("Imax : " + String(maxcurrent) + " A", 40, 42, 2);
  myGLCD.drawString("Imin : " + String(mincurrent) + " A", 40, 62, 2);
  
  myGLCD.setTextSize(2);
  
  myGLCD.drawLine(0, 90, 240, 90,TFT_CYAN);
  myGLCD.setTextSize(1);
  myGLCD.drawString("Actual current:" , 40, 105, 2);
  myGLCD.setTextSize(2);
  myGLCD.drawString(String(current, 2) + " A ", 40, 130, 2);
  myGLCD.drawLine(0, 180, 240, 180,TFT_CYAN);
    

myGLCD.setTextSize(2);
      myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
      myGLCD.drawString(String(maxcurrent), 42, 222, 2);
      myGLCD.setTextSize(1);
  
      myGLCD.drawString("Set max current " , 40, 190, 2);    
      myGLCD.setTextSize(2);
  tlac3 = digitalRead(BUTTON_1);
  tlac2 = digitalRead(BUTTON_2);
  tlac1 = digitalRead(BUTTON_3);
  if (!tlac3)
    {
    maxcurrent = maxcurrent + 0.1;
   
    }

if (!tlac2)
    {
  
  EEPROM.writeFloat(0, maxcurrent);
  EEPROM.commit();
  page = 3;
  myGLCD.fillScreen(0x000000);
  delay(200);
     } 
     
if (!tlac1)
    {
    maxcurrent = maxcurrent - 0.1;
    //maxcur =0 ;
     }
  

}
void thirdpage() {

  
   myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
  myGLCD.setTextSize(1);
    myGLCD.drawLine(0, 15, 240, 15,TFT_CYAN);
    myGLCD.drawString("Current limits :", 40, 22, 2);
  myGLCD.drawString("Imax : " + String(maxcurrent) + " A", 40, 42, 2);
  myGLCD.drawString("Imin : " + String(mincurrent) + " A", 40, 62, 2);
  
  myGLCD.setTextSize(2);
  
  myGLCD.drawLine(0, 90, 240, 90,TFT_CYAN);
  myGLCD.setTextSize(1);
  myGLCD.drawString("Actual current:" , 40, 105, 2);
  myGLCD.setTextSize(2);
  myGLCD.drawString(String(current, 2) + " A ", 40, 130, 2);
  myGLCD.drawLine(0, 180, 240, 180,TFT_CYAN);
    



      myGLCD.setTextColor(TFT_CYAN );
      myGLCD.setTextSize(2);
      myGLCD.setTextColor(TFT_CYAN,TFT_BLACK );
      myGLCD.drawString(String(mincurrent), 40, 222, 2);
      myGLCD.setTextSize(1);
  
      myGLCD.drawString("Set min current " , 40, 190, 2);    
      myGLCD.setTextSize(2);

  tlac3 = digitalRead(BUTTON_1);
  tlac2 = digitalRead(BUTTON_2);
  tlac1 = digitalRead(BUTTON_3);
  
if (!tlac3)
    {
    mincurrent = mincurrent + 0.1;

    }

if (!tlac2)
    {

  EEPROM.put(10, mincurrent);
  EEPROM.commit();
 
   page = 1;
   myGLCD.fillScreen(0x000000);
   delay(200);
   
     } 
     
if (!tlac1)
    {
    mincurrent = mincurrent - 0.1;
    //maxcur =0 ;
     }
  
  
}
void menu(){
  if (page==1){
  myGLCD.setTextSize(2);
   myGLCD.drawLine(0, 296, 240, 296,TFT_CYAN);
myGLCD.drawString(String("SET"), 10, 301, 1);
myGLCD.drawString(String("START"), 95, 301, 1);
myGLCD.drawString(String("KILL"), 195, 301, 1);
}
  if (page>=2){
  myGLCD.setTextSize(2);
   myGLCD.drawLine(0, 296, 240, 296,TFT_CYAN);
myGLCD.drawString(String("+"), 10, 301, 1);
myGLCD.drawString(String("SET"), 95, 301, 1);
myGLCD.drawString(String("-"), 195, 301, 1);
}


void buttonread() {


//  reading = analogRead(VIN);
  int average = 0;
  for (int i=0; i < 1000; i++) {
  average = average + analogRead(CUR_SENS_PIN);
 }
 
//  Serial.println(average);
average = average/1000;
voltage = (average*0.00080586)*1000;
//  Serial.println(voltage);
//  voltage = voltage;
current = (voltage-1610)/100;
if (maxcur < current){
   maxcur = current; 
 }

if (mincur > current){
  mincur = current;
}
  

if (spindle){ 
if (current > maxcurrent ){
  duvod = "Imax ALERT ! " + String(current) + " A";
  pausedd();
  } 

if (current < mincurrent ){
  duvod = "Imin ALERT ! " + String(current) + " A";
  pausedd();
  } 
}
  myGLCD.setTextSize(2);

  
}




void pausedd(){
   
   
   //myGLCD.drawString(String("zmacknuto"), 100, 280, 1);
  if (active){ 
  // if (!paused){
   
   digitalWrite(PAUZA_RELE_PIN, LOW);
   delay(100);
   digitalWrite(PAUZA_RELE_PIN, HIGH);
   paused = true;
   
   digitalWrite(SPINDLE_RELE_PIN,HIGH);
   spindle = false;
   myGLCD.fillRect(2, 185, 236,105,TFT_RED);
   myGLCD.setTextColor(TFT_CYAN,TFT_RED );
  
    myGLCD.setTextSize(2);    
    

  
delay(1000);
}}
  


  

void setup()
{
  Serial.begin(115200);
  int i = 10;
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(CUR_SENS_PIN, INPUT_PULLDOWN );
  pinMode(PAUZA_RELE_PIN, OUTPUT);
  digitalWrite(PAUZA_RELE_PIN,HIGH);
  pinMode(SPINDLE_RELE_PIN, OUTPUT);
  digitalWrite(SPINDLE_RELE_PIN,HIGH);
  spindle = false;
  ledcSetup(ledChannel, freq, resolution);
  EEPROM.begin(512);


  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, podsvit);
  myGLCD.init();
  myGLCD.setRotation(0);
  myGLCD.setTextSize(2);
  myGLCD.setTextColor(TFT_CYAN, TFT_BLACK );
  myGLCD.fillScreen(0x000000);
  paused = false;
  digitalWrite(PAUZA_RELE_PIN, HIGH);
 




  
  wifiManager.setConfigPortalBlocking(false);
   if (wifiManager.autoConnect("CNC")){;
    Serial.println("connected...yeey :)");
   }
  myGLCD.fillScreen(0x000000);

myGLCD.setTextColor(TFT_CYAN);
EEPROM.get(0, maxcurrent);
EEPROM.commit();
EEPROM.get(10 , mincurrent);
EEPROM.commit();
 
}



void loop()
{
 //myGLCD.fillScreen(0x000000); 
 
   menu();
 
  
if (page==1){
   firstpage();
}
if (page==2){
  secondpage();
}
if (page==3){
  thirdpage();
} 
buttonread();


}
