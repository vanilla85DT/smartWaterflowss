#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Adafruit_SSD1306.h>
#define BLYNK_TEMPLATE_ID "TMPLfDKxqG3K"
#define BLYNK_DEVICE_NAME "waterflow"
#define BLYNK_AUTH_TOKEN "B-JrVOXwRGkAD4wyywuLpVCz_eDvnncG"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#include <BlynkSimpleEsp8266.h>
#define OLED_RESET 0  // GPIO0
#define APP_DEBUG
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include <Servo.h>
#include <ESP8266WiFi.h>
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
unsigned long peroid = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
int X;
int Y;
int Buzzer = D8;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
int Flowsensor = D5;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mai_2.4GHz";
char pass[] = "Nok07200";
 

Servo servo;

void setup()
{
  Serial.begin(115200);
  servo.attach(D3);//D8 pin
  pinMode (Buzzer, OUTPUT);
  pinMode(Flowsensor,INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  // Display static text
  display.display(); 
}

void loop()
{
  Blynk.run();
//X และ Y รับค่าทั้งหมด
X = pulseIn(Flowsensor, HIGH);
Y = pulseIn(Flowsensor, LOW);
// หาเวลา
TIME = X + Y;
// คำนวณความถี่
FREQUENCY = 1000000/TIME;
// คำนวณค่าน้ำตามสูตร
WATER = FREQUENCY/7.5;
// หารด้วย 60 ได้ อัตราเร็ว
LS = WATER/60;
 
// ถ้าน้ไม่ไหลจะแสดง ปริมาณลิตร
if(FREQUENCY >= 0)
{
if(isinf(FREQUENCY))
{

Serial.println(TIME);
Blynk.virtualWrite(V1,TOTAL,  "L" );
Blynk.virtualWrite(V2,TIME/100);
}
 // ถ้าน้ำไหลจะแสดงอัตราเร็ว
else
{
TOTAL = TOTAL + LS;
Blynk.virtualWrite(V2,TIME/100);
Serial.println(FREQUENCY);
 
 Serial.println(WATER);
 Serial.println(TOTAL);
 Blynk.virtualWrite(V4,TOTAL,  "L/H" );
 
}
delay(1000);
  }
}

BLYNK_WRITE(V0)

{
servo.write(param.asInt());
}
BLYNK_WRITE(V3)
{
servo.write(30);

peroid = millis()-last_time;
Serial.println(peroid);
Blynk.virtualWrite(V6 ,peroid/1000);
display.setFont(&FreeSans9pt7b);
display.setFont();
display.setCursor(33, 15);
display.clearDisplay();
display.display(); 
digitalWrite (Buzzer, HIGH); //turn buzzer on
delay(3000);
digitalWrite (Buzzer, LOW);  //turn buzzer off
delay(1000);
}
BLYNK_WRITE(V4)

{

last_time = millis();

servo.write(180);
display.setFont(&FreeSans9pt7b);
display.setFont();
display.setCursor(33, 15);
display.println("timed!");
display.display(); 
digitalWrite (Buzzer, HIGH); //turn buzzer on
delay(3000);
digitalWrite (Buzzer, LOW);  //turn buzzer off
delay(1000);
}
