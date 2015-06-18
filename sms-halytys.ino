

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM800L_Serial(2, 3);
Adafruit_SSD1306 display(OLED_RESET);

#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define DHT21_PIN 5

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 


dht DHT;  // HDT kosteus ja lämpötila-anturi
int State;  //Tila - ajatus oli 0 default, 1 anturi-häly, 2 anturi-häly jne. Tilat sen takia että saan pysäytettyä loopin hälyytyksern sattuessa johonkin kohtaan aina resettiin saakka

void setup()   {                
  
  Serial.begin(9600); //näyttö i2c
  SIM800L_Serial.begin(19200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D 
  pinMode(7, OUTPUT);
  State == 0;
}

void loop() 
  {
  int chk = DHT.read21(DHT21_PIN); 
  display.display();
  delay(200);
  display.clearDisplay();
  
  switch (State) {
    case 0:  //Reading values
          

      switch (chk)       
     
      OLED_begin();
              
          if (50 < DHT.humidity)
        {
            digitalWrite(7, HIGH);
            //OLED_virhe();
            State = 1;
            
        }
            else
        {
        digitalWrite(7, LOW);
        OLED_begin();
        State = 0;
        }
        
      delay(500);
      break;
      
      
    case 1: //Kosteushälytys
      switch (chk) 
      OLED_virhe();
      
        if (50 < DHT.humidity)
        {
            digitalWrite(7, HIGH);
            OLED_virhe();
            State = 1;
            
        }
            else
        {
        digitalWrite(7, LOW);
        OLED_begin();
        State = 0;
        }
        
      //0 == State;
      break;
      }
    
     
  }

void OLED_begin() // Oled perusnäyttö
{
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Infoo");
      display.print(DHT.humidity, 1);  
      display.println("%"); 
      
     // display.setTextColor(BLACK, WHITE); // 'inverted' text
      
      display.print(DHT.temperature, 1);
      display.println("\tC");
      display.println("0505346600");
      display.display();
      delay(500);  
}


void OLED_virhe() // Oled virhenäyttö - nyt vaan testinä kosteus
  {                 
    display.clearDisplay();              
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("virhe!"); 
        display.setTextSize(3);
    display.print(DHT.humidity, 1);
    display.println("%"); 
    display.display();
    //sms();  // sms-lähetys toimii mutta lähettää viestejä niin pitkään kun ollaan rajan yläpuolella
        delay(5000);
    
  }

void sms()  // tekstiviestin lähetys - vielä kovakoodattu puhelinnumero
{
delay(1200);
SIM800L_Serial.print("AT");

delay(1200);
bool bOK = false;
while (SIM800L_Serial.available() > 0)
{
char inChar = (char)SIM800L_Serial.read();

bOK = true;
}

if(bOK)
{
int index = 0;
SIM800L_Serial.println();
SIM800L_Serial.println("AT+CMGF=1"); // sets the SMS mode to text
delay(100);
delay(1200);
bool bOK = false;
while (SIM800L_Serial.available() > 0) {
char inChar = (char)SIM800L_Serial.read();
bOK = true;
}
if(bOK)
{
SIM800L_Serial.println();
SIM800L_Serial.print("AT+CMGS=\""); // send the SMS number
SIM800L_Serial.print("+358505346600");
SIM800L_Serial.println("\"");
delay(1000); 
SIM800L_Serial.print("Varoitus: "); // SMS 
SIM800L_Serial.print(DHT.humidity, 1); // SMS
SIM800L_Serial.print("%"); // SMS
delay(500);
SIM800L_Serial.write(0x1A); //ctrl+z send sms
}
}
}



