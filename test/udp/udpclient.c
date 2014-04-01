
/* Sample UDP client */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[1000];
   
   if (argc != 2)
   {
      printf("usage:  udpcli <IP address>\n");
      exit(1);
   }

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);

   
  //format F<board><clock frequency>
#if 1
  sprintf(sendline, "F1200000,F2900000,F31600000,F41200000");					//Clock frequencies
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
  //Format C<CPU><online/offline>
#endif
#if 1
  sprintf(sendline, "C0a,C1a,C2a,C3a");							//Cores board 1
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
#endif
#if 1
  sprintf(sendline, "C4a,C5a,C6b,C7b");							//Cores board 2
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
  sprintf(sendline, "C8a,C9b,C10b,C11b");							//Cores board 3
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
  sprintf(sendline, "C12b,C13b,C14b,C15b");							//Cores board 4
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
  //format L<board><load>
#endif
#if 1
  sprintf(sendline, "L1100,L250,L30,L410");							//Load
  sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
#endif
  
}
