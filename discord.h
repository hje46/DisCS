//discord_send code by Madnerd - https://create.arduino.cc/editor/madnerd/429197a3-8a40-4d2c-bc16-1a502cb26cd9/preview
//ArduinoHttpClient library by Arduino-Libraries forked from amcwen https://github.com/arduino-libraries/ArduinoHttpClient
//ArduinoHttpServer library by QuickSander https://github.com/QuickSander/ArduinoHttpServer

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"


const char DCserver[] = "discordapp.com";
const int port = 443;
const String discord_webhook = SECRET_WEBHOOK;
const String discord_tts = SECRET_TTS;

WiFiSSLClient client;
HttpClient http_client = HttpClient(client, DCserver, port);

void discord_send(String content) {
  Serial.println("[HTTP] Connecting to Discord...");
  Serial.println("[HTTP] Message: " + content);
  Serial.println("[HTTP] TTS: " + discord_tts);
  http_client.post(discord_webhook, "application/json", "{\"content\":\"" + content + "\", \"tts\":" + discord_tts + "}");
  // read the status code and body of the response
  int statusCode = http_client.responseStatusCode();
  String response = http_client.responseBody();

  Serial.print("[HTTP] Status code: ");
  Serial.println(statusCode);
  Serial.print("[HTTP] Response: ");
  Serial.println(response);
}
