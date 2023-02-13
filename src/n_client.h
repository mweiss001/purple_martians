// n_client.h

void client_read_packet_buffer(void);
void client_fast_packet_loop(void);
int  ClientInitNetwork(const char *serveraddress);
void ClientExitNetwork(void);
int  ClientCheckResponse(void);
int  ClientReceive(void *data);
void ClientSend(void *data, int len);
void client_flush(void);
int  client_init_join(void);
void client_exit(void);
int  client_init(void);
void client_read_game_move_from_packet(int x);
void client_send_ping(void);
void client_process_stdf_packet(double timestamp);
void client_apply_dif();
void client_block_until_initial_state_received(void);
void client_process_sdat_packet(void);
void client_process_serr_packet(void);
void client_proc_player_drop(void);
void client_control(void);
void client_local_control(int p);
void process_bandwidth_counters(int p);
