extern NET_CONN *ServerConn;
int ClientInit(char *serveraddress);
void ClientExit(void);
int ClientCheckResponse(void);
int ClientReceive(void *data);
void ClientSend(void *data, int len);
