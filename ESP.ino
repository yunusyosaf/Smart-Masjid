#include <ESP8266WiFi.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "Arduino.h"
#include <EMailSender.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266HTTPClient.h>
#include <stdlib.h>
#include <TimeLib.h>
#include <regex>

#include <BlynkSimpleEsp8266.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Replace with your Wi-Fi credentials

// Twilio API credentials
const char* accountSID = "AC5013fd87e84a9402c5196a5a8de1ebb0";
const char* authToken = "ad9d23675a8a2cc5914dcb392fabfc6a";
const String twilioNumber = "+16187422016";
const String Number1 = "+923430288030";

 
// Array to store Quranic verses
const char* quranicVerses[] = {
};

// Replace with your timezone offset in seconds (+5 hours for Pakistan Standard Time)
const long  utcOffsetInSeconds = 5 * 3600;
String Time;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

EMailSender emailSend("uet.swat@gmail.com", "atzwcjudvjywiylf");  // Mail Object

// WiFi network
char auth[] = "FCJQk52Vxi437L9Ew0fIvnJf2EUpBuBE";  
const char* ssid     = "Wifi";
const char* password = "39933.com";

String Fajr = "5:40 AM";
String Dhuhr = "1:00 PM";
String Asar = "4:45 PM";
String Maghrib = "6:22 PM";
String Isha = "8:30 PM";

String previousFajr = Fajr;
String previousDhuhr = Dhuhr;
String previousAsar = Asar;
String previousMaghrib = Maghrib;
String previousIsha = Isha;

String SMSTime = "9:30 PM";
const int numVerses = sizeof(quranicVerses) / sizeof(quranicVerses[0]);

bool eventCalled = false;
unsigned long previousEventTime = 0;

String Play_Adhan = "";

unsigned long lcdTimer = 0;  // Timer variable    // MILLIS FOR DISPLAY THAT WILL NOT EFFECT THE OTHER CODE EVEN IF DELAY IS THERE
unsigned long lcdInterval = 5000;  // Interval in milliseconds
int displayState = 0;  // Display state variable   

void setup() {
  
  // Start serial
  Serial.begin(9600);
  delay(10);

  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Connect to WiFi and get time from NTP server
  configTime(5.5 * 3600, 0, "pool.ntp.org");

  // ---------Initialize NTP client-----------------------------------

  timeClient.begin();
  timeClient.update();
  Blynk.begin(auth, ssid, password);


  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

}

void loop() {

  Blynk.run();

  Serial.println(PSTime());
  Serial.println(PSDate());
  Serial.println(DAY());
  Serial.println(NoSec());

  NamazTime(); //Namaz Time Funcion
  Daily_SMS();  // Daily Inspiration SMS
  LCD_text(); // Display Time and More on LCD
  checkPrayerTimeChanges();  // Check If Time get changed

    Serial.println(Adhan_time(Asar).substring(1,8));
    if(Play_Adhan == "Play"){
    if(Adhan_time(Fajr).substring(1,8) == NoSec()||Adhan_time(Dhuhr).substring(1,8) == NoSec()||Adhan_time(Asar).substring(1,8) == NoSec()||Adhan_time(Maghrib).substring(1,8) == NoSec()||Adhan_time(Isha).substring(1,8) == NoSec())
    {
      MP3();
      delay(70000);
    }
    }
  

  Blynk.virtualWrite(V0, "Time "+ PSTime());  // Send string value to V0 pin
  Blynk.virtualWrite(V1, DAY().substring(0,3)+" "+ PSDate());  // Send string value to V0 pin

}

String PSTime()
{
  
  // ----------Update time from NTP server--------------
  timeClient.update();
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();
  time_t timestamp = timeClient.getEpochTime();
  struct tm *tm = gmtime(&timestamp);

  // --------- Get current day from time_t--------------
  char day[20];
  strftime(day, sizeof(day), "%A", tm);
  
  bool isAM = true;
  if (hour == 0) {
    hour = 12;
  } else if (hour > 12) {
    hour -= 12;
    isAM = false;
  }

  // Format time and date as string
  String formattedTime = String(hour) + ":" +
                          (minute < 10 ? "0" : "") + String(minute) + ":" +
                          (second < 10 ? "0" : "") + String(second) + " " +
                          (isAM ? "AM" : "PM");
  String formattedDate = String(tm->tm_year + 1900) + "-" +
                          (tm->tm_mon < 9 ? "0" : "") + String(tm->tm_mon + 1) + "-" +
                          (tm->tm_mday < 10 ? "0" : "") + String(tm->tm_mday);


  delay(1000);

  return formattedTime;

}

String PSDate()
{
  
  // ----------Update time from NTP server--------------
  timeClient.update();
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();
  time_t timestamp = timeClient.getEpochTime();
  struct tm *tm = gmtime(&timestamp);

  // --------- Get current day from time_t--------------
  char day[20];
  strftime(day, sizeof(day), "%A", tm);
  
  bool isAM = true;
  if (hour == 0) {
    hour = 12;
  } else if (hour > 12) {
    hour -= 12;
    isAM = false;
  }

  // Format time and date as string
  String formattedTime = String(hour) + ":" +
                          (minute < 10 ? "0" : "") + String(minute) + ":" +
                          (second < 10 ? "0" : "") + String(second) + " " +
                          (isAM ? "AM" : "PM");
  String formattedDate = String(tm->tm_year + 1900) + "-" +
                          (tm->tm_mon < 9 ? "0" : "") + String(tm->tm_mon + 1) + "-" +
                          (tm->tm_mday < 10 ? "0" : "") + String(tm->tm_mday);

  delay(1000);

  return formattedDate;
}

String NoSec()
{
  
  // ----------Update time from NTP server--------------
  timeClient.update();
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();
  time_t timestamp = timeClient.getEpochTime();
  struct tm *tm = gmtime(&timestamp);

  // --------- Get current day from time_t--------------
  char day[20];
  strftime(day, sizeof(day), "%A", tm);
  
  bool isAM = true;
  if (hour == 0) {
    hour = 12;
  } else if (hour > 12) {
    hour -= 12;
    isAM = false;
  }

  // Format time and date as string
  String formattedTime = String(hour) + ":" +
                          (minute < 10 ? "0" : "") + String(minute) + " " +
                          (isAM ? "AM" : "PM");

  delay(1000);

  return formattedTime;
}

String DAY() {
   // ----------Update time from NTP server--------------
 timeClient.update();
  
  // Get current day (0 = Sunday, 1 = Monday, etc.)
  int currentDay = timeClient.getDay();
  String day = "";
  switch (currentDay) {
    case 0:
      day = "Sunday";
      break;
    case 1:
      day = "Monday";
      break;
    case 2:
      day = "Tuesday";
      break;
    case 3:
      day = "Wednesday";
      break;
    case 4:
      day = "Thursday";
      break;
    case 5:
      day = "Friday";
      break;
    case 6:
      day = "Saturday";
      break;
  }

  return day;
}
void NamazTime()
{
  Blynk.virtualWrite(V2, "Fajr    "+ Fajr);  // Send string value to V0 pin
  Blynk.virtualWrite(V3, "Dhuhr   "+ Dhuhr);  // Send string value to V0 pin
  Blynk.virtualWrite(V4, "Asar    "+ Asar);  // Send string value to V0 pin
  Blynk.virtualWrite(V5, "Maghrib "+ Maghrib);  // Send string value to V0 pin
  Blynk.virtualWrite(V6, "Isha    "+ Isha);  // Send string value to V0 pin
}



// BLYNK_WRITE() function to handle the received string
BLYNK_WRITE(V7) // This function will be called every time the text input widget on V1 is updated
{

  String PrayerTime = param.asStr(); // Get the string value from the widget
  String  msg = "Namaz Time Updated: "+PrayerTime;
  const char* msgBody = msg.c_str();
  // Do whatever you want with the string here, such as sending it to another device
  Serial.println(PrayerTime);
  if(PrayerTime != "")
  {
    Blynk.logEvent("namaz_time","Prayer Time Updated!"+ PrayerTime);
    
  }

  if(PrayerTime.indexOf("fajr")!=-1)
  {
    Fajr = PrayerTime.substring(5,17);

  }else if(PrayerTime.indexOf("dhuhr")!=-1)
  {
    Dhuhr = PrayerTime.substring(6,18);

  }else if(PrayerTime.indexOf("asar")!=-1)
  {
    Asar = PrayerTime.substring(5,17);

  }else if(PrayerTime.indexOf("maghrib")!=-1 )
  {
    Maghrib = PrayerTime.substring(8,21);

  }else if(PrayerTime.indexOf("isha")!=-1)
  {
    Isha = PrayerTime.substring(5,17);  // extract the substring containing the Flame sensor value
  }
}

BLYNK_WRITE(V9) // This function will be called every time the text input widget on V1 is updated
{

  String SMStime = param.asStr(); // Get the string value from the widget
  // Do whatever you want with the string here, such as sending it to another device
  Serial.println(SMStime);
  if(SMStime.indexOf("change")!=1)
  {
    SMSTime = SMStime.substring(7,15);
  }

  if(SMStime.indexOf("PlayAdhan")!=1)
  {
    Play_Adhan = "Play";
  }else if(SMStime.indexOf("StopAdhan")!=1)
  {
    Play_Adhan = "stop";
  }else
  {
    Play_Adhan = "";
  }

  if(SMStime.indexOf("Event")!=-1)
  {
    Blynk.logEvent("inspiration_sms",SMStime);
  }

}

void sendSMS(const char* messageBody, const String recipientNumber) {
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
  delay(1000);
  http.end();
}


// Function to select a random verse and store it in the variable "OneAyah"
const char* Ayat() {
  int randomIndex = rand() % numVerses;
  const char* randomVerse = quranicVerses[randomIndex];
  
  // Store the selected verse in the variable "OneAyah"
  const char* OneAyah = randomVerse;
  
  // Print the selected verse to the serial monitor
  return OneAyah;
}

void Daily_SMS()
{
  if(SMSTime == NoSec())
  {
    Blynk.logEvent("inspiration_sms",Ayat());
    sendSMS(Ayat(),Number1);
    Mail("Get Inspired From Islamic Quraan Save Your Iman",String(Ayat()));
    delay(70000);
  }

}

void LCD(int curs1, int curs2 , String txt)
{
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(curs1, curs2);
    display.println(txt);
  display.display();
}

void LCD_text() {

   if(WiFi.status()!=WL_CONNECTED)
  {
    display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
    display.println("Wifi Disconnected");
  }
  // Check if it's time to update the LCD display
  if (millis() - lcdTimer >= lcdInterval) {
    lcdTimer = millis();  // Reset the timer

    // Update the display based on the current state
    switch (displayState) {
      case 0:  // Display time
        display.clearDisplay();
        LCD(5, 10, NoSec());
        LCD(10, 20, DAY());
        LCD(20, 40, PSDate());
        displayState = 1;
        break;
      case 1:  // Display prayer times
        display.clearDisplay();
        LCD(0, 10, "Fajr: " + Fajr);
        LCD(10, 20, "Dhuhr: " + Dhuhr);
        LCD(20, 30, "Asar: " + Asar);
        LCD(30, 40, "Maghrib: " + Maghrib);
        LCD(40, 50, "Isha: " + Isha);
        displayState = 0;
        break;
    }
  }
}

String Adhan_time(String timeStr) {
  // Parse the input time string
  int hour, minute;
  char meridiem[3];
  sscanf(timeStr.c_str(), "%d:%d %2s", &hour, &minute, meridiem);

  // Adjust the hour for PM times
  if (strcmp(meridiem, "PM") == 0 && hour != 12) {
    hour += 12;
  }

  // Convert the time to minutes
  int totalMinutes = hour * 60 + minute;

  // Subtract 10 minutes
  totalMinutes -= 10;

  // Wraparound for negative minutes
  if (totalMinutes < 0) {
    totalMinutes += 24 * 60;
  }

  // Convert the new time back to hours and minutes
  hour = totalMinutes / 60;
  minute = totalMinutes % 60;

  // Format the new time as a string
  char newTimeStr[9];
  sprintf(newTimeStr, "%02d:%02d %s", (hour % 12 == 0) ? 12 : hour % 12, minute, (hour < 12) ? "AM" : "PM");

  return String(newTimeStr);
}

void MP3()
{
  Serial.write((byte)0x7E); // Start code
  Serial.write((byte)0xFF); // Version
  Serial.write((byte)0x06); // Command length
  Serial.write((byte)0x0D); // Command code: play file
  Serial.write((byte)0x00); // Parameter 1: no feedback
  Serial.write((byte)0x00); // Parameter 2: folder number
  Serial.write((byte)0x01); // Parameter 3: file number (Adhan1.mp3)
  Serial.write((byte)0xEF); // End code 
  delay(10000); // Wait for the MP3 to finish playing the file
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

    delay(1000);
}

void checkPrayerTimeChanges() {

  if (Fajr != previousFajr) {
    SMS_MAIL("Fajr", Fajr);
    previousFajr = Fajr;
  }
  if (Dhuhr != previousDhuhr) {
    SMS_MAIL("Dhuhr", Dhuhr);
    previousDhuhr = Dhuhr;
  }
  if (Asar != previousAsar) {
    SMS_MAIL("Asar", Asar);
    previousAsar = Asar;
  }
  if (Maghrib != previousMaghrib) {
    SMS_MAIL("Maghrib", Maghrib);
    previousMaghrib = Maghrib;
  }
  if (Isha != previousIsha) {
    SMS_MAIL("Isha", Isha);
    previousIsha = Isha;
  }
}

void SMS_MAIL(String prayerName, String prayerTime) {
  String mailBody = "Namaz Time Updated:<br>";
  mailBody += prayerName + " prayer time has changed to " + prayerTime + "<br>";
  mailBody += "Always Remember Allah<br>";
  mailBody += String(Ayat());
  const char* smsBody = mailBody.c_str();
  Mail("Namaz Time Updated! ",mailBody);
  delay(5000);
  sendSMS(smsBody,Number1);
}
