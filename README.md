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
| Code Editor | Arduino IDE 1.8.19 (Stable Legacy Version) |
| Driver | • CH340 USB Driver<br>• CP210X USB Driver |
| IoT Platform | Antares |
| Communications Protocol | • Hypertext Transfer Protocol (HTTP)<br>• Universal Asynchronous Receiver-Transmitter (UART)<br>• Serial Peripheral Interface (SPI)<br>• Inter Integrated Circuit (I2C) |
| IoT Architecture | 4 Layer |
| Application Support | MIT App Inventor |
| Programming Language | C/C++ |
| Arduino Library | • WiFi (default)<br>• HTTPClient (default)<br>• ESP8266WiFi (default)<br>• ESP8266HTTPClient (default)<br>• SPI (default)<br>• DHT_sensor_library by Adafruit (Version: 1.4.4)<br>• LiquidCrystal_I2C by Frank de Brabander (Version: 1.1.2)<br>• ESP_FC28 by cakraawijaya (Version: 1.0.0)<br>• MFRC522 by GithubCommunity (Version: 1.4.10) |
| Actuators | • Submersible pump (x1)<br>• Solenoid Door Lock (x1)<br>• Piezo buzzer (x1) |
| Sensor | • FC-28: Resistive Soil Moisture (x1)<br>• LDR: Light Dependent Resistor 12mm (x1)<br>• DHT22: Air Temperature & Humidity (x1)<br>• SW-420: Vibration (x1)<br>• RFID Reader (x1) |
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
<table><tr><td width="840">

```ino
/*
  =====================================================
  I2C Scanner for Arduino / ESP32 / ESP8266
  by: Devan Cakra Mudra Wijaya, S.Kom.
  =====================================================

  Functions:
  - Detects all connected I2C devices
  - Displays device addresses in HEX format
  - Displays the total number of detected devices


  =====================================================
  SDA and SCL Pins for Arduino / ESP32 / ESP8266
  =====================================================
  Arduino I2C Connection (default):
  - Arduino Uno / Nano (ATmega328P)
    SDA -> A4
    SCL -> A5

  - Arduino Mega 2560
    SDA -> D20
    SCL -> D21

  - Other Arduino boards
    SDA -> SDA pin
    SCL -> SCL pin
    (Refer to the datasheet or board pinout)

  ESP32 I2C Connection (default):
  SDA -> GPIO 21
  SCL -> GPIO 22

  ESP8266 I2C Connection (default):
  SDA -> GPIO 4 (D2)
  SCL -> GPIO 5 (D1)
*/

// Include the Wire library for I2C communication
#include <Wire.h>

// Constant that defines the delay between scans (5000 ms = 5 seconds)
const uint32_t SCAN_INTERVAL = 5000;


// Function to initialize I2C communication
// SDA and SCL pin configuration will be adjusted automatically based on the board being used
void initI2C() {

  // If the board being used is ESP32:
  #if defined(ESP32)

    // Enable I2C communication
    // SDA = GPIO21
    // SCL = GPIO22
    Wire.begin(21, 22);

  // If the board being used is ESP8266:
  #elif defined(ESP8266)

    // Enable I2C communication
    // SDA = D2 (GPIO4)
    // SCL = D1 (GPIO5)
    Wire.begin(D2, D1);

  // If the board is neither ESP32 nor ESP8266
  // Examples: Arduino Uno, Nano, Mega, Leonardo, etc.
  #else

    // Enable I2C communication using the board's built-in hardware pins
    Wire.begin();

  #endif

}


// The setup() function runs once when the board is powered on or reset
// It is used to initialize hardware, serial communication, sensors, modules, and the program's initial configuration
void setup() {

  // Start Serial communication at 115200 baud rate
  Serial.begin(115200);

  // Check whether the board uses native USB
  // Examples: Arduino Leonardo, Arduino Micro, some ESP32-S2/S3 boards
  #if defined(USBCON) || defined(ARDUINO_USB_CDC_ON_BOOT)

    // If yes:
    // The program will wait until the Serial Monitor is connected before continuing execution
    while (!Serial);

  #endif

  // Wait for 2 seconds before starting the program
  delay(2000);

  // Display program header
  Serial.println("====================================");
  Serial.println("         I2C DEVICE SCANNER         ");
  Serial.println("by: Devan Cakra Mudra Wijaya, S.Kom.");
  Serial.println("====================================");

  // Print an empty line
  Serial.println();

  // Initialize I2C communication
  initI2C();
}


// The loop() function runs continuously after setup() has finished
// The main program logic is typically placed inside this function
void loop() {

  // Variable to store the error code returned from I2C communication
  uint8_t error;

  // Variable to store the I2C address currently being checked
  uint8_t address;

  // Counter variable for the number of detected devices
  uint8_t deviceCount = 0;

  // Display information indicating that the scan process has started
  Serial.println("------------------------------------");
  Serial.println("Scanning I2C bus...");
  Serial.println("------------------------------------");

  // Loop through addresses from 1 to 126
  // Valid I2C addresses range from 0x01 to 0x7E
  for (address = 1; address < 127; address++) {

    // Start communication with the address currently being tested
    Wire.beginTransmission(address);

    // End the transmission and store the result
    // 0 = success
    // 1 = data too long
    // 2 = NACK received when address was sent
    // 3 = NACK received when data was sent
    // 4 = other error
    error = Wire.endTransmission();

    // If no error occurs:
    if (error == 0) {

      // Display information that a device was found
      Serial.print("[FOUND] Device at address 0x");

      // If the address is less than 16:
      // Add a leading zero to keep HEX formatting aligned
      if (address < 16) {
        Serial.print("0");
      }

      // Display the address in HEX format
      Serial.println(address, HEX);

      // Increment the detected device count
      deviceCount++;
    }

    // If an unknown error occurs:
    else if (error == 4) {

      // Display an error message
      Serial.print("[ERROR] Unknown error at address 0x");

      // If the address is less than 16:
      // Add a leading zero to keep HEX formatting aligned
      if (address < 16) {
        Serial.print("0");
      }

      // Display the problematic address in HEX format
      Serial.println(address, HEX);
    }

    // If the error is neither 0 nor 4:
    // Ignore it, as this usually means no device exists at that address
  }

  // Print an empty line
  Serial.println();

  // If no devices were found:
  if (deviceCount == 0) {

    // Display a message indicating that no devices were found
    Serial.println("No I2C devices found.");
  }
  else { // If at least one device was found:

    // Display the total number of detected devices
    Serial.print("Total devices found: ");

    // Display the value of deviceCount
    Serial.println(deviceCount);
  }

  // Display information about the next scan
  Serial.print("Next scan in ");

  // Convert milliseconds to seconds
  Serial.print(SCAN_INTERVAL / 1000);

  // Display the unit in seconds
  Serial.println(" seconds.");

  // Empty line
  Serial.println("\n");

  // Wait 5 seconds before performing the next scan
  delay(SCAN_INTERVAL);
}
```

</td></tr></table><br><br>

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
