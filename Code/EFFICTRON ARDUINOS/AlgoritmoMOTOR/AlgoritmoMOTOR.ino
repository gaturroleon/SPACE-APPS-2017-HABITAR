
boolean stringComplete = false;  // whether the string is complete
String inputString = "";         // a string to hold incoming data
int vkg = 0;
int diff = 50;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("INICIO");
}

void loop() {
  // put your main code here, to run repeatedly:
      if (stringComplete) {
          //diff = map(inputString.toInt(),0,100,0,100);
          diff = inputString.toInt();
          Serial.println(diff);
          stringComplete = false;
          inputString = "";
      }

      operar(diff);
}

void operar(int d){
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


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if(inChar != '*'){inputString += inChar;}
    //Serial.print(inChar);
    if ((inChar == '*') || (inChar == '\n')) {
      stringComplete = true;
    }
  }
}


void ApagarMotor(){
  //Serial.println("apagado");
        //display.clearDisplay();display.setCursor(60,0); 
        //display.setTextSize(1);
        //display.println("APG:");
        //display.display();
  pinMode(10,OUTPUT); digitalWrite(10,LOW);//pwm para el motor
  pinMode(11,OUTPUT); digitalWrite(11,LOW);//pwm para el motor
}

void Avanzar(int vel){
  if(vel > 255){vel = 255;}
  //Serial.println("AVZ");
        //display.clearDisplay();display.setCursor(60,0); 
        //display.setTextSize(1);
        //display.println("AVZ:");
        //display.display();
  pinMode(10,OUTPUT); digitalWrite(10,LOW);//pwm para el motor
  pinMode(11,OUTPUT); analogWrite(11,vel);//pwm para el motor

}


void Retroceder(int vel){
  if(vel < -255){vel = -255;}
  //Serial.print("RTD");
  //Serial.println(vel);
        //display.clearDisplay();display.setCursor(60,0); 
        //display.setTextSize(1);
        //display.println("RTD:");
        //display.display();
  pinMode(11,OUTPUT); digitalWrite(11,LOW);//pwm para el motor
  pinMode(10,OUTPUT); analogWrite(10,abs(vel));//pwm para el motor
}
