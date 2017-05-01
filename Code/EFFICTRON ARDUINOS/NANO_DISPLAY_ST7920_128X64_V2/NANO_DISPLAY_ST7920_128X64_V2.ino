

#include <EEPROM.h>
#include "U8glib.h"
//ARDUINO UNO
//U8GLIB_ST7920_128X64 u8g(13, 12, 11, U8G_PIN_NONE);//Enable 6, R/W 5, RS 4, RESET

//ARDUINO MEGA
//U8GLIB_ST7920_128X64 u8g(52, 50, 51, U8G_PIN_NONE);//Enable 6, R/W 5, RS 4, RESET


//ARDUINO NANO
U8GLIB_ST7920_128X64_1X u8g(13, 12, 11);  // SPI Com: SCK = en = 13, MOSI = rw = 12, CS = di = 11


long t = 0;
int LecturaCAD = 0;
int Idioma = 0;

String inputStringRODD = "";         // a string to hold incoming data
String inputStringRODI = "";         // a string to hold incoming data
String inputStringCODD = "";         // a string to hold incoming data
String inputStringCODI = "";         // a string to hold incoming data
boolean stringComplete = false;      // whether the string is complete
String inputString = "";             // a string to hold incoming data

bool btnMenos = true;
bool btnSelec = true;
bool btnMas = true;
bool CambioVAL = false;

int c = 0;//cursor

int diffCODI = 10; // fidicultad
int diffRODI = 10; // fidicultad
int diffCODD = 10; // fidicultad
int diffRODD = 10; // fidicultad

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(3), Interrup, CHANGE);//detachInterrupt(digitalPinToInterrupt(3));
  Serial.begin(9600);
  Serial.print("INICIO P"); Serial.print(char(250));Serial.println("lsAR V3.0");
  //Serial1.begin(9600);//serial del bluethooth
  //for(int n = 22 ; n < 32 ; n++){ pinMode(n,OUTPUT); digitalWrite(n,HIGH); }
  //pinMode(13,INPUT);


  // flip screen, if required
  // u8g.setRot180();

  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  //int t = millis();
  u8g.firstPage();
  do {
    MostrarINICIO();
  } while ( u8g.nextPage() );
  //int t2 = millis();
  //Serial.println(t2-t);
  delay(2000);
  u8g.firstPage();
  do {
    MostrarINFO();
  } while ( u8g.nextPage() );
}



float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void loop() {

  LeerBotones();


  if (stringComplete) {
    //Serial.println(inputString);//ESPERO ROD D 180  -100  10%
    // clear the string:
    int CDI = inputString.indexOf("COD I"); //Serial.println(CDD);
    int ROI = inputString.indexOf("ROD I"); //Serial.println(ROD);
    int CDD = inputString.indexOf("COD D"); //Serial.println(CDI);
    int ROD = inputString.indexOf("ROD D"); //Serial.println(ROI);
    int CLS = inputString.indexOf("CLS"); //Serial.println(ROI);

    if (CDI == 0) {
      inputStringCODI = inputString ;
    }
    if (ROI == 0) {
      inputStringRODI = inputString ;
    }
    if (CDD == 0) {
      inputStringCODD = inputString ;
    }
    if (ROD == 0) {
      inputStringRODD = inputString ;
    }

    if (CLS == 0) {
      inputStringCODI = "" ;
      inputStringRODI = "" ;
      inputStringCODD = "" ;
      inputStringRODD = "" ;
    }

    u8g.firstPage();  do {
      MostrarINFO();
    } while ( u8g.nextPage() );

    inputString = "";
    stringComplete = false;
  }
}



void LeerBotones() {
  btnMenos = digitalRead(2);
  btnSelec = digitalRead(3);
  btnMas = digitalRead(4);

  if (btnMenos == false && btnMas == false) {
    long t = millis();
    do { } while ((millis() - t) < 2000);
    btnMas = digitalRead(4); btnMenos = digitalRead(2);
    if (btnMenos == false && btnMas == false) {
      //ir al menu de lenguaje
      SelecLenguaje();
    }

  }



  if (btnSelec == false) {
    u8g.firstPage();
    do {
      u8g.drawRFrame(31, 54, 69 , 9, 2);
      MostrarINFO();
    } while ( u8g.nextPage() );
    delay(200);
    ConfigDiff();
  }
  //if(btnMas == false)  {btnPush(); }
}



void SelecLenguaje() {
  DisplayCLS();
  int Y = 0;
  long t = millis();
  do {
    btnMenos = digitalRead(2);
    btnSelec = digitalRead(3);
    btnMas = digitalRead(4);

    if (btnMenos == false) {
      t = millis();
      delay(200);
      if (btnMenos == false) {
        Idioma++;
        if (Y > 55) {
          Y = 0;
        } if (Idioma > 50) {
          Idioma = -7;
        }
        Y = 16 + (Idioma * 7);
      }


    }

    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr90(11, 56, ">");
      u8g.drawRFrame(0, 53, 30 , 11, 2); //x; y; ancho; alto; radio

      u8g.drawStr( 50, 62, "Elegir");
      u8g.drawRFrame(30, 53, 68 , 11, 2); //x; y; ancho; alto; radio

      u8g.drawStr90(110, 56, "<");
      u8g.drawRFrame(98, 53, 30 , 11, 2); //x; y; ancho; alto; radio


      u8g.setPrintPos( 50,  8); u8g.print("IDIOMAS");
      u8g.setFont(u8g_font_04b_03);
      u8g.setPrintPos( 15,  16); u8g.print("Latino Americano");
      u8g.setPrintPos( 15,  23); u8g.print("Ingles");
      u8g.setPrintPos( 15,  30); u8g.print("Ruso");
      u8g.setPrintPos( 15,  37); u8g.print("Jaopnes");
      u8g.setPrintPos( 15,  44); u8g.print("Chino");

      u8g.setPrintPos( 15,  52); u8g.print(t); u8g.print("   "); u8g.print(Y);
      u8g.drawTriangle(0, Y, 0, Y + 4, 0, Y / 2);
      //u8g.drawTriangle(90,9, 90,17, 95,13);

    } while ( u8g.nextPage() );

  } while ((millis() - t) < 5000);

  u8g.firstPage();  do {
    MostrarINFO();
  } while ( u8g.nextPage() );

}




void MostrarINICIO() {
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g.setFont(u8g_font_04b_03);
  u8g.setFont(u8g_font_fub17);
  //u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_fur11n);
  u8g.setPrintPos( 25,  17); u8g.print("P"); u8g.print(char(250)); u8g.print("lsAR");
  u8g.setPrintPos( -2, 64);  u8g.print("EFFICTRON");

  //delay(5000);
}

void MostrarINFO() {


  //u8g.firstPage();
  //do {
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g.setFont(u8g_font_04b_03);
  //u8g.setFont(u8g_font_fub17);
  u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_fur11n);
  u8g.setPrintPos( 0,  7); u8g.print(" ART   ANG  Kg/F  DIF");//HASTA 21 CHAR
  u8g.setPrintPos( 0, 17);  u8g.print("COD I");
  u8g.setPrintPos( 0, 27);  u8g.print("ROD I");
  u8g.setPrintPos( 0, 37);  u8g.print("COD D");
  u8g.setPrintPos( 0, 47);  u8g.print("ROD D");
  //u8g.setPrintPos( 0, 64);  u8g.print("12345678901234567890123456");


  //lineas
  u8g.drawLine(0, 8, 128, 8);//linea horizontal
  u8g.drawLine(0, 18, 128, 18);//linea horizontal
  u8g.drawLine(0, 28, 128, 28);//linea horizontal
  u8g.drawLine(0, 38, 128, 38);//linea horizontal
  u8g.drawLine(0, 48, 128, 48);//linea horizontal

  u8g.drawLine(30, 0, 30, 48);//linea vertical
  u8g.drawLine(64, 0, 64, 48);//linea vertical
  u8g.drawLine(100, 0, 100, 48);//linea vertical
  //u8g.setPrintPos( 82, 63);

  //botones
  //              if(c != 0){
  //                Serial.println("no es dist. 0");
  //                u8g.drawStr( 3, 62, "Menos");
  //                u8g.drawRFrame(0, 53,34 ,11, 2); //x; y; ancho; alto; radio
  //
  //                u8g.drawStr( 105, 62, "Mas");
  //                u8g.drawRFrame(95, 53,33 ,11, 2); //x; y; ancho; alto; radio
  //              }

  u8g.drawStr( 33, 62, "Cambiar DIF");
  u8g.drawRFrame(30, 53, 71 , 11, 2); //x; y; ancho; alto; radio




//ESTOS SON LOS DATOS QUE SE VAN ACTUALIZANDO
  String CDa =""; String CDk = ""; String CDd = "";
  //int CD = inputString.indexOf("a"); 
  //int CD = inputString.indexOf("a"); 
  //.substring(19)
  Serial.println(inputStringCODD);
  for(int CD = 3; CD < 50; CD++){
    if(inputStringCODD.substring(CD) != "k"){
      CDa = CDa +  inputStringCODD.substring(CD); 
      u8g.setPrintPos( 42, 37);  u8g.print(CDa);
      }
    else {CDa = ""; break;}
  }

  
  
  u8g.setPrintPos( 72, 37);  u8g.print(CDk);
  u8g.setPrintPos( 108, 37);  u8g.print(CDd);
  
  u8g.setPrintPos( 0, 17);  u8g.print(inputStringCODI);
  u8g.setPrintPos( 0, 27);  u8g.print(inputStringRODI);
  u8g.setPrintPos( 0, 37);  u8g.print(inputStringCODD);
  u8g.setPrintPos( 0, 47);  u8g.print(inputStringRODD);
  //MostrarINFO();
  //} while( u8g.nextPage() );


}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    //Serial.print(inChar);
    if ((inChar == '%') || (inChar == '\n')) {
      stringComplete = true;
    }
  }
}

void DisplayCLS() {
  u8g.firstPage();
  do {  } while ( u8g.nextPage());

}

void ConfigDiff() {
  //Serial.println("ConficDiff");
  DisplayCLS(); delay(50);
  long t = millis();
  //Serial.print("t = "); Serial.println(t);
  do {

    //leo los botones
    btnMenos = digitalRead(2);
    btnSelec = digitalRead(3);
    btnMas = digitalRead(4);
    delay(50);
    //si se presiono uno
    if (c > 4) {
      c = 0;
    }

    if (btnSelec == false) {
      /*delay(200);*/  if (btnSelec == false) {
        t = millis(); /*delay(150);*/  c ++;
      }
    }

    //si no estan presionados
    if (btnMenos == true) {
      {
        u8g.drawStr( 3, 62, "Menos");
        u8g.drawRFrame(0, 53, 34 , 11, 2);
      }
    }
    if (btnSelec == true) {
      {
        u8g.drawRFrame(36, 53, 57 , 11, 2);
      }
    }
    if (btnMas == true)  {
      {
        u8g.drawStr( 105, 62, "Mas");
        u8g.drawRFrame(95, 53, 33 , 11, 2);
      }
    }

    if (c == 1) {
      if (btnMenos == false) {
        /*u8g.drawRFrame( 1, 54,32 ,9, 2);*/ t = millis();
        diffCODI --;
        CambioVAL = true;
      }
    }
    if (c == 1) {
      if (btnMas == false)  {
        /*u8g.drawRFrame(96, 54,31 ,9, 2); t =*/ millis();
        diffCODI ++;
        CambioVAL = true;
      }
    }

    if (c == 2) {
      if (btnMenos == false) {
        /*u8g.drawRFrame( 1, 54,32 ,9, 2);*/ t = millis();
        diffRODI --;
        CambioVAL = true;
      }
    }
    if (c == 2) {
      if (btnMas == false)  {
        /*u8g.drawRFrame(96, 54,31 ,9, 2);*/ t = millis();
        diffRODI ++;
        CambioVAL = true;
      }
    }

    if (c == 3) {
      if (btnMenos == false) {
        /*u8g.drawRFrame( 1, 54,32 ,9, 2); */t = millis();
        diffCODD --;
        CambioVAL = true;
      }
    }
    if (c == 3) {
      if (btnMas == false)  {
        /*u8g.drawRFrame(96, 54,31 ,9, 2); */t = millis();
        diffCODD ++;
        CambioVAL = true;
      }
    }

    if (c == 4) {
      if (btnMenos == false) {
        /*u8g.drawRFrame( 1, 54,32 ,9, 2);*/ t = millis();
        diffRODD --;
        CambioVAL = true;
      }
    }
    if (c == 4) {
      if (btnMas == false)  {
        /*u8g.drawRFrame(96, 54,31 ,9, 2);*/ t = millis();
        diffRODD ++;
        CambioVAL = true;
      }
    }



    u8g.firstPage();
    do {

      if (btnMenos == false) {
        t = millis();
        u8g.drawRFrame( 1, 54, 32 , 9, 2);
      }
      if (btnSelec == false) {
        t = millis();
        u8g.drawRFrame(37, 54, 55 , 9, 2);
      }
      if (btnMas == false)  {
        t = millis();
        u8g.drawRFrame(96, 54, 31 , 9, 2);
      }

      //textos
      u8g.setFont(u8g_font_6x10);
      //u8g.setFont(u8g_font_fur11n);
      u8g.setPrintPos( 0,  7); u8g.print("ARTICULACION     %DIF");//HASTA 21 CHAR
      u8g.setPrintPos( 0, 17);  u8g.print("CODO Izq.        "); u8g.print(diffCODI); u8g.print("%");
      u8g.setPrintPos( 0, 27);  u8g.print("RODODILLA Izq.   "); u8g.print(diffRODI); u8g.print("%");
      u8g.setPrintPos( 0, 37);  u8g.print("CODO Der.        "); u8g.print(diffCODD); u8g.print("%");
      u8g.setPrintPos( 0, 47);  u8g.print("RODILLA Der.     "); u8g.print(diffRODD); u8g.print("%");

      //lineas
      u8g.drawLine(0, 8, 128, 8);//linea horizontal
      u8g.drawLine(0, 18, 128, 18);//linea horizontal
      u8g.drawLine(0, 28, 128, 28);//linea horizontal
      u8g.drawLine(0, 38, 128, 38);//linea horizontal
      u8g.drawLine(0, 48, 128, 48);//linea horizontal

      if (c != 0) {
        u8g.drawStr( 3, 62, "Menos");
        u8g.drawRFrame(0, 53, 34 , 11, 2); //x; y; ancho; alto; radio

        u8g.drawStr( 105, 62, "Mas");
        u8g.drawRFrame(95, 53, 33 , 11, 2); //x; y; ancho; alto; radio
      }

      u8g.drawStr( 38, 62, "Seleccion");
      u8g.drawRFrame(36, 53, 57 , 11, 2); //x; y; ancho; alto; radio

      switch (c) {
        case 1:
          u8g.drawTriangle(90, 9, 90, 17, 95, 13);
          break;
        case 2:// no hay nada seleccionado
          u8g.drawTriangle(90, 19, 90, 27, 95, 23);
          break;
        case 3:// no hay nada seleccionado
          u8g.drawTriangle(90, 29, 90, 37, 95, 33);
          break;
        case 4:// no hay nada seleccionado
          u8g.drawTriangle(90, 39, 90, 47, 95, 43);
          break;

        default:
          break;
      }


    } while ( u8g.nextPage() );

  } while ((millis() - t) < 5000);


  c = 0;
  DisplayCLS(); delay(100);

  if ( CambioVAL == true) {
    CambioVAL = false;
    //aca mando la info para LOS NANOS
    Serial.print("D"); Serial.println(diffCODD);
    u8g.firstPage(); do {
      u8g.drawStr( 0 , 36, "Aplicando Cambios... ");
    } while ( u8g.nextPage() ); delay(2000);
  }

  u8g.firstPage(); do {
    MostrarINFO();
  } while ( u8g.nextPage() );
}

void Interrup() {
  //  btnSelec = digitalRead(3);
  //  long t = micros();
  //  u8g.firstPage();
  //      do {
  //        MostrarINICIO();
  //      } while( u8g.nextPage() );
  //long t2 = micros();
  //Serial.print(t2-t);   Serial.print("  mmS "); Serial.println(millis());

}



