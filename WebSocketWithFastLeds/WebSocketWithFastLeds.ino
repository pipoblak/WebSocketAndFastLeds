#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "FastLED.h"
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

#include <Thread.h>
#include <ThreadController.h>
#include <WebSocketsServer.h>

//Wifi Settings
const char* ssid     = "RocketzNode";
const char* password = "node1708";

//THREADS
ThreadController controll = ThreadController();
Thread threadRead = Thread();
Thread threadLight = Thread();

//SETTING STRIPS
#define PIN 1
#define NUM_LEDS 0
#define PIN2 2
#define NUM_LEDS2 368
#define PIN3 3
#define NUM_LEDS3 288

//CREATING STRIPS
CRGB strip1[NUM_LEDS];
CRGB strip2[NUM_LEDS2];
CRGB strip3[NUM_LEDS3];

//STRIP EVENT
int strip1Event;
int strip2Event;
int strip3Event;

//STRIP RGBS
int r1=0,g1=0,b1=255;
int r2=0,g2=0,b2=255;
int r3=0,g3=0,b3=255;

//STRIP COUNTERS
int strip1Count;
int strip2Count;
int strip3Count;
int strip1CountJ;
int strip2CountJ;
int strip3CountJ;

//VELOCITY
int Speed = 5;

//WEBSOCKET SERVER
WebSocketsServer webSocket = WebSocketsServer(81);



//------------------------------------- WEBSOCKET EVENT ----------------------------- x x x x x  ----------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;

    case WStype_ERROR:{
        break;
      }
    
    case WStype_TEXT: {
      int Red = 0, Green = 0, Blue = 0;
      int indexR, indexG, indexB, indexS, indexSize, indexD = 0;
      String command = "", commandBalance = "";
      int ID = 0;
        //DETECT EVENT
        if (payload[0] == '$') {
          webSocket.sendTXT(num, "Coloreyeze Device PC");

        }

        //SETTING RGB like #R255G255B255S0
        if (payload[0] == '#') {
          for (int iC = 1; iC <= lenght; iC++) {
            if (payload[iC] == 'R')
              indexR = iC;
            if (payload[iC] == 'G')
              indexG = iC;
            if (payload[iC] == 'B')
              indexB = iC;
            if (payload[iC] == 'S')
              indexS = iC;
            //  myDelay(1);
          }
          int idStrip = 0;
          command = (char*)payload;
          commandBalance = "";
          char char_array[lenght];
          command.toCharArray(char_array, lenght + 1);
          Serial.println(command);
          for (int cont = indexS + 1; cont <= lenght; cont++) {
            commandBalance.concat(char_array[cont]);
 
          }

          idStrip = commandBalance.toInt();
          commandBalance = "";

          for (int cont = indexR + 1; cont < indexG; cont++) {
            commandBalance.concat(char_array[cont]);
 
          }
          Red = commandBalance.toInt();
          commandBalance = "";
          for (int cont = indexG + 1; cont < indexB; cont++) {
            commandBalance.concat(char_array[cont]);

          }
          Green = commandBalance.toInt();
          commandBalance = "";
          for (int cont = indexB + 1; cont < indexS; cont++) {
            commandBalance.concat(char_array[cont]);

          }
          Blue = commandBalance.toInt();
          commandBalance = "";

          if (idStrip == 0 ) {
            r1 = Red;
            g1 = Green;
            b1 = Blue;
            r2 = Red;
            g2 = Green;
            b2 = Blue;
            r3 = Red;
            g3 = Green;
            b3= Blue;

          }
          if (idStrip == 1) {
            r1 = Red;
            g1 = Green;
            b1 = Blue;
          }
          else if (idStrip == 2) {
            r2 = Red;
            g2 = Green;
            b2 = Blue;
          }
          else if (idStrip == 3) {
            r3 = Red;
            g3 = Green;
            b3 = Blue;
          }
          
        }




        if (payload[0] == '@') {
          Serial.println("SELECT EVENT EVENT");
          for (int iC = 1; iC <= lenght; iC++) {
            if (payload[iC] == 'D')
              indexD = iC;
            if (payload[iC] == 'S')
              indexS = iC;
          }

          int idStrip = 0;
          command = (char*)payload;
          commandBalance = "";
          char char_array[lenght];
          command.toCharArray(char_array, lenght + 1);

          for (int cont = 1; cont < indexD; cont++) {
            commandBalance.concat(char_array[cont]);
          }
          ID = commandBalance.toInt();
          commandBalance = "";

          for (int cont = indexD + 1; cont < indexS; cont++) {
            commandBalance.concat(char_array[cont]);
          }
          Speed = commandBalance.toInt();
          commandBalance = "";

          for (int cont = indexS + 1; cont < lenght + 1; cont++) {
            commandBalance.concat(char_array[cont]);
          }
          int idStri = commandBalance.toInt();
          Serial.println(idStri);
          commandBalance = "";

          if (idStri == 1) {
            strip1Event = ID;
            strip1Count = 0;
            strip1CountJ = 0;
          }
          else if (idStri == 2) {
            strip2Event = ID;
            strip2Count = 0;
            strip2CountJ = 0;
          }
          else if (idStri == 3) {
            strip3Event = ID;
            strip3Count = 0;
            strip3CountJ = 0;
          }
          else if (idStri == 0) {
            strip1Event = ID;
            strip2Event = ID;
            strip3Event = ID;
            strip1Count = 0;
            strip2Count = 0;
            strip3Count = 0;
            strip1CountJ = 0;
            strip2CountJ = 0;
            strip3CountJ = 0;
          }

        }

        if (payload[0] == '!') {
        }


        

     
        
      }
      break;
       default:{ }
       break;
  }
}

//------------------------------------------------------------------ x x x x x  ------------------------------------------------------

void webSocketRun() {

  webSocket.loop();
}


void lightCall(){
   
  //STRIP 1 EVENT
  switch(strip1Event){
    case 0:{
      simpleRainbow(0,1);
      }
      
      break;
    case 1:{
      setAll(r1,g1,b1,1);
      }

      break;
    }

  //STRIP 2 EVENT
  switch(strip2Event){
    case 0:{
      simpleRainbow(0,2);
      }
      
      break;
    case 1:{
      setAll(r2,g2,b2,2);
      }

      break;
    }

  //STRIP 3 EVENT
  switch(strip3Event){
    case 0:{
      simpleRainbow(0,3);
      }
      
      break;
    case 1:{
      setAll(r3,g3,b3,3);
      }

      break;
    }
  
  
  }

//SETUP INFORMATIONS 
void setup(){
  Serial.begin(115200);
  
  //INITIALIZING WIFI
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);



  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.setDebugOutput(true);
  


  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  MDNS.addService("ws", "tcp", 81);
  
//INITIALIZE ALL STRIPS
 FastLED.addLeds<WS2812B, PIN, GRB>(strip1, NUM_LEDS).setCorrection( TypicalLEDStrip );
 FastLED.addLeds<WS2812B, PIN2, GRB>(strip2, NUM_LEDS2).setCorrection( TypicalLEDStrip );
 FastLED.addLeds<WS2812B, PIN3, GRB>(strip3, NUM_LEDS3).setCorrection( TypicalLEDStrip );

  //Setup StartEvents
  strip1Event=0;
  strip2Event=1;
  strip3Event=1;
  
  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  //ADDING THREADS
  threadRead.onRun(webSocketRun);
  threadRead.setInterval(1);

  threadLight.onRun(lightCall);
  threadLight.setInterval(1);

  controll.add(&threadRead);
  controll.add(&threadLight);
  
  }
  
void loop(){
 controll.run();
}
 
//SHOW SPECIFC STRIP
void showStrip() {
 FastLED.show();
   }

//SET A SPECIFC PIXEL INTO A SPECIFC STRIP
void setPixel(int Pixel, byte red, byte green, byte blue, int stripID) {
  
    if (stripID == 1) {
      strip1[Pixel].r=red;
      strip1[Pixel].g=green;
      strip1[Pixel].b=blue;
    }
    else if (stripID == 2) {
      strip2[Pixel].r=red;
      strip2[Pixel].g=green;
      strip2[Pixel].b=blue;
    }
    else if (stripID == 3) {
      strip3[Pixel].r=red;
      strip3[Pixel].g=green;
      strip3[Pixel].b=blue;
    }
}

//SET ALL PIXELS TO A SPECIFC STRIP
void setAll(byte red, byte green, byte blue, int stripID) {
      
  int numberLeds;
  if (stripID == 1) {
    numberLeds = NUM_LEDS;
  }
  else if (stripID == 2) {
    numberLeds = NUM_LEDS2;
  }
  else if (stripID == 3) {
    numberLeds = NUM_LEDS3;
  }
  for (int i = 0; i < numberLeds; i++ ) {
    setPixel(i, red, green, blue, stripID);
  }
  showStrip(); 
}

//LED EFFECTS ------------------------------------------------------------------------------------------------- x x x x x x x x x x x x ---------------------------------------------------------------------------------------------------------------

//WHEEL FOR RAINBOW
byte * Wheel(byte WheelPos) {
        
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  delay(0);
  return c;
}


void simpleRainbow(int SpeedDelay, int stripID) {
         
  byte *c;
  int contTempo;
  int numberLeds;
  int cont;

  if (stripID == 1) {
    contTempo = strip1Count;
    strip1Count++;
    numberLeds = NUM_LEDS;
    cont = strip1CountJ;
  }
  else if (stripID == 2) {
    contTempo = strip2Count;
    strip2Count++;
    numberLeds = NUM_LEDS2;
    cont = strip2CountJ;
  }
  else if (stripID == 3) {
    contTempo = strip3Count;
    strip3Count++;
    numberLeds = NUM_LEDS3;
    cont = strip3CountJ;
  }

  if (contTempo >= SpeedDelay) {
    for (int i = 0; i < numberLeds; i++) {
      c = Wheel(((i * 256 / numberLeds) + cont) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2), stripID);
    }

    contTempo = 0;
    if (stripID == 1) {
      strip1Count = contTempo;
      strip1CountJ++;
    }
    else if (stripID == 2) {
      strip2Count = contTempo;
      strip2CountJ++;
    }
    else if (stripID == 3) {
      strip3Count = contTempo;
      strip3CountJ++;
    }
  //Serial.printf("[%s] func: %s line: %d\n", __FILE__, __func__, __LINE__ ); 
    showStrip();
  }


}

//SINCRO RAINBOW
void sincroRainbow(int SpeedDelay) {
  byte *c;
  int contTempo;
  int numberLeds;
  int cont;
  
  numberLeds = NUM_LEDS + NUM_LEDS2 + NUM_LEDS3;
  contTempo = strip1Count;
  strip1Count++;
  cont = strip1CountJ;

  if (contTempo >= SpeedDelay) {

    for (int i = 0; i <= numberLeds; i++) {
      c = Wheel(((i * 256 / numberLeds) + cont) & 255);
      if (i <= NUM_LEDS) {
        setPixel(i, *c, *(c + 1), *(c + 2), 1);
      }
      else if (i > NUM_LEDS && i <= (NUM_LEDS + NUM_LEDS2)) {
        setPixel(i - NUM_LEDS, *c, *(c + 1), *(c + 2), 2);
      }
      else if (i > NUM_LEDS2 && i <= numberLeds) {
        setPixel(NUM_LEDS3 - (i - (NUM_LEDS + NUM_LEDS2)), *c, *(c + 1), *(c + 2), 3);
      }
      delay(0);
    }

    contTempo = 0;

    strip1Count = contTempo;
    strip1CountJ++;

    showStrip();
  }
}
