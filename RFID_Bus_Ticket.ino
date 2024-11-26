//#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

SoftwareSerial mySerial(12, A1);
Servo myservo1;
Servo myservo2;

//LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

LiquidCrystal_I2C lcd(0x27,  16, 2);

char tag1[] ="6C001D915DBD";
char tag2[] ="030023692960";
char tag3[] ="030022D29D6E";
char tag4[] ="030023757722";
char tag5[] ="39008F6B3DE0";
char tag;

char input[12];
int count = 0;

boolean flag1 = 0;
boolean flag2 = 0;
boolean flag3 = 0;
boolean flag4 = 0;
boolean flag5 = 0;

boolean flagP = 0;

boolean flag = 0;

int Mode = 0;

int Sw1 = 5;
int Sw2 = 6;
int Sw3 = 7;
int Sw4 = 8;

int SwP = A2;

int Motor = 11;

int Bal_1 = 1000;
int Bal_2 = 1000;
int Bal_3 = 1000;
int Bal_4 = 1000;
int Bal_5 = 1000;

int Add_Bal;

int Dis;

int Dis_1;
int Dis_2;
int Dis_3;
int Dis_4;
int Dis_5;

long Time;

int T_Dis;
int Price = 2;
int Bill;

int Rem_Bal;
int Rec_Bal;
int Che_Bal;

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);

  myservo2.attach(9);
  myservo1.attach(3);
  
  lcd.begin();
  lcd.setCursor(0,0); lcd.print(" RFID BASED BUS ");
  lcd.setCursor(0,1); lcd.print("BUS BILLING SYS.");

  delay(3000);
  
  myservo1.write(30); 
  myservo2.write(30);

  pinMode( Sw1, INPUT_PULLUP );
  pinMode( Sw2, INPUT_PULLUP );
  pinMode( Sw3, INPUT_PULLUP );
  pinMode( Sw4, INPUT_PULLUP ); 

  pinMode( 2, INPUT_PULLUP );

  pinMode( Motor, OUTPUT );
  pinMode( SwP, INPUT_PULLUP );

  attachInterrupt(0,RPM,RISING);

  Dis = 0;

  Serial.println("GSM Setup");
      
  mySerial.println("AT+CMGF=1");                        delay(300);
  mySerial.println("AT+CNMI=1,1,0,0,0");                delay(300);
  mySerial.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");     delay(300);
  mySerial.println("AT+CMGD=1,4");                      delay(300);
  mySerial.println("AT+CLIP=1\r\n");                    delay(300);
}

void RPM() { Dis++; Time = millis(); }

void loop(){
  /*Serial.print("  B1 "); Serial.print(Dis_1);
  Serial.print("  B2 "); Serial.print(Dis_2);
  Serial.print("  B3 "); Serial.print(Dis_3);
  Serial.print("  B4 "); Serial.print(Dis_4);
  Serial.print("  B5 "); Serial.print(Dis_5);
  Serial.println(" ");
  */
  if ( digitalRead(Sw1) == LOW) { Mode = !Mode; delay(1000); }
  if ( digitalRead(SwP) == LOW) { flagP = !flagP; delay(1000); }

  digitalWrite(Motor,flagP);

  if(flagP==0){lcd.setCursor(13,1); lcd.print("OFF");}
  else        {lcd.setCursor(13,1); lcd.print("ON");}

  if ( Mode == 1 ) { RM(); }
  if ( Mode == 0 ) { BM(); }

  //myservo1.detach();
 // myservo2.detach();
  
}

void BM(){
  lcd.setCursor(0,0); lcd.print("DISTANCE: ");  lcd.print(Dis);  lcd.print(" km  ");

  //if ( millis() - Time > 2000 ){ 
    lcd.setCursor(0,1); lcd.print("SCAN CARD       ");  
    ReadCard_M();
    ReadCard_S();
 //   }

 // else {
  //  lcd.setCursor(0,1); lcd.print("BUS RUNNING     ");  
 //   }
  
}

void RM(){
  ReadCard_M();
  
  if ( Add_Bal == 0 ){
    lcd.setCursor(0,0); lcd.print("RECHARGE MODE   ");
    lcd.setCursor(0,1); lcd.print("SELECT AMOUNT...");

    if ( flag1 == 1 ) { Bal_1 = Bal_1 + Add_Bal; flag1 = 0; Che_Bal =  Bal_1; Not2(); }
    if ( flag2 == 1 ) { Bal_2 = Bal_2 + Add_Bal; flag2 = 0; Che_Bal =  Bal_2; Not2(); }
    if ( flag3 == 1 ) { Bal_3 = Bal_3 + Add_Bal; flag3 = 0; Che_Bal =  Bal_3; Not2(); }
    if ( flag4 == 1 ) { Bal_4 = Bal_4 + Add_Bal; flag4 = 0; Che_Bal =  Bal_4; Not2(); }
    if ( flag5 == 1 ) { Bal_5 = Bal_5 + Add_Bal; flag5 = 0; Che_Bal =  Bal_5; Not2(); }  
    }

  else {
    lcd.setCursor(0,0); lcd.print("AMOUNT : ");  lcd.print(Add_Bal);  lcd.print(" ");
    lcd.setCursor(0,1); lcd.print("SCAN CARD       ");

    if ( flag1 == 1 ) { Bal_1 = Bal_1 + Add_Bal; flag1 = 0; Rec_Bal = Bal_1; Not1(); }
    if ( flag2 == 1 ) { Bal_2 = Bal_2 + Add_Bal; flag2 = 0; Rec_Bal = Bal_2; Not1(); }
    if ( flag3 == 1 ) { Bal_3 = Bal_3 + Add_Bal; flag3 = 0; Rec_Bal = Bal_3; Not1(); }
    if ( flag4 == 1 ) { Bal_4 = Bal_4 + Add_Bal; flag4 = 0; Rec_Bal = Bal_4; Not1(); }
    if ( flag5 == 1 ) { Bal_5 = Bal_5 + Add_Bal; flag5 = 0; Rec_Bal = Bal_5; Not1(); }
    }
    
    if      ( digitalRead(Sw2) == LOW) { Add_Bal = 1000; }
    else if ( digitalRead(Sw3) == LOW) { Add_Bal = 2000; }
    else if ( digitalRead(Sw4) == LOW) { Add_Bal = 5000; }
    //else                               { Add_Bal = 0;    }
  
  }
  
void ReadCard_M() {
  if(Serial.available()){
    count = 0;
    
    while(Serial.available() && count < 12){  
      Serial.println( input[count] );
      
      input[count] = Serial.read();
      count++;
      delay(5);
      }
    
    if(count == 12){
      Serial.println(input);
      count =0;
      flag = 1;
      
      while(count<12 && flag !=0){
        if(input[count] == tag1[count]) { flag1 = 1; } else { flag1 = 0; }
        if(input[count] == tag2[count]) { flag2 = 1; } else { flag2 = 0; }
        if(input[count] == tag3[count]) { flag3 = 1; } else { flag3 = 0; }
        if(input[count] == tag4[count]) { flag4 = 1; } else { flag4 = 0; }
        if(input[count] == tag5[count]) { flag5 = 1; } else { flag5 = 0; }
        count++;
        }
      }

    if ( Mode == 0 ) {  
      //myservo1.attach(3); delay(2000);
      myservo1.write(120);
      delay(2000);
      lcd.setCursor(0,0); lcd.print("ENTRY...         ");
      lcd.setCursor(0,1); lcd.print("BAL. = "); 
          
      if(flag1 == 1 ){ Serial.println("Card1 "); Dis_1 = Dis; lcd.print(Bal_1); lcd.print(" INR "); SendSMS(1,0);}
      if(flag2 == 1 ){ Serial.println("Card2 "); Dis_2 = Dis; lcd.print(Bal_2); lcd.print(" INR "); SendSMS(2,0);}
      if(flag3 == 1 ){ Serial.println("Card3 "); Dis_3 = Dis; lcd.print(Bal_3); lcd.print(" INR "); SendSMS(3,0);}
      if(flag4 == 1 ){ Serial.println("Card4 "); Dis_4 = Dis; lcd.print(Bal_4); lcd.print(" INR "); SendSMS(4,0);}
      if(flag5 == 1 ){ Serial.println("Card5 "); Dis_5 = Dis; lcd.print(Bal_5); lcd.print(" INR "); }

      myservo1.write(30);       
      delay(2000); 
     // myservo1.detach();
    }
    
    for( int i=0; i<12; i++) { input[i]= 'F'; }
    count = 0;
    }   
  }

void ReadCard_S() {
  if(mySerial.available()){
    count = 0;
    
    while(mySerial.available() && count < 12){
      input[count] = mySerial.read();
      count++;
      delay(5);
      }
    
    if(count == 12){
      Serial.println(input);
      count =0;
      flag = 1;
      
      while(count<12 && flag !=0){
        if(input[count] == tag1[count]) { flag1 = 1; } else { flag1 = 0; }
        if(input[count] == tag2[count]) { flag2 = 1; } else { flag2 = 0; }
        if(input[count] == tag3[count]) { flag3 = 1; } else { flag3 = 0; }
        if(input[count] == tag4[count]) { flag4 = 1; } else { flag4 = 0; }
        if(input[count] == tag5[count]) { flag5 = 1; } else { flag5 = 0; }
        count++;
        }
      }

    if(flag1 == 1){ Serial.println("Card1 "); }
    if(flag2 == 1){ Serial.println("Card2 "); }
    if(flag3 == 1){ Serial.println("Card3 "); }
    if(flag4 == 1){ Serial.println("Card4 "); }
    if(flag5 == 1){ Serial.println("Card5 "); }

    if ( Mode == 0 ) {
      lcd.setCursor(0,0); lcd.print("EXIT...          ");
      //myservo2.attach(11);  delay(2000);
      myservo2.write(120);
      delay(2000);
      
      if(flag1 == 1 ){ T_Dis = Dis - Dis_1; Bill = T_Dis*Price; Bal_1 = Bal_1 - Bill; Dis_1 = Dis; Rem_Bal = Bal_1; SendSMS(1,1);}
      if(flag2 == 1 ){ T_Dis = Dis - Dis_2; Bill = T_Dis*Price; Bal_2 = Bal_2 - Bill; Dis_2 = Dis; Rem_Bal = Bal_2; SendSMS(2,1);}
      if(flag3 == 1 ){ T_Dis = Dis - Dis_3; Bill = T_Dis*Price; Bal_3 = Bal_3 - Bill; Dis_3 = Dis; Rem_Bal = Bal_3; SendSMS(3,1);}
      if(flag4 == 1 ){ T_Dis = Dis - Dis_4; Bill = T_Dis*Price; Bal_4 = Bal_4 - Bill; Dis_4 = Dis; Rem_Bal = Bal_4; SendSMS(4,1);}
      if(flag5 == 1 ){ T_Dis = Dis - Dis_5; Bill = T_Dis*Price; Bal_5 = Bal_5 - Bill; Dis_5 = Dis; Rem_Bal = Bal_5; }

      lcd.setCursor(0,0); lcd.print("REM BAL= "); lcd.print(Rem_Bal); lcd.print(" INR ");
      
      lcd.setCursor(0,1); lcd.print("D= ");  lcd.print(T_Dis);  lcd.print(" Km "); 
      lcd.setCursor(8,1); lcd.print("B= ");  lcd.print(Bill);   lcd.print(" INR "); 

      myservo2.write(30); delay(2000); 
      //myservo2.detach();
    }
    
    for( int i=0; i<12; i++) { input[i]= 'F'; }
    count = 0;
    }   
  }

void Not1(){
  Serial.println("Notification 1 ");
  lcd.setCursor(0,0); lcd.print("RECHARGE SUCESS "); 
  lcd.setCursor(0,1); lcd.print("BALANCE : "); lcd.print(Rec_Bal); lcd.print("  ");
  
  Add_Bal = 0; 

  delay(2000);
  }

void Not2(){
  Serial.println("Notification 2 ");
  lcd.setCursor(0,1); lcd.print("BALANCE : "); lcd.print(Che_Bal); lcd.print("  ");   
  Add_Bal = 0; 

  delay(2000);
  }
