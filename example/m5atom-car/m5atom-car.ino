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

#if __has_include(".env.h")
#  include ".env.h"
#else
const char ssid[] = "YOUR_SSID";
const char password[] = "YOUR_PASSWORD";
#endif

#if __has_include("index-html.h")
#  include "index-html.h"
#else
const char index_html[] = "<html><body><h1>There is not index-html.h</h1></body></html>";
#endif

void updateMoters(double angle, double force);

SimpleHTTPServer server(ssid, password);

void handleRoot(WiFiClient& client, const String& query) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(index_html);
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
  updateMoters(angle * 360.0 / 256.0, force / 255.0);
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

double moterRight = 0.0, moterLeft = 0.0;

unsigned long prevTime = millis();

void loop() {
  const unsigned long deltaTime = millis() - prevTime;
  prevTime = millis();

  if (moterRight >= 0.0) moterRight = max(moterRight - deltaTime / 1000.0, 0.0);
  else moterRight = min(moterRight + deltaTime / 50.0, 0.0);
  if (moterLeft >= 0.0) moterLeft = max(moterLeft - deltaTime / 1000.0, 0.0);
  else moterLeft = min(moterLeft + deltaTime / 50.0, 0.0);

  server.handleClient();

  ledcWrite(PWM_CH_A, (uint16_t)(sqrt(max(0.0, moterRight)) * 255));
  ledcWrite(PWM_CH_B, (uint16_t)(sqrt(max(0.0, moterLeft)) * 255));

  M5.update();
}

void updateMoters(double angle, double force) {
  if (angle < 15 || angle >= 345) {
    moterRight = -0.5;
    moterLeft = 0.5;
  } else if (angle >= 165 && angle < 195) {
    moterRight = 0.5;
    moterLeft = -0.5;
  } else if (angle < 90) {
    moterRight = constrain(angle / 40 - 0.875, -0.5, 1.0);
    moterLeft = constrain(angle / 40 + 0.125, 0.5, 1.0);
  } else if (angle < 180) {
    moterRight = constrain(-angle / 40 + 4.625, 0.5, 1.0);
    moterLeft = constrain(-angle / 40 + 3.625, -0.5, 1.0);
  } else if (angle < 270) {
    //moterRight = constrain(-angle / 40 + 4.375, -1.0, -0.5);
    //moterLeft = constrain(-angle / 40 + 5.375, -1.0, 0.5);
    moterRight = angle < 255 ? 0.5 : -0.5;
    moterLeft = -0.5;
  } else {
    //moterRight = constrain(angle / 40 - 8.125, -1.0, 0.5);
    //moterLeft = constrain(angle / 40 - 9.125, -1.0, -0.5);
    moterRight = -0.5;
    moterLeft = angle < 285 ? -0.5 : 0.5;
  }

  moterRight *= force;
  moterLeft *= force;
}
