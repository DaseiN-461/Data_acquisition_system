#include <Arduino.h>


#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define width 128
#define hight 64

#define rst_oled 4

Adafruit_SSD1306 oled(width, hight, &Wire, rst_oled);









bool unstable_state = LOW;

float temperature, humidity, pressure, illuminance, uva, uvb, uvIndex;

void handle_packet();
bool refresh_screen(unsigned long interval);
void handle_screen();




void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  delay(3000);      

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.setCursor(0,0);
}

void loop() {
 

  handle_packet();
  handle_screen();
  
}

void handle_packet(){
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.println("Received packet ");
    String str_received = {};
    ////////////////TEST////////////////
    unstable_state = !unstable_state;
    digitalWrite(LED_BUILTIN, unstable_state);
    ////////////////////////////////////
    
    // read packet
    while (LoRa.available()){
      
      str_received += (char)LoRa.read();
      
    }

    String str_temperature = str_received.substring(0,4);
    String str_humidity = str_received.substring(5,9);
    String str_pressure = str_received.substring(10,14);
    String str_illuminance = str_received.substring(15,19);
    String str_uva = str_received.substring(20,24);
    String str_uvb = str_received.substring(25,29);
    String str_uvIndex = str_received.substring(30,34);
    
    
    temperature = str_temperature.toFloat();
    humidity = str_humidity.toFloat();
    pressure = str_pressure.toFloat();
    illuminance = str_illuminance.toFloat();
    uva = str_uva.toFloat();
    uvb = str_uvb.toFloat();
    uvIndex = str_uvIndex.toFloat();


    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

}



void handle_screen(){
  if(refresh_screen(3000)){
    oled.clearDisplay();
    oled.setCursor(0,0);

    oled.print("Temp: ");
    oled.print(temperature);
    oled.println("C");

    oled.print("Hume: ");
    oled.print(humidity);
    oled.println(" %");

    oled.print("Pres: ");
    oled.print(pressure);
    oled.println(" kPa");

    oled.print("Ilum: ");
    oled.print(illuminance);
    oled.println(" lx");

    oled.print("UVA:  ");
    oled.println(uva);
    
    oled.print("UVB:  ");
    oled.println(uvb);

    oled.print("UVIn: ");
    oled.println(uvIndex);

    oled.display();
    
  }
}

bool refresh_screen(unsigned long interval){
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

