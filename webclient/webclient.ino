// Load Wi-Fi library
// Library link in README
#include <ESP8266WiFi.h>

// Replace with your network credentials
// Set up your own network hotspot
const char* ssid     = "air_quality";
const char* password = "clean_air";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign variables that will be printed
int sensorValue;
int digitalValue;
// int airCount = 0;

// Pins
int first_input = 5;
int second_input = 6;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(first_input, INPUT);
  pinMode(second_input, INPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            sensorValue = analogRead(first_input);
            digitalValue = analogRead(second_input);

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            // Button css (not necessary right now)
            // client.println(".button2 {background-color: #77878A;}</style></head>");
            // client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            
            // Web Page Heading
            client.println("<body><h1>D1 Mini Air Quality Web Server</h1>");
            
            // Display current state
            client.println("<p>Sensor value: " + String(sensorValue) + "</p>");
            client.println("<p>Digital value: " + String(digitalValue) + "</p>"); 
            if (sensorValue < 200) {
              client.println("<p>Air Quality: Good</p>");
            }
            if (sensorValue < 250) {
              client.println("<p>Air Quality: Good</p>");
            }
            if (sensorValue < 275) {
              client.println("<p>Air Quality: Poor</p>");
            }
            if (sensorValue < 1000) {
              client.println("<p>Air Quality: Bad</p>");
            }
            client.println("<a href='https://www.parthiv.dev/'>Website developed by Parthiv</a>")
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}