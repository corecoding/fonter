/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <sys/types.h>
#include <stdio.h>
#include <ncurses/curses.h>
#include <signal.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <termios.h>

#include "inc/credits.h"
#include "inc/compiled.h"
#include "inc/fontdir.h"
#include "inc/fonter.h"
#include "inc/fade.h"
#include "inc/bdatapcf.h"
#include "inc/edatapcf.h"


/************* MAIN CODE BEGINS HERE *************/

int main(int argc, char **argv)
{
	if (argc > 1) {
		for (i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-c")) {
				COL = 0;
			}
			if (!strcmp(argv[i], "-a")) {
				ASC = 0;
			}
			if (!strcmp(argv[i], "-s")) {
				SET = 0;
			}
			if (!strcmp(argv[i], "-f")) {
				FDE = 0;
			}
			if (!strcmp(argv[i], "-n")) {
				DET = 0;
			}
			if (!strcmp(argv[i], "-d")) {
				printf("Last revised on %s.\n", COMP);
				exit(1);
			}
		}

		if ((!strcmp(argv[1], "-h"))
		    || (!strcmp(argv[1], "--help"))) {
			printf("\nUsage: fonter [OPTIONS]...\n\nOPTIONS		DESCRIPTION\n  -n		Skip color detection at startup (Please don't use X).\n  -a		Don't use high ascii, just low.\n  -f		Don't use the spiffy fading effects.\n  -s		Don't update characters immediately (not advised).\n  -c		Don't use colors.  (OUCH!!)\n  -d		Date when last revised.\n\n");
			exit(0);
		}
	}
	init();
	fadeout();
	clr();
	drwscrn();
	fadein();
	refresh();

	while (1) {
		key = getch();

		switch (isalpha(key) ? toupper(key) : key) {

		case KEY_LEFT:
			if (m == 1) {
				barX--;
			} else {
				cnX--;
			}
			if (w == 2) {
				if (barX < 1)
					barX = 1;
				if (cnX < 0)
					cnX = 0;
			}
			if (w == 1) {
				if (barX < 1)
					barX = 8;
				if (cnX < 0)
					cnX = 15;
			}
			status("Going Left");
			break;

		case KEY_RIGHT:
			if (m == 1) {
				barX++;
			} else {
				cnX++;
			}
			if (w == 2) {
				if (barX > 8)
					barX = 8;
				if (cnX > 15)
					cnX = 15;
			}
			if (w == 1) {
				if (barX > 8)
					barX = 1;
				if (cnX > 15)
					cnX = 0;
			}
			status("Going Right");
			break;

		case KEY_UP:
			if (m == 1) {
				barY--;
			} else {
				cnY--;
			}
			if (w == 2) {
				if (barY < 1)
					barY = 1;
				if (cnY < 0)
					cnY = 0;
			}
			if (w == 1) {
				if (barY < 1)
					barY = 16;
				if (cnY < 0)
					cnY = 15;
			}
			status("Going Up");
			break;

		case KEY_DOWN:
			if (m == 1) {
				barY++;
			} else {
				cnY++;
			}
			if (w == 2) {
				if (barY > 16)
					barY = 16;
				if (cnY > 15)
					cnY = 15;
			}
			if (w == 1) {
				if (barY > 16)
					barY = 1;
				if (cnY > 15)
					cnY = 0;
			}
			status("Going Down");
			break;

		case '\t':
			if (m++ >= 2)
				m = 1;
			if (m == 1) {
				status("Window #1");
			} else {
				status("Window #2");
			}
			break;

		case '\n':
			status("Going to Editor");
			if (m == 2)
				m = 1;
			break;

		case 'Q':
		case 27:
			endprog();
			break;

		case '/':
			keypad(stdscr, FALSE);
			i = getch();
			keypad(stdscr, TRUE);
			cnX = (i / 16) - 1;
			cnY = i - (i - cnY);
			printf("\e[1;32m\e[1;1H%d:%d", cnX, cnY);
			enlarge(i);
			break;

		case ' ':
			if (m == 1)
				space();
			break;

		case '1':
			save();
			break;

		case '2':
			filemenu();
			break;

		case '3':
			if (w++ >= 2)
				w = 1;
			if (w == 1) {
				status("Wrap mode on");
				mvprintz(5, 46, "%YON ");
			}
			if (w == 2) {
				status("Wrap mode off");
				mvprintz(5, 46, "%YOFF");
			}
			break;
		case '4':
			if ((m == 1) || (m == 2))
				clearit();
			break;

		case '5':
			if ((m == 1) || (m == 2))
				inverse();
			break;

		case '6':
			if ((m == 1) || (m == 2))
				flipud();
			break;

		case '7':
			if ((m == 1) || (m == 2))
				fliprl();
			break;

		case 'A':
			shrinkud();
			break;

		case 'C':
			if ((m == 1) || (m == 2))
				copychar();
			break;

		case 'H':
			goleft();
			break;

		case 'I':
			info();
			break;

		case 'J':
			godown();
			break;

		case 'K':
			goup();
			break;

		case 'L':
			goright();
			break;

		case 'O':
			backup();
			break;

		case 'P':
			if ((m == 1) || (m == 2))
				pastechar();
			break;

		case 'R':
			reschar();
			break;

		case 'S':
			shrinkrl();
			break;

		case 262:
			cnY--;
			if (w == 2) {
				if (cnY < 0)
					cnY = 0;
			}
			if (w == 1) {
				if (cnY < 0)
					cnY = 15;
			}
			status("Going Up Row");
			break;

		case 265:
			savepcf();
			status("Saved vga.pcf");
			break;

		case 330:
			status("2nd Help Screen");
			for (i = 16; i <= 31; i++) {
				mvprintz(i - 8, 41, cmds[i]);
				charz(42 - strlen(cmds[i]), " ");
			}
			hlp = 2;
			break;

		case 331:
			status("1st Help Screen");
			for (i = 0; i <= 15; i++) {
				mvprintz(i + 8, 41, cmds[i]);
				charz(42 - strlen(cmds[i]), " ");
			}
			hlp = 1;
			break;

		case 338:
			status("Next Char");
			if ((m == 1) || (m == 2))
				if (((cnY * 16) + cnX) < 255) {
					if (cnX == 15) {
						cnX = 0;
						if (cnY < 15)
							cnY++;
					} else {
						cnX++;
					}
				}
			break;

		case 339:
			status("Previous Char");
			if ((m == 1) || (m == 2))
				if (((cnY * 16) + cnX) > 0) {
					if (cnX == 0) {
						cnX = 15;
						if (cnY != 0)
							cnY--;
					} else {
						cnX--;
					}
				}
			break;

		case 360:
			cnY++;
			if (w == 2) {
				if (cnY > 15)
					cnY = 15;
			}
			if (w == 1) {
				if (cnY > 15)
					cnY = 0;
			}
			status("Going Down Row");
			break;

		default:
			status("Unknown command!");
			break;

		case ERR:
			break;

		}
		updateboxes();
		refresh();
	}
}

void line()
{
	printz("%W");
	switch (m) {
	case 1:
		move_to(7, 1);
		printf("%s%s%s%s%s", Asc[6], Asc[8], Asc[8], Asc[8], Asc[8]);
		move_to(7, 14);
		printf("%s%s%s%s%s", Asc[8], Asc[8], Asc[8], Asc[8], Asc[7]);
		break;
	case 2:
		move_to(7, 20);
		printf("%s%s%s", Asc[6], Asc[8], Asc[8]);
		move_to(7, 35);
		printf("%s%s%s", Asc[8], Asc[8], Asc[7]);
		break;
	}
	printz("%K");
	switch (om) {
	case 1:
		move_to(7, 1);
		printf("%s%s%s%s%s", Asc[0], Asc[4], Asc[4], Asc[4], Asc[4]);
		move_to(7, 14);
		printf("%s%s%s%s%s", Asc[4], Asc[4], Asc[4], Asc[4], Asc[1]);
		break;
	case 2:
		move_to(7, 20);
		printf("%s%s%s", Asc[0], Asc[4], Asc[4]);
		move_to(7, 35);
		printf("%s%s%s", Asc[4], Asc[4], Asc[1]);
		break;
	}
	refresh();

	printz("%Y");
	if (m == 1) {
		move_to(2, 17);
		printf("%02d", barX);
		move_to(2, 13);
		printf("%02d", barY);
	}
	if (m == 2) {
		move_to(2, 17);
		printf("%02d", cnX + 1);
		move_to(2, 13);
		printf("%02d", cnY + 1);
	}
}

void endprog()
{
	char bl[500];

	status("Leaving NOW!");
	fadeout();
	sprintf(bl, "%%w%%!Goodbye, and thanks for using fonter %%Wv%s%%w.", VER);
	printz(bl);
	endwin();
	fadein();
	printf("\e[2;1H");
	exit(1);
}

void error(char *er)
{
	endwin();
	printf("Error - %s\n", er);
	exit(1);
}

void init()
{
	initscr();
	signal(SIGINT, SIG_IGN);
	printf("\e(U\e(K");

	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	fd = open("/dev/tty", O_RDWR);
	getfont();
	memset(cpaste, 0, sizeof(cpaste));
	setupchars();
	asciido();

	if (COLS < 80 || LINES < 25)
		error("try a 80x25 terminal next time.");

	if (!has_colors() && (DET == 1))
		error("no color support on this terminal.  You can try \"-n\".");

	if (start_color() != OK && (DET == 1))
		error("could not initalize colors");

	if (getenv("DISPLAY"))
		error("please don't run fonter from X windows.");
}

void clr()
{
	clear();
	refresh();
}

void drawbox(int y0, int x0, int dy, int dx, char *str)
{
	register x, y;
	char temp[30];

	for (x = 0; x <= dx; x++) {
		mvprintz(y0, x0 + x, Asc[4]);
		mvprintz(y0 + dy + 1, x0 + x, Asc[4]);
	}
	for (y = 0; y <= dy; y++) {
		mvprintz(y0 + y, x0, Asc[5]);
		mvprintz(y0 + y, x0 + dx + 1, Asc[5]);
	}

	mvprintz(y0, x0, Asc[0]);
	mvprintz(y0 + dy + 1, x0, Asc[2]);
	mvprintz(y0, x0 + dx + 1, Asc[1]);
	mvprintz(y0 + dy + 1, x0 + dx + 1, Asc[3]);

	sprintf(temp, "%s%s%s", "%B[%G", str, "%B]%K");

	mvprintz(y0, ((x0 + (dx / 2) + 1) - (strlen(str) / 2)) - 1, temp);
}

void drwscrn()
{
	int i, p, key;

	printz("%K");

	drawbox(7, 1, 16, 16, "Editor");
	drawbox(7, 20, 16, 16, "Char Table");
	drawbox(7, 39, 16, 38, "Help");
	drawbox(1, 67, 1, 5, "Key");
	drawbox(4, 1, 1, 28, "Filename");
	drawbox(4, 53, 1, 19, "Status");
	drawbox(1, 1, 1, 5, "Chr");
	drawbox(1, 10, 1, 10, "Location");
	drawbox(1, 24, 1, 5, "Num");
	drawbox(1, 33, 1, 5, "Hex");
	drawbox(1, 42, 1, 7, "Octal");
	drawbox(1, 53, 1, 10, "Binary");
	drawbox(4, 43, 1, 6, "Wrap");
	drawbox(4, 33, 1, 5, "Ver");

	mvprintz(2, 3, "%Y\" \"");
	mvprintz(2, 12, "%G[%Y01%G][%Y01%G]");
	mvprintz(2, 28, "%Y0");
	mvprintz(2, 36, "00");
	mvprintz(2, 45, "0000");
	mvprintz(2, 55, "00000000");
	mvprintz(2, 69, "000");
	mvprintz(5, 46, "OFF");
	mvprintz(5, 35, VER);
	mvprintz(5, 3, "%Wno name yet           .fnt");

	printz("%W");

	for (i = 0; i <= 15; i++)
		mvprintz(8 + i, 41, cmds[i]);

	printz("%g%N");
	key = 0;
	for (i = 0; i <= 15; i++)
		for (p = 0; p <= 15; p++)
			printf("\e[%d;%df%c", i + 9, p + 22, chars[key++]);

	printz("%W");
	move_to(7, 1);
	printf("%s%s%s%s%s", Asc[6], Asc[8], Asc[8], Asc[8], Asc[8]);
	move_to(7, 14);
	printf("%s%s%s%s%s", Asc[8], Asc[8], Asc[8], Asc[8], Asc[7]);
	mvprintz(8, 21, "%n%P%5 %0");
	status("Waiting for input");
	enlarge(0);
}

void move_to(int a0, int a1)
{
	printf("\e[%d;%dH", a0 + 1, a1 + 1);
}

void mvprintz(int a0, int a1, char *str)
{
	printf("\e[%d;%df", a0 + 1, a1 + 1);
	printz(str);
	refresh();
}

void printz(char *str,...)
{
	int z;
	char bl[500];
	va_list ap;

//      va_start(ap, str);
//      vsprintf(bl, str, ap);

	for (z = 0; z <= strlen(str); z++) {
		if (str[z] == '%') {
			if (COL) {
				switch (str[z + 1]) {
				case '!':
					printf("\e[H\e[2J");
					refresh();
					break;
				case 'n':
					printf("\e[10m");
					break;
				case 'N':
					printf("\e[12m");
					break;
				case 'k':
					printf("\e[0;30m");
					break;
				case 'K':
					printf("\e[1;30m");
					break;
				case 'r':
					printf("\e[0;31m");
					break;
				case 'R':
					printf("\e[1;31m");
					break;
				case 'g':
					printf("\e[0;32m");
					break;
				case 'G':
					printf("\e[1;32m");
					break;
				case 'y':
					printf("\e[0;33m");
					break;
				case 'Y':
					printf("\e[1;33m");
					break;
				case 'b':
					printf("\e[0;34m");
					break;
				case 'B':
					printf("\e[1;34m");
					break;
				case 'p':
					printf("\e[0;35m");
					break;
				case 'P':
					printf("\e[1;35m");
					break;
				case 'c':
					printf("\e[0;36m");
					break;
				case 'C':
					printf("\e[1;36m");
					break;
				case 'w':
					printf("\e[0;37m");
					break;
				case 'W':
					printf("\e[1;37m");
					break;
				case 'F':
					printf("\e[5m");
					break;
				case '0':
					printf("\e[40m");
					break;
				case '1':
					printf("\e[41m");
					break;
				case '2':
					printf("\e[42m");
					break;
				case '3':
					printf("\e[43m");
					break;
				case '4':
					printf("\e[44m");
					break;
				case '5':
					printf("\e[45m");
					break;
				case '6':
					printf("\e[46m");
					break;
				case '7':
					printf("\e[47m");
					break;
				}
			}
			z++;
			refresh();
		} else {
			printf("%c", str[z]);
		}
	}
}

void charz(int a, char *ch)
{
	int bl;

	for (bl = 1; bl <= a; bl++)
		printf("%s", ch);
}

void updateboxes()
{
	unsigned char t;
	int mod = 256;

	if (m != om) {
		line();
		om = m;
	}
	if ((obY != barY) || (obX != barX)) {
		printz("%Y");
		move_to(2, 17);
		printf("%02d", barX);
		move_to(2, 13);
		printf("%02d", barY);

		i = (cnY * 16) + cnX;

		printz((buf[(i * 32) + (barY - 1)] & arr[barX - 1])
		       ? "%Y" : "%R");

		mvprintz(barY + 7, barX * 2, "ÛÛ");

		printz((buf[(i * 32) + (obY - 1)] & arr[obX - 1]) ? "%b" : "%k");

		mvprintz(obY + 7, obX * 2, "ÛÛ");

		if (!COL)
			enlarge(i);

		obY = barY;
		obX = barX;
	}
	if ((ocY != cnY) || (ocX != cnX)) {
		i = (cnY * 16) + cnX;
		mvprintz(2, 4, "%W\e[12m");
		printf("%c", chars[i]);
		mvprintz(cnY + 8, cnX + 21, "%P%5");
		printf("%c", chars[i]);
		mvprintz(ocY + 8, ocX + 21, "%g%0");
		printf("%c\e[10m", chars[(ocY * 16) + ocX]);

		mvprintz(2, 55, "%Y");

		while (mod != 1) {
			mod /= 2;
			if ((i & mod) == 0) {
				printf("0");
			} else {
				printf("1");
			}
		}

		printz("%Y");
		move_to(2, 36);
		printf("%.2X", i);
		move_to(2, 26);
		printf("%.3d", i);
		move_to(2, 45);
		printf("%.4o", i);
		move_to(2, 17);
		printf("%02d", cnX + 1);
		move_to(2, 13);
		printf("%02d", cnY + 1);

		ocY = cnY;
		ocX = cnX;

		enlarge(i);

	}
	mvprintz(2, 69, "%Y");
	printf("%03d", key);
}

void setfont()
{
	if (SET)
		ioctl(fd, PIO_FONT, buf);
}

void getfont()
{
	int d, o = 0;

	memset(buf, 0, sizeof(buf));
	memset(font, 0, sizeof(font));
	if (SET)
		ioctl(fd, GIO_FONT, buf);
	for (i = 0; i < 256; i++)
		for (d = 0; d < fs; d++)
			font[o++] = buf[(i * 32) + d];
}

void setupchars()
{
	int blanks[] = {155, 27, 8, 10, 12, 13, 14, 15, 0};

	for (i = 0; i < 256; i++) {
		chars[i] = i;
	}

	for (i = 0; blanks[i] != 0; i++)
		chars[blanks[i]] = ' ';

	chars[0] = ' ';

	if (!ASC) {
		for (i = 1; i < 32; i++)
			chars[i] = 32;
		for (i = 127; i < 256; i++)
			chars[i] = 32;
	}
}

void enlarge(int charnum)
{
	int mod, num, cnt;

	printz("%b");
	for (i = 0; i <= (fs - 1); i++) {
		num = buf[(charnum * 32) + i];
		cnt = 0;
		mod = 256;
		while (mod != 1) {
			cnt++;
			mod /= 2;
			move_to(i + 8, cnt * 2);
			printf("%s", (num & mod) ? "ÛÛ" : "  ");
		}
	}

	printz((buf[(charnum * 32) + (barY - 1)] & arr[barX - 1])
	       ? "%Y" : "%R");

	mvprintz(barY + 7, barX * 2, "ÛÛ");

	refresh();
}

void inverse()
{
	for (i = 0; i <= 15; i++)
		buf[(((cnY * 16) + cnX) * 32) + i] =
		    ~(buf[(((cnY * 16) + cnX) * 32) + i]);

	status("Inverting");
	setfont();
	enlarge((cnY * 16) + cnX);
}

void space()
{
	buf[(((cnY * 16) + cnX) * 32) + (barY - 1)] ^= (arr[barX - 1]);
	if (buf[(((cnY * 16) + cnX) * 32) + (barY - 1)] & arr[barX - 1]) {
		status("Bit goes: ON");
	} else {
		status("Bit goes: OFF");
	}

	setfont();
	enlarge((cnY * 16) + cnX);
}

void clearit()
{
	for (i = 0; i <= 15; i++)
		buf[(((cnY * 16) + cnX) * 32) + i] = 0;
	status("Clearing Char");
	setfont();
	enlarge((cnY * 16) + cnX);
}

void copychar()
{
	for (i = 0; i <= 15; i++)
		cpaste[i] = buf[(((cnY * 16) + cnX) * 32) + i];
	status("Copying Char");
}

void pastechar()
{
	for (i = 0; i <= 15; i++)
		buf[(((cnY * 16) + cnX) * 32) + i] = cpaste[i];
	status("Pasting Char");
	setfont();
	enlarge((cnY * 16) + cnX);
}

void flipud()
{
	unsigned char ch = (cnY * 16) + cnX;
	int o = 15, d;

	for (d = 0; d <= 7; d++) {
		i = buf[(ch * 32) + d];
		buf[(ch * 32) + d] = buf[(ch * 32) + o];
		buf[(ch * 32) + o--] = i;
	}

	status("Flipping Y-Vert");
	setfont();
	enlarge(ch);
}

void fliprl()
{
	unsigned char ch = (cnY * 16) + cnX, val2, x;

	for (i = 0; i <= 15; i++) {
		tmp = buf[(ch * 32) + i];
		for (x = 0, val2 = 0; x < 8; x++)
			if (tmp & (1 << x))
				val2 |= (0x80 >> x);

		buf[(ch * 32) + i] = val2;
	}

	status("Flipping X-Vert");
	setfont();
	enlarge(ch);
}

void goleft()
{
	unsigned char ch = (cnY * 16) + cnX, tmp;

	for (i = 0; i <= 15; i++) {
		tmp = buf[(ch * 32) + i] << 1;
		if (buf[(ch * 32) + i] & arr[0])
			tmp ^= arr[7];
		buf[(ch * 32) + i] = tmp;
	}

	status("Scooting Left");
	setfont();
	enlarge(ch);
}

void goright()
{
	unsigned char ch = (cnY * 16) + cnX;

	for (i = 0; i <= 15; i++) {
		tmp = buf[(ch * 32) + i] >> 1;
		if (buf[(ch * 32) + i] & arr[7])
			tmp ^= arr[0];
		buf[(ch * 32) + i] = tmp;
	}

	status("Scooting Right");
	setfont();
	enlarge(ch);
}

void goup()
{
	unsigned char ch = (cnY * 16) + cnX;

	tmp = buf[ch * 32];

	for (i = 0; i <= 14; i++)
		buf[(ch * 32) + i] = buf[(ch * 32) + i + 1];

	buf[(ch * 32) + 15] = tmp;

	status("Scooting Up");
	setfont();
	enlarge(ch);
}

void godown()
{
	unsigned char ch = (cnY * 16) + cnX;

	tmp = buf[(ch * 32) + 15];

	for (i = 15; i >= 0; i--)
		buf[(ch * 32) + (i + 1)] = buf[(ch * 32) + i];

	buf[ch * 32] = tmp;

	status("Scooting Down");
	setfont();
	enlarge(ch);
}

void backup()
{
	int d, o = 0;

	for (i = 0; i < 256; i++)
		for (d = 0; d < fs; d++)
			buf[(i * 32) + d] = font[o++];

	status("Pasting Old Font");
	setfont();
	enlarge((cnY * 16) + cnX);
}

void status(char *str)
{
	mvprintz(5, 55, "%W");
	printf("%s", str);
	charz(17 - strlen(str), " ");
}

void asciido()
{
	if (ASC) {
		Asc[0] = "Ú";
		Asc[1] = "¿";
		Asc[2] = "À";
		Asc[3] = "Ù";
		Asc[4] = "Ä";
		Asc[5] = "³";
		Asc[6] = "Õ";
		Asc[7] = "¸";
		Asc[8] = "Í";
	} else {
		Asc[0] = ".";
		Asc[1] = ".";
		Asc[2] = "`";
		Asc[3] = "'";
		Asc[4] = "-";
		Asc[5] = "|";
		Asc[6] = ".";
		Asc[7] = ".";
		Asc[8] = "=";
	}
}

void reschar()
{
	int d;

	i = (cnY * 16) + cnX;

	for (d = 0; d < fs; d++)
		buf[(i * 32) + d] = font[(16 * i) + d];

	status("Pasting Old Char");
	setfont();
	enlarge(i);
}

void shrinkud()
{
	int ch = (cnY * 16) + cnX;

	buf[ch * 32] = 0;
	buf[(ch * 32) + 15] = 0;

	for (i = 7; i >= 0; i--)
		buf[(ch * 32) + i] = buf[(ch * 32) + (i - 1)];

	for (i = 8; i <= 14; i++)
		buf[(ch * 32) + i] = buf[(ch * 32) + (i + 1)];

	status("Shrinking Y-Vert");
	setfont();
	enlarge(ch);
}

void shrinkrl()
{
	int ch = (cnY * 16) + cnX;

	for (i = 0; i <= 15; i++) {
		tmp = buf[(ch * 32) + i];
		tmp = ((tmp & 11100000) >> 4 | (tmp & 00011110) << 3);
		buf[(ch * 32) + i] = tmp;
	}

	status("Shrinking X-Vert");
	setfont();
	enlarge(ch);
}

int loadfiles()
{
	DIR *d;
	struct dirent *de = calloc(1, sizeof *de), *tmp;
	char bl[500];
	int dip = 0;

	cnt = 0;
	bzero(bl, sizeof(bl));

	if (!de)
		return perror("calloc"), -1;

	if (!(d = opendir(FONTDIR)))
		return perror("opendir(\".\")"), -1;

	while ((tmp = readdir(d))) {
		de = realloc(de, sizeof *de * (cnt + 1));
		if (!de)
			return perror("realloc"), -1;
		de[cnt++] = *tmp;
	}

	memset(files, 0, sizeof(files));

	for (i = 0; i < cnt; i++) {
		if (strcmp(de[i].d_name, ".fnt") > 0) {
			files[dip++] = de[i].d_name;
//              files[dip][13] = 0;
			//              files[dip][14] = 0;
		}
	}
	cnt = dip;

	for (dip = 0; dip < (cnt - 1); dip++)
		for (i = 0; i < (cnt - 1); i++)
			if (strcmp(files[i], files[i + 1]) > 0) {
				strcpy(bl, files[i + 1]);
				strcpy(files[i + 1], files[i]);
				strcpy(files[i], bl);
			}
}

void info()
{
	int p;

	status("Information");
	move_to(10, 0);
	printf("%s", screendata);
	refresh();
	getch();

	printz("%K");

	for (i = 10; i <= 16; i++) {
		mvprintz(i, 0, " ");
		mvprintz(i, 19, " ");
		mvprintz(i, 38, " ");
		mvprintz(i, 40, " ");
		mvprintz(i, 77, " ");
		mvprintz(i, 79, " ");
		mvprintz(i, 1, Asc[5]);
		mvprintz(i, 18, Asc[5]);
		mvprintz(i, 20, Asc[5]);
		mvprintz(i, 37, Asc[5]);
		mvprintz(i, 39, Asc[5]);
		mvprintz(i, 78, Asc[5]);
	}

	printz("%g%N");

	key = 0;

	for (i = 0; i <= 15; i++)
		for (p = 0; p <= 15; p++)
			printf("\e[%d;%df%c", i + 9, p + 22, chars[key++]);

	if (hlp == 1) {
		for (i = 0; i <= 15; i++) {
			mvprintz(i + 8, 41, cmds[i]);
			charz(42 - strlen(cmds[i]), " ");
		}
	} else {
		for (i = 16; i <= 31; i++) {
			mvprintz(i - 8, 41, cmds[i]);
			charz(42 - strlen(cmds[i]), " ");
		}
	}

	i = (cnY * 16) + cnX;

	printz("%P%5");
	printf("\e[%d;%df%c", 9 + cnY, 22 + cnX, chars[i]);
	printz("%0%n");

	enlarge(i);

	status("Waiting for input");
}

void filemenu()
{
	int countah = 0, liner = 0, oliner = 0, ocnt = 0, updown = 0;
	FILE *fpo;
	char bl[50];

	bzero(bl, sizeof(bl));
	status("Listing files");
	if (m != 1) {
		m = 1;
		line();
	}
	mvprintz(7, 7, "%GFiles!%w");
	loadfiles();
	move_to(24, 3);
	printf("001/%03d", cnt);

	for (i = 0; i <= 15; i++) {
		mvprintz(8 + i, 2, "                ");
	}
	for (i = 1; i <= 15; i++) {
		move_to(8 + i, 3);
		printf("%.14s", files[i]);
	}
	if ((countah + liner + 16) < cnt)
		mvprintz(24, 15, "%w");
	mvprintz(8, 3, "%W%4");
	printf("%s", files[liner]);
	for (i = 0; i <= 13 - strlen(files[liner]); i++)
		printf(" ");
	refresh();

	while (1 != 2) {
		/*      if (updown == 0) { */
		key = getch();
		/*      } else {
		   if (updown < 0) {
		   updown++;
		   key = 258;
		   } else {
		   updown--;
		   key = 259;
		   }
		   } */

		mvprintz(2, 69, "%0%Y");
		printf("%03d", key);
		refresh();

		switch (key) {

		case 'q':
		case 'Q':
		case 27:
			status("Loading cancelled");
			mvprintz(24, 2, "%KÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
			mvprintz(7, 7, "%0%GEditor");
			i = (cnY * 16) + cnX;
			enlarge(i);
			return;
			break;
		case 10:
			sprintf(bl, "%s%s", FONTDIR, files[liner + countah]);
			if ((fpo = fopen(bl, "r")) == NULL)
				error("cannot read filename");

			for (i = 0; i < 256; i++)
				fread(&buf[i * 32], fs, 1, fpo);

			fclose(fpo);
			mvprintz(5, 3, "%W");
			sprintf(bl, "%s", files[liner + countah]);
			bl[strlen(bl) - 4] = 0;
			printf("%s", bl);
			if ((strlen(files[liner + countah])) < 20)
				for (i = 0; i <= 21 - strlen(files[liner + countah]); i++)
					printf(" ");
			setfont();
			getfont();
			mvprintz(24, 2, "%KÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
			mvprintz(7, 7, "%0%GEditor");
			i = (cnY * 16) + cnX;
			enlarge(i);
			status("Font is loaded");
			return;
			break;
		case 258:
			oliner = liner;
			ocnt = countah;
			liner++;
			if (liner > 15) {
				liner = 15;
				countah++;
				if ((countah + liner) >= cnt) {
					countah--;
				}
			}
			break;
		case 259:
			oliner = liner;
			ocnt = countah;
			liner--;
			if (liner < 0) {
				liner = 0;
				countah--;
				if (countah < 0)
					countah = 0;
			}
			break;
		case 339:
/* the idea here was to only redraw it as much as necessary.  At least on
   my computer, it's very slow to go down 15 times, redrawing the whole panel
   for up to 15 of those times. Also, I wanted it so that you wouldn't miss
   any font file names by moving down 15 when you're already halfway down the
   panel.  Also the liner will move to the first item if we're positioned at
   the top page. */
			ocnt = countah;
			oliner = liner;
			countah -= 15;
			if (countah < 0) {
				countah = 0;
				if (ocnt == 0) {
					liner = 0;
					break;
				}
			}
			ocnt = countah;
			printz("%w");
			for (i = countah; i <= (15 + countah); i++) {
				move_to(8 + (i - countah), 3);
				printz("              ");
				move_to(8 + (i - countah), 3);
/* the .14 prints only 14 characters of the file, aleviating a previous
   bug */
				printf("%.14s", files[i]);
			}
/* did I do these codes right? */
			mvprintz(8 + liner, 3, "%W%4");
/* this simultaneously truncates at 14 characters and pads to 14
   characters. */
			printf("%-14.14s", files[countah + liner]);
			break;
		case 338:
			oliner = liner;
			countah += 15;
			if (countah >= (cnt - 15)) {
				if (ocnt == (cnt - 15)) {
					liner = cnt - ocnt - 1;
/* the - 1 because otherwise it'd be an off-by-one-problem */
					countah = ocnt;
					break;
				}
				countah = cnt - 16;
			}
			ocnt = countah;
			printz("%w");
			for (i = countah; i <= (15 + countah); i++) {
				move_to(8 + (i - countah), 3);
				printz("              ");
				move_to(8 + (i - countah), 3);
				printf("%.14s", files[i]);
			}
			mvprintz(8 + liner, 3, "%W%4");
			printf("%-14.14s", files[countah + liner]);
			break;
		case 262:
			updown = countah + liner;
			break;
		case 360:
			updown = -(cnt - (countah + liner));
			break;
		}

		printz("%w");
		if ((ocnt != countah) || (oliner != liner)) {
			if (ocnt != countah)
				for (i = (0 + countah); i <= (15 + countah); i++) {
					move_to(8 + (i - countah), 3);
					printf("              ");
					move_to(8 + (i - countah), 3);
					printf("%.14s", files[i]);
				}
			mvprintz(8 + oliner, 3, "%w%0");
			printf("%-14.14s", files[oliner + countah]);
			mvprintz(8 + liner, 3, "%W%4");
			printf("%-14.14s", files[liner + countah]);
		}
		if ((cnt - 16) > countah) {
			mvprintz(24, 15, "%w");
		} else {
			mvprintz(24, 15, "%0%KÄ");
		}
		if (countah > 0) {
			mvprintz(24, 14, "%w");
		} else {
			mvprintz(24, 14, "%0%KÄ");
		}

		mvprintz(24, 3, "%w");
		printf("%03d", countah + liner + 1);
		refresh();
	}
}

void save()
{
	FILE *fpo;
	int key, d = 0;
	char bl[50], sbl[75];

	status("Saving the font");
	bzero(bl, sizeof(bl));
	bzero(sbl, sizeof(sbl));
	mvprintz(5, 3, "%W                      ");
	move_to(5, 3);
	curs_set(TRUE);
	refresh();
	while (key != 10) {
		key = getch();
		if (key != 263) {
			if (d != 22) {
				printf("%c", key);
				bl[d] = key;
				d++;
			}
		} else if (d > 0) {
			printf("\b \b");
			bl[d] = 0;
			d--;
		}
		refresh();
	}
	curs_set(FALSE);
	if (strlen(bl) == 22) {
		bl[d] = 0;
	} else {
		bl[d - 1] = 0;
	}
	strcat(bl, ".fnt");

	if (strlen(bl) == 4) {
		bzero(bl, sizeof(bl));
		strcat(bl, "yourfont.fnt");
		mvprintz(5, 3, "%Wyourfont");
	}
	sprintf(sbl, "%s%s", FONTDIR, bl);
	if ((fpo = fopen(sbl, "w")) == NULL)
		error("cannot create filename");

	for (i = 0; i < 256; i++)
		fwrite(&buf[i * 32], fs, 1, fpo);

	fclose(fpo);
	status("Font was saved!");
}


void savepcf()
{
	FILE *output;
	unsigned long i, p = 16;
	unsigned int tmp;
	unsigned char tmp2;

	output = fopen("vga.pcf", "w");
	fwrite(bdata, 2820, 1, output);

	for (i = 0; i < 4064; i++) {
		tmp = font[p++];
		if (i != 0) {
			putc(0, output);
			putc(0, output);
			putc(0, output);
		}
		tmp2 = ((tmp & (1 << 0)) << 7) | ((tmp & (1 << 1)) << 5) | ((tmp & (1 << 2)) << 3) | ((tmp & (1 << 3)) << 1) \
		    |((tmp & (1 << 7)) >> 7) | ((tmp & (1 << 6)) >> 5) | ((tmp & (1 << 5)) >> 3) | ((tmp & (1 << 4)) >> 1);
		putc(tmp2, output);
	}
	fwrite(edata, 5559, 1, output);
	fclose(output);
}
