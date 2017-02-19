#include "LowPower.h"
#include "VirtualWire.h"
int trig=4,echo=3,i=0,tx=12,ledPin=13;
long duration,distance;
byte hb,lb;
void setup() {
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(9,OUTPUT);
  Serial.begin(9600);
  vw_set_tx_pin(tx);
  vw_setup(2000);
  digitalWrite(ledPin,LOW);
}

void loop() {
  arduino_sleep();
  digitalWrite(9,HIGH);
  getdistance();
  sendmessage();
  digitalWrite(9,LOW);
}
void arduino_sleep()
{ 
  //for(i=0;i<15;i++)
  LowPower.powerDown(SLEEP_8S,ADC_OFF,BOD_ON);
}
void getdistance()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  distance=(duration/2)/29.1;
  Serial.println(distance);}
void sendmessage()
{
  char msgtosend[2];
  hb=distance>>8;
  lb=distance&0xFF;
  msgtosend[0]=hb;
  msgtosend[1]=lb;
  digitalWrite(ledPin,HIGH);
  vw_send((uint8_t *)msgtosend,2);
  vw_wait_tx();
  digitalWrite(ledPin,HIGH);
}

