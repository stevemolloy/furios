// Much of this is based on James Molloys tutorial,
// but it also represents my first attempts to write code myself!

#include "monitor.h"

int cursor_x = 0;
int cursor_y = 0;
u16int *video_memory = (u16int *)0xB8000; // Video framebuffer begins at 0xB8000
u8int attrib = 15;   // white on black

// Updates the hardware cursor.
static void move_cursor()
{
   // The screen is 80 characters wide...
   u16int cursorLocation = cursor_y * 80 + cursor_x;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
} 

void monitor_clear()
{
    u8int space  = 0x20; // a space symbol
    u8int attrib = 15;   // white on black
    u16int sym = (attrib << 8) | space;
    int i;
    for (i=0; i < 80*25; i++)
    {
        video_memory[i] = sym;
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void monitor_put(char c)
{
    // Newline
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }

    // Carriage return
    if (c == '\r')
    {
        cursor_x = 0;
    }

    if (c >= ' ')
    {
        u16int sym = (attrib << 8) | c;
        video_memory[cursor_y*80 + cursor_x] = sym;
        cursor_x++;
    }

    if (cursor_x>=80)
    {
        cursor_x = 0;
        cursor_y++;
    }
    move_cursor();
}

void monitor_write(char *c)
{
    int i=0;
    while (c[i])
    {
        monitor_put(c[i++]);
    }
}

