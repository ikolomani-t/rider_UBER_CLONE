#include <TVout.h>
#include <fontALL.h>
#include <I2C_RTC.h>
#include <Wire.h>
#include <ezBuzzer.h>
#include <Keypad.h>
#include <EEPROM.h>
#include<avr/wdt.h>
#include <avr/pgmspace.h>

const byte ROWS PROGMEM= 4;
const byte COLS PROGMEM= 4;
TVout TV;
static DS3231 RTC;
//int bell=10;
//int eme=11;
//int manu=12;
const char hexaKeys[ROWS][COLS] PROGMEM= {

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

 char cumm[45];
 char ikosh[20];
 char datedis[45];
 char datedis2[45];
 char alarm1[45];
 char alarm2[45];
 char datedis3[45];
 char password[7];
 char fontdis[10];
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

 const byte rowPins[ROWS] PROGMEM= {1, 0, 2, 3};
 const byte colPins[COLS] PROGMEM= {14, 15, 16, 17};

 Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 bool A0s=true,A1s=true,A2s=true, A3s=true,A4s =true,A5s=true,A6s=true,A7s =true,A8s=true;
 long count2,count3,count4; 
 uint8_t day; uint8_t month; uint16_t year;
 uint8_t hour; uint8_t minute; uint8_t second;
 int na=0;

void setup() {
  int address;
  int intial;
  long fon;
  char password[7];
  TV.begin(PAL); // Start TVout in PAL mode (change to NTSC if needed)
  TV.select_font(font6x8); // Select font size
  //strcpy(datedis,"HOUR              MINS             SECS");
  //strcpy(datedis3,"DAY              MONTH            YEAR");
  //strcpy(alarm1, "Hour              MIN              SECS");
  address=35;
  EEPROM.get(address, intial);
  if(intial!=4){
   strcpy(password, (const char *) F("000000"));
   fon=2;
   strcpy(nameschool,(const char *) F("HERITAGE MACHINES LTD"));
   intial=4;
   int mode=PAL;
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
  address=30;
  EEPROM.put(address, mode);
  address=35;
  EEPROM.put(address, intial);
  address=900;
  EEPROM.put(address,nameschool);
  address=890;
  EEPROM.put(address,fon);

  TV.print(10,10,"SET COMP");
  delay(2000);
 }
  pinMode(10,OUTPUT);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  digitalWrite(10,LOW);
  address=890;
  EEPROM.get(address, fon);
  selectfont(fon);
  TV.clear_screen(); // Clear the screen
  TV.println(10, 10, (const char *) F("WELCOME TO"));  // Print message at coordinates (10, 10)
  TV.println(10, 10, (const char *) F("HERITAGE MACHINES"));  // Print message at coordinates (10, 10)
  TV.print((const char *) F("CNT: 0705 008 557"));
  delay(2000);
  if(!RTC.begin()){
  TV.println(10, 10, (const char *) F("RTC ERR"));  // Print message at coordinates (10, 10)
  TV.println((const char *) F("CHK POW")); 
  TV.print((const char *) F("CNT: 0705 008 557"));
  while(1){
    digitalWrite(10,HIGH);
    wdt_reset();}
  }
  if (!RTC.isRunning())
  {
    TV.print(10, 10, (const char *) F("RTC Err, Set time!"));  // Print message at coordinates (10, 10)
    delay(2000);
    //while(1){wdt_reset();}
  }
  wdt_disable();
  wdt_enable(WDTO_4S);
}

void loop() {
long fon;
buzzer.loop();
buts();
alarmring();
checka();
address=890;
EEPROM.get(address, fon);
selectfont(fon);
char customKey;
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
char customKey;
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  
ikosh[0] = customKey;
strcat(cumm,ikosh);
}
  menu1();
  activemenu();
  buzzer.loop();
  if(customKey) {
  buzzer.beep(80);
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

void displaytime(){
  TV.print(10,10, RTC.getYear(), DEC);
  TV.print('/');
  TV.print(RTC.getMonth(), DEC);
  TV.print('/');
  TV.print(RTC.getDay(), DEC);
  TV.println(" ");
  TV.print(RTC.getHours(), DEC);
  TV.print(':');
  TV.print(RTC.getMinutes(), DEC);
  TV.print(':');
  TV.print(RTC.getSeconds(), DEC);
  TV.println();
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==0)
  TV.print((const char *) F("SUN"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==1)
  TV.print((const char *) F("MON"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==2)
  TV.print((const char *) F("TUE"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==3)
  TV.print((const char *) F("WED"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==4)
  TV.print((const char *) F("THUR"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==5)
  TV.print((const char *) F("FRI"));
  if(weekday(RTC.getYear(), RTC.getMonth(), RTC.getDay())==6)
  TV.print((const char *) F("SAT"));
}

void selectfont(long fon){

  if(fon==1)
  TV.select_font(font4x6);
  if(fon==2)
  TV.select_font(font6x8);
  if(fon==3)
  TV.select_font(font8x8);
}

  void menu1()
  {  
 char customKey;
 customKey = customKeypad.getKey();  
    if (customKey == '.')
{
  menu=menu+1;  
  }
     if (customKey == '*')
{
  menu=menu-1;  
  }

if(menu>6){menu=6;}
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
  strcpy(currentmenu, (const char *) F("SET TIME"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("==>1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();
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
  strcpy(currentmenu, (const char *) F("ALARM ON/OFF"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
    TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("==>2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();
  
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
   strcpy(currentmenu, (const char *) F("SET ALARM"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("==>3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();

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
   strcpy(currentmenu, (const char *) F("FACTORY RESET"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("==>4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();

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
   strcpy(currentmenu, (const char *) F("CHANGE FONT"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("==>5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();

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
   strcpy(currentmenu, (const char *) F("IN NAME"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("==>6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();

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
  strcpy(currentmenu, (const char *) F("SET DATE"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("==>7.Set Date"));
  TV.println((const char *) F("8.Set Clk sys"));
  TV.println();

  }//*/

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
  strcpy(currentmenu, (const char *) F("SET CLOCK SYSTEM"));
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.println(10,10, (const char *) F("MENU"));
  TV.select_font(font6x8);
  TV.println((const char *) F("1.Set time"));
  TV.println((const char *) F("2.Alarm on/off"));
  TV.println((const char *) F("3.Set Alarm"));
  TV.println((const char *) F("4.Fact Rst"));
  TV.println((const char *) F("5.Chang font"));
  TV.println((const char *) F("6.Inst Name"));
  TV.println((const char *) F("7.Set Date"));
  TV.println((const char *) F("==>8.Set Clk sys"));
  TV.println();

  }
    }

  
  void activemenu(){
    char customKey;
    customKey = customKeypad.getKey();
  if (customKey == '=')
  {
     if(strcmp (currentmenu, (const char *) F("SET DATE") )==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("SET DATE"));
     TV.select_font(font6x8);
     TV.println(datedis3);
     TV.println(datedis2);

     menucom3(2, 0,datedis2);
     menucom3(2, 17,datedis2);
     menucom3(4, 35,datedis2);
     
     strncpy(cumm,datedis[0],2);
     day=atoi(cumm);
     strcpy(cumm,"");

     strncpy(cumm,datedis[17],2);
     month=atoi(cumm);
     strcpy(cumm,"");

     strncpy(cumm,datedis[35],4);
     year=atoi(cumm);
     strcpy(cumm,"");
     RTC.setDate(day, month, year);
      }

  if(strcmp (currentmenu, (const char *) F("FACTORY RESET") )==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("FACT RST"));
     TV.select_font(font6x8);
     reset();
      }

  if(strcmp (currentmenu, (const char *) F("SET CLOCK SYSTEM") )==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("SET CLK SYS"));
     TV.select_font(font6x8);
     TV.println((const char *) F("SEL 12/24 MODE"));
     menucom4(2);
     TV.println("");
     if (atoi(cumm)==24 || atoi(cumm)==12){
     if(atoi(cumm)==24){
      RTC.setHourMode(CLOCK_H24);
      }
        if(atoi(cumm)==12){
        RTC.setHourMode(CLOCK_H12);
      }
     TV.println(RTC.getHourMode(), DEC);
     TV.print((const char *) F("CLK SYS SUCC"));
     }else{
      TV.print((const char *) F("CLK SYS ERR"));
     }

      }


   if(strcmp (currentmenu, (const char *) F("IN NAME") )==0)
    { strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("INST NAME"));
     TV.select_font(font6x8);
     menucom4(30);
     TV.println("");
     strcpy(nameschool,cumm);
     address=900;
     EEPROM.put(address,nameschool);
     if(strcmp(cumm,"")==0){
      TV.print((const char *) F("INST ERR"));
     }else{
      TV.print((const char *) F("INST NAME SET"));
     }
     strcpy(cumm, "");
      }

if(strcmp (currentmenu, (const char *) F("SET TIME") )==0)
  {  strcpy(cumm, "");
     strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("SET TIME"));
     TV.select_font(font6x8);
     TV.println(datedis);
     TV.println(datedis2);

     menucom3(2, 0,datedis2);
     menucom3(2, 18,datedis2);
     menucom3(2, 35,datedis2);

     strncpy(cumm,datedis[0],2);
     hour=atoi(cumm);
     strcpy(cumm,"");

     strncpy(cumm,datedis[18],2);
     minute=atoi(cumm);
     strcpy(cumm,"");

     strncpy(cumm,datedis[35],2);
     second=atoi(cumm);
     strcpy(cumm,"");
     RTC.setTime(hour, minute, second);
    }
      if(strcmp (currentmenu, (const char *) F("ALARM ON/OFF") )==0)
    {strcpy(cumm, "");
     strcpy(ikosh, "");
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("ALARM ON/OFF"));
     TV.print((const char *) F("SEL DAY"));
     TV.select_font(font6x8);
     TV.print((const char *) F("0.SUN"));
     TV.print((const char *) F("1.MON"));
     TV.print((const char *) F("2.TUE"));
     TV.print((const char *) F("3.WED"));
     TV.print((const char *) F("4.THUR"));
     TV.print((const char *) F("5.FRI"));
     TV.print((const char *) F("6.SAT"));
     menucom4(1);
     if(atoi(cumm)==0 || atoi(cumm)==1 || atoi(cumm)==2 || atoi(cumm)==3 || atoi(cumm)==4 || atoi(cumm)==5 || atoi(cumm)==6){
      update2(atoi(cumm));
     }
      }

     if(strcmp (currentmenu, (const char *) F("SET ALARM") )==0)
    { 
      strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("SEL ALARM DAY"));
     TV.select_font(font6x8);
     TV.print((const char *) F("0.SUN"));
     TV.print((const char *) F("1.MON"));
     TV.print((const char *) F("2.TUE"));
     TV.print((const char *) F("3.WED"));
     TV.print((const char *) F("4.THUR"));
     TV.print((const char *) F("5.FRI"));
     TV.print((const char *) F("6.SAT"));
     menucom4(1);
     //weekday(now.year(), now.month(), now.day());
     if(atoi(cumm)==0 || atoi(cumm)==1 || atoi(cumm)==2 || atoi(cumm)==3 || atoi(cumm)==4 || atoi(cumm)==5 || atoi(cumm)==6){
       update(atoi(cumm));
     }
     
      } 
      if(strcmp (currentmenu, (const char *) F("CHANGE FONT") )==0)
    { long fon;
      strcpy(cumm, "");
      strcpy(ikosh, "");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("CHANG FONT"));
     TV.select_font(font6x8);
     TV.println((const char *) F("SELECT FONT NO."));
     TV.println((const char *) F("1.font4x6"));
     TV.println((const char *) F("2.font6x8"));
     TV.println((const char *) F("3.font8x8"));
     menucom4(1);
     TV.println();
     if(atoi(cumm)==1 || atoi(cumm)==3 || atoi(cumm)==2 ){
     fon=atoi(cumm);
     address=890;
     EEPROM.put(address,fon);
     TV.print((const char *) F("FONT SUCC"));
      } else{
       TV.print((const char *) F("INV CHOICE"));
      }
    }
  }
  }
  void show(){   
  if(showp==1){
  address=900;
  EEPROM.get(address, nameschool);
  TV.select_font(font8x8);
  TV.println(0,10, nameschool);
  TV.println((const char *) F("Alarm System"));
  TV.select_font(font6x8);
  displaytime();
  showp=0;
  }   
    }
 
void menucom3(int h, int a, char* str1){
  pause=1;
  char customKey;
  customKey = customKeypad.getKey();
  while(strlen(cumm)<h  && pause==1){
      wdt_reset();
    customKey = customKeypad.getKey();

while(strlen(cumm)<h && pause==1){
wdt_reset();
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  

ikosh[0] = customKey;
strcat(cumm,ikosh);
strcpy(str1[a],cumm);
TV.print(str1);

}
if(strcmp(cumm,"")==0){
if (customKey =='C')
   {  
  modemenu=0;
  showp=1;
  show();
  strcpy(cumm, "");
  pause=0;
break;
  }
}
  
if (customKey == 'C')
   {
  strcpy(cumm,"");
  strcpy(str1[a],cumm);
  TV.print(str1);
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
  pause=0;
  break;
  }
  }
if (customKey == 'C')
   {
  strcpy(cumm,"");
  }

  }
   strcpy(cumm, "");
   strcpy(ikosh, "");
  }

  void menucom4(int h){
  char customKey;
  customKey = customKeypad.getKey();
  while((customKey!='=')){
  wdt_reset();
  customKey = customKeypad.getKey();

while((strlen(cumm)<h) && (customKey !='=')){
wdt_reset();
customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){  
ikosh[0] = customKey;
strcat(cumm,ikosh);
TV.print(cumm);
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
  TV.print(cumm);
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
  void reset(){
  long fon;
  long intial;
  strcpy(password,"000000");
  fon=2;
  strcpy(nameschool,(const char *) F("HERITAGE MACHINES LTD"));
  intial=3;
  long mode=PAL;
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
  address=30;
  EEPROM.put(address, mode);
  address=35;
  EEPROM.put(address, intial);
  address=900;
  EEPROM.put(address,nameschool);
  address=890;
  EEPROM.put(address,fon);
  TV.print(10,10,(const char *) F("SETTING COMPLETE"));
  delay(2000);
  }

  int weekday(int year, int month, int day)
/* Calculate day of week in proleptic Gregorian calendar. Sunday == 0. */
  {
  int adjustment, mm, yy;
  if (year<2000) year+=2000;
  adjustment = (14 - month) / 12;
  mm = month + 12 * adjustment - 2;
  yy = year - adjustment;
  return (day + (13 * mm - 1) / 5 +
    yy + yy / 4 - yy / 100 + yy / 400) % 7;
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
    if(na<12){
    int base2=base+(na*9);
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("SET HOUR  MIN  SECS"));
     TV.select_font(font6x8);
     TV.println(alarm1);
     TV.print(alarm2);
     String temp = String(day);
     temp.toCharArray(cumm, 2);

     menucom3(2, 0, alarm2);
     menucom3(2, 18, alarm2);
     menucom3(2, 35, alarm2);

     strncpy(password2[0],alarm2[0],2);
     strncpy(password2[2],alarm2[18],2);
     strncpy(password2[4],alarm2[35],2);
     strncpy(password2[6],cumm,1);

     strcpy(password2[7],"1");
     EEPROM.put(base2,password2);
     na=na+1;
     EEPROM.put(add, na);
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
    char customKey;
    customKey = customKeypad.getKey();
  EEPROM.get(add1, na);
     strcpy(cumm,"");
     strcpy(password2,"");
     TV.clear_screen();
     TV.select_font(font8x8);
     TV.println(10,10, (const char *) F("ON/OFF ALARMS"));
     TV.select_font(font6x8);
  int i=0;
  int x=0;
  while((customKey!='C')){
  wdt_reset();
  while(x<na){
  wdt_reset();
    EEPROM.get(base1+i,password2);
    strncpy(cumm,password2[7],1);
    strcpy(alarm2,password2);
    if(strcmp(cumm,"1")==0){
      strcat(alarm2,(const char *) F("     ON"));
    }
    if(strcmp(cumm,"0")==0){
      strcat(alarm2,(const char *) F("     OFF"));
    }
    if(x==ac){
      strcpy(cumm,"==>");
      strcat(cumm,alarm2);
      strcpy(alarm2,cumm);
    }
    TV.println(alarm2);
    x=x+1;
    i=i+9;
    strcpy(cumm,"");
  }
 customKey = customKeypad.getKey();
  if (customKey == '.')
  {
  ac=ac+1; 
  i=0;
  x=0; 
  }
  if (customKey == '*')
  {
  ac=ac-1; 
  i=0;
  x=0; 
  }
  if(ac>na){ac=na;}
  if(ac<0){ac=0;}

    if(customKey == '=')
  { char g[1];
  EEPROM.get(base1+(ac*9)+7,g);
  if(strcmp(g,"1")==0){
  EEPROM.put(base1+(ac*9)+7,"0");
  }else{
  EEPROM.put(base1+(ac*9)+7,"1");
  }
  i=0;
  x=0; 
  }
  }
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
    char comp[7];
    EEPROM.get(add2, na2);
    while(c<na2){
    wdt_reset();
    EEPROM.get(base2+u,comp);
    strncpy(comp[0],h1,2);
    if(RTC.getHours()==(uint8_t)atoi(h1)){
       strncpy(comp[2],h1,2);
       if(RTC.getMinutes()==(uint8_t)atoi(h1)){
           strncpy(comp[4],h1,2);
           if(RTC.getSeconds()==(uint8_t)atoi(h1)){
             strncpy(comp[7],h1,1);
             if(strcmp(h1,"1")==0){
              alarm10=1;
             }
             
           }
       }
    }
    u=u+9;
    c=c+1;
    }
  }

  void alarmring(){
  if(alarm10==1){
   count2=millis()+600000;
   digitalWrite(10,HIGH);
   alarm10=0;
  }
  if(millis()>count2 && millis()< count2+2000){
   digitalWrite(10,LOW);
  }
  }


  void buts(){
    
     if(state1==0 && millis()>count3){
    if(digitalRead(11)==LOW){
      if(busy1==0  && busy2==0){
        digitalWrite(10,HIGH);
        state1=1;
        count3=millis()+1000;
        busy1=1;
        busy2=0;
    }
    }
  }
       if(state1==1 && millis()>count3){
    if(digitalRead(11)==LOW){
      if(busy1==1  && busy2==0){
        digitalWrite(10,LOW);
        state1=0;
        count3=millis()+1000;
        busy1=0;
        busy2=0;
      }
    }
  }
  
       if(state2==0 && millis()>count4){
    if(digitalRead(12)==LOW){
      if(busy1==0   && busy2==0){
        digitalWrite(10,HIGH);
        state2=1;
        count4=millis()+1000;
        busy2=1;
        busy1=0;
    }
    }
  }
       if(state2==1 && millis()>count4){
    if(digitalRead(12)==LOW){
      if(busy1==0   && busy2==1){
        digitalWrite(10,LOW);
        state2=0;
        count4=millis()+1000;
        busy2=0;
        busy1=0;
    }
    }
  }
  
  }