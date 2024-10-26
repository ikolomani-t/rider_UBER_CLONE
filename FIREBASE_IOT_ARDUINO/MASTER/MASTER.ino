#include "U8glib.h"
#include <Key.h>
#include <Keypad.h>
#include <ezBuzzer.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <ctype.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <Arduino_JSON.h>
#include <assert.h>
#include<avr/wdt.h>

#define SDAPIN 20
#define CLKPIN 21

int localbusy = 0;
int pending = 0;

//void init_gsm();
//void gprs_connect();
//boolean gprs_disconnect();
//int is_gprs_connected();
//void post_to_firebase(String data);
char* waitResponse(char response[80]="OK", unsigned int timeout=2000);

#define rxPin 53
#define txPin 48
//SoftwareSerial mySerial(rxPin,txPin);
AltSoftSerial mySerial;

const String APN  = "safaricom";
const String USER = "saf";
const String PASS = "data";

//const String FIREBASE_HOST  = "https://vika-travel.firebaseio.com/";
//const String FIREBASE_SECRET  = "Y7makprFANJGaNnjQuvlUOD5o68ugE6TGfv2JzNh";

const String FIREBASE_HOST  = "https://vika-travel-default-rtdb.firebaseio.com/RidersInformation/";   //RidersInformation/
const String FIREBASE_SECRET  = "Z9syk8noQlbI9v9jHRenIWfv65rYLvFO2u6Run75";


#define USE_SSL true
#define DELAY_MS 500

//File myFile,myFile1;
int sdstatus=0;
int x=0;
int y=0;
int cond=0;
int i;
long yu,ya;
int op=0;
int q=0;
int rcv=0;
String temp_str;
long m;
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, A4, A0, A1, A3, A2,U8G_PIN_NONE);  // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, A6, A1, A0, A3, A2,U8G_PIN_NONE);
const byte ROWS = 4;
const byte COLS = 4;

/*char hexaKeys[ROWS][COLS] = {

{'7','8','9','/'},

{'4','5','6','*'},

{'1','2','3','.'},

{'C','0','=','+'}

};//*/


char hexaKeys[ROWS][COLS] = {
{'/','*','.','+'},

{'3','6','9','='},

{'2','5','8','0'},

{'1','4','7','C'}
};
byte rowPins[ROWS] = {0, 19, 2, 3};

byte colPins[COLS] = {14, 15, 16, 17};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 ezBuzzer buzzer(18);
 char customKey;
  char customKey2;
  char cumm1[20];
 char cumm2[20];
 char cumm3[20];
 char cumm4[20];
 char ikosh[20];
 char ikosh1[20];
 char ikosh2[20];
 char tr[32];
char tre[64];
char tre2[64];
char tre3[64];
char tre4[64];
char mak8[10];
char mak[10];
char mak2[10];
char mak3[10];
char mak4[30];
String dat;
String tree,tree2;
String h="testing cabin";



long intial, address;
 double j;
 double b;
 double w;
 double k;
 double p;
int  position=0;
int simcad=0;

void setup() 
{
u8g.begin();
Wire.begin(2);
Wire.setWireTimeout(20000, true);
pinMode(25, OUTPUT);
SD.begin(25);
pinMode(44,OUTPUT);
pinMode(32,OUTPUT);
pinMode(33,OUTPUT);
pinMode(34,OUTPUT);
pinMode(18,OUTPUT);
strcpy(tre,"");
strcpy(tre4,"");
strcpy(tr,"");
strcpy(mak,"");
strcpy(mak2,"");
strcpy(mak3,"");
strcpy(mak4,"");
u8g.firstPage();  
uint8_t a=5;
 dat = "{";
 dat += "\"humidity\":\"" + h + "\""; 
 dat += "}";
 digitalWrite(44, HIGH);
     j=millis();
     b=millis();
     k=millis();
     w=millis();
     p=millis();
     ya=millis();
     yu=0;
     tree2 = "";
     tree = "";
     m=0;
     rcv=0;
  
  Serial.begin(19200);
  mySerial.begin(19200);
  Wire.onReceive(receiveEvent);
   wdt_enable(WDTO_4S);
    wdt_reset();
   Serial.println("Starting Cabin System...");
}



void loop(void) {
  wdt_reset();
  while (!SD.begin(25)){
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "INSERT MC");
  } while( u8g.nextPage() ); 
  digitalWrite(32, LOW);  
  digitalWrite(33, LOW);  
  digitalWrite(34, LOW);  
  wdt_reset();
  simcad=1;
  }
    while (SD.begin(25) && simcad==1){
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "MC INSERTED");
  } while( u8g.nextPage() ); 
  digitalWrite(32, LOW);  
  digitalWrite(33, LOW);  
  digitalWrite(34, LOW);  
  wdt_reset();
  simcad=0;
  }
  fileab();
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){ 
ikosh[0] = customKey;
strcat(cumm1,ikosh);

u8g.firstPage();
      do {
        u8g.setFont(u8g_font_helvR08);  
        u8g.drawStr(0, 10, cumm1); 
      } while( u8g.nextPage() );

}

if(strlen(cumm1)>18)
{
  strcpy(cumm1,"");
}
position=1;
if(rcv!=1){
int mv=9;
while(mv<11){
  wdt_reset();
  recslave(mv,6);
   delay(100);
  mv++;
}
}
 printfile();
 position=0;
 yu=millis()-ya;
while(yu>4000){
   wdt_reset();
   ya=millis();
   yu=millis()-ya;
   if (position==0){
   sentopendoors("OFF"," ST- ");
   sentopendoors("ON"," ST* ");
   }
 }
 position=2;
 if(strstr(somaa("1STATE.txt",32),"ON")!=NULL){

if(strstr(somaa("1POZ.txt",32),"ON")!=NULL){
 //digitalWrite(32, HIGH);
 delay(100);
 digitalWrite(32, LOW);
 andika4("1POZ.txt", "OFF");
}
 }else{
  // digitalWrite(32, LOW);
  if(strstr(somaa("1POZ.txt",32),"OFF")!=NULL){
 digitalWrite(32, HIGH);
 delay(100);
 digitalWrite(32, LOW);
 andika4("1POZ.txt", "ON");
}

 }
  if(strstr(somaa("2STATE.txt",33),"ON")!=NULL){
 if(strstr(somaa("2POZ.txt",33),"ON")!=NULL){
 //digitalWrite(33, HIGH);
 delay(100);
 digitalWrite(33, LOW);
 andika4("2POZ.txt", "OFF");
}

 }else{
   //digitalWrite(33, LOW);  
   if(strstr(somaa("2POZ.txt",33),"OFF")!=NULL){
 digitalWrite(33, HIGH);
 delay(100);
 digitalWrite(33, LOW);
 andika4("2POZ.txt", "ON");
}
 }
 if(strstr(somaa("3STATE.txt",34),"ON")!=NULL){

  if(strstr(somaa("3POZ.txt",34),"ON")!=NULL){

digitalWrite(32, HIGH);
 delay(100);
 digitalWrite(34, LOW);
 andika4("3POZ.txt", "OFF");
}

 }else{
   
  if(strstr(somaa("3POZ.txt",34),"OFF")!=NULL){
 digitalWrite(34, HIGH);
 digitalWrite(32, LOW);
 delay(100);
 digitalWrite(34, LOW);
 andika4("3POZ.txt", "ON");
} 
 }

 if(customKey =='C')
  {
   if(strlen(cumm1)>0){
    char tra[65];
    strcpy(tra,cumm1);
     memset(cumm1, '\0', sizeof(cumm1));
    strncpy(cumm1,tra,strlen(tra)-1);
    u8g.firstPage();  
      do {
        u8g.setFont(u8g_font_helvR08);  
        u8g.drawStr(5, 15, cumm1); 
      } while( u8g.nextPage() );

   strcpy(tra,"");
  }
  }

  onbulb();
}

void onbulb(){
  int rq=1;
  char strr[10];
  char strr2[10];
  strcpy(strr,"");
  strcpy(strr2,"");

  char strrc[10];
  char strrc2[10];
  strcpy(strrc,"");
  strcpy(strrc2,"");
  
  while(rq<3){
    sprintf(strr, "%d", rq);
    strcpy(strr2,strr);
    strcat(strr2,"POZ.txt");

    sprintf(strrc, "%d", rq);
    strcpy(strrc2,strrc);
    strcat(strrc2,"STATE.txt");

  if(strstr(somaa(strr2,34),"OFF")!=NULL){
if(strstr(somaa(strrc2,34),"ON")!=NULL){
digitalWrite(32, HIGH);
 delay(100);
 digitalWrite(34, LOW);
 andika4(strr2, "OFF");
  }
}
strcpy(strr,"");
strcpy(strrc,"");
rq++;
  }
}

void updateSerial()
{
  while (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available())
  {
   
        Serial.write(mySerial.read());       
  }
  
}

void printfile(){
     if(tree2.length() >= 5 ){
   Serial.println(tree2);
   char buf6[tree2.length()+1];
   strcpy(buf6,"");
   tree2.toCharArray(buf6,tree2.length());
   andika2("CABIN4",buf6);
   tree2 = "";
   }  
}

int numberofbytes(char deta[])
{
 int s=0;
  int g=0;
  char c[2];
  String ch;
  String ch1;
  String ch2;
  ch="";
  ch2="";
  ch.concat(deta);
while(g<strlen(deta)){
  wdt_reset();
  ch2.concat(ch.charAt(g));
  if(ch2.indexOf("|")<0){
   ch1.concat(ch.charAt(g));
  }
  g++;
}
  s=ch1.toInt();
  ch="";
   ch1="";
  return s;
}

void receiveEvent(int how) {
  rcv=1;
  strcpy(tre4,"");
  strcpy(tr,"");
  while (Wire.available()) {
  wdt_reset();
  int c = Wire.read();
   sprintf(tr,"%c",c);
   if(strcmp(tr,"#")!=0){
    strcat(tre4,tr);
   }
  }
  Serial.println(tre4);
  if(strstr(tre4,"%")!=NULL)
   {
      andika3("CABIN4",tre4+3);
      strcpy(tre4,"");
    }

      if(strcmp(tre4,"")!=0){
        if(strstr(tre4,"=")==NULL){
        tree.concat(tre4);
        }
      }

    if(strstr(tre4,"=")!=NULL)
    {
    char buf4[tree.length()+10];
    char buf5[tree.length()+10];
    strcpy(buf4,"");
    strcpy(buf5,"");
    tree.toCharArray(buf4,tree.length()+10);
    strcpy(buf5,buf4);
    tree2 = "";
    tree2=String(buf5);
    tree = "";
    op=0;
    rcv=0;
    
    }

  strcpy(tre4,"");
  strcpy(tr,"");
}

void fileab(){
if(SD.exists("1STATE.txt")!=1){
digitalWrite(32, LOW);  
}  
if(SD.exists("2STATE.txt")!=1){
digitalWrite(33, LOW);  
} 
if(SD.exists("3STATE.txt")!=1){
digitalWrite(34, LOW);  
} 
wdt_reset();
}

void updateSerial4() {
  if(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if(Serial.available()) {
    mySerial.write(Serial.read());
  }
}


void sentopendoors(char r1[],char r2[]){
  int we=1;
  char h[20];
  char opend[100];
  char x[20];
  char fg[100];
  strcpy(h,"");
  strcpy(opend, r2);
  strcpy(x,"");
  strcpy(fg,"");
  while(we<4){
     wdt_reset();
itoa(we, h, 10);
 Serial.println(h);
strcpy(x,h);
strcat(h,"STATE.txt");
  if(strcmp(somaa1(h), r1)==0){
    strcat(x,",");
   strcat(opend,x);
  }
  we++;
  }
  Serial.println("Start data");
  int adr = 9;
  while(adr<11){
    delay(80);
  wdt_reset();
  trans(adr, opend);
  adr++;
  }
Serial.println("End data");
}

void updateSerial3()
{
  char sto[100];
  char br[100];
  strcpy(br,"");
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
   
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

void recslave(int add, int nb){
  q=0;
  char mak5[64];
  char mak6[64];
  char tre5[64];
  strcpy(tre,"");
  strcpy(mak8,"");
  strcpy(mak,"");
  strcpy(mak2,"");
  strcpy(mak3,"");
  strcpy(mak4,"");
  strcpy(mak5,"");
  strcpy(mak6,"");
  strcpy(tre5,"");
  byte len = Wire.requestFrom(add, nb,true); 
  if (len == 0) {
    #if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag()){
      Serial.println("It was a timeout");
    
      i2cRecovery();
    }
    #endif
  }
  delay(80); 
  while (Wire.available()){
  wdt_reset();
  char c = Wire.read();
  sprintf(tr,"%c",c);
  if(!(c==126) ){
   if(strcmp(tr,"-")==0){
    cond=1;

      u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, tr);
  } while( u8g.nextPage() ); 

    strcpy(tr,"");
   }
if(cond==0){
  strcat(mak,tr);
}
   if(cond==1){
    strcat(tre,tr);
   }

  u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, mak);
  } while( u8g.nextPage() ); 

  }
  
  }

         strcpy(mak2,mak);
         strcpy(mak8,mak);
         strcat(mak,"CODE");
         strcat(mak2,"STATE");
         strcat(mak8,"POZ");
         strcpy(mak5,mak);
         strcpy(mak6,mak2);
         cond=0;


  if(strcmp(tre,"")!=0){

    strcpy(tre5,tre);
  Serial.println(somaa2("CABIN4.txt",tre));
  if(strcmp(somaa2("CABIN4.txt",tre),"OKK")==0){
  if(strcmp(mak,"")!=0 && strcmp(mak2,"")!=0 ){
    if(strstr(soma1(mak6,32),"OFF")!=NULL){
     andika(mak,tre);
    }
    strcpy(mak3,mak2);
    strcpy(mak4,mak5);
    strcat(mak2,".txt");
    if(strstr(somaa1(mak2),"OFF")!=NULL){
      Serial.println("writting On");
      strcpy(mak2,mak3);
      andika(mak2,"ON");
      andika(mak8,"ON");
    }else{
      strcat(mak5,".txt");
     if(strstr(somaa1(mak5),tre5)!=NULL){
      Serial.println("Writting Off");
      strcpy(mak2,mak3);
      andika(mak2,"OFF");
      andika(mak8,"OFF");
     }
     else{
  trans(add," FR Incorrect Code-in");
   }
      }
  
    }

       if(q==1){
         strcpy(mak,mak4);
         u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);
 u8g.drawStr(0, 15, mak);
  } while( u8g.nextPage());
    Serial.println(mak);
    char feed[100];
    strcpy(feed," FB ");
    strcat(feed, soma(mak));
  
    trans(add," FB Success Code");
  }
}else{
  trans(add," FR Incorrect Code-out");
}
}
  strcpy(tre,"");
  strcpy(tr,"");
  strcpy(mak,"");
  strcpy(mak2,"");
  strcpy(mak3,"");
  strcpy(mak4,"");
  strcpy(tre5,"");
  strcpy(mak8,"");

  }




  void trans(int address, char data[])
{
  Serial.println(data);
wdt_reset();
Wire.beginTransmission(address); 
  Wire.write(data);       
  byte error = Wire.endTransmission();
  delay(80);
    if (error == 5){
    Serial.println("It was a timeout");
    i2cRecovery();
  }
  strcpy(tre,"");
  wdt_reset();
  delay(80);
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.clearWireTimeoutFlag();
  #endif
  }


void andika(char lira[100], char mass[]){
    strcat(lira,".txt");
    //SD.begin(25);
    if(SD.exists(lira)==1){
    if(SD.remove(lira)==1){
    
    }else{
      u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr(0, 45, "delete failed"); 
  } while( u8g.nextPage() ); 
    }
    }
    File myFile1;
   myFile1 = SD.open(lira, FILE_WRITE);

 if (myFile1){ 
    wdt_reset();
    myFile1.print(mass);
    Serial.println(mass);
    wdt_reset();
  q=1;
  u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Opening file S1-2");
 u8g.drawStr( 0, 45, mass);
  } while( u8g.nextPage() ); 
    myFile1.close();
  } else { 
         wdt_reset();
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Openning file E1-2");
  } while( u8g.nextPage() );
  myFile1.close();
  }
  //SD.end();
}


void andika2(char lira[100], char mass[]){
    //strcat(lira,".txt");
    char lire[64];
    strcpy(lire,"CABIN4.txt");
    //SD.begin(25);
    if(SD.exists(lire)==1){
    if(SD.remove(lire)==1){
    
    }else{

      u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr(0, 30, "Delete failed"); 
  } while( u8g.nextPage() ); 
    }
    }
    File myFile2;
   myFile2 = SD.open(lire, FILE_WRITE);

 if (myFile2) {
  wdt_reset();
  myFile2.print(mass);
  u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08); 
 u8g.drawStr( 0, 30, "Firebase file S1");
 u8g.drawStr( 0, 45, mass);
  } while( u8g.nextPage() );
    myFile2.close();
  } else {
    wdt_reset(); 
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Firebase file E1");
  } while( u8g.nextPage() );
  myFile2.close();
  }
  //SD.end();
}


void andika3(char lira[100], char mass[]){
    //strcat(lira,".txt");
    char lire[64];
    strcpy(lire,"CABIN4.txt");
    //SD.begin(25);
   File myFile3;
   myFile3 = SD.open(lire, FILE_WRITE);

 if (myFile3) {
  wdt_reset();
  myFile3.seek(EOF);
  myFile3.print(mass);
  u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);
 u8g.drawStr( 0, 30, "Append S2");
 u8g.drawStr( 0, 45, mass);
  } while( u8g.nextPage() );
    myFile3.close();
  } else {
    wdt_reset(); 
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Append E2");
  } while( u8g.nextPage() );
  myFile3.close();
  }

  //SD.end();
}

void andika4(char lira[100], char mass[]){
     
    //strcat(lira,".txt");
    //SD.begin(25);
    if(SD.exists(lira)==1){
    if(SD.remove(lira)==1){
    
    }else{
      u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr(0, 45, "delete failed"); 
  } while( u8g.nextPage() ); 
    }
    }
  File myFile4;
   myFile4 = SD.open(lira, FILE_WRITE);

 if (myFile4) { 
    wdt_reset();
    myFile4.print(mass);
    Serial.println(mass);
    wdt_reset();
  //q=1;
  u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Opening file S1-3");
 u8g.drawStr( 0, 45, mass);
  } while( u8g.nextPage() ); 
    myFile4.close();
  } else { 
         wdt_reset();
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, "Openning file E1-3");
  } while( u8g.nextPage() );
  myFile4.close();
  }
  
  //SD.end();
}

char* soma(char lira[100]){
  
 //SD.begin(25);
 strcpy(tre2,"");
  strcpy(tre3,"");
 static char buf [64];
 strcpy(buf,"");
 strcat(lira,".txt");
 u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 30, lira);
  } while( u8g.nextPage() );
  File myFile5;
  myFile5 = SD.open(lira, O_READ );
  if (myFile5) {
    q=0;
while (myFile5.available()) {
    wdt_reset();
    sprintf(tre3,"%c",myFile5.read());
    if(strlen(tre2)<5){
      strcat(tre2,tre3);
      }
      } 
      strcpy(buf,tre2);
      u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 45, "Read S1");
  } while( u8g.nextPage() ); 
    myFile5.close();
  } else {
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "Closing file E3");
  } while( u8g.nextPage() ); 
  myFile5.close();

  }
  strcpy(tre3,"");
  strcpy(tre2,"");
  strcpy(mak,"");
  strcpy(mak2,"");
 // SD.end();
  return buf;
}

char* somaa(char lira[100], int p){
  
 //SD.begin(25);
 strcpy(tre3,"");
 strcpy(tre2,"");
 static char buf [64];
 strcpy(buf,"");
 File myFile6;
  myFile6 = SD.open(lira, O_READ);
  if (myFile6) {
while (myFile6.available()) {
     wdt_reset();
     sprintf(tre3,"%c",myFile6.read());
     strcat(tre2,tre3);
      }
      strcpy(buf,tre2);
    myFile6.close();
    strcpy(mak,"");
    strcpy(mak2,"");
  } else {
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "Control file E23");
  } while( u8g.nextPage() );
     myFile6 =SD.open(lira, FILE_WRITE); 
    if(strstr(lira,"STATE")!=NULL){
    myFile6.print("OFF");
  }
    myFile6.close();
  }
  strcpy(tre3,"");
  strcpy(tre2,"");
  strcpy(mak,"");
  strcpy(mak2,"");
  //SD.end();
  return buf;
}



char* soma1(char lira[100], int p){
  
 //SD.begin(25);
 strcpy(tre3,"");
 strcpy(tre2,"");
 static char buf [64];
 strcpy(buf,"");
 strcat(lira,".txt");
 File myFile7;
  myFile7 = SD.open(lira, O_READ);
  if (myFile7) {
while (myFile7.available()) {
     wdt_reset();
     sprintf(tre3,"%c",myFile7.read());
     strcat(tre2,tre3);
      }
      strcpy(buf,tre2);
    myFile7.close();
  } else {
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "Control file E22");
  } while( u8g.nextPage() ); 
    myFile7.close();
  }
  strcpy(tre3,"");
  strcpy(tre2,"");
  
  //SD.end();
  return buf;
}



char* somaa1(char lira[100]){
  
 //SD.begin(25);
 strcpy(tre3,"");
 strcpy(tre2,"");
 static char buf [64];
 strcpy(buf,"");
 Serial.println(lira);
 File myFile8;
  myFile8 = SD.open(lira, O_READ);
  if (myFile8) {
    Serial.println("openning S2");
    while (myFile8.available()) {
      wdt_reset();
    sprintf(tre3,"%c",myFile8.read());
      strcat(tre2,tre3);
      }
      Serial.println(tre2);
      strcpy(buf,tre2);
    myFile8.close();
  } else {
    Serial.println("Openning Los");
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "Control file E1E");
  } while( u8g.nextPage() ); 
  myFile8 =SD.open(lira, FILE_WRITE);
  if(strstr(lira,"STATE")!=NULL){
    myFile8.print("OFF");
  }
    myFile8.close();
    wdt_reset();
  }
  strcpy(tre3,"");
  strcpy(tre2,"");
  strcpy(mak,"");
  strcpy(mak2,"");
  
  //SD.end();
  return buf;
}

char* somaa2(char lira[100],char inco[]){
  
  String am;
  am="";
 //SD.begin(25);
 strcpy(tre3,"");
 strcpy(tre2,"");
 static char buf [64];
 strcpy(buf,"");
 File myFile9;
  myFile9 = SD.open(lira, O_READ);
  if (myFile9) {
while (myFile9.available()) {
     wdt_reset();
     sprintf(tre3,"%c",myFile9.read());
     //strcat(tre2,tre3);
     am.concat(tre3);
      }
      char cmp[am.length()+1];
      am.toCharArray(cmp, am.length()+1);
      if(strstr(cmp,inco)!=NULL){
      strcpy(buf,"OKK");
      }else
      {
        strcpy(buf,"Data not found");
      }
    myFile9.close();
  } else {
    u8g.firstPage();
      do {
    u8g.setFont(u8g_font_helvR08);  
 u8g.drawStr( 0, 15, "Control file E21");
  } while( u8g.nextPage() ); 
    myFile9.close();
  }
  strcpy(tre3,"");
  strcpy(tre2,"");
  
  //SD.end();
  return buf;
}

void post_to_firebase(String data)
{
  
  char buf[600];
  //Start HTTP connection
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  delay(DELAY_MS);
  
  //Enabling SSL 1.0
  if(USE_SSL == true){
    mySerial.println("AT+HTTPSSL=1");
    updateSerial();
    delay(DELAY_MS);
  }
  
  //Setting up parameters for HTTP session
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  updateSerial();
  delay(DELAY_MS);
  
  //Set the HTTP URL - Firebase URL and FIREBASE SECRET
  mySerial.println("AT+HTTPPARA=\"URL\","+FIREBASE_HOST+".json?auth="+FIREBASE_SECRET);
  updateSerial();
  delay(DELAY_MS);
  
  //Setting up re direct
  mySerial.println("AT+HTTPPARA=\"REDIR\",0");//0
  updateSerial();
  delay(DELAY_MS);
  
  //Setting up content type
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  updateSerial();
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
    
    delay(3000);
    String response = mySerial.readString();
    
    if(response.indexOf("+HTTPACTION:") > 0)
    {
       Serial.println(response);

      if(!(response.indexOf("601") > 0))
      {
        k=millis();
      }
      if(!(response.indexOf("200") > 0) && (millis()-k > 60000) )
      {
      digitalWrite(44, LOW);
      delay(100);
      digitalWrite(44, HIGH);
      delay(100);
      gprs_disconnect();
      delay(100);
      gprs_connect();
      k=millis();
      }
      
    }
  
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

   

     memset(buf, '\0', sizeof(buf));
     while(mySerial.available()>0)
    {
      wdt_reset();
    int c= mySerial.read();
    sprintf(tr,"%c",c);
    strcat(buf,tr);
    
    }
    response = mySerial.readString();

  
    char buf2 [response.length()+1];
    memset(buf2, '\0', sizeof(buf2));
    if(response.indexOf("+HTTPREAD:") > 0)
    {
      
     //Serial.println(response);
     response.toCharArray(buf2, (response.length()+1));
     Serial.println(buf2+27);
     if(strstr(buf2+27,"ON")!=NULL)
     {
      digitalWrite(32, HIGH); 
     }
     if(strstr(buf2+27,"OFF")!=NULL)
     {
       digitalWrite(32, LOW); 
     }
     Serial.println(response.length()+1);
     Serial.println(strlen(buf2));
     andika("CABIN3",buf2+27);
     
      delay(DELAY_MS);
    }
  
  
}


void post_to_firebase2(String data)
{
  
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Start HTTP connection
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Enabling SSL 1.0
  if(USE_SSL == true){
    mySerial.println("AT+HTTPSSL=1");
    updateSerial();
    delay(DELAY_MS);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up parameters for HTTP session
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Set the HTTP URL - Firebase URL and FIREBASE SECRET
  mySerial.println("AT+HTTPPARA=\"URL\","+FIREBASE_HOST+".json?auth="+FIREBASE_SECRET);
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up re direct
  mySerial.println("AT+HTTPPARA=\"REDIR\",1");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up content type
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Setting up Data Size
  //+HTTPACTION: 1,601,0 - error occurs if data length is not correct
  
  mySerial.println("AT+HTTPDATA=" + String(data.length()) + ",10000");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Sending Data
  mySerial.println(data);
  updateSerial();
  delay(DELAY_MS);
   //*/
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Sending HTTP POST request
  mySerial.println("AT+HTTPACTION=1");
    w=millis();
  for (uint32_t start = millis(); millis() - start < 20000;){
    while(!mySerial.available())
    {
    if(millis()-w > 30000){
      w=millis();
      break;
    }

    }
    String response = mySerial.readString();
    if(response.indexOf("+HTTPACTION:") > 0)
    {
      Serial.println(response);
      break;
    }
  }
    
  delay(DELAY_MS);
  
  //+HTTPACTION: 1,603,0 (POST to Firebase failed)
  //+HTTPACTION: 0,200,0 (POST to Firebase successfull)
  //Read the response
  mySerial.println("AT+HTTPREAD");
  delay(10000);
  updateSerial();
  delay(DELAY_MS);
  
  //Stop HTTP connection
  /*mySerial.println("AT+HTTPTERM");
  updateSerial();
  delay(DELAY_MS);//*/
}


void init_gsm()
{
  mySerial.println("AT");
  updateSerial3();
  delay(DELAY_MS);

  mySerial.println("AT+CMGF=1");
  updateSerial3();
  delay(DELAY_MS);
 mySerial.println("AT+CNMI=2,2,0,0,0");
 updateSerial3();
 delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM  AT+CPIN="3053”
  //Checks if the SIM is ready
  mySerial.println("AT+CPIN?");
  updateSerial3();
  delay(DELAY_MS);

   mySerial.println("AT+CPIN=\"8183\"");
  updateSerial3();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Turning ON full functionality
  mySerial.println("AT+CFUN=1");
   updateSerial3();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Turn ON verbose error codes
  mySerial.println("AT+CMEE=2");
   updateSerial3();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Enable battery checks
  mySerial.println("AT+CBATCHK=1");
   updateSerial3();
  delay(DELAY_MS);
  //screen(0,15,waitResponse());

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  mySerial.println("AT+CREG?");
   updateSerial3();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //setting SMS text mode
  mySerial.print("AT+CMGF=1\r");
   updateSerial3();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}





//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Connect to the internet
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void gprs_connect()
{
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //DISABLE GPRS
  mySerial.println("AT+SAPBR=0,1");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Connecting to GPRS: GPRS - bearer profile 1
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
 updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //sets the APN settings for your sim card network provider.
  mySerial.println("AT+CSTT=\"safaricom\",\"saf\",\"data\"");
 updateSerial();
  delay(DELAY_MS);
  
    mySerial.println("AT+SAPBR=1,1");
    updateSerial();
  delay(DELAY_MS);
  
  
    mySerial.println("AT+SAPBR=2,1");
   updateSerial();
  delay(DELAY_MS);
  
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //after executing the following command. the LED light of 
  //mySeriall blinks very fast (twice a second) 
  //enable the GPRS: enable bearer 1
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Get IP Address - Query the GPRS bearer context status
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  updateSerial();
  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Function: gprs_disconnect()
* AT+CGATT = 1 modem is attached to GPRS to a network. 
* AT+CGATT = 0 modem is not attached to GPRS to a network
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
boolean gprs_disconnect()
{
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Disconnect GPRS
  mySerial.println("AT+CGATT=0");
  updateSerial();
  delay(DELAY_MS);

  return true;
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
        int c = mySerial.read();
        sprintf(tr,"%c",c);
        strcat(response,tr);
    }
    if(strstr(response,expected_answer)!= NULL){
        strcpy(answer, "1");
        }

  strcpy(response,"");
  return answer;
}



void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void u8g_box_frame(uint8_t a) {
  u8g.drawStr( 0, 0, "drawBox");
  u8g.drawBox(5,10,20,10);
  u8g.drawBox(10+a,15,30,7);
  u8g.drawStr( 0, 30, "drawFrame");
  u8g.drawFrame(5,10+30,20,10);
  u8g.drawFrame(10+a,15+30,30,7);
}

void u8g_disc_circle(uint8_t a) {
  u8g.drawStr( 0, 0, "drawDisc");
  u8g.drawDisc(10,18,9);
  u8g.drawDisc(24+a,16,7);
  u8g.drawStr( 0, 30, "drawCircle");
  u8g.drawCircle(10,18+30,9);
  u8g.drawCircle(24+a,16+30,7);
}

void u8g_r_frame(uint8_t a) {
  u8g.drawStr( 0, 0, "drawRFrame/Box");
  u8g.drawRFrame(5, 10,40,30, a+1);
  u8g.drawRBox(50, 10,25,40, a+1);
}

void u8g_string(uint8_t a) {
  u8g.drawStr(30+a,31, " 0");
  u8g.drawStr90(30,31+a, " 90");
  u8g.drawStr180(30-a,31, " 180");
  u8g.drawStr270(30,31-a, " 270");
}

void u8g_line(uint8_t a) {
  u8g.drawStr( 0, 0, "drawLine");
  u8g.drawLine(7+a, 10, 40, 55);
  u8g.drawLine(7+a*2, 10, 60, 55);
  u8g.drawLine(7+a*3, 10, 80, 55);
  u8g.drawLine(7+a*4, 10, 100, 55);
}

void u8g_triangle(uint8_t a) {
  uint16_t offset = a;
  u8g.drawStr( 0, 0, "drawTriangle");
  u8g.drawTriangle(14,7, 45,30, 10,40);
  u8g.drawTriangle(14+offset,7-offset, 45+offset,30-offset, 57+offset,10-offset);
  u8g.drawTriangle(57+offset*2,10, 45+offset*2,30, 86+offset*2,53);
  u8g.drawTriangle(10+offset,40+offset, 45+offset,30+offset, 86+offset,53+offset);
}

void u8g_ascii_1() {
  char s[2] = " ";
  uint8_t x, y;
  u8g.drawStr( 0, 0, "ASCII page 1");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 32;
      u8g.drawStr(x*7, y*10+10, s);
    }
  }
}

void u8g_ascii_2() {
  char s[2] = " ";
  uint8_t x, y;
  u8g.drawStr( 0, 0, "ASCII page 2");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 160;
      u8g.drawStr(x*7, y*10+10, s);
    }
  }
}

void u8g_extra_page(uint8_t a)
{
  if ( u8g.getMode() == U8G_MODE_HICOLOR || u8g.getMode() == U8G_MODE_R3G3B2) {
    /* draw background (area is 128x128) */
    u8g_uint_t r, g, b;
    b = a << 5;
    for( g = 0; g < 64; g++ )
    {
      for( r = 0; r < 64; r++ )
      {
	u8g.setRGB(r<<2, g<<2, b );
	u8g.drawPixel(g, r);
      }
    }
    u8g.setRGB(255,255,255);
    u8g.drawStr( 66, 0, "Color Page");
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
  {
    u8g.drawStr( 66, 0, "Gray Level");
    u8g.setColorIndex(1);
    u8g.drawBox(0, 4, 64, 32);    
    u8g.drawBox(70, 20, 4, 12);
    u8g.setColorIndex(2);
    u8g.drawBox(0+1*a, 4+1*a, 64-2*a, 32-2*a);
    u8g.drawBox(74, 20, 4, 12);
    u8g.setColorIndex(3);
    u8g.drawBox(0+2*a, 4+2*a, 64-4*a, 32-4*a);
    u8g.drawBox(78, 20, 4, 12);
  }
  else
  {
    u8g.drawStr( 0, 12, "setScale2x2");
    u8g.setScale2x2();
    u8g.drawStr( 0, 6+a, "setScale2x2");
    u8g.undoScale();
  }
}


uint8_t draw_state = 0;

void draw1(void) {
  u8g_prepare();
  switch(draw_state >> 3) {
    case 0: u8g_box_frame(draw_state&7); break;
    case 1: u8g_disc_circle(draw_state&7); break;
    case 2: u8g_r_frame(draw_state&7); break;
    case 3: u8g_string(draw_state&7); break;
    case 4: u8g_line(draw_state&7); break;
    case 5: u8g_triangle(draw_state&7); break;
    case 6: u8g_ascii_1(); break;
    case 7: u8g_ascii_2(); break;
    case 8: u8g_extra_page(draw_state&7); break;
  }
}

void draw(void){
  u8g.setFont(u8g_font_helvB10);  
 u8g.drawStr( x, y, cumm1);
}

void i2cRecovery()
{
   Serial.println("Starting I2C bus recovery");
   wdt_reset();
  delay(100);
  wdt_reset();
  //try i2c bus recovery at 100kHz = 5uS high, 5uS low
  pinMode(SDAPIN, OUTPUT);//keeping SDA high during recovery
  digitalWrite(SDAPIN, HIGH);
  pinMode(CLKPIN, OUTPUT);
  for (int i = 0; i < 10; i++) { //9nth cycle acts as NACK
    digitalWrite(CLKPIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(CLKPIN, LOW);
    delayMicroseconds(5);
  }

  //a STOP signal (SDA from low to high while CLK is high)
  digitalWrite(SDAPIN, LOW);
  delayMicroseconds(5);
  digitalWrite(CLKPIN, HIGH);
  delayMicroseconds(2);
  digitalWrite(SDAPIN, HIGH);
  delayMicroseconds(2);
  //bus status is now : FREE

  Serial.println("bus recovery done, starting scan in 2 secs");
  //return to power up mode
  pinMode(SDAPIN, INPUT);
  pinMode(CLKPIN, INPUT);
  wdt_reset();
  delay(100);
  wdt_reset();
   Wire.begin();
  //only pins: no signal on clk and sda
  //only begin: no signal on clk, no signal on sda


}


