#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>



pid_t pid1=1;
pid_t pid2=1;
int cpt=0;

void handCtrlZ(int sig);
void fils11(int sig);
void fils12(int sig);
void fils2(int sig);
void handCtrlZ2(int sig);
void handCtrlC(int sig);
void Ex(int sig);

int flag=0;
int cptC=0;



int main(){

   signal(SIGTSTP,SIG_IGN); // pour que les processus F1 et F2 ignorent le 'CTRL+Z'
   signal(SIGINT, SIG_IGN);// pour que les processus F1 et F2 ignorent le 'CTRL+C'
   signal(SIGUSR1, fils11);

   pid1=fork();

   if(pid1==0){
      signal(SIGCONT, Ex);
      signal(SIGUSR2,fils12);
      signal(SIGUSR1, fils11);
      while(1);
   }

   pid2=fork();

   if(pid2 ==0){
      signal(SIGUSR1, Ex);
      signal(SIGUSR2,fils2);
      while (1);
   }
   signal(SIGINT,handCtrlC);
   signal(SIGTSTP, handCtrlZ);  
   while(1);

   return 0;
   }
   
   
   
   
   
void handCtrlZ(int sig){
   
   signal(SIGALRM, handCtrlZ);

   if(flag==0){
      if(cpt % 3==0){         
         signal(SIGTSTP, handCtrlZ2);
         kill(pid1, SIGUSR1);
         cpt++;
         alarm(60); // Pour le test on execute alarm(5) pour reduire le temps d'attente au lieu d'une minute c'est 5 secondes
      }
      else{
         if(cpt%3==1){            
            signal(SIGTSTP, handCtrlZ2);
            kill(pid1,SIGUSR2);
            cpt++;
            alarm(120);// Pour le test on execute alarm(10) pour reduire le temps d'attente au lieu de deux minutes c'est 10 secondes
         }
         else{            
            signal(SIGTSTP, handCtrlZ2);
            kill(pid2, SIGUSR2);
            cpt++;
            alarm(60); // Pour le test on execute alarm(5) pour reduire le temps d'attente au lieu d'une minute c'est 5 secondes
         }
      }
   }
}
   
   
void fils11(int sig){
   printf("\nLe processus_1=%d : mesure 1'\n",getpid());
}   

void fils12(int sig){
   printf("Le processus_1=%d : mesure 2'\n",getpid());
}   

void fils2(int sig){
   printf("Le processus_2=%d : mesure'\n",getpid());
}   

void handCtrlZ2(int sig){

   flag=1;   

   signal(SIGTSTP,handCtrlZ);
   alarm(0);

   flag=0;
   cpt=0;
}


void handCtrlC(int sig){
   cptC++;

   if (cptC==3){
      signal(SIGINT, Ex);    // On peut envoyer aussi un SIGKILL au processus P, F1, F2 pour arreter l'execution sans
      kill(pid1, SIGCONT);   // c-à-d on execute kill(pid1, SIGKILL); kill(pid2, SIGKILL);  kill(getpid(), SIGKILL);
      kill(pid2, SIGUSR1);
      kill(getpid(), SIGINT);
   }
}

void Ex(int sig){
   exit(0);
}


   
   
