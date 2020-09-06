#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <string.h>
#include <sys/stat.h>


#include <unistd.h>
#include <fcntl.h>


void registration(int sock){

	int n;
	char buffer[256];
	bzero(buffer,255);

	FILE *fp;
	int m;

	n = read(sock,buffer,256);
	if(n < 0){
		perror("Error reading from socket");
		exit(1);
	}

	fp = fopen("registration.txt","a");
	if (m == NULL){
		perror("Error on creating file");
		exit(1);
	}

	n = read(sock,buffer,256);
	if(n < 0){
		perror("Error reading from socket");
		exit(1);
	}

	fwrite(buffer, sizeof(char), sizeof(buffer), fp);

	n = read(sock,buffer,256);
	if(n < 0){
		perror("Error reading from socket");
		exit(1);
	}

	fwrite(buffer, sizeof(char), sizeof(buffer), fp);

	fclose(fp);

	printf("%s\n", "Done!");

}


void browse_master(int sock){
	
//TODO:change the fix length char array.
	int n;
	char buffer[8192];
	char buff[256];
	bzero(buffer,8192);
	char *token;
	char all_string[8192];
	FILE *fp;
	fp=fopen("master.txt","r");
	if (fp!= NULL){
    	fseek(fp, 0, SEEK_SET); 
    	while(!feof(fp)){
        	memset(buff, 0x00, 256); 
        	fgets(buff, 255, (FILE*)fp);
        	strcat(all_string, buff);
    	}
  
	}
	fclose(fp);
	n = write(sock,all_string,8192);
	if(n < 0){
		perror("Error writing to socket");
		exit(1);
	}
	printf("%s\n", "Done!");


}




int download_mp3_files(int sock,int mid){
	int n,rc,sent_bytes,m;
	off_t *offset;
	char *buffer;
	char buff[256];
	char buffer_recv[256];
	char *filepath;
	char *fileData;
	bzero(buff,255);
	bzero(buffer_recv,256);
	struct stat file_stat;

	int mid_count;
   	char *token;

   	mid_count = 0;
   	FILE *fp;
	fp=fopen("master.txt","r");
	if (fp!= NULL){
    	fseek(fp, 0, SEEK_SET); 
    	while(!feof(fp)){
        	memset(buff, 0x00, 256); 
        	fgets(buff, 255, fp);
        	mid_count += 1;
    	}
		mid_count -= 1 ;
	}
	fclose(fp);

	if(mid > mid_count){
		mid = mid_count;
	}
	fp=fopen("master.txt","r");
	if (fp!= NULL){
    	fseek(fp, 0, SEEK_SET); 
    	while(!feof(fp)){
    		mid_count += 1;
        	memset(buff, 0x00, 256); 
        	fgets(buff, 255, (FILE*)fp);
        	token = strtok(buff, " ");
   			if(mid == atoi(token)){
   				printf("%s\n",token );
				int count = 0;
				   	while( token != NULL ){	
				   		if(count == 0){
				   			fileData = strtok(NULL, " ");
				   			count += 1;
				   		}else if(count == 1){
				   			filepath = strtok(NULL," ");
				   			count += 1 ;
				   		}else {
				   			break;
				   		}
				    }
   				break;
			}
        }
	}






    fclose(fp);
    char *temp;
    temp = strtok(filepath, "\n");
    filepath = temp;
	fp = open(filepath,O_RDONLY);
	if (fp == -1){
		perror("Error on opening file");
		exit(1);
	}
	if (fstat(fp, &file_stat) < 0){
		perror("fstat error...");
		exit(1);
	}
	printf("%zu\n", file_stat.st_size);
	int size = (int)file_stat.st_size;
	char str[128];
	sprintf(str, "%d", size);
	strcat(fileData,"-");
	strcat(fileData,str);
	strcat(fileData,"-");

	printf("%s%s\n", "sending file data: ",fileData);

	n = send(sock,fileData,512,0);
	if(n < 0){
		perror("Error writing to socket");
		exit(1);
	}
	printf("%s\n", "filedata sent!");
	
	sleep(3);
	int remain_data = 10;
	char ack[10];
	n = recv(sock, ack, 10, 0);
   	if(n < 0){
      perror("Receive error:");
      exit(1);
  	}
	if(strcmp(ack,"temp") == 0){
	    offset = 0;
	    rc = sendfile (sock, fp, &offset, file_stat.st_size);
	    if (rc == -1) {
	      perror("Sending failure...");
	      exit(1);
	    }
	    if (rc != file_stat.st_size) {
	      fprintf(stderr, "incomplete transfer from sendfile: %d of %d bytes\n",
	              rc,
	              (int)file_stat.st_size);
	      exit(1);
	    }
	    fclose(fp);
	}
}

void inquire_MID(int sock){
	int n,counter;
   	char buffer[256];
   	char buff[256];
   	bzero(buffer,256);

   	counter = 0;
   	FILE *fp;
	fp=fopen("master.txt","r");
	if (fp!= NULL){
    	fseek(fp, 0, SEEK_SET); 
    	while(!feof(fp)){
        	memset(buff, 0x00, 256); 
        	fgets(buff, 255, fp);
        	counter += 1;
    	}
		counter -= 1 ;
	}
	fclose(fp);

	sprintf(buff, "%d", counter);
	n = write(sock,buff,256);
	if(n < 0){
		perror("Error writing to socket");
		exit(1);
	}
	
	printf("%s\n", "Done!");

}





void welcomming(int sock){
	int n,num2;
	char buffer[256];
	bzero(buffer,256);
	char *token;
	printf("%s\n", "waiting to receive..");
	n = recv(sock,buffer,256,0);
	if(n < 0){
		perror("Error reading from socket");
		exit(1);
	}
	token = strtok(buffer,",");

	int num = atoi(token);

	if (num == 3){
		num2 = atoi(strtok(NULL,","));
	}
	if(num == 1){
		inquire_MID(sock);
	}else if(num == 2 ){
		browse_master(sock);
	}else if(num == 3){
		download_mp3_files(sock,num2);
	}else if(num == 4){
		registration(sock);
	}
}





int main(int argc, char const *argv[]){
	int sockfd, newsockfd, portno, clilen, childpid;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0){
		perror("Error: opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR on binding");
    	exit(1);
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0){
			perror("Error on accept");
			exit(1);
		}
		childpid = fork();
		if( childpid < 0){
			perror("Error on forking");
			exit(1);
		}else if(childpid == 0){
			close(sockfd);
			welcomming(newsockfd);
			exit(0);

		}else {
			close(newsockfd);
		}
	}

	return 0;
}