#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <EMailSender.h>
#include <ESP8266HTTPClient.h>


EMailSender emailSend("uet.swat@gmail.com", "atzwcjudvjywiylf");  // Mail Object

char auth[] = "ZGCdftu9cSEeA-Bhcr_1P-edQ_vRgtn2";
const char* ssid = "Wifi";
const char* password = "39933.com";

// Twilio API credentials
const char* accountSID = "AC5013fd87e84a9402c5196a5a8de1ebb0";
const char* authToken = "ad9d23675a8a2cc5914dcb392fabfc6a";
const String twilioNumber = "+16187422016";
const String Number1 = "+923430288030";

int Temp;
int Hum;
int Level;
int gase;
int Flame;
int Fire;
int MQ5;

String Mega;
void setup() {
  Serial.begin(115200); // Arduino Serial Monito
  Wifi();
  Blynk.begin(auth, ssid, password);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);

}

void loop() {
  Blynk.run();

  if (Serial.available()>0) {
    Mega = Serial.readString();
    //Serial.println(Temp);
  Serial.println(Mega);
   Temp = Mega.substring(5,7).toInt();
  Hum = Mega.substring(14,16).toInt();
  Level = Mega.substring(23,26).toInt();
  gase = Mega.substring(30,33).toInt();
  Flame = Mega.substring(40,44).toInt();
  MQ5 = Mega.substring(0,1).toInt();
  Fire = Mega.substring(1,2).toInt();
  }

  Blynk.virtualWrite(V0,Temp);
  Blynk.virtualWrite(V3,Hum);
  Blynk.virtualWrite(V5,Level);
  Blynk.virtualWrite(V8,gase);
  Blynk.virtualWrite(V9,Flame);

  if(Fire==1)
  {
    String message = "Emergency Please there is fire in Masjid please visit Location is Pabbi, Cherat Rd, Jallozai, Nowshera, Khyber Pakhtunkhwa, Pakistan! click on it to launch google map. or use link https://goo.gl/maps/CeCtw3ZzNeoXtoxV7 ";  // Body of the SMS message"
    Blynk.logEvent("alertmasjid",message);
    sendSMS(message,Number1);
    Mail("Fire Alert In Masjid!!!!",message);
    delay(30000);
  }else if(MQ5==1)
  {
    String message = "There is Gase Leakage detected in the Smart Masjid Please Take Action!";
    Blynk.logEvent("alertmasjid","Gase Leakage Alert! Please Take Action");
    sendSMS(message,Number1);
    Mail("Leakage Aler!!",message);
    delay(30000);
  }

  //Serial.println(Mega.substring(0,1));
  //Serial.println(Mega.substring(1,2));

  delay(1000); 
}

void Wifi()
{
  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
}

Serial.println("Connected to WiFi!");
}

void sendSMS(String messageBody, const String recipientNumber) {
  String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(accountSID) + "/Messages.json";

  WiFiClientSecure client;  // Use WiFiClientSecure for HTTPS
  client.setInsecure();  // Ignore SSL certificate verification (for testing purposes)

  HTTPClient http;

  http.begin(client, url);  // Pass the client object itself
  http.setAuthorization(accountSID, authToken);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //"Emergency Please there is fire in UET Jalozai please visit Location is Pabbi, Cherat Rd, Jallozai, Nowshera, Khyber Pakhtunkhwa, Pakistan! click on it to launch google map. or use link https://goo.gl/maps/CeCtw3ZzNeoXtoxV7 ";  // Body of the SMS message

  String postData = "To=" + recipientNumber + "&From=" + twilioNumber + "&Body=" + messageBody;
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("Twilio request sent. Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending request. HTTP error code: ");
    Serial.println(httpResponseCode);
  }
  delay(3000);
  http.end();
}


void Mail(String Subject, String Body)
{
  // put your main code here, to run repeatedly:

  EMailSender::EMailMessage message;
    message.subject = Subject;
    message.message = Body;

    EMailSender::Response resp = emailSend.send("appleyounas@gmail.com", message);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
    delay(3000);
}

BLYNK_WRITE(V11) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D0,HIGH);
  }else
  {
    digitalWrite(D0,LOW);
  }
}

BLYNK_WRITE(V1) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D1,HIGH);
  }else
  {
    digitalWrite(D1,LOW);
  }
}
BLYNK_WRITE(V12) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D2,HIGH);
  }else
  {
    digitalWrite(D2,LOW);
  }
}
BLYNK_WRITE(V10) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D3,HIGH);
  }else
  {
    digitalWrite(D3,LOW);
  }
}
BLYNK_WRITE(V2) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D4,HIGH);
  }else
  {
    digitalWrite(D4,LOW);
  }
}
BLYNK_WRITE(V4) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D5,HIGH);
  }else
  {
    digitalWrite(D5,LOW);
  }
}
BLYNK_WRITE(V6) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D6,HIGH);
  }else
  {
    digitalWrite(D6,LOW);
  }
}
BLYNK_WRITE(V13) {
  int value = param.asInt(); // Read the integer value from virtual pin 1
  if(value == 1)
  {
    digitalWrite(D7,HIGH);
  }else
  {
    digitalWrite(D7,LOW);
  }
}
