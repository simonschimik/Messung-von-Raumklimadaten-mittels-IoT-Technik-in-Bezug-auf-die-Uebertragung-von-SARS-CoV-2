# README
This repository contains the source code for the project referenced in my VWA "Messung von Raumklimadaten mittels IoT-Technik in Bezug auf die Übertragung von SARS-CoV-2". 

***Please read the following document in order to properly understand which files are supposed to be downloaded / edited if you want to recreate the project yourself.***

## Project objective
The target of this project is to use an Internet-Of-Things device, which measures and logs various indoor climate metrics, in order to provide evidence of the effectiveness of FFP2-mask mandates and mandatory ventilation in classrooms.

## Project components

 - [ESP32-DevKitC - Microcontroller](https://www.espressif.com/en/products/devkits/esp32-devkitc/overview)
 - [ST7735 1.8 Inch SPI TFT Display](https://www.az-delivery.de/products/1-8-zoll-spi-tft-display)
 - [GY-BME280 - Temperature, humidity and pressure sensor](https://www.az-delivery.de/products/gy-bme280)
 - [MH-Z19C - CO2-concentration sensor](https://www.reichelt.de/de/de/infrarot-co2-sensor-mh-z19c-pinleiste-rm-2-54-co2-mh-z19c-ph-p297320.html?r=1)
 - [novaPM SDS011 - PM10 and PM2.5 aerosol sensor ](https://www.reichelt.at/at/de/feinstaubsensor-nova-fitness-inkl-usb-adapter-sds011-p243264.html?r=1)

## Libraries used

 - [Adafruit BME280 Library](https://www.arduino.cc/reference/en/libraries/adafruit-bme280-library/)
 - [Adafruit BusIO](https://www.arduino.cc/reference/en/libraries/adafruit-busio/)
 - [Adafruit FONA Library](https://www.arduino.cc/reference/en/libraries/adafruit-fona-library/)
 - [Adafruit GFX Library](https://www.arduino.cc/reference/en/libraries/adafruit-gfx-library/)
 - [Adafruit MQTT Library](https://www.arduino.cc/reference/en/libraries/adafruit-mqtt-library/)
 - [Adafruit seesaw Library](https://www.arduino.cc/reference/en/libraries/adafruit-seesaw-library/)
 - [Adafruit SleepyDog Library](https://www.arduino.cc/reference/en/libraries/adafruit-sleepydog-library/)
 - [Adafruit ST7735 and ST7789 Library](https://www.arduino.cc/reference/en/libraries/adafruit-st7735-and-st7789-library/)
 - [Adafruit Unified Sensor](https://www.arduino.cc/reference/en/libraries/adafruit-unified-sensor/)
 - [AsyncElegantOTA](https://www.arduino.cc/reference/en/libraries/asyncelegantota/)
 - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
 - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
 - [EspSoftwareSerial](https://www.arduino.cc/reference/en/libraries/espsoftwareserial/)
 - [MH-Z19](https://www.arduino.cc/reference/en/libraries/mh-z19/)
 - [Nova Fitness Sds dust sensors library](https://www.arduino.cc/reference/en/libraries/nova-fitness-sds-dust-sensors-library/)

## How to download, edit and recreate

### What's included in the releases

 1. The needed source code files for either PlatformIO or the ArduinoIDE
 2. A Fritzing file showcasing the needed physical setup for the project
 3. **A "Reference.lnk" file, which opens the included docs, describing the entire functionality of the code**

### PlatformIO

 1. Download the corresponding project files from the [releases page](https://github.com/itssimsch/Messung-von-Raumklimadaten-mittels-IoT-Technik-in-Bezug-auf-die-Uebertragung-von-SARS-CoV-2/releases)
 2. Unzip the project files
 3. Open in PlatformIO and you should be good to go
 4. (A doxygen file which was used to generate the docs, but his has no real use)

### ArduinoIDE

 1. If not already done, **you need to install ESP32-Support in the Arduino IDE**. Please refer to this [guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) for detailed instructions
 2. Download the corresponding project files from the [releases page](https://github.com/itssimsch/Messung-von-Raumklimadaten-mittels-IoT-Technik-in-Bezug-auf-die-Uebertragung-von-SARS-CoV-2/releases)
 3. Unzip the downloaded archive - you should now have two folders: "libraries" and "Final-Arduino")
 4. Find your Arduino Sketchbook Folder in the Arduino IDE (File -> Preferences-> Sketchbook location). Open the specified path in your file explorer
 5. In said path, you should locate the Folder called "Arduino", containing the subdirectories "libraries" and "Sketchbooks" (and "tools", which we do not need to edit for this project)
 6. Move the folder "Final-Arduino" into the folder "Sketchbooks". 
 7. You can now open the "Final-Arduino.ino" file inside the "Final-Arduino" folder in the Arduino IDE
 8. Lastly, you need to install the needed libraries. There are two options for that:
		
	 - **The easiest way would** be to copy all folders from the "libraries" folder that I provided in the release (the libraries folder you extracted earlier) into the "libraries" folder in your Arduino IDE Sketchbook location.

	 - Manually download the libraries: Download all libraries from the list above using the library manager inside the Arduino IDE. **NOTE: There are two libraries that cannot be downloaded using the library manager. For "AsyncTCP" and "ESPAsyncWebServer" you need to visit the linked GitHub repositories, download the project files and extract them into the libraries folder.** 

 9. No matter which of the two methods you use, you should end up with a folder structure (inside the Arduino folder) somewhat similar to this:
```
    ├───libraries
    │   ├───Adafruit_BME280_Library
    │   ├───Adafruit_BusIO
    │   ├───Adafruit_FONA_Library
    │   ├───Adafruit_GFX_Library
    │   ├───Adafruit_MQTT_Library-2.1.0
    │   ├───Adafruit_seesaw_Library
    │   ├───Adafruit_SleepyDog_Library
    │   ├───Adafruit_ST7735_and_ST7789_Library
    │   ├───Adafruit_Unified_Sensor
    │   ├───AsyncElegantOTA
    │   ├───AsyncTCP-master
    │   ├───ESPAsyncWebServer-master
    │   ├───EspSoftwareSerial
    │   ├───MH-Z19
    │   └───Nova_Fitness_Sds_dust_sensors_library
    ├───Sketchbooks
    │   └───Final-Arduino
    └───tools

```

**If you have any problems, please create an issue at my GitHub repository!**

## Source code - Folders (only needed if you download the entire repos.)
### ESP8266-Archive
Contains archived code from an early version of the project, in which the smaller microcontroller ESP8266 was used. This project was archived because said microcontroller didn't provide enough GPIOs to support all sensors. Hence, the project was continued with the ESP32 microcontroller.

### Components

Contains short code snippets that showcase the general principle of how communication with the respective sensors works. Moreover, it contains Fritzing files that display the used wiring.

### Ver1.0

Includes the source code for the first combination of all sensors. Functionality was very limited and bugged. **This version was still developed using the Arduino IDE**

### Ver2.0
Contains all project files from the second project version. This version incorporated most of the desired functionalities and was working without major problems. There are only minor differences to the final version. **As opposed to version 1.0, the development of version 2.0 was carried out using [PlatformIO](https://platformio.org/) and Visual Studio Code.**

### Final-PIO
Contains all project files, that were used in the final setup of the project. **This folder still contains all source code in the PlatformIO format.**
 
 ### Final-Arduino
 Contains all project files, that were used in the final setup of the project. **This folder contains the project files of Final-PIO, but I converted them, so they can be used in the Arduino IDE.**

## License
MIT License

Copyright (c) 2022 Schimik Simon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
