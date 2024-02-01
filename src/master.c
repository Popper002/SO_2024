#include "header/common.h"
#include "header/master.h"
char const*args_atom[100];
char const *args_[100];


static int scan_data(FILE*fp)
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
static void argument_creator(char * argv[]) {
    char n_atomi_init[10];
    char energy_demand[10];
    char n_atom_max[10];
    char min_a_atomico[10];
    char n_nuovi_atomi[10];
    char sim_duration[10];
    char energy_explode_threshold[10];
    
    sprintf(n_atomi_init,"%d",config.N_ATOMI_INIT);
    sprintf(energy_demand,"%d",config.ENERGY_DEMAND);
    sprintf(n_atom_max,"%d",config.N_ATOM_MAX);
    sprintf(min_a_atomico,"%d",config.MIN_A_ATOMICO);
    sprintf(n_nuovi_atomi,"%d",config.N_NUOVI_ATOMI);
    sprintf(sim_duration,"%d",config.SIM_DURATION);
    sprintf(energy_explode_threshold,"%d",config.ENERGY_EXPLODE_THRESHOLD);
    
    
    argv[1] = strdup(n_atomi_init);
    argv[2] = strdup(energy_demand);
    argv[3] = strdup(n_atom_max);
    argv[4] = strdup(min_a_atomico);
    argv[5] = strdup(n_nuovi_atomi);
    argv[6] = strdup(sim_duration);
    argv[7] = strdup(energy_explode_threshold);
    argv[19] = NULL;

}
pid_t atom_generator( struct config config)
{ 
    pid_t atom_pid; 
    switch (atom_pid=fork() )
    {
    case -1 : 
            TEST_ERROR;
            exit(EXIT_FAILURE);
    case 0: 
        argument_creator(args_atom); 
        execvp(ATOM_PATH ,&args_atom);
        fprintf(__func__,__LINE__,getpid(),"%s LINE: %d[MASTER %d  , ATOM_GENERATOR(){PROBLEM IN EXECVP}\n"); 
        exit(EXIT_FAILURE);
        break;
    
    default:
        
        return atom_pid;
        break;
    }
}
pid_t activator_generator( struct config config)
{ 
    pid_t activator_pid; 
    switch (activator_pid =fork() )
    {
    case -1 : 
            TEST_ERROR;
            exit(EXIT_FAILURE);
    case 0: 
        argument_creator(args_atom); 
        execvp(ACTIVATOR_PATH ,&args_atom);
        fprintf(__func__,__LINE__,getpid(),"%s LINE: %d[MASTER %d  , ACTIVATOR_GENERATOR(){PROBLEM IN EXECVP}\n"); 
        exit(EXIT_FAILURE);
        break;
    
    default:
        sleep(1); 
        return activator_pid;
        break;
    }
}
pid_t fuel_generator( struct config config)
{ 
    pid_t fuel_pid; 
    switch (fuel_pid =fork())
    {
    case -1 : 
            TEST_ERROR;
            exit(EXIT_FAILURE);
    case 0: 
        argument_creator(args_atom); 
        execvp(FUEL_PATH ,&args_atom);
        fprintf(__func__,__LINE__,getpid(),"%s LINE: %d[MASTER %d  , FUEL_GENERATOR(){PROBLEM IN EXECVP}\n"); 
        exit(EXIT_FAILURE);
        break;
    
    default:
        sleep(1); 
        return fuel_pid;
        break;
    }
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
    pid_t atom;
    fp= fopen("/Users/popper/Documents/Uni/secondo anno /SO_2024/SO_2024/src/config/config1.txt","r"); 
    if( fp ==NULL ){ fprintf(stderr,"%d\n");exit(EXIT_FAILURE);}
    args_atom[0]=ATOM_PATH;
    printf("MAIN %d\n",getpid()); 
    scan_data(fp); 
    atom_generator(config); 
    close(fp);
    
    return 0;
}
