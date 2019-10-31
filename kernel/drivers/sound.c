#include <stdint.h>
#include <io.h>
#include <timer.h>
#include <sound.h>

 static void play_sound(uint32_t nFrequence) {
	asm volatile("sti");
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shutup
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
 }
 
static void playnote(uint32_t freq, uint32_t time){
	play_sound(freq);
	waitm(time);
	nosound();
}
void happy(){
	playnote(C5, 1);
	playnote(D5, 1);
	playnote(E5, 1);
	playnote(F5, 1);
	playnote(G5, 1);
	playnote(A5, 1);
	playnote(B5, 1);
	playnote(C6, 1);
	playnote(D6, 1);
	playnote(E6, 1);
	playnote(F6, 1);
	playnote(G6, 1);
	playnote(A6, 1);
	playnote(B6, 1);
	playnote(C7, 1);
	playnote(D7, 1);
	playnote(E7, 1);
	playnote(F7, 1);
	playnote(G7, 1);
	playnote(A7, 1);
	playnote(B7, 1);
}
 //Make a beep
 void beep() {
 	 playnote(C6, 100);
 }