#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>


void downloadMusic(int sock,int mid){
   int n;
   char buffer[256];
   char temp[10];
   char buff[512];
   char recv_buff[8192];
   bzero(buffer,255);
   bzero(buff,511);
   bzero(recv_buff,8191);


   char *fileName;
   int file_size,len; 
   

   sprintf(buffer, "%d", 3);
   sprintf(temp, "%d", mid);
   strcat(buffer, ",");
   strcat(buffer, temp);

   n = send(sock,buffer,256,0);
   if(n < 0){
      perror("Error writing to socket");
      exit(1);
   }

   printf("%s\n","request to download sent!" );
   int remain_data = 512 ;
   n = recv(sock, buff, 512, 0);
   if(n < 0){
      perror("Receive error:");
      exit(1);
   }
   printf("%s%s\n","received buffer: ", buff );
   fileName = strtok(buff, "-");
   file_size = atoi(strtok(NULL, "-"));
   printf("%s%s%d\n",fileName, " ",file_size );

   strcat(fileName,"(1).mp3");
   FILE *received_file;
   received_file = fopen(fileName, "w");


   if (received_file == NULL){
      perror("Failed to open file");
       exit(1);
   }
   sleep(1);
   printf("%s\n", "sending ack..");
   sprintf(temp, "%d", 5);
   printf("%s%s\n","temp: " , temp );
   n = send(sock,"temp",10,0);
   if(n < 0){
      perror("Error writing to socket");
      exit(1);
   }

   remain_data = file_size;

   while (((len = recv(sock, recv_buff, 8192, 0)) > 0) && (remain_data > 0)){
      fwrite(recv_buff, sizeof(char), len, received_file);
      remain_data -= len;
      fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
   }
   printf("%s\n", "file downloaded!" );
   fclose(received_file);
}









void listMusic(int sock){

   int n;
   char buffer[8192];
   bzero(buffer,8192);
   char temp[256];
   bzero(temp,255);

   sprintf(temp, "%d", 2);
   strcat(temp, ",");

   n = send(sock,temp,256,0);
   if(n < 0){
      perror("Error writing to socket");
      exit(1);
   }

   n = read(sock,buffer,8192);
   if(n < 0){
      perror("Error reading from socket");
      exit(1);
   }
   printf("%s\n",buffer);

}





void inquire_MID(int sock){
   int n;
   char buffer[256];
   bzero(buffer,255);

   char temp[256];
   bzero(temp,255);

   sprintf(temp, "%d", 1);
   strcat(temp, ",");

   n = send(sock,temp,256,0);
   if(n < 0){
      perror("Error writing to socket");
      exit(1);
   }

   n = read(sock,buffer,256);
   if(n < 0){
      perror("Error reading from socket");
      exit(1);
   }
   printf("%s%s\n","Largest MID number: ",buffer);

}




int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
   bzero(buffer,255);
   int file_size;
   FILE *received_file;
   int remain_data = 0;

   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[2]);
   
   while(1){
      /* Now read server response */
         /* Create a socket point */
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }
      
      server = gethostbyname(argv[1]);
      
      if (server == NULL) {
         fprintf(stderr,"ERROR, no such host\n");
         exit(0);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
      serv_addr.sin_port = htons(portno);
      
   /* Now connect to the server */
      if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
         perror("ERROR connecting");
         exit(1);
      }
      printf("%s\n", "------------------------------------\n---------------------------------\n----------------------------");
      printf("%s\n","type the number assosicated with the method: \n1 for inquire_MID...\n2 for browse master...\n3 for download mp3" );
      printf("%s\n", "------------------------------------\n---------------------------------\n----------------------------");

      int num,num2;
      scanf("%d", &num);
      
      if(num == 1){         
         printf("%s\n", "//////////////////////////////////////");
         inquire_MID(sockfd);
      }else if(num == 2){
         printf("%s\n", "//////////////////////////////////////");
         listMusic(sockfd);
      }else if(num == 3){
         printf("%s", "enter a mid number: ");
         scanf("%d",&num2);
         printf("%s\n", "//////////////////////////////////////");
         downloadMusic(sockfd,num2);
      }
      close(sockfd);
   }




   return 0;
}