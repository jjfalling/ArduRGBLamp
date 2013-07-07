/* 
Seriously modified version of:
IKEA Dudero mods Ethernet Version
 by Boris Landoni
http://www.open-electronics.org/arduino-wifi-rgb-lamp-ikea-dudero-modding/
 
Using a web interface with an ajax color picker to send the desired color codes to the arduino
Web interface is based on farbtastic. Also see process.pl for a few lines you might need to modifiy. 

 */

#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

int red = 3;    // RED LED connected to PWM pin 3
int green = 5;    // GREEN LED connected to PWM pin 5
int blue = 6;    // BLUE LED connected to PWM pin 6
int buttonPin = 4; // digital pin going to human touch sensor. this is used to turn the lamp on/off w/o a computer

int r=50; int g=100; int b=150;
int rup; int gup; int bup;
int fader=0;
int inc=10;
String inString = String(50);
char buffer[160]; // make sure this is large enough for the largest string it must hold

byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x1D, 0x89 };
byte ip[] = { 192,168,15,17 };
byte gateway[] = { 192, 168, 15, 1 };
byte subnet[] = { 255, 255, 255, 0 };

int buttonState = 0;
int onOffState = 0;



prog_char string_0[] PROGMEM  = "done";


PROGMEM const char *string_table[] = // change "string_table" name to suit
{
string_0
};

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup()
{
  // start the Ethernet connection and the server:
      Serial.begin(9600);
      Ethernet.begin(mac, ip,gateway,subnet);
      server.begin();
      Serial.println("Booting...\nSerial READY");
      //r = EEPROM.read(1);
      //g = EEPROM.read(2);
      //b = EEPROM.read(3);
      //inc = EEPROM.read(4);
      //fader = EEPROM.read(5);
      
      pinMode(buttonPin, INPUT); 
      
      
  Serial.println();
  Serial.print("Device ready, http to ");
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.print(ip[3]);
  Serial.println(" to use this device.");
} 

void loop()  {
//  // fade in from min to max in increments of 5 points:
//  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) {
//    // sets the value (range from 0 to 255):
//    //analogWrite(red, fadeValue);
//    // wait for 30 milliseconds to see the dimming effect
//    rgb(fadeValue,fadeValue,fadeValue);
//    delay(30);
//  }
//
//  // fade out from max to min in increments of 5 points:
//  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) {
//    // sets the value (range from 0 to 255):
//    //analogWrite(red, fadeValue);
//    // wait for 30 milliseconds to see the dimming effect
//    rgb(fadeValue,fadeValue,fadeValue);
//    delay(30);
//  } 

  if (fader==1){
    funcfader();
  }

      EthernetClient client = server.available();

      if (client) {
        Serial.println("client");
        // an http request ends with a blank line
        boolean current_line_is_blank = true;
        while (client.connected()) {

          if (client.available()) {

            char c = client.read();
            if (inString.length() < 50) {
            inString.concat(c);

            } 

            if (c == '\n' && current_line_is_blank) {
                char colorArr[5];
                String temp="";
                Serial.print("inString: ");
                Serial.println(inString);

                int Pos_r = inString.indexOf("r");
                int Pos_g = inString.indexOf("g");
                int Pos_b = inString.indexOf("b");
                int Pos_i = inString.indexOf("inc");
                int Pos_f = inString.indexOf("fad");
                int End = inString.indexOf("HTTP");
                Serial.print("Pos_r: ");
                Serial.println(Pos_r);
                Serial.print("Pos_g: ");
                Serial.println(Pos_g);
                Serial.print("Pos_b: ");
                Serial.println(Pos_b);
                Serial.print("Pos_i: ");
                Serial.println(Pos_i);
                Serial.print("Pos_f: ");
                Serial.println(Pos_f);
                Serial.print("End: ");
                Serial.println(End);

                if(Pos_r>=0){
                   temp=inString.substring((Pos_r+2), (Pos_g-1));
                   temp.toCharArray(colorArr, 5);
                   r=(atoi(colorArr));
                   Serial.print("red: ");
                   Serial.println(r);
                   EEPROM.write(1, r);
                }
                if(Pos_g>=0){
                   temp=inString.substring((Pos_g+2), (Pos_b-1));
                   temp.toCharArray(colorArr, 5);
                   g=(atoi(colorArr));
                   Serial.print("green: ");
                   Serial.println(g);
                   EEPROM.write(2, g);
                }         

                if(Pos_b>=0){
                   temp=inString.substring((Pos_b+2), (Pos_i-1));
                   temp.toCharArray(colorArr, 5);
                   b=(atoi(colorArr));
                   Serial.print("blue: ");
                   Serial.println(b);
                   EEPROM.write(3, b);
                }      

                if(Pos_i>=0){
                   temp=inString.substring((Pos_i+4), (Pos_f-1));
                   temp.toCharArray(colorArr, 5);
                   inc=(atoi(colorArr));
                   Serial.print("inc: ");
                   Serial.println(inc);
                   EEPROM.write(4, inc);
                } 

                if(Pos_f>=0){
                   temp=inString.substring((Pos_f+4), (End-1));
                   temp.toCharArray(colorArr, 5);
                   fader=(atoi(colorArr));
                   Serial.print("fader: ");
                   Serial.println(fader);
                   EEPROM.write(5, fader);
                }     

               if ((Pos_r>=0)&&(Pos_g>=0)&&(Pos_b>=0)) {
                 rgb(r,g,b);
               }
            printWebPage( &client);
            break;
            }
            if (c == '\n') {
            // we're starting a new line
            current_line_is_blank = true;
            } else if (c != '\r') {
            // we've gotten a character on the current line
            current_line_is_blank = false;
            }
          }
        }
        // give the web browser time to receive the data
        delay(1);

        inString = "";
        client.stop();
      }
      
     //check if the touch thing is used, if so:
     buttonState = digitalRead(buttonPin);
     if (buttonState == HIGH) {    
          Serial.println("btn press");
          
      //check if current status if on or off, update status var
      if (r<255) onOffState=1;
      else if (g<255) onOffState=1;
      else if (b<255) onOffState=1;
      
      if (r==255) onOffState=0;
      else if (g==255) onOffState=0;
      else if (b==255) onOffState=0;

      //check the current state, make it the opposite
      if (onOffState==0) {
        onOffState=1;
        r=0;
        b=0;
        g=0;
      }
      else if (onOffState==1) {
        onOffState=0;
        r=255;
        b=255;
        g=255;
      }
      
        analogWrite(red, r);
        analogWrite(green, g);
        analogWrite(blue, b);
  
        delay (500); //keeps the sensor from freaking out....
     }

}

void printWebPage(Client *client)
{
      int tmp=0;
      Serial.println("printWebPage");
      // send a standard http response header
      client->println("HTTP/1.1 200 OK");
      client->println("Content-Type: text/html");
      client->println();
      client->println("done");

}

void funcfader(){
    Serial.println("fader");
    if (rup==1){r+=1;}
    else{r-=1;}
    if (r>=255){rup=0;}
    if (r<=0){rup=1;}

    if (gup==1){g+=1;}
    else{g-=1;}
    if (g>=255){gup=0;}
    if (g<=0){gup=1;}

    if (bup==1){b+=1;}
    else{b-=1;}
    if (b>=255){bup=0;}
    if (b<=0){bup=1;}

    delay(inc*2);
    rgb(r, g, b);
}

void rgb(int r, int g, int b)
{

  Serial.print("RGB: ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print(b);
  if (r>255) r=255;
  if (g>255) g=255;
  if (b>255) b=255;
  if (r<0) r=0;
  if (g<0) g=0;
  if (b<0) b=0;

  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}