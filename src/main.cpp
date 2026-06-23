#include <Arduino.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "RF24.h"
#include "esp_bt.h"
#include "esp_wifi.h"

#define LED_PIN 2

bool configureRadio(RF24 &radio, int channel, SPIClass *spi);
void jamBLE();
void jamBluetoothDifferentChannelsFull();
void jamBluetoothOptimal();
void jamAll();

SPIClass *spiVSPI = nullptr;
SPIClass *spiHSPI = nullptr;

RF24 radioVSPI(4, 5, RF24_SPI_SPEED);   // CE, CSN pins
RF24 radioHSPI(16, 15, RF24_SPI_SPEED); // CE, CSN pins

int bluetooth_channels[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41};            // Bluetooth channels (2-41)
int bluetooth_channelsTwo[] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80}; // Bluetooth channels (41-80)
int bluetooth_channelsOptimal[] = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};
int ble_channels[] = {2, 26, 80}; // advertising channels for BLE

bool failed = false;
String failedMessage = "Fail message: ";

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_wifi_disconnect();

  Serial.begin(9600);
  while (!Serial)
    ;

  spiVSPI = new SPIClass(VSPI);
  spiVSPI->begin();
  if (!configureRadio(radioVSPI, ble_channels[0], spiVSPI))
  {
    failed = true;
    failedMessage += "Failed to configure radio on VSPI";
    Serial.println(failedMessage);
  }

  spiHSPI = new SPIClass(HSPI);
  spiHSPI->begin();
  if (!configureRadio(radioHSPI, bluetooth_channels[0], spiHSPI))
  {
    failed = true;
    failedMessage += "Failed to configure radio on HSPI";
    Serial.println(failedMessage);
  }

  digitalWrite(LED_PIN, HIGH);
}

bool configureRadio(RF24 &radio, int channel, SPIClass *spi)
{
  // Set the radio to output a constant carrier wave on the specified channel. This is used to jam Bluetooth and Wi-Fi signals.
  if (radio.begin(spi))
  {
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.startConstCarrier(RF24_PA_MAX, channel);
    return true;
  }
  return false;
}

void loop()
{
  if (failed)
  {
    // flash the led rapidly to show we have failed
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    Serial.println(failedMessage);
  }
  jamBluetoothDifferentChannelsFull();
}

void jamBLE()
{
  int randomIndex = random(0, sizeof(ble_channels) / sizeof(ble_channels[0]));
  int channel = ble_channels[randomIndex];

  radioVSPI.setChannel(channel);
  radioHSPI.setChannel(channel);
}

void jamBluetoothDifferentChannelsFull()
{
  int randomIndex = random(0, sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]));
  int channel = bluetooth_channels[randomIndex];
  int nextChannel = bluetooth_channelsTwo[randomIndex];

  radioVSPI.setChannel(channel);
  radioHSPI.setChannel(nextChannel);
}

void jamBluetoothOptimal()
{
  int randomIndex = random(0, sizeof(bluetooth_channelsOptimal) / sizeof(bluetooth_channelsOptimal[0]));
  int channel = bluetooth_channelsOptimal[randomIndex];

  radioVSPI.setChannel(channel);
  radioHSPI.setChannel(channel);
}

void jamAll()
{
  if (random(0, 2))
  {
    jamBluetoothDifferentChannelsFull();
  }
  else
  {
    jamBLE();
  }
}
