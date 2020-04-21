#include <ESP8266WiFi.h>  // include Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <string.h> // string manipulation library

#define LED 4

const char* ssid = "Mon Calamari";
const char* password = "";

char html[2048];

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

"<body>";

const char ROOT_HTML[] = // index.html string here
"<h1>Welcome to Mon Calamari</h1>";

const char FORM_HTML[] = // index.html string here
"<h1>Ohm's Law Current Calculator</h1>"
"<form oninput=\"current.value = 1000 * parseFloat(voltage.value) / parseFloat(resistance.value)\">"

"<input type=\"number\" id=\"voltage\" value=\"9\"> Volts <br>"
"<input type=\"number\" id=\"resistance\" value=\"470\"> Ohms <br><br>"

"<b>Current=<output name=\"current\" ></output> mA</b><br><br>"
"</form>"

"<form action=\"/\" method=\"post\"><p>LED<br>"
"<input type=\"radio\" name=\"LED\" value=\"1\">On<br>"
"<input type=\"radio\" name=\"LED\" value=\"0\">Off<br>"
"<input type=\"submit\" value=\"Send\"></p>"
"</form>";

const char ERROR_HTML[] = // error.html string here
"<h1>500 - Internal Server Error</h1>";

ESP8266WebServer server(8080); // port 80 http

// This function is called whenever the root Webpage
// is required. This displays the Webpage
void handleRoot()
{
  strcat(html, ROOT_HTML);
  strcat(html, "</body></html>");
  server.send(200, "text/html", html); // 200-no error, format, html file
}

// This function is called whenever the error Webpage
// is required. This displays the Webpage
void handleError()
{
  strcat(html, ERROR_HTML);
  strcat(html, "</body></html>");
  server.send(500, "text/html", html); // 500 - internal server error, format, html file
}

bool writeLED(bool LEDvalue) 
{
  Serial.println(LEDvalue);
  switch(LEDvalue) {
    case 1: digitalWrite(LED, HIGH); handleRoot();
    case 0: digitalWrite(LED, LOW); handleRoot();
    default: handleError();
  }
}

//
void handleForm()
{
  strcat(html, FORM_HTML);
  strcat(html, "</body></html>");
  server.send(200, "text/html", html);
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
  strcpy(html, INDEX_HTML);
  server.handleClient(); // handle any client requests
}
