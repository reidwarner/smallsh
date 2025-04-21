#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "signals.h"

int fgMode = 0;
int sigSTP = 0;

void catchSIGTSTP(int signo) {
    if (fgMode == 0) {
        char* msg = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, msg, 51);
        fgMode = 1;
    } else {
        char* msg = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, 31);
        fgMode = 0;
    }
    sigSTP = 1;
}

void setup_signal_handlers() {
    struct sigaction SIGTSTP_action = {0}, ignore_action = {0};
    ignore_action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ignore_action, NULL);

    SIGTSTP_action.sa_handler = catchSIGTSTP;
    sigfillset(&SIGTSTP_action.sa_mask);
    SIGTSTP_action.sa_flags = 0;
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}
