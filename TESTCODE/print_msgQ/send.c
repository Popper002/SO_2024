#include "library.h"

/**/
int ret_integer()
{
    return rand()%100; 

}
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
float floatfrom(float min, float max) 
{
    float range = (max - min); 
    float div = RAND_MAX / range;
    return min + (rand() / div);
}
int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int snd_id;
    int timer =10;
    pid_t snd_pid; 
    snd_pid = getpid();
    if(snd_pid <0)
    {
        fprintf(stderr,"PID ERROR <ERRNO:%s>\n",strerror(errno));
        exit(EXIT_FAILURE);
    } 
    snd_id = msgget(KEY_ , IPC_CREAT|0666); 
    if(snd_id <0)
    {
        fprintf(stderr,"ERROR IN MSGGET <ERRNO:%s>\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    MyData data;
    while(1)
    {
    data.type =0; 
    int ret;
    double d;
    float f;
    ret = ret_integer();
    sprintf(data.array ,"%d",ret);
    msgsnd(snd_id ,&data , sizeof(data)-sizeof(long) ,0);
    fprintf(stdout,"SEND THIS %s \n",data.array);
    data.type=1;
    d = randfrom(0,100);
    sprintf(data.array ,"%f",d);
    msgsnd(snd_id ,&data , sizeof(data)-sizeof(long) ,0);
    fprintf(stdout,"SEND THIS %s \n",data.array);
    data.type=2;
    f = floatfrom(0,100);
    sprintf(data.array ,"%f",f);
    msgsnd(snd_id ,&data , sizeof(data)-sizeof(long) ,0);
    fprintf(stdout,"SEND THIS %s \n",data.array);

   
   }
  // fprintf(stdout,"FINISH SENDING\n");

    return 0;
}
