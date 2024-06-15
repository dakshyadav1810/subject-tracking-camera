#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ESP32Servo.h>

Servo servo;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        Serial.println("onWrite invoked");
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0)
        {
            int intValue = value[0] - '0'; // Parse the first character as an integer
            Serial.print(intValue);
            Serial.println();
            if (intValue == 2)
            {

                Serial.print("Rotating Right");
                int currentAngle = servo.read();
                int targetAngle = currentAngle + 3;
                servo.write(targetAngle);
                Serial.print(targetAngle);
                Serial.print(currentAngle);
                delay(50);
            }
            else if (intValue == 0)
            {
                // Rotate servo left by a small amount
                // rotateServo(-10);
                Serial.print("Rotating Left");
                int currentAngle = servo.read();
                int targetAngle = currentAngle - 2;
                servo.write(targetAngle);
                Serial.print(targetAngle);
                Serial.print(currentAngle);
                delay(50);
            } else {
              Serial.print("Centered");
              
            }
        }
    }
};

void setup()
{
    Serial.begin(115200);
    servo.attach(13);
    servo.write(90);
    BLEDevice::init("Subject Tracking Camera Mount");
    BLEServer *pServer = BLEDevice::createServer();

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("DIY Subject Tracking Camera Mount");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}