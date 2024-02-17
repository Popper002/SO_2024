#include "library.h"
    int id_shm; 
    key_t key;
static void fetch_argv(const * argv[])
{ 
    id_shm = atoi(argv[1]);
    key = atoi(argv[2]); 
    argv[3]= NULL; 
}

int main(int argc, char const *argv[])
{
    printf("{%s} [%s] {PID %d}\n",__func__, __FILE__,getpid()); 
    fetch_argv(argv); 
    printf(" SHID %d  , KEY %d\n",id_shm , key );
    return 0;
}
