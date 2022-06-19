//Sensor en base a la placa MKR WAN 1300 y MKR ENV SHIELD
//Comunicación punto a punto mediante modulación LoRa



#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

////////////////////////////////////////////////////////////
#include <Arduino_MKRENV.h>
////////////////////////////////////////////////////////////

float temperature, humidity, pressure, illuminance, uva, uvb, uvIndex; 
////////////////////////////////////////////////////////////
float packet_to_send[7];



int count = 0;
bool unstable_state = LOW;


// [4B][4B][4B][4B][4B][4B][4B] 7x4 Bytes
// temp,hum,press,illu,uva,uvb,uvi


void MKR_ENV_init();
void LoRa_init(int freq);

void get_values();
/* Toma los valores de los sensores y los guarda
en las variables globales del tipo flotante */

void print_values();
/* Imprime los valores por el puerto serial */

void send_packet(float packet[]);
/* Envía un paquete de tipo String por la comunicación
LoRa */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //while(!Serial);
  delay(3000);
  Serial.println("LoRa sender");

  MKR_ENV_init();
  LoRa_init(868E6);
  
}

void loop() {
  
  get_values();
  print_values();

  
  send_packet(packet_to_send);

  

  delay(5000);
}


void LoRa_init(int freq){
  if(!LoRa.begin(freq)){
    Serial.println("Starting LoRa failed!");
    while(1);
  }
}

void send_packet(float packet[]){
  Serial.print("Sending packet: ");
  Serial.println(count);


  Serial.println("--------");

  //send packet
  LoRa.beginPacket();

  for(int i=0; i<7; i++){
    LoRa.print(packet_to_send[i]);
  }

  LoRa.endPacket();

  Serial.println("--------");

  for(int i=0; i<7; i++){
    Serial.println(packet_to_send[i]);
    Serial.println(sizeof(packet_to_send[i]));
    Serial.println();
  }
  
  count++;
  // Luz indicadora de paquete transmitido
  digitalWrite(LED_BUILTIN, unstable_state);
  unstable_state = !unstable_state;
  
}



void MKR_ENV_init(){
  if (!ENV.begin()) {

    Serial.println("Failed to initialize MKR ENV shield!");

    while (1);

  }
}

void get_values(){
  temperature = ENV.readTemperature();

  humidity    = ENV.readHumidity();

  pressure    = ENV.readPressure();

  illuminance = ENV.readIlluminance();

  uva         = ENV.readUVA();

  uvb         = ENV.readUVB();

  uvIndex     = ENV.readUVIndex();

  packet_to_send[0] = temperature;
  packet_to_send[1] = humidity;
  packet_to_send[2] = pressure;
  packet_to_send[3] = illuminance;
  packet_to_send[4] = uva;
  packet_to_send[5] = uvb;
  packet_to_send[6] = uvIndex;


}


void print_values(){
  // read all the sensor values

  

  Serial.print("Temperature = ");

  Serial.print(temperature);

  Serial.println(" &#xB0;C");

  Serial.print("Humidity    = ");

  Serial.print(humidity);

  Serial.println(" %");

  Serial.print("Pressure    = ");

  Serial.print(pressure);

  Serial.println(" kPa");

  Serial.print("Illuminance = ");

  Serial.print(illuminance);

  Serial.println(" lx");

  Serial.print("UVA         = ");

  Serial.println(uva);

  Serial.print("UVB         = ");

  Serial.println(uvb);

  Serial.print("UV Index    = ");

  Serial.println(uvIndex);

  // print an empty line

  Serial.println();
}

