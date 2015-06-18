#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

void setup()
{
  mySerial.begin(19200);
  Serial.begin(19200);
}
void loop()
{
delay(1200);
mySerial.print("AT");

delay(1200);
bool bOK = false;
while (mySerial.available() > 0)
{
char inChar = (char)mySerial.read();

bOK = true;
}

if(bOK)
{
int index = 0;
mySerial.println();
mySerial.println("AT+CMGF=1"); // sets the SMS mode to text
delay(100);
delay(1200);
bool bOK = false;
while (mySerial.available() > 0) {
//Serial.write(Serial.read());
char inChar = (char)mySerial.read();

//  if (mySerial.available())
//  Serial.write(mySerial.read());
  
bOK = true;
}
if(bOK)
{
mySerial.println();

  if (mySerial.available())
  Serial.write(mySerial.read());
  
mySerial.print("AT+CMGS=\""); // send the SMS number
mySerial.print("+358505346600");
mySerial.println("\"");
delay(1000);
mySerial.print("Latee "); // SMS body
delay(500);
mySerial.write(0x1A);

//mySerial.write(0x0D);
//mySerial.write(0x0A);
}
}
}
