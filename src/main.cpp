// main.cpp

#include "mwMain.h"
#include <csignal>

void signal_handler(int signal)
{
   mMain.fast_exit(signal);
}

int main(int argument_count, char **argument_array)
{
   signal(SIGINT,  signal_handler);
   signal(SIGABRT, signal_handler);
   signal(SIGTERM, signal_handler);
   mMain.pm_main(argument_count, argument_array);
   return 1;
}
