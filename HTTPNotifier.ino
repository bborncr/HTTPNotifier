/*HTTPNotifier version 1.1
* 
 */

#include <SPI.h>        
#include <Ethernet.h>

#define LED_PIN_RED 2
#define LED_PIN_GREEN 5
#define LED_PIN_BLUE 3
#define BUZZER_PIN 6

int ledState = 0;
int red_value = 0;
int green_value = 0;
int blue_value = 0;
long previousMillis = 0;
long previousMillis_BUZ = 0;
boolean buzzerOn = false;
long interval = 500; // sets blink speed
long buzzer_interval = 2000; // buzzer time
int currentColor = 5;
int currentState = 0;
String commandString = "null";

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x87 };//make sure this doens't conflict with anything in your network
byte ip[] = { 
  192, 168, 1, 1 };

EthernetServer server(80);

void setup()
{
  pinMode(LED_PIN_RED,OUTPUT);
  pinMode(LED_PIN_GREEN,OUTPUT);
  pinMode(LED_PIN_BLUE,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  setLED(0,1); //red
  delay(1000);
  Serial.begin(9600);
  Serial.println("Serial connection ready...");
  setLED(2,1); //blue
  if(Ethernet.begin(mac) == 0) { //change to Ethernet.begin(mac,ip) for static ip
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
  server.begin();
  delay(1000);
  Serial.print("IP Address: ");
  Serial.print((Ethernet.localIP()));
  Serial.println("...ready");
  setLED(0,1);
  delay(1000);
  setLED(1,1); //green
  delay(1000);
  setLED(2,1); //blue
  delay(1000);
  setLED(5,0);

}

void loop()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        if( client.find("GET /") ) {  // search for 'GET'
          // find tokens starting with "cmd" and stop on the first blank line
          while(client.findUntil("cmd", "\n\r")){  // search to the end of line for 'cmd'
            int cmd = client.parseInt(); // receive command number

            switch (cmd) {
            case 0 : 
              currentColor=0; 
              currentState=0; 
              commandString = ("Red On"); 
              break;
            case 1 : 
              currentColor=1; 
              currentState=0; 
              commandString = ("Green On"); 
              break;
            case 2 : 
              currentColor=2; 
              currentState=0; 
              commandString = ("Blue On"); 
              break;
            case 3 : 
              currentColor=3; 
              currentState=0; 
              commandString = ("Yellow On"); 
              break;
            case 4 : 
              currentColor=4; 
              currentState=0; 
              commandString = ("White On"); 
              break;
            case 5 : 
              currentColor=5; 
              currentState=0; 
              commandString = ("LEDs Off"); 
              break;
              // case 6 : tone(BUZZER_PIN, 2000, 3000); commandString = ("Buzzer On"); digitalWrite(BUZZER_PIN,HIGH); break;
            case 6 : 
              buzzerOn = true; 
              commandString = ("Buzzer On"); 
              break;
            case 10 : 
              commandString = ("Gridshield HTTP Notifier v1.1"); 
              break;
            default: 
              commandString = ("Unknown"); 
              Serial.println("Unknown command"); 
              break;
            }
          }
        }

        // the findUntil has detected the blank line (a lf followed by cr)
        // so the http request has ended and we can send a reply
        // send a standard http response header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();

        // Respond with the handled request
        client.print("Command received: "); 
        client.print(commandString);
        client.println("<br />");
        client.println("<br />");

        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
    commandString="null";
  }

  updateLED();
  updateBUZZER();

}

void updateLED(){

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == 0)
      ledState = 1;
    else
      ledState = 0;

    // set the LED with the ledState of the variable:
    setLED(currentColor, ledState);


  }
}
void updateBUZZER(){

  unsigned long currentMillis = millis();

  if (!buzzerOn){
    previousMillis_BUZ = currentMillis; // if buzzer is not on reset timer
  } 
  else if(currentMillis - previousMillis_BUZ < buzzer_interval) {
    digitalWrite(BUZZER_PIN,HIGH);
  } 
  else {
    buzzerOn=false;
    digitalWrite(BUZZER_PIN,LOW);
  }

}

void setLED(int color, int led_state){
  if (led_state==1){

    switch (color) {
    case 0 : 
      red_value=1; 
      green_value=0; 
      blue_value=0; 
      break;
    case 1 : 
      red_value=0; 
      green_value=1; 
      blue_value=0; 
      break;
    case 2 : 
      red_value=0; 
      green_value=0; 
      blue_value=1; 
      break;
    case 3 : 
      red_value=1; 
      green_value=1; 
      blue_value=0; 
      break;
    case 4 : 
      red_value=1; 
      green_value=1; 
      blue_value=1; 
      break;
    case 5 : 
      red_value=0; 
      green_value=0; 
      blue_value=0; 
      break;
    default: 
      Serial.println("Unknown color value"); 
      break;
    }
  }
  else{
    red_value=0; 
    green_value=0; 
    blue_value=0;
  }
  digitalWrite(LED_PIN_RED, red_value);
  digitalWrite(LED_PIN_GREEN, green_value);
  digitalWrite(LED_PIN_BLUE, blue_value);
}


