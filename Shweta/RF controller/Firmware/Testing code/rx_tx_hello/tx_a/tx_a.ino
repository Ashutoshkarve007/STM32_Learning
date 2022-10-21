#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(7, 8);

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;
char payload[7] = "hello";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }
  Serial.println(F("RF24/examples/GettingStarted"));
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payload));
  radio.openWritingPipe(address[radioNumber]);
  radio.stopListening();
}

void loop() {

  unsigned long start_timer = micros();                // start the timer
  //for (int i = 0 ; i < 7 ; i++)
  
    radio.write(&payload, 7);  // transmit & save the report
    unsigned long end_timer = micros();                  // end the timer

    Serial.print(F("Transmitting... "));  // payload was delivered
    Serial.print(F("Time req = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload);  // print payload sent
    delay(1000);
  
}
