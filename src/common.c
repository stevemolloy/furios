/*common.c -- Defines some global functions.*/
/*From JamesM's kernel development tutorials.*/

#include "common.h"

/*Write a byte out to the specified port.*/
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
   u8int ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

u16int inw(u16int port)
{
   u16int ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
} 

int strlen(char *s)
{
    int i;
    for (i=0; *s!='\0'; s++)
    {
        i++;
    }
    return i;
}

/*Copy "count" bytes from src to dest, and return dest*/
/*unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)*/
void memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    int i;
    for (i=0; i<count; i++)
    {
        dest[i] = src[i];
    }
}

/*Set "count" bytes (16-bit) in "dest" to "val"*/
/*unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)*/
void memsetw(unsigned short *dest, unsigned short val, int count)
{
    int i;
    for (i=0; i<count; i++)
    {
        dest[i] = val;
    }
}

/*Set "count" bytes (8-bit) in "dest" to "val"*/
/*unsigned char *memset(unsigned char *dest, unsigned char val, int count)*/
void memset(unsigned char *dest, unsigned char val, int count)
{
    int i;
    for (i=0; i<count; i++)
    {
        dest[i] = val;
    }
}

