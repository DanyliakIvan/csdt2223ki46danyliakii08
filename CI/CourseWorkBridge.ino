
int ledPin = 13;

char speed = 1;
char heightOfJumping = 110;

bool pressedUp = false;
bool pressedDown = false;
bool pressedLeft = false;
bool pressedRight = false;

char jumping = 100;
bool jumpingIncrease = true;

char arrPlacementOfCactus[6] = {126, 1, 126, 1, 126, 1};
char appearanceOfCactus = 125;

String sending = "";

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  //digitalWrite(ledPin, HIGH);
  //delay(500);
  //digitalWrite(ledPin, LOW);
}

void loop() 
{
  sending = "";
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  
  //String sending = "";
  char test;
  
  while(Serial.available() == 0);
  
  byte k = 0;
  
  while(Serial.available())
  {
	  char temp = Serial.read();
	  if(k == 0)
	  {
	    test = temp;
		k++;
	  }
  }
  
  decodeData(test);
  controlingDrawDino();
  controlingDrawCactus();
  sending += codeData();

  Serial.println(sending);
}

void decodeData(char dataReceived)
{
	if(dataReceived)
	{
		if(dataReceived % 2)
			pressedRight = dataReceived % 2;
		dataReceived = dataReceived >> 1;
		
		
		if(dataReceived % 2)
			pressedLeft = dataReceived % 2;
		dataReceived = dataReceived >> 1;
		
		if(dataReceived % 2)
			pressedDown = dataReceived % 2;
		dataReceived = dataReceived >> 1;
		
		if(dataReceived % 2)
			pressedUp = dataReceived % 2;
	}
}

String codeData()
{
	String sending = "";
	sending += jumping; //TEST++;
	sending += arrPlacementOfCactus[0];
	sending += arrPlacementOfCactus[2];
	sending += arrPlacementOfCactus[4];
	
	char fifthByte = 0;
	
	fifthByte = 0;
	
	fifthByte += pressedUp;
	fifthByte = fifthByte << 1;
	
	fifthByte += pressedDown;
	fifthByte = fifthByte << 1;
	
	fifthByte += pressedLeft;
	fifthByte = fifthByte << 1;
	
	fifthByte += pressedRight;
	fifthByte = fifthByte << 1;
	
	
	fifthByte += arrPlacementOfCactus[1];
	fifthByte = fifthByte << 1;
	
	fifthByte += arrPlacementOfCactus[3];
	fifthByte = fifthByte << 1;
	
	fifthByte += arrPlacementOfCactus[5];
	
	fifthByte = fifthByte + 50;
	sending += fifthByte;
	
	return sending;
}

void controlingDrawDino()
{
    if(pressedUp && pressedDown)
    {
            jumping = 100;
            jumpingIncrease = true;
            pressedUp = false;
            pressedDown = false;
    }
    else if(pressedUp)
    {
        (jumpingIncrease) ? (jumping += speed / 3 + 1) : (jumping -= speed / 3 + 1);

        if(jumping >= heightOfJumping)
            jumpingIncrease = false;
        else if(jumping == 100)
        {
            jumpingIncrease = true;
            pressedUp = false;
        }
    }
    else if(pressedLeft)
    {
        speed--;
        pressedLeft = false;
    }
    else if(pressedRight)
    {
        speed++;
        pressedRight = false;
    }
}

void controlingDrawCactus()
{
    if(arrPlacementOfCactus[0] <= speed && arrPlacementOfCactus[2] != 126)
    {
        //"delete" first cactus and move: 1 <- 2;  2 <- 3
        arrPlacementOfCactus[0] = arrPlacementOfCactus[2];
        arrPlacementOfCactus[1] = arrPlacementOfCactus[3];

        arrPlacementOfCactus[2] = arrPlacementOfCactus[4];
        arrPlacementOfCactus[3] = arrPlacementOfCactus[5];
        arrPlacementOfCactus[4] = 126;
    }

    if(arrPlacementOfCactus[0] == 126)
    {
        arrPlacementOfCactus[0] = appearanceOfCactus;
        arrPlacementOfCactus[1] = random(2);
    }
    else if(arrPlacementOfCactus[2] == 126)
    {
        if(arrPlacementOfCactus[0] < appearanceOfCactus - 40 && random(10) == 0)
        {
            arrPlacementOfCactus[2] = appearanceOfCactus;
            arrPlacementOfCactus[3] = random(2);
        }
        else if(arrPlacementOfCactus[0] < appearanceOfCactus - 50)
        {
            arrPlacementOfCactus[2] = appearanceOfCactus;
            arrPlacementOfCactus[3] = random(2);
        }
    }
    else if(arrPlacementOfCactus[4] == 126)
    {
        if(arrPlacementOfCactus[2] < appearanceOfCactus - 40 && random(10) == 0)
        {
            arrPlacementOfCactus[4] = appearanceOfCactus;
            arrPlacementOfCactus[5] = random(2);
        }
        else if(arrPlacementOfCactus[2] < appearanceOfCactus - 50)
        {
            arrPlacementOfCactus[4] = appearanceOfCactus;
            arrPlacementOfCactus[5] = random(2);
        }
    }


    for(int i = 0; i < 6; i += 2)
    {
        if(arrPlacementOfCactus[i] != 126)
        {
            arrPlacementOfCactus[i] -= speed;
        }
    }
	
	
    if(arrPlacementOfCactus[0] >= 3 && arrPlacementOfCactus[0] <= 8)
    {
        if((arrPlacementOfCactus[1] == 1 && jumping <= 106) || (arrPlacementOfCactus[1] == 0 && jumping <= 104))
        {
            restartGame();
            //delay(2000);
        }
    }
}

void restartGame()
{
	speed = 1;
    arrPlacementOfCactus[0] = arrPlacementOfCactus[2] = arrPlacementOfCactus[4] = 126;
    arrPlacementOfCactus[1] = arrPlacementOfCactus[3] = arrPlacementOfCactus[5] = 1;
    jumping = 100;
    jumpingIncrease = true;
    pressedUp = false;
    pressedDown = false;
    pressedLeft = false;
    pressedRight = false;
}
