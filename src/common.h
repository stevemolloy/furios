/*common.h -- Defines typedefs and some global functions.*/
/*From JamesM's kernel development tutorials.*/

#ifndef COMMON_H
#define COMMON_H

/*Some nice typedefs, to standardise sizes across platforms.*/
/*These typedefs are written for 32-bit X86.*/
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

/*Port I/O*/
void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

/*Count characters in a string*/
int strlen(char *s);

/*Set "count" bytes (16-bit) in "dest" to "val"*/
/*unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);*/
void memsetw(unsigned short *dest, unsigned short val, int count);

/*Set "count" bytes (8-bit) in "dest" to "val"*/
/*unsigned char *memset(unsigned char *dest, unsigned char val, int count);*/
void memset(unsigned char *dest, unsigned char val, int count);

/*Copy count bytes (8-bit) from src to dest*/
/*unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);*/
void memcpy(unsigned char *dest, const unsigned char *src, int count);

void PANIC(char *mess);

#endif

