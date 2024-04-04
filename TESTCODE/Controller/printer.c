#include "library.h"


int main(int argc, char const *argv[])
{
    while (1)
    {
       fprintf(stdout,"PRINTER PID %d\n",getpid());
       sleep(1);
    }
    
    return 0;
}
