void SendSMS(int Num, int MSG){
  myservo1.detach();
  myservo2.detach();

  Serial.println("Sending SMS");
  
  mySerial.println("AT");                            delay(1000);
  mySerial.println("AT+CMGF=1");                            delay(1000);
  if(Num==1) {mySerial.println("AT+CMGS=\"+917219038167\"\r");          delay(1000);}
  if(Num==2) {mySerial.println("AT+CMGS=\"+918669274527\"\r");          delay(1000);}
  if(Num==3) {mySerial.println("AT+CMGS=\"+919359470245\"\r");          delay(1000);}
  if(Num==4) {mySerial.println("AT+CMGS=\"+919552820156\"\r");          delay(1000);}

  
  if(MSG==0) {
    mySerial.println("HAPPY JOURNEY");                      delay(100);
  }

  if(MSG==1) {
    mySerial.println("Thank You");                          delay(100);
    mySerial.print("Travelled Distance : ");                delay(100);
    mySerial.println(T_Dis);                                delay(100);
    mySerial.print("Your Bill : ");                         delay(100);
    mySerial.println(Bill);                                 delay(100);
    mySerial.print("Remaining Balance : ");                 delay(100);
    mySerial.print(Rem_Bal);                                delay(100);
    mySerial.println();                                     delay(100);
  }
  mySerial.println((char)26);                               delay(1000);

  myservo2.attach(9);
  myservo1.attach(3);
}
