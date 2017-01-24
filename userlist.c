#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

#define CLADDR_LEN 100
#define ipa 16
#define user 50
#define BUF_SIZE 200
#define MAXBUF 200
#define BUFLEN 200
#define buff 200
#define PORT 4444

static int y=0;

 struct ip_list
  {
   char usr[user];
   char ip[ipa]; 
   int busy;
   int away;
   struct ip_list *next;
  };

  struct ip_list *head=NULL;
  struct ip_list *node;
  struct ip_list *newnode;

struct info
 {
  char username[50];
  int busy;
 int away;
 };
 struct info myinfo;

 void* sender(void *ptr)
  {
   int sockfd, status, buflen, sinlen,ret;
   struct sockaddr_in sock_in;
   int yes = 1,interval=10,seconds_to_sleep,duration,elapsed,i;
   
  
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
   
   for(;;)
       {
        myinfo.busy=getbusystatus();
         if(y>=10)
         {
         myinfo.away=1;
         }
          else 
          {
         myinfo.away=0;
          } 
        // printf("%d",myinfo.away);
         ret = sendto(sockfd, &myinfo, sizeof(struct info), 0, (struct sockaddr *)&sock_in, sinlen);  
         if (ret < 0) 
          {  
            printf("Error sending data!\n\t-%s\n",&myinfo);  
          }
         sleep(1);
       }
    // close(sockfd);
   }
 

   void *receiver(void *ptr)
    {
     struct sockaddr_in si_me, si_other;
     int s, i, slen = sizeof(si_other) , recv_len,ret,k,x,z;
     struct info userinfo;
     
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
      exit(0);
     }

  
      
     for(;;)
       {
        fflush(stdout);
       
        if ((recv_len = recvfrom(s, &userinfo,sizeof(struct info), 0, (struct sockaddr *) &si_other, &slen)) == -1)
         {
          printf("error in recvfrom()");
         }
      
          if(head==NULL)
            {
             head=(struct ip_list *)malloc(sizeof(struct ip_list));
             strcpy(head->usr,userinfo.username);
             strcpy(head->ip,inet_ntoa(si_other.sin_addr));
             head->busy=userinfo.busy;
             head->away=userinfo.away;
             head->next=NULL;
             node=head;
             continue;
            } 
           else
             {
              newnode=(struct ip_list *)malloc(sizeof(struct ip_list));
              strcpy(newnode->usr,userinfo.username);
              strcpy(newnode->ip,inet_ntoa(si_other.sin_addr));
              newnode->busy=userinfo.busy;
              newnode->away=userinfo.away;
              k=findipinlist(newnode->usr);
              x=changebusy(newnode->busy,newnode->ip);
              z=changeaway(newnode->away,newnode->ip);
              newnode->next=NULL;
             } 
           
           if(k==-1)
            {
             node->next=newnode;
             node=newnode;
            } 
           
          
           
           if(z==0)
            {
              node->away=newnode->away;
            }
                  
       } 
    close(s);
    return 0;
   }
 
int getUserListCount()
 {
    int i =0;
    node=head;
    while(node!=NULL)
    { 
       i++;
       node=node->next;
      
    }
   return i; 
  
 }

 char *getUserIP(int index)
  {
   int i=1;
   node=head;
   while(node!=NULL)
    { 
	
 	if(i!=index)
          {
 	    node=node->next;
            i++;
          }
         else
          return node->ip;
          
     }
   return -1;
  }

 char *getUserID(int index)
  { 
   int i=1;
   node=head;
   while(node!=NULL)
     {
      if(i!=index)
        {
         node=node->next;
         i++;
        }
       else
         return node->usr;
         
       }
     return -1;
    }     

 int findipinlist(char *ip)
   {
    node=head;
    while(node!=NULL)
    { 
     
     if (strcmp(node->ip ,newnode->ip))
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
 int *awaystatus(int index)
  { 
   int i=1;
   node=head;
   while(node!=NULL)
     {
      if(i!=index)
        {
         node=node->next;
         i++;
        }
       else
         return node->away;;
         
       }
     return -1;
    } 
 char *getuserstatus(int index)
  {
    int i=1;
   node=head;
   while(node!=NULL)
     {
      if(i!=index)
        {
         node=node->next;
         i++;
        }
       else
         return node->busy;
      }
     return -1;
   }
  
 int changebusy(int *busy,char *ip)
   { 
    
    node=head;
    
    while(node!=NULL)
    { 
      while(strcmp(node->ip,newnode->ip))
          {
             if(node->next)
             {
             node=node->next;
             }	
             else
             return-1;
          }
       
            if(node->busy!=newnode->busy)
               {
                node->busy=newnode->busy;
               }   
              return 0; 
           }
   return -1; 
  }
int changeaway(int *away,char *ip)
   { 
    
    node=head;
    
    while(node!=NULL)
    { 
      while (strcmp(node->ip,newnode->ip))
          {
             if(node->next)
             {
             node=node->next;
             }	
             else
             return-1;
          }
       if(node->away!=newnode->away)
              {
              node->away=newnode->away;
              return 0;    
              }  
          return 0;
     }
   return -1; 
  }
 int GetIdleTime () 
      {
        time_t idle_time;
        static XScreenSaverInfo *mit_info;
        Display *display;
        int screen;
        mit_info = XScreenSaverAllocInfo();
        if((display=XOpenDisplay(NULL)) == NULL) { return(-1); }
        screen = DefaultScreen(display);
        XScreenSaverQueryInfo(display, RootWindow(display,screen), mit_info);
        idle_time = (mit_info->idle) / 1000 ;
        XFree(mit_info);
        XCloseDisplay(display);
        return idle_time;
      }

     void *idle(void *ptr)
       {
          while(1)
          {
          sleep(1);
          if(GetIdleTime()>0)
           {
            y++;
           } 
          else
            y=0;
          }
       }
  
   int main(int argc,char *argv[])
     {
      if(argc<2)
       {
        printf(" enter username\n");
        exit(0);
       }
      
      strcpy(myinfo.username,argv[1]);
      //myinfo.busy=0;                          
      pthread_t thread_reader,thread_writer,thread_idletime;
      int iret1,iret2,iret3;
      char *message1="thread_reader";
      char *message2="thread_writer";
      char *message3="thread_idletime";
      iret1=pthread_create(&thread_reader,NULL,receiver,(void *)message2);
      iret2=pthread_create(&thread_writer,NULL,sender,(void *)message1);
      iret3=pthread_create(&thread_idletime,NULL,idle,(void *)message3); 
        
      list1();
     }
