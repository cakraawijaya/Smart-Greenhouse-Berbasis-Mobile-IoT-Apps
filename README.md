[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?style=flat)](https://github.com/ellerbrock/open-source-badges/)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?logo=github&color=%23F7DF1E)](https://opensource.org/licenses/MIT)
![GitHub last commit](https://img.shields.io/github/last-commit/cakraawijaya/Smart-Greenhouse-Berbasis-Mobile-IoT-Apps?logo=Codeforces&logoColor=white&color=%23F7DF1E)
![Project1](https://img.shields.io/badge/Project-%2D1-ESP32-light.svg?style=flat&logo=arduino&logoColor=white&color=%23F7DF1E)
![Project2](https://img.shields.io/badge/Project-%2D2-NodeMCU-light.svg?style=flat&logo=espressif&logoColor=white&color=%23F7DF1E)
![Type](https://img.shields.io/badge/Type-Apprenticeship-light.svg?style=flat&logo=gitbook&logoColor=white&color=%23F7DF1E)

# Smart Greenhouse Berbasis Mobile IoT Apps
<strong>Edspert.id Final Project: Intensive Bootcamp Internet of Things Batch-10</strong><br><br>
Greenhouse is a building for plant cultivation that has a translucent roof and wall structure. Farmers are helped by the existence of this Greenhouse, but it still requires certain conditioning in order to achieve what is desired. This is usually done traditionally, so it is considered less effective, especially in an era full of sophisticated technology like today. In addition, theft and building break-ins are topics that need attention in addition to vegetable maintenance. This project was created to help farmers to manage their Greenhouse quickly and precisely both in close and long distances. This project has been carried out and took approximately 1 month. The system can monitor air temperature, air humidity, soil moisture, light intensity, pump status, door status, and security alarms. In addition, this system has also been provided with automatic pump control and automatic door lock features, so that this can increase efficiency in maintaining vegetables and security in the Greenhouse area. This system is based on the Internet of Things (IoT), using HTTP as the communication protocol. The system interface uses the MIT App Inventor application.

<br><br>

## Project Requirements
| Part | Description |
| --- | --- |
| Development Board | • DOIT ESP32 DEVKIT V1<br>• NodeMCU V3 ESP8266 Lolin |
| Code Editor | Arduino IDE |
| Driver | • CH340 USB Driver<br>• CP210X USB Driver |
| IoT Platform | Antares |
| Communications Protocol | • Hypertext Transfer Protocol (HTTP)<br>• Universal Asynchronous Receiver-Transmitter (UART)<br>• Serial Peripheral Interface (SPI)<br>• Inter Integrated Circuit (I2C) |
| IoT Architecture | 4 Layer |
| Application Support | MIT App Inventor |
| Programming Language | C/C++ |
| Arduino Library | • WiFi (default)<br>• HTTPClient (default)<br>• ESP8266WiFi (default)<br>• ESP8266HTTPClient (default)<br>• SPI (default)<br>• DHT_sensor_library<br>• LiquidCrystal_I2C<br>• ESP_FC28<br>• MFRC522 |
| Actuators | • Submersible pump (x1)<br>• Solenoid Door Lock (x1)<br>• Piezo buzzer (x1) |
| Sensor | • FC-28: Resistive Soil Moisture (x1)<br>• LDR: Light Dependent Resistor (x1)<br>• DHT22: Air Temperature & Humidity (x1)<br>• SW-420: Vibration (x1)<br>• RFID Reader (x1) |
| Display | LCD I2C (x1) |
| Other Components | • Micro USB cable - USB type A (x1)<br>• Jumper cable (1 set)<br>• Switching power supply 12V 3A (x1)<br>• Electromechanical relay 1-channel (x1)<br>• Step Down LM2596 Adjustable 3A DC-DC (x2)<br>• Breadboard (x2)<br>• NodeMCU expansion board (x1)<br>• Casing box (x1)<br>• RFID Card (x2)<br>• Connector male jack DC (x1)<br>• Bolts plus (1 set)<br>• Nuts (1 set) |

<br><br>

## Download & Install
1. Arduino IDE

   <table><tr><td width="810">

   ```
   https://bit.ly/ArduinoIDE_Installer
   ```

   </td></tr></table><br>

2. CH340 USB Driver

   <table><tr><td width="810">
   
   ```
   https://bit.ly/CH340_USBdriver
   ```

   </td></tr></table><br>

3. CP210X USB Driver

   <table><tr><td width="810">
   
   ```
   https://bit.ly/CP210X_USBdriver
   ```

   </td></tr></table>

<br><br>

## Project Designs
<table>
<tr>
<th width="840">Infrastructure</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Diagram/Infrastructure.jpg" alt="infrastructure"></td>
</tr>
</table>
<table>
<tr>
<th width="420">Pictorial Diagram for ESP32</th>
<th width="420">Pictorial Diagram for NodeMCU</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Diagram/Pictorial Diagram 1.jpg" alt="pd-esp32"></td>
<td><img src="Assets/Documentation/Diagram/Pictorial Diagram 2.jpg" alt="pd-nodemcu"></td>
</tr>
</table>
<table>
<tr>
<th width="840">Systems Diagram</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Diagram/Systems Diagram.jpg" alt="systems-diagram"></td>
</tr>
</table>
<table>
<tr>
<th width="840">Wiring</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Table/Device Wiring-1.jpg" alt="device-wiring1"></td>
</tr>
<tr>
<td><img src="Assets/Documentation/Table/Device Wiring-2.jpg" alt="device-wiring2"></td>
</tr>
</table>

<br><br>

## Scanning the I2C Address on the LCD
```ino
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor
  Serial.println("\nI2C Scanner");
}

void loop() {
  int nDevices = 0;
  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of the Wire.endTransmission to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000); // Wait 5 seconds for next scan
}
```

<br><br>

## Arduino IDE Setup
1. Open the ``` Arduino IDE ``` first, then open the project by clicking ``` File ``` -> ``` Open ``` :

   <table><tr><td width="810">
   
      • ``` Final_Project_Edspert_Batch10_ESP32Project.ino ```
      
      • ``` Final_Project_Edspert_Batch10_NodeMCUProject.ino ```

   </td></tr></table><br>
   
2. Fill in the ``` Additional Board Manager URLs ``` in Arduino IDE

   <table><tr><td width="810">
      
      Click ``` File ``` -> ``` Preferences ``` -> enter the ``` Boards Manager Url ``` by copying the following link :
   
      ```
      https://dl.espressif.com/dl/package_esp32_index.json
      http://arduino.esp8266.com/stable/package_esp8266com_index.json
      ```

   </td></tr></table><br>
   
3. ``` Board Setup ``` in Arduino IDE

   <table>
      <tr><th>
         
      i
         
      </th><th width="780">
            
      How to setup the ``` DOIT ESP32 DEVKIT V1 ``` board
   
      </th></tr>
      <tr><td colspan="2" width="780">

      • Click ``` Tools ``` section -> ``` Board ``` -> ``` Boards Manager ``` -> Install ``` esp32 ```.

      • Then selecting a board by clicking: ``` Tools ``` -> ``` Board ``` -> ``` ESP32 Arduino ``` -> ``` DOIT ESP32 DEVKIT V1 ```.
              
      </td></tr>
   </table><br><table>
      <tr><th>
         
      ii
         
      </th><th width="775">

      How to setup the ``` NodeMCU V3 ESP8266 Lolin ``` board
            
      </th></tr>
      <tr><td colspan="2" width="775">

      • Click ``` Tools ``` section -> ``` Board ``` -> ``` Boards Manager ``` -> Install ``` esp8266 ```.

      • Then selecting a board by clicking: ``` Tools ``` -> ``` Board ``` -> ``` ESP8266 Boards ``` -> ``` NodeMCU 1.0 (ESP-12E Module) ```.
            
      </td></tr>
   </table><br>
   
4. ``` Change the Board Speed ``` in Arduino IDE

   <table>
      <tr><th>
         
      i
         
      </th><th width="780">
            
      How to change the speed of ``` DOIT ESP32 DEVKIT V1 ``` board
   
      </th></tr>
      <tr><td colspan="2" width="780">

      Click ``` Tools ``` -> ``` Upload Speed ``` -> ``` 115200 ```
              
      </td></tr>
   </table><br><table>
      <tr><th>
         
      ii
         
      </th><th width="775">

      How to change the speed of ``` NodeMCU V3 ESP8266 Lolin ``` board
            
      </th></tr>
      <tr><td colspan="2" width="775">

      Click ``` Tools ``` -> ``` Upload Speed ``` -> ``` 115200 ```
            
      </td></tr>
   </table><br>
   
5. ``` Install Library ``` in Arduino IDE

   <table><tr><td width="810">
         
      Download all the library zip files. Then paste it in the: ``` C:\Users\Computer_Username\Documents\Arduino\libraries ```

   </td></tr></table><br>

6. ``` Port Setup ``` in Arduino IDE

   <table><tr><td width="810">
         
      Click ``` Port ``` -> Choose according to your device port ``` (you can see in device manager) ```

   </td></tr></table><br>

7. Change the ``` WiFi Name ```, ``` WiFi Password ```, and so on according to what you are currently using.<br><br>

8. Before uploading the program please click: ``` Verify ```.<br><br>

9. If there is no error in the program code, then please click: ``` Upload ```.<br><br>
    
10. Some things you need to do when using the ``` ESP32 board ``` :

    <table><tr><td width="810">
       
       • If ``` ESP32 board ``` cannot process ``` Source Code ``` totally -> Press ``` EN (RST) ``` button -> ``` Restart ```.

       • If ```ESP32 board ``` cannot process ``` Source Code ``` automatically then :<br>

      - When information: ``` Uploading... ``` has appeared -> immediately press and hold the ``` BOOT ``` button.<br>

      - When information: ``` Writing at .... (%) ``` has appeared -> release the ``` BOOT ``` button.

      • If message: ``` Done Uploading ``` has appeared -> ``` The previously entered program can already be operated ```.

      • Do not press the ``` BOOT ``` and ``` EN ``` buttons at the same time as this may switch to ``` Upload Firmware ``` mode.

    </td></tr></table><br>

11. If there is still a problem when uploading the program, then try checking the ``` driver ``` / ``` port ``` / ``` others ``` section.

<br><br>

## Antares Setup
1. Getting started with Antares :

   <table><tr><td width="810">

   • Please <a href="https://beta-console.antares.id/id/signup">Sign Up</a> first.

   • Then please <a href="https://beta-console.antares.id/id">Sign In</a> to access the service.

   </td></tr></table><br>
   
2. Activate Access Key :

   <table><tr><td width="810">
      
   • Go to ``` Account ``` menu.

   • Click ``` Get Access Key ``` to generate an access key. This process only needs to be done once.

   • If you have activated an access key before, skip this step.

   </td></tr></table><br>
   
3. Create applications :

   <table><tr><td width="810">
      
   • Go to ``` Applications ``` menu.

   • Click ``` + Create an Application ```.

   • In the ``` Add Application ``` menu, please specify the following :
      - ``` Application Name ``` -> ``` Name of the App you will create ```.
      - ``` Application ID ``` -> ``` ID of the App you will create ```.
      - ``` Labels ``` -> determine according to project needs.

   </td></tr></table><br>
   
4. Create a device :

   <table><tr><td width="810">
      
   • Make sure you are on the ``` Home / Applications / The app you created ``` menu.

   • Click ``` + Add Device ```.

   • You should specify the name of this device based on the variables in the project.

   </td></tr></table><br>
   
5. Firmware configuration :

   <table><tr><td width="810">
      
   • Make sure you are on the ``` Account ``` menu.

   • Copy ``` Access Key ``` mentioned.

   • Paste in the firmware code, for example like this :

   ```ino
   #define ACCESSKEY "1444e88d02acb758:b996115b1c2f6f0f"
   ```

   • Then, the ``` Project name ``` and ``` Device name ``` must match what was created earlier. For example :
   
   ```ino
   #define projectName "FinalProject_Edspert10"
   #define deviceName "GreenHouse_Device1"
   ```

   </td></tr></table><br><br>

## MIT App Inventor Setup
1. Open the official website ``` MIT App Inventor ``` :

   <table><tr><td width="810">
   
   ```
   https://appinventor.mit.edu/
   ```

   </td></tr></table><br>
   
2. Click ``` Create Apps! ```, then log in using google account.<br><br>

3. Click ``` Project ``` -> then import the files in the ``` Smart-Green-House-Berbasis-IoT-Mobile-Apps\Src\MIT App Inventor Project\ ``` directory :

   <table><tr><td width="810">

   • ``` Smart_Green_House_Device_1.aia ```
   
   • ``` Smart_Green_House_Device_2.aia ```

   </td></tr></table><br>

4. Click the ``` Block ``` button and set the following points :<br><br>
   • Fill the ``` initialize global url to ``` section with the following format :

   <table><tr><td width="810">
      
   ``` https://platform.antares.id:8443/~/antares-cse/antares-id/[YOUR_APPLICATION_NAME]/[YOUR_DEVICE_NAME]/la ```

   </td></tr></table><br>
   
   • Fill the ``` initialize global header to -> make a list ``` section as follows :

      <table>
      <tr><th width="810" colspan="3">
            
      ``` make a list : ```
   
      </th></tr>
      <tr><td width="270">
         
      ``` X-M2M-Origin ```.

      ``` YOUR_ACCESS_KEY ```.
   
      </td><td width="270">

      ``` Content-Type ```.

      ``` application/json;ty=4 ```.
         
      </td><td width="270">

      ``` Accept ```.

      ``` application/json ```.
   
      </td></tr>
      </table><br>

5. Then click ``` Connect ``` -> next select ``` AI Companion ```.<br><br>

6. Open your smartphone, then in the ``` Google Play Store ``` search for the ``` MIT AI2 Companion ``` application, then install it.<br><br>

7. Open the ``` MIT AI2 Companion ``` app.<br><br>

8. Select ``` Scan QR Code ``` method.<br><br>

9. Point your smartphone at the ``` QR Code ``` area on the ``` MIT App Inventor ``` site.<br><br>

10. If you want to operate 2 systems at the same time, then you can do so with 2 different devices so that the systems do not clash.

<br><br>

## Get Started
1. Download and extract this repository.<br><br>
    
2. Make sure you have the necessary electronic components.<br><br>
   
3. Make sure your components are designed according to the diagram.<br><br>
      
4. Configure your device according to the settings above.<br><br> 
 
5. Please enjoy [Done].

<br><br>

## Highlights
<table>
<tr>
<th width="420">Monitoring Device using ESP32</th>
<th width="420">Antares Platform for ESP32</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Experiment/Percobaan Device 1 ESP32.jpg" alt="product-esp32"></td>
<td><img src="Assets/Documentation/Experiment/Percobaan Device 1 Antares.jpg" alt="antaresplatform-esp32"></td>
</tr>
<tr>
<th colspan="2">MIT App Inventor Configuration for ESP32</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/MIT App Inventor ESP32.jpg" alt="mitappinventor-device1"></td>
</tr>
</table>
<table>
<tr>
<th colspan="2">Control Device using NodeMCU</th>
</tr>
<tr>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 NodeMCU 1.jpg" alt="product-nodemcu-ac1"></td>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 NodeMCU 2.jpg" alt="product-nodemcu-ac2"></td>
</tr>
<tr>
<th colspan="2">Antares Platform for NodeMCU</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/Percobaan Device 2 Antares.jpg" alt="antaresplatform-nodemcu"></td>
</tr>
<tr>
<th colspan="2">MIT App Inventor Configuration for NodeMCU</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/MIT App Inventor NodeMCU.jpg" alt="mitappinventor-device2"></td>
</tr>
</table>
<table>
<tr>
<th colspan="2">Simulation of Monitoring with Mobile Apps</th>
</tr>
<tr>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 1 MIT App Inventor.jpg" alt="simulation-1"></td>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 MIT App Inventor.jpg" alt="simulation-2"></td>
</tr>
</table>

<br><br>

## <img src="https://github.com/user-attachments/assets/932b96eb-cbc7-42f1-8938-43cb431889a5" width="16" height="16"> Notes
<blockquote>
   There is a Little Bug in the RFID Reading Part.
</blockquote>

<br><br>

## Appreciation
If this work is useful to you, then support this work as a form of appreciation to the author by clicking the ``` ⭐Star ``` button at the top of the repository.

<br><br>

## Disclaimer
This application is my own work and is not the result of plagiarism from other people's research or work, except those related to third party services which include: libraries, frameworks, and so on.

<br><br>

## LICENSE
MIT License - Copyright © 2023 - Devan C. M. Wijaya, S.Kom

Permission is hereby granted without charge to any person obtaining a copy of this software and the software-related documentation files to deal in them without restriction, including without limitation the right to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons receiving the Software to be furnished therewith on the following terms:

The above copyright notice and this permission notice must accompany all copies or substantial portions of the Software.

IN ANY EVENT, THE AUTHOR OR COPYRIGHT HOLDER HEREIN RETAINS FULL OWNERSHIP RIGHTS. THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, THEREFORE IF ANY DAMAGE, LOSS, OR OTHERWISE ARISES FROM THE USE OR OTHER DEALINGS IN THE SOFTWARE, THE AUTHOR OR COPYRIGHT HOLDER SHALL NOT BE LIABLE, AS THE USE OF THE SOFTWARE IS NOT COMPELLED AT ALL, SO THE RISK IS YOUR OWN.
