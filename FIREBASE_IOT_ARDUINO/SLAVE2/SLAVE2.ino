#include "U8glib.h"
#include <Key.h>
#include <Keypad.h>
#include <ezBuzzer.h>
#include <Wire.h>
#include<avr/wdt.h>


//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, A4, A0, A1, A3, A2,U8G_PIN_NONE);  // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, A6, A1, A0, A3, A2,U8G_PIN_NONE);
const byte ROWS = 4;
const byte COLS = 4;
int x=0;
int y=0;

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
  char cummf[20];
  char cumm1j[20];
  char cumm1i[20];
  char sc[20];
 char cumm2[20];
 char ikosh[20];
 char ikosh1[20];
 char ikosh2[20];
 char tr[32];
char tre[64];
char tre2[64];
char tre3[64];
String opendoors;
String feedback;
String typedata;
String typedata1;
char status[300];
String typedata1x;
char statusx[300];
char feedbac[20];
int screen=0;
long yu,ya;
int setcode=0;
int setcabin=0;
int sta=0;
int check=0;
int de=0;
 

void setup() {
u8g.begin();
pinMode(32,INPUT);
pinMode(33,INPUT);
pinMode(34,INPUT);
pinMode(51,OUTPUT);
pinMode(52,OUTPUT);
pinMode(53,OUTPUT);
pinMode(18,OUTPUT);
strcpy(tre,"");
strcpy(tr,"");
strcpy(cumm1j,"");
strcpy(cumm1i,"");
strcpy(cumm1,"");
typedata1="";
yu=0;
strcpy(feedbac,"Cn   4.digit code");
ya=millis();
digitalWrite(32, HIGH);  
digitalWrite(33, HIGH);  
digitalWrite(34, HIGH);  
 u8g.firstPage();  
      do {
        u8g.setFont(u8g_font_helvB10);  
        u8g.drawStr(35, 10, "Welcome "); 
        u8g.drawStr(60, 30, "To ");
        u8g.drawStr(0, 50, "Heritage machines");
      } while( u8g.nextPage() );
      delay(3000); 
      Serial.begin(19200); 
Wire.begin(10);                // join I2C bus with address #10
Wire.onReceive(receiveEvent);
Wire.onRequest(requestEvent); 
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

   wdt_disable();
   wdt_enable(WDTO_4S);

strcpy(cumm2,"");
}

void loop(void) {
wdt_reset();
buzzer.loop();
main1();
customKey = customKeypad.getKey();
if(customKey =='/')
{
  screen=1;
  operate();
}

if(customKey =='*')
{
  screen=1;
  operate2();
}

if(customKey)
{
  buzzer.beep(80);
}
  
}

void del(){
  if(strlen(cumm1j)>0){
    char tra1[65];
    strcpy(tra1,cumm1j);
     memset(cumm1j, '\0', sizeof(cumm1j));
    strncpy(cumm1j,tra1,strlen(tra1)-1);
   strcpy(tra1,"");
  }
   check=0;
   setcabin=0;
  setcode=0;
  
  if(strlen(cumm1j)<1){
    de=0;
  }
}
void operate(){
  while(1){
  wdt_reset();
 customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){ 
ikosh[0] = customKey;
strcat(cumm1,ikosh);
  de=0;    
}
if(strlen(cumm1j)<0){
  strcpy(cumm1i,"");
}

if(strlen(cumm1i)<1){
  if(strlen(cumm1)>0){
strncpy(cumm1i,cumm1,1);
strcpy(cumm1j,cumm1i);
strcpy(cumm1,"");
  }
}

if(strlen(cumm1j)<2 && check==0){
  if(strlen(cumm1)>0){
strncpy(cumm1i,cumm1,1);
strcat(cumm1j,cumm1i);
strcpy(cumm1,"");
}
}//*/

 if(customKey =='C')
  {
    if(strlen(cumm1)<1){
  de=1;
  }

    if(strlen(cumm1)>0){
    char tra[65];
    strcpy(tra,cumm1);
    memset(cumm1, '\0', sizeof(cumm1));
    strncpy(cumm1,tra,strlen(tra)-1);
    strcpy(tra,"");
  }
  if(de==1){
  del();
  }
  }
  if(screen==1){
  homescreen(status+4,cumm1,cumm1j,"Choose cabin from list","Open Cabins(Cn)",feedbac);
  }

   if(screen==2){
  homescreen2(status+20,cumm1,cumm1j,"Choose cabin from list","Open Cabins(Cn)",feedbac);
  }

  if(customKey =='/')
{
  screen=2;
}

  if(customKey =='=')
{
  if(sta==1){
  if(strlen(cumm1)==4){
   setcode=1;
  }else{
    strcpy(feedbac,"Incorrect count");
    }
}

if(setcabin==0){
if(typedata1.indexOf(cumm1j)>0){
strcpy(cumm1,"");
  setcabin=1;
  sta=1;
  check=1;
  strcpy(feedbac,"Success Cabin");
}else{
  strcpy(feedbac,"Wrong Cabin");
}
}

  
  if(setcabin==1 && setcode==1){
  screen=1;
  strcpy(cummf,cumm1j);
  strcat(cummf,"-");
  strcat(cummf,cumm1);
   if(strlen(cummf)==6 || strlen(cummf)==6){
  memset(cumm2, '\0', sizeof(cumm2));
  strcpy(cumm2,cummf);
  strcpy(cummf,"");
  setcabin=0;
  setcode=0;
  check=0;
  }else{
    strcpy(feedbac,"Incorrect count");
    strcpy(cummf,"");
  }
  }
}

if(customKey =='+')
{
  if(screen==1){
  break;
  }else{screen=1;}

}

yu=millis()-ya;
if(yu>4000){
strcpy(feedbac,"Cn   4.digit code");
  ya=millis();
}
}
}


void operate2(){
  while(1){
  wdt_reset();
 customKey = customKeypad.getKey();
if (customKey && (customKey !='.') && (customKey !='=') && (customKey !='C') && (customKey !='/') && (customKey !='*') && (customKey !='-') && (customKey !='+')){ 
ikosh[0] = customKey;
strcat(cumm1,ikosh);
  de=0;    
}
if(strlen(cumm1j)<0){
  strcpy(cumm1i,"");
}

if(strlen(cumm1i)<1){
  if(strlen(cumm1)>0){
strncpy(cumm1i,cumm1,1);
strcpy(cumm1j,cumm1i);
strcpy(cumm1,"");
  }
}

if(strlen(cumm1j)<2 && check==0){
  if(strlen(cumm1)>0){
strncpy(cumm1i,cumm1,1);
strcat(cumm1j,cumm1i);
strcpy(cumm1,"");
}
}//*/

 if(customKey =='C')
  {
    if(strlen(cumm1)<1){
  de=1;
  }

    if(strlen(cumm1)>0){
    char tra[65];
    strcpy(tra,cumm1);
    memset(cumm1, '\0', sizeof(cumm1));
    strncpy(cumm1,tra,strlen(tra)-1);
    strcpy(tra,"");
  }
  if(de==1){
  del();
  }
  }
  if(screen==1){
  homescreen(statusx+4,cumm1,cumm1j,"Choose cabin from list","Closed Cabins(Cn)",feedbac);
  }

   if(screen==2){
  homescreen2(statusx+20,cumm1,cumm1j,"Choose cabin from list","Closed Cabins(Cn)",feedbac);
  }

  if(customKey =='/')
{
  screen=2;
}

  if(customKey =='=')
{
  if(sta==1){
  if(strlen(cumm1)==4){
   setcode=1;
  }else{
    strcpy(feedbac,"Incorrect count");
    }
}

if(setcabin==0){
if(typedata1x.indexOf(cumm1i)>0){
strcpy(cumm1,"");
  setcabin=1;
  sta=1;
  check=1;
  strcpy(feedbac,"Success Cabin");
}else{
  strcpy(feedbac,"Wrong Cabin");
}
}

  
  if(setcabin==1 && setcode==1){
  screen=1;
  strcpy(cummf,cumm1j);
  strcat(cummf,"-");
  strcat(cummf,cumm1);
   if(strlen(cummf)==6 || strlen(cummf)==6){
  memset(cumm2, '\0', sizeof(cumm2));
    strcpy(cumm2,cummf);
  strcpy(cummf,"");
  setcabin=0;
  setcode=0;
  check=0;
  }else{
    strcpy(feedbac,"Incorrect count");
    strcpy(cummf,"");
  }
  }
}

if(customKey =='+')
{
  if(screen==1){
  break;
  }else{screen=1;}

}

yu=millis()-ya;
if(yu>4000){
strcpy(feedbac,"Cn   4.digit code");
  ya=millis();
}
}
}


void receiveEvent(int how) {

  strcpy(tre,"");
  while (Wire.available()) { 
  wdt_reset();
  int c = Wire.read();
   sprintf(tr,"%c",c);

     if(strlen(tre)<20){
    strcat(tre,tr);
   
    typedata.concat(tr);
  }
  
  }
  if(typedata.indexOf("#")>0){
    typedata="";
  }
   if(typedata.indexOf("|")>0){
    typedata="";
  }

  if(typedata.indexOf("=")>0){
    typedata="";
  }
 
 if(typedata.indexOf("ST-")>0){
    strcpy(status,tre);
    typedata1=String(status+2);
  }

  if(typedata.indexOf("ST*")>0){
    strcpy(statusx,tre);
    typedata1x=String(statusx+2);
  }

  if(typedata.indexOf("FB")>0){
    strcpy(feedbac,tre);
  }
  if(typedata.indexOf("FR")>0){
    strcpy(feedbac,tre);
  }
  typedata="";
  strcpy(tre,"");
}


void requestEvent() {
   wdt_reset();
   if(strcmp(cumm1j,"")!=0){
  if(strcmp(cumm2,"")!=0 ){
  Wire.write(cumm2);
  strcpy(cumm2,"");
  strcpy(cumm1,"");
  strcpy(cumm1+1,"");
  strcpy(cumm1i,"");
  strcpy(cumm1j,"");
  }
}else{
  strcpy(cumm2,"");
   strcpy(cumm1,"");
  strcpy(cumm1i,"");
  strcpy(cumm1j,"");
  strcpy(cumm1+1,"");
}
  if(strcmp(cumm2,"")==0){
  Wire.write("~~~~~~~~~~~~~~~~");
  strcpy(cumm2,"");
  }  
}


void homescreen(char r1[],char r2[],char r3[],char r4[],char r5[],char r6[]){
   u8g.firstPage();
   do {
    u8g.setFont(u8g_font_helvR08);   
    u8g.drawStr( 0, 8, r4);
    u8g.drawStr( 0, 20, r5);
    u8g.drawStr( 0, 33, r1);
    u8g.drawStr( 0, 46, r6);
    u8g.drawStr(19, 62, r2);
    u8g.drawStr(3, 62, r3);
    u8g.drawFrame(113,54,15,10);
    u8g.drawFrame(0,49,15,15);
    u8g.drawFrame(17,49,80,15);
    u8g.drawStr(115, 63, "<-"); 
    u8g.drawFrame(113,0,15,10);
    u8g.drawStr(116,8, "c>");
  } while( u8g.nextPage() );

}

void homescreen2(char r1[],char r2[],char r3[],char r4[],char r5[],char r6[]){
   u8g.firstPage();
   do {
    u8g.setFont(u8g_font_helvR08);   
    u8g.drawStr( 0, 8, r4);
    u8g.drawStr( 0, 20, r5);
    u8g.drawStr( 0, 33, r1);
    u8g.drawStr( 0, 46, r6);
    u8g.drawStr(19, 62, r2);
    u8g.drawStr(3, 62, r3);
    u8g.drawFrame(113,54,15,10);
    u8g.drawFrame(0,49,15,15);
    u8g.drawFrame(17,49,80,15);
    u8g.drawStr(115, 63, "<-"); 
    u8g.drawFrame(113,0,15,10);
    u8g.drawStr(116,8, "c>");
  } while( u8g.nextPage() );

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

void draw(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( x, y, cumm1);
}

 void main1(){
  u8g.firstPage();  
      do {
        u8g.setFont(u8g_font_helvR08);
        u8g.setColorIndex(2);
        u8g.drawFrame(0,0,15,10);
        u8g.drawFrame(17,0,15,10);
        u8g.drawFrame(34,0,15,10);
        u8g.drawFrame(51,0,15,10);

        u8g.drawFrame(0,12,15,10);
        u8g.drawFrame(17,12,15,10);
        u8g.drawFrame(34,12,15,10);
        u8g.drawFrame(51,12,15,10);

        u8g.drawFrame(0,24,15,10);
        u8g.drawFrame(17,24,15,10);
        u8g.drawFrame(34,24,15,10);
        u8g.drawFrame(51,24,15,10);

        u8g.drawFrame(0,36,15,10);
        u8g.drawFrame(17,36,15,10);
        u8g.drawFrame(34,36,15,10);
        u8g.drawFrame(51,36,15,10);

        //u8g.drawFrame(113,54,15,10);
        //u8g.drawStr(115, 63, "<-"); 

        u8g.setFont(u8g_font_helvR08);
        u8g.drawStr(1, 45, "del"); 
        u8g.drawStr(36, 44, "en"); 
        u8g.drawStr(53, 45, "<-"); 
        u8g.drawStr(53, 32, "dn"); 
        u8g.drawStr(53, 20, "up"); 
        u8g.drawStr(54,8, "c>"); 
        u8g.drawStr(68, 9, "Close cabin"); 
        u8g.drawStr(68, 21, "Open cabin");




      } while( u8g.nextPage() );
}

