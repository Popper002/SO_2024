#include "header/common.h"
#include "header/master.h"

struct config config; 

static int scan_data(FILE*fp)
{
    int value; 
    char name_param[100];
    int error =1; 
    printf("Reading data from file...\n");
    while(fscanf(fp,"%s %d",name_param , &value) != EOF)
    {
        if(strcmp(name_param,"N_ATOMI_INIT") == 0)
        {
            config.N_ATOMI_INIT = value;
            error = 0; 
        }
        else if(strcmp(name_param,"ENERGY_DEMAND") == 0)
        {
            config.ENERGY_DEMAND = value;
            error = 0; 
        }
        else if(strcmp(name_param,"N_ATOMI_MAX") == 0)
        {
            config.N_ATOM_MAX = value;
            error = 0; 
        }
        else if(strcmp(name_param,"MIN_A_ATOMICO") == 0)
        {
            config.MIN_A_ATOMICO = value;
            error = 0; 
        }
        else if(strcmp(name_param,"N_NUOVI_ATOMI") == 0)
        {
            config.N_NUOVI_ATOMI = value;
            error = 0; 
        }
        else if(strcmp(name_param,"SIM_DURATION") == 0)
        {
            config.SIM_DURATION = value;
            error = 0; 
        }
        else if(strcmp(name_param,"ENERGY_EXPLODE_THRESHOLD") == 0)
        {
            config.ENERGY_EXPLODE_THRESHOLD = value;
            error = 0; 
        }
        else
        {
            error = 1; 
        }
    }
    printf("Data read from file!\n");
    return error;
}



int main(int argc, char const *argv[])
{
    FILE *fp; 
    fp= fopen("config/confi1.txt",'r'); 
    if(fp <=-1 )
    { 
        printf("%d %s ",__LINE__,__func__); /* is a preprocessor macro that expands to
         current line number in the source file, as an integer*/
    }
    return 0;
}
