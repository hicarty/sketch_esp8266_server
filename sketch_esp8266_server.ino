#include <ESP8266WiFi.h>  // include Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Mon Calamari";
const char* password = "";

const char INDEX_HTML[] = // index.html string here
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Mon Calamari</title>"
"<style>"
"  body {"
"    width: 100vw;"
"    margin: 50px auto;"
"    font-family: Tahoma, Verdana, Arial, sans-serif;"
"    text-align: center;"
"  }"
"</style>"
"</head>"

"<body>"
"<h1>Welcome to Mon Calamari</h1>"

"<form oninput=\"current.value = 1000 * parseFloat(voltage.value) / parseFloat(resistance.value)\">"

"<input type=\"number\" id=\"voltage\" value=\"9\"> Volts <br>"
"<input type=\"number\" id=\"resistance\" value=\"470\"> Ohms <br><br>"

"<b>Current=<output name=\"current\" ></output> mA</b><br><br>"
"</form>"

"<form action=\"/\" method=\"post\"><p>LED<br>"
"<input type=\"radio\" name=\"LED\" value=\"1\">On<br>"
"<input type=\"radio\" name=\"LED\" value=\"0\">Off<br>"
"<input type=\"submit\" value=\"Send\"></p>"
"</form>"
"</body>"
"</html>";

ESP8266WebServer server(8080); // port 80 http

// This function is called whenever the root Webpage
// is required. This displays the Webpage
void handleRoot()
{
  server.send(200, "text/html", INDEX_HTML); // 200-no error, format, html file
}

void setup(void) {
  Serial.begin(115200);
  WiFi.softAP(ssid, password); // setup access point
  Serial.println("");

  Serial.println(""); Serial.print("Connect to ");
  Serial.println(ssid); Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot); // start session at root, call function handleRoot
  server.begin();

  Serial.print("Connected to http://");
  Serial.println(WiFi.softAPIP());
}

void loop(void) {
  server.handleClient(); // handle any client requests
}
