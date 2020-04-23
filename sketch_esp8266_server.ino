#include <ESP8266WiFi.h>  // include Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <string.h> // string manipulation library

#define LED 2 // GPIO2 = D4
#define Port  8080

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

"<form action=\"/form\" method=\"post\"><p>LED<br>"
"<input type=\"radio\" name=\"LED\" value=\"1\">On<br>"
"<input type=\"radio\" name=\"LED\" value=\"0\">Off<br>"
"<input type=\"submit\" value=\"Send\"></p>"
"</form>";

const char ERROR_HTML[] = // error.html string here
"<h1>500 - Internal Server Error</h1>";

ESP8266WebServer server(Port); // port 80 http

void buildDocument(const char* HTML, int status_code)
{
  strcpy(html, INDEX_HTML);
  strcat(html, HTML);
  strcat(html, "</body></html>");
  server.send(status_code, "text/html", html); 
}

// This function is called whenever the root Webpage
// is required. This displays the Webpage
void handleRoot()
{
  buildDocument(ROOT_HTML, 200); // 200-no error, format, html file
}

// This function is called whenever the error Webpage
// is required. This displays the Webpage
void handleError()
{
  buildDocument(ERROR_HTML, 500); // 500 - internal server error, format, html file
}

//
void handleForm()
{
  String LEDvalue;
  buildDocument(FORM_HTML, 200);

  if (!server.hasArg("LED")) return;
  LEDvalue = server.arg("LED");
  
  if (LEDvalue == "0") {
    digitalWrite(LED, LOW); handleForm(); // illuminate LED then send HTML response
  }
  else if (LEDvalue == "1") {
    digitalWrite(LED, HIGH); handleForm(); // turn off LED then send HTML response
  }
  else {
    handleError();
    return; // send HTML error response
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  WiFi.softAP(ssid, password); // setup access point
  
  Serial.println(""); Serial.println(""); 
  Serial.print("Connect to "); Serial.println(ssid); 
  Serial.print("IP address: "); Serial.println(WiFi.softAPIP());
  Serial.print("Port: ");Serial.println(Port);

  server.on("/", handleRoot); // start session at root, call function handleRoot
  server.on("/form", handleForm);
  server.begin();

  Serial.print("Connected to http://");
  Serial.println(WiFi.softAPIP());
}

void loop(void) {
  server.handleClient(); // handle any client requests
}
