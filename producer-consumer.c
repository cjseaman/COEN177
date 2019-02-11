/* Collin Seaman
 * COEN 177L
 * 1/23/2018
 * Lab 3
 * Part 5: producer and consumer implementation. Two producers pass "Producer 1" and "producer 2" 10 times
 * to the consumer process 
 * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/wait.h> 
//main
int main(int argc,char *argv[]){
   int  fds[2];
   char buff[60];
   int count;
   int i;
   pipe(fds);   
   if (fork()==0) {
//      printf("START Producer 1\n");
       close(fds[0]);
       for(i=0;i<10;i++){
           write(fds[1],"Producer 1\n",11);
       }
       exit(0);
   }
   else if (fork()==0) {
//       printf("START Producer 2\n");
       close(fds[0]);
       for(i=0;i<10;i++){
           write(fds[1],"Producer 2\n",11);
       }
   } 
   else if(fork()==0) {
//       printf("START Consumer\n");
       close(fds[1]);
       while((count=read(fds[0],buff,60))>0){
           for(i=0;i<count;i++){
               write(1,buff+i,1);
           }
       }
       exit(0);
    }
   else{     
      close(fds[0]);
      close(fds[1]);
      wait(0);
      wait(0);
   }
return 0;
}
