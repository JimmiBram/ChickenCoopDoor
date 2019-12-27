//GLOBALS
WiFiServer server(80);
String header;
const char* ssid       = "xxx";
const char* password   = "xxx";
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
string


// Timeout variables
unsigned long w_currentTime = millis();
unsigned long w_previousTime = 0; 
const long w_timeoutTime = 2000;

void SetupWifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Log(".");
  }
  Log("Wifi connected - IP:");
  Serial.print(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  server.begin();
  }


void UpdateTime(){
    if(wholeSecond){
      timeClient.update();
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      Serial.print(", ");
      Serial.print(timeClient.getHours());
      Serial.print(":");
      Serial.print(timeClient.getMinutes());
      Serial.print(":");
      Serial.println(timeClient.getSeconds());
      //Serial.println(timeClient.getFormattedTime());
    }
  }

void HandleWifi(){
  WiFiClient client = server.available();   // Listen for incoming clients
   if (client) {
        Log("Client Connected");
        String currentLine = "";
        w_currentTime = millis();
        w_previousTime = w_currentTime;
            while (client.connected() && w_currentTime - w_previousTime <= w_timeoutTime) { // loop while the client's 
                w_currentTime = millis();
                    if (client.available()) {
                                char c = client.read();             // read a byte, then
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
                                    
                                    // turns the GPIOs on and off
                                    if (header.indexOf("GET /door/open") >= 0) {
                                      Serial.println("Opening door");
                                      //output5State = "on";
                                      OpenDoor();
                                    } else if (header.indexOf("GET /door/close") >= 0) {
                                      Serial.println("Closing door");
                                      //output5State = "off";
                                      CloseDoor();
                                    }
                                    
                                    // Display the HTML web page
                                    client.println("<!DOCTYPE html><html>");
                                    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                                    client.println("<link rel=\"icon\" href=\"data:,\">");
                                    // CSS to style the on/off buttons 
                                    // Feel free to change the background-color and font-size attributes to fit your preferences
                                    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                                    client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                                    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                                    client.println(".button2 {background-color: #77878A;}</style></head>");
                                    
                                    // Web Page Heading
                                    client.println("<body><h1>Chickendoor System</h1>");
                                    
                                    // Display current state, and ON/OFF buttons for GPIO 5  
                                    client.println("<p>Time is 20:00</p>");
                                    client.println("<p>Door is " + doorState + "</p>");
                                    
                                    // If the output5State is off, it displays the ON button       
                                    if (doorState=="closed") {
                                      client.println("<p><a href=\"/5/open\"><button class=\"button\">Open door</button></a></p>");
                                    } else {
                                      client.println("<p><a href=\"/5/close\"><button class=\"button button2\">Close door</button></a></p>");
                                    }

                                    client.println("<p><a href=\"../\"><button class=\"button button2\">Refresh view</button></a></p>");
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
        }
    }
