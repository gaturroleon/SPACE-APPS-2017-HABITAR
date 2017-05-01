


//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
// ARDN  - //display
// GND   - GND                       - 8
// pin 2 - LIGHT                     - 7
// VCC   - VCC 5V                    - 6
// pin 4 - Serial clock out (SCLK)   - 5
// pin 5 - Serial data out (DIN)     - 4
// pin 6 - Data/Command select (D/C) - 3
// pin 7 - LCD chip select (CS)      - 2
// pin 8 - LCD reset (RST)           - 1
//Adafruit_PCD8544 //display = Adafruit_PCD8544(4, 5, 6, 7, 8);


#include "HX711.h"

// HX711.DOUT  - pin #A1
// HX711.PD_SCK - pin #A0

HX711 scale(A1, A0);    // parameter "gain" is ommited; the default value 128 is used by the library


boolean stringComplete = false;  // whether the string is complete
String inputString = "";         // a string to hold incoming data
int Angulo = 0;
int DificultadCODO = 10;
int NegDificultad = -10;

int DificultadRODILLA = 10;

void setup() {
  ////display.begin();
  ////display.setContrast(43); //delay(500);
  ////display.clear//display();
  pinMode(20, OUTPUT); digitalWrite(20, LOW); //lo usode masa para el brillo //display
  pinMode(10, OUTPUT); digitalWrite(10, LOW); //pwm para el motor
  pinMode(11, OUTPUT); digitalWrite(11, LOW); //pwm para el motor
  pinMode(13, OUTPUT); digitalWrite(13, LOW);

  Serial.begin(9600);//38400
  Serial.println("CD Iniciado");
  pinMode(20, OUTPUT); digitalWrite(20, 0);
  digitalWrite(13, HIGH); //Listo e iniciado
  MuestroLogo();
}

//
//float fKG = 0;//kg.toFloat() ;
int fKG = 0;//kg.toFloat() ;

//char inChar;
//String inSerial1 = "";


void loop() {//-------------------------------------------------------------

//ESPERO LA NUEVA DIFICULTAD, SI ES QUE SE MODIFICO.
//LA DIFICULTAD VA A LLEGAR DEL SIG MODO: D###;### 
//DONDE EL PRIMER VALOR DE DIFICULTAD ES PARA EL CODO Y EL SEGUNDO ES PARA LA RODILLA
  if(Serial.available()){
      long t = millis();
      int inByte = Serial.read(); delay(5);
      if( inByte == 'D'){ String DiffCodo;
        do{ 
          int inByte = Serial.read(); delay(5);
          if(inByte == ';'){break;} //SI ES ENTER, SALE DEL LOOP
          DiffCodo =+ inByte; //SINO, CARGO LA VARIABLE.
        }while ((millis() - t) < 100);
        DificultadCODO = DiffCodo.toInt(); //aca defino cuanto vale la dificultad del CODO
      
      
      int inByte = Serial.read(); delay(5);
      if( inByte != '\n'){ String DiffRodilla; //SI ES DISTINTO DE ENTER, SALE DEL LOOP
        do{ 
          int inByte = Serial.read(); delay(5);
          if(inByte == '\n'){break;} //SI ES ENTER, SALE DEL LOOP
          DiffRodilla =+ inByte; //SINO, CARGO LA VARIABLE.
        }while ((millis() - t) < 100);
        DificultadRODILLA = DiffRodilla.toInt();//aca defino cuanto vale la dificultad de la RODILLA
      }
      }
  }  

// CONTROL DEL MOTOR DEL CODO DERECHO SEGUN EL VALOR DE LA CELDA DE CARGA Y LA DIFICULTAD

    String kg = String(scale.get_units(), 1); //promedia 5 lecturas para no poner dealys
    
    Angulo = analogRead(A7);
    Angulo = map(Angulo, 0, 1023, 0, 120);

    fKG =  kg.toInt(); //.toFloat();
    //Serial.println(int(fKG));
    //int AnguloAnt;
    Angulo = analogRead(A7);
    Angulo = map(Angulo, 0, 1023, 0, 120);
  
    NegDificultad = -DificultadCODO;
    //Serial.println(Dificultad);
    //Serial.println(NegDificultad);
    if (fKG < DificultadCODO && fKG > NegDificultad ) {
      ApagarMotor();
    }
    
    OPERAR_MOTOR_CODO(diff);

    
//  if (fKG > DificultadCODO) {
//    Avanzar(int(fKG) / 4);
//  };
//  if (fKG < NegDificultad) {
//    Retroceder(int(fKG) / 4);
//  }



//ESPERO QUE ME PIDAN LOS DATOS DEL CODO Y RODILLA DERECHO
// I ES EL COMANDO QUE ESPERO
   
    if(Serial.available()){
      int inByte = Serial.read(); delay(5);
      if( inByte == 'I'){
       String toSend = "CDa" + String(Angulo) + "k" + String(kg) + "d" + String(DificultadCODO); 
       Serial.println(toSend);


       //basicamente lo mismo para rodilla
       //String toSend = "RDa" + String(Angulo) + "k" + String(kg) + "d" + String(DificultadCODO); 
       //Serial.println(toSend);
        }
      }

}
//fin del loop principal


void OPERAR_MOTOR_CODO(int d){
  //ApagarMotor();
  int a = d; int r = d; 
    if(a <= 50){
      a = map(a,0,50,250,0);
      Avanzar(a);
      }
    
    if(r >= 51){
      r = map(r,50,101,0,250); //r va a valer entre 0 y 250
      int dly =  abs(map(r,250,4,0,250) / 2); //dly va a valer entre 2 y 250
      Retroceder(r); delay(dly);
      //Serial.println(dly);
      ApagarMotor();delay(dly);
      }
  
}



void ApagarMotor() {
  //Serial.println("apagado");
  //display.clear//display();//display.setCursor(60,0);
  //display.setTextSize(1);
  //display.println("APG:");
  //display.//display();
  pinMode(10, OUTPUT); digitalWrite(10, LOW); //pwm para el motor
  pinMode(11, OUTPUT); digitalWrite(11, LOW); //pwm para el motor
  analogWrite(13,0);
}

void Avanzar(int vel) {
  if (vel > 255) {
    vel = 255;
  }
  //Serial.println("AVZ");
  //display.clear//display();//display.setCursor(60,0);
  //display.setTextSize(1);
  //display.println("AVZ:");
  //display.//display();
  analogWrite(13,vel);
  pinMode(10, OUTPUT); digitalWrite(10, LOW); //pwm para el motor
  pinMode(11, OUTPUT); analogWrite(11, vel); //pwm para el motor

}


void Retroceder(int vel) {
  if (vel < -255) {
    vel = -255;
  }
  //Serial.print("RTD");
  //Serial.println(vel);
  //display.clear//display();//display.setCursor(60,0);
  //display.setTextSize(1);
  //display.println("RTD:");
  //display.//display();
  analogWrite(13,vel);
  pinMode(11, OUTPUT); digitalWrite(11, LOW); //pwm para el motor
  pinMode(10, OUTPUT); analogWrite(10, abs(vel)); //pwm para el motor
}


long t1 = 0; long t2 = 0;
void MuestroLogo() {
  digitalWrite(2, LOW);
  t1 = millis();
  //display.clear//display();
  //display.setTextSize(1);
  //display.setTextColor( BLACK);
  //display.setCursor(0,0);
  //display.println("HX711 Demo");
  Serial.println("HX711 Demo");
  //display.println("Before SET:");
  Serial.println("Before setting up the scale:");
  //display.print("read: \t\t");
  Serial.print("read: \t\t");
  //display.println(scale.read());
  Serial.println(scale.read());
  //display.print("read average: \t\t");
  Serial.print("read average: \t\t");
  //display.//display(); delay(1000); //display.setCursor(0,0);
  //display.println(scale.read_average(20));
  Serial.println(scale.read_average(20));
  //display.print("get value: \t\t");
  Serial.print("get value: \t\t");
  //display.println(scale.get_value(5));
  Serial.println(scale.get_value(5));
  //display.print("get units: \t\t");
  Serial.print("get units: \t\t");
  //display.println(scale.get_units(5), 1);
  Serial.println(scale.get_units(5), 1);
  scale.set_scale(2280.f);
  scale.tare();
  //display.//display(); delay(1000); //display.setCursor(0,0);
  //display.println("After setting up the scale:");
  Serial.println("After setting up the scale:");
  //display.print("read: \t\t");
  Serial.print("read: \t\t");
  //display.println(scale.read());
  Serial.println(scale.read());
  //display.setCursor(0,0);
  //display.print("read average: \t\t");
  Serial.print("read average: \t\t");
  //display.println(scale.read_average(20));
  Serial.println(scale.read_average(20));
  //display.print("get value: \t\t");
  Serial.print("get value: \t\t");
  //display.println(scale.get_value(5));
  Serial.println(scale.get_value(5));
  //display.//display(); delay(1000); //display.setCursor(0,0);
  //display.print("get units: \t\t");
  Serial.print("get units: \t\t");
  //display.println(scale.get_units(5), 1);
  Serial.println(scale.get_units(5), 1);
  //display.println("Readings:");
  Serial.println("ADQUIRIENDO:");
  ////display.println("HIGROMETRO");




  //
  //                //display.setTextSize(1);
  //                //display.setTextColor( BLACK);
  //                //display.setCursor(0,8);
  //                //display.println("   PARA SUELOS");
  //                //display.println("                       V 1.0");
  //                //display.//display();

//  do {
//    t2 = millis();
//  } while ((t2 - t1) < 2000);

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


