#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <util.h>

// Ethernet config
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,233,165,253);
EthernetServer server(2000);

// LCD config
LiquidCrystal lcd(9, 8, 7, 2, 1, 0);

// PWM pins
#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

// PWM min and max levels
#define R_MIN 0L
#define R_MAX 255L
#define G_MIN 0L
#define G_MAX 255L
#define B_MIN 0L
#define B_MAX 255L

// What to view on lcd?
#define SHOW_PWM_VALS false

// Color globals
int current_color[3];

// Init
void setup() {
  
  // Turn off leds
  display_color(0, 0, 0);
  delay(500);
  lcd.begin(16, 2);
  lcd.print("Project Xmas3   ");
  lcd.setCursor(0,1);
  lcd.print("Raadhuis United ");
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  // Do led test sequence
  delay(1000);
  display_color(255, 0, 0);
  delay(500);
  display_color(0, 0, 0);
  delay(500);
  display_color(0, 255, 0);
  delay(500);
  display_color(0, 0, 0);
  delay(500);
  display_color(0, 0, 255);
  delay(500);
  display_color(0, 0, 0);
  
}

// Main loop
void loop() {
  int new_color[3];
  int new_color_id = 0;
  int* color;
  unsigned long t_timeout;
  
  /*for(int i = 0;i<255;i++) {
    display_color(i, 0, 255-i); 
    delay(20);
  }

  for(int i = 0;i<255;i++) {
    display_color(255-i, i, 0); 
    delay(20);
  }

  for(int i = 0;i<255;i++) {
    display_color(0, 255-i, i); 
    delay(20);
  }*/
    
  // listen for incoming connections
  EthernetClient client = server.available();
  if (client) {
    // Suddenly, le wild client connects, send old color
    if (client.read() == 's') {
      // Client wants to set color
      t_timeout = millis()+1000;
      while (client.connected()) {
        // While the client is connected
        if (client.available()) {
          // Save byte
          new_color[new_color_id++] = client.read();
          if (new_color_id == 4) {
            // Color received
            display_color(new_color[0], new_color[1], new_color[2]);
            client.stop();
          }
        }
        if (millis() > t_timeout) {
          client.stop();
        }
      }
    } else {
      // Client wants to get color
      color = get_color();
      client.write(color[0]);
      client.write(color[1]);
      client.write(color[2]);
      client.stop();
    }
  }
}

// Functions
void update_pwm() {
  char lcd_buf[17];
  int* color = get_color();
  int pwm[3];
  pwm[0] = R_MIN+(int)(color[0]*(R_MAX-R_MIN)/255);
  pwm[1] = G_MIN+(int)(color[1]*(G_MAX-G_MIN)/255);
  pwm[2] = B_MIN+(int)(color[2]*(B_MAX-B_MIN)/255);
  analogWrite(R_PIN, pwm[0]);
  analogWrite(G_PIN, pwm[1]);
  analogWrite(B_PIN, pwm[2]);
  lcd.setCursor(0,1);
  if (SHOW_PWM_VALS) {
    sprintf(lcd_buf, "R%03d G%03d B%03d p", pwm[0], pwm[1], pwm[2]);
  } else {
    sprintf(lcd_buf, "R%03d G%03d B%03d c", color[0], color[1], color[2]);
  }
  lcd.print(lcd_buf);
}

void set_color(int r_val, int g_val, int b_val) {
  current_color[0] = r_val;
  current_color[1] = g_val;
  current_color[2] = b_val;
}
  
int* get_color() {
  return current_color;
}

void display_color(int r_val, int g_val, int b_val) {
  set_color(r_val, g_val, b_val);
  update_pwm();
}
