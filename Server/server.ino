#include <Wire.h>
#include <avr/wdt.h>

int ledPin = 13;
byte switcher;
byte amountCharacters, SLAVE_ID = 8; 

void setup() 
{
  wdt_disable;

  Wire.begin();
  pinMode(ledPin, OUTPUT);  
  wdt_enable (WDTO_4S);
  
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

void loop() 
{
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  wdt_reset();

  switcher = 1;
  transmitSwitcher();
  
  readLenghtMessage();
  
  if(amountCharacters)
  {
    switcher = 2;
    transmitSwitcher();
    delay(50);
    
    String massage = readMessage();

    switcher = 3;
    byte response[massage.length()+2];
    response[0] = switcher;

    for (byte i = 0; i < massage.length(); i++) 
    {
      response[i + 1] = (byte)massage.charAt(i);
    }

    response[massage.length()+1] = massage.length();
    
    Wire.beginTransmission(SLAVE_ID);
    Wire.write(response, massage.length()+2);
    Wire.endTransmission();
  }
  
  delay(100);
}

void transmitSwitcher()
{
  Wire.beginTransmission(SLAVE_ID); 
  Wire.write(switcher); 
  Wire.endTransmission();     
}
	
void readLenghtMessage()
{
  Wire.requestFrom(SLAVE_ID, (uint8_t)1);
  while (Wire.available() == 0);
  
  amountCharacters = Wire.read(); 
}

String readMessage()
{
  String message = "";

  Wire.requestFrom(SLAVE_ID, amountCharacters);
  
  while (Wire.available()) 
  {
    char symbol = Wire.read();
    message += symbol;
  } 

  return message;
}