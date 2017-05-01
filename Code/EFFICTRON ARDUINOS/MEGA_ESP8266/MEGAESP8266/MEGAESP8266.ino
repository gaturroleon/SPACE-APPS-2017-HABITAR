/*
ESP8266 library

When you use with UNO board, uncomment the follow line in uartWIFI.h.
#define UNO

When you use with MEGA board, uncomment the follow line in uartWIFI.h.
#define MEGA

Connection:
When you use it with UNO board, the connection should be like these:
ESP8266_TX->D0
ESP8266_RX->D1
ESP8266_CH_PD->3.3V
ESP8266_VCC->3.3V
ESP8266_GND->GND

FTDI_RX->D3			//The baud rate of software serial can't be higher that 19200, so we use software serial as a debug port
FTDI_TX->D2

When you use it with MEGA board, the connection should be like these:
ESP8266_TX->RX1(D19)
ESP8266_RX->TX1(D18)
ESP8266_CH_PD->3.3V
ESP8266_VCC->3.3V
ESP8266_GND->GND

When you want to output the debug information, please use DebugSerial. For example,

DebugSerial.println("hello");


Note:	The size of message from ESP8266 is too big for arduino sometimes, so the library can't receive the whole buffer because  
the size of the hardware serial buffer which is defined in HardwareSerial.h is too small.

Open the file from \arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h.
See the follow line in the HardwareSerial.h file.

#define SERIAL_BUFFER_SIZE 64

The default size of the buffer is 64. Change it into a bigger number, like 256 or more.





*/


//#define SSID       "DONVICHO"
//#define PASSWORD   "02009"
String SSID  =     "DONVICHO";
String PASSWORD =  "02009";

String newSSID; String newPASS;

#include "uartWIFI.h"
#include <SoftwareSerial.h>
WIFI wifi;
String ipstringTXT;
extern int chlID;	


void setup()
{
  pinMode(13,OUTPUT);digitalWrite(13,HIGH);delay(1000);
  wifi.begin();
  bool b = wifi.Initialize(STA, SSID, PASSWORD);
  //bool b = wifi.Initialize(STA, "DONVICHO", "02009");
  if(!b)
  {
    DebugSerial.println("Init error");
  }
  delay(2000);  //make sure the module can have enough time to get an IP address 
  String ipstring  = wifi.showIP();
  ipstringTXT = ipstring;
  DebugSerial.println(ipstring);		//show the ip address of module
  
  delay(3000);
  wifi.confMux(1);
  delay(100);
  if(wifi.confServer(1,80))
	DebugSerial.println("Server is set up");
	
  DebugSerial.println(ipstring);
  
}

long llam = 0;
char inChar;
void loop()
{
int cambioAlgo = 0 ;

 if(DebugSerial.available()>0){
     inChar = char(DebugSerial.read());
     String in="";
      switch (inChar){
        case 'U': 
        //HAY QUE PONER UNA PREGUNTA ANTES DE ENTRAR EN LA RUTINA
                newSSID = ""; newPASS="";
                DebugSerial.println("CAMBIO DE SSID Y PASS");
                DebugSerial.println("PARA CANCELAR PRESIONE ;; Y ENTER");
                DebugSerial.println("Ingrese el nuevo SSID + ; y presione ENTER");
                
                do{ 
                  delay(5);
                  if(Serial.available()){
                      in = String(char(Serial.read()));  delay(1);
                      //Serial.print(in);
                      if(in != ";"){newSSID += (in);  delay(1);}
                      //if (in == ';') {break;}
                  }
                }while(in != ";");
                
                in = "";
                DebugSerial.println("");
                DebugSerial.println("Ingrese el nuevo PASS + ; y presione ENTER");
        
                do{  //String in;
                  delay(5);
                  if(Serial.available()){
                      in = String(char(Serial.read()));  delay(1);
                      //Serial.print(in);
                      if(in != ";"){newPASS += (in);  delay(1);}
                      //if (in == ';') {break;}
                  }
                }while(in != ";");
                in = "";

                if(newSSID != ""){SSID = newSSID; cambioAlgo ++;}
                if(newPASS != ""){SSID = PASSWORD = newPASS; cambioAlgo ++; }
                //Serial.println(cambioAlgo);
                if(cambioAlgo != 0){
                    DebugSerial.println("LOS NUEVOS DATOS YA ESTAN GUARDADOS Y SON: ");
                    DebugSerial.print("SSID: "); DebugSerial.println(newSSID); 
                    DebugSerial.print("PASS: "); DebugSerial.println(newPASS); 
                    DebugSerial.println("REINICIANDO PARA QUE LOS CAMBIOS HAGAN EFECTO; AGUARDE POR FAVOR!!");
                    Reconectar(); 
                }
                else{
                  DebugSerial.println("NO SE REALIZO NINGUN CAMBIO!!");
                  DebugSerial.print("SSID: "); DebugSerial.println(SSID); 
                  DebugSerial.print("PASS: "); DebugSerial.println(PASSWORD);
                }
                
                
                
                ; break;
        case 'u':
                DebugSerial.print("SSID: "); DebugSerial.print(SSID); 
                DebugSerial.print("PASS: "); DebugSerial.println(PASSWORD);
            ; break;
        case 'R':
                DebugSerial.print("Reconectar a: ");
                DebugSerial.print("SSID: "); DebugSerial.print(SSID); 
                DebugSerial.print(" PASS: "); DebugSerial.println(PASSWORD);
                Reconectar();
                ; break;
            
            //capturar los datos del serial
            //guardarlos ordenadamente en eemprom
            //reemplazar las variables  SSID y PASS
            //Reconectar al nuevo ruter
            //SEC(); break;

//    int Pin = 255; 
//    do{
//        delay(5);
//        inSerial1 = String(char(Serial.read()));  delay(1);
//        inSerial1 += String(char(Serial.read())); delay(1);
//        //Serial.println(inSerial1);
//        Pin = inSerial1.toInt();
//        if (Pin == 0) {Pin = 50;}
//        if (Pin == 1) {Pin = 51;}
//        if (Pin == 20) {Pin = 52;}
//        if (Pin == 21) {Pin = 53;}
//         // int(inSerial1);
//        inSerial1 = String(char(Serial.read())); delay(1);
//        tipo = inSerial1.toInt();
//        switch (tipo){
//          case 0:
//              pinMode(Pin,INPUT); break;
//               
//          case 1:
//              pinMode(Pin,INPUT_PULLUP);break;
//        
//        }
//     }while(Serial.read() == char(13));
  }
  }

  
  
  char buf[500];
  int iLen = wifi.ReceiveMessage(buf);
  if(iLen > 0)
  {
    //DebugSerial.println(ipstring);
    
      DebugSerial.println(ipstringTXT);
      delay(100);
    //DebugSerial.println(buf);
      if (strncmp(buf, "GET /actual", 11) == 0) {  
        DebugSerial.println("llego un GET");
      
      String cmd;
      cmd = "HTTP/1.1 200 OK\r\n";
      cmd += "Content-Type: text/html\r\n";
      cmd += "Connection: close\r\n";
      cmd += "Refresh: 3\r\n";
      cmd += "\r\n";
      cmd += "<!DOCTYPE HTML>\r\n";
      cmd += "<html>\r\n";
      for(int analogChannel = 0; analogChannel < 6; analogChannel++)
      {
        int sensorReading = analogRead(analogChannel);
        cmd += "analog input ";
        cmd += String(analogChannel);
        cmd += " is ";
        cmd += String(sensorReading);
        cmd += "<br />\r\n";
      
      }     
        cmd += String("chlID ");
        cmd += String(chlID);
        cmd += String(" Llamados \r\n");
        cmd += String(llam);
      cmd += "<html>\r\n";

      llam ++;
      
        wifi.Send(chlID,cmd);
        delay(300);
        wifi.closeMux(chlID);
        DebugSerial.println(chlID);
		//delay(1000);
  }
  //wifi.closeMux(chlID);
  }
}





void Reconectar(){

//wifi.begin();


digitalWrite(13,LOW); delay(500);digitalWrite(13,HIGH);delay(1000);

  bool b = wifi.Initialize(STA, SSID, PASSWORD);
  //bool b = wifi.Initialize(STA, "DONVICHO", "02009");
  if(!b)
  {
    DebugSerial.println("Init error");
  }
  delay(2000);  //make sure the module can have enough time to get an IP address 
  String ipstring  = wifi.showIP();
  ipstringTXT = ipstring;
  DebugSerial.println(ipstring);    //show the ip address of module
  
  delay(3000);
  wifi.confMux(1);
  delay(100);
  if(wifi.confServer(1,80))
  DebugSerial.println("Reiniciado");
  
  DebugSerial.println(ipstring);
  
}

