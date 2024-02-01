#include "header/common.h"
#include "header/master.h"

struct config config; 

int scan_data(FILE*fp)
{
    int value; 
    char name_param[500];
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
        else if(strcmp(name_param,"N_ATOM_MAX") == 0)
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

static void print_para_TEST(struct config config )
{ 
printf("N_ATOMI_INIT: %d\n"
 "ENERGY_DEMAND :%d\n"
 "N_ATOM_MAX:%d\n"
 "MIN_A_ATOMICO :%d\n"
 "N_NUOVI_ATOMI :%d\n"
 "SIM_DURATION :%d\n"
"ENERGY_EXPLODE_THRESHOLD :%d\n",config.N_ATOMI_INIT,config.ENERGY_DEMAND , config.N_ATOM_MAX,config.MIN_A_ATOMICO
 , config.N_NUOVI_ATOMI,config.SIM_DURATION,config.ENERGY_EXPLODE_THRESHOLD);
}

int main(int argc, char const *argv[])
{
    FILE *fp; 
    fp= fopen("/Users/popper/Documents/Uni/secondo anno /SO_2024/SO_2024/src/config/config1.txt","r"); 
    if( fp ==NULL ){ fprintf(stderr,"%d\n");exit(EXIT_FAILURE);}
    printf("MAIN %d\n",getpid()); 
    scan_data(fp); 
    print_para_TEST(config);
    exit(EXIT_SUCCESS);
    return 0;
}
