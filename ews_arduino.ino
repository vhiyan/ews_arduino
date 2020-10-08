#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BH1750.h> 

#include <Wire.h>
#include <Adafruit_GFX.h> //display
#include <Adafruit_SSD1306.h>

#include <Keypad.h>

#define OLED_RESET 4
#define b1 0
#define b2 8
#define b3 16
#define b4 24


static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
double longitude,latitude;

float lux =0;

unsigned long currentTime[2], previousTime[2];

unsigned int state =0;

int k=0;
char pass[7] = {'_','_','_','_','_','_','_'};
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
bool huruf = false;
//initialize an instance of class NewKeypad
bool adapass = false;
char key;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


String inputString = ""; // a String to hold incoming data
bool stringComplete = false; // whether the string is complete


BH1750 lightMeter;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
Adafruit_SSD1306 display(OLED_RESET);




void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
ss.begin(GPSBaud);  

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
/// lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

}

void loop() {
  // put your main code here, to run repeatedly:

  char customKey = customKeypad.getKey();
  if(customKey!='\0'){
     if(customKey=='#'&& k>5 && state ==0){
      state = 1;
      Serial.print("DAFTAR#");
      Serial.print((String)pass);
      Serial.print("#");
      Serial.print(get_long());
      Serial.print("#");
      Serial.println(get_lat());
      //enter
     }
     else if(customKey=='#' && state==1){
      Serial.println("CHANGE");
      k=0;
      for(int i =0 ;i<7;i++)
      {
      pass[i] ='_';
      }
       state=0;
     }
     
     else if(customKey=='*' && state == 0)
     {
      k=0;
      for(int i =0 ;i<7;i++)
      {
      pass[i] ='_';
      }
     }
     else
     {
      if(state==0){
      pass[k]=customKey;
      k++;
      if(k>6)k=6;
      while(customKeypad.getKey()!='\0');
      }}
      
    }



switch(state) {
case 0:
if (timer(0,500)){  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, b1);
  display.println("INPUT DEVICE ID:");
  for(int i=0;i<7;i++)
  {
  display.setCursor(0+(i*8), b2);
  display.println(pass[i]);
  }
  display.display();  
}

break;
case 1:
//lux =read_lux();
  if (timer(0,2000)){  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, b1);
  display.print("lux meter : ");
  display.println(lux);   
  for(int i=0;i<7;i++)
  {
  display.setCursor(0+(i*8), b2);
  display.println(pass[i]);
  }
   
  display.setCursor(0, b4);
  display.print("# utk ganti id ");
  
  
  display.display();
}

default:
break;
  }

if(stringComplete){
    inputString.trim();
    if (inputString == "FETCH") {
      Serial.print("DATA#");
      Serial.println(lux);
    }
    if (inputString == "DAFTAR"){
      Serial.println("CHANGE");
      k=0;
      for(int i =0 ;i<7;i++)
      {
      pass[i] ='_';
      }
       state=0;
    }
    stringComplete = false;
    inputString = "";
}

}

void serialEvent()
{
   while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar != '\n' || inChar != '\r') {
      inputString += inChar;
    }

    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
