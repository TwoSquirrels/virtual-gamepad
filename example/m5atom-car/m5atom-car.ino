// WiFi Control Car Example

#include "SimpleHTTPServer.h"
#include <M5Atom.h>

#define LEDright (22)
#define LEDleft (19)
#define MotorA (23)
#define MotorB (33)

const double PWM_freq = 2000; // PWM 周波数.
const uint8_t PWM_res = 8;    // PWM 分解能 16bit(0～256).
const uint8_t PWM_CH_A = 1;   // チャンネル.
const uint8_t PWM_CH_B = 2;   // チャンネル.

#if __has_include("env.h")
#  include "env.h"
#else
const char ssid[] = "YOUR_SSID";
const char password[] = "YOUR_PASSWORD";
#endif

#if __has_include("index-html.h")
#  include "index-html.h"
#else
// Brotli-compressed: "<html><body><h1>There is not index-html.h</h1></body></html>"
const unsigned char index_html_br[] = {
  0x1f, 0x3c, 0x00, 0x10, 0x1c, 0x09, 0x36, 0x4e, 0x72, 0xef, 0xe5, 0xce,
  0x46, 0xa8, 0xf4, 0x44, 0xe3, 0x51, 0x80, 0x0d, 0x38, 0xd2, 0x40, 0x1e,
  0x30, 0xa4, 0xaf, 0x18, 0x3a, 0xd8, 0x80, 0x03, 0xf7, 0x34, 0xd2, 0x80,
  0x26, 0x3a, 0x3a, 0xba, 0x84, 0x05, 0x87, 0x47, 0xf6, 0x33, 0xe9, 0xc7,
  0x47, 0x83, 0x09, 0x56, 0xde, 0x90, 0x08
};
const size_t index_html_br_len = sizeof(index_html_br);
#endif

void updateMotors(double angle, double force);

SimpleHTTPServer server(ssid, password);

// HTTP chunk size for streaming large responses
// 512 bytes is appropriate for WiFi tethering scenarios with M5Atom
#define HTTP_CHUNK_SIZE 512

void handleRoot(WiFiClient& client, const String& query) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Content-Encoding: br");
  client.print("Content-Length: ");
  client.println(index_html_br_len);
  client.println();
  
  // Send Brotli-compressed data in chunks
  for (size_t i = 0; i < index_html_br_len; i += HTTP_CHUNK_SIZE) {
    size_t len = min((size_t)HTTP_CHUNK_SIZE, index_html_br_len - i);
    client.write(&index_html_br[i], len);
  }
}

void respondOk(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println();
  client.println("OK");
}

void respondBad(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println();
  client.println("OK");
}

void handlePostJoystick(WiFiClient& client, const String& query) {
  if (query.length() != 6) {
    respondBad(client);
    return;
  }
  const int angle = strtol(query.substring(2, 4).c_str(), NULL, 16);
  const int force = strtol(query.substring(4, 6).c_str(), NULL, 16);
  updateMotors(angle * 360.0 / 256.0, force / 255.0);
  Serial.print("======== Joystick (angle=");
  Serial.print(angle);
  Serial.print(", force=");
  Serial.print(force);
  Serial.println(") ========");
  respondOk(client);
}

void handlePostA(WiFiClient& client, const String& query) {
  M5.dis.drawpix(0, 0xFF0000);
  Serial.println("======== Pushed A button! ========");
  respondOk(client);
}

void handlePostB(WiFiClient& client, const String& query) {
  M5.dis.drawpix(0, 0x00FF00);
  Serial.println("======== Pushed B button! ========");
  respondOk(client);
}

void setup() {
  M5.begin(false, false, true);
  Serial.begin(115200);
  M5.dis.drawpix(0, 0x0000FF);
  pinMode(LEDright, OUTPUT);
  pinMode(LEDleft, OUTPUT);
  pinMode(MotorA, OUTPUT);
  pinMode(MotorB, OUTPUT);
  // チャンネル1と周波数の分解能を設定.
  ledcSetup(PWM_CH_A, PWM_freq, PWM_res);
  // チャンネル2と周波数の分解能を設定.
  ledcSetup(PWM_CH_B, PWM_freq, PWM_res);

  // LED とモータのピンとチャンネルの設定.
  ledcAttachPin(LEDright, PWM_CH_A);
  ledcAttachPin(LEDleft, PWM_CH_B);
  ledcAttachPin(MotorA, PWM_CH_A);
  ledcAttachPin(MotorB, PWM_CH_B);

  delay(10);

  server.begin();

  server.get("/", handleRoot);
  server.post("/joystick", handlePostJoystick);
  server.post("/a", handlePostA);
  server.post("/b", handlePostB);
}

double motorRight = 0.0, motorLeft = 0.0;

unsigned long prevTime = millis();

void loop() {
  const unsigned long deltaTime = millis() - prevTime;
  prevTime = millis();

  if (motorRight >= 0.0) motorRight = max(motorRight - deltaTime / 1000.0, 0.0);
  else motorRight = min(motorRight + deltaTime / 50.0, 0.0);
  if (motorLeft >= 0.0) motorLeft = max(motorLeft - deltaTime / 1000.0, 0.0);
  else motorLeft = min(motorLeft + deltaTime / 50.0, 0.0);

  server.handleClient();

  ledcWrite(PWM_CH_A, (uint16_t)(sqrt(max(0.0, motorRight)) * 255));
  ledcWrite(PWM_CH_B, (uint16_t)(sqrt(max(0.0, motorLeft)) * 255));

  M5.update();
}

void updateMotors(double angle, double force) {
  if (angle < 15 || angle >= 345) {
    motorRight = -0.5;
    motorLeft = 0.5;
  } else if (angle >= 165 && angle < 195) {
    motorRight = 0.5;
    motorLeft = -0.5;
  } else if (angle < 90) {
    motorRight = constrain(angle / 40 - 0.875, -0.5, 1.0);
    motorLeft = constrain(angle / 40 + 0.125, 0.5, 1.0);
  } else if (angle < 180) {
    motorRight = constrain(-angle / 40 + 4.625, 0.5, 1.0);
    motorLeft = constrain(-angle / 40 + 3.625, -0.5, 1.0);
  } else if (angle < 270) {
    //motorRight = constrain(-angle / 40 + 4.375, -1.0, -0.5);
    //motorLeft = constrain(-angle / 40 + 5.375, -1.0, 0.5);
    motorRight = angle < 255 ? 0.5 : -0.5;
    motorLeft = -0.5;
  } else {
    //motorRight = constrain(angle / 40 - 8.125, -1.0, 0.5);
    //motorLeft = constrain(angle / 40 - 9.125, -1.0, -0.5);
    motorRight = -0.5;
    motorLeft = angle < 285 ? -0.5 : 0.5;
  }

  motorRight *= force;
  motorLeft *= force;
}
