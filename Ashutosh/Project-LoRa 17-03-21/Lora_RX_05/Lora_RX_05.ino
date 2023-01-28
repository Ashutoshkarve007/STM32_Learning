#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

#define LED_Busy    D0 
#define LED_OUT     D5
#define SW          D2 
#define PROG        D3
#define CTS         D6

#define SwOff       HIGH
#define SwOn        LOW
#define DeBDelayVal 10

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "X4EudSfxVQluYONL2Qj7gOori9OO-1Y8";
char auth[] = "ygLMRgn3Pmj7-MIM67GShLHvRBcPsulH";


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "mace";
char pass[] = "mace6009";

int msCount = 0,deBDelay;
bool swFlag = false;
char serialRx[50] = "{\"T\":16.3,\"H\":23.4,\"M\":34.5}";
char serialPoint=0,serialRxByte;
char json[] = "{\"T\":16.3,\"H\":23.4,\"M\":34.5}";
      
float Temperature =31.2, Humidity=67;
float moisture_percentage = 50;
int sw_m = 0;

Ticker ms_100;

void clktick(){
  if(deBDelay) deBDelay--;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(100);
  Blynk.begin(auth, ssid, pass);
  
  pinMode(LED_Busy,   OUTPUT);
  pinMode(LED_OUT,    OUTPUT);
  //delay(100);
  pinMode(SW,   INPUT_PULLUP);
  
  pinMode(PROG,       INPUT);
  pinMode(CTS,        INPUT);
  
  Serial.write("START\n");
  //delay(1000);
  Serial.swap();

  digitalWrite(PROG ,0);
  digitalWrite(CTS  ,0);

  ms_100.attach_ms(100, clktick);
  digitalWrite(LED_Busy,HIGH);
}
void extract_lora_data(){                                             // To Get data from usart Json
  StaticJsonDocument<50> jsonRx;
  DeserializationError error = deserializeJson(jsonRx, serialRx);     // Take 
  if (error) {
    return;
  }
  
  Temperature         = jsonRx["T"];
  Humidity            = jsonRx["H"];
  moisture_percentage = jsonRx["M"];
  Blynk.virtualWrite(V0, Temperature);
  Blynk.virtualWrite(V1, Humidity);
  Blynk.virtualWrite(V2, moisture_percentage);
  jsonRx.clear();
}
void lora_receive(){
  if (Serial.available() > 0) {
    serialRxByte = Serial.read();
    if(serialRxByte == '{') serialPoint = 0;
    
    serialRx[serialPoint] = serialRxByte;
    serialPoint++;
    if(serialRxByte == '}'){
      extract_lora_data();
      serialPoint=0;
    } 
  }
}

BLYNK_WRITE(V3)                        // Button Press from BLYNK end
{
  int sw_m = param.asInt();
  if(sw_m == 0)
  {
    Serial.write("R=0;");
    digitalWrite(LED_OUT,0);
    swFlag = 0;
  } else {
    Serial.write("R=1;");
    digitalWrite(LED_OUT,1);
    swFlag = 1;
  }
}
    
void sw_check(){  
  if(digitalRead(SW) == SwOn && !deBDelay){
    if(swFlag){
      Serial.write("R=0;");
      digitalWrite(LED_OUT,0);
      Blynk.virtualWrite(V3, 0);
      swFlag = 0;
      deBDelay = DeBDelayVal;  
    } else{
      Serial.write("R=1;");
      digitalWrite(LED_OUT,1);
      Blynk.virtualWrite(V3, 1);
      swFlag = 1;
      deBDelay = DeBDelayVal;
    }
  }
}

void loop(){
  Blynk.run();
  lora_receive();
  sw_check();
}
