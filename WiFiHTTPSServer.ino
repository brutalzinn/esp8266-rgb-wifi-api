#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

const int led = 2;
const int redPin   = 5;        // choose the pin for each of the LEDs
const int greenPin = 4;
const int bluePin  = 14;
const boolean invert = false; // set true if common anode, false if common cathodeint color = 0; // a value from 0 to 255 representing the hue
int color = 255;
int R, G, B;  // the Red Green and Blue color components

const int freq = 5000;
const int resolution = 8;

void handleRoot();              // function prototypes for HTTP handlers
void handleLED();
void handleNotFound();


  
void setup(void){
  analogWrite(redPin, 0);
analogWrite(greenPin, 0);
analogWrite(bluePin, 0 );
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  pinMode(led, OUTPUT);

  wifiMulti.addAP("paes", "garfield laranja");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
 
  IPAddress ip(192, 168, 0, 25); 
  IPAddress gateway(192, 168, 0, 1);
 IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(8, 8, 8, 8); 
    WiFi.config(ip, gateway, subnet, dns);
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer



  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/LED", HTTP_POST, handleLED);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handleLED() {   
StaticJsonDocument<256> doc;
auto error = deserializeJson(doc, server.arg("plain"));
if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
}
serializeJson(doc, Serial);
if(doc["on"]){
hueToRGB( doc["hue"], doc["sat"]);  // call function to convert hue to RGB
// write the RGB values to the pins
analogWrite(redPin, R);
analogWrite(greenPin, G);
analogWrite(bluePin, B );
}else{
  analogWrite(redPin, 0);
analogWrite(greenPin, 0);
analogWrite(bluePin, 0 );
}
    server.send ( 200, "text/json", "{success:true}" );

  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void hueToRGB( int hue, int brightness)
{
unsigned int scaledHue = (hue * 6);
unsigned int segment = scaledHue / 256; // segment 0 to 5 around the
// color wheel
unsigned int segmentOffset =
scaledHue - (segment * 256); // position within the segment

unsigned int complement = 0;
unsigned int prev = (brightness * ( 255 - segmentOffset)) / 256;
unsigned int next = (brightness *  segmentOffset) / 256;

if(invert)
{
brightness = 255-brightness;
complement = 255;
prev = 255-prev;
next = 255-next;
}

switch(segment ) {
case 0:      // red
R = brightness;
G = next;
B = complement;
break;
case 1:     // yellow
R = prev;
G = brightness;
B = complement;
break;
case 2:     // green
R = complement;
G = brightness;
B = next;
break;
case 3:    // cyan
R = complement;
G = prev;
B = brightness;
break;
case 4:    // blue
R = next;
G = complement;
B = brightness;
break;
case 5:      // magenta
default:
R = brightness;
G = complement;
B = prev;
break;
}
}
