#include <mule.h>
#include <cstdlib>
#include <iostream>

void main() {
	MuleApplication mApp;
	int ntimebeep = rand() % 10;
	muleprintf("Beeping " + muleinttostr(ntimebeep) + " times");
	for (int i = 0; i < ntimebeep; i++)
		MuleSoundPlayer::beep();
	muleprintf("Goodbye");
	mApp.exit(0);
}
