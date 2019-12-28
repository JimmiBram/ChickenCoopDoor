//GLOBALS
WiFiServer server(80);
String header;
const char* ssid       = "ChiliNet";
const char* password   = "xxx";
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


//Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Timeout variables
unsigned long w_currentTime = millis();
unsigned long w_previousTime = 0;
const long w_timeoutTime = 2000;

void SetupWifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Log("Wifi connected");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  timeClient.forceUpdate();
  server.begin();
  }


void UpdateTime(){
      timeClient.update();
  }

void HandleWifi(bool wholeSecond, bool wholeMinute, bool wholeHour){

  if(wholeHour){
      UpdateTime();
    }

  if(wholeSecond){
      currentTime = timeClient.getFormattedTime();
      currentHour = timeClient.getHours();
      currentMinute = timeClient.getMinutes();
      currentDay = timeClient.getDay();
    }
  
  WiFiClient client = server.available();   // Listen for incoming clients
   if (client) {
        Serial.print("Client Connected ");
        header = "";
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

                                    
                                    if (header.indexOf("GET /door/open") >= 0) {
                                      Serial.println("WR:Opening door");
                                      OpenDoor();
                                    } 
                                    
                                    if (header.indexOf("GET /door/close") >= 0) {
                                      Serial.println("WR:Closing door");
                                      CloseDoor();
                                    }
                                    
                                    if (header.indexOf("GET /door/stop") >= 0) {
                                      Serial.println("WR:Stopping door");
                                      StopDoor();
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
                                    client.println("<p>Time is " + currentTime + "</p>");
                                    client.println("<p>Light is " + String(currentLux) + "lux</p>");
                                    client.println("<p>Door is " + doorState + "</p>");
                                    
                                    // If the output5State is off, it displays the ON button       
                                    if (doorState=="closed" || doorState=="closing" || doorState=="unknown") {
                                      client.println("<p><a href=\"/door/open?ms=" + currentTime + "\"><button class=\"button\">Open door</button></a></p>");
                                    }
                                    
                                    if (doorState=="open" || doorState=="opening" || doorState=="unknown"){
                                      client.println("<p><a href=\"/door/close?ms=" + currentTime + "\"><button class=\"button button2\">Close door</button></a></p>");
                                    }

                                    if (doorState=="opening" || doorState=="closing") {
                                      client.println("<p><a href=\"/door/stop?ms=" + currentTime + "\"><button class=\"button\">Stop door</button></a></p>");
                                    }

                                    //doorState = "unknown";  //States: opening, closing, closed, open, unknown

                                    client.println("<p><a href=\"../\"><button class=\"button button2\">Refresh view</button></a></p>");

                                    client.println("<p><b>LOG:</b></p>");
                                    client.println("<p>" + GetLog() + "</p>");
                                    
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




String logHist;

void Log(String logEntry){
  Serial.println(logEntry);
  logHist = logEntry + String(" (") + currentTime + String(")<br>") + logHist;
  int logSize = logHist.length();
  if(logSize > 300){
      int lastBreak = logHist.lastIndexOf("<br>");
      logHist = logHist.substring(0,lastBreak-4);
    };
  }

String GetLog(){
  return logHist;
  }
