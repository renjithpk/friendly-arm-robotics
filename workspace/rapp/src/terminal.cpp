/*
 * Terminal.h
 *
 *  Created on: 10-Jan-2015
 *      Author: renjith
 */

#include <syslog.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <stdio.h>
#include "terminal.h"

static struct termios old_termios, new_termios;
/* Initialize new terminal i/o settings */
void Terminal::initTermios(int echo) 
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	tcgetattr(0, &old_termios); /* grab old terminal i/o settings */
	new_termios = old_termios; /* make new settings same as old settings */
	new_termios.c_lflag &= ~ICANON; /* disable buffered i/o */
	new_termios.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new_termios); /* use these new terminal i/o settings now */
	isNewSettings = true;
}

/* Restore old_termios terminal i/o settings */
void Terminal::resetTermios(void) 
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	tcsetattr(0, TCSANOW, &old_termios);
}
Terminal::Terminal()
{
	isNewSettings = false;
}
/* Read 1 character - echo defines echo mode */
char Terminal::getch_(int echo) 
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char Terminal::getch(void) 
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	return getch_(0);
}

/* Read 1 character with echo */
char Terminal::getche(void) 
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	return getch_(1);
}
Terminal::~Terminal()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(isNewSettings)
	{
		syslog(LOG_INFO,"reset to old settings");
		resetTermios();	
	}
}
