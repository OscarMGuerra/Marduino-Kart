#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
 
int s0 = 0;       //Module pins  wiring
int s1 = 1;
int s2 = 10;
int s3 = 13;
int out = 12;


// Define motor control pins
int IN1 = 11;
int IN2 = 9;
int IN3 = 7;
int IN4 = 8;
int ENA = 6;
int ENB = 5;
int ABS = 90;


int  Red=0, Blue=0, Green=0;  //RGB values
 
int r,g,w,y,b = 0; //count values


// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);


// Create the Player object
DFRobotDFPlayerMini player;


void setup()
{
// Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);


//pin modes
   pinMode(s0,OUTPUT);    
   pinMode(s1,OUTPUT);
   pinMode(s2,OUTPUT);
   pinMode(s3,OUTPUT);
   pinMode(out,INPUT);


   Serial.begin(9600);   //intialize the serial monitor  baud rate
   
   digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels  means the output frequency scalling is at 100% (recommended)
   digitalWrite(s1,HIGH);  //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
   
   // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);


  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
   Serial.println("OK");


    // Set volume to maximum (0 to 30).
    player.volume(30);
    player.play(4);
    delay(6000);
    player.play(6);
    delay(2000);
 
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}


void loop()
{


  GetColors();                                     //Execute the GetColors function  to get the value of each RGB color
 //Depending  of the RGB values given by the sensor we can define the color and displays it on the monitor
 Serial.println("R: " + String(Red) + "      G: " + String(Green) + "      B: " + String(Blue));


  if (Red <=15 && Green <=10 && Blue<40 && ((Green+Blue+Red)<30)){         //If the values  are low it's likely the white color (all the colors are present)
      Serial.println("White");
      w++;
      if(w == 2){
      ABS= ABS*2;
      tRight();
      delay(500);
      ABS = ABS/2;
      mForward();
      delay(500);
      resetCount();
      }
  }
  else if (Red<Blue && Red<Green && ((Green+Blue+Red)<200)){      //if  Red value is the lowest one and smaller thant 23 it's likely Red
      Serial.println("Red");
      r++;
      if(r == 2){
        player.play(5);
        mstop();
        delay(4000);
        resetCount();
      }
  }
  else if (Green<Red && Green<Blue && ((Green+Blue+Red)<200)){           //Green it was a little tricky,  you can do it using the same method as above (the lowest), but here I used a reflective  object
      Serial.println("Green");
      g++;
      if(g == 2){
        player.play(1);                   //which means the  blue value is very low too, so I decided to check the difference between green and  blue and see if it's acceptable
        jiggle();
        delay(500);
        resetCount();
      }
  }
   else if (Blue<Red && Blue<Green && (Green+Blue+Red)<150){
    Serial.println("Blue");
    y++;
    if(y == 2){
    player.play(2);
    mstop();
    star();
    resetCount();
    }
  }
  else{    
      Serial.println("Black");
      b++;
      if(b == 2){
      mForward();
      delay(500);
      ABS= ABS*2;
      tLeft();
      delay(500);
      ABS = ABS/2;
      mForward();
      delay(500);
      resetCount();
      }
  }
  delay(250);                                   //.100s delay
}




void GetColors()  
{    
  digitalWrite(s2,  LOW);                                           //S2/S3 levels define which set  of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH  is for green
  digitalWrite(s3, LOW);                                          
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       //here we wait  until "out" go LOW, we start measuring the duration and stops when "out" is  HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);  
  digitalWrite(s3, HIGH);                                         //Here  we select the other color (set of photodiodes) and measure the other colors value  using the same techinque
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW  : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  Green = pulseIn(out,  digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
}
void resetCount() {
  r,w,y,g,b = 0;
}
void mForward(){
  digitalWrite(IN1, HIGH);      
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);      
  digitalWrite(IN4, LOW);
}
void tLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void tRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void mstop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void jiggle() {
  mstop();
  for (int i = 1; i <= 5; i++) {
    tRight();
    delay(250);
    tLeft();
    delay(250);
  }
  mForward();
  delay(1000);
}
void spinOut() {
  mstop();
  ABS *= 2;
  tRight();
  delay(2500);
  ABS /= 2;
  mForward();
  delay(500);


}
void star() {
  ABS = ABS * 2;
  mForward();
  delay(1000);
  ABS = ABS / 2;
  delay(1000);
}

