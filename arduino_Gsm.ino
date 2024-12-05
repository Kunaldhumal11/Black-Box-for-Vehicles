#include <SoftwareSerial.h>

SoftwareSerial Sim(9, 10);//tx rx

const int sendButtonPin = 2;  // Pin for send SMS button
 // Pin for receive SMS button

void setup() {
  Sim.begin(9600);                            // Setting the baud rate of Sim Module  
  Serial.begin(9600);                         // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(sendButtonPin, INPUT_PULLUP);       // Configure send button pin as input with pull-up
  //pinMode(receiveButtonPin, INPUT_PULLUP);    // Configure receive button pin as input with pull-up
  delay(100);
  Serial.println("Setup complete. Press the buttons to send or receive SMS.");
}

void loop() {
  // Read the state of the send and receive buttons
  int sendButtonState = digitalRead(sendButtonPin);
  
  // If the send button is pressed (active low)
  if (sendButtonState == LOW) {
    SendMessage();
    delay(1000);  // Debounce delay to prevent multiple triggers
  }

  // Forward any data from the GSM module to the Serial Monitor
  if (Sim.available() > 0) {
    Serial.write(Sim.read());
  }
}

void SendMessage() { 
  Serial.println("Sending message...");
  Sim.println("AT+CMGF=1");                    // Sets the Sim Module in send SMS mode
  delay(1000);                                 // Delay of 1 second
  Sim.println("AT+CMGS=\"+919132171718\"\r");  // Replace with mobile number
  delay(1000);                                 // Delay of 1 second
  Sim.println(" **** EMERGENCY ALERT !!!!! **** ");    
  Sim.println(" LOCATION  :- 18.4642530, 73.8677942  ");
  Sim.println("https://www.google.com/maps/place/18%C2%B027'51.3%22N+73%C2%B052'04.1%22E/@18.464253,73.8652193,17z/data=!3m1!4b1!4m4!3m3!8m2!3d18.464253!4d73.8677942?entry=ttu");    
  Sim.print(" ThingSpeek Monitoring Link :-  ");    // Type in the SMS text you want to send
  Sim.println("   https://thingspeak.com/channels/2476652 ");   
  Sim.println(" I Require Help ................ ");   
  delay(100);                                  // Delay of 0.1 second
  Sim.println((char)26);                       // ASCII code of CTRL+Z (to exit out)
  delay(1000);                                 // Delay of 1 second
  Serial.println("Message sent.");
}