

//#include "floatToString.h" 
#include "U8glib.h"
//ARDUINO MEGA
U8GLIB_ST7920_128X64 u8g(52, 51, 50, U8G_PIN_NONE);//Enable 6, R/W 5, RS 4, RESET



void setup() {
  // initialize both serial ports:
  Serial.begin(9600); //BLUETOOTH / WIFI
  Serial1.begin(9600);//NANO CODO / RODILLA DERECHA
  Serial2.begin(9600);//NANO CODO / RODILLA IZQUIERDA
  Serial3.begin(9600);//Display

  char buffer[25]; 
//Serial.println(floatToString(buffer, 100.321, 5));

//INICIO EL DISPLAY 64*128 7920
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
    u8g.setHiColorByRGB(255,255,255);
  } 

  u8g.firstPage();  
      do {
        MostrarINICIO();
      } while( u8g.nextPage() );

Serial.println("INICIO MEGA Central");
  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


long TM = millis();

void loop() {

/*
 * COMM LADO DERECHO
 * PIDO INFO AL NANO CODO Y RODILLA serial1
 * ALERTO AL NANO DEL DISPLAY
 * aguardo respuesta (la respuesta que espero es ej: C25;102;55; CODO;ANGULO;KG/F;DIFICULTAD
 * envio al nano display
 * ENVIO AL BLUETOOHT / WIFI
*/

  Serial1.println("I"); //PIDO INFO AL CODO Y RODILLA DERECHA
  Serial3.println("CRD"); // LE INDICO AL NANO DISPLAY QUE ESPERE INFO 
  long t = millis();
  String SCodoD ="";
  //AGUARDO RESPUESTA
  delay(20);//aguardo a que llegue algun dato
  do{
    if (Serial1.available()) {delay(5);
      int inByte = Serial1.read(); 
      if( inByte != '*' ){SCodoD += char(inByte);} //reemplazar el '*' x '\n'
      else{
        //ENVIO AL DISPLAY
        Serial3.println(SCodoD); 
        //ENVIO AL BLUETOOTH / WIFI
        Serial.println(SCodoD);
        break;}
      }
  }while ((millis() - t) < 1000);


/*
 * COMM LADO IZQUIERDO
 * PIDO INFO AL NANO CODO Y RODILLA serial2
 * ALERTO AL NANO DEL DISPLAY
 * aguardo respuesta (la respuesta que espero es ej: C25;102;55; CODO;ANGULO;KG/F;DIFICULTAD
 * envio al nano display
 * ENVIO AL BLUETOOHT / WIFI
*/

  Serial2.println("CRI"); //PIDO INFO AL CODO Y RODILLA DERECHA
  Serial3.println("CRI"); // LE INDICO AL NANO DISPLAY QUE ESPERE INFO 
  //long t = millis();
  String SCodoI ="";
  //AGUARDO RESPUESTA
  do{
    
    if (Serial2.available()) {delay(5);
      int inByte = Serial2.read(); delay(5);      
      if( inByte != '*' ){SCodoI += char(inByte);} 
      else{
        //ENVIO AL DISPLAY
        Serial3.println(SCodoI); 
        //ENVIO AL BLUETOOTH / WIFI
        Serial.println(SCodoI);
        break;}
        
      }
  }while ((millis() - t) < 1000);



//CHEQUEO SI ENTRA INFO DESDE EL NANO DISPLAY HACIA EL NANO DERECHO
    if (Serial3.available()) {delay(5);
        int inByte = Serial3.read(); delay(5);
        if( inByte == '/') {
        String DifC =""; String DifR ="";
         //do{
          int inByte = Serial3.read(); 
          if( inByte == 'C') { do { inByte = Serial3.read(); if(inByte == '\n'){break;} DifC =+ inByte; delay(5);}while ((millis() - t) < 150);          
          }

          if( inByte == 'R') {         
          }
         //}while ((millis() - t) < 1000);
        
      }
      }






  
//  // read from port 1, send to port 0:
//  if (Serial3.available()) {
//    int inByte = Serial3.read(); rxBI += char(inByte);
//    Serial.write(inByte);
//    //BrazoDT(String(rxBI));
//  }
//
//  // read from port 0, send to port 1:
//  if (Serial.available()) {
//    int inByte = Serial.read(); 
//    Serial3.write(inByte);
////    rxBI += char(inByte);
////      u8g.firstPage();  
////      do {
////        BrazoDT(String(rxBI));
////        //rxBI = "";
////      } while( u8g.nextPage() );
//    
//  }
//
//      if((millis() - tm)>250){ 
//          u8g.firstPage();  
//          do {
////            BrazoDT(String(rxBI));
//          } while(u8g.nextPage()); 
////          rxBI = "";
//      }
//

  
}

void BrazoDT(String txt){
              //u8g.setFont(u8g_font_unifont);
              //u8g.setFont(u8g_font_osb21);
              //u8g.setFont(u8g_font_04b_03);
              //u8g.setFont(u8g_font_fub17);
              u8g.setFont(u8g_font_6x10);
              //u8g.setFont(u8g_font_fur11n);
              u8g.setPrintPos( 0,  7); u8g.print("DT BI: "); u8g.print(txt);
              //u8g.setPrintPos( 0, 45);  u8g.print(txt);
              
//    u8g.firstPage();  
//      do {
//        BrazoDT(String(rxBI));
//      } while(u8g.nextPage()); //rxBI = "";

}


void MostrarINICIO(){
              //u8g.setFont(u8g_font_unifont);
              //u8g.setFont(u8g_font_osb21);
              //u8g.setFont(u8g_font_04b_03);
              u8g.setFont(u8g_font_fub17);
              //u8g.setFont(u8g_font_6x10);
              //u8g.setFont(u8g_font_fur11n);
              u8g.setPrintPos( 10,  21); u8g.print("MARNICH");
              u8g.setPrintPos( 0, 45);  u8g.print("EFFICTRON");
              

}





//MUESTRO LOS DATOS EN EL DISPLAY----------------------------------------------------
//void MostrarDatos(){
////EN EL DISPLAY
//
//              u8g.setFont(u8g_font_04b_03);
//              u8g.setPrintPos( 63,  6); u8g.print("SET POINT   "); u8g.print(psetT); 
//              u8g.setPrintPos( 63, 12);  
//              if(OffsetPrint == true){ u8g.print("OFFSET *C");} else{u8g.print("TEMP. *C");}
//              u8g.setPrintPos( 63, 38); u8g.print("SET POINT   "); u8g.print(psetH); 
//              u8g.setPrintPos( 63, 44); 
//              if(OffsetPrint == true){ u8g.print("OFFSET H. REL.");} else{u8g.print("% H. REL.");}
//              
//              u8g.setPrintPos( 0, 5); u8g.print("50*C");
//              u8g.setPrintPos( 0, 30); u8g.print("0*C");
//              u8g.setPrintPos( 0, 38); u8g.print("100%");
//              u8g.setPrintPos( 0, 63); u8g.print("0%");
//               
//              
//              u8g.setFont(u8g_font_fub17);//u8g.setFont(u8g_font_freedoomr25n);//u8g.setFont(u8g_font_freedoomr10r);//u8g.setFont(u8g_font_profont22);
//              u8g.setPrintPos( 82, 30);
//              u8g.print(tA,1);
//              
//              u8g.drawLine(0, 31, 128, 31);//linea horizontal
//              u8g.drawLine(61, 0, 61, 64);//linea vertical
//              u8g.drawLine(15, 30, 15, 32);//linea vertical
//              u8g.drawLine(30, 29, 30, 33);//linea vertical
//              u8g.drawLine(45, 30, 45, 32);//linea vertical
//              u8g.drawLine(60, 30, 60, 32);//linea vertical
//              u8g.setPrintPos( 82, 63);
//              u8g.print(hA,1);
//
//              if(ReleT == true){u8g.drawDisc(70, 24, 5);} else{u8g.drawCircle(70, 24, 5);}
//              if(ReleH == true){u8g.drawDisc(70, 57, 5);} else{u8g.drawCircle(70, 57, 5);}
//              if(Punto == true){u8g.drawPixel(127,63);}//else{u8g.drawCircle(70, 57, 5);}
//
//              
//// ACA HAGO EL GRAFICO
//
//              GT[60] = map(tA,0,50,31,0);//grafico temperatura
//              u8g.drawPixel(60,GT[60]);
//              for(int x = 1; x < 61; x++){ if(GT[x] > 1 && GT[x] < 51 ){ u8g.drawPixel(x,GT[x]);}}
//              
//              GH[60] = map(hA,0,100,64,31);//grafico humedad
//              u8g.drawPixel(60,GH[60]);
//              for(int z = 1; z < 61; z++){ if(GH[z] > 1 && GH[z] < 101 ){u8g.drawPixel(z,GH[z]);}}
//
//
//          if(Graficar == true){ 
//              //re ordeno los datos
//              Graficar = false;
//              for(int x = 1; x < 61; x++){GT[x] = GT[x+1];/*Serial.print(" x "); Serial.print(x); Serial.print(" GT[x] "); Serial.println(GT[x]);*/   }
//              for(int z = 1; z < 61; z++){GH[z] = GH[z+1];/*Serial.print(" x "); Serial.print(z); Serial.print(" GH[x] "); Serial.println(GH[z]);*/   }
//              
//          }
//}


//--------------------------------------------------------------------------------









//MUESTRO EL CURSOR---------------------------------------------------------------------
void Cursor(int c){
  //Serial.print(" Cursor "); Serial.println(c);
  switch (c){
    case 0:// no hay nada seleccionado
      //u8g.drawBox(10,12,20,30);
    break;
    case 1:// Selecciona SetPoint Temperatura
          u8g.firstPage();
          do {
            u8g.drawRFrame(110,0,18,8,3);//x,y,w,h,radio
//            MostrarDatos();
          } while( u8g.nextPage() );

          //u8g.setPrintPos( 63,  6); u8g.print("SET POINT  "); u8g.print(psetT,1); 
          //Serial.print(" Valor "); Serial.println(c);
    break;
    
    case 2:// Selecciona SetPoint Humedad
          u8g.firstPage();
          do {
            u8g.drawRFrame(110,32,18,8,3);//x,y,w,h,radio
//            MostrarDatos();
          } while( u8g.nextPage() );
          
          //u8g.setPrintPos( 63, 38); u8g.print("SET POINT  "); u8g.print(psetH,1); 
          //Serial.print(" Valor "); Serial.println(c);
    break;

    default:

    break;
  }
  
}
