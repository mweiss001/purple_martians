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

class mwNetgame
{
   public:

   mwNetgame(); // constructor

   int NetworkDriver;
   int NetworkInit();

   mwStateHistory mStateHistory[8];

   // debug testing of sending more packets
   int srv_exp_num = 0;
   int srv_exp_siz = 0;

   int ima_server = 0;
   int ima_client = 0;
   int remote_join_reply = 0;

   char serveraddress[256] = "192.168.1.2";
   int zlib_cmp = 7;

   float client_chase_offset;
   float client_chase_offset_auto_offset;
   int client_chase_offset_mode; // 0 = manual, 1 = auto, 2 = server

   int server_dirty_frame = 0;

   // local client's buffer for building compressed dif from packets
   char client_state_buffer[STATE_SIZE];
   int  client_state_buffer_pieces[16];   // to mark packet pieces as received

   // local client's uncompressed dif
   char client_state_dif[STATE_SIZE];
   int  client_state_dif_src;          // src frame_num
   int  client_state_dif_dst;          // dst frame_num

   void game_vars_to_state(char * b);
   void state_to_game_vars(char * b);
   void get_state_dif(char *a, char *b, char *c, int size);
   void apply_state_dif(char *a, char *c, int size);
   void reset_states(void);

   // --------------------------------------------------------------------
   // ---   mwNetgameClient.cpp  -----------------------------------------
   // --------------------------------------------------------------------
   NET_CONN *ServerConn = NULL;
   NET_CHANNEL *ServerChannel = NULL;


   int  ClientInitNetwork(void);
   void ClientExitNetwork(void);
   int  ClientCheckResponse(void);
   int  ClientReceive(void *data);
   void ClientSend(void *data, int len);
   void ClientFlush(void);








   void client_read_game_move_from_packet(int x);
   void client_timer_adjust(void);
   void client_apply_dif();
   void client_block_until_initial_state_received(void);


   void client_send_ping_packet(void);
   void client_send_cjon_packet(void);
   void client_send_cjrc_packet(void);
   void client_send_rctl_packet(int type, double val);
   void client_send_stak_packet(int ack_frame);
   void client_send_cdat_packet(int p);

   void client_proc_pong_packet(char *data);
   void client_proc_sjon_packet(int i);
   void client_proc_stdf_packet(int i);
   void client_proc_sjrc_packet(int i);
   void client_proc_snfo_packet(int i);



   void client_proc_player_drop(void);



   void client_control(void);
   void client_local_control(int p);
   void process_bandwidth_counters(int p);



   float tmaj_i;




   // --------------------------------------------------------------------
   // ---   mwNetgameServer.cpp  -----------------------------------------
   // --------------------------------------------------------------------
   int ClientNum;
   NET_CONN *ListenConn = NULL;                         // listening connection
   NET_CONN *ClientConn[MAX_CLIENTS] = {NULL, };        // array of connections for each client
   NET_CHANNEL *ListenChannel = NULL;                   // listen channel
   NET_CHANNEL *ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client


   int  ServerInitNetwork(void);
   void ServerExitNetwork(void);
   void ServerListen(void);
   int  ServerReceive(void *data, int *sender);
   void ServerBroadcast(void *data, int len);
   void ServerSendTo(void *data, int len, int who);
   void ServerFlush(void);



   void headless_server_setup(void);

   void server_reload(int level);



   void server_send_compressed_dif(int p, int src, int dst, char * dif);
   void server_send_dif(int frame_num);
   void server_create_new_state(void);
   void show_rewind_states(const char *format, ...);
   void server_rewind(void);
   void server_proc_player_drop(void);
   void server_lock_client(int p);


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
   int server_get_player_num_from_who(int who);

};
extern mwNetgame mNetgame;


#endif // mwNetgame_H

