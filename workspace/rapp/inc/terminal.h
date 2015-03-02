/*
 * Terminal.h
 *
 *  Created on: 10-Jan-2015
 *      Author: renjith
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

class Terminal
{
    bool isNewSettings;
	void initTermios(int echo); 
	/* Restore old_termios terminal i/o settings */
public:
	Terminal();
	/* Read 1 character - echo defines echo mode */
	char getch_(int echo);

	/* Read 1 character without echo */
	char getch(void);

	/* Read 1 character with echo */
	char getche(void); 
	static void resetTermios(void);
	~Terminal();
};
#endif
