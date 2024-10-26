/*
  Gamebuino TV Adapter - by Myndale (mailto:accounts@ppl-pilot.com)
 
 This adapter plugs into the Gamebuino ICSP port and generates a composite PAL TV signal.
 
 == PAL Non-interlaced Signal Format ==
 
 There are 312 lines, each of which is 64uS (1024 cycles) wide:
 
 Line 1: Long Sync, Long Sync
 Line 2: Long Sync, Long Sync
 Line 3: Long Sync, Short Sync
 Line 4: Short Sync, Short Sync
 Line 5: Short Sync, Short Sync
 Line 6: Horz Sync, Front Porch, Image
 ....
 Line 309: Horz  Sync, Front Porch, Image
 Line 310: Short Sync, Short Sync
 Line 311: Short Sync, Short Sync
 Line 312: Short Sync, Short Sync
 
 Long sync: 30 uS (480 cycles) @ 0.0V followed by 2uS (32 cycles) @ 0.3V
 Short sync: 2 uS (32 cycles) @ 0.0V followed by 30uS (480 cycles) @ 0.3V
 Horz sync: 4 uS (64 cycles) @ 0.0V
 Front porch: 8uS (128 cycles) @0.3V
 Image: 52uS (832 cycles) of image data ranging from 0.3V (black) to 1.0V (white).

 Most functions are designed with a 5 cycle lead time between set up (i.e. register
 initialization and the call to the function) and the first output to the screen.
 They're also designed to give the next function a 5 cycle lead once their ret
 instruction has executed.
 
 == Register usage ==
 r1  - pixel mask
 r2:r3 - current pixel vals
 r4 - current buffer number
 r5 - read counter (time since last SPI byte received)
 //r6:r7 - spi buffer ptr at start of frame
 r8:r9 - spi buffer at start of frame capture
 r15 - scratch
 r16 - loop
 r17 - sync port value  ( 0.0V)
 r18 - black port value (~0.3V)
 r19 - white port value (~1.0V)
 r20 - gray value
 r21 - line counter
 r22 - flip flag
 r24:r25 - scratch
 r26:27 - line ptr
 r28:29 - spi dest ptr
 r30:31 - spi buffer end
 
 == Gamebuino Hardware Interface ==
 Arduino Gnd            - Gamebuino ICSP GND
 Arduino Pin D11 (PB3)  - Gamebuino ICSP MOSI
 Arduino Pin D13 (PB5)  - Gamebuino ICSP SCLK
 
 *                          IMPORTANT: ARDUINO=5V, GAMEBUINO=3.3V!                       *
 * DO NOT CONNECT AN ICSP PROGRAMMING CABLE BETWEEN ARDUINO AND GAMEBUINO OR DO ANYTHING *
 * ELSE TO TIE THEIR VOLTAGE LINES TOGETHER. LIKEWISE DO NOT HAVE THE TWO CONNECTED IN   *
 * IN ANY WAY WHILST PROGRAMMING ARDUINO AS THE SCLK AND MOSI LINES WILL BE ACTIVE HIGH. *
 
 == Video Hardware Interface ==
 Arduino Pin A0 --------> 1k resistor  (sync)  -----------+-------> Composite PAL Out
 Arduino Pin A1 --------> 470 ohm resistor (video) -------^
 
The Arduino can be replaced with an ATMega 328p chip with 16MHz crystal osc and 22pF stabilizing capacitors.
In this case the chip can be powered by the Gamebuino's ICSP 3.3V power pin but the video output
resistors will need to be changed to 680 ohms (sync) and 470 ohms (video).
 
*/

#include <SPI.h>

// one full frame of SPI data is 517 bytes. in the event of an overrun I reset the buffer back to 518
// at the end of the line. in high-speed mode (F_CLCK/4) we can overrun this by a maximum of 32 bytes,
// so to be safe the total buffer size should be 550 bytes multiplied by 2 for double-buffering.
byte buffer[550*2] __attribute__((used)) = {
  0x40,0x80,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0xaa,0x00,
  0x02,0xe0,0xf2,0x70,0x32,0x30,0x32,0x30,0x72,0xf0,0xe2,0x00,0x02,0x54,0xaa,0x00,0x02,0xe0,0xf2,0x70,0x32,
  0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,
  0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,
 
  0x41,0x80,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x03,0x51,0x05,0xfd,0x05,0xc5,0x05,0xe5,0xff,0x00,0xaa,0x00,
  0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x55,0xaa,0x00,0x00,0x1f,0x3f,0x38,0x30,
  0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
  0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
 
  0x42,0x80,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x02,0x03,0x02,0x02,0x02,0x03,0x03,0x00,0xaa,0x00,
  0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x55,0x00,0x71,0xfc,0xfd,0xdc,0xdd,0xd8,
  0x51,0x00,0x41,0x00,0x01,0x00,0x01,0x04,0x01,0x00,0x01,0x04,0x41,0x00,0x01,0x00,0x01,0x00,0x41,0x00,0x01,
  0x00,0x01,0x04,0x01,0x00,0x01,0xc0,0x41,0x40,0x41,0x40,0x41,0x78,0x09,0x10,0x21,0x40,0x81,0x00,0x01,0x00,
 
  0x43,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0x00,
  0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x55,0x00,0x10,0x00,0x01,0x01,0x81,0x00,
  0x00,0x04,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x80,0x00,0x00,0x04,0x00,0x00,0x00,
  0x40,0x00,0x00,0x00,0x00,0x00,0x0f,0x08,0x08,0x08,0x08,0x08,0x78,0x40,0x20,0x10,0x08,0x04,0x03,0x00,0x00,
 
  0x44,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0x00,
  0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x55,0x00,0x00,0x04,0x40,0x00,0x00,0x00,
  0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x40,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,
  0x00,0x00,0x04,0x00,0xaa,0x00,0x02,0xe0,0xf2,0x70,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,0x30,0x32,
 
  0x45,0x80,
  0x7c,0x44,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0x00,
  0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x55,0x00,0x10,0x00,0x00,0x00,0x80,0x00,
  0x00,0x04,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x80,0x00,0x00,0x04,0x00,0x00,0x00,
  0x40,0x00,0x00,0x00,0xaa,0x00,0x00,0x1f,0x3f,0x38,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
 
  0x40
};


#define SYNC_ON()      "out 0x08, r17   \n"
#define SYNC_OFF()     "out 0x08, r18   \n"

#define BLACK()        "out 0x08, r18   \n"
#define WHITE()        "out 0x08, r19   \n"
#define GREY()         "out 0x08, r20   \n"

#define BORDER()       WHITE()

// this macro is the heart and soul of the adapter, it loads, masks and displays 3 pixels from the current screen
// buffer whilst simultaneously polling the SPI bus for available bytes. timing is critical, it must be 24 cycles
// long with the 3 pixel outputs spaced 8 cycles apart.
#define DRAW_PIXELS()                                                                  \
    "ld r2, X+              \n" /* 2 - get screen byte 1                            */ \
    "and r2, r1             \n" /* 1 - mask screen byte 1                           */ \
    "in r2, 63              \n" /* 1 - get pixel 1                                  */ \
    "ld r3, X+              \n" /* 2 - get screen byte 2                            */ \
    "and r3, r1             \n" /* 1 - mask screen byte 2                           */ \
    "out 0x08, r2           \n" /* 1 - output pixel 1                               */ \
    "in r3, 63              \n" /* 1 - get pixel 2                                  */ \
    "ld r2, X+              \n" /* 2 - get screen byte 3                            */ \
    "and r2, r1             \n" /* 1 - mask screen byte 3                           */ \
    "in r2, 63              \n" /* 1 - get pixel 3                                  */ \
    "in r24, 0x2D           \n" /* 1 - load status                                  */ \
    "andi r24,0x80          \n" /* 1 - is the SPIF bit set?                         */ \
    "out 0x08, r3           \n" /* 1 - output pixel 2                               */ \
    "breq .+6               \n" /* 1/2 - is it set?                                 */ \
    "in r25, 0x2E           \n" /* 1 - read SPI byte                                */ \
    "st Y+, r25             \n" /* 2 - save it to the SPI buffer                    */ \
    "rjmp .+8               \n" /* 2 - all done                                     */ \
    "nop                    \n" /* 1 - cycle burn                                   */ \
    "nop                    \n" /* 1 - cycle burn                                   */ \
    "nop                    \n" /* 1 - cycle burn                                   */ \
    "nop                    \n" /* 1 - cycle burn                                   */ \
    "nop                    \n" /* 1 - cycle burn                                   */ \
    "out 0x08, r2           \n" /* 1 - output pixel 3                               */
   
// similar to the above routine but only checks for a single SPI byte
// and does no video output. burns 7 cycles exactly.
#define CHECK_SPI()                                                                    \
    "in r24, 0x2D           \n" /* 1 - load status                                  */ \
    "andi r24,0x80          \n" /* 1 - isolate the SPIF bit                         */ \
    "breq .+6               \n" /* 1/2 - is it set?                                 */ \
    "in r25, 0x2E           \n" /* 1 - yes, so read the SPI bytes                   */ \
    "st Y+, r25             \n" /* 2 - and save it                                  */ \
    "rjmp .+8               \n" /* 2                                                */ \
    "nop                    \n" /* 1 - SPIF bit wasn't set, so burn off a few       */ \
    "nop                    \n" /* 1 - nops in order to maintain exactly 11         */ \
    "nop                    \n" /* 1 - cycles per loop                              */ \
    "nop                    \n" /* 1                                                */
   
// polls the SPI bus for incoming bytes. burns 11*loops cycles exactly,
// so use wherever we need a long delay so that we don't miss any data.
#define POLL_SPI(loops)                                                                \
    "ldi r16," #loops "     \n"                                                        \ 
    CHECK_SPI()                                                                        \
    "subi r16,1             \n" /* 1 - finished?                                    */ \
    "brne .-24              \n" /* 1/2 - nope, jump back for more                   */

   
   
#define POLL_SPI_1(loops) POLL_SPI(loops)    "nop \n"
#define POLL_SPI_2(loops) POLL_SPI_1(loops)  "nop \n"
#define POLL_SPI_3(loops) POLL_SPI_2(loops)  "nop \n"
#define POLL_SPI_4(loops) POLL_SPI_3(loops)  "nop \n"
#define POLL_SPI_5(loops) POLL_SPI_4(loops)  "nop \n"
#define POLL_SPI_6(loops) POLL_SPI_5(loops)  "nop \n"
#define POLL_SPI_7(loops) POLL_SPI_6(loops)  "nop \n"
#define POLL_SPI_8(loops) POLL_SPI_7(loops)  "nop \n"
#define POLL_SPI_9(loops) POLL_SPI_8(loops)  "nop \n"
#define POLL_SPI_10(loops) POLL_SPI_9(loops) "nop \n"
   
// horizontal sync, 0V for 4uS
#define HSYNC()         SYNC_ON() POLL_SPI_8(5)

// front porch, 0.3V for 8uS.
#define FRONT_PORCH()   SYNC_OFF() POLL_SPI_6(11)

void setup() {
    // do lazy init for initial spi synch
    //SPI.begin();

  // make the sync and video pins outputs
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

}
   
void loop() {
 
  // enable SPI polling
  SPCR = (1<<SPE);
 
  __asm__ __volatile__ (

    "cli                    \n"
    "eor r0, r0             \n"
    "ldi r17, 0x00          \n" // sync output
    "ldi r18, 0x01          \n" // black output
    "ldi r19, 0x03          \n" // white ouput
    "ldi r20, 0x02          \n" // grey output
    "eor r4, r4             \n" // start on buffer 0
    "eor r5, r5             \n" // reset read timer
    "eor r22, r22           \n" // clear flip flag
    "call set_spi_buffer    \n"
   
    //////////////////////////////////////// main_loop ///////////////////////////////////////////
   
    "start_frame:           \n"
   
    // load current draw buffer address into X
    //"ldi r24, 38            \n"
    //"ldi r25, 2             \n"
    "ldi r26, lo8(buffer+2) \n"
    "ldi r27, hi8(buffer+2) \n"
    //"sbrc r4,0              \n"
    //"add r26,24             \n"
    //"sbrc r4,0              \n"
    //"adc r27,r25            \n"
   
    // store a copy of the current spi address
    //"mov r6,r28             \n"
    //"mov r7,r29             \n"
   
    // initialize the pixel mask to 0x01
    "eor r1, r1             \n"
    "inc r1                 \n"
    "eor r0, r0 \n"
   
    // lines 1-5, vertical sync
    "call long_sync         \n"
    "call long_sync         \n"
    "call long_sync         \n"
    "call long_sync         \n"
    "call long_sync         \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
   
   // 40 lines of header, first 16 aren't visible.
   // todo: lots of spare cycles here, maybe use for greyscale?
    "ldi r21, 40            \n"
    "header:                \n"   
    "call border_line       \n"
    "dec r21                \n"
    "brne header            \n"

    // draw the 48 LCD lines
    "ldi r21, 48            \n"
    "draw_frame:            \n"
   
    // each LCD line is drawn with 5 screen lines
    "call draw_line         \n"
    POLL_SPI_3(2)
    "call draw_line         \n"
    POLL_SPI_3(2)
    "call draw_line         \n"
    POLL_SPI_3(2)
    "call draw_line         \n"
    POLL_SPI_3(2)
    "call draw_line         \n"
    CHECK_SPI()
   
    // rotate the pixel mask each LCD line
    "add r1, r1             \n"
    "adc r1, r0             \n"
    //"nop \n nop \n"
   
    // if we've advanced a full row (8 LCD lines) then advance the
    // buffer ptr by 86 bytes (the dummy adds are to keep timing
    // constant when we haven't advanced)
    "sbrc r1,0              \n"
    "adiw r26,43            \n"   
    "sbrc r1,0              \n"
    "adiw r26,43            \n"   
    "sbrs r1,0              \n"
    "adiw r26,0             \n"   
    "sbrs r1,0              \n"
    "adiw r26,0             \n"
 
    // if we have more lines to draw then loop back
    "dec r21                \n"
    "breq frame_finished    \n"
    "jmp  draw_frame        \n"
    "frame_finished:        \n"
    "nop                    \n"

    // 24 lines of footer
    "ldi r21, 24            \n"
    "footer:                \n"   
    "call border_line       \n"
    "dec r21                \n"
    "brne footer            \n"
 
    // 3 lines of sync. ordinarily we should end with a final
    // call to short_sync but we need to lose a few cycles due
    // to per-screen overhead
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call short_sync        \n"
    "call last_sync         \n"
   
    // if it isn't time to flip then keep going
    //"or r22,r22             \n"
    //"breq still_reading     \n"
    //"eor r22,r22            \n" // clear the flip flag
   
    // calculate next buffer number
    //"mov r15,r4             \n"
    //"neg r15                \n"
    //"inc r15                \n"
   
    // how many SPI bytes have we received in total?
    //"mov r10,r28            \n"
    //"mov r11,r29            \n"
    //"sub r10,r8             \n"
    //"sbc r11,r9             \n"
   
    // if we've read at least 517 bytes then flip the frame
    //"ldi r24,5              \n"
    //"ldi r25,2              \n"
    //"sub r10,r24            \n"
    //"sbc r11,r25            \n"
    //"brlt still_reading     \n"
   
    // adjust for end of buffer
   
    // if we have exactly 517 bytes then swap the buffers. if we have more
    // than 517 bytes then we've had a buffer overrun, in which case just
    // restart the current buffer.
    //"breq .+2               \n"
    //"mov r4, r15            \n"
   
    // either way wait until the end of the frame and reset the SPI buffers
    //POLL_SPI(2)
    //"reset_frame:           \n"
    //"call set_spi_buffer    \n
    //POLL_SPI(2)
    //"jmp start_frame        \n"
   
    //"still_reading:         \n"
    //POLL_SPI_4(3)
    "jmp start_frame        \n"
   
    //////////////////////////////////////////// set_spi_buffer ///////////////////////////////////////////

    // called whenever the SPI buffer changes or needs to be reset due to invalid data
    "set_spi_buffer:       \n"

    // load start of current spi buffer into Y
    "ldi r28, lo8(buffer)  \n"
    "ldi r29, hi8(buffer)  \n"
    //"ldi r24, 38           \n"
    //"ldi r25, 2     \n"
    //"sbrc r4,0             \n"
    //"add r28,r24           \n"
    //"sbrc r4,0             \n"
    //"adc r29,r25           \n"
   
    // save into r9:r8
    "mov r8,r28            \n"
    "mov r9,r29            \n"
 
    // load end of current SPI buffer into Z. a buffer size >= 518
    // flags the entire block invalid so use that as the end so
    // that we have as many free bytes for overrun as possible
    "ldi r30, 6            \n"
    "ldi r31, 2            \n"
    //"ldi r30, 43            \n"
    //"ldi r31, 3            \n"
    "add r30,r28           \n"
    "adc r31,r29           \n"
   
    "ret                   \n"   
   
    //////////////////////////////////////////// overrun_check ///////////////////////////////////////////////
   
    // clamps the current SPI buffer pointer to the end of the current SPI buffer,
    // should be called at least once per line. total time = 12 cycles including the call.
    "overrun_check2:         \n"
    "mov r24,r28            \n"
    "mov r25,r29            \n"
    "sub r24,r30            \n"
    "sbc r25,r31            \n"
    "brmi .+2               \n"
    "mov r28,r30            \n"
    "brmi .+2               \n"
    "mov r29,r31            \n"
    "ret                    \n"

    //////////////////////////////////////////// draw_line ///////////////////////////////////////////
   
    "draw_line:             \n"
    HSYNC()
    FRONT_PORCH()
    BORDER()
    POLL_SPI_5(6)
   
    // save the current write ptr
    "mov r15, r28         \n"

    // the sync line is at PORTC bit-0 to improve chances of being able to implement greyscale in future,
    // for that reason it needs to be set to 1 while streaming out the pixel data.
    "sec                  \n"
    DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS()
    DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS()
    DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS()
    DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS() DRAW_PIXELS()
   
    // display the last pixel for an extra 7 cycles and finish off with an 80-cycle border
    "nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
    BORDER()
   
    // if we received display data on this line then reset the read timer, otherwise increment it
    "cp r15,r28             \n"
    "breq .+2               \n"
    "mov r5,r0              \n"
    "brne .+2               \n"
    "inc r5                 \n"
   
    CHECK_SPI()
   
    // when the read timer hits 5ms (i.e. ~78 lines) it's time to flip the buffer
    "ldi r24,78             \n"
    "cp r24,r5              \n"
    "brne line_no_flip        \n"
    //"mov r28,r8            \n"
    //"mov r29,r9            \n"
    //"mov r28,r8            \n"
    //"eor r5,r5             \n"
    //"dec r5                 \n"
    //"ldi r30, 43            \n"
    //"ldi r31, 3            \n"
    //"add r30,r28           \n"
    //"adc r31,r29           \n"
    //"nop                   \n"
    //"nop                   \n"
    //"nop                   \n"
    //"nop                   \n"
    //POLL_SPI_1(1)
   
    "ldi r28, lo8(buffer)  \n"
    "ldi r29, hi8(buffer)  \n"
    "mov r8,r28            \n"
    "mov r9,r29            \n"
    "ldi r30, 43           \n"
    "ldi r31, 3            \n"
    "add r30,r28           \n"
    "adc r31,r29           \n"
    "mov r5,r0            \n"
    //"nop                   \n"
    //"in r25, 0x2E           \n" /* 1 - yes, so read the SPI bytes                   */ \
    //"out 0x2c, 0 \n"
    //"ldi r24,0x40 \n"
    "out 0x2e, 0 \n"
   
    "jmp line_done         \n"
   
    "line_no_flip:           \n"
    POLL_SPI_2(1)
   
    "line_done:              \n"
   
    // restore display ptr to what it was at the start and we're done with this line
    "sbiw  r26, 42          \n"
    "sbiw  r26, 42          \n"
   
    "jmp overrun_check2     \n"
   
    //////////////////////////////////////////// border_line ///////////////////////////////////////////////
   
    "border_line:           \n" // cue bad Madonna joke....
    HSYNC()
    FRONT_PORCH()
    BORDER()
    POLL_SPI_6(73)
    "jmp  overrun_check2     \n"
   
    //////////////////////////////////////////// long_sync ///////////////////////////////////////////////
   
    // 0V for 30uS then 0.3V for 2uS
    "long_sync:             \n"
    "nop                    \n"
    SYNC_ON()
    POLL_SPI_6(43)
    SYNC_OFF()
    POLL_SPI(1)
    "jmp  overrun_check2     \n"
   
    //////////////////////////////////////////// short_sync //////////////////////////////////////////////
   
    // 0V for 2uS then 0.3V for 30uS
    "short_sync:            \n"
    "nop                    \n"
    SYNC_ON()
    POLL_SPI_9(2)
    SYNC_OFF()
    POLL_SPI_8(41)
    "jmp  overrun_check2     \n"
   
    ///////////////////////////////////////////// last_sync //////////////////////////////////////////////
   
    // same as short sync, but we return early so that we
    // have time left over for end-of-frame house-keeping
    "last_sync:             \n"
    "nop                    \n"
    SYNC_ON()
    POLL_SPI_9(2)
    SYNC_OFF()
    POLL_SPI_2(37)
    "jmp  overrun_check2     \n"
  );
}
