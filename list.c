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
#define PORT 4444

int i,index1=0,a;
char *id[100];
char *ip[100],*buf1[ipa+1]; 

void * receiveMessage(void * socket) 
{
 int sockfd, ret;
 char buffer[BUF_SIZE]; 
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
         fputs(buffer, stdout);
              if((strncmp(buffer,"exit",4))==0)
                           {
                            printf("Client Exit...\n");
                              close(sockfd);
                               list1();
                            } 
   }
}

void *sender1(void *ptr)
{
 struct sockaddr_in addr, cl_addr;  
 int sockfd, ret;  
 char buffer[BUF_SIZE]; 
 char * serverAddr;
 pthread_t rThread;

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
  
     ret = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);
     if (ret) 
     {
       printf("ERROR: Return Code from pthread_create() is %d\n", ret);
       exit(1);
     }

     while (fgets(buffer, BUF_SIZE, stdin) != NULL) 
     {
                      
       ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
         if (ret < 0) 
          {  
            printf("Error sending data!\n\t-%s", buffer);  
          }
         if((strncmp(buffer,"exit",4))==0)
                           {
                               list1();
                                //break;
                            }
             
     }

 close(sockfd);
 pthread_exit(NULL);
 return 0;
}


 void *receiver1(void *ptr)
{
 struct sockaddr_in addr, cl_addr;
 int sockfd, len, ret, newsockfd,status,yes=1;
 char buffer[BUF_SIZE];
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 pthread_t rThread;
 
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
         ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
         status = setsockopt(sockfd, SOL_SOCKET,SO_REUSEPORT, &yes, sizeof(int) );
           if (ret < 0) 
             {
             printf("Error binding!\n");
             exit(1);
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
             printf("ERROR: Return Code from pthread_create() is %d\n", ret);
             exit(1);
             }

                while (fgets(buffer, BUF_SIZE, stdin) != NULL) 
                 {
                       
                   ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);  
                      if (ret < 0) 
                   {  
                     printf("Error sending data!\n");  
                     exit(1);
                   }
                  }   

      close(newsockfd);
        close(sockfd);

      pthread_exit(NULL);
     
} 
 
char list()
{
int index;
char *k,*j;

 for(index=1;index<=a;index++)
    {
             k=getUserIP(index);
             j=getUserID(index);
             printf("\n\n%d.%s\n",index,j);
             printf("  %s\n",k);
             ip[index]=k;
          
   }
}

      int list1()
	{
         pthread_t udp1,thread_reader1,thread_writer1;
         int iret1,iret2,iret3,iret4;
 	 char *message3="thread_reader1";
 	 char *message4="thread_writer2";
         int l,index1;

         printf("enter the 1 to display list");
           scanf("%d",&l);
           if(l==1) 
              printf("the number of users are=");
              a=getUserListCount();
              printf("%d\n\n",a);
              list();
             
             
          printf("enter 1 to send or 0 to receive");
          scanf("%d",&a);
         
          if(a==0)
           {
           iret3=pthread_create(&thread_reader1,NULL,receiver1,(void *)message4);
           pthread_join(thread_reader1,NULL);
           }

         printf("enter the uerid no ");
         scanf("%d",&index1);
         buf1[ipa]=ip[index1];
         printf("%s",buf1[ipa]);
         
         iret4=pthread_create(&thread_writer1,NULL,sender1,(void *)message3);
 	 pthread_join(thread_writer1,NULL);
       }
