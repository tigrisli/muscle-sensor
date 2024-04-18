#include <WiFi.h>

// Network credentials ssid and password
const char* ssid = "yourSSID";
const char* password = "yourPassword";

// Server details
const char* host = "192.168.0.25"; Raspberry pi IP
const int port = 5000; // Raspberry pi port number is fixed to 5000 so no need to change

// WiFi client
WiFiClient client;

// Variables for EMA calculation
float alpha = 0.5;
float ema_ms[5] = {0, 0, 0, 0, 0};
int plot_ms[5] = {0, 0, 0, 0, 0};
int thresholds[5] = {1000, 3700, 90, 1000, 1000}; //threshold can be changed sequentially for {1000, 3700, 90, 1000, 1000} [A0,A1,A2,A3,A4]

void setup() {
  Serial.begin(9600);
  
  // Connect to WiFi network
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to the TCP server
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    while (1);
  }
  Serial.println("Connected to server");
}

void loop() {
  String dataToSend = "";
  for (int i = 0; i < 5; i++) {
    int raw_value = analogRead(A0 + i);
    ema_ms[i] = alpha * raw_value + (1 - alpha) * ema_ms[i];
    if (ema_ms[i] < thresholds[i]) {
      plot_ms[i] = int(ema_ms[i]) + thresholds[i];
    } else {
      plot_ms[i] = thresholds[i];
    }
    dataToSend += String(plot_ms[i]);
    if (i < 4) dataToSend += ",";
  }

  // Send the data to the server
  client.println(dataToSend);

  // Check if the server has replied
  if (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  // If the server's disconnected, reconnect
  if (!client.connected()) {
    Serial.println("Server disconnected");
    client.stop();
    while (!client.connect(host, port)) {
      Serial.println("Attempting to reconnect to server...");
      delay(5000);
    }
    Serial.println("Reconnected to server");
  }
  
  delay(100); // Wait a bit to prevent saturating the serial output
}
