
#define BUTTON_2    37
#define BUTTON_1    38
#define BUTTON_3    39
#define BUTTON_4    0
#define CUR_SENS_PIN  35
#define PAUZA_RELE_PIN    26
#define SPINDLE_RELE_PIN    19
#include <EEPROM.h>
#include <Arduino.h>
#include <ESP_Mail_Client.h>

#include <WiFiManager.h>
//#include <WiFi.h>

bool active=false;
int page=1;
#include <Wire.h>

#include "SPI.h"

//#define SerialMon Serial
#include "TFT_eSPI.h"

String duvod ;
float maxcur;
float mincur= 10;
bool wifi;
int interval;


  WiFiManager wifiManager;

#define WIFI_SSID1 "Petr Odstrcil - iPhone"
#define WIFI_PASSWORD1 "po155033"
#define WIFI_SSID2 "FOOD Media"
#define WIFI_PASSWORD2 "petajekabrnak"
#define WIFI_SSID3 "Petr Odstrcil - iPhone"
#define WIFI_PASSWORD3 "po155033"
#define SMTP_HOST "smtp.foodmedia.cz"
#define SMTP_PORT 587

/* The sign in credentials */
#define AUTHOR_EMAIL "cnc@foodmedia.cz"
#define AUTHOR_PASSWORD "155033"

/* Recipient's email*/
#define RECIPIENT_EMAIL "petr.odstrcil@me.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

bool spindle;
const int ledPin = 4; 
int podsvit = 50;
float reading, current, maxcurrent, mincurrent, voltage, voltage_raw;
float voltage_null=1.62;
long pauzadur,pauzastart,pauzacur;
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
    //maxcur =0 ;
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
    //delay(1000);
    //spindle = true;
    }

if (!tlac2)
    {
   // digitalWrite(RELE_PIN_PAUZA, HIGH);
  // pausedd();
 // active = true;
  EEPROM.writeFloat(0, maxcurrent);
  EEPROM.commit();
 // EEPROM.writeFloat(0, mincurrent);
   //   delay(200);
   //pauzastart == millis();
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
    //delay(1000);
    //spindle = true;
    }

if (!tlac2)
    {
   // digitalWrite(RELE_PIN_PAUZA, HIGH);
  // pausedd();
 // active = true;
  EEPROM.put(10, mincurrent);
  EEPROM.commit();
 // EEPROM.writeFloat(0, mincurrent);
   //   delay(200);
   //pauzastart == millis();
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
  if (page==2){
  myGLCD.setTextSize(2);
   myGLCD.drawLine(0, 296, 240, 296,TFT_CYAN);
myGLCD.drawString(String("+"), 10, 301, 1);
myGLCD.drawString(String("SET"), 95, 301, 1);
myGLCD.drawString(String("-"), 195, 301, 1);
}
if (page==3){
  myGLCD.setTextSize(2);
   myGLCD.drawLine(0, 296, 240, 296,TFT_CYAN);
myGLCD.drawString(String("+"), 10, 301, 1);
myGLCD.drawString(String("SET"), 95, 301, 1);
myGLCD.drawString(String("-"), 195, 301, 1);
}
}

void buttonread() {


//  reading = analogRead(VIN);
  int average = 0;
  for (int i=0; i < 1000; i++) {
  average = average + analogRead(CUR_SENS_PIN);
// Serial.println(i);
// Serial.println("Error sending Email, " + smtp.errorReason());
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
  
//  if(abs(voltage) > cutOff ){
//    Serial.print("V: ");
//    Serial.print(voltage,3);// print voltage with 3 decimal places
//    Serial.print("V, I: ");
//    Serial.print(current,2); // print the current with 2 decimal places
//    Serial.println("A");
//  }else{
//    Serial.println("No Current");
//  }

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


void rele() {

}


void pausedd() {
   
   
   //myGLCD.drawString(String("zmacknuto"), 100, 280, 1);
  if (active){ 
  // if (!paused){
   
   digitalWrite(PAUZA_RELE_PIN, LOW);
   delay(100);
   digitalWrite(PAUZA_RELE_PIN, HIGH);
   paused = true;
   pauzastart = millis();
   digitalWrite(SPINDLE_RELE_PIN,HIGH);
   spindle = false;
   myGLCD.fillRect(2, 185, 236,105,TFT_RED);
   myGLCD.setTextColor(TFT_CYAN,TFT_RED );
  
    myGLCD.setTextSize(2);    
    

  
   smtp.debug(0);

  /* Set the callback function to get the sending results */
  

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "CNC";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "!!KONFLIKT!!";
  message.addRecipient("Peta", RECIPIENT_EMAIL);


  String textMsg = "Je to v prdeli Peto ! " + duvod +"Imax=" + String(maxcur) + "A I=" + String(current) + "A Vin= " + String(voltage)+ "mV vreteno:" + String(spindle);
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  //message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  MailClient.sendMail(&smtp, &message);
  //  Serial.println("Error sending Email, " + smtp.errorReason());

   
   delay(200);
  // } 
//   else {
//    myGLCD.fillRect(2, 190, 236,100,TFT_BLACK);
//    paused = false;
//    digitalWrite(SPINDLE_RELE_PIN, LOW);
//    spindle = true;
//          }
delay(1000);
}}
  
void wific(){
    myGLCD.setTextSize(1);
    myGLCD.setTextColor(TFT_CYAN, TFT_BLACK );
    if (WiFi.status() == WL_CONNECTED)
        {
        myGLCD.drawString(String(" ON-LINE  ") , 1, 5, 1);
        wifi = true;
         }else {
        myGLCD.drawString(String(" OFF-LINE") , 1, 5, 1);
        wifi = false;
         }

    //debuging shows leng of cicle in millis 
   // myGLCD.drawString(String(millis()-interval) + " ms" , 185, 15, 1);
    interval=millis();
    myGLCD.setTextSize(2);
    myGLCD.setTextColor(TFT_CYAN );
}


  

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
  EEPROM.begin(1000);


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
 

//  WiFi.begin(WIFI_SSID1, WIFI_PASSWORD1);
//  while (i>0 && WiFi.status() != WL_CONNECTED) {
//     myGLCD.drawString(String("cekam na wifi 1") , 10, 190, 2);
//    i= i-1;
//    delay(200);
//  }
//  if (WiFi.status() != WL_CONNECTED){
//  WiFi.begin(WIFI_SSID2, WIFI_PASSWORD2);
//  i = 20;
//  while (i>0 && WiFi.status() != WL_CONNECTED) {
//    myGLCD.drawString(String("cekam na wifi 2") , 10, 190, 2);
//    i= i-1;
//   delay(200);
//  }}
//  if (WiFi.status() != WL_CONNECTED){
//   WiFi.begin(WIFI_SSID3, WIFI_PASSWORD3);
//   i = 20;
//  while (i>0 && WiFi.status() != WL_CONNECTED) {
//     myGLCD.drawString(String("cekam na wifi 3") , 10, 190, 2);
//    i= i-1;
//    delay(200);
//  }}



  
  wifiManager.setConfigPortalBlocking(false);
   if (wifiManager.autoConnect("CNC")){;
    Serial.println("connected...yeey :)");
   }
  myGLCD.fillScreen(0x000000);
if (WiFi.status() == WL_CONNECTED){
  myGLCD.drawString(String("Spojeni navazano.") , 10, 190, 2);
  wifi = true;
  delay(1000);
  
  }else {
     myGLCD.drawString(String("Wifi nenalezena ") , 10, 190, 2);
  wifi = false;
  delay(1000);
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
   wific();


 wifiManager.process();

 // delay(20);
// myGLCD.fillScreen(0x000000);
  

}
