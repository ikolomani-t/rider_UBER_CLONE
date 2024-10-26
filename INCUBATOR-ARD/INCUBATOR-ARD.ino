#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <ezBuzzer.h>
#include<avr/wdt.h>
#include <Keypad.h>
#include<dht.h>

dht DHT;
const int rs = 9, en = 10, d4 = 11, d5 = 12, d6 = 13, d7 = A0;
bool A0s=true,A1s=true,A2s=true, A3s=true,A4s=true, A5s=true,A6s=true,A7s=true,A8s=true;
LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);
const byte ROWS = 4;
const byte COLS = 4;

/*char hexaKeys[ROWS][COLS] = {

{'/','3','2','1'},//membrane keypad

{'*','6','5','4'},

{'.','9','8','7'},

{'+','=','0','C'}

};//*/

char hexaKeys[ROWS][COLS] = {

{'C','7','4','1'},//plastic keypad

{'0','8','5','2'},

{'=','9','6','3'},

{'+','.','*','/'}

};//*/

byte rowPins[ROWS] = {4, 3, 2, 1}; 
byte colPins[COLS] = {A3, A2, A1, 0}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


long delaytime=0, delaytime2=0;
int cycle=0;
int HT_sensor = A5;
int MOTOR = 5;

int tempout = 8;
int humidout = 7;
int turnout = 6;
int rtemp,nav2,nav3;
int menu=0;
int modemenu=0;
int rhumi,adl,adh,adc,adhl,adhh,adhc;
int address;
char customKey;
char customKey2;
long tempupper,templower,tempcritical,humidupper,humidlower,humidcritical,offdu,ondu,intial,serial,mode,showp,showp2,lc,hc,hlc,hhc,ssd,var;
unsigned long mill=0,incubation,dayscounter;
char cumm[20];
char cumm1[20];
char ikosh[20];
char currentmenu[20];
float sky,sky2;
int sys_s=0;
String temp_str;
ezBuzzer buzzer(A4);

void setup() {
  buzzer.loop();
  lcd2.begin(16, 2);
 address=0;
 EEPROM.get(address, intial);
 if(intial!=3){
  tempupper=40;
  templower=20;
  tempcritical=45;
  humidupper=40;
  humidlower=20;
  humidcritical=45;
  dayscounter=0;
  offdu=3600000;
  ondu=10000;
  lc=0;
  hc=0;
  incubation=2592000000;
  intial=3;
  serial=1;
  mode=5;
  address=5;
  EEPROM.put(address,tempupper);
  address=10;
  EEPROM.put(address,templower);
  address=15;
  EEPROM.put(address,tempcritical);
  address=20;
  EEPROM.put(address,humidupper);
  address=25;
  EEPROM.put(address,humidlower);
  address=30;
  EEPROM.put(address,humidcritical);
  address=35;
  EEPROM.put(address,offdu);
  address=40;
  EEPROM.put(address,ondu);
  address=45;
  EEPROM.put(address,incubation);
  address=0;
  EEPROM.put(address,intial);
  address=50;
  EEPROM.put(address,serial);
  address=55;
  EEPROM.put(address,mode);
  address=60;
  EEPROM.put(address,dayscounter);
  address=65;
  EEPROM.put(address,hc);
  address=70;
  EEPROM.put(address,lc);
   address=75;
  EEPROM.get(address,hhc);
  address=80;
  EEPROM.get(address,hlc);

  lcd2.setCursor(0,0);
  lcd2.print("Setting complete");
  delay(2000);
 }
  welcome();
  showp=1;
  showp2=1;
  hc=0;
  lc=0;
  ssd=0;
  pinMode(HT_sensor,INPUT_PULLUP);
  pinMode(MOTOR,OUTPUT);
  pinMode(tempout,OUTPUT);
  pinMode(turnout,OUTPUT);
  pinMode(humidout,OUTPUT);
  digitalWrite(tempout,LOW);
  digitalWrite(turnout,LOW);
  digitalWrite(humidout,LOW);
  digitalWrite(MOTOR,LOW);
  wdt_disable();
  wdt_enable(WDTO_4S);
  buzzer.loop();
  buzzer.beep(80);
 
}


void loop(){
  buzzer.loop();
     if (customKey) {
    buzzer.beep(80);
  }
  wdt_reset();
  menu_u();
  sysdata();
  if(dayscounter>incubation){
  sys_s=1;
  digitalWrite(tempout,LOW);
  digitalWrite(turnout,LOW);
  digitalWrite(humidout,LOW);
  }else{
    sys_s=0;
  }
    if(sys_s==0)
  {
  rup();
  control();
  dayc();
  show();
  }else{
  show2();
  }
}

void sysdata(){
  cmode();
  address=adh;
  EEPROM.get(address,tempupper);
  address=adl;
  EEPROM.get(address,templower);
  address=adc;
  EEPROM.get(address,tempcritical);
  address=adhh;
  EEPROM.get(address,humidupper);
  address=adhl;
  EEPROM.get(address,humidlower);
  address=adhc;
  EEPROM.get(address,humidcritical);
  address=35;
  EEPROM.get(address,offdu);
  address=40;
  EEPROM.get(address,ondu);
  address=45;
  EEPROM.get(address,incubation);
  address=55;
  EEPROM.get(address,mode);
  address=60;
  EEPROM.get(address,dayscounter);
  address=65;
  EEPROM.get(address,hc);
  address=70;
  EEPROM.get(address,lc);
  address=75;
  EEPROM.get(address,hhc);
  address=80;
  EEPROM.get(address,hlc);
  sky2=(incubation-dayscounter)/86400000;
  //sky2=dayscounter/86400000;
}

void control(){
  DHT.read11(HT_sensor);
  rtemp = DHT.temperature;
  rtemp=rtemp+hc;
  rtemp=rtemp-lc;
  rhumi = DHT.humidity;
  rhumi=rhumi+hhc;
  rhumi=rhumi-hlc;
  
  if(rtemp<=templower){
    digitalWrite(tempout,HIGH);
  }
   if(rtemp>=tempupper){
    digitalWrite(tempout,LOW);
  }

    if(rtemp<=templower-5){
    //buzzer.beep(80);
    pip(2000,6000,200);
  }

   if(rhumi<=humidlower){
    digitalWrite(humidout,HIGH);
  }
   if(rhumi>=humidupper){
    digitalWrite(humidout,LOW);
  }
    if(rhumi>=humidcritical || rtemp>=tempcritical ){
     digitalWrite(MOTOR,HIGH);
     if(rhumi>=humidcritical ){
       pip(500,3000,50);
     }
       if(rtemp>=tempcritical ){
       pip(2000,3000,200);
     }
    //buzzer.beep(80);
  }else{
    digitalWrite(MOTOR,LOW);
  }

    if(rhumi<=(humidlower-5)){
    //buzzer.beep(80);
    pip(500,6000,50);
  }
  mo();
}

void mo(){
  if(sky2>3){
  if(millis() > delaytime2 && cycle ==1){
    cycle=0;
  }

  if(millis() < delaytime2 && millis() > delaytime){
   digitalWrite(turnout,LOW);
  }

  if(millis() > delaytime && cycle ==0){
    digitalWrite(turnout,HIGH);
    delaytime=millis()+ondu;
    delaytime2=delaytime+offdu;
    cycle=1;
  }
  }
}

void dayc(){
if(millis()>mill){
  mill=millis()+1800000;
  address=60;
  EEPROM.get(address, dayscounter);
  dayscounter=dayscounter+1800000;
  address=60;
  EEPROM.put(address, dayscounter);
}
}

void welcome(){
    lcd2.setCursor(0,0);
  lcd2.print("age Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print("ystem");
  delay(200);
  
  lcd2.setCursor(0,0);
  lcd2.print("tage Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print("System");
  delay(200);
 

  lcd2.setCursor(0,0);
  lcd2.print("itage Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print(" System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("ritage Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print("C System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("eritage Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print("LC System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("Heritage Ncubator");
  lcd2.setCursor(0,1);
  lcd2.print("PLC System");
  delay(1200);
  
 

  lcd2.setCursor(0,0);
  lcd2.print("                ");
  lcd2.setCursor(0,1);
  lcd2.print("            ");
  
 

  lcd2.setCursor(0,0);
  lcd2.print("Customer Care");
  lcd2.setCursor(0,1);
  lcd2.print("0705 008 557");
  lcd2.cursor();
  delay(2000);

   

  lcd2.setCursor(0,0);
  lcd2.print("               ");
  lcd2.setCursor(0,1);
  lcd2.print("              ");
  
  

  lcd2.setCursor(0,0);
  lcd2.print("Model number");
  lcd2.setCursor(0,1);
  lcd2.print("H23M");
  lcd2.cursor();
  delay(2000);
}

void menu_u(){
 customKey = customKeypad.getKey();
 if (customKey == '/')
{
  lcd2.clear();
  menu=0;
  A0s=true;
  modemenu=1;
  nav2=1;
  nav3=1;
  lcd2.setCursor(8,1);
  lcd2.print("              ");
  lcd2.clear();
  }

  while(modemenu==1)
{
  wdt_reset();
customKey = customKeypad.getKey();

if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  
 
//lcd2.print(customKey); 
ikosh[0] = customKey;
strcat(cumm,ikosh);
}
  
  menu1();
  if(nav2==1){
  activemenu();
  }
  buzzer.loop();
   if (customKey) {
    buzzer.beep(80);
  }

   if (customKey == 'C')
   {   
  modemenu=0;
  showp=1;
  showp2=1;
  show();
  strcpy(cumm, "");
  }

     if (customKey == '=')
   {   
    nav2=1;
    nav3=1;
  }
  
  }
}


  void menu1()
  {    
    if (customKey == '.')
{
  menu=menu+1;  
  }
     if (customKey == '*')
{
  menu=menu-1;  
  }

if(menu>8){menu=8;}
if(menu<0){menu=0;}
if(menu==0 && A0s==true)
{
  A0s=false;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=true;
  strcpy(currentmenu, "TEMPERATURE");
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("TEMPERATURE"); 
  lcd2.setCursor(16,1); 
  }
  if(menu==1 && A1s==true)
{
  A0s=true;
  A1s=false;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=true;
   strcpy(currentmenu, "HUMIDITY");

   lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("HUMIDITY");
  lcd2.setCursor(16,1);
  //lcd2.noCursor();
  
  }
  if(menu==2 && A2s==true)
{
  A0s=true;
  A1s=true;
  A2s=false;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=true;
   strcpy(currentmenu, "EGGTUNER");
 
  lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("EGG TUNER");
  lcd2.setCursor(16,1);
  //lcd2.noCursor();
  }
  if(menu==3 && A3s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=false;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=true;
   strcpy(currentmenu, "INCDAYS");
  
  lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("INC DAYS");
  lcd2.setCursor(16,1);
  //lcd2.noCursor();
  }

  if(menu==4 && A4s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=false;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=true;
  strcpy(currentmenu, "FACTORYRESET");
  

  lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("FACTORY RESET");
  lcd2.setCursor(16,1);

  }

    if(menu==5 && A5s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=false;
  A6s=true;
  A7s=true;
  A8s=true;
  strcpy(currentmenu, "DAYSCOUNTER");
  

  lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("DAYS COUNTER");
  lcd2.setCursor(16,1);
 
  }
  

      if(menu==6 && A6s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=false;
  A7s=true;
  A8s=true;
  strcpy(currentmenu, "DAYSRESET");
  
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("START CYCLE");
  lcd2.setCursor(16,1);
  }

    if(menu==7 && A7s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=false;
  A8s=true;
  strcpy(currentmenu, "PRESETMODES");
  
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("PRESET MODES");
  lcd2.setCursor(16,1);
  }


      if(menu==8 && A8s==true)
{
  A0s=true;
  A1s=true;
  A2s=true;
  A3s=true;
  A4s=true;
  A5s=true;
  A6s=true;
  A7s=true;
  A8s=false;
  strcpy(currentmenu, "CALIBRATE");
  
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("CALIBRATE");
  lcd2.setCursor(16,1);
  }

    }

void activemenu(){
  
  if (customKey == '=' && nav2==1)
  {
    
    if(strcmp (currentmenu, "DAYSRESET" )==0 && (nav3==1))
  { strcpy(cumm, "");
    strcpy(ikosh, "");
  wdt_reset();
  dayscounter=0;
  address=60;
  EEPROM.put(address, dayscounter);
  lcd2.setCursor(0,1);
  lcd2.print("N-cycle success");
  wdt_reset();
  delay(2000);
  lcd2.clear();
  wdt_reset();
  //modemenu=0;
  showp=1;
  showp2=1;
  //show();
  strcpy(cumm, "");
  cm();
  nav2=0;
  nav3=0;
  }

 if(strcmp (currentmenu, "DAYSCOUNTER" )==0 && (nav3==1))
 { strcpy(cumm, "");
 strcpy(cumm1, "");
      strcpy(ikosh, "");
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print("DAYS COUNTER");
  customKey2 = customKeypad.getKey();
  address=60;
  EEPROM.get(address, dayscounter);
   address=45;
  EEPROM.get(address, incubation);
  sky=dayscounter/86400000;
  temp_str = String(sky);
  temp_str.toCharArray(cumm,15);
  strcat(cumm,"dys");
  sky=incubation/86400000;
  temp_str = String(sky);
  temp_str.toCharArray(cumm1,15);
  
  lcd2.setCursor(0,1);
  lcd2.print("DS=");
  lcd2.setCursor(3,1);
  lcd2.print(cumm);
  lcd2.setCursor((strlen(cumm)+4),1);
  lcd2.print("TD=");
  lcd2.setCursor((strlen(cumm)+7),1);
  lcd2.print(cumm1);
  customKey2="";
    while((customKey2!='C')){
        wdt_reset();
      customKey2 = customKeypad.getKey(); 
  showp=1;
  showp2=1;
  strcpy(cumm, "");
   cm();
   nav2=0;
   nav3=0;
  
        }//*/
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
   strcpy(cumm1, "");
  strcpy(cumm, "");
  lcd2.clear();
      }


    if(strcmp (currentmenu, "FACTORYRESET" )==0 && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
      customKey2 = customKeypad.getKey();
      while(customKey2!='=' && (nav3==1)){
      customKey2 = customKeypad.getKey();
  wdt_reset();
  lcd2.setCursor(0,0);
  lcd2.print("Enter resetcode");
  lcd2.setCursor(0,1);
  menucom3(4,0);
  address=50;
  EEPROM.get(address, serial);
  if((atof(cumm)+serial)==26434){ 
  lcd2.setCursor(0,0);
  lcd2.print("Resetting.....  ");
  lcd2.setCursor(0,1);
  lcd2.print("                ");
  wdt_reset();
  delay(1000);
  wdt_reset();
  facr();
  }else{
 
  lcd2.setCursor(0,1);
  lcd2.print("Wrong passcode");
  delay(2000);
  wdt_reset();
  lcd2.setCursor(0,1);
  lcd2.print("                 ");
    } 
    if (customKey2 =='C')
   {  
  showp=1;
  showp2=1;
  strcpy(cumm, "");
  cm();
  nav2=0;
  nav3=0;
  }
     }
     wdt_reset();
      delay(4000);
      wdt_reset();
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
  strcpy(cumm, "");
      }


if(strcmp (currentmenu, "EGGTUNER" )==0 && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
customKey2 = customKeypad.getKey();
        while((customKey2!='=') && (nav3==1)){
          customKey2 = customKeypad.getKey();
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.ON DU 2.OFF DU");
lcd2.setCursor(0,1);
menucom3(0,0); 

  if((atoi(cumm)==1))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 1->10 SEC");
  lcd2.setCursor(0,1);
  address=40;
  EEPROM.get(address, ondu);
  sky=ondu/1000;
  temp_str = String(sky);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
   nav3=1;
   menucom3(1,0);


if(strlen(cumm)>=1 && strlen(cumm)<=2){
ondu=atol(cumm);
ondu=ondu*1000;
  address=40;
  EEPROM.put(address, ondu);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 strcpy(cumm, "");
  delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();
}

   bl();
        }

  if((atoi(cumm)==2))
{

lcd2.clear();                 
lcd2.setCursor(0,0);
  lcd2.print("R: 30->300 MIN");
  lcd2.setCursor(0,1);
  address=35;
  EEPROM.get(address, offdu);
  sky=offdu/60000;
  temp_str = String(sky);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
  strcpy(cumm, "");

  lcd2.setCursor(0,1);
  nav3=1;
  menucom3(2,0);

if(strlen(cumm)>=2 && strlen(cumm)<=3 ){
offdu=atol(cumm);
offdu=offdu*60000;
  address=35;
  EEPROM.put(address, offdu);

  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
  delay(1000);
 lcd2.clear();
}
else
{
 
 lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();
}

bl();
        }else{
 lcd2.setCursor(0,1);
}

    }
      wdt_reset();
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
   strcpy(cumm, "");
   bl();
      }

 if(strcmp (currentmenu, "INCDAYS" )==0 && (nav3==1))
 {
      strcpy(cumm, "");
      strcpy(ikosh, "");
      customKey2 = customKeypad.getKey();
      while(customKey2 != '=' && (nav3==1)){
wdt_reset();

lcd2.clear();
lcd2.setCursor(0,0);
  lcd2.print("1 -> 30 DAYS");
  lcd2.setCursor(0,1);

  address=45;
  EEPROM.get(address, incubation);
  sky=incubation/86400000;//milliseconds per day
  temp_str = String(sky);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);

menucom3(1,0);
if(strlen(cumm)>=1 && strlen(cumm)<=2){
  incubation=atol(cumm);
  incubation=incubation*86400000;
  address=45;
  EEPROM.put(address, incubation);
 lcd2.setCursor(0,1);
 lcd2.print("                  ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 delay(1000);
 lcd2.clear();
  }
 bl();
  }
      wdt_reset();
      delay(2000);
      wdt_reset();
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
  strcpy(cumm, "");
      }


      if(strcmp (currentmenu, "TEMPERATURE" )==0 && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
customKey2 = customKeypad.getKey();
 while((customKey2!='=') && (nav3==1)){
customKey2 = customKeypad.getKey();
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.HIGH  2.LOW ");
lcd2.setCursor(0,1);
lcd2.print("3.CRIT");
lcd2.setCursor(7,1);
//nav3=1;
menucom3(0,7);

  if((atoi(cumm)==1))
{

  lcd2.clear();               
  lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 DEG");
  lcd2.setCursor(0,1);
  address=adh;
  EEPROM.get(address, tempupper);
  temp_str = String(tempupper);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
   lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
   nav3=1;
   menucom3(2,0);

if(strlen(cumm)>=1 && strlen(cumm)<=3){
tempupper=atol(cumm);
  address=adh;
  EEPROM.put(address, tempupper);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();

}
    bl();
        }

 if((atoi(cumm)==2))
{
  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 DEG");
  lcd2.setCursor(0,1);
  address=adl;
  EEPROM.get(address, templower);
  temp_str = String(templower);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
   lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
    nav3=1;
   menucom3(2,0);


if(strlen(cumm)>=1 && strlen(cumm)<=3){
templower=atol(cumm);
  address=adl;
  EEPROM.put(address, templower);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
delay(1000);
lcd2.clear();

}
 bl();
        }

  if((atoi(cumm)==3))
{

lcd2.clear();                 
lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 DEG");
  lcd2.setCursor(0,1);
  address=adc;
  EEPROM.get(address, tempcritical);
  temp_str = String(tempcritical);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
  strcpy(cumm, "");

  lcd2.setCursor(0,1);
  nav3=1;
  menucom3(2,0);

if(strlen(cumm)>=1 && strlen(cumm)<=3 ){
tempcritical=atol(cumm);
  address=adc;
  EEPROM.put(address, tempcritical);

  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 delay(1000);
 lcd2.clear();
}
else
{
 
 lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();

}

bl();
        }else{
 lcd2.setCursor(0,1);
}
bl();
    cm();
    }
wdt_reset();
   strcpy(cumm, "");
   
      }


if(strcmp (currentmenu, "HUMIDITY" )==0  && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
customKey2 = customKeypad.getKey();
        while((customKey2!='=' )  && (nav3==1)){
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.HIGH  2.LOW ");
lcd2.setCursor(0,1);
lcd2.print("3.CRIT");
lcd2.setCursor(7,1);
menucom3(0,7); 

  if((atoi(cumm)==1))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 %");
  lcd2.setCursor(0,1);
  address=adhh;
  EEPROM.get(address, humidupper);
  temp_str = String(humidupper);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
  nav3=1;
   menucom3(2,0);

if(strlen(cumm)>=1 && strlen(cumm)<=3){
humidupper=atol(cumm);
  address=adhh;
  EEPROM.put(address, humidupper);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
  delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();
}

bl();
        }

 if((atoi(cumm)==2))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 %");
  lcd2.setCursor(0,1);
  address=adhl;
  EEPROM.get(address, humidlower);
  temp_str = String(humidlower);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
   nav3=1;
   menucom3(2,0);


if(strlen(cumm)>=1 && strlen(cumm)<=3){
humidlower=atol(cumm);
  address=adhl;
  EEPROM.put(address, humidlower);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
  delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
 delay(1000);
 lcd2.clear();
}

bl();
        }

  if((atoi(cumm)==3))
{

lcd2.clear();                 
lcd2.setCursor(0,0);
  lcd2.print("R: 0->100 %");
  lcd2.setCursor(0,1);
  address=adhc;
  EEPROM.get(address, humidcritical);
  temp_str = String(humidcritical);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
  strcpy(cumm, "");

  lcd2.setCursor(0,1);
  nav3=1;
  menucom3(2,0);

if(strlen(cumm)>=1 && strlen(cumm)<=3 ){
humidcritical=atol(cumm);
  address=adhc;
  EEPROM.put(address, humidcritical);

  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
  delay(1000);
 lcd2.clear();
}
else
{
 
 lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Out of range");
  delay(1000);
 lcd2.clear();
}

bl();
        }else{
 lcd2.setCursor(0,1);

}
bl();
cm();
    }
wdt_reset();
   strcpy(cumm, "");
   
      }


  if(strcmp (currentmenu, "PRESETMODES" )==0  && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
customKey2 = customKeypad.getKey();
        while(customKey2!='='  && (nav3==1)){
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.Cn  2.Dk  3.Gs");
lcd2.setCursor(0,1);
lcd2.print("4.Pn 5.Oth");
lcd2.setCursor(13,1);
menucom3(0,13); 
if((atoi(cumm)==1) || (atoi(cumm)==2) || (atoi(cumm)==3) || (atoi(cumm)==4) || (atoi(cumm)==5) )
{
  mode=atol(cumm);
  address=55;
EEPROM.put(address, mode);
  data();
  lcd2.setCursor(0,1);
  lcd2.print("Mode success");
  delay(2000);
  wdt_reset();
  lcd2.setCursor(0,1);
  lcd2.print("                 ");
}else{
 mode=5;
address=55;
EEPROM.put(address, mode);
}
bl();
  }
        wdt_reset();
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
  strcpy(cumm, "");
      }//*/


  if(strcmp (currentmenu, "CALIBRATE" )==0  && (nav3==1))
    { strcpy(cumm, "");
      strcpy(ikosh, "");
customKey2 = customKeypad.getKey();
        while((customKey2!='=')  && (nav3==1)){
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.TEMP  2.HUMI");
lcd2.setCursor(0,1);
menucom3(0,0); 

  if((atoi(cumm)==1))
{
strcpy(cumm, "");
strcpy(ikosh, "");
nav3=1;
while((customKey2!='=')  && (nav3==1)){
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.SET H 2.SET L");
lcd2.setCursor(0,1);
menucom3(0,0); 

  if((atoi(cumm)==1))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 1 -> 99 ");
  lcd2.setCursor(0,1);
  address=65;
  EEPROM.get(address, hc);
  temp_str = String(hc);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
   nav3=1;
   menucom3(1,0);


if(strlen(cumm)>=1 && strlen(cumm)<=2){
hc=atol(cumm);

  address=65;
  EEPROM.put(address, hc);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 strcpy(cumm, "");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
delay(1000);
lcd2.clear();
}

bl();
  
        }

  if((atoi(cumm)==2))
{
  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 1 -> 99 ");
  lcd2.setCursor(0,1);
  address=70;
  EEPROM.get(address, lc);
  temp_str = String(lc);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
  nav3=1;
   menucom3(1,0);


if(strlen(cumm)>=1 && strlen(cumm)<=2){
lc=atol(cumm);

  address=70;
  EEPROM.put(address, lc);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 strcpy(cumm, "");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
delay(1000);
 lcd2.clear();
}

bl();
        }else{
 lcd2.setCursor(0,1);
}

    }
        }

  if((atoi(cumm)==2))
{
strcpy(cumm, "");
strcpy(ikosh, "");
nav3=1;
while((customKey2!='=')  && (nav3==1)){
wdt_reset();
lcd2.setCursor(0,0);
lcd2.print("1.SET H 2.SET L");
lcd2.setCursor(0,1);
menucom3(0,0); 

  if((atoi(cumm)==1))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 1 -> 99 ");
  lcd2.setCursor(0,1);
  address=75;
  EEPROM.get(address, hhc);
  temp_str = String(hhc);
  temp_str.toCharArray(cumm,15); 
  
   lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
   nav3=1;
   menucom3(1,0);


if(strlen(cumm)>=1 && strlen(cumm)<=2){
hhc=atol(cumm);

  address=75;
  EEPROM.put(address, hhc);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 strcpy(cumm, "");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
delay(1000);
lcd2.clear();
}

bl();
  
        }

  if((atoi(cumm)==2))
{

  lcd2.clear();                  
  lcd2.setCursor(0,0);
  lcd2.print("R: 1 -> 99 ");
  lcd2.setCursor(0,1);
  address=80;
  EEPROM.get(address, hlc);
  temp_str = String(hlc);
  temp_str.toCharArray(cumm,15); 
  
  lcd2.setCursor(9,1);
  lcd2.print(cumm);
   strcpy(cumm, "");
   lcd2.setCursor(0,1);
  nav3=1;
   menucom3(1,0);


if(strlen(cumm)>=1 && strlen(cumm)<=2){
hlc=atol(cumm);

  address=80;
  EEPROM.put(address, hlc);
  lcd2.setCursor(0,1);
 lcd2.print("                ");
 lcd2.setCursor(0,1);
 lcd2.print("Update success");
 strcpy(cumm, "");
 delay(1000);
 lcd2.clear();
}
else
{

  lcd2.setCursor(0,1);
 lcd2.print("                ");
lcd2.setCursor(0,1);
lcd2.print("Out of range");
delay(1000);
 lcd2.clear();
}

bl();
        }else{
 lcd2.setCursor(0,1);
}

    } }else{
 lcd2.setCursor(0,1);
}

    }
      wdt_reset();
  showp=1;
  showp2=1;
  cm();
  nav2=0;
  nav3=0;
   strcpy(cumm, "");
   
      }

    }
  }

void menucom3(int h, int a){
  customKey2 = customKeypad.getKey();
  while((customKey2!='=') && (nav3==1)){
    wdt_reset();
    customKey2 = customKeypad.getKey();

while((strlen(cumm)<=h) && (customKey2 !='=') && (nav3==1)){
wdt_reset();
customKey2 = customKeypad.getKey();
if (customKey2 && (customKey2 !='.') && (customKey2 !='=') && (customKey2 !='C') && (customKey2 !='/') && (customKey2 !='*') && (customKey2 !='-') && (customKey2 !='+')){  
lcd2.print(customKey2);
ikosh[0] = customKey2;
strcat(cumm,ikosh);

}

if(strcmp(cumm,"")==0){
if (customKey2 =='C')
   {  
  lcd2.clear();
  customKey2="";
  showp=1;
  showp2=1;
  strcpy(cumm, "");
  nav3=0;
  cm();
  }
}
  
if (customKey2 == 'C')//
   {
  strcpy(cumm,"");
  lcd2.setCursor(a,1);
  lcd2.print("        ");
  lcd2.setCursor(a,1);
  }
buzzer.loop();
   if (customKey2) {
    buzzer.beep(80);
 }
if (customKey2 == '=')//
   {
    lcd2.clear();
    customKey2="";
    nav3=0;
    cm();
  }

  }

   if(strcmp(cumm,"")==0){
  if (customKey2 =='C')
   { 
  lcd2.clear();
  customKey2="";
  showp=1;
  showp2=1;
  strcpy(cumm, "");
  nav3=0;
  cm();
  }
  }
if (customKey2 == 'C')//
   {
  strcpy(cumm,"");
  lcd2.setCursor(a,1);
  lcd2.print("        ");
  lcd2.setCursor(a,1);
  }

if (customKey2 == '=')//
   {
    nav3=0;
    lcd2.clear();
    customKey2="";
    cm();
  }

  }
  }  
   
  void facr(){
  tempupper=40;
  templower=20;
  tempcritical=45;
  humidupper=40;
  humidlower=20;
  humidcritical=45;
  dayscounter=0;
  offdu=3600000;
  ondu=10000;
  lc=0;
  hc=0;
  incubation=2592000000;
  intial=2;
  serial=1;
  mode=1;
  address=5;
  EEPROM.put(address,tempupper);
  address=10;
  EEPROM.put(address,templower);
  address=15;
  EEPROM.put(address,tempcritical);
  address=20;
  EEPROM.put(address,humidupper);
  address=25;
  EEPROM.put(address,humidlower);
  address=30;
  EEPROM.put(address,humidcritical);
  address=35;
  EEPROM.put(address,offdu);
  address=40;
  EEPROM.put(address,ondu);
  address=45;
  EEPROM.put(address,incubation);
  address=0;
  EEPROM.put(address,intial);
  address=50;
  EEPROM.put(address,serial);
  address=55;
  EEPROM.put(address,mode);
  address=60;
  EEPROM.put(address,dayscounter);
  address=65;
  EEPROM.put(address,hc);
  address=70;
  EEPROM.put(address,lc);


  lcd2.setCursor(0,0);
  lcd2.print("Reset success");
  wdt_reset();
  delay(2000);   
  wdt_reset(); 
}

   void show(){   
  if(showp==1){
  lcd2.clear();
  lcd2.setCursor(0,0);
  address=55;
  EEPROM.get(address, mode);
  
  if(mode==1){
  strcpy(cumm,"Cn");
  }
    if(mode==2){
  strcpy(cumm,"Dk");
  }
    if(mode==3){
  strcpy(cumm,"Gs");
  }
    if(mode==4){
  strcpy(cumm,"Pn");
  }
    if(mode==5){
  strcpy(cumm,"Oth");
  }

  lcd2.print("M");
  lcd2.setCursor(1,0);
  lcd2.print(cumm);
  
  
  
  
  lcd2.setCursor(5,0);
  lcd2.print("T=");
  //lcd2.setCursor(7,0);
  //lcd2.print(cumm);
  lcd2.setCursor(11,0);
  lcd2.print("H=");
  //lcd2.setCursor(13,0);
  //lcd2.print(cumm1);
  lcd2.setCursor(0,1);

 
  //sky=(incubation/86400000)-sky;
  //temp_str = String(sky);
  //temp_str.toCharArray(cumm,15);
  lcd2.setCursor(0,1);
  lcd2.print("ReDays=");
  //lcd2.setCursor(7,1);
  //lcd2.print(cumm);
  //lcd2.print(offdu);
  lcd2.setCursor(0,1);
  showp=0;
  showp2=1;
  }   
    }
  

void update(){
  temp_str = String(rtemp);
  temp_str.toCharArray(cumm,15);
  temp_str = String(rhumi);
  temp_str.toCharArray(cumm1,15);

  lcd2.setCursor(7,0);
  lcd2.print(cumm);

  lcd2.setCursor(13,0);
  lcd2.print(cumm1);
  
  address=60;
  EEPROM.get(address, dayscounter);
  address=45;
  EEPROM.get(address, incubation);
  sky=dayscounter/86400000;
  sky=(incubation/86400000)-sky;
  temp_str = String(sky);
  temp_str.toCharArray(cumm,15);

  lcd2.setCursor(7,1);
  lcd2.print(cumm);
}

void rup(){
  if(millis()> ssd){
    update();
    ssd=millis()+2000;
  }
}


  void show2(){
     if(showp2==1){
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print("OVERDAYS-SHUT");
      showp2=0;
     }
  }
  void cm(){
           if(A0s==false){
      A0s=true;
    }
    if(A1s==false){
      A1s=true;
    }
        if(A2s==false){
      A2s=true;
    }
 
        if(A3s==false){
      A3s=true;
    }
        if(A4s==false){
      A4s=true;
    }
        if(A5s==false){
      A5s=true;
    }
        if(A6s==false){
      A6s=true;
    }
        if(A7s==false){
      A7s=true;
    }
           if(A8s==false){
      A8s=true;
    }
  }

  void bl(){
        if (customKey2 =='=')
   {  
  cm();
  }
    
if (customKey2 =='C')
   { 
  showp=1;
  showp2=1;
  strcpy(cumm, "");
  nav2=0;
  cm();
  }
  }

  void cmode(){
  address=55;
  EEPROM.get(address,mode);

  if(mode==1){
  adl=175;
  adh=180;
  adc=185;
  adhl=190;
  adhh=195;
  adhc=200;
  }
    if(mode==2){
  adl=145;
  adh=150;
  adc=155;
  adhl=160;
  adhh=165;
  adhc=170;
  }
    if(mode==3){
  adl=115;
  adh=120;
  adc=125;
  adhl=130;
  adhh=135;
  adhc=140;
  }
      if(mode==4){
  adl=85;
  adh=90;
  adc=95;
  adhl=100;
  adhh=105;
  adhc=110;
  }
    if(mode==5){
  adl=10;
  adh=5;
  adc=15;
  adhl=25;
  adhh=20;
  adhc=30;
  }
  }

  void data(){
   cmode();
  if(mode==1){
  var=35;
  EEPROM.put(adl,var);
  var=40;
  EEPROM.put(adh,var);
  var=45;
  EEPROM.put(adc,var);
  var=50;
  EEPROM.put(adhl,var);
  var=70;
  EEPROM.put(adhh,var);
  var=75;
  EEPROM.put(adhc,var);
  }
    if(mode==2){
      var=35;
  EEPROM.put(adl,var);
  var=41;
  EEPROM.put(adh,var);
  var=46;
  EEPROM.put(adc,var);
  var=50;
  EEPROM.put(adhl,var);
  var=75;
  EEPROM.put(adhh,var);
  var=80;
  EEPROM.put(adhc,var);
  }
    if(mode==3){
      var=35;
  EEPROM.put(adl,var);
  var=40;
  EEPROM.put(adh,var);
  var=45;
  EEPROM.put(adc,var);
  var=50;
  EEPROM.put(adhl,var);
  var=75;
  EEPROM.put(adhh,var);
  var=80;
  EEPROM.put(adhc,var);
  }
      if(mode==4){
        var=35;
  EEPROM.put(adl,var);
  var=41;
  EEPROM.put(adh,var);
  var=46;
  EEPROM.put(adc,var);
  var=50;
  EEPROM.put(adhl,var);
  var=70;
  EEPROM.put(adhh,var);
  var=75;
  EEPROM.put(adhc,var);
  }
    if(mode==5){
      var=20;
  EEPROM.put(adl,var);
  var=45;
  EEPROM.put(adh,var);
  var=50;
  EEPROM.put(adc,var);
  var=30;
  EEPROM.put(adhl,var);
  var=60;
  EEPROM.put(adhh,var);
  var=75;
  EEPROM.put(adhc,var);
  }

  }
 long pip1=0,pip2=0;
 int cycle2=0;
  void pip(int on,int off,int lia){
  if(millis() > pip1 && cycle2 ==1){
    cycle2=0;
  }
  if(millis() < pip1 && millis() > pip2){
   //digitalWrite(turnout,LOW);
  }
  if(millis() > pip2 && cycle2 ==0){
    buzzer.beep(lia);
    pip2=millis()+on;
    pip1=pip2+off;
    cycle2=1;
  }
  }

 