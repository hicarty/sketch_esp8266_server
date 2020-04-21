#include <ESP8266WiFi.h>  // include Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define LED 4

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

const char ERROR_HTML[] = // error.html string here
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
"<h1>500 - Internal Server Error</h1>"
"</body>"
"</html>";

ESP8266WebServer server(8080); // port 80 http

// This function is called whenever the root Webpage
// is required. This displays the Webpage
void handleRoot()
{
  server.send(200, "text/html", INDEX_HTML); // 200-no error, format, html file
}

// This function is called whenever the error Webpage
// is required. This displays the Webpage
void handleError()
{
  server.send(500, "text/html", ERROR_HTML); // 500 - internal server error, format, html file
}

bool writeLED(bool LEDvalue) 
{
  switch(LEDvalue) {
    case 1: digitalWrite(LED, HIGH); handleRoot();
    case 0: digitalWrite(LED, LOW); handleRoot();
    default: handleError();
  }
}

//
void handleForm()
{
  server.send(200, "text/html", INDEX_HTML);
  String LEDvalue;

  if (!server.hasArg("LED")) return;
  LEDvalue = server.arg("LED");
  if (LEDvalue == "1") {
    writeLED(true); // illuminate LED then send HTML response
  }
  else if (LEDvalue == "0") {
    writeLED(false); // turn off LED then send HTML response
  }
  else {
    return; // send HTML error response
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  WiFi.softAP(ssid, password); // setup access point
  Serial.println("");

  Serial.println(""); Serial.print("Connect to ");
  Serial.println(ssid); Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot); // start session at root, call function handleRoot
  server.on("/form", handleForm);
  server.begin();

  Serial.print("Connected to http://");
  Serial.println(WiFi.softAPIP());
}

void loop(void) {
  server.handleClient(); // handle any client requests
}
