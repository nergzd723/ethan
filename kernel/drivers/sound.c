#include <stdint.h>
#include <io.h>
#include <timer.h>
#include <sound.h>

 static void play_sound(uint32_t nFrequence) {
	asm volatile("sti");
 	uint32_t Div;
 	uint8_t tmp;
 
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
    // play the sound using the PC speaker
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
	spause(time);
	nosound();
}
void spause(uint32_t time){
	waitm(time);
}
void happy(){
	playnote(D5, QUADRO);
	playnote(D5, QUADRO);
	playnote(E5, QUADRO);
	playnote(D5, QUADRO);
	playnote(G5, QUADRO);
	playnote(F5, DOUBLE);
	playnote(D5, QUADRO);
	playnote(D5, QUADRO);
	playnote(E5, QUADRO);
	playnote(D5, QUADRO);
	playnote(A5, QUADRO);
	playnote(G5, DOUBLE);
	playnote(D5, QUADRO);
	playnote(D5, QUADRO);
	playnote(D7, QUADRO);
	playnote(B5, QUADRO);
	playnote(A5, QUADRO);
	playnote(G5, QUADRO);
	playnote(F5, QUADRO);
	playnote(C6, QUADRO);
	playnote(C6, QUADRO);
	playnote(B5, QUADRO);
	playnote(G5, QUADRO);
	playnote(A5, QUADRO);
	playnote(G5, DOUBLE);
}
 //Make a beep
 void beep() {
 	 playnote(C6, 100);
 }