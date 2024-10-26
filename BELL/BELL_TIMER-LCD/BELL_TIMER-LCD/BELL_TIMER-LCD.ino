#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <ezBuzzer.h>
#include<avr/wdt.h>
#include <I2C_RTC.h>
#include <Wire.h>


static DS3231 RTC;

const int rs = 7, en = 8, d4 = 10, d5 = 9, d6 = 11, d7 = 12;
LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {

{'/','3','2','1'},//membrane keypad

{'*','6','5','4'},

{'.','9','8','7'},

{'+','=','0','C'}

};//*/

/*char hexaKeys[ROWS][COLS] = {

{'C','7','4','1'},//plastic keypad

{'0','8','5','2'},

{'=','9','6','3'},

{'+','.','*','/'}

};//*/

byte rowPins[ROWS] = {0, 2, 1, 4};
byte colPins[COLS] = {14, 15, 16, 17};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

 char cumm[45];
 char ikosh[20];
 char tim[45];
 char alarm1[45];
 char alarm2[45];
 char password[7];
 char password2[9];
 char nameschool[45];
 char currentmenu[20];
 int modemenu=0;
 int showp=1;
 int address,pause=0,state1=0,state2=0,busy1=0,busy2=0;
 int menu=0;
 ezBuzzer buzzer(13);
 int ac=0;
 int alarm10=0;
 bool A0s=true,A1s=true,A2s=true, A3s=true,A4s =true,A5s=true,A6s=true,A7s =true,A8s=true;
 long count2,count3,count4,intial,rin1,rin2,rin3,rin4; 
 uint8_t day; uint8_t month; uint16_t year;
 uint8_t hour; uint8_t minute; uint8_t second;
 int na=0;
 int bell=5;
 int eme=6;
 int manu=3;
 int pass1=1;
 int pass11=1;
 int pass2=0;
 int pass22=0;
 int pass3=0;
 int pass33=0;
 int tr=0;
 int tr1=0;
 long startt,stime,wait=0;
 int start=1,run,op;
 char customKey, customKey2;


 void setup() {
  pinMode(bell,OUTPUT);
  pinMode(eme,INPUT_PULLUP);
  pinMode(manu,INPUT_PULLUP);
  digitalWrite(bell,LOW);
  showp=1;
  lcd2.begin(16, 2);
  if(!RTC.begin()){
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("RTC ERR");
  lcd2.setCursor(8,0);
  lcd2.print("CHK POW");
  lcd2.setCursor(0,1);
  lcd2.print("CNT: 0705 008 557");
  while(1){
  wdt_reset();
  }
  }
  if (!RTC.isRunning())
  {
      lcd2.clear();
      lcd2.setCursor(0,0);
     // lcd2.print("RTC Err, Set time!");
     // delay(2000);
    //while(1){wdt_reset();}
  }
  address=35;
  EEPROM.get(address, intial);
  if(intial!=5){
   strcpy(password, "000000");
   strcpy(nameschool,"HERITAGE MACHINES LTD");
   intial=5;
  address=0;
  EEPROM.put(address, na);
  address=2;
  EEPROM.put(address, na);
  address=4;
  EEPROM.put(address, na);
  address=6;
  EEPROM.put(address, na);
  address=8;
  EEPROM.put(address, na);
  address=10;
  EEPROM.put(address, na);
  address=12;
  EEPROM.put(address, na);
  address=25;
  EEPROM.put(address, password);
  address=35;
  EEPROM.put(address, intial);
  address=900;
  EEPROM.put(address,nameschool);
  lcd2.setCursor(0,0);
  lcd2.print("Setting complete");
  delay(2000);
 }

   lcd2.setCursor(0,0);
  lcd2.print("age Machines");
  lcd2.setCursor(0,1);
  lcd2.print("ystem");
  delay(200);
  
  lcd2.setCursor(0,0);
  lcd2.print("tage Machines");
  lcd2.setCursor(0,1);
  lcd2.print("System");
  delay(200);
 

  lcd2.setCursor(0,0);
  lcd2.print("itage Machines");
  lcd2.setCursor(0,1);
  lcd2.print(" System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("ritage Machines");
  lcd2.setCursor(0,1);
  lcd2.print("C System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("eritage Machines");
  lcd2.setCursor(0,1);
  lcd2.print("LC System");
  delay(200);
  

  lcd2.setCursor(0,0);
  lcd2.print("Heritage Machines");
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

 


  wdt_disable();
  wdt_enable(WDTO_4S);
 }

void loop() {
wdt_reset();
buzzer.loop();
buts();
checka();
alarmring();//&& millis()> wait+2000
if(op==1 ){
  ring(rin1,rin2,rin3,rin4);
}
lcd2.setCursor(11,0);
lcd2.print(op);
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){ 
ikosh[0] = customKey;
strcat(cumm,ikosh);
}
if (customKey == '/')
{
  menu=0;
  A0s=true;
  modemenu=1;
  }
while(modemenu==1)
{
wdt_reset();
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  
ikosh[0] = customKey;
strcat(cumm,ikosh);
}
  menu1();
  activemenu();
  buzzer.loop();
  if(customKey) {
  buzzer.beep(50);
   }

   if (customKey == 'C')
   {   
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
  }
  }
 show();
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
  strcpy(currentmenu, "SET DATE");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("SET DATE"); 
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
   strcpy(currentmenu, "SET TIME");
   lcd2.clear();
   lcd2.setCursor(0,0);
   lcd2.print("              ");
    lcd2.setCursor(0,0);
  lcd2.print("SET TIME");
  lcd2.setCursor(16,1);
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
   strcpy(currentmenu, "INST NAME");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("INST NAME");
  lcd2.setCursor(16,1);
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
   strcpy(currentmenu, "SET CLOCK SYSTEM");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("SET CLOCK SYSTEM");
  lcd2.setCursor(16,1);
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
  strcpy(currentmenu, "ALARM ON/OFF");
  
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("ALARM ON/OFF");
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
   strcpy(currentmenu, "SET ALARM");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("SET ALARM");
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
  strcpy(currentmenu, "CLEAR ALARMS");
  
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("CLR/MDFY ALARMS");
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
  strcpy(currentmenu, "FACTORY RESET");
  
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("FACTORY RESET");
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
   strcpy(currentmenu, "MAN SERIAL ID");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("              ");
  lcd2.setCursor(0,0);
  lcd2.print("MAN SERIAL ID");
  lcd2.setCursor(16,1);
  }   
    }

    void activemenu(){
  if (customKey == '=')
  {
     if(strcmp (currentmenu, "SET DATE" )==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
    wdt_reset();
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("SET DATE DAY:");
  lcd2.setCursor(0,1);

     menucom3(2,"SET DATE DAY:");
     if(!(strcmp(cumm,"")==0)){
     day=atoi(cumm);
     }else{
      String temp=String(RTC.getDay());
      temp.toCharArray(cumm, 2);
      strncpy(password2+4,cumm,2);
     }
     strcpy(cumm,"");
    menucom3(2,"SET DATE MONTH:");
    if(!(strcmp(cumm,"")==0)){
     month=atoi(cumm);
     }else{
      String temp=String(RTC.getMonth());
      temp.toCharArray(cumm, 2);
      strncpy(password2+4,cumm,2);
     }
     strcpy(cumm,"");

     menucom3(4,"SET DATE YEAR:");
     if(!(strcmp(cumm,"")==0)){
     year=atoi(cumm);
      }else{
      String temp=String(RTC.getYear());
      temp.toCharArray(cumm, 4);
      strncpy(password2+4,cumm,4);
     }
     strcpy(cumm,"");
     RTC.setDate(day, month, year);
  wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }

  if(strcmp (currentmenu, "FACTORY RESET")==0)
  { strcpy(cumm, "");
  strcpy(ikosh, "");
  customKey2 = customKeypad.getKey();
  while(customKey2!='='){
  wdt_reset();
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Enter resetcode");
  lcd2.setCursor(0,1);
  menucom3(5,"Enter resetcode");
  if(atof(cumm)==26434){
     
  lcd2.setCursor(0,0);
  lcd2.print("Resetting.....  ");
  lcd2.setCursor(0,1);
  lcd2.print("                ");
  wdt_reset();
  delay(1000);
  wdt_reset();
  reset();
  }else{
 
  lcd2.setCursor(0,1);
  lcd2.print("Wrong password");
    } 
    if (customKey2 =='C')
   {  
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
break;
  }
     }
  wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }

  if(strcmp (currentmenu, "SET CLOCK SYSTEM")==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("SEL 12/24 MODE");
    lcd2.setCursor(0,1);
     menucom4(2,0,1);
     if(atoi(cumm)==24 || atoi(cumm)==12){
     if(atoi(cumm)==24){
      RTC.setHourMode(CLOCK_H24);
      lcd2.setCursor(0,1);
      lcd2.print("24HR SUC");
      delay(1000);
      wdt_reset();
      }
        if(atoi(cumm)==12){
        RTC.setHourMode(CLOCK_H12);
        lcd2.setCursor(0,1);
        lcd2.print("12HR SUC");
        delay(1000);
        wdt_reset();
      }
      lcd2.setCursor(0,0);
      lcd2.print(RTC.getHourMode(), DEC);
      lcd2.setCursor(0,1);
      lcd2.print("CLK SYS SUCC");
      delay(1000);
      wdt_reset();
     }else{
      lcd2.setCursor(0,0);
      lcd2.print(RTC.getHourMode(), DEC);
      lcd2.setCursor(0,1);
      lcd2.print("CLK SYS ERR");
      delay(1000);
     }
           wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }


   if(strcmp (currentmenu, "INST NAME" )==0)
    { strcpy(cumm, "");
    strcpy(ikosh, "");
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("SET INST NAME");
    lcd2.setCursor(0,1);
     menucom4(15,0,1);
     if(!(strcmp(cumm,"")==0)){
     strcpy(nameschool,cumm);
     }else{
      strcpy(nameschool,"HERITAGE MACHINES");
     }
     address=900;
     EEPROM.put(address,nameschool);
     if(strcmp(cumm,"")==0){
      lcd2.setCursor(0,1);
      lcd2.print("INST ERR");
      delay(1000);
     }else{
      lcd2.setCursor(0,1);
      lcd2.print("INST NAME SET");
      delay(1000);
     }
     strcpy(cumm, "");
  wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }

    if(strcmp (currentmenu, "SET TIME" )==0)
    {strcpy(cumm, "");
    strcpy(ikosh, "");
    wdt_reset();
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("SET TIME HOUR:");
    lcd2.setCursor(0,1);
     menucom3(2,"SET TIME HOUR:");
     if(!(strcmp(cumm,"")==0)){
     hour=atoi(cumm);
     }else{
      hour=RTC.getHours();
     }
     strcpy(cumm,"");
     menucom3(2,"SET TIME MIN:");
     if(!(strcmp(cumm,"")==0)){
     minute=atoi(cumm);
     }else{
      minute=RTC.getMinutes();
     }
     strcpy(cumm,"");
     menucom3(2,"SET TIME SECS:");
     if(!(strcmp(cumm,"")==0)){
     second=atoi(cumm);
     }else{
     second=RTC.getSeconds();
     }
     strcpy(cumm,"");
     RTC.setTime(hour, minute, second);
  wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }
    if(strcmp (currentmenu, "ALARM ON/OFF")==0)
    {
     strcpy(cumm, "");
     strcpy(ikosh, "");
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("0.S");
     lcd2.setCursor(4,0);
     lcd2.print("1.M");
     lcd2.setCursor(8,0);
     lcd2.print("2.T");
     lcd2.setCursor(12,0);
     lcd2.print("3.W");
     lcd2.setCursor(0,1);
     lcd2.print("4.T");
     lcd2.setCursor(4,1);
     lcd2.print("5.F");
     lcd2.setCursor(8,1);
     lcd2.print("6.SA");
     lcd2.setCursor(13,1);
     menucom4(1,13,1);
     if(atoi(cumm)==0 || atoi(cumm)==1 || atoi(cumm)==2 || atoi(cumm)==3 || atoi(cumm)==4 || atoi(cumm)==5 || atoi(cumm)==6){
      update2(atoi(cumm));
     }
  wdt_reset();
  delay(2000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      }

     if(strcmp (currentmenu, "SET ALARM")==0)
    { 
     strcpy(cumm, "");
     strcpy(ikosh, "");
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("0.S");
     lcd2.setCursor(4,0);
     lcd2.print("1.M");
     lcd2.setCursor(8,0);
     lcd2.print("2.T");
     lcd2.setCursor(12,0);
     lcd2.print("3.W");
     lcd2.setCursor(0,1);
     lcd2.print("4.T");
     lcd2.setCursor(4,1);
     lcd2.print("5.F");
     lcd2.setCursor(8,1);
     lcd2.print("6.SA");
     lcd2.setCursor(13,1);
     menucom4(1 ,13,1);
     if(atoi(cumm)==0 || atoi(cumm)==1 || atoi(cumm)==2 || atoi(cumm)==3 || atoi(cumm)==4 || atoi(cumm)==5 || atoi(cumm)==6){
       update(atoi(cumm));
     }
  wdt_reset();
  delay(1000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
      } 
      if(strcmp (currentmenu, "CLEAR ALARMS")==0)
    { int thru=0;
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("1.MODIFY 2.CLR");
     menucom4(1 ,0,1);
      if(atoi(cumm)==1){
         thru=1;
      }
      if(atoi(cumm)==2){
         thru=2;
      }
       
     if(thru==1 || thru==2){
     strcpy(cumm, "");
     strcpy(ikosh, "");
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("0.S");
     lcd2.setCursor(4,0);
     lcd2.print("1.M");
     lcd2.setCursor(8,0);
     lcd2.print("2.T");
     lcd2.setCursor(12,0);
     lcd2.print("3.W");
     lcd2.setCursor(0,1);
     lcd2.print("4.T");
     lcd2.setCursor(4,1);
     lcd2.print("5.F");
     lcd2.setCursor(8,1);
     lcd2.print("6.SA");
     lcd2.setCursor(13,1);
     menucom4(1 ,13,1);
     if(atoi(cumm)==0 || atoi(cumm)==1 || atoi(cumm)==2 || atoi(cumm)==3 || atoi(cumm)==4 || atoi(cumm)==5 || atoi(cumm)==6){
     if(thru==1){
     update4(atoi(cumm));
     }
     if(thru==2){
     update5(atoi(cumm));
     }
     }
    }
  wdt_reset();
  delay(1000);
  wdt_reset();
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");

    }
  }
  }

  void menucom3(int h, char dis[20]){
  customKey2 = customKeypad.getKey();
  strcpy(cumm,"");
  strcpy(ikosh,"");
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print(dis);
  lcd2.setCursor(0,1);
  while((customKey2!='=')){
  wdt_reset();
  customKey2 = customKeypad.getKey();

while((strlen(cumm)<h) && (customKey2 !='=')){
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
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
break;
  }
}
  
if (customKey2 == 'C')//
   {
  strcpy(cumm,"");
  lcd2.setCursor(0,1);
  lcd2.print("        ");
  lcd2.setCursor(0,1);
  }
   buzzer.loop();
   if (customKey2) {
   buzzer.beep(80);
 }


  }

  if(strcmp(cumm,"")==0){
  if (customKey2 =='C')
   { 
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
  break;
  }
  }
if (customKey2 == 'C')//
   {
  strcpy(cumm,"");
  
  lcd2.setCursor(0,1);
  lcd2.print("        ");
  lcd2.setCursor(0,1);
  }
   if (customKey2 =='=')
   {  
    char p[10];
    strcpy(p,cumm);
    int y=h-strlen(cumm);
    strcpy(cumm,"");
   while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
    strcat(cumm,p);
  }
  }
  }

    int weekday(int year, int month, int day)/* Calculate day of week in proleptic Gregorian calendar. Sunday == 0. */
  {
  int adjustment, mm, yy;
  if (year<2000) year+=2000;
  adjustment = (14 - month) / 12;
  mm = month + 12 * adjustment - 2;
  yy = year - adjustment;
  return (day + (13 * mm - 1) / 5 +
    yy + yy / 4 - yy / 100 + yy / 400) % 7;
  }

  void reset(){
  strcpy(password,"000000");
  strcpy(nameschool,"HERITAGE MACHINES LTD");
  intial=5;
  na=0;
  int address=0;
  EEPROM.put(address, na);
  address=2;
  EEPROM.put(address, na);
  address=4;
  EEPROM.put(address, na);
  address=6;
  EEPROM.put(address, na);
  address=8;
  EEPROM.put(address, na);
  address=10;
  EEPROM.put(address, na);
  address=12;
  EEPROM.put(address, na);
  address=25;
  EEPROM.put(address, password);
  address=35;
  EEPROM.put(address, intial);
  address=900;
  EEPROM.put(address,nameschool);
  delay(2000);
  }
  void menucom4(int h, int a, int b){
    strcpy(cumm,"");
    strcpy(ikosh,"");
  customKey = customKeypad.getKey();
  while((customKey!='=')){
  wdt_reset();
  customKey = customKeypad.getKey();

while((strlen(cumm)<h) && (customKey !='=')){
wdt_reset();
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  
ikosh[0] = customKey;
lcd2.setCursor(a, b);
strcat(cumm,ikosh);
lcd2.print(cumm);
}
if(strcmp(cumm,"")==0){
if (customKey =='C')
   {  
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
break;
  }
}
  
if (customKey == 'C')//
   {
  strcpy(cumm,"");
  lcd2.setCursor(a, b);
  lcd2.print(cumm);
  }
  buzzer.loop();
  if (customKey) {
  buzzer.beep(80);
 }
  }

   if(strcmp(cumm,"")==0){
  if (customKey =='C')
   { 
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
  break;
  }
  }
if (customKey == 'C')
   {
  strcpy(cumm,"");
  }
  }
  } 


    void update2(int u){
    if(u==0){
    address=0;
    mod1(address,120,u);
    }
    if(u==1){
    address=2;
    mod1(address,229,u);
    }
    if(u==2){
    address=4;
    mod1(address,338,u);
    }
    if(u==3){
    address=6;
    mod1(address,447,u);
    }
    if(u==4){
    address=8;
    mod1(address,556,u);
    }
    if(u==5){
    address=10;
    mod1(address,665,u);
    }
    if(u==6){
    address=12;
    mod1(address,774,u);
    }
  }
  void mod1(int add1, int base1, int day2){
  customKey = customKeypad.getKey();
  EEPROM.get(add1, na);
       strcpy(cumm,"");
       strcpy(password2,"");
       lcd2.clear();
       lcd2.setCursor(0,0);
       lcd2.print("USE UP & DWN KEYS");
       lcd2.setCursor(0,1);
       int i=0;
       int ac=0;
       int rd=1;
  while((customKey!='C')){
    wdt_reset();
    EEPROM.get(base1+i,password2);
    strncpy(cumm,password2+7,1);
    strcpy(alarm2,password2);
    if(strcmp(cumm,"1")==0 && rd==1){
      strcat(alarm2,"   M1");
      lcd2.setCursor(0,1);
      lcd2.print("                ");
      lcd2.setCursor(0,1);
      lcd2.print(alarm2);
      rd=0;
    }
      if(strcmp(cumm,"2")==0 && rd==1){
      strcat(alarm2,"   M2");
      lcd2.setCursor(0,1);
      lcd2.print("                ");
      lcd2.setCursor(0,1);
      lcd2.print(alarm2);
      rd=0;
    }
      if(strcmp(cumm,"3")==0 && rd==1){
      strcat(alarm2,"   M3");
      lcd2.setCursor(0,1);
      lcd2.print("                ");
      lcd2.setCursor(0,1);
      lcd2.print(alarm2);
      rd=0;
    }
    if(strcmp(cumm,"0")==0 && rd==1){
      strcat(alarm2, "   OFF");
      lcd2.setCursor(0,1);
      lcd2.print("                ");
      lcd2.setCursor(0,1);
      lcd2.print(alarm2);
      rd=0;
    }
    strcpy(cumm,"");

 customKey = customKeypad.getKey();
  if (customKey == '.')
  {
  ac=ac+1;
  i=ac*9;
  rd=1;
  }
  if (customKey == '*')
  {
  ac=ac-1;
  i=ac*9; 
  rd=1;
  }
  if(ac>=na){
    ac=na-1;
    i=ac*9;
    }
  if(ac<=0){
    ac=0;
    i=ac*9;
    }

    if(customKey == '=')
  { char g[2];
  rd=1;
  EEPROM.get(base1+(ac*9)+7,g);
  if(strcmp(g,"0")==0){
  EEPROM.put(base1+(ac*9)+7,"1");
  }
    if(strcmp(g,"1")==0){
  EEPROM.put(base1+(ac*9)+7,"2");
  }
    if(strcmp(g,"2")==0){
  EEPROM.put(base1+(ac*9)+7,"3");
  }
    if(strcmp(g,"3")==0){
  EEPROM.put(base1+(ac*9)+7,"0");
  }
    
  }
  }
  }

  void show(){  
  if(showp==1){
  lcd2.clear();
  lcd2.setCursor(0,0);
  showp=0;
  } 
  displaytime();
  }

  void displaytime(){
  sprintf(tim, "%02d:%02d %02d/%02d/%02d ",  RTC.getHours(), RTC.getMinutes(), RTC.getDay(), RTC.getMonth(), RTC.getYear());
  lcd2.setCursor(0,0);
  lcd2.print("Time/Date:");
  lcd2.setCursor(0,1);
  lcd2.print(tim);
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==0)
  {
  lcd2.setCursor(13,0);
  lcd2.print("SUN");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==1)
  {
  lcd2.setCursor(13,0);
  lcd2.print("MON");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==2)
    {
  lcd2.setCursor(13,0);
  lcd2.print("TUE");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==3)
    {
  lcd2.setCursor(13,0);
  lcd2.print("WED");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==4)
    {
  lcd2.setCursor(13,0);
  lcd2.print("THU");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==5)
    {
  lcd2.setCursor(13,0);
  lcd2.print("FRI");
  }
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==6)
    {
  lcd2.setCursor(13,0);
  lcd2.print("SAT");
  }
    }
    void buts(){
    if(state1==0 && millis()>count3){
    if(digitalRead(eme)==LOW){
      if(busy1==0  && busy2==0){
        digitalWrite(bell,HIGH);
        state1=1;
        count3=millis()+1000;
        busy1=1;
        busy2=0;
    }
    }
  }
       if(state1==1 && millis()>count3){
    if(digitalRead(eme)==LOW){
      if(busy1==1  && busy2==0){
        digitalWrite(bell,LOW);
        state1=0;
        count3=millis()+1000;
        busy1=0;
        busy2=0;
      }
    }
  }
  
       if(state2==0 && millis()>count4){
    if(digitalRead(manu)==LOW){
      if(busy1==0   && busy2==0){
        digitalWrite(bell,HIGH);
        state2=1;
        count4=millis()+1000;
        busy2=1;
        busy1=0;
    }
    }
  }
       if(state2==1 && millis()>count4){
    if(digitalRead(manu)==LOW){
      if(busy1==0   && busy2==1){
        digitalWrite(bell,LOW);
        state2=0;
        count4=millis()+1000;
        busy2=0;
        busy1=0;
    }
    }
  }
  
  }

  void alarmring(){
  if(alarm10==1){
  ringa(100,100,8000,1000);
  }
   if(alarm10==2){
   ringa(100,8000,100,1000);
  }
   if(alarm10==3){
   ringa(8000,100,100,1000);

  }

  }

  void ringa(long a,long b,long c,long d){
    rin1=a;
    rin2=b;
    rin3=c;
    rin4=d;
    op=1;
  }

  void ring(long ring1, long ring2,long ring3,long delay){
    /*    lcd2.setCursor(12,0);
        lcd2.print("4");
    if(pass11==1){
      start=1;
        lcd2.setCursor(11,0);
        lcd2.print("1");
    }
    if(start==1){
      start=0;
      startt=millis();
    }
   if(stime <= (50+startt+ring1+ring2+ring3+(2*delay))){
   stime=millis();
   run=1;
   }else{
    run=0;
    
    }

   if(run==1){//*/
   
   if(pass1==1){
   if(pass11==1){
   count2=millis()+ring1;
   count3=millis()+ring1+delay;
   }
   //if(ring1==0){pass11=0;}
   if(millis()<count2 ){
    if(ring1!=1 && ring1!=2 && ring1!=3){
   digitalWrite(bell,HIGH);
    }
   pass11=0;
   }
   if(millis()>count2){
   digitalWrite(bell,LOW);
   if(millis()>count3){
   pass1=0;
   pass11=0;
   pass2=1;
   pass22=1;
   pass3=0;
   pass33=0;
   }
  }
   }

  if(pass2==1){
    if(pass22==1){
   count2=millis()+ring2;
   count3=millis()+ring2+delay;
   }
   //if(ring2==0){pass22=0;}
   if(millis()<count2 ){
    if(ring2!=1 && ring2!=2 && ring2!=3){
   digitalWrite(bell,HIGH);
    }
   pass22=0;
   }                //&& millis()< count2+2000
   if(millis()>count2 ){            
   digitalWrite(bell,LOW);
   if(millis()>count3){
   pass1=0;
   pass11=0;
   pass2=0;
   pass22=0;
   pass3=1;
   pass33=1;
   }
  }
   }

  if(pass3==1){
    if(pass33==1){
   count2=millis()+ring3;
   count3=millis()+ring3+delay;
   }
   //if(ring3==0){pass33=0;}
   if(millis()<count2 ){
    if(ring3!=1 && ring3!=2 && ring3!=3){
   digitalWrite(bell,HIGH);
    }
   pass33=0;
       // lcd2.setCursor(10,0);
       // lcd2.print("8");
   }
   if(millis()>count2){
   digitalWrite(bell,LOW);
   if(millis()>count2){
    // lcd2.setCursor(11,0);
    // lcd2.print("h");
   pass1=1;
   pass11=1;
   pass2=0;
   pass22=0;
   pass3=0;
   pass33=0;
   alarm10=0;
   tr=0;
   tr1=0;
   op=0;
   wait=millis();
   }
  }
   }



   //}


   
  }
    void checka(){
    int q=weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay());
    if(q==0){
    address=0;
    mod2(address,120,q);
    }
    if(q==1){
    address=2;
    mod2(address,229,q);
    }
    if(q==2){
    address=4;
    mod2(address,338,q);
    }
    if(q==3){
    address=6;
    mod2(address,447,q);
    }
    if(q==4){
    address=8;
    mod2(address,556,q);
    }
    if(q==5){
    address=10;
    mod2(address,665,q);
    }
    if(q==6){
    address=12;
    mod2(address,774,q);
    }
  }

  void mod2(int add2, int base2, int day2){
    int na2;
    int c=0;
    int u=0;
    char h1[2];
    char h2[2];
    char h3[2];
    strcpy(h2,"");
    strcpy(h3,"");
    char comp[9];
    char comp1[9];
    char comp2[4];
    char tim2[45];
    char tim3[15];
    strcpy(h1,"");
    strcpy(comp,"");
    strcpy(comp1,"");
    strcpy(comp2,"");
    strcpy(tim2,"");
    strcpy(tim3,"");
    int qt;
    int nh;
    EEPROM.get(add2, na2);
    while(c<=na2){
    wdt_reset();
    EEPROM.get(base2+u,comp);
    strncpy(comp1,comp,6);
    strncpy(h2,comp,2);
    sprintf(tim2, "%02d%02d%02d",  RTC.getHours(), RTC.getMinutes(), RTC.getSeconds());
    sprintf(tim3, "%02d%02d%02d",  RTC.getHours(), RTC.getMinutes()+5, RTC.getSeconds());
    //strncpy(h3,tim2,2);
    qt=convert1(h2)-RTC.getHours();
    strncpy(comp2,tim2+2,4);
    if(strstr(comp2,"5500")!=NULL && qt==1){
       nh=1;
    }else{nh=0;}
      
     /* lcd2.setCursor(0,1);
      lcd2.print(nh);
      //lcd2.print(comp2);
     // lcd2.setCursor(8,1);
     // lcd2.print(nh);
      //lcd2.setCursor(12,0);
      //lcd2.print(qt);
      lcd2.setCursor(7,1);
      lcd2.print(qt);
      lcd2.setCursor(12,1);
      lcd2.print(na2);//*/
      if(RTC.getMinutes()!=0){
      tr=1;
      }

    if(strstr(comp, tim2)==NULL){
    if(strstr(comp, tim3)==NULL){
    if(RTC.getMinutes()==0){
      if(tr==1){
      ringa(5,1,1,200);
      }
    }
    }
    }
    if(strstr(comp, tim3)==NULL){
     tr1=1;
    }
    if(strstr(comp, tim3)!=NULL || nh==1){
       strcpy(h1,"");
       strncpy(h1,comp+7,1);
       if(strstr(h1,"0")==NULL){
        ringa(5,1,1,100);
       }
      }

    if(strstr(comp, tim2)!=NULL){
         //lcd2.setCursor(10,0);
       // lcd2.print("2");

       strcpy(h1,"");
       strncpy(h1,comp+7,1);

          //lcd2.setCursor(12,0);
       // lcd2.print(h1);

       if(strstr(h1,"1")!=NULL){
       alarm10=1;
        // lcd2.setCursor(10,0);
        //lcd2.print("1");
        }
       if(strstr(h1,"2")!=NULL){
       alarm10=2;
       //  lcd2.setCursor(10,0);
        //lcd2.print("1");
        }
       if(strstr(h1,"3")!=NULL){
       alarm10=3;
         //lcd2.setCursor(10,0);
        //lcd2.print("1");
        }
        }
    u=u+9;
    c=c+1;
    }
  }

    void update(int u){
    if(u==0){
    address=0;
    mod(address,120,u);
    }
    if(u==1){
    address=2;
    mod(address,229,u);
    }
    if(u==2){
    address=4;
    mod(address,338,u);
    }
    if(u==3){
    address=6;
    mod(address,447,u);
    }
    if(u==4){
    address=8;
    mod(address,556,u);
    }
    if(u==5){
    address=10;
    mod(address,665,u);
    }
    if(u==6){
    address=12;
    mod(address,774,u);
    }
  }
  void mod(int add, int base, int day){
    EEPROM.get(add, na);
    strcpy(password2,"");
    if(na<=12){
    int base2=base+(na*9);
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("SET HOUR");

     menucom3(2, "SET HOUR");
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getHours());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
    while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
      strcat(cumm,p);
      strcat(password2,cumm);
     }
     menucom3(2, "SET MINS");
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getMinutes());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
   while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
    strcat(cumm,p);
      strcat(password2,cumm);
     }
     menucom3(2, "SET SECS");
    if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getSeconds());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
   while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
    strcat(cumm,p);
      strcat(password2,cumm);
     }

     dei(day);
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("SET MOD");
     lcd2.setCursor(10,0);
     lcd2.print("1.M1");
     lcd2.setCursor(0,1);
     lcd2.print("2.M2 3.M3");
     menucom4(1, 11,1);
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
     strcat(password2,"1");
     }
     EEPROM.put(base2,password2);
     na=na+1;
     EEPROM.put(add, na);
     delay(1000);
     wdt_reset();
     lcd2.setCursor(0,1);
     lcd2.print("ALARM ADDED");
  }
  }

  void dei(int da){
     if(da==0){
      strcat(password2,"0");
     }
     if(da==1){
      strcat(password2,"1");
     }
     if(da==2){
      strcat(password2,"2");
     }
     if(da==3){
      strcat(password2,"3");
     }
     if(da==4){
      strcat(password2,"4");
     }
     if(da==5){
      strcat(password2,"5");
     }
     if(da==6){
      strcat(password2,"6");
     }

  }

    void update4(int u){
    if(u==0){
    address=0;
    mod4(address,120,u);
    }
    if(u==1){
    address=2;
    mod4(address,229,u);
    }
    if(u==2){
    address=4;
    mod4(address,338,u);
    }
    if(u==3){
    address=6;
    mod4(address,447,u);
    }
    if(u==4){
    address=8;
    mod4(address,556,u);
    }
    if(u==5){
    address=10;
    mod4(address,665,u);
    }
    if(u==6){
    address=12;
    mod4(address,774,u);
    }
  }
  void mod4(int add1, int base1, int day2){
  customKey = customKeypad.getKey();
  EEPROM.get(add1, na);
       strcpy(cumm,"");
       strcpy(password2,"");
       lcd2.clear();
       lcd2.setCursor(0,0);
       lcd2.print("USE UP & DWN KEYS");
       lcd2.setCursor(0,1);
       int i=0;
       int ac=0;
       int rd=1;
  while((customKey!='C')){
    wdt_reset();
    EEPROM.get(base1+i,password2);
    strncpy(cumm,password2+7,1);
    strcpy(alarm2,password2);
    if(rd==1){
      strcat(alarm2,"  MDFY");
      lcd2.setCursor(0,1);
      lcd2.print("                ");
      lcd2.setCursor(0,1);
      lcd2.print(alarm2);
      rd=0;
    }

    strcpy(cumm,"");

 customKey = customKeypad.getKey();
  if (customKey == '.')
  {
  ac=ac+1;
  i=ac*9;
  rd=1;
  }
  if (customKey == '*')
  {
  ac=ac-1;
  i=ac*9; 
  rd=1;
  }
  if(ac>=na){
    ac=na-1;
    i=ac*9;
    }
  if(ac<=0){
    ac=0;
    i=ac*9;
    }

    if(customKey == '=')
  {rd=1;
  mod41(add1, base1+i, day2);
  }
  }
  }

    void mod41(int add, int base, int day){
    strcpy(password2,"");
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("SET HOUR");

     menucom3(2, "SET HOUR");
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getHours());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
    while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
      strcat(cumm,p);
      strcat(password2,cumm);
     }
     menucom3(2, "SET MINS");
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getMinutes());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
   while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
    strcat(cumm,p);
      strcat(password2,cumm);
     }
     menucom3(2, "SET SECS");
    if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
      String temp=String(RTC.getSeconds());
      temp.toCharArray(cumm, 2);
    char p[10];
    strcpy(p,cumm);
    int y=2-strlen(cumm);
    strcpy(cumm,"");
   while(y>0){
    strcat(cumm,"0");
    y=y-1;
   }
    strcat(cumm,p);
      strcat(password2,cumm);
     }
     dei(day);
     lcd2.clear();
     lcd2.setCursor(0,0);
     lcd2.print("SET MOD");
     lcd2.setCursor(10,0);
     lcd2.print("1.M1");
     lcd2.setCursor(0,1);
     lcd2.print("2.M2 3.M3");
     menucom4(1, 11,1);
     if(!(strcmp(cumm,"")==0)){
     strcat(password2,cumm);
     }else{
     strcat(password2,"1");
     }
     EEPROM.put(base,password2);
     delay(1000);
     wdt_reset();
     lcd2.setCursor(0,1);
     lcd2.print("ALARM MDFY");
  }

    void update5(int u){
    if(u==0){
    address=0;
    mod5(address,120,u);
    }
    if(u==1){
    address=2;
    mod5(address,229,u);
    }
    if(u==2){
    address=4;
    mod5(address,338,u);
    }
    if(u==3){
    address=6;
    mod5(address,447,u);
    }
    if(u==4){
    address=8;
    mod5(address,556,u);
    }
    if(u==5){
    address=10;
    mod5(address,665,u);
    }
    if(u==6){
    address=12;
    mod5(address,774,u);
    }
  }

  void mod5(int add5,int base5, int day5){
  int ne=0;
  for (int i = 0 ; i < 109 ; i++) {
  EEPROM.write(base5+i, "\0");
  }
  EEPROM.put(add5,ne);
  }

  uint8_t convert1(char w[2]){
    uint8_t q1;
     if(strstr(w,"00")!=NULL){q1=0;}
     if(strstr(w,"01")!=NULL){q1=1;}
     if(strstr(w,"02")!=NULL){q1=2;}
     if(strstr(w,"03")!=NULL){q1=3;}
     if(strstr(w,"04")!=NULL){q1=4;}
     if(strstr(w,"05")!=NULL){q1=5;}
     if(strstr(w,"06")!=NULL){q1=6;}
     if(strstr(w,"07")!=NULL){q1=7;}
     if(strstr(w,"08")!=NULL){q1=8;}
     if(strstr(w,"09")!=NULL){q1=9;}
     if(strstr(w,"10")!=NULL){q1=10;}
     if(strstr(w,"11")!=NULL){q1=11;}
     if(strstr(w,"12")!=NULL){q1=12;}
     if(strstr(w,"13")!=NULL){q1=13;}
     if(strstr(w,"14")!=NULL){q1=14;}
     if(strstr(w,"15")!=NULL){q1=15;}
     if(strstr(w,"16")!=NULL){q1=16;}
     if(strstr(w,"17")!=NULL){q1=17;}
     if(strstr(w,"18")!=NULL){q1=18;}
     if(strstr(w,"19")!=NULL){q1=19;}
     if(strstr(w,"20")!=NULL){q1=20;}
     if(strstr(w,"21")!=NULL){q1=21;}
     if(strstr(w,"22")!=NULL){q1=22;}
     if(strstr(w,"23")!=NULL){q1=23;}
return q1;
  }