

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// ARDN  - DISPLAY
// GND   - GND                       - 8
// pin 2 - LIGHT                     - 7 
// VCC   - VCC 5V                    - 6
// pin 4 - Serial clock out (SCLK)   - 5
// pin 5 - Serial data out (DIN)     - 4
// pin 6 - Data/Command select (D/C) - 3
// pin 7 - LCD chip select (CS)      - 2
// pin 8 - LCD reset (RST)           - 1
Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);


#include "HX711.h"

// HX711.DOUT  - pin #A1
// HX711.PD_SCK - pin #A0

HX711 scale(A1, A0);    // parameter "gain" is ommited; the default value 128 is used by the library



int Angulo = 0;
int Dificultad = 10;
int NegDificultad = -10;
void setup() {
  display.begin();
  display.setContrast(43); //delay(500);
  display.clearDisplay();
  pinMode(20,OUTPUT); digitalWrite(20,LOW);//lo usode masa para el brillo display
  pinMode(10,OUTPUT); digitalWrite(10,LOW);//pwm para el motor
  pinMode(11,OUTPUT); digitalWrite(11,LOW);//pwm para el motor
  MuestroLogo();
  
  Serial.begin(9600);//38400
  Serial.println("INICIADO");
  pinMode(20,OUTPUT); digitalWrite(20,0);
}

//
float fKG = 0;//kg.toFloat() ;


//char inChar;
String inSerial1 = "";


void loop() {
  String kg = String(scale.get_units(),1);//promedia 5 lecturas para no poner dealys

 Angulo = analogRead(A7); 
 Angulo = map(Angulo,0,1023,0,120);

 ////config digital

 //angulo: fuerza, dificultad, inicio rutina
  if( Serial.available()){ char inChar = char(Serial.read()); 

  switch (inChar){
      case 'D': //dificultad
            //SEC(); break;
        delay(5);
        inSerial1 = char(Serial.read());  delay(5);
        inSerial1 += String(char(Serial.read()));  delay(5);
        inSerial1 += String(char(Serial.read()));  delay(5);
        //inSerial1 += String(char(Serial.read()));  //delay(5);
        Serial.println (inSerial1);
        Dificultad = inSerial1.toInt();
        Serial.println (Dificultad);
      break;
      
      case 'A': //modo asistivo 
            //SEC(); 
            break;

      case 'R': //modo Resistivo 
            //SEC();
            break;
      case 'a': //envio de datos
            //String dt = Angulo + ":" + kg + ":" + Dificultad;
            Serial.print("BI:"); Serial.print(Angulo);
            Serial.print(";");Serial.print(kg);
            Serial.print(";");Serial.println(Dificultad);
            break;
      
  }     
}
  /////////////////////////// 

  
  fKG =  kg.toFloat();
  //Serial.println(int(fKG));
//int AnguloAnt;
 Angulo = analogRead(A7); 
 Angulo = map(Angulo,0,1023,0,120);
  //if((Angulo != AnguloAnt)) {Serial.print("Angulo: "); Serial.println(Angulo);} 
 //AnguloAnt = Angulo;




 
 NegDificultad = -Dificultad;
  //Serial.println(Dificultad);
  //Serial.println(NegDificultad);
  if(fKG < Dificultad && fKG > NegDificultad ){ApagarMotor();}
  if(fKG > Dificultad){Avanzar(int(fKG)/4);};
  if(fKG < NegDificultad){Retroceder(int(fKG)/4);};
  
  
        //display.setContrast(analogRead(A7)/10); //delay(500);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor( BLACK);
        display.setCursor(0,0);  
        display.println("Lectura:"); //Serial.print("one reading:\t");
        display.println("Dificultad: "); //Serial.print("one reading:\t");
        display.println(Dificultad);
        display.setTextSize(2);
        display.setCursor(0,32);
        display.println(kg);//Serial.print(scale.get_units(), 2);
        //display.setTextSize(1);
        //display.println("Promedio(10):"); Serial.print("\t| average:\t");
        //display.setTextSize(2);
        //display.println(scale.get_units(10), 1);Serial.println(scale.get_units(10), 1);
        display.display();
        //scale.power_down();             // put the ADC in sleep mode
        //delay(100);
        //scale.power_up();
}

//20:16:06:12:81:15
void ApagarMotor(){
  //Serial.println("apagado");
        display.clearDisplay();display.setCursor(60,0); 
        display.setTextSize(1);
        display.println("APG:");
        display.display();
  pinMode(10,OUTPUT); digitalWrite(10,LOW);//pwm para el motor
  pinMode(11,OUTPUT); digitalWrite(11,LOW);//pwm para el motor
}

void Avanzar(int vel){
  if(vel > 255){vel = 255;}
  //Serial.println("AVZ");
        display.clearDisplay();display.setCursor(60,0); 
        display.setTextSize(1);
        display.println("AVZ:");
        display.display();
  pinMode(10,OUTPUT); digitalWrite(10,LOW);//pwm para el motor
  pinMode(11,OUTPUT); analogWrite(11,vel);//pwm para el motor

}


void Retroceder(int vel){
  if(vel < -255){vel = -255;}
  //Serial.print("RTD");
  //Serial.println(vel);
          display.clearDisplay();display.setCursor(60,0); 
        display.setTextSize(1);
        display.println("RTD:");
        display.display();
  pinMode(11,OUTPUT); digitalWrite(11,LOW);//pwm para el motor
  pinMode(10,OUTPUT); analogWrite(10,abs(vel));//pwm para el motor
}

long t1 = 0; long t2 = 0;
void MuestroLogo(){digitalWrite(2,LOW);
        t1 = millis();
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor( BLACK);
        display.setCursor(0,0);        
        display.println("HX711 Demo"); Serial.println("HX711 Demo");
        display.println("Before SET:"); Serial.println("Before setting up the scale:");      
        display.print("read: \t\t"); Serial.print("read: \t\t");
        display.println(scale.read()); Serial.println(scale.read());    
        display.print("read average: \t\t"); Serial.print("read average: \t\t"); 
        display.display(); delay(1000); display.setCursor(0,0); 
        display.println(scale.read_average(20)); Serial.println(scale.read_average(20));
        display.print("get value: \t\t"); Serial.print("get value: \t\t");
        display.println(scale.get_value(5)); Serial.println(scale.get_value(5)); 
        display.print("get units: \t\t"); Serial.print("get units: \t\t");
        display.println(scale.get_units(5), 1); Serial.println(scale.get_units(5), 1);
        scale.set_scale(2280.f);
        scale.tare(); 
        display.display(); delay(1000); display.setCursor(0,0); 
        display.println("After setting up the scale:"); Serial.println("After setting up the scale:");
        display.print("read: \t\t");Serial.print("read: \t\t");
        display.println(scale.read());Serial.println(scale.read());
        display.setCursor(0,0);
        display.print("read average: \t\t");Serial.print("read average: \t\t");
        display.println(scale.read_average(20));Serial.println(scale.read_average(20));
        display.print("get value: \t\t");Serial.print("get value: \t\t");
        display.println(scale.get_value(5));Serial.println(scale.get_value(5));
        display.display(); delay(1000); display.setCursor(0,0);
        display.print("get units: \t\t");Serial.print("get units: \t\t");
        display.println(scale.get_units(5), 1);Serial.println(scale.get_units(5), 1);
        display.println("Readings:");Serial.println("Readings:");
                //display.println("HIGROMETRO"); 




//        
//                display.setTextSize(1);
//                display.setTextColor( BLACK);
//                display.setCursor(0,8);
//                display.println("   PARA SUELOS"); 
//                display.println("                       V 1.0");
//                display.display();
                
        do{
                t2 = millis();
        }while ((t2 - t1) < 2000);

  //Serial.println("HX711 Demo");

  //Serial.println("Before setting up the scale:");
  //Serial.print("read: \t\t");
  //Serial.println(scale.read());     // print a raw reading from the ADC

  //Serial.print("read average: \t\t");
  //Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  //Serial.print("get value: \t\t");
  //Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  //Serial.print("get units: \t\t");
  //Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided 
            // by the SCALE parameter (not set yet)  

  //scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
 //scale.tare();               // reset the scale to 0

  //Serial.println("After setting up the scale:");

  //Serial.print("read: \t\t");
  //Serial.println(scale.read());                 // print a raw reading from the ADC

  //Serial.print("read average: \t\t");
  //Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  //Serial.print("get value: \t\t");
  //Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  //Serial.print("get units: \t\t");
  //Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
            // by the SCALE parameter set with set_scale

  //Serial.println("Readings:");

}


