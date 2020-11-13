# LedWall

Drive a Matrix of WS2812 LEDs using the ESP32 as controller.

There are a lot of Projects out there doing almost the same. The target of this Project however was to fully use the ESP32-IDF (and not some Arduino _bridges_/_ports_) and learn to use it.

See companion at https://github.com/nehlsen/LedWall-Grafitti and https://github.com/nehlsen/LedWallStudio

## Features working so far
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
- Minimalistic embedded WebUI
  - Change Mode (but not options, yet)
  - Display System-Info
  - Edit Config

## Planned features / Wishlist
- Temp, Humidity, Atmospheric Pressure and ambient Light Sensor Data via MQTT
- More hard-programmed animations
- Manual animations or pixmaps via MQTT or REST
- Automatic brightness adjustment depending on ambient Light Sensor
- Presence detection using BT-LE

## Dependencies
- https://github.com/nehlsen/FastLedIdf
- https://github.com/nehlsen/Mates-for-LEDs
- ESP-IDF v4.0-386-gb0f053d82 https://github.com/espressif/esp-idf/tree/release/v4.0

## Used Hardware
- ESP32 (https://www.aliexpress.com/item/33040503442.html)
- LED Strip (yes, no Matrix) (https://www.aliexpress.com/item/32619222937.html)
- Power Supply (https://www.aliexpress.com/item/33037781855.html)

## Credits
- Animations/Samples/Demos from https://github.com/eshkrab/fastLED-idf
- Animations/Samples/Demos from https://gist.github.com/hsiboy/f9ef711418b40e259b06

## Build
```shell script
# !!! init ESP32 IDF (source export.sh)
cd My_Projects_dir
git clone --recursive git@github.com:nehlsen/LedWall.git
cd LedWall
mkdir build
cd build
cmake ..
make flash
```

## Usage samples

- Trigger OTA update \
`curl -X POST <IP-OF-LEDWALL>/ota "https://url.to/update"`

## MQTT

Following Topics are **subscribed** to by the device. All Topics are subscribed to twice, once for the device topic and once for the group topic.
In case of e.g. `/cmd/power` this results in `mywall/cmd/power` and `all-my-walls/cmd/power` assuming the device topic is `mywall` and the group topic is `all-my-walls`.

| Topic              | Type         | Possible Values        |
| ------------------ | ------------ | ---------------------- |
| /cmd/power         | int          | 0: Off, 1: On          |
| /cmd/brightness    | int          | 0-255, brightness      |
| /cmd/mode/index    | int          | 0-x, mode index        |
| /cmd/mode/name     | string       | mode name              |
| /cmd/mode/options  | string/json  | mode options           |
| /cmd/preset/save   | string       | preset name            |
| /cmd/preset/load   | string       | preset name            |
| /cmd/preset/delete | string       | preset name            |
| /cmd/preset/clear  | -none-       | -none-                 |
| /cmd/reboot        | -none-       | -none-                 |

Following Topics are **published** by the device. Each Topic is published once per device-topic.

| Topic                | Type         | Possible Values        |
| -------------------- | ------------ | ---------------------- |
| /state/power         | int          | 0: Off, 1: On          |
| /state/brightness    | int          | 0-255, brightness      |
| /state/mode/index    | int          | 0-x, mode index        |
| /state/mode/name     | string       | mode name              |
| /state/mode/options  | string/json  | mode options           |
