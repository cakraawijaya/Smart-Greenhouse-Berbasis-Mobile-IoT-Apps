// Pustaka yang digunakan
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <ESP_FC28.h>

// Koneksi
#define WIFISSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"
#define ACCESSKEY "YOUR_ANTARES_ACCESSKEY"
#define projectName "YOUR_ANTARES_APPLICATIONS_NAME"
#define deviceName "YOUR_ANTARES_DEVICE_NAME"
#define serverName "http://platform.antares.id:8080/~/antares-cse/antares-id/"+String(projectName)+"/"+String(deviceName)
WiFiClient client;
HTTPClient http;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
String httpRequestData;
int httpResponseCode;

// Sensor
#define PIN_DHT 13 // Pin Antarmuka Sensor DHT
#define DHTTYPE DHT22 // Tipe Sensor DHT -> DHT22
DHT dht(PIN_DHT, DHTTYPE); // Konstruktor DHT -> dht
#define PIN_LDR 35 // Pin Antarmuka Sensor LDR
#define PIN_MOISTURE 36 // Pin Antarmuka Sensor FC-28
#define wetSoil 60 // Nilai batas minimum untuk kondisi tanah 'basah'
#define drySoil 40 // Nilai batas maksimum untuk kondisi tanah 'kering'
FC28Sensor fc28(PIN_MOISTURE); // Konstruktor FC28Sensor -> fc28

// Aktuator
#define PIN_WATERPUMP 4 // Pin Antarmuka Pompa
bool relayON = HIGH; bool relayOFF = LOW; // Jika anda menggunakan NO pada relay maka yang terjadi adalah Active Low, sedangkan jika anda menggunakan NC pada relay maka yang terjadi adalah Active High
String old_pump = "OFF", pump; // Status pompa

// Variabel untuk keperluan sensor
int old_moisture = 0, moisture = 0; // FC-28
int adcLDR = 0, old_lux = 0, lux; const float R_FIXED = 10.0, calibrationValue = 1.2; float voltage, resistance; // LDR
int old_temp = 0, temp = 0, old_hum = 0, hum = 0; // DHT

// Method untuk mengatur konektivitas
void connectToWiFi() {
  WiFi.mode(WIFI_STA); // Membuat perangkat sebagai station
  WiFi.begin(WIFISSID, PASSWORD); Serial.print("Menyambungkan ke jaringan"); // Memulai jaringan
  while (WiFi.status() != WL_CONNECTED) { // Selama tidak berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.print("."); delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) { // Jika berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.println("\nTelah terhubung ke "+String(WIFISSID)+"\n");
  }
}

// Baca Data Sensor
void readSensor() {
  // Mengukur nilai temperatur udara
  temp = dht.readTemperature();

  // Cek perubahan nilai temperatur udara
  if(temp != old_temp){  
    Serial.println("Suhu Udara: "+String(temp)+"°C");
    sendAntares(String(temp),String(old_hum),String(old_moisture),String(old_lux),String(old_pump));
    old_temp = temp; 
  }

  // Mengukur nilai kelembaban udara
  hum = dht.readHumidity(); 

  // Cek perubahan nilai kelembaban udara
  if(hum != old_hum){ 
    Serial.println("Kelembaban Udara: "+String(hum)+"%");  
    sendAntares(String(old_temp),String(hum),String(old_moisture),String(old_lux),String(old_pump));
    old_hum = hum; 
  }

  // Mengukur nilai kelembaban tanah
  fc28.calibration(7); // 7 => agar pembacaan sensor fc28 mendekati benar (diisi bebas)
  moisture = fc28.getSoilMoisture(); 

  // Cek perubahan nilai kelembaban tanah
  if(moisture != old_moisture){ 
    Serial.println("Kelembaban Tanah: "+String(moisture)+"%");
    sendAntares(String(old_temp),String(old_hum),String(moisture),String(old_lux),String(old_pump));
    old_moisture = moisture; 
  }

  // Mengukur nilai intensitas cahaya
  adcLDR = analogRead(PIN_LDR); // Baca ADC Sensor LDR
  voltage = (adcLDR / 4095.0) * 5; // ESP bit=12 -> 4095, 5=Tegangan Referensi
  resistance = (5 * R_FIXED / voltage) - R_FIXED; // Menghitung Resistansi Cahaya

  // Menghitung nilai lux
  lux = 500 / pow(resistance / 1000, calibrationValue);
  if(lux >= 100000){ lux = 100000; }
  if(lux < 0){ lux = 0; }

  // Cek perubahan nilai lux
  if(lux != old_lux){ 
    Serial.println("Intensitas Cahaya: "+String(lux)+"lux");
    sendAntares(String(old_temp),String(old_hum),String(old_moisture),String(lux),String(old_pump));
    old_lux = lux; 
  }
}

// Kendali Otomatis Pompa
void waterPumpControl(){
  // Jika suhu udara rendah / kelembaban tinggi / intensitas cahaya rendah / tanah basah, maka : 
  if (temp >= 0 && temp < 16 || hum > 90 && hum <= 100 || lux >= 0 && lux < 200 || moisture >= wetSoil) {
    pump = "OFF"; // status pompa: OFF
    digitalWrite(PIN_WATERPUMP, relayOFF); // Pompa air mati
  }

  // Jika suhu udara sedang / kelembaban sedang / intensitas cahaya sedang / tanah lembab, maka :
  if (temp >= 16 && temp <= 34 || hum >= 30 && hum <= 90 || lux >= 200 && lux < 500 || moisture > drySoil && moisture < wetSoil) { 
    pump = "OFF"; // status pompa: OFF
    digitalWrite(PIN_WATERPUMP, relayOFF); // Pompa air mati
  }

  // Jika suhu udara tinggi / kelembaban rendah / intensitas cahaya tinggi / tanah kering, maka :
  if (temp > 34 && temp <= 80 || hum >= 0 && hum < 30 || lux >= 500 && lux <= 100000 || moisture <= drySoil) { 
    pump = "ON"; // status pompa: ON
    digitalWrite(PIN_WATERPUMP, relayON); // Pompa air menyala
  }
  
  // Cek perubahan status pompa
  if(old_pump != pump){
    Serial.println("Status Pompa: "+String(pump)); 
    sendAntares(String(old_temp),String(old_hum),String(old_moisture),String(old_lux),String(pump));
    old_pump = pump;
  }
}

// Kirim Data ke Antares
void sendAntares(String suhu_udara, String kelembaban_udara, String kelembaban_tanah, String cahaya, String pompa) {
  if ((millis() - lastTime) > timerDelay) { // Jika waktu sekarang dikurangi waktu terakhir lebih besar dari 5 detik maka :
    if (WiFi.status() == WL_CONNECTED) { // Jika tersambung ke jaringan maka :
      // Memulai request http
      http.begin(client, serverName);

      // Header http
      http.addHeader("X-M2M-Origin",ACCESSKEY);
      http.addHeader("Content-Type","application/json;ty=4");
      http.addHeader("Accept","application/json");

      // Data sensor semuanya dikirim ke server melalui protokol http
      httpRequestData += "{\"m2m:cin\": { \"con\":\"{\\\"Suhu Udara (°C)\\\":\\\"";
      httpRequestData += String(suhu_udara);
      httpRequestData += "\\\",\\\"Kelembapan Udara (%)\\\":\\\"";
      httpRequestData += String(kelembaban_udara);
      httpRequestData += "\\\",\\\"Kelembapan Tanah (%)\\\":\\\"";
      httpRequestData += String(kelembaban_tanah);
      httpRequestData += "\\\",\\\"Intensitas Cahaya (lux)\\\":\\\"";
      httpRequestData += String(cahaya);
      httpRequestData += "\\\",\\\"Status Pompa\\\":\\\"";
      httpRequestData += String(pompa);
      httpRequestData += "\\\"}\"}}";
      // Serial.println(httpRequestData); Serial.println(); // Buka komen ini untuk debugging

      // Respon http
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Mengakhiri request http
      http.end();
    }
    else { // Jika tidak tersambung ke jaringan maka :
      Serial.println("WiFi Disconnected");
    } 
    lastTime = millis(); // Untuk menghitung waktu yang telah berlalu sejak pengiriman data terakhir
  }
}

// Method untuk mengatur inisiasi awal
void setup() {
  Serial.begin(115200); // Baudrate untuk papan ESP
  fc28.begin(); // Memulai sensor fc-28
  dht.begin(); // Memulai sensor dht
  pinMode(PIN_LDR, INPUT); // LDR sebagai input
  pinMode(PIN_WATERPUMP, OUTPUT); // Pompa sebagai output
  connectToWiFi(); // Memanggil method connectToWiFi
}

// Method untuk mengatur perulangan
void loop() {
  readSensor(); // Memanggil method readSensor
  waterPumpControl(); // Memanggil method waterPumpControl
}

// Nama Final Project : Smart Green House (Device-1: ESP32)
// Nama Peserta Edspert.id : Devan Cakra Mudra Wijaya, S.Kom.
