
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1000
struct data{
	str::string ip;
	int port;
	str::string result;
}

struct request{
	char who; 
	int id;
	int status; 
	int term; 
	int msgSize;
	struct data body;
};

struct response{
	int term;
	int msgSize; 
	struct data body;
}


int sendOutJob(int socket_desc, struct sockaddr* client_addr);


int main(void)
{
	/*--------------------------------------------------------*/
	/* section 0 prepare creator log and queue*/

	// total job queue
	int log = 1000000; 
	struct request* queue[MAXSIZE];
	int queue_desc[MAXSIZE];

	/*---------------------------------------------------------*/
	/* section 1 prepare response*/
	struct response res; 

	char* job = "127.0.0.0";
	int port = 666;
	res.term =0;
	res.msgSize=1;	
	strcpy(res.body.ip, job);	
	res.body.port = port;

	/*----------------------*/
	/* section 2 prepare socket*/

	int socket_desc, client_sock, client_size;
	struct sockaddr_in server_addr, client_addr;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0){
		printf("Error while creating socket\n");
		return -1;
	}
	printf("Socket created successfully\n");

	// Set port and IP:
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Bind to the set port and IP:
	if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
		printf("Couldn't bind to the port\n");
		return -1;
	}
	printf("Done with binding\n");

	// Listen for clients:
	if(listen(socket_desc, 1) < 0){
		printf("Error while listening\n");
		return -1;
	}
	printf("\nListening for incoming connections.....\n");


	/*--------------------------------------------------------*/
	/* section 3 send out job update log*/


	// Accept client request
	for(int index =0; index < log; index++){

		queues_desc[index] = sendOutJob(int socket_desc, struct sockaddr* client_addr);
		struct request * reqt =(struct request*) malloc(sizeof(struct reques));
		if (recv(client_sock, *reqt, sizeof(struct request), 0) < 0){
			printf("Couldn't receive\n");
			return -1;
		}
		queue[index] = reqt;
	}		

	/*--------------------------------------------------------*/
	/* section 4 recieve report*/

	// Receive client report, remove from queue and close out connection 
	while(log>1){
		if (recv(client_sock, req, sizeof(struct request), 0) < 0){
			printf("Couldn't receive\n");
			return -1;
		}
		printf("%s",req.body.result);

		if (send(client_sock, res, sizeof(struct response), 0) < 0){
			printf("Can't send\n");
			return -1;
		}
		close(queue_desc[index]);

		// find finished task and remove from queue 
		int id = reqt->id;
		for(int index = 0; index< log, index++){
			if(id == queue[index]->id){
				queue[index]=queue[log];
			}	
		}
		log--;
	}
	/*--------------------------------------------------------*/
	/* section 5 terminate all connection*/
	close(socket_desc);
	return 0;
}

int sendOutJob(int socket_desc, struct sockaddr* client_addr){
	// Accept an incoming connection:
	client_size = sizeof(* client_addr);
	client_sock = accept(socket_desc, client_addr, &client_size);

	if (client_sock < 0){
		printf("Can't accept\n");
		return -1;
	}
	printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	return client_sock;
}
