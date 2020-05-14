# LedWall

Drive a Matrix of WS2812 LEDs using the ESP32 as controller.

There are a lot of Projects out there doing almost the same. The target of this Project however was to fully use the ESP32-IDF (and not some Arduino _bridges_/_ports_) and learn to use it.

See companion at https://github.com/nehlsen/LedWall-Grafitti

Features working so far
- Connect to Wifi, wifi provisioning using the espressif sample app
- Connect to MQTT Broker
  - Control _Power State_
  - Control _Mode_ 
- Provide a RESTful API
  - Control _Power State_
  - Control _Mode_
  - Read and Set various configuration parameters
  - Read and Set various led-mode options
  - Trigger OTA

Planned features / Wishlist
- Embedded WebUI
- Temp, Humidity, Atmospheric Pressure and ambient Light Sensor Data via MQTT
- More hard-programmed animations
- Manual animations or pixmaps via MQTT or REST
- Automatic brightness adjustment depending on ambient Light Sensor
- Presence detection using BT-LE

Dependencies
- https://github.com/nehlsen/FastLedIdf
- https://github.com/nehlsen/Mates-for-LEDs
- ESP-IDF v4.1-dev-1086-g93a8603c5

Used Hardware
- ESP32 (https://www.aliexpress.com/item/33040503442.html)
- LED Strip (yes, no Matrix) (https://www.aliexpress.com/item/32619222937.html)
- Power Supply (https://www.aliexpress.com/item/33037781855.html)

Credits
- Animations/Samples/Demos from https://github.com/eshkrab/fastLED-idf
- Animations/Samples/Demos from https://gist.github.com/hsiboy/f9ef711418b40e259b06
