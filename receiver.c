#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "list.h"

#define CLADDR_LEN 100
#define ipa 16
#define user 50
#define BUF_SIZE 200
#define MAXBUF 200
#define BUFLEN 200
#define buff 200
#define PORT 44444

static int flag,index1,f=0,q=0;
static int i,index1=0,a,sockfd,newsockfd;
static int *status[250],*away[250];
static char *ip[250],*buf1[ipa+1],buffer[BUF_SIZE+1],*b[BUF_SIZE+1]; 
pthread_t thread_reader1,rThread;

void * receiveMessage(void * socket) 
{
  struct sockaddr_in addr, cl_addr; 
 int sockfd, ret, ret1=0;
 char buffer[BUF_SIZE+1]; 
 sockfd = (int)socket;
 memset(buffer, 0, BUF_SIZE);  
 

         while(1) 
        
               {                      
                            
                           
                          fgets(buffer, BUF_SIZE, stdin); 
                            
                            ret= sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
      
                               if((ret1=strncmp(buffer,"stop",4))==0)
                                 {
                                   resend1();
                                 }


                
                }
}
 


void *receiver1(void *ptr)
{
 
struct sockaddr_in addr, cl_addr;
 int len, ret,status,c;
 int yes=1,iret3;
 char buffer[BUF_SIZE];
 char clientAddr[CLADDR_LEN];
 
 
          sockfd = socket(AF_INET, SOCK_STREAM, 0);
           
              if (sockfd < 0)
                {
                  printf("Error creating socket!\n");
                  exit(1);
                }
         
          printf("Socket created...\n");
          memset(&addr, 0, sizeof(addr));
         
          addr.sin_family = AF_INET;
          addr.sin_addr.s_addr = INADDR_ANY;
          addr.sin_port = PORT;
        
         status = setsockopt(sockfd, SOL_SOCKET,SO_REUSEPORT, &yes, sizeof(int) );
         ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
         status = setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int) ); 
           
             if (ret < 0) 
               {
                 printf("Error binding!\n");
                 exit(0);
               }
         printf("Binding done...\n");
         printf("Waiting for a connection...\n");
         
         listen(sockfd, 5);

         len = sizeof(cl_addr);
        
         newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
     
             if (newsockfd < 0) 
               {
                printf("Error accepting connection!\n");
                exit(1);
               } 

        inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
        printf("Connection accepted from %s...\n", clientAddr); 
        
        memset(buffer, 0, BUF_SIZE);
        printf("Enter your messages one by one and press return key!\n");
 
        ret = pthread_create(&rThread, NULL, receiveMessage, (void *) newsockfd);
             
             if (ret) 
               {
                 printf("ERROR: Return Code from pthread_create() kkk is %d\n", ret);
                  exit(1);
              }

                 

                while(1) 
                 {    
                   flag=1; 
                                  
                                  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL); 
 
                                   strcpy(b,buffer);
                                      
                                      if(strncmp(b,"stop",4)==0)
                                        {
                                         f=1;
                                         printf("%s",buffer);
                                        // printf("press enter key");
                                         memset(buffer, 0, BUF_SIZE);
                                         memset(b, 0, BUF_SIZE); 
                                         } 
                                   
                                 printf("%s",buffer);
                                 memset(buffer, 0, BUF_SIZE);
                                 memset(b, 0, BUF_SIZE);
                              
                                  if((f==1))
                                       
                                     {
                                       f=0;
                                       flag=0;
                                       printf("%dk",f);
                                       close(sockfd);
                                       pthread_cancel(rThread);
                                       //pthread_cancel(away1);
                                       pthread_exit(NULL);
                                     } 
                  }   
                
      close(newsockfd);
        close(sockfd);

      pthread_exit(NULL);
     
} 


int getbusystatus()
  {
    return flag;
  }
 
char list()
{
int index;
char *k,*j;
int x,y,z;
for(index=1;index<=a;index++)
    {         
             
             k=getUserIP(index);
             j=getUserID(index);
             x=getuserstatus(index);
             y=awaystatus(index);
             ip[index]=k;
             status[index]=x;
             away[index]=y;
             printf("\n\n%d.%s\n",index,j);
             printf("  %s\n",k);
               
                  if(status[index]==1)
                        {
                         printf("  Busy \a \n");
                        } 
                     
                      else
                        {
                         printf("  Free \a \n");
                        }
                   
                   if(away[index]==1)
                        {
                         printf("  Away \a \n");
                        }
 
                       else
                        {
                         printf("  Free \a \n");
                        }
             
   }
}

  int list1()
	{
         pthread_t thread_reader1;
         int iret1,iret2,iret3,iret4;
         char *message2="udp1";
 	 char *message3="thread_reader1";
 	 char *message4="thread_writer1";
         int l,b,c=1;
        
         f:
         printf("enter the 1 to list the users\n");
         printf("enter 2 to exit\n");
         scanf("%d",&l);
          
           if(l==1) 
            {
              printf("the number of users are=");
              a=getUserListCount();
              printf("%d\n\n",a);
              list();
             }
          
           if(l==2)
             {
              exit(0);
             }
         
          j:
          printf("\nenter 1 to list \n");
          printf("enter 0 to chat \n");
          printf("enter 2 to exit \n");
          scanf("%d",&b);
           
           if(b==1)
            {
             printf("the number of users are=");
             a=getUserListCount();
             printf("%d\n\n",a);
             list();
             goto j;
            } 
             
           if(b==2)
             {
              exit(0);
             } 
           
           if(b==0)
             {
              iret4=pthread_create(&thread_reader1,NULL,receiver1,(void *)message3); 
              pthread_join(thread_reader1,NULL);        
             } 
           
           printf("wrong option select from the list again \n");
            
           goto f;            
        }

int resend1()
    {
       flag=0;
       close(sockfd);
       pthread_cancel(rThread);
       pthread_exit(NULL);
    }

