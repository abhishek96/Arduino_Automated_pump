#include<VirtualWire.h>
#include<SoftwareSerial.h>
SoftwareSerial BT(4,5);
int button=3,blueLED=11,greenLED=12,redLED=13,output=9,t1=0,t2=0,distance=0,thresh=20,i,j;
String message;
char state='a',a[25];
byte hb,lb;
void setup() 
{
  Serial.begin(9600);
  BT.begin(9600);
  
  vw_set_rx_pin(2);
  vw_setup(2000);   // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  pinMode(button,INPUT);
  pinMode(blueLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(output,OUTPUT);
  digitalWrite(blueLED,HIGH);
}

void loop() 
{ 
  t1=millis();
  pollbutton();
  getBTmessage();
  if(state=='a')
  {
    getmessage();
    checkheartbeat();
    setthreshold(thresh,distance);
  }
}
  
void pollbutton()
{
  if(digitalRead(button)==HIGH)
  {
    swap();
    delay(400);
  }
}
void swap()
{
  digitalWrite(blueLED,LOW);
  digitalWrite(greenLED,LOW);
  digitalWrite(redLED,LOW);
  if(state=='a')
  {
    state='O';
    digitalWrite(greenLED,HIGH);//glow green LED
    digitalWrite(output,HIGH);
  }
  else if(state=='O')
  {
    state='o';
    digitalWrite(redLED,HIGH);//glow red LED
    digitalWrite(output,LOW);
  }
  else if(state=='o')
  {
    state='a';
    digitalWrite(blueLED,HIGH);//glow blue LED
  }
}
void getmessage()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen))
      {
        digitalWrite(13, HIGH);
        hb=(byte)buf[0];
        lb=(byte)buf[1];
        distance=(hb<<8)|lb;
        Serial.println(distance);
        digitalWrite(13, LOW);
        t2=t1;
      }
}
void getBTmessage()
{
   if(BT.available())
   {
    message="<";
    while(BT.available())
    {
      message=message+((char)BT.read());
    }
    message[(message.length())-2]='>';
    i=message.indexOf('<');
    j=message.indexOf('>');
    Serial.print(message);
    processmessage();   
   }
}
void processmessage()
{
  if(message=="<dist>\n")
  {
    BT.println(distance);
  }
  else if(message=="<on>\n")
  {
    state='a';
    swap();
  }
  else if(message=="<off>\n")
  {
    state='O';
    swap();
  }
  else if(message=="<auto>\n")
  {
    state='o';
    swap();
  }
  else
  {
    BT.println("Invalid");
  }
}
void checkheartbeat()
{
  if((t1-t2)>20000)
  {
    distance=0;
    digitalWrite(output,LOW);
  }
}
void setthreshold(int t,int d)
{
  if(d>t)
    digitalWrite(output,HIGH);
  else
    digitalWrite(output,LOW);
}
