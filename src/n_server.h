// n_server.h

int ServerInitNetwork(void);
void ServerExitNetwork(void);
void ServerListen(void);
int ServerReceive(void *data, int *sender);
void ServerBroadcast(void *data, int len);
void ServerSendTo(void *data, int len, int who, int player);
void server_flush(void);
int  server_init(void);
void server_exit(void);
void server_send_dif(int p);
void server_create_new_state(void);
void server_rewind(void);
void server_send_sdat(void);
void server_proc_player_drop(void);
void server_proc_cdat_packet(double timestamp);
void server_proc_stak_packet(double timestamp);
void server_proc_cjon_packet(int who);
void server_control();
void server_fast_packet_loop(void);
