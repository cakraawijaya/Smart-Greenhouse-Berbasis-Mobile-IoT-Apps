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
String pump; // Status pompa

// Variabel untuk keperluan sensor
int moisture = 0; // FC-28
int adcLDR = 0; const float R_FIXED = 10.0, calibrationValue = 1.2; float voltage, resistance, lux; // LDR
int temp = 0, hum = 0; // DHT

// Method untuk mengatur konektivitas
void ConnectToWiFi() {
  WiFi.mode(WIFI_STA); // Membuat perangkat sebagai station
  WiFi.begin(WIFISSID, PASSWORD); Serial.print("Menyambungkan ke jaringan"); // Memulai jaringan
  while (WiFi.status() != WL_CONNECTED) { // Selama tidak berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.print("."); delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) { // Jika berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.println("\nTelah terhubung ke "+String(WIFISSID)+"\n\n");
  }
}

// Baca Data Sensor
void bacaSensor() {
  adcLDR = analogRead(PIN_LDR); // Baca ADC Sensor LDR
  voltage = (adcLDR / 4095.0) * 5; // ESP bit=12 -> 4095, 5=Tegangan Referensi
  resistance = (5 * R_FIXED / voltage) - R_FIXED; // Menghitung Resistansi Cahaya
  
  // Mengukur nilai intensitas cahaya
  lux = 500 / pow(resistance / 1000, calibrationValue);
  if(lux >= 100000){ lux = 100000; }
  if(lux < 0){ lux = 0; }
  
  fc28.calibration(7); // 7 => agar pembacaan sensor fc28 mendekati benar (diisi bebas)
  moisture = fc28.getSoilMoisture(); // Mengukur nilai kelembaban tanah
  temp = dht.readTemperature(); // Mengukur nilai temperature udara
  hum = dht.readHumidity(); // Mengukur nilai kelembaban udara
  
  // Display data ke serial monitor
  Serial.println("=========================================");
  Serial.println("Suhu Udara: "+String(temp)+"°C");
  Serial.println("Kelembaban Udara: "+String(hum)+"%");
  Serial.println("Kelembaban Tanah: "+String(moisture)+"%");
  Serial.println("Intensitas Cahaya: "+String(lux)+"lux");
}

// Kendali Otomatis Pompa
void Threshold(){
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
  
  // Cetak status pompa ke serial monitor
  Serial.println("Status Pompa: "+String(pump)); 
  Serial.println("=========================================");
}

// Kirim Data ke Antares
void kirimAntares() {
  if ((millis() - lastTime) > timerDelay) { // Jika waktu sekarang dikurangi waktu terakhir lebih besar dari 5 detik maka :
    bacaSensor(); // Memanggil method bacaSensor
    Threshold(); // Memanggil method Threshold
    
    if (WiFi.status() == WL_CONNECTED) { // Jika tersambung ke jaringan maka :
      // Memulai request http
      http.begin(client, serverName);

      // Header http
      http.addHeader("X-M2M-Origin",ACCESSKEY);
      http.addHeader("Content-Type","application/json;ty=4");
      http.addHeader("Accept","application/json");

      // Data sensor semuanya dikirim ke server melalui protokol http
      httpRequestData += "{\"m2m:cin\": { \"con\":\"{\\\"Suhu Udara (°C)\\\":\\\"";
      httpRequestData += String(temp);
      httpRequestData += "\\\",\\\"Kelembapan Udara (%)\\\":\\\"";
      httpRequestData += String(hum);
      httpRequestData += "\\\",\\\"Kelembapan Tanah (%)\\\":\\\"";
      httpRequestData += String(moisture);
      httpRequestData += "\\\",\\\"Intensitas Cahaya (lux)\\\":\\\"";
      httpRequestData += String(lux);
      httpRequestData += "\\\",\\\"Status Pompa\\\":\\\"";
      httpRequestData += String(pump);
      httpRequestData += "\\\"}\"}}";
      // Serial.println(httpRequestData); Serial.println(); // Buka komen ini untuk debugging

      // Respon http
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("\n");

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
  ConnectToWiFi(); // Memanggil method ConnectToWiFi
}

// Method untuk mengatur perulangan
void loop() {
  kirimAntares(); // Memanggil method kirimAntares
}

// Nama Final Project : Smart Green House (Device-1: ESP32)
// Nama Peserta Edspert.id : Devan Cakra Mudra Wijaya, S.Kom.
