// ##############################################################################################
//PROJECT TITLE : IOT WEATHER STATION 
//BOARD USED : ESP32 DEV BOARD AND NEO 6M MODULE
//SENSOR : DHT11
// ##############################################################################################

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Adafruit_Sensor.h"
#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//the pins of the esp32 that will be connected to the gps module
static const int RXPin = 18, TXPin = 19;
static const uint32_t GPSBaud = 9600;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
//getting the gps instance
TinyGPSPlus gps;

// Import required libraries


// Replace with your network credentials
const char* ssid = "GIONEE GN5005L";
const char* password = "00000000";

 #define DHTPIN 23     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

String gpshour(){
  if(gps.time.isValid()){
    int hour1 = gps.time.hour() + 1;
    if(hour1 < 10){
      int hour2 = 0 + hour1;
      return String(hour2);
    }
    else{
      return String(hour1);
    }
  }
  else{
    return "00";
  }
}

String gpsminute(){
  if(gps.time.isValid()){
    int minute1 = gps.time.minute();
    if(minute1 < 10){
      int minute2 = 0 + minute1;
      return String(minute2);
    }
    else{
      return String(minute1);
    }
  }
  else{
    return "00";
  }
}

String gpssecond(){
  if(gps.time.isValid()){
    int second1 = gps.time.second();
    if(second1 < 10){
      int second2 = 0 + second1;
      return String(second2);
    }
    else{
      return String(second1);
    }
  }
  else{
    return "00";
  }
}

String gpstime(){
  String hour1, minute1, second1;
  hour1 = gpshour();
  minute1 = gpsminute();
  second1 = gpssecond();

  String time1 = hour1 + ":" + minute1 + ":" + second1;
  Serial.println(time1);
  return time1;
}


 const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
      background-color: blue;
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>IOT WEATHER STATION PROJECT</h2>
  <h4>PROJECT INVOLVES GETTING THE TEMPERATURE, HUMIDITY, TIME AND LOCATION OF A PLACE</h4>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    
    <span class="dht-labels">Time</span>
    <span id="time">%TIME%</span>
    <sup class="units"></sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("time").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/time", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  String gpstime1 = gpstime();
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "TIME"){
    //to ensure signal is moving to and fro the devices
     while (ss.available() > 0)
      if (gps.encode(ss.read()))
        
        Serial.print(gpstime1);
        return gpstime1;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  ss.begin(GPSBaud);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
 
}
