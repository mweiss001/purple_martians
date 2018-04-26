#define MAX_CLIENTS 32
extern NET_CONN *ListenConn;
extern NET_CONN *ClientConn[MAX_CLIENTS];
extern int ClientNum;

int ServerInit();
void ServerExit();
int ServerListen();
int ServerReceive(void *data, int *sender);
void ServerBroadcast(void *data, int len);
void ServerSendTo(void *data, int len, int who);
