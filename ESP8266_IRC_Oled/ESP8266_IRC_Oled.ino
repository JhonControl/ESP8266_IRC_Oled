// Blog PDAControl English   http://goo.gl/4py0H3  -  http://pdacontrolenglish.blogspot.com.co/   
// Blog PDAControl Español   http://goo.gl/rZ6wPo  -   http://pdacontrol.blogspot.com.co/
// https://www.youtube.com/c/JhonValenciaPDAcontrol/videos    https://goo.gl/Cn7AQq

#include <ESP8266WiFi.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);


const char* ssid     = "1503523";
const char* password = "D2E7D32DBC883";

const char* host = "chat.freenode.net";

WiFiClient client;
   
void setup() {
  
    Wire.begin(0,2);                                // Initialize I2C and OLED Display
 // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);    
 
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(INVERSE); 
    
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
 display.clearDisplay();  
 display.setCursor(0,0);
 display.print("Connecting to "); 
 display.print(ssid);
 display.display(); 
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}



void loop()
{
 
  if (!client.connected()) {
    Serial.println("connecting ...");
    if (client.connect(host, 6667)) {
      
      display.clearDisplay();  
      display.setCursor(0,0);
      display.print("connected"); 
      display.setCursor(0,8);
      display.print("NICK Esp8266IRC\r"); 
      display.setCursor(0,17);      
      display.print("JOIN #tawano\r"); 
      
      display.display(); 
      
      
      
        
      
      
      Serial.println("connected");
      delay(1000);
      client.println("USER  uk 8 *  : Uwe Kamper\r");
      delay(500);
      client.println("NICK Esp8266IRC\r");
      delay(500);
      client.println("JOIN #tawano\r");
      delay(500);
      
      display.clearDisplay(); 
      display.setCursor(0,0);
            
      handle_irc_connection();
    } 
    else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      
      
      display.clearDisplay();  
      display.setCursor(0,0);
      display.print("connection failed");
      display.display(); 
      
      delay(2000);
    }
  }





  
}
  
#define IRC_BUFSIZE  32
char from[IRC_BUFSIZE];
char type[IRC_BUFSIZE];
char to[IRC_BUFSIZE];

void handle_irc_connection() {
  char c;
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while(true) {
    if (!client.connected()) {
     
      return;
    }
    if(client.available()) {
      c = client.read();
      
      
    }
    else {
      continue;
    }
    
    if(c == ':') {
      memset(from, 0, sizeof(from));
      memset(type, 0, sizeof(type));
      memset(to, 0, sizeof(to));
      
      read_until(' ', from);
      read_until(' ', type);
      read_until(' ', to);
     
      if(strcmp(type, "PRIVMSG") == 0) {
        print_nick(from);
        ignore_until(':');
        print_until('\r');
      }
      else {
        
        ignore_until('\r');
      } 
    }
    // could be a PING request by the server.
    else if (c == 'P') {
      char buf[5];
      memset(buf, 0, sizeof(buf));
      buf[0] = c;
      for(int i = 1; i < 4; i++) {
        c = client.read();
        buf[i] = c;
      }
      ignore_until('\r');
      if(strcmp(buf, "PING") == 0) {
        client.println("PONG\r");
        //Serial.println("PING->PONG");
      }
    }
  } // end while
   
}

void print_nick(char buffer[]) {
  Serial.print("<");
  for(int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if(buffer[i] == '!') { 
      break; 
    }    
    Serial.print(buffer[i]);
  }
  Serial.print(">");
}

int read_until(char abort_c, char buffer[]) {
  int bytes_read = 0;
  memset(buffer, 0, sizeof(buffer));
  for(int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if (client.available()) {
      char c = client.read();  
      bytes_read++;
      if(c == abort_c) {
        return bytes_read;
      }
      else if (c == '\n') {
        return bytes_read;
      }
      buffer[i] = c;
    }
  }
  ignore_until(abort_c);
  return bytes_read;
}

// reads characters from the connection until
// it hits the given character.
void ignore_until(char c) {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == c) {
        return;  
      }
    }
  }
}

// reads characters from the connection until
// it hits the given character.
void print_until(char c) {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == c) {
        Serial.println("");
        display.clearDisplay();  
        display.setCursor(0,0);
        
        return;
      }
      Serial.print(curr_c);
      
       display.print(curr_c);               
       display.display(); 
    }
  }
}



// reads characters from the connection until
// it hits the given character.
void print_until_endline() {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == '\r') {
        curr_c = client.read();
        if (curr_c == '\n') { return; }
      }
      Serial.print(curr_c);    
      
      
    }
  }
}
