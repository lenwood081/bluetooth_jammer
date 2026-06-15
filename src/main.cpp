#include <Arduino.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define LED_PIN 2

RH_NRF24 nrf24(4, 16); // CE, CSN pins

bool failed = false;

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
    ;

  if (!nrf24.init())
  {
    failed = true;
    Serial.println("init failed");
  }

  if (!nrf24.setChannel(1))
  {
    failed = true;
    Serial.println("setChannel failed");
  }

  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm))
  {
    failed = true;
    Serial.println("setRF failed");
  }
}

void loop()
{
  if (failed)
  {
    // flash the led rapidly to show we have failed
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }

  // Send message to rf24 test
  Serial.println("Sending to test reciver");
  uint8_t data[] = "Hello World!";
  nrf24.send(data, sizeof(data));

  delay(3000);
}