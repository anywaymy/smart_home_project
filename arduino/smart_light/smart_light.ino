#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Keenetic-4678";
const char* password = "mop754lop";

ESP8266WebServer server(80);
int ledPin = D0;  // Или используйте 16 для GPIO16

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=================================");
  Serial.println("   ESP8266 Smart Light Starting");
  Serial.println("=================================");
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Начинаем с ВЫКЛюченного состояния
  
  // Быстрая проверка лампочки (3 мигания)
  Serial.println("Testing LED...");
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH); delay(300);  // ВКЛ
    digitalWrite(ledPin, LOW); delay(300);   // ВЫКЛ
  }
  
  // Подключение к WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int dots = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, !digitalRead(ledPin));  // Мигаем при подключении
    
    dots++;
    if (dots > 40) {
      Serial.println("\n❌ WiFi connection failed!");
      while(1) {
        digitalWrite(ledPin, HIGH); delay(100);
        digitalWrite(ledPin, LOW); delay(100);
        digitalWrite(ledPin, HIGH); delay(100);
        digitalWrite(ledPin, LOW); delay(500);
      }
    }
  }
  
  Serial.println("\n✅ WiFi CONNECTED!");
  Serial.print("📡 IP Address: ");
  Serial.println(WiFi.localIP());
  
  digitalWrite(ledPin, LOW);  // Гасим лампочку после подключения
  
  // ⭐⭐ ВАЖНО: ИНВЕРТИРОВАННАЯ ЛОГИКА ⭐⭐
  server.on("/light/on", []() {
    digitalWrite(ledPin, HIGH);  // ВКЛЮЧАЕМ лампочку
    server.send(200, "application/json", "{\"status\":\"on\", \"success\":true}");
    Serial.println("➡️  /light/on - LED turned ON (HIGH)");
  });
  
  server.on("/light/off", []() {
    digitalWrite(ledPin, LOW);  // ВЫКЛЮЧАЕМ лампочку
    server.send(200, "application/json", "{\"status\":\"off\", \"success\":true}");
    Serial.println("➡️  /light/off - LED turned OFF (LOW)");
  });
  
  server.on("/light/status", []() {
    // Проверяем состояние: HIGH = включено, LOW = выключено
    bool isOn = (digitalRead(ledPin) == HIGH);
    String state = isOn ? "on" : "off";
    server.send(200, "application/json", "{\"status\":\"" + state + "\"}");
    Serial.println("➡️  /light/status - State: " + state + " (pin: " + String(digitalRead(ledPin)) + ")");
  });
  
  // Главная страница
  server.on("/", []() {
    bool isOn = (digitalRead(ledPin) == HIGH);
    String html = "<html><body style='font-family:Arial;padding:20px;'>";
    html += "<h1>💡 ESP8266 Smart Light</h1>";
    html += "<p><strong>IP:</strong> " + WiFi.localIP().toString() + "</p>";
    html += "<p><strong>Status:</strong> <span style='color:" + String(isOn ? "green" : "red") + ";font-weight:bold;'>";
    html += String(isOn ? "ON 🟢" : "OFF 🔴") + "</span></p>";
    html += "<p><strong>Pin D0 value:</strong> " + String(digitalRead(ledPin)) + "</p>";
    html += "<p><a href='/light/on' style='padding:10px 20px;background:green;color:white;text-decoration:none;margin:5px;'>TURN ON</a>";
    html += "<a href='/light/off' style='padding:10px 20px;background:red;color:white;text-decoration:none;margin:5px;'>TURN OFF</a></p>";
    html += "<p><a href='/light/status' style='padding:10px 20px;background:blue;color:white;text-decoration:none;'>CHECK STATUS</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
    Serial.println("➡️  / (Home page) - LED is " + String(isOn ? "ON" : "OFF"));
  });
  
  server.begin();
  Serial.println("✅ HTTP Server STARTED");
  Serial.println("🌐 Open in browser: http://" + WiFi.localIP().toString());
  Serial.println("=================================\n");
}

void loop() {
  server.handleClient();
}