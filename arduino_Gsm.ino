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
  Sim.println("AT+CMGS=\"Number\"\r");  // Replace with mobile number
  delay(1000);                                 // Delay of 1 second
  Sim.println(" **** EMERGENCY ALERT !!!!! **** ");    
  Sim.println(" LOCATION  :- Lon,lat  ");
  Sim.println("LOcation on map");    //change as per your use
  Sim.print(" ThingSpeek Monitoring Link :-  ");    // Type in the SMS text you want to send
  Sim.println("    ");   //thingspeak channel
  Sim.println(" I Require Help ................ ");   
  delay(100);                                  // Delay of 0.1 second
  Sim.println((char)26);                       // ASCII code of CTRL+Z (to exit out)
  delay(1000);                                 // Delay of 1 second
  Serial.println("Message sent.");
}
