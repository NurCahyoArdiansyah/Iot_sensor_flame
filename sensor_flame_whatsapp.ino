#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>

const char* ssid = "ardiansyah"; //nama wifi
const char* password = "ardiansyah12"; //password wifi

const int flamePin = D2; // Flame sensor connected to digital pin D2

String phoneNumber = "+6281615530299";  //nomor whatsapp yang telah mengaktifkan bot sensor
String apiKey = "4065301";  //apikey yang dikirimkan oleh bot 

bool messageSent = false; // Variable to track if message has been sent

void sendMessage(String message){
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + urlEncode(message) + "&apikey=" + apiKey ;
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Pesan Terkirim ");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(flamePin, INPUT); // Set flamePin as input
}

void loop() {
  int flameStatus = digitalRead(flamePin); // Read flame sensor status
  if(flameStatus == HIGH && !messageSent) { // If flame detected and message not sent
    sendMessage("Bahaya !!!!! /nTelah terdeteksi adanya API/nDiharapakan segera mengungsi");
    messageSent = true; // Set messageSent to true to indicate message has been sent
    delay(5000); // Delay 10 seconds to avoid spamming messages
  }
  else if (flameStatus == LOW && messageSent) { // If flame not detected and message has been sent
    messageSent = false; // Reset messageSent to false
  }
  delay(1000); // Check every second
}
