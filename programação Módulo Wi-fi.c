#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
WidgetLCD lcd(V2);
String data;
String I;
char auth[] = "hTCKr_dSZHf8bfI1qQU7J07aAUmBnRnB";
char ssid[] = "Net Virtua_240. 2.4ghz";
char pass[] = "201978770"; 

SimpleTimer timer;
String myString;
char rdata;
#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on
physical board "D4 pin on the ndoemcu Module"
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature DS18B20(&oneWire);
float temp;
float Fahrenheit=0;
void myTimerEvent()
{
// You can send any value at any time.
// Please don't send more that 10 values per second.
Blynk.virtualWrite(V1, millis() / 1000);
}
void setup()
{
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
DS18B20.begin();
timer.setInterval(1000L, getSendData);
timer.setInterval(1000L,sensorvalue1);
}
void loop()
{
if (Serial.available() == 0 )
{
timer.run(); // Initiates SimpleTimer
Blynk.run(); 
}
if (Serial.available() > 0 )
{
rdata = Serial.read();
myString = myString+ rdata;
Serial.print(rdata);
if( rdata == '\n')
{
I = getValue(myString, ',', 0);
myString = "";
// Serial.println(I);
lcd.print(0,0,"pH Value:");
}
}
}
void sensorvalue1()
{
data = data + I;
lcd.print(0,0,"pH Value:");
lcd.print(0,1,data);
data = "";
}
String getValue(String data, char separator, int index)
{
int found = 0;
int strIndex[] = { 0, -1 };
int maxIndex = data.length() - 1; 
for (int i = 0; i <= maxIndex && found <= index; i++) {
if (data.charAt(i) == separator || i == maxIndex) {
found++;
strIndex[0] = strIndex[1] + 1;
strIndex[1] = (i == maxIndex) ? i+1 : i;
}
}
return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/***************************************************
* Send Sensor data to Blynk
**************************************************/
void getSendData()
{
DS18B20.requestTemperatures();
temp = DS18B20.getTempCByIndex(0); // Celcius
Fahrenheit = DS18B20.toFahrenheit(temp); // Fahrenheit
Serial.println(temp);
Serial.println(Fahrenheit);
Blynk.virtualWrite(V3, temp); //virtual pin V3
Blynk.virtualWrite(V4, Fahrenheit); //virtual pin V4
} 