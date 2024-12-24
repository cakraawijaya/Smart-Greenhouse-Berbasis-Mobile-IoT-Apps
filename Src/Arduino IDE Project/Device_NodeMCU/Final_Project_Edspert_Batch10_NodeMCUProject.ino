// Pustaka yang digunakan
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Koneksi
#define WIFISSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"
#define ACCESSKEY "YOUR_ANTARES_ACCESSKEY"
#define projectName "YOUR_ANTARES_APPLICATIONS_NAME"
#define deviceName "YOUR_ANTARES_DEVICE_NAME"
#define serverName "http://platform.antares.id:8080/~/antares-cse/antares-id/"+String(projectName)+"/"+String(deviceName)
WiFiClient client;
HTTPClient http;
String httpRequestData;
int httpResponseCode;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// Sensor
#define SW420_PIN D8 // Pin Antarmuka Sensor Getaran
constexpr uint8_t RST_PIN = D1; // Pin Antarmuka RST Sensor RFID
constexpr uint8_t SDA_PIN = D2; // Pin Antarmuka SDA Sensor RFID
#define UID_RFID1 "YOUR_USERID_RFID1" // User ID-1
#define UID_RFID2 "YOUR_USERID_RFID2" // User ID-2
MFRC522 rfid(SDA_PIN, RST_PIN); // Konstruktor MFRC522 -> rfid

// Aktuator
#define BUZZER_PIN D3 // Pin Antarmuka Buzzer
#define RSOLENOID_DOORLOCK_PIN D4 // Pin Antarmuka Solenoid Door Lock

// Layar
LiquidCrystal_I2C lcd(0x27,16,2);

// Variabel untuk keperluan sensor
byte UniqueIDentifier[4]; // Array untuk menampung User ID
String accessCode; // Variabel ini untuk menampung hasil pembacaan kode RFID

// Variabel untuk keperluan aktuator
int vibration; // Variabel ini untuk menampung nilai getaran
String old_alarm = "OFF", alarm_keamanan; // Variabel ini untuk status buzzer: ON/OFF
String old_doorstate = "Closed", doorstate; // Variabel ini untuk status kunci pintu: Terbuka/Tertutup
bool relayON = HIGH; bool relayOFF = LOW; // Jika anda menggunakan NO pada relay maka yang terjadi adalah Active Low, sedangkan jika anda menggunakan NC pada relay maka yang terjadi adalah Active High

// Method untuk mengatur konektivitas
void connectToWiFi() {
  WiFi.mode(WIFI_STA); // Membuat perangkat sebagai station
  WiFi.begin(WIFISSID, PASSWORD); Serial.print("Menyambungkan ke jaringan"); // Memulai jaringan
  while (WiFi.status() != WL_CONNECTED) { // Selama tidak berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.print("."); delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) { // Jika berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.println("\n=========================================");
    Serial.println("Telah terhubung ke "+String(WIFISSID));
    Serial.println("=========================================");
  }
}

// Method untuk inisialisasi sensor RFID
void initSensorRFID(){
  SPI.begin(); // Memulai komunikasi SPI
  rfid.PCD_Init(); // Memulai sensor RFID
}

// Method untuk baca sensor RFID
void readSensorRFID(){
  // Jika tidak ada kartu baru yang terdeteksi (mengembalikan false), maka keluar dari blok if
  if(!rfid.PICC_IsNewCardPresent()) { 
    return;
  }
  
  // Jika pembacaan UID gagal (mengembalikan false), maka keluar dari blok if
  if(!rfid.PICC_ReadCardSerial()) { 
    return;
  }

  // Melanjutkan verifikasi UID Tag RFID
  if (rfid.uid.uidByte[0] != UniqueIDentifier[0] || rfid.uid.uidByte[1] != UniqueIDentifier[1] || rfid.uid.uidByte[2] != UniqueIDentifier[2] || rfid.uid.uidByte[3] != UniqueIDentifier[3]) {
    // Menampung User ID ke dalam array
    for (byte i = 0; i < 4; i++) {
      UniqueIDentifier[i] = rfid.uid.uidByte[i];
      accessCode.concat(String(UniqueIDentifier[i] < 0x10 ? " 0" : " "));
      accessCode.concat(String(UniqueIDentifier[i], HEX));
    } accessCode.toUpperCase();

    // Cetak UID Tag RFID
    Serial.print(F("\nUID tag :"));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    
    if(accessCode.substring(1) == UID_RFID1){ // Jika kartu RFID dikenali oleh scanner sebagai User ID-1, maka lakukan :
      digitalWrite(RSOLENOID_DOORLOCK_PIN, relayON); delay(1000);
      if(old_doorstate != doorstate){
        doorstate = "Open"; 
        Serial.print("\nPesan RFID : "); Serial.println("Berhasil Buka Pintu Green House");
        responRFID();
        sendAntares(String(old_alarm), String(doorstate));
        old_doorstate = doorstate;
      }
    }
    else if(accessCode.substring(1) == UID_RFID2){ // Jika kartu RFID dikenali oleh scanner sebagai User ID-2, maka lakukan :
      digitalWrite(RSOLENOID_DOORLOCK_PIN, relayOFF); delay(1000);
      if(old_doorstate != doorstate){
        doorstate = "Closed";
        Serial.print("\nPesan RFID : "); Serial.println("Berhasil Tutup Pintu Green House");
        responRFID();
        sendAntares(String(old_alarm), String(doorstate));
        old_doorstate = doorstate;
      }      
    }  
    else{ // Jika kartu RFID tidak dikenali oleh scanner maka :
      digitalWrite(RSOLENOID_DOORLOCK_PIN, relayOFF); delay(1000); 
      if(old_doorstate != doorstate){
        doorstate = "Closed";
        Serial.print("\nPesan RFID : "); Serial.println("Akses Green House Gagal/UID Belum Terdaftar");
        responRFID();
        sendAntares(String(old_alarm), String(doorstate));
        old_doorstate = doorstate;
      }      
    }
  }
  else { // Jika tidak ada aktivitas maka :
    digitalWrite(RSOLENOID_DOORLOCK_PIN, relayOFF); delay(1000);
    if(old_doorstate != doorstate){
      doorstate = "Closed";
      Serial.print("\nPesan RFID : "); Serial.println("Green House Sudah Steril\n");
      responRFID();
      sendAntares(String(old_alarm), String(doorstate));
      old_doorstate = doorstate;
    }      
  }
}

// Method untuk mencetak nilai hex
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  
  // Hentikan PICC
  rfid.PICC_HaltA();
  
  // Hentikan enkripsi pada PCD
  rfid.PCD_StopCrypto1();
}

// Method untuk baca sensor SW-420
void readSensorSW420(){
  vibration = digitalRead(SW420_PIN); // Mengukur nilai getaran

  if(vibration == HIGH){ // Jika terdeteksi getaran maka:
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer menyala
    alarm_keamanan = "ON";

    // Cek perubahan status alarm keamanan saat ada getaran
    if(old_alarm != alarm_keamanan){
      Serial.println("Status buzzer = "+String(alarm_keamanan)+" - Danger"); // Status Bahaya
      sendAntares(String(alarm_keamanan), String(old_doorstate));
      old_alarm = alarm_keamanan;
    }
  } else { // Jika tidak ada getaran maka:
    digitalWrite(BUZZER_PIN, LOW); // Buzzer mati
    alarm_keamanan = "OFF"; 
    
    // Cek perubahan status alarm keamanan saat tak ada getaran
    if(old_alarm != alarm_keamanan){
      Serial.println("Status buzzer = "+String(alarm_keamanan)+" - Safe"); // Status Aman
      sendAntares(String(alarm_keamanan), String(old_doorstate));
      old_alarm = alarm_keamanan;
    }
  }
}

// Method untuk memulai LCD
void lcdInit(){
  lcd.init(); // Memulai LCD
  lcd.backlight(); delay(250); lcd.noBacklight(); delay(250); lcd.backlight(); // Splash Screen
  lcd.setCursor(0,0); lcd.print("Smart GreenHouse"); lcd.setCursor(4,1); lcd.print("Device-2");delay(10000); // Menampilkan data pada LCD
  lcd.clear(); // Menghapus tampilan data yang ada pada LCD
}

// Method untuk menampilkan status pintu ke LCD
void responRFID(){
  lcd.backlight(); lcd.setCursor(0,0); lcd.print("INFO -----------"); lcd.setCursor(0,1); lcd.print("Stt.Pintu:"+String(doorstate)); // Cetak respon RFID pada LCD
  Serial.println("Status Pintu: "+doorstate); // Cetak respon RFID pada serial monitor
}

// Kirim Data ke Antares
void sendAntares(String alarm, String pintu){
  if ((millis() - lastTime) > timerDelay) { // Jika waktu sekarang dikurangi waktu terakhir lebih besar dari 5 detik maka :
    if (WiFi.status() == WL_CONNECTED) { // Jika tersambung ke jaringan maka :
      // Memulai request http
      http.begin(client, serverName);

      // Header http
      http.addHeader("X-M2M-Origin",ACCESSKEY);
      http.addHeader("Content-Type","application/json;ty=4");
      http.addHeader("Accept","application/json");

      // Data sensor semuanya dikirim ke server melalui protokol http
      httpRequestData += "{\"m2m:cin\": { \"con\":\"{\\\"Alarm Keamanan\\\":\\\"";
      httpRequestData += String(alarm);
      httpRequestData += "\\\",\\\"Status Pintu\\\":\\\"";
      httpRequestData += String(pintu);
      httpRequestData += "\\\"}\"}}";
      // Serial.println(httpRequestData); Serial.println(); // Buka komen ini untuk debugging

      // Respon http
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println("=========================================");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("=========================================");

      // Mengakhiri request http
      http.end();
    }
    else { // Jika tidak tersambung ke jaringan maka :
      Serial.println("=========================================");
      Serial.println("WiFi Disconnected");
      Serial.println("=========================================");
    } 
    lastTime = millis(); // Untuk menghitung waktu yang telah berlalu sejak pengiriman data terakhir
  }
}

// Method untuk mengatur inisiasi awal
void setup() {
  Serial.begin(115200); // Baudrate untuk papan NodeMCU
  pinMode(SW420_PIN,INPUT); // Inisialisasi pin sw-420 sebagai INPUT
  pinMode(BUZZER_PIN,OUTPUT); // Inisialisasi pin buzzer sebagai OUTPUT
  pinMode(RSOLENOID_DOORLOCK_PIN,OUTPUT); // Inisialisasi pin solenoid door lock sebagai OUTPUT
  digitalWrite(BUZZER_PIN, LOW); // Default buzzer: OFF
  digitalWrite(RSOLENOID_DOORLOCK_PIN, HIGH); // Default solenoid door lock: OFF (Mengunci)
  initSensorRFID(); // Memanggil method initSensorRFID
  connectToWiFi(); // Memanggil method connectToWiFi
  lcdInit(); // Memanggil method lcdInit
}

// Method ini akan dikerjakan berulang kali
void loop(){
  readSensorRFID(); // Memanggil method readSensorRFID
  readSensorSW420(); // Memanggil method readSensorSW420
}

// Nama Final Project : Smart Green House (Device-2: NodeMCU)
// Nama Peserta Edspert.id : Devan Cakra Mudra Wijaya, S.Kom.
