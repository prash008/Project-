#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#define CLADDR_LEN 100
#define ipa 16
#define user 50
#define BUF_SIZE 200
#define MAXBUF 200
#define BUFLEN 200
#define buff 200
#define PORT 4444

int i,index=0;
char *id[100];
char *ip[100],*buf1[ipa+1];

 struct ip_list
  {
   char usr[user];
   char ip[ipa]; 
   struct ip_list *next;
  };

  struct ip_list *head=NULL;
  struct ip_list *node;
  struct ip_list *newnode;


 void* sender(void *ptr)
  {
   int sockfd, status, buflen, sinlen,ret;
   char buffer[MAXBUF+1];
   struct sockaddr_in sock_in;
   int yes = 1,interval=10,seconds_to_sleep,duration,elapsed;
  
   sinlen = sizeof(struct sockaddr_in);
   memset(&sock_in, 0, sinlen);
   buflen = MAXBUF;
   pthread_t rThread;
   sockfd= socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   
   sock_in.sin_addr.s_addr=inet_addr("192.168.1.255"); 
   sock_in.sin_port = htons(44444); 
   sock_in.sin_family = AF_INET;
   status = bind(sockfd, (struct sockaddr *)&sock_in, sinlen);
   status = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
        

   memset(buffer, 0, BUF_SIZE);
 

    if(ret) 
     {
      printf("ERROR: Return Code from pthread_create() is %d\n", ret);
      exit(1);
     }

                
      for(i=0;i<=3;i++) 
       {
        
         sprintf(buffer, "umesh");
         ret = sendto(sockfd, buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);  
         if (ret < 0) 
          {  
            printf("Error sending data!\n\t-%s\n", buffer);  
          }
         sleep(1);
          }
    // close(sockfd);
   }
 

   void *receiver(void *ptr)
    {
     struct sockaddr_in si_me, si_other;
     int s, i, slen = sizeof(si_other) , recv_len,ret,k;
     char buf[BUFLEN+1];
    
     if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
       {
        printf("error in socket");
       }
     
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(44444);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
   
    if( bind(s,(struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
     {
      printf("error in creating bind");
     }
      
      for(i=0;i<=3;i++)
       {
        fflush(stdout);
       
         if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
          {
            printf("error in recvfrom()");
          }
         
          if(head==NULL)
            {
             head=(struct ip_list *)malloc(sizeof(struct ip_list));
             strcpy(head->usr,buf);
             strcpy(head->ip,inet_ntoa(si_other.sin_addr));
             head->next=NULL;
             node=head;
             userlist();
             continue;
             } 
           else
             {
              newnode=(struct ip_list *)malloc(sizeof(struct ip_list));
              strcpy(newnode->usr,buf);
              strcpy(newnode->ip,inet_ntoa(si_other.sin_addr));
              k=findipinlist(newnode->ip);
              newnode->next=NULL;
             } 
           if(k==-1)
            {
          node->next=newnode;
          node=newnode;
          userlist();
            }       
       } 
    close(s);
    return 0;
   }
 
  int findipinlist(char *ip)
   {
    node=head;
    while(node!=NULL)
    { 
     
     if (strcmp(node->ip,newnode->ip))
     {
	if(node->next)
	 node=node->next;
	else
         return -1;
     }
     else
      return 0;  
   }
   return -1; 
  }

int userlist()
{   
    
    id[index]=node->usr;
    ip[index]=node->ip;
    index++;
    //list();
}
int list1()
	{
         pthread_t thread_reader,thread_writer,thread_reader1,thread_writer1;
         int iret1,iret2;
 	 char *message1="thread_reader";
 	 char *message2="thread_writer";
         int a,l;
         iret1=pthread_create(&thread_writer,NULL,receiver,(void *)message2);
         iret2=pthread_create(&thread_reader,NULL,sender,(void *)message1);
 	 pthread_join(thread_writer,NULL);
 	 pthread_join(thread_reader,NULL); 
         }

int list()
{
for(i=0;i<=3;i++) 
{
 if (id[i]==NULL)
  break;
 printf("%d.%s\n",i,id[i]);
 printf("  %s\n",ip[i]);
}
}

int main()
     {
        int a,l;
        list1();
        printf("enter 1 to display list");
        scanf("%d",&l);
         switch (l)
           {
           case 1:list();
                   break;
           default :
           printf("Invalid option\n" );
            }
     }
