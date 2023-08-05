 #include <DHT.h>
#include <NewPing.h>

#define DHTPIN 2
#define DHTTYPE DHT11

// Sable PIR Sensor Setting
const int pirsable1 = 5; // 
const int pirsable2 = 6; // 
const int pirsable3 = 7; // 
const int pirsable4 = 8; // 
const int pirindoor = 4; // Control indoor fan and light
const int piroutdoor = 3; // Control outdoor light and fan
 

// Light Pin's Setting
const int outdoorwhitlight = 22; // 
const int bluestreetlight = 24; //
const int decorationlight = 26; //
const int gombatlight = 28; //
const int watertanklight = 30; //
const int indoorlight = 39; //
const int waterpressure = 34;

// 8th Relay Board
const int waterpump = 23; // 1st Relay >  12v Water Pump to fill water tank               
const int sablevalve1 = 33;  //  6th Relay >
const int sablevalve2 = 27;  //  3rd Relay >
const int sablevalve3 = 29;  //  4th Relay >
const int sablevalve4 = 31;  //  5th Relay > 
const int tankvalve = 25;
const int outdoorfan_two = 35; // 7th Relay >
const int indoorfan_two = 37; // 8th Relay >
const int exhaust = 32;
const int alarm = 50;

// KY-026 Flame Sensor Pin Configuration
const int flame = 40;  // Digital pin connected to the sensor
const int mq5 = 41;

const int trigPin = 9;  // Trig pin connected to Arduino digital pin 2
const int echoPin = 10;  // Echo pin connected to Arduino digital pin 3
const int maxDistance = 200;   // Maximum distance to measure in centimeters

// Create an instance of the NewPing library
NewPing sonar(trigPin, echoPin, maxDistance);


float previousTemp = 0.0;
int previousFlameValue = 0;
int previousmq5 = 0;

DHT dht(DHTPIN, DHTTYPE);

String Data = "";
String ESP = "";
float Temp = 0.0;
float Hum  = 0.0;
int Pin1 = 0;
int Pin2 = 0;
int Pin3 = 0;
int Pin4 = 0;
int Pin5 = 0;
int Pin6 = 0;
int Pin7 = 0;
int Pin8 = 0;

int Emergency1 = 0;
int Emergency2 = 0;

bool motorRunning = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  pinMode(flame, INPUT);
  pinMode(mq5,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(outdoorfan_two, OUTPUT);
  pinMode(indoorfan_two, OUTPUT);
  pinMode(waterpump, OUTPUT);
  pinMode(tankvalve, OUTPUT);
  pinMode(sablevalve1, OUTPUT);
  pinMode(sablevalve2, OUTPUT);
  pinMode(sablevalve3, OUTPUT);
  pinMode(sablevalve4, OUTPUT);

  pinMode(pirsable1, INPUT);
  pinMode(pirsable2, INPUT);
  pinMode(pirsable3, INPUT);
  pinMode(pirsable4, INPUT);
  pinMode(pirindoor, INPUT);
  pinMode(piroutdoor, INPUT);

  pinMode(outdoorwhitlight, OUTPUT);
  pinMode(exhaust,OUTPUT);
  pinMode(bluestreetlight, OUTPUT);
  pinMode(decorationlight, OUTPUT);
  pinMode(gombatlight, OUTPUT);
  pinMode(watertanklight, OUTPUT);
  pinMode(indoorlight, OUTPUT);
  pinMode(waterpressure,OUTPUT); 
  pinMode(alarm,OUTPUT);

  pinMode(42,INPUT); //D0
  pinMode(43,INPUT); //D1
  pinMode(44,INPUT); //D2
  pinMode(45,INPUT);  //D3
  pinMode(46,INPUT);  // D4
  pinMode(47,INPUT);  //D6
  pinMode(48,INPUT);  // D5
  pinMode(49,INPUT); //D7

  delay(5000);

}

void loop() {

  DHT();

  Pump();
  //Valve();
  // put your main code here, to run repeatedly:
  // Reading data from ESP8266
Pin1 = digitalRead(42);  // D0 
Pin2 = digitalRead(43);  //D1
Pin3 = digitalRead(44);  //D2
Pin4 = digitalRead(45);  // D3
Pin5 = digitalRead(46);  //D4
Pin6 = digitalRead(47);  //D6
Pin7 = digitalRead(48); // D5
Pin8 = digitalRead(49);  //D7

if(Pin2 == 1 || PIR_Sensor(pirindoor)== 1)
{
  digitalWrite(indoorlight,1);
}else
{
  digitalWrite(indoorlight,0);
}

if(Pin5 == 1 || PIR_Sensor(piroutdoor)== 1)
{
  digitalWrite(outdoorwhitlight,1);
}else
{
  digitalWrite(outdoorwhitlight,0);
}


if(Pin6 == 1)
{
  digitalWrite(bluestreetlight,1);
}else
{
  digitalWrite(bluestreetlight,0);
}



if(Pin7 == 1 && PIR_Sensor(pirsable1) == true)
{
  digitalWrite(tankvalve,1);
  digitalWrite(sablevalve1,1);
    digitalWrite(waterpressure,0);
}else if(Pin7 == 1 && PIR_Sensor(pirsable2) == true)
{
  digitalWrite(tankvalve,1);
  digitalWrite(sablevalve2,1);
    digitalWrite(waterpressure,0);
}else if(Pin7 == 1 && PIR_Sensor(pirsable4) == true)
{
  digitalWrite(tankvalve,1);
  digitalWrite(sablevalve4,1);
    digitalWrite(waterpressure,0);
}
else
{
  digitalWrite(tankvalve,0);
  digitalWrite(waterpressure,1);
  digitalWrite(sablevalve1,0);
  digitalWrite(sablevalve2,0);
  digitalWrite(sablevalve3,0);
  digitalWrite(sablevalve4,0);
}



if(Pin4 == 1)
{
  if(Temp>12)
  {
    digitalWrite(outdoorfan_two,1);
  }else

  {
    digitalWrite(outdoorfan_two,0);
  }
}
else

  {
    digitalWrite(outdoorfan_two,0);
  }
if(Pin3 == 1)
{
  if(Temp>12)
  {
    digitalWrite(indoorfan_two,1);
  }else

  {
    digitalWrite(indoorfan_two,0);
  }
}else

  {
    digitalWrite(indoorfan_two,0);
  }



if(Pin1 == 1)
{
  if(Temp>12)
  {
    digitalWrite(exhaust,0);
  }else

  {
    digitalWrite(exhaust,1);
  }
}else

  {
    digitalWrite(exhaust,1);
  }

if(Pin8 ==1)
{
  digitalWrite(decorationlight,1);
}else
{
  digitalWrite(decorationlight,0);
}



  //digitalWrite(outdoorfan_two,HIGH);
  //digitalWrite(indoorfan_two,HIGH);

  if(Pin1 == 1 || Pin2 == 1 || Pin3 == 1 || Pin4 == 1 || Pin5 == 1 || Pin6 == 1|| Pin7 == 1 ||Pin8 == 1)
  {
    Data = String(Emergency1)+String(Emergency2)+"T: "+String(Temp)+" H: "+String(Hum) + " L: " + String(WaterLevel()) + "   M: "+ String(MQ5()) +"   F:   "+ String(Flame());

   Serial.println(Data);
  }else
  {
    check();

    Data = String(Emergency1)+String(Emergency2)+"T: "+String(Temp)+" H: "+String(Hum) + " L: " + String(WaterLevel()) + "   M: "+ String(MQ5()) +"   F:   "+ String(Flame());

   Serial.println(Data);
   Analysis();

  }
  delay(1000);
}

void check()
{
  if(!isnan(Temp))
  {

  if(Emergency1 == 1)
  {
    digitalWrite(alarm,1);
  }else
  {
    digitalWrite(alarm,0);
  }
  }else
  Serial.println("Wait for response");
}

void DHT()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Temp =temperature;
  Hum  =humidity;

}

int WaterLevel()
{
  // Send a ping and get the distance measurement in centimeters
  int distance = sonar.ping_cm();
  // Wait for a short delay before taking the next measurement
  return distance;
}

int MQ5()
{
   // Read the analog sensor value
  int sensorValue = analogRead(A1);
  return sensorValue;
}

int Flame()
{
  // Read the sensor value
  int flameValue = digitalRead(flame);
  int flamA0 = analogRead(A0);
  return flamA0;
}

bool PIR_Sensor(int sensorPin) {
  int val = digitalRead(sensorPin);   // read sensor value
  if (val == HIGH) 
  {  return true;  } 
  else 
  {  return false; }
}

void Analysis()
{

int flameValue = Flame();
  float temperature = Temp;
  int mq5 = MQ5();

  float tempChange = abs(temperature - previousTemp);
  int flameChange = abs(flameValue - previousFlameValue);
  int mq5Change = abs(mq5-previousmq5);

  if(isnan(Temp))
  {
    Emergency1 = 0;
    Emergency2 = 0;
  }else
  {
  if (tempChange > Temp+5 || Flame()<=0 )
    {
      Emergency1 = 1;
    }else
    {
      Emergency1 = 0;
    }

  if(mq5Change > MQ5() || MQ5()<0)
  {
    Emergency2 = 1;
  }else
  {
    Emergency2 = 0;
 }
  }

  previousmq5 = mq5Change;
  previousTemp = tempChange;
  previousFlameValue = flameChange;
}



void Pump()
{
  if (WaterLevel()>=14) {
    if (!motorRunning) {
      digitalWrite(waterpump, 1);  // Start the motor
      digitalWrite(tankvalve, 0);  // Start the motor
      motorRunning = true;
    }
  } else if (WaterLevel() <= 3) {
    if (motorRunning) {
      digitalWrite(waterpump, 0);  // Stop the motor
      motorRunning = false;
    }
  }
  }
