
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXSIZE 1000000

struct request{
	char who; 
	int id[4];
	int status; 
	int term; 
	int msgSize;
	char body[MAXSIZE]; 
};

struct response{
	int term;
	int msgSize; 
	char body[MAXSIZE];
}

int main(void)
{

	/*--------------------------------------------------------------*/
	/* section 0 prepare request*/
	struct request req;
	struct response res; 
	memset(req,'\0',sizeof(struct request)); 
	memset(res,'\0',sizeof(struct response));
	// Get input from the user or file:
	// in future dev will reading from file 
	// in this version will use dummy request for testing purpose
	req.who = 's';
	req.id ={1,2,3,4};
	req.status = 1; 
	req.term =0; 
	req.msgSize = 0;


	/*--------------------------------------------------------------*/
	/* section 1 prepare socket*/
	int socket_desc;
	struct sockaddr_in server_addr;
	// Create socket:
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0){
		printf("Unable to create socket\n");
		return -1;
	}

	printf("Socket created successfully\n");

	// Set port and IP the for server 
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Send connection request to server:
	if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		printf("Unable to connect\n");
		return -1;
	}
	printf("Connected with server successfully\n");


	/*--------------------------------------------------------------*/
	/* section 2 interact with Creator */

	// send a request for job
	if(send(socket_desc, req, sizeof(struct request), 0) < 0){
		printf("Unable to send message\n");
		return -1;
	}

	// Receive job:
	if(recv(socket_desc, res, sizeof(struct response), 0) < 0){
		printf("Error while receiving server's msg\n");
		return -1;
	}

	// Maintian or terminate connection based on job requirement 
	if(res.term>0){
		close(socket_desc);
	}

	// handle job and prepare report 
	printf("now working on job");
	char strip[32];
	strcpy(res.body, strip);	
	bool result = icmppacket(strip);
	if (result){
		req.status = 4;
	}	
	else{
		req.status = 3; 
	}
	req.term = 1; 

	// Send final report
	if(send(socket_desc, req, sizeof(struct request), 0) < 0){
		printf("Unable to send message\n");
		return -1;
	}

	// Close the socket:
	close(socket_desc);

	return 0;
}

