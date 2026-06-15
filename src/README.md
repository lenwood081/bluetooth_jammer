# Bluetooth jammer

! This is purely for educational purposes, and is often illegal

## Project specifications

Esp32U (other esp32 will work) with two NRF24Lo1 modules (plus antenna). Must be powered by usb as the current hardware uses two hw-200 5v-3.3v converter chips,
and the 5v out is only 5v when powered by usb. can work without the converters just wire 3.3v directly with a 10uf capacitor between gnd and vout to smooth voltage.
