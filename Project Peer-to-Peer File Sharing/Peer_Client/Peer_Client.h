/**
* create_tracker: message from peer to the tracker server only for files originally from this peer
* if command successful: <createtracker succ>\n 
* if command unsuccessful: <createtracker fail>\n 
* if tracker file already exists: <createtracker ferr>\n
*/
int create_tracker(void);
/**
* update_tracker: message from peer to the tracker server only for files originally from this peer
* <updatetracker filename start_bytes end_bytes ip-address port-number>\n
* updatetracker: message from tracker server to the peer program 
* if tracker file does not exist: <updatetracker filename ferr>\n 
* if tracker file update successful: <updatetracker filename succ>\n 
* any other error / unable to update tracker file: <updatetracker filename fail>\n
*/
int update_tracker(void);

/**
* LIST – This command is sent by a connected peer to the tracker server to send over to the 
* requesting peer the list of  (tracker) files in the shared directory at the server. The format 
* of the incoming message from the connected peer will be  
* <REQ LIST>\n
*/
void request_list(void);

/**
* Additionally the P2P peer must be able to request the tracker server for a particular 
* tracker file so that it can initiate the file transfer process. Protocol for the message sent by 
* P2P peer to the tracker server in order to request a particular tracker: 
 equals to search 
* <GET filename.track >\n 
*/
//void get_track(char[] file_name);
