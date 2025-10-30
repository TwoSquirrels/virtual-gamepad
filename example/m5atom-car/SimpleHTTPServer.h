// SimpleHTTPServer written by ChatGPT

#include <WiFi.h>

#define MAX_HANDLERS (16)

class SimpleHTTPServer {
  private:
    struct Handler {
      const char* path;
      void (*handler)(WiFiClient&, const String&);
    };

    const char* ssid;
    const char* password;
    WiFiServer server;
    Handler getHandlers[MAX_HANDLERS];
    Handler postHandlers[MAX_HANDLERS];
    int getHandlerCount;
    int postHandlerCount;

  public:
    SimpleHTTPServer(const char* ssid, const char* password, int port = 80)
      : ssid(ssid), password(password), server(port), getHandlerCount(0), postHandlerCount(0) {}

    void begin() {
      Serial.begin(115200);
      connectToWiFi();
      server.begin();
      Serial.println("Server started");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    }

    void handleClient() {
      WiFiClient client = server.available();
      if (client) {
        Serial.println("New Client.");
        String currentLine = "";
        String requestType = "";
        String path = "";
        String queryString = "";
        bool acceptsBrotli = false;

        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            if (c == '\n') {
              if (currentLine.length() == 0) {
                if (requestType == "GET") {
                  handleRequest(path, queryString, client, getHandlers, getHandlerCount, acceptsBrotli);
                } else if (requestType == "POST") {
                  handleRequest(path, queryString, client, postHandlers, postHandlerCount, acceptsBrotli);
                }
                break;
              } else {
                currentLine = "";
              }
            } else if (c != '\r') {
              currentLine += c;
              if (currentLine.startsWith("GET ")) {
                requestType = "GET";
                path = extractPathAndQuery(currentLine, queryString);
              } else if (currentLine.startsWith("POST ")) {
                requestType = "POST";
                path = extractPathAndQuery(currentLine, queryString);
              } else if (currentLine.indexOf("Accept-Encoding:") >= 0) {
                if (currentLine.indexOf("br") >= 0) {
                  acceptsBrotli = true;
                }
              }
            }
          }
        }
        client.stop();
        Serial.println("Client Disconnected.");
      }
    }

    void get(const char* path, void (*handler)(WiFiClient&, const String&)) {
      if (getHandlerCount < MAX_HANDLERS) {
        getHandlers[getHandlerCount++] = { path, handler };
      }
    }

    void post(const char* path, void (*handler)(WiFiClient&, const String&)) {
      if (postHandlerCount < MAX_HANDLERS) {
        postHandlers[postHandlerCount++] = { path, handler };
      }
    }

  private:
    void connectToWiFi() {
      Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("WiFi connected.");
    }

    String extractPathAndQuery(const String& requestLine, String& queryString) {
      int firstSpace = requestLine.indexOf(' ');
      int secondSpace = requestLine.indexOf(' ', firstSpace + 1);
      String fullPath = requestLine.substring(firstSpace + 1, secondSpace);

      int queryIndex = fullPath.indexOf('?');
      if (queryIndex != -1) {
        queryString = fullPath.substring(queryIndex + 1);
        return fullPath.substring(0, queryIndex);
      } else {
        queryString = "";
        return fullPath;
      }
    }

    void handleRequest(const String& path, const String& queryString, WiFiClient& client, Handler* handlers, int handlerCount, bool acceptsBrotli) {
      for (int i = 0; i < handlerCount; i++) {
        if (path == handlers[i].path) {
          handlers[i].handler(client, queryString);
          return;
        }
      }
      defaultResponse(client);
    }

    void defaultResponse(WiFiClient& client) {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-type:text/html");
      client.println();
      client.print("<html><body><h1>404 Not Found</h1></body></html>");
      client.println();
    }
};
