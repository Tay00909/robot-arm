#include <Wire.h> // arduino wire library allows the wire comunicate.
#include <Adafruit_PWMServoDriver.h> // Driver class, allows the driver kit to create servo pulses.
#include <NewPing.h> // Library for ultrasonic sensor.

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); // allows the wire to communicate with the driver kit.

enum { BASE = 0, WRIST = 1, GRIP = 2 }; // creates variable name for each of the servo motos.

const uint16_t MIN_US = 500; // setting max/min for strenth of each servo motor so there is not overpower(waste battery/overspin)
const uint16_t MAX_US = 2500;
const uint8_t TRIG1 = 7, ECHO1 = 6;

NewPing sonar1(TRIG1, ECHO1, 200); // pin numbers used for first ultrasonic sensor 
// sonar 1 is a previously written function provided that controls sets max distance the echo will happen

struct CmdPacket { uint8_t id; uint16_t us; }; //defines command packet to send iunfo from computer to pwm channel and from you to the pulse in microseconds.
void setup() {                           
    Serial.begin(115200);
    pwm.begin(); 
    pwm.setPWMFreq(50);   // sets the frequency of the servo driver to 50Hz, which is the standard for servos.
  }// opens a USB serial at the location so the rasbery Pie can give conversation/commands to the arduino.

  void loop() {  
    // 1) handle incoming command bytes
    while (Serial.available() >= sizeof(CmdPacket)) {   
      CmdPacket cmd;  
      Serial.readBytes((char*)&cmd, sizeof(cmd));         
      cmd.us = constrain(cmd.us, MIN_US, MAX_US);          
      pwm.writeMicroseconds(cmd.id, cmd.us);          
    }
    static uint32_t t0 = 0;

    if (millis() - t0 > 50) {
        t0 = millis();
        uint16_t mm = sonar1.ping_cm();         // 0 = no echo
        Serial.print("R"); Serial.println(mm);  // e.g., "R123"
    }// initializes a timer (static) line 31, line 33 creates an update loop every 50milliseconds, finding the distance in cm based on results from the ultrasonic senor line 35, finally line 36 shows the operator what the distance is in a print function.
    






