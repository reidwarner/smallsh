#ifndef SIGNALS_H
#define SIGNALS_H

void setup_signal_handlers();
void catchSIGTSTP(int signo);
extern int fgMode;
extern int sigSTP;

#endif
