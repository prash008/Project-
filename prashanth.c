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

static int flag,f=0;
static int i,index1=0,a,sockfd,newsockfd;
static char *status[250];
static char *ip[250],*buf1[ipa+1],buffer[BUF_SIZE+1],*b[BUF_SIZE+1]; 
pthread_t udp1,thread_reader1,thread_writer1,rThread;

void * receiveMessage(void * socket) 
{
 int sockfd, ret, ret1=0;
 char buffer[BUF_SIZE+1]; 
 sockfd = (int)socket;
 memset(buffer, 0, BUF_SIZE);  
 
  while(1) 
  {
    ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
     if (ret < 0) 
      {  
       printf("Error receiving data!\n");    
      } 
      else
                 strcpy(b,buffer);
                
              if(strncmp(b,"stop",4)==0)
                   {
                     f=1;
                     printf("%s",buffer);
                     memset(buffer, 0, BUF_SIZE);
                     memset(b, 0, BUF_SIZE);
                     printf("press enter key to continue");
                     pthread_exit(NULL);  
                    }
             
              
             
            printf("%s",buffer);
            memset(buffer, 0, BUF_SIZE);
            memset(b, 0, BUF_SIZE);      
   }
}

void *sender1(void *ptr)
{
 struct sockaddr_in addr, cl_addr;  
 int  ret,c,ret1;  
 char buffer[BUF_SIZE]; 
 char * serverAddr;
 //pthread_t rThread;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd < 0) 
    {  
     printf("Error creating socket!\n");  
     exit(1);  
    }  
       printf("Socket created...\n");   
       memset(&addr, 0, sizeof(addr));  
       addr.sin_family = AF_INET;  
       addr.sin_addr.s_addr = inet_addr(buf1[ipa]);
       addr.sin_port = PORT;     

    ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
      if (ret < 0) 
       {  
         printf("Error connecting to the server!\n");  
         exit(1);  
       }  
     
     printf("Connected to the server...\n");  
    
     memset(buffer, 0, BUF_SIZE);
      printf("Enter your messages one by one and press return key!\n");
  
     ret1 = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);
     if (ret1) 
     {
       printf("ERROR: Return Code from pthread_create() is %d\n", ret);
       exit(1);
     }

     while (1) 
     {  
                         flag=1;      
                               if(f==1)
                                {
                                   f=0;
                                   flag=0;
                                   printf("%dk",f);
                                   close(sockfd);
                                   pthread_exit(NULL);
                                }
                  fgets(buffer, BUF_SIZE, stdin); 
                  
                  ret1 = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
        
                        if (ret < 0) 
                          {  
                            printf("Error sending data!\n\t-%s", buffer); 
                            exit(1); 
                           }
             
                         if((ret1=strncmp(buffer,"stop",4))==0)
                             {
                              resend1();
                             }
        
     }

 close(sockfd);
 pthread_exit(NULL);
 return 0;
}




int getbusystatus()
  {
    return flag;
  }
 
char list()
{
int index;
char *k,*j;
int x;
for(index=1;index<=a;index++)
    {         
             
             k=getUserIP(index);
             j=getUserID(index);
             x=getuserstatus(index);
             printf("\n\n%d.%s\n",index,j);
             printf("  %s\n",k);
             sleep(1);
             printf("  %d\n",x);
             ip[index]=k;
             status[index]=x;
   }
}

      int list1()
	{
         pthread_t udp1,thread_reader1,thread_writer1;
         int iret1,iret2,iret3,iret4;
 	 char *message3="thread_reader1";
 	 char *message4="thread_writer1";
         int l,index1,b,c=1;
         f:
         printf("enter the 1 to display list");
           scanf("%d",&l);
           if(l==1) 
            {
              printf("the number of users are=");
              a=getUserListCount();
              printf("%d\n\n",a);
              list();
             }
         j:
          printf("enter 1 to list again or 0 to not");
          scanf("%d",&b);
           if(b==1)
           {
             printf("the number of users are=");
             a=getUserListCount();
             printf("%d\n\n",a);
             list();
            goto j;
           } 
   
             e:
             printf("enter the uerid no ");
             scanf("%d",&index1);
             if(status[index1]==1)
              {
                printf("can not communicate slected user is busy");
                goto e;
              }
             buf1[ipa]=ip[index1];
             printf("%s",buf1[ipa]); 
             iret4=pthread_create(&thread_writer1,NULL,sender1,(void *)message3);           
             pthread_join(thread_writer1,NULL); 
             printf("%d",flag);     
              goto f;            
        }

int resend1()
    {
       flag=0;
       close(sockfd);
       pthread_cancel(rThread);
       pthread_exit(NULL);
    }

  
