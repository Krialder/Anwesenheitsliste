#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

class RFIDManager 
{
    public:
    void begin() 
    {
        SPI.begin();
        rfid.PCD_Init();
    }

    String readRFID() 
    {
        if (!rfid.PICC_IsNewCardPresent()) return "";
        if (!rfid.PICC_ReadCardSerial()) return "";

        String rfidTag = "";
        for (byte i = 0; i < rfid.uid.size; i++) 
        {
            rfidTag += String(rfid.uid.uidByte[i], HEX);
        }
        rfid.PICC_HaltA();
        return rfidTag;
    }
    private:
    MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
};

RFIDManager rfidManager;

void setup() 
{
    Serial.begin(9600); // Initialize Serial communication with NodeMCU
    rfidManager.begin();
}

void loop()
{
    String rfidTag = rfidManager.readRFID();
    if (rfidTag != "")
    {
        Serial.println(rfidTag); // Send RFID tag to NodeMCU via Serial
        delay(2000);
    }
}
