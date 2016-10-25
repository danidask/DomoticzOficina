/*
 * MySensors Intercom Interface
  *******************************
 * DESCRIPTION
 * It sends bell signals to the controller and receive orders
 * to open the door
 *
 * REVISION HISTORY
 * 05/01/2016 V1.0 Daniel Alvarez
 * 09/01/2016 V1.1 Daniel Alvarez
 * 23/02/2016 V1.2 Daniel Alvarez
 * 25/02/2016 V1.3 Daniel Alvarez
  */

#include <MySensor.h>  
#include <SPI.h>

#define DEBUG 0
#define REPEATER 0 // This node can work as a signal repeater

#define BELL_SENSOR_PIN 3   // Entrada digital para el sensor (Solo 2 y 3 generan interrupciones)
//#define INTERRUPT BELL_SENSOR_PIN-2 // Normalmente la interrupcion es pin-2 (en nano)
#define BELL_SENSOR_CHILD_ID 1   // Id hijo del sensor de movimiento
#define DOOR_RELAY_PIN 4   // Entrada digital para el sensor (Solo 2 y 3 generan interrupciones)
#define DOOR_RELAY_CHILD_ID 2   // Id hijo del sensor de movimiento
#define RELAY_ON 0  // Some relays are activated with a 0, others with 1
#define RELAY_OFF 1
#define RELAY_TIME 3000 // How long the open door signal will be (in miliseconds)

MySensor gw;
int value;
int oldValue=0;
bool state;
bool relay_action = 0;

// Initialize motion message
MyMessage msgBell(BELL_SENSOR_CHILD_ID, V_TRIPPED);
MyMessage msgDoor(DOOR_RELAY_CHILD_ID, V_STATUS);

void setup()  
{ 
  // Initialize library and add callback for incoming messages
  gw.begin(incomingMessage, AUTO, REPEATER);

  // Send the Sketch Version Information to the Gateway
  gw.sendSketchInfo("Intercom interface", "1.3");

  pinMode(BELL_SENSOR_PIN, INPUT);
  //digitalWrite(MOTION_INPUT_SENSOR,HIGH); //activa pullips (normalmente no necesario)
  pinMode(DOOR_RELAY_PIN, OUTPUT);
  digitalWrite(DOOR_RELAY_PIN, RELAY_OFF);
  
  // Register all sensors to gw (they will be created as child devices)
  gw.present(BELL_SENSOR_CHILD_ID, S_DOOR);
  gw.present(DOOR_RELAY_CHILD_ID, S_LIGHT);
}

void loop()     
{
  gw.process(); // Process incoming messages

  value = digitalRead(BELL_SENSOR_PIN);
  if (value != oldValue) {
     gw.wait(75);
     value = digitalRead(BELL_SENSOR_PIN);
     if (value != oldValue){
      gw.wait(75);
      value = digitalRead(BELL_SENSOR_PIN);
      if (value != oldValue){
        oldValue = value;
        gw.send(msgBell.set(value==HIGH ? 1 : 0)); // Send in the new value
        gw.wait(1500); //avoid another triguer
       }
     }
  }
  if (relay_action) {
    // Trigger relay for a while
    digitalWrite(DOOR_RELAY_PIN, RELAY_ON);
    gw.wait(RELAY_TIME);
    digitalWrite(DOOR_RELAY_PIN, RELAY_OFF);    
  relay_action = 0;
  gw.send(msgDoor.set(0));
  }
  gw.wait(50);
}
// Incoming Messages
void incomingMessage(const MyMessage &message) {
  #ifdef DEBUG
  if (message.isAck()) {
    Serial.println("This is an ack from gateway");
  }
  #endif
  if (message.type == V_LIGHT) {
    state = message.getBool();
    if (state) //only process 1 (0 will be ignored)
      relay_action = 1;
    #ifdef DEBUG
    Serial.print("Incoming change for sensor:");
    Serial.print(message.sensor);
    Serial.print(", New status: ");
    Serial.println(state);
    #endif
  } 
}
