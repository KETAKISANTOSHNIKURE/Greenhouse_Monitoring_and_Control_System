#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN D5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define FAN_PIN D2   // FAN RELAY
#define MOTOR_PIN D7
WidgetLED FAN(V0);
WidgetLED MOTOR(V1);
WidgetLED LDR(V7);

char auth[] = "DN5Ty98xhCl733UCqjqxikdr6R8B-j_6";
char ssid[] = "nodemcU";
char pass[] = "12345678";

float humDHT = 0;
float tempDHT = 0;
int Val=0;
int Val2=0;

int percentage=0;
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent=0;

const int ldr = D0;
const int led = D3;




// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  pinMode(ldr,INPUT);
  pinMode(led,OUTPUT);
  
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Blynk.begin( auth, ssid , pass );
}

BLYNK_WRITE(V2)
{
  Val = param.asInt(); // assigning incoming value from pin V3 to a variable

  Serial.print(" The Threshhold value is: ");
  Serial.println(Val);
  Serial.println();

}

BLYNK_WRITE(V4)
{
  Val2 = param.asInt(); // assigning incoming value from pin V3 to a variable

  Serial.print(" The Threshhold value is: ");
  Serial.println(Val2);
  Serial.println();

}

void loop() {
  
  Blynk.run();

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humDHT = dht.readHumidity();
  // Read temperature as Celsius (the default)
  tempDHT = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humDHT) || isnan(tempDHT))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(tempDHT);
  Serial.print(F("°C "));
  Serial.println();
  Serial.print(F("Humidity: "));
  Serial.print(humDHT);
  Serial.print(F("%"));
  Serial.println();
  
  Serial.println("***");
  Serial.println();

soilMoistureValue = analogRead(A0);

Serial.println(percentage);

percentage = map(soilMoistureValue, 490, 1023, 100, 0);

if(percentage < 10)  

{

  Serial.println(" pump on");

  digitalWrite(7,LOW);
  MOTOR.on();
}

if(percentage >80)

{

  Serial.println("pump off");

  digitalWrite(7,HIGH);
  MOTOR.off();
  

}
  // Compare Threshold value from Blynk and DHT Temperature value.
  if (Val > tempDHT)
  {
    digitalWrite(FAN_PIN, HIGH);
        FAN.off();

}
  else {
    digitalWrite(FAN_PIN, LOW);  
        FAN.on();
 
  } 
  //ldr status
Serial.print(digitalRead(ldr));
if(digitalRead(ldr) == 1)
{
      
  digitalWrite(led, HIGH);
        LDR.off();

}
  else {
    digitalWrite(led, LOW);
    LDR.on();
    delay(3000);
    digitalWrite(led, HIGH);
    
  }

  
  
  Blynk.virtualWrite(V5, tempDHT);
  Blynk.virtualWrite(V6, humDHT);
  Blynk.virtualWrite(V3, percentage);
}
