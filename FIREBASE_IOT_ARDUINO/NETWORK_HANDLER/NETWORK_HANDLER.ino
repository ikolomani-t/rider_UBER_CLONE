#include <Wire.h>
#include <EEPROM.h>
#include <time.h>
#include <ctype.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <Arduino_JSON.h>
#include <assert.h>
#include<avr/wdt.h>


#define rxPin 10
#define txPin 11
//SoftwareSerial Serial(rxPin,txPin);
AltSoftSerial mySerial;

const String APN  = "safaricom";
const String USER = "saf";
const String PASS = "data";
String major;
String transfer;

//const String FIREBASE_HOST  = "https://vika-travel.firebaseio.com/";
//const String FIREBASE_SECRET  = "Y7makprFANJGaNnjQuvlUOD5o68ugE6TGfv2JzNh";

const String FIREBASE_HOST  = "https://vika-travel-default-rtdb.firebaseio.com/RidersInformation/CABIN/";   //RidersInformation/
const String FIREBASE_SECRET  = "Z9syk8noQlbI9v9jHRenIWfv65rYLvFO2u6Run75";

const String FIREBASE_HOST2  = "https://vika-travel-default-rtdb.firebaseio.com/RidersInformation/RETA/"; 


void init_gsm();
void gprs_connect();
boolean gprs_disconnect();
int is_gprs_connected();
void post_to_firebase(String data);
char* waitResponse(char response[80]="OK", unsigned int timeout=2000);//*/

#define USE_SSL true
#define DELAY_MS 800

 char tr[32];
char tre[64];
char tre2[64];
char tre3[64];
char mak[10];
char mak2[10];
char mak3[10];
char mak4[30];
String dat;
String dat2;
String h="testing cabin";
String kickdata="";
long yu,ya;

long intial, address;
 double j;
 double b;
 double w;
 double k;
 double p;
 int tu=0;



void setup() {
 Wire.begin(1);
 //Wire.begin(9);
pinMode(44,OUTPUT);
strcpy(tre,"");
strcpy(tr,"");
strcpy(mak,"");
strcpy(mak2,"");
strcpy(mak3,"");
strcpy(mak4,"");
major="";
 dat = "{";
 dat += "\"humidity\":\"" + h + "\""; 
 dat += "}";
 yu=0;
 ya=millis();
 digitalWrite(44, HIGH);
     j=millis();
     b=millis();
     k=millis();
     w=millis();
     p=millis();
  Wire.onRequest(requestEvent);
  mySerial.begin(19200);
  Serial.begin(19200);
  //Serial.begin(19200); 
  //Serial.println("Initializing mySerial...");
  delay(1000);
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial4();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  //updateSerial4();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  //updateSerial4();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  //updateSerial4();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  //updateSerial4();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  //updateSerial4();

  delay(1000);
  Serial.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial5();
  Serial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  //updateSerial5();
  Serial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  //updateSerial5();
  Serial.println("AT+CREG?"); //Check whether it has registered in the network
  //updateSerial5();
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  //updateSerial5();
  Serial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  //updateSerial5();
  wdt_disable(); //Disable WDT
  delay(1000);
  wdt_enable(WDTO_4S); //Enable WDT with a timeout of 4 seconds

  init_gsm();

}

void loop(){
  wdt_reset();
  post_to_firebase(dat);

  updateSerial6();
}


void updateSerial()
{
while(mySerial.available()) {
  wdt_reset();
    Serial.write(mySerial.read());
  }

}

void updateSerial6()
{
  while(Serial.available())
  {
    wdt_reset();
    String res = Serial.readString();
    major=res;
    updateSerial2(major);
  // Serial.println(res);       
  }
  
}


void updateSerial4() {
  while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while(Serial.available()) {
    mySerial.write(Serial.read());
  }
  delay(100);
}

void updateSerial5() {
  while(Serial.available()) {
    Serial.write(Serial.read());
  }
  while(Serial.available()) {
    Serial.write(Serial.read());
  }
  delay(100);
}

void updateSerial3()
{
  char sto[100];
  char br[100];
  strcpy(br,"");
  while (Serial.available()) 
  {
     wdt_reset();
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
     wdt_reset();
   
        Serial.write(mySerial.read());
        int d=mySerial.read();
        sprintf(br,"%c",d);
        if(isascii(d)!=0){
        strcat(sto,br);  
        }     
  }
  if(strcmp(sto,"")!=0)
  {
    
  
   Serial.println(sto);
  }
   strcpy(sto,"");
}


void init_gsm()
{
  mySerial.println("AT");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);

  mySerial.println("AT+CMGF=1");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
 mySerial.println("AT+CNMI=2,2,0,0,0");
 //updateSerial();
  wdt_reset();
 delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM  AT+CPIN="3053”
  //Checks if the SIM is ready
  mySerial.println("AT+CPIN?");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);

   mySerial.println("AT+CPIN=\"8183\"");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Turning ON full functionality
  mySerial.println("AT+CFUN=1");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Turn ON verbose error codes
  mySerial.println("AT+CMEE=2");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Enable battery checks
  mySerial.println("AT+CBATCHK=1");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  //screen(0,15,waitResponse());

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  mySerial.println("AT+CREG?");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //setting SMS text mode
  mySerial.print("AT+CMGF=1\r");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}


int is_gprs_connected()
{
  static int ab=0;
  mySerial.println("AT+CGATT?");
  
  if(strcmp(waitResponse("+CGATT: 1",6000),"1") != 0) 
  { 
    ab=0;

   }else{
     ab=1;
   }
  return ab;
}

char* waitResponse(char expected_answer[80], unsigned int timeout)
{
  static char answer[10];
  strcpy(answer,"0");
  char response[80];
  unsigned long previous;
    strcpy(response,"");
  
    while(mySerial.available()){
        wdt_reset();
        int c = mySerial.read();
        sprintf(tr,"%c",c);
        strcat(response,tr);
    }
    if(strstr(response,expected_answer)!= NULL){
        strcpy(answer, "1");
        }
   major.concat(response);
   //updateSerial2(major);
  strcpy(response,"");
  return answer;
}


void post_to_firebase(String data)
{
  //Start HTTP connection
  mySerial.println("AT+HTTPINIT");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);

  //Enabling SSL 1.0
  if(USE_SSL == true){
    mySerial.println("AT+HTTPSSL=1");
    //updateSerial();
     wdt_reset();
    delay(DELAY_MS);
  }
  
  //Setting up parameters for HTTP session
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Set the HTTP URL - Firebase URL and FIREBASE SECRET
  mySerial.println("AT+HTTPPARA=\"URL\","+FIREBASE_HOST+".json?auth="+FIREBASE_SECRET);
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  
  //Setting up re direct
  mySerial.println("AT+HTTPPARA=\"REDIR\",0");//0
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Setting up content type
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Sending HTTP POST request
  mySerial.println("AT+HTTPACTION=0");
  w=millis();

    while(!mySerial.available())
    {
       wdt_reset();
    if(millis()-w > 30000){
      w=millis();
       gprs_disconnect();
    delay(100);
    gprs_connect();
      break;
    }

    }
    wdt_disable();
    delay(3000);
    wdt_enable(WDTO_4S);
    wdt_reset();
    String response = mySerial.readString();

    if(response.indexOf("+HTTPACTION:") > 0)
    {
       //Serial.println(response);

      if(!(response.indexOf("601") > 0))
      {
        k=millis();
      }
      if(!(response.indexOf("200") > 0) && (millis()-k > 60000) )
      {
      digitalWrite(44, LOW);
      delay(50);
      digitalWrite(44, HIGH);
      delay(50);
      k=millis();
      }
      
    }

     if(response.indexOf("SIM not inserted")>0)
      {
      digitalWrite(44, LOW);
      delay(80);
      digitalWrite(44, HIGH);
      delay(80);
      }


      if(response.indexOf("ERROR")>0)
      {
      digitalWrite(44, LOW);
      delay(80);
      digitalWrite(44, HIGH);
      delay(80);
      }

   wdt_reset();
  delay(DELAY_MS);
  
  mySerial.println("AT+HTTPREAD");
    w=millis();
    while(!mySerial.available())
    {
       wdt_reset();
    if(millis()-w > 30000){
      w=millis();
      break;
    }

    }

    response = mySerial.readString();
     char buf2 [response.length()+1];
     char buf50 [response.length()+33];
    strcpy(buf2,"");
    strcpy(buf50,"");
    if(response.indexOf("+HTTPREAD:") > 0)
    {
      ya=millis();
     response.toCharArray(buf2, (response.length()+1));
     kickdata="";
     int q=0;
     int r=27;
     strcpy(buf50,"bbbbbbbbbbbbbbbbbaaaaaaaaffffdds ");
     strcat(buf50,buf2);
     //Serial.println(buf50);
      if(!(kickdata.indexOf("error")>0)){
     }
     //Serial.println(response.length()+1);
     //Serial.println(strlen(buf50));
     char wx[20];
     strcpy(wx,"");
     int wy= (strlen(buf50)/31);
      //Serial.println(wy);
       if((strlen(buf50)%31)>0)
     {
      wy=wy+1;
      //Serial.println(wy);
     }
     String temp_str = String(strlen(buf50));
     temp_str.toCharArray(wx,20);
     strcat(wx, "|");
     wdt_disable();
     delay(80);
     wdt_enable(WDTO_4S);
     wdt_reset();
     //Serial.println(wx);
     int zx=0;
     int dy=0;

    
     while( zx < wy)
     {
        wdt_reset();
        char buffer[strlen(buf50)+10];
        strcpy(buffer,"");
        strcpy(buffer,"#");
        strcat(buffer,buf50+dy);
     trans(2,buffer);
     dy=dy+31;
     zx=zx+1;
     //delay(100);
     //Serial.println(buffer);
     }
    
        trans(2," = ");
      wdt_reset();
      delay(DELAY_MS);
      if(!(kickdata.indexOf("error")>0)){
     }
    }else{
      gprs_connect();
    }
  
  //strcpy(buf50,"");
}


  void trans(int address, char data[])
{
  wdt_reset();
Wire.beginTransmission(address); 
  Wire.write(data);      
  Wire.endTransmission(); 
  delay(100);
  }


void post_to_firebase2(String data)
{
  
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Start HTTP connection
  mySerial.println("AT+HTTPINIT");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Enabling SSL 1.0
  if(USE_SSL == true){
    mySerial.println("AT+HTTPSSL=1");
    //updateSerial();
    wdt_reset();
    delay(DELAY_MS);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up parameters for HTTP session
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Set the HTTP URL - Firebase URL and FIREBASE SECRET
  mySerial.println("AT+HTTPPARA=\"URL\","+FIREBASE_HOST2+".json?auth="+FIREBASE_SECRET);
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up re direct
  mySerial.println("AT+HTTPPARA=\"REDIR\",1");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up content type
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up Data Size
  //+HTTPACTION: 1,601,0 - error occurs if data length is not correct
  
  mySerial.println("AT+HTTPDATA=" + String(data.length()) + ",10000");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Sending Data
  //mySerial.println(data);
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
   //*/
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Sending HTTP POST request
  mySerial.println("AT+HTTPACTION=1");
  w=millis();
  for (uint32_t start = millis(); millis() - start < 20000;){
    wdt_reset();
    while(!mySerial.available())
    {
    wdt_reset();
    if(millis()-w > 30000){
      w=millis();
      break;
    }

    }
    String response2 = mySerial.readString();
    if(response2.indexOf("+HTTPACTION:") > 0)
    {
      //Serial.println(response2);
      //break;
    }
  }
    wdt_reset();
  delay(DELAY_MS);
  
  //+HTTPACTION: 1,603,0 (POST to Firebase failed)
  //+HTTPACTION: 0,200,0 (POST to Firebase successfull)
  //Read the response
  mySerial.println("AT+HTTPREAD");
  wdt_reset();
  delay(100);
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  
  //Stop HTTP connection
  /*mySerial.println("AT+HTTPTERM");
  updateSerial();
  delay(DELAY_MS);//*/
}

boolean gprs_disconnect()
{
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Disconnect GPRS
  mySerial.println("AT+CGATT=0");
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  return true;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Connect to the internet
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void gprs_connect()
{
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //DISABLE GPRS
 /* mySerial.println("AT+SAPBR=0,1");
  updateSerial();
   wdt_reset();
  delay(DELAY_MS);//*/
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Connecting to GPRS: GPRS - bearer profile 1
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
 //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //sets the APN settings for your sim card network provider.
  mySerial.println("AT+CSTT=\"safaricom\",\"saf\",\"data\"");
 //updateSerial();
 wdt_reset();
  delay(DELAY_MS);
  
    mySerial.println("AT+SAPBR=1,1");
    //updateSerial();
     wdt_reset();
  delay(DELAY_MS);
  
  
    mySerial.println("AT+SAPBR=2,1");
   //updateSerial();
    wdt_reset();
  delay(DELAY_MS);
  
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //after executing the following command. the LED light of 
  //mySeriall blinks very fast (twice a second) 
  //enable the GPRS: enable bearer 1
  mySerial.println("AT+HTTPINIT");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Get IP Address - Query the GPRS bearer context status
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

void updateSerial2(String sms)
{
    wdt_reset();
    char sms1[sms.length()+1];
    sms.toCharArray(sms1, sms.length()+1);
    if(strstr(sms1,"+CMT:")!=NULL){
    //Serial.println(sms1+49);
    char bufferr[sms.length()+1];
    strcpy(bufferr,"#%");
    strcat(bufferr,sms1+49);
    //Serial.println(bufferr);
    trans(2,bufferr);
    strcpy(bufferr,"");
    }
    wdt_reset();
  delay(DELAY_MS);
}

void post_to_firebase3(String data)
{
  //Start HTTP connection
  mySerial.println("AT+HTTPINIT");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);

  //Enabling SSL 1.0
  if(USE_SSL == true){
    mySerial.println("AT+HTTPSSL=1");
    //updateSerial();
     wdt_reset();
    delay(DELAY_MS);
  }
  
  //Setting up parameters for HTTP session
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Set the HTTP URL - Firebase URL and FIREBASE SECRET
  mySerial.println("AT+HTTPPARA=\"URL\","+FIREBASE_HOST+".json?auth="+FIREBASE_SECRET);
  //updateSerial();
  wdt_reset();
  delay(DELAY_MS);
  
  //Setting up re direct
  mySerial.println("AT+HTTPPARA=\"REDIR\",0");//0
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Setting up content type
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  //updateSerial();
   wdt_reset();
  delay(DELAY_MS);
  
  //Sending HTTP POST request
  mySerial.println("AT+HTTPACTION=0");
  w=millis();

    while(!mySerial.available())
    {
       wdt_reset();
    if(millis()-w > 30000){
      w=millis();
      break;
    }

    }
    wdt_disable();
    delay(2000);
    wdt_enable(WDTO_4S);
    wdt_reset();
    String response = mySerial.readString();

    if(response.indexOf("+HTTPACTION:") > 0)
    {
       //Serial.println(response);

      if(!(response.indexOf("601") > 0))
      {
        k=millis();
      }
      if(!(response.indexOf("200") > 0) && (millis()-k > 60000) )
      {
      digitalWrite(44, LOW);
      delay(50);
      digitalWrite(44, HIGH);
      delay(50);
      gprs_disconnect();
      delay(50);
      gprs_connect();
      k=millis();
      }
      
    }
   wdt_reset();
  delay(DELAY_MS);
  
  mySerial.println("AT+HTTPREAD");
    w=millis();
    while(!mySerial.available())
    {
       wdt_reset();
    if(millis()-w > 30000){
      w=millis();
      break;
    }

    }
   
    response = mySerial.readString();
     char buf2 [response.length()+1];
    memset(buf2, '\0', sizeof(buf2));
    strcpy(buf2,"");
    if(response.indexOf("+HTTPREAD:") > 0)
    {
      ya=millis();
     response.toCharArray(buf2, (response.length()+1));
     transfer=response;
    
    }
  
}

void requestEvent() {
if(transfer.length()>5  && tu==0){
  Wire.write(zerofill(transfer));
tu=1;
}

if(tu==1){
   wdt_reset();
   if(transfer.length()>5){
    char buff7[transfer.length()+10];
    transfer.toCharArray(buff7, (transfer.length()+1));
     int wy= (strlen(buff7)/32);
      //Serial.println(wy);
       if((strlen(buff7)%32)>0)
     {
      wy=wy+1;
      //Serial.println(wy);
     }
     int zx=0;
     int dy=0;
     while( zx < wy)
     {
        wdt_reset();
        char buffer[strlen(buff7)+10];
        strcpy(buffer,"");
        strcpy(buffer,buff7+dy);
     Wire.write(buffer);
     dy=dy+32;
     zx=zx+1;
     delay(80);
     //Serial.println(buffer);
     }
    transfer="";
   }
   tu=0;
}

  if(transfer.length()<5){
  Wire.write("~~~~~~~~~~~~~~~~");
  transfer="";
  wdt_reset();
  }  
}


char* zerofill(String response){
char buff10[64];
  strcpy(buff10,"");
  char buff12[64];
  strcpy(buff12,"");
  char buff11[64];
  strcpy(buff11,"");
  itoa(response.length(),buff11,10);
 int r8=15-strlen(buff11);
 int r9=0;
 int n=0;
 while(r9<r8){
 strcat(buff10,"0");
   r9++;
 }
 strcat(buff10,buff11);
return buff10;
}




