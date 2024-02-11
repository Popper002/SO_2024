//piû un estratto della lezione che altro ,farô altri file con i casi d'uso specifici ai nostri fini 

#include "library.h"
#define SEM_KEY 1 //for practice 
struct sembuf my_op;




int main(int argc, char const *argv[])
{
    
    int sem_id = semget(SEM_KEY , 1 , IPC_CREAT); /*Creating semaphore */ //unico oggetto IPC per n semafori 
    if(sem_id<=-1){perror("ERROR TO CREATE SEM \n");exit(EXIT_FAILURE);}
    printf("SEM ID %d ,<STATUS : CREATED> \n",sem_id); 
    semctl(sem_id, 0, SETVAL , 1);

    my_op.sem_num = 0; /* only one semaphore in array of semaphores */
    my_op.sem_flg = IPC_NOWAIT; 
    my_op.sem_op = -1; 
    semop(sem_id , &my_op, 1); /* blocking if others hold resource */
    int print_flag =1 ;
    //se il semaforo vale  0 significa che qualcuno ha già preso la risorsa quindi è in stato di blocco ,finchè la risorsa non è di nuovo disponibile 
    //quando finisce incrementa il semaforo (risorsa :sezione critica : porzione di codice che puô stare solo un processo alla volta)
    // si puô anche bloccare se c'è una risorsa ,al contratrio puô anche funzionare ,mi blocco finché c'è la risorsa.
    //sincronizzazione a barriera ovvero tanti processi partono insieme su un singolo evento ,mettendo il semaforo a 1 e bloccando i processi in attesa dello zero.

    //array di semafori , puoi fare op su tanti array 
    //le operazioni si svolgono su un singolo semaforo con una struct 
    //se voglio fare delle operazioni sui semafori che nell'array di semafori hanno posizione 0  e 3 io devo creare due array di operazioni /due struct per fare le operazioni 
    //dopo avere compilato la struct con le operazioni che vogliamo fare usiamo la funzione semop ,che ha il compito di procedere all'esecuzione di quelle operazioni  ed è bloccante 
    //wait any event. 
    //quando consumo il campo myop.semop ,deve essere negativo ,e quando la rendo disponibile ripristino il contatore aggiungendo la risorsa 
    //semnum è l'indice su cui vogliamo operare negli array di operazioni 
    //all'inziio il semaforo non ha un valore quindi devo impostarlo tramite semctl(id_sem , 0 , SETVAL , 1 ) //imposto a 1 il contatore per la risorsa



    //caso utile a fine progetto : Master genera e gli altri devono aspettare --> Ai processi in wait di un processo B 

    //inizializzo il semaforo a 1
    //blocco i processi Ai finchè il semaforo non è a 0 --> infatti quando il semaforo è a zero non metto ne tolgo risorsa ma aspetto che sia a zero , mi blocco finchè non è zero 
    //si bloccheranno i Ai processi perchè il semaforo è inizializzato a 1 ,B quando fa -1 e quindi il semaforo vale 0 e quindi partono i processi 



    //caso Aspetto ma non per sempre 

    /*struct semop myop 
    
    myop.sem_flg = IPC_NOWAIT; //anche se la risorsa non è presente io vado avanti ma mi lancerà un errore EAGAIN su errno ,e ritorna -1 
    semop(...,&myop , 1 ) non aspetto 
    */

   //posso anche bloccarmi per un tempo ben preciso usando la struct di timespec 
   /*   struct timespec 
            {   
                    time_t tv_sec //secondi 
                    long tv_nsec //nanosecondi 
            } 
            semtimeop(/* come semop  , struct timespec *timeout)*/ 

    /* COSE DA NON FARE 
        sop.sem_flg = IPC_NOWAIT ; 
        do 
        { 
            semop(id , &sop , 1 ) //cicli di semop inutili perchè un altro processo potrebbe sbloccare la situazione e invece ciclo inutilmente 
    
        while(errno == EAGAIN); 
        } 


        
        */

    semctl(sem_id,IPC_RMID,NULL);
    return 0;
}
