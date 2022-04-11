// {\rtf1\ansi\ansicpg1252\cocoartf2580
// \cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
// {\colortbl;\red255\green255\blue255;}
// {\*\expandedcolortbl;;}
// \margl1440\margr1440\vieww11520\viewh8400\viewkind0
// \pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

// \f0\fs24 \cf0 

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/dir.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include <vector.c>



//maximum size of data from peer
#define MAX_IN_DATA_SIZE 1024
#define MAX_LIST_PEERS 10

//thread functions
void *clientThread(void *);
void *readLineThread(void *);

//
char* Search(char *);
char* Update(char *);
char* Upload(int);

//handles commands given from the client
char* handleCommandsClient(char *);
//insertion sort for sorting client ID's after clients join
void insertionSort(void);
//binary search for searching for client ID's
int binarySearchID(int, int, int);

int handle_list_req(int i);
int handle_createtracker_req(int i);
int handle_updatetracker_req(int i);
char* xtrct_fname(char *, char *);
int tokenize_updatemsg(char *);
int tokenize_createmsg(char *);


//insertion sort for sorting client ID's after clients join
void insertionSort(void);

//binary search for searching for client ID's
int binarySearchID(int, int, int);

//structure containing client info, and the vector for the client files
struct client {
  int ID;
  char* address;
  int port;
  int accept_files;
  struct files* client_files;
};

struct {
	client c;
	int start_byte;
	int end_byte;
	int time_stamp;
}list_peers[MAX_LIST_PEERS];

// structure containing file info
struct files {
	int ID;
	char *file_name;
	int file_size;
	char *description;
	char *MD5;
};

//command used for getting server commands, server address for ipv4
char command[50];
char server_address[20];

//constant integer used to indicate true
const int true_int = 1;
//creates the client structure pointer for creating a vector later
struct client* client_vector;
//integer pointer for a vector containing the ID's of peers willing to accept
//files
int* upload_vector;




//structure containing client info, and the vector for the client files
struct client {
  int ID;
  char* address;
  int port;
  int accept_files;
  struct files* client_files;
};
//structure containing file info
struct files {
  int ID;
  char* file_name;
};
//command used for getting server commands, server address for ipv4
char command[50];
char server_address[20];
//constant integer used to indicate true
const int truetwo_electric_truealoo_two = 1;
//creates the client structure pointer for creating a vector later
struct client* client_vec;
//integer pointer for a vector containing the ID's of peers willing to accept
//files
int* upload_vec;
//default parameter is the ipv4 address of the server
int main(int argc, char **argv) {
  //sets the server_address to the ipv4 address of the machine given as a
  //parameter
  //this is due to needing to use this network between peers on the computer
  //running the server, and peers running on different machines as it would store
  //local peers as 127.0.0.1 and that address is no good to peers on different machines
  memcpy(server_address, argv[1], sizeof(server_address));
  //creates the thread for server commands
  pthread_t read_line;
  if(pthread_create(&read_line, NULL, readLineThread, NULL) < 0) {

    perror("Could not create read line thread");
    return 1;

  }
  //sets client_vector and upload_vec to be vectors
  client_vector = vector_create();
  upload_vector = vector_create();
  //socket info as well as structures for address and port info
  int socket_desc, client_sock, c, *new_sock;
  struct sockaddr_in server, client;

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {

    printf("Could not create socket");

  }

  puts("Socket created");

  //Prepare the server sockaddr_in structure with default port (3000)
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(3000);

  //Bind
  if(bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
    //print the error message
    perror("Bind failed. Error");
    return 1;

  }

  puts("Bind done");

  //Listen for peers
  listen(socket_desc, 3);

  //Accept and incoming connection
  puts("Waiting for incoming connections...");

  c = sizeof(struct sockaddr_in);
  while((client_sock = accept(socket_desc, (struct sockaddr*) &client,
  (socklen_t*) &c))) {
    puts("Connection accepted:");
    //generates a new thread using the new connection
    pthread_t client_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;
    //grabs the client ip
    char clientip[20];
    sprintf(clientip, "%s", inet_ntoa(client.sin_addr));
    //temporary structure for adding to the client_vector
    struct client curr;
    //assigns the address to a point in memory, without allocating the memory
    //the address will be the same as the last client to connect
    curr.address = (char *) malloc(sizeof(char*)*(sizeof(clientip)));
    //sees if the connection is local, if it is, it assigns the peer
    //the ipv4 address
    if(strcmp(clientip, "127.0.0.1") == 0) {
      strcpy(curr.address, server_address);
    } else {
      strcpy(curr.address, clientip);
    }
    vector_add(&client_vector, curr);
    //creates the thread, passing the socket as a parameter
    if(pthread_create(&client_thread, NULL, clientThread, (void*) new_sock) < 0) {

      perror("Could not create read thread");
      return 1;

    }

  }

  if (client_sock < 0) {

    perror("Accept failed");
    return 1;

 }

 free(new_sock);
 return 0;

}

//used for debugging in the server, /list lists the files, /clients lists the
//client ID's
void *readLineThread(void *unused) {

  char command_in[50];
  while(truetwo_electric_truealoo_two) {

    fgets(command_in, 50, stdin);

    if(strcmp(command_in, "/list") == 10) {

      for(int i = 0; i < vector_size(client_vector); i++) {

        for(int j = 0; j < vector_size(client_vector[i].client_files); j++) {

          printf("Client ID: %d File ID: %d File Name: %s\n", client_vector[i].ID, client_vector[i].client_files[j].ID, client_vector[i].client_files[j].file_name);

        }

      }

    } else if(strcmp(command_in, "/clients") == 10) {

      for(int i = 0; i < vector_size(client_vector); i++) {
        printf("%d ", client_vector[i].ID);
      }
      printf("\n");

    } else if(strcmp(command_in, "/quit") == 10) {
      exit(0);
    }

  }

}

//method for use in a thread
void *clientThread(void *socket_desc) {
  //creates the socket, neccesary info like the id and location of the peer
  //in the client_vec vector and the send and read buffs
  int sock = *(int*) socket_desc;
  int id = vector_size(client_vector)-1;
  int location;
  char *send_buff, read_buff[MAX_IN_DATA_SIZE], data;
  //first message contains info like the peer port, ID, and it creates
  //the client_files vector within the vector and whether the peer is willing
  //to accept files
  recv(sock, read_buff, MAX_IN_DATA_SIZE, 0);
  char *token = strtok(read_buff, "+");
  client_vector[id].port = atoi(token);
  token = strtok(NULL, "+");
  client_vector[id].ID = atoi(token);
  client_vector[id].client_files = vector_create();
  id = atoi(token);
  token = strtok(NULL, "+");
  client_vector[id].accept_files = atoi(token);
  if(client_vector[id].accept_files) {
    vector_add(&upload_vec, id);
  }
  //sorts the client_vec vector  so the ID's are in order for searching
  insertionSort();
  //grabs the location of the peer in client_vec through a binary search
  char port_str[20];
  location = binarySearchID(0, vector_size(client_vector), id);
  sprintf(port_str, "Peer port: %d", client_vector[location].port);
  puts(port_str);

  puts("Handler assigned");
  //while the server is still getting at least one byte of data
  while(recv(sock, &data, 1, MSG_PEEK) == 1) {
    //layout of interactions with the peer is get a message, handle the message,
    //then send a response
    recv(sock, read_buff, MAX_IN_DATA_SIZE, 0);
    send_buff = handleCommandsClient(read_buff);
    send(sock, send_buff, MAX_IN_DATA_SIZE, 0);

    bzero(read_buff, MAX_IN_DATA_SIZE);

  }

  close(sock);
  //when the client disconnects it does a search for where it was in client_vec
  //then proceeds to remove all data from its client_files vector and removes
  //the entry from client_vec
  printf("Client %d Disconnected", id);
  char temp[20];
  location = binarySearchID(0, vector_size(client_vector), id);
  if(location != -1) {

    vector_erase(client_vector[location].client_files, 0, vector_size(client_vector[location].client_files));
    free(client_vector[location].address);
    vector_remove(client_vector, location);
    for(int i = 0; i < vector_size(upload_vec); i++) {

      if(upload_vec[i] == id) {

        vector_remove(upload_vec, i);
        break;

      }

    }

  }

  puts(temp);

}

//the layout of all messages from the peer come with the first term seperated by
//a +, 'command+message'
char* handleCommandsClient(char* command) {
  //checks whether the message was one for searching for a file, updating the
  //files that the peer has, or uploading a file
  char *token = strtok(command, "+");
  if(strcmp("search", token) == 0) {

    token = strtok(NULL, "+");
    return Search(token);

  } else if(strcmp("update", token) == 0) {

    token = strtok(NULL, "+");
    return Update(token);

  } else if(strcmp("upload", token) == 0) {

    token = strtok(NULL, "+");
    return Upload(atoi(token));

  }

}

//method for searching through the files that the peers have
//and returning the info for that file if it is available
char* Search(char* command) {
  char locations[MAX_IN_DATA_SIZE];
  bzero(locations, MAX_IN_DATA_SIZE);
  char *end;
  free(end);

  for(int i = 0; i < vector_size(client_vector); i++) {

    for(int j = 0; j < vector_size(client_vector[i].client_files); j++) {

      if(strcmp(command, client_vector[i].client_files[j].file_name) == 0) {

        sprintf(locations, "%s%d+%s+%s+%d$\n", locations,
        client_vector[i].client_files[j].ID, client_vector[i].client_files[j].file_name,
        client_vector[i].address, client_vector[i].port);

      }

    }

  }

  end = (char *) malloc(sizeof(char*)*(sizeof(locations)));
  strcpy(end, locations);
  //returns the data of file data, addresses, ports, ID's, etc
  return end;

}

//method for updating files in the client_files vector within the client_vector
char* Update(char* id_files) {

  //sets token to be the ID of the peer, creates a vector for file names
  char *end = (char *) malloc(sizeof(char*)*(sizeof("Update Completed")));
  strcpy(end, "Update Completed");
  char *token = strtok(id_files, "$");
  char** names;
  names = vector_create();
  //searches for the ID given from token
  int id = binarySearchID(0, vector_size(client_vector), atoi(token));
  //frees previous data the is held within the client_files vector
  for(int i = 0; i < vector_size(client_vector[id].client_files); i++) {
    free(client_vector[id].client_files[i].file_name);
  }
  //erases the vector entirely
  vector_erase(client_vector[id].client_files, 0, vector_size(client_vector[id].client_files));
  //grabs the file names and adds them to the names vector
  while((token = strtok(NULL, "$")) != NULL) {

    vector_add(&names, token);

  }
  //temp char pointer for the file names
  char* file_names;
  //iterates through the whole names vector
  for(int i = 0; i < vector_size(names); i++) {
    //token for splitting the contents of the names vector
    char *token2 = strtok(names[i], "@");
    file_names = (char *) malloc(sizeof(char*)*(sizeof(token2)));
    //sets the file name
    strcpy(file_names, token2);
    //creates a temporary files structure that can be given file names
    //and ID's and pass them to the client_files vector within client_vector
    struct files temp_file;
    temp_file.file_name = file_names;
    token2 = strtok(NULL, "@");
    temp_file.ID = atoi(token2);
    vector_add(&client_vector[id].client_files, temp_file);

  }
  //returns confirmation of the update taking place
  return end;

}

//method for retrieving a peer address+port for the peer to connect to
//and upload the file
char* Upload(int curr_id) {
  //variables needed, end_up is allocated and freed as a hack to free the data
  //allocated later on
  time_t t;
  int selected;
  char* end_up = (char *) malloc(1);
  free(end_up);
  char temp[20];
  bzero(temp, 20);
  srand((unsigned) time(&t));
  //grabs a peer at random from the size of upload_vec and passes the id to the
  //binary search to get the location of the peer within client_vector
  selected = rand()%vector_size(upload_vec);
  int id = binarySearchID(0, vector_size(client_vector), upload_vec[selected]);
  //checks whether the selected peer is the peer making the request
  //if it is it runs until it isn't
  while(curr_id == client_vector[id].ID) {
    selected = rand()%vector_size(upload_vec);
    id = binarySearchID(0, vector_size(client_vector), upload_vec[selected]);
  }
  //sets the return variable to be address+port
  sprintf(temp, "%s+%d", client_vector[id].address, client_vector[id].port);
  end_up = (char *) malloc(sizeof(char*)*(sizeof(temp)));
  strcpy(end_up, temp);

  return end_up;

}

//method for insertion sorting the client_vector vector
void insertionSort(void) {

  int temp, j;
  struct client temp_vec;

  for(int i = 1; i < vector_size(client_vector); i++){

      temp = client_vector[i].ID;
      temp_vec = client_vector[i];
      j = i - 1;

      while((temp < client_vector[j].ID) && (j >= 0)){

         client_vector[j + 1] = client_vector[j];
         j = j - 1;

      }

      client_vector[j + 1] = temp_vec;

   }

}

//method for binary searching the client_vector vector based on client_vector.ID
int binarySearchID(int start, int end, int id) {

    if (end >= start) {

        int mid = start + (end - start)/2;

        if (client_vector[mid].ID == id) {

          return mid;

        }

        if (client_vector[mid].ID > id) {

          return binarySearchID(start, mid - 1, id);

        }

        return binarySearchID(mid + 1, end, id);

    }

    return -1;

}


// main(int argc, char **argv){

// 	pid_t pid;

//    struct sockaddr_in server_addr, client_addr;   

//    if ((sockid = socket(AF_INET,SOCK_STREAM,0)) < 0){//create socket connection oriented

// 	   printf("socket cannot be created \\n"); exit(0); 

//    }

    

//    //socket created at this stage

//    //now associate the socket with local port to allow listening incoming connections

//    server_addr.sin_family = AF_INET;// assign address family

//    server_addr.sin_port = htons(server_port);//change server port to NETWORK BYTE ORDER

//    server_addr.sin_addr.s_addr = htons(INADDR_ANY);

    

//    if (bind(sockid ,(struct sockaddr *) &server_addr,sizeof(server_addr)) ==-1){//bind and check error

// 	   printf("bind  failure\\n"); exit(0); 

//    }

    

//    printf("Tracker SERVER READY TO LISTEN INCOMING REQUEST.... \\n");

//    if (listen(sockid) < 0){ //(parent) process listens at sockid and check error

// 	   printf(" Tracker  SERVER CANNOT LISTEN\\n"); exit(0);

//    }                                        

   

//    while(1) { //accept  connection from every requester client

// 	   if ((sock_child = accept(sockid ,(struct sockaddr *) &client_addr,&clilen))==-1){ /* accept connection and create a socket descriptor for actual work */

// 		   printf("Tracker Cannot accept...\\n"); exit(0); 

// 	   }



// 	   if ((pid=fork())==0){//New child process will serve the requester client. separate child will serve separate client

// 		   close(sockid);   //child does not need listener

// 		   peer_handler(sock_child);//child is serving the client.		   

// 		   close (sock_child);// printf("\\n 1. closed");

// 		   exit(0);         // kill the process. child process all done with work

//         }

// 	   close(sock_child);  // parent all done with client, only child will communicate with that client from now

	   

//      }  //accept loop ends            

// } // main fun ends  


// peer_handler(int sock_child){ // function for file transfer. child process will call this function     

//     //start handiling client request	

// 	int length;

// 	length=read(sock_child,read_msg,MAXLINE);			

// 	read_msg[length]='\\0';

// 	if((!strcmp(read_msg, "REQ LIST"))||(!strcmp(read_msg, "req list"))||(!strcmp(read_msg, "<REQ LIST>"))||(!strcmp(read_msg, "<REQ LIST>\\n"))){//list command received

// 		handle_list_req(sock_child);// handle list request

// 		printf("list request handled.\\n");

// 	}

// 	else if((strstr(read_msg,"get")!=NULL)||(strstr(read_msg,"GET")!=NULL)){// get command received

// 		xtrct_fname(read_msg, " ");// extract filename from the command		

// 		handle_get_req(sock_child, fname);		

// 	}

// 	else if((strstr(read_msg,"createtracker")!=NULL)||(strstr(read_msg,"Createtracker")!=NULL)||(strstr(read_msg,"CREATETRACKER")!=NULL)){// get command received

// 		tokenize_createmsg(read_msg);

// 		handle_createtracker_req(sock_child);

		

// 	}

// 	else if((strstr(read_msg,"updatetracker")!=NULL)||(strstr(read_msg,"Updatetracker")!=NULL)||(strstr(read_msg,"UPDATETRACKER")!=NULL)){// get command received

// 		tokenize_updatemsg(read_msg);

// 		handle_updatetracker_req(sock_child);		

// 	}

	

// }//end client handler function

