//Time
#include <virtuabotixRTC.h>
//LCD Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//SMS
#include <SoftwareSerial.h>
#include <Wire.h>


//SMS
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "09687555526"; 

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Pumps
//Refiller Water Pump
byte pump1 = 7;

//Reducer Water Pump
byte pump2 = 8;


//Water Level Sensors
//Water Level Sensor for Fish Tank
int resval1 = 0;  
int respin1 = A0;

//Water Lever Sensor for Refiller Tank
int resval2 = 0;  
int respin2 = A1;

//Water Lever Sensor for Reducer Tank
int resval3 = 0;  
int respin3 = A2;



//Time
virtuabotixRTC myRTC(5, 4, 3);



void setup() { 
 Serial.begin(9600);

 //Refiller Water Pump 
 pinMode(pump1, OUTPUT); 

 //Reducer Water Pump 
 pinMode(pump2, OUTPUT); 

 //LCD Setup
 lcd.init();         
 lcd.backlight();

 //SMS
 _buffer.reserve(50);
 sim.begin(9600);

 // seconds, minutes, hours, day of the week, day of the month, month, year
 myRTC.setDS1302Time(50, 59, 6, 7, 18, 6, 2023);
} 



void loop() { 
  //Water Level data from Fish Tank to LCD 
  FT();
  
  //Water level Sensor for Refiller Tank
  WT1();

  //Water level Sensor for Replacer Tank
  WT2();

  //Time
  time();

 delay(2000); 
}



//Codes
void FT(){
  //Water Level data from Fish Tank to LCD 
  resval1 = analogRead(respin1);
  if (resval1<=470){ 
    lcd.clear();
    digitalWrite(pump1, HIGH); //Refill Water Pump
  //Stop Reducing Water Pump
    lcd.setCursor(0, 0);
    lcd.print("WaterLevel:");
    lcd.setCursor(0,1);
    lcd.print("Below Minimum");
    Serial.println("WaterLevel: Below Minimum");
    } 
  else if (resval1>=471 && resval1<=800){ 
    lcd.clear();  
    digitalWrite(pump1, LOW); //Refill Water Pump
    lcd.setCursor(0, 0);
    lcd.print("WaterLevel:");
    lcd.setCursor(0,1);
    lcd.print("Enough water");
    Serial.println("WaterLevel: Enough water");
  }
}


void WT2(){
  //Water level Sensor for Replacer Tank
  resval3 = analogRead(respin3);
  if (resval3<200){ 
    Serial.println("Water tank 2 is not full");
  }
  else if (resval3>400){ 
    Serial.println("Water tank 2 is full");
    //notifWT2(); //TEXT
  }
}



void notifWT1(){
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Water Tank 1 Need A Refill.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
}



void notifWT2(){
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Water Tank 2 Is Full.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
}



void time(){
  myRTC.updateTime();
  // Start printing elements as individuals
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  Serial.print(":");
  Serial.println(myRTC.dayofweek);
  
  // Delay so the program doesn't print non-stop
  delay(500);

  if (myRTC.dayofweek == 7){
    if (myRTC.hours == 7){
      if(myRTC.minutes== 0){
        if(myRTC.seconds== 0){
          //notifRW(); //TEXT
          digitalWrite(pump2, LOW);
          delay(5000);
          digitalWrite(pump2,HIGH);
        }
        else{
            digitalWrite(pump2,HIGH);
        }
      }
      else{
   digitalWrite(pump2,HIGH);
      }
    }
    else{
 digitalWrite(pump2,HIGH);
    }
  }
  else{
digitalWrite(pump2,HIGH);
  }
      
}



void notifRW(){
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Water in the Fishtank is being replaced.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
}
