#include "U8glib.h"
//U8GLIB_ST7920_128X64_1X u8g(13, 12, 11);  // SPI Com: SCK = en = 13, MOSI = rw = 12, CS = di = 11
U8GLIB_ST7920_128X64 u8g(52, 51, 50, U8G_PIN_NONE);//Enable 6, R/W 5, RS 4, RESET


bool boton = false;
int Decomp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(13, INPUT_PULLUP);
  //pinMode(12, INPUT_PULLUP);
  //pinMode(11, INPUT_PULLUP);
  //pinMode(10, OUTPUT);digitalWrite(10,LOW);
  pinMode(47, OUTPUT);digitalWrite(47,HIGH);
  pinMode(46, OUTPUT);digitalWrite(46,LOW);

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
  delay(2000);
  //int t2 = millis();
  //Serial.println(t2-t);
//  delay(2000);
//  u8g.firstPage();
//  do {
//    MostrarINFO();
//  } while ( u8g.nextPage() );

}

long t=0;
void loop() {
  // put your main code here, to run repeatedly:
  
  delay(250);
  MostrarINFO();



  if(digitalRead(13) == 0 ){boton = true; t = millis(); }//digitalWrite(46,HIGH);
    Decomp = analogRead(0)*3;
    do{
      delay(250);
      
       Decomp = Decomp -40;
      if( Decomp < 0) {Decomp =40; digitalWrite(46,HIGH);digitalWrite(47,LOW); MostrarINFO();  delay(5000); }
      
     //if(Decomp <=40 ){}
      MostrarINFO(); 
        Serial1.println("X0.5 Y1F100");
        Serial1.println("X0.0 Y0.5F100");
    }while((millis() - t) < 10000);
    boton = false;
    //Decomp =40; 
    digitalWrite(47,HIGH);digitalWrite(46,LOW);
  
   
  //do { } while ((millis() - t) < 2000);
  Serial1.println("X0.5 Y1F100");
  Serial1.println("X0.0 Y0.5F100");


}

//IMPACTO
//CREATIVIDAD
//QUE TA \N
//ROSARIOGAMEDEVS

void MostrarINFO() {

    int VInt = analogRead(0)*3;
    //int VExt = analogRead(5)3;
  u8g.firstPage();  
  do {
  
    
  u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_fur11n);
  u8g.setPrintPos( 0,  7); u8g.print("PRESION INTERNA:");   //HASTA 21 CHAR
  
  if(boton){
    u8g.setPrintPos( 0, 17); u8g.print(Decomp); u8g.print(" hPa");} //u8g.print(VInt);// u8g.print("Comp EXT. = "); u8g.print(VExt);}
  else{ 
    u8g.setPrintPos( 0, 17); u8g.print(VInt); u8g.print(" hPa");} //u8g.print(VInt);// u8g.print("Comp EXT. = "); u8g.print(VExt); }

  if(boton){
    u8g.setPrintPos( 0, 47);  u8g.print("Pedido de acceso:");// u8g.print(!digitalRead(13));
    u8g.setPrintPos( 0, 57);  u8g.print("Igualando presiones");// u8g.print(!digitalRead(13));
  }
  //u8g.setPrintPos( 0, 37);  u8g.print("Boton 2 = "); u8g.print(!digitalRead(12));
  //u8g.setPrintPos( 0, 47);  u8g.print("Boton 3 = "); u8g.print(!digitalRead(11));
  
  
  //u8g.setPrintPos( 0, 27);  u8g.print("Comp INT. = ");
  //u8g.setPrintPos( 0, 37);  u8g.print("COD D");
  //u8g.setPrintPos( 0, 47);  u8g.print("ROD D");
  //u8g.setPrintPos( 0, 64);  u8g.print("12345678901234567890123456");
  // MostrarINFO();
  } while ( u8g.nextPage() );
  
}

void MostrarINICIO() {
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g.setFont(u8g_font_04b_03);
  u8g.setFont(u8g_font_fub17);
  //u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_fur11n);
  u8g.setPrintPos( 25,  20); u8g.print("P"); u8g.print(char(250)); u8g.print("lsAR");
  u8g.setPrintPos( 35, 48);  u8g.print("I + D");

 //delay(1000);
}
