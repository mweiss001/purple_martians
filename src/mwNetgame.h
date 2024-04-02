// mwNetgame.h

#ifndef mwNetgame_H
#define mwNetgame_H

#define MAX_CLIENTS 32

#include "mwStateHistory.h"


#define PM_RCTL_PACKET_TYPE_keep_alive             0
#define PM_RCTL_PACKET_TYPE_force_client_offset    1
#define PM_RCTL_PACKET_TYPE_client_offset_adj      2
#define PM_RCTL_PACKET_TYPE_zlib_compression_adj   3
#define PM_RCTL_PACKET_TYPE_exra_packet_num_adj    4
#define PM_RCTL_PACKET_TYPE_exra_packet_siz_adj    5
#define PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj    6

#define PM_RCTL_PACKET_TYPE_pvp_shots_toggle       10
#define PM_RCTL_PACKET_TYPE_pvs_shots_toggle       11
#define PM_RCTL_PACKET_TYPE_fakekey_toggle         12
#define PM_RCTL_PACKET_TYPE_server_reload          20



struct client_session
{
   int active;
   int status;
   int who;

   int player_num;

   char session_name[64];

   char timestamp[16];
   char ip[16];
   int port;

   char host_name[16];

   double start_time;
   double end_time;
   double duration;

   int next_levels;
   int exits;

   int cdats_rx;

   int respawns;
   int shots_fired;
   int enemy_hits;

   int player_hits;
   int self_hits;
   int purple_coins;


   int tx_total_bytes;
   int tx_total_packets;
   int tx_max_bytes_per_frame;
   int tx_max_packets_per_frame;

   int rx_total_bytes;
   int rx_total_packets;
   int rx_max_bytes_per_frame;
   int rx_max_packets_per_frame;

};












struct file_to_send
{
   int active;
   int attempts;
   int last_sent_frame;
   int id;
   int who;
   char name[256];
};


class mwNetgame
{
   public:

   mwNetgame(); // constructor

   struct file_to_send files_to_send[20];

   struct client_session client_sessions[16];

   int NetworkDriver;
   int NetworkInit();


   void change_address_port(char * address, int port);
   int get_dynamic_port(void);

   int get_local_port_from_channel(NET_CHANNEL *chan);



   mwStateHistory mStateHistory[8];

   // debug testing of sending more packets
   int srv_exp_num = 0;
   int srv_exp_siz = 0;

   int ima_server = 0;
   int ima_client = 0;
   int remote_join_reply = 0;

   char serveraddress[256] = "192.168.1.2";
   int server_UDP_listen_port;

   int zlib_cmp = 7;

   float client_chase_offset;
   float client_chase_offset_auto_offset;
   int client_chase_offset_mode; // 0 = manual, 1 = auto, 2 = server

   int server_dirty_frame = 0;

   // local client's buffer for building compressed dif from packets
   char client_state_buffer[STATE_SIZE];
   int  client_state_buffer_pieces[16];   // to mark packet pieces as received




   // local client's buffer for building compressed sfil from packets
   char client_sfil_buffer[100000];
   int  client_sfil_buffer_pieces[100];   // to mark packet pieces as received


   // local client's uncompressed dif
   char client_state_dif[STATE_SIZE];
   int  client_state_dif_src;          // src frame_num
   int  client_state_dif_dst;          // dst frame_num

   void game_vars_to_state(char * b);
   void state_to_game_vars(char * b);
   void get_state_dif(char *a, char *b, char *c, int size);
   void apply_state_dif(char *a, char *c, int size);
   void reset_states(void);

   void process_bandwidth_counters(int p);



   // --------------------------------------------------------------------
   // ---   mwNetgameClient.cpp  -----------------------------------------
   // --------------------------------------------------------------------
   NET_CHANNEL *ServerChannel = NULL;

   int  ClientInitNetwork(void);



   void ClientExitNetwork(void);
   int  ClientCheckResponse(void);
   int  ClientReceive(void *data);
   void ClientSend(void *data, int len);
   void ClientFlush(void);

   void client_timer_adjust(void);
   void client_apply_dif();

   void client_send_ping_packet(void);
   void client_send_cjon_packet(void);
   void client_send_cjrc_packet(void);
   void client_send_rctl_packet(int type, double val);
   void client_send_stak_packet(int ack_frame);
   void client_send_cdat_packet(int p);

   void client_proc_pong_packet(char *data);
   void client_proc_sjon_packet(int i);
   void client_proc_stdf_packet(int i);

   void client_proc_sfil_packet(int i);

   void client_send_sfak_packet(int id);
   void client_send_crfl(void);



   void client_proc_sjrc_packet(int i);
   void client_proc_snfo_packet(int i);
   void client_proc_player_drop(void);
   void client_control(void);

   // --------------------------------------------------------------------
   // ---   mwNetgameServer.cpp  -----------------------------------------
   // --------------------------------------------------------------------

   NET_CHANNEL *ListenChannel = NULL;                   // listen channel
   NET_CHANNEL *ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client

   int ClientChannelLastRX[MAX_CLIENTS] = {0};          // keep track of last rx from channel




   int  ServerInitNetwork(void);
   void ServerExitNetwork(void);
   void ServerListen(void);
   int  ServerReceive(void *data, int *sender);
   void ServerSendTo(void *data, int len, int who);
   void ServerFlush(void);

   int ServerFindUnusedChannel(void);

   int ServerGetNewDynamicPort(void);

   int server_get_player_num_from_who(int who);

   void headless_server_setup(void);

   void server_rewind(void);
   void server_create_new_state(void);
   void server_send_dif(int frame_num);
   void server_send_compressed_dif(int p, int src, int dst, char * dif);


   void server_proc_player_drop(void);
   void server_proc_limits(void);
   void server_reload(int level);


   void server_send_snfo_packet(void);
   void server_send_sjon_packet(int who, int level, int frame, int player_num, int player_color);
   void server_send_sjrc_packet(int who);

   void server_proc_ping_packet(char *data, int who);
   void server_proc_pang_packet(char *data, int who);
   void server_proc_cdat_packet(int i);
   void server_proc_stak_packet(int i);
   void server_proc_cjon_packet(int i);
   void server_proc_cjrc_packet(int i);
   void server_proc_rctl_packet(int i);
   void server_control();


   // --------------------------------------------------------------------
   // ---   mwNetgameSessionLog.cpp  -------------------------------------
   // --------------------------------------------------------------------
   void session_clear_entry(int i);
   int session_get_empty(void);
   int session_get_index_from_who(int who);

   void session_add_entry(const char* address, int who);
//   void session_update_entry(int who, int status, const char* hostname, int player_num);

   void session_close_entry_server_full(int who, const char* hostname);
   void session_update_entry_client_active(int p);


   void session_add_log(int i);
   void session_drop_player(int p);

   void session_check_active(void);

   void session_save_active_at_level_done(void);
   void session_flush_active_at_server_exit(void);

   void session_tally(int i);


   // --------------------------------------------------------------------
   // ---   mwNetgameServerFileTransfer.cpp  -----------------------------
   // --------------------------------------------------------------------
   void server_proc_files_to_send(void);
   void server_send_file(int i);
   void server_add_file_to_send(const char * filename, int who);
   void server_proc_crfl_packet(int i);
   void server_proc_sfak_packet(int i);





};
extern mwNetgame mNetgame;

#endif // mwNetgame_H

