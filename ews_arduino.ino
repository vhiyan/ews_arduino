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
#define jmlNodeid

#define pinSaklarPH 5
int statusPH = 0;
bool kirimsekalion =  false;
bool kirimsekalioff =  false;
 

static const int RXPin = 3, TXPin = 2; //yellow on pin 2, green on pin 3
static const uint32_t GPSBaud = 9600;
double longitude, latitude;

float lux = 0;

unsigned long currentTime[3], previousTime[3];

unsigned int state = 0;

int k = 0;
char pass[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 12}; //connect to the column pinouts of the keypad
bool huruf = false;
//initialize an instance of class NewKeypad
bool adapass = false;
char key;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String kirim = "";

String inputString = ""; // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

double gpslat,gpslong;

BH1750 lightMeter;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
Adafruit_SSD1306 display(OLED_RESET);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.begin(GPSBaud);
  while (!Serial) {
    ;
  }
  pinMode(pinSaklarPH,INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

}

void loop() {
  //main loop without delay
  lux =read_lux();
  statusPH = digitalRead(pinSaklarPH);
  if(statusPH == LOW){
    if(timer(2,300000))
    { 
    Serial.println("PHON");
    }
  }
  else
  {
 if(timer(2,120000))
    { 
    Serial.println("PHOFF");
    }
  }

  //loop for keypad
  char customKey = customKeypad.getKey();
  if (customKey != '\0') {
    if (customKey == '#' && state == 0) {
      kirim ="";
      kirim = (String)pass;
      kirim.trim();
      state = 2;
      //enter
    }
    else if (customKey == '*' && state == 1) {
      Serial.println("CHANGE");
      k = 0;
      for (int i = 0 ; i < 9; i++)
      {
        pass[i] = ' ';
      }
      state = 0;
    }

    else if (customKey == '*' && state == 0)
    {
      k = 0;
      for (int i = 0 ; i < 9; i++)
      {
        pass[i] = ' ';
      }
    }
    else
    {
      if (state == 0) {
        pass[k] = customKey;
        k++;
        if (k > 8)k = 8;
        while (customKeypad.getKey() != '\0');
      }
    }

  }


//loop state
  switch (state) {
    case 0:
      if (timer(0, 500)) {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.clearDisplay();
        display.setCursor(0, b1);
        display.println("INPUT DEVICE ID:");
        for (int i = 0; i < 9; i++)
        {
          display.setCursor(0 + (i * 8), b2);
          display.println(pass[i]);
        }
        display.display();
      }

      break;
    case 1:
      //
      if (timer(0, 2000)) {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.clearDisplay();
        display.setCursor(0, b2);
        display.print("lux meter : ");
        display.println(lux);
        for (int i = 0; i < 9; i++)
        {
          display.setCursor(0 + (i * 8), b1);
          display.println(pass[i]);
        }
        display.setCursor(0, b3);
        display.print("long:");
        display.println(gpslong,6);
        display.setCursor(0, b4);
        display.print("lat:");
        display.println(gpslat,6);
        display.display();
      }
      break;

    case 2:
    if (timer(0, 1000)) {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.clearDisplay();
        display.setCursor(0, b1);
        display.println("Menunggu");
        display.setCursor(0, b2);
        display.println("Pendaftaran");
        display.display();
    }
      while (ss.available() > 0)
        if (gps.encode(ss.read()))
        {
          if (gps.location.isValid())
          {
gpslat = get_lat();
gpslong = get_long();

          Serial.print(get_lat(), 6);
          Serial.print(" ");
          Serial.println(get_long(), 6);
          Serial.print("DAFTAR#");
          Serial.print(kirim);
          Serial.print("#");
          Serial.print(get_long(), 6);
          Serial.print("#");
          Serial.println(get_lat(), 6);
          state = 1;
          }
        }
      break;

    default:
      break;
  }

//loop read serial string
  if (stringComplete) {
    inputString.trim();
    if (inputString == "FETCH") {
      state = 1;
//kirimsekalion = false;
//kirimsekalioff = false;
      Serial.print("DATA#");
      Serial.println(lux);
    }
    if (inputString == "DAFTAR") {
      Serial.println("CHANGE");
      k = 0;
kirimsekalion= false;
kirimsekalioff= false;
      for (int i = 0 ; i < 9; i++)
      {
        pass[i] =' ';
      }
      state = 0;
    }
if(inputString == "CEK"){
Serial.print("cek#");
Serial.println(statusPH);
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
