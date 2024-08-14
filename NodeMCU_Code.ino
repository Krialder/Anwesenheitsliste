#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Server URL
const char* serverName = "http://your_server_ip/handle_rfid.php";

// Baud rate for serial communication with Mega 2560
#define BAUD_RATE 9600

// NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 1); // CET is UTC+1

void setup() 
{
    Serial.begin(BAUD_RATE); // Initialize Serial for communication with Mega 2560
    WiFi.begin(ssid, password); // Connect to Wi-Fi

    // Wait for the Wi-Fi to connect
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi");

    // Initialize NTP Client
    timeClient.begin();
}

void loop() 
{
    timeClient.update();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();

    // Auto-logout at 16:00 CET
    if (currentHour == 16 && currentMinute == 0) 
    {
        autoLogout();
        delay(60000); // Wait for a minute to avoid multiple logout requests
    }

    if (Serial.available()) 
    {
        // Read RFID data from Mega 2560
        String rfidTag = Serial.readStringUntil('\n');
        rfidTag.trim(); // Remove any whitespace/newline characters

        if (rfidTag.length() > 0) 
        {
            // Send RFID data to the server
            if (WiFi.status() == WL_CONNECTED) 
            {
                HTTPClient http;
                http.begin(serverName);
                http.addHeader("Content-Type", "application/x-www-form-urlencoded");

                String httpRequestData = "rfid=" + rfidTag;
                int httpResponseCode = http.POST(httpRequestData);

                if (httpResponseCode > 0) {
                    String response = http.getString();
                    Serial.println("Server Response: " + response);
                } else {
                    Serial.println("Error sending POST request");
                }

                http.end(); // Close the connection
            } 
            else 
            {
                Serial.println("Wi-Fi not connected");
            }
        }
    }
}

void autoLogout() 
{
    if (WiFi.status() == WL_CONNECTED) 
    {
        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String httpRequestData = "auto_logout=true";
        int httpResponseCode = http.POST(httpRequestData);

        if (httpResponseCode > 0) 
        {
            String response = http.getString();
            Serial.println("Auto-logout Response: " + response);
        } 
        else 
        {
            Serial.println("Error sending auto-logout POST request");
        }

        http.end(); // Close the connection
    } 
    else 
    {
        Serial.println("Wi-Fi not connected for auto-logout");
    }
}
