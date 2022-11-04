#include <Wire.h>

String message;
byte whatToDo;
byte amountCharacters = 0;
bool allowToReceiveFromUser = false;

void setup()
{
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onRequest(requestEvent);  
  Wire.onReceive(receiveEvent);  
}

void loop() 
{
  if(whatToDo == 1)
  {
    message = "";

    if(amountCharacters == 0)
    {
      while(Serial.available() == 0);
      delay(50);

      amountCharacters = Serial.available();
      if(amountCharacters > 30) amountCharacters = 30;
    }
  }
  else if(whatToDo == 2)
  {
    int k = amountCharacters;

    while(Serial.available() > 0 && allowToReceiveFromUser == true)
    {
      char c = Serial.read();
      
      if(k > 0)
      {
        message += c;
        k--;
      }
    }

    allowToReceiveFromUser = false;    
  }
}

void requestEvent() 
{
	if(whatToDo == 1)
  {
    Wire.write(amountCharacters);
  }
  else if(whatToDo == 2)
  {
    Wire.write(message.c_str());
  }
}

void receiveEvent(int howMany)
{
  whatToDo = Wire.read();
  
  if(whatToDo == 2)
    allowToReceiveFromUser = 1;

  if(whatToDo == 3)
  {
    String receiveMessage = "";
    byte j = 0;
    byte size;

    while (Wire.available()) 
    {
      if(j < amountCharacters)
      {
        char b = Wire.read();
        receiveMessage += b;
        j++;
      }
      else
        size = Wire.read();
    } 

    Serial.println(receiveMessage + size);
    amountCharacters = 0;
  }
}
