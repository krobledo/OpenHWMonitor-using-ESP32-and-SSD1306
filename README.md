# OpenHWMonitor-using-ESP32-and-SSD1306
a project to grab openhardwaremonitor information and load it onto the oled screen connected to my ESP32-WROOM-32

this project was made as an afforable alternative to hardware monitoring for my Windows PC

I was able to make 3 of these for 38$USD, about 13$ each with parts leftover

**********************
**In Progress**

-working on geting a 3D model made to give this a case

-web client to easily edit what is displayed
**********************
![20240601_051727466_iOS](https://github.com/krobledo/OpenHWMonitor-using-ESP32-and-SSD1306/assets/22754721/bea1cbdd-a525-43a4-b713-f6ea4371b82f)
************************************************************************************************************************
**Requirements**
*******************
**Hardware**

-ESP-WROOM-32

-SSD1306 OLED Display

-Dupont Connectors

-3D Printed Case

*********************
1. Install OpenHardwareMonitor from the following website or feel free to google it :) 
https://openhardwaremonitor.org/

2. Download [monitor.py](https://github.com/krobledo/OpenHWMonitor-using-ESP32-and-SSD1306/blob/main/monitor.py) (loads data from OpenHWMonitor)

3. Upload [esphwmonitor.ino](https://github.com/krobledo/OpenHWMonitor-using-ESP32-and-SSD1306/blob/main/esphwmonitor.ino) to ESP32 (I used ArduinoIDE to load)

4. Run monitor.py

Your ESP should now be displaying CPU/GPU Temps and GPU/Memory Loads, along with a small graph for CPU Load as shown above.
