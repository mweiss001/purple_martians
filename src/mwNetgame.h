// mwNetgame.h

#ifndef mwNetgame_H
#define mwNetgame_H

#include "mwStateHistory.h"

#define PM_RCTL_PACKET_TYPE_keep_alive             0
#define PM_RCTL_PACKET_TYPE_force_client_offset    1
#define PM_RCTL_PACKET_TYPE_client_offset_adj      2
#define PM_RCTL_PACKET_TYPE_zlib_compression_adj   3
#define PM_RCTL_PACKET_TYPE_extra_packet_num_adj   4
#define PM_RCTL_PACKET_TYPE_extra_packet_siz_adj   5
#define PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj    6

#define PM_RCTL_PACKET_TYPE_pvp_shots_toggle       10
#define PM_RCTL_PACKET_TYPE_pvs_shots_toggle       11
#define PM_RCTL_PACKET_TYPE_fakekey_toggle         12
#define PM_RCTL_PACKET_TYPE_server_reload          20

struct client_session
{
   int active;

   double start_time;
   double end_time;
   char session_name[64];

   char timestamp[16];
   double duration;
   char ip[16];
   int port;
   char hostname[16];

   int endreason;

   int cdats_rx;
   int player_num;
   int next_levels;
   int exits;
   int respawns;
   int shots_fired;
   int enemy_hits;
   int player_hits;
   int self_hits;
   int purple_coins;

   int64_t tx_total_bytes;
   int64_t tx_total_packets;
   int tx_max_bytes_per_frame;
   int tx_max_packets_per_frame;

   int64_t rx_total_bytes;
   int64_t rx_total_packets;
   int rx_max_bytes_per_frame;
   int rx_max_packets_per_frame;

};


struct mwChannel
{
   int channel_active;
   char channel_address[256];
};


struct file_to_send
{
   int active;
   int attempts;
   int last_sent_frame;
   int id;
   int p;
   char name[256];
};


class mwNetgame
{
   public:

   mwNetgame(); // constructor

   struct mwChannel mwChannels[8];

   struct file_to_send files_to_send[20];

   struct client_session client_sessions[8];

   int NetworkDriver;
   int NetworkInit(void);
   void NetworkExit(void);



   void clear_channel(int c);

   mwStateHistory mStateHistory[8];

   // debug testing of sending more packets
   int srv_exp_num = 0;
   int srv_exp_siz = 0;

   int ima_server = 0;
   int ima_client = 0;

   char server_address[256] = "purplemartians.org";
   int server_port = 24785;

   int zlib_cmp = 7;

   float client_chase_offset;
   float client_chase_offset_auto_offset;
   int client_chase_offset_mode; // 0 = manual, 1 = auto, 2 = server

   int server_dirty_frame = 0;

   int server_lev_seq_num = 0;


   int server_num_clients = 0;



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

   NET_CHANNEL *Channel = NULL;                   // main channel

   void ChannelFlush(void);

   // --------------------------------------------------------------------
   // ---   mwNetgameClient.cpp  -----------------------------------------
   // --------------------------------------------------------------------

   int ClientInitNetwork(void);
   int ClientJoin(void);
   int RemoteJoin(void);

   void ClientExitNetwork(void);
   int  ClientCheckResponse(void);
   void ClientSend(void *data, int len);

   void client_timer_adjust(void);
   void client_apply_dif();

   void client_send_ping_packet(void);
   void client_send_cjon_packet(void);
   void client_send_cjrc_packet(void);
   void client_send_rctl_packet(int type, double val);
   void client_send_stak_packet(int ack_frame);
   void client_send_cdat_packet(int p);
   void client_send_sfak_packet(int id);
   void client_send_crfl(void);

   void client_send_clog_packet(int type, int sub_type, int f, double agt, char* smsg);


   void client_proc_pong_packet(char *data);
   int  client_proc_sjon_packet(char *data);
   void client_proc_srrf_packet(int i);
   void client_proc_stdf_packet(int i);
   void client_proc_sfil_packet(int i);
   void client_proc_snfo_packet(int i);

   void client_proc_player_drop(void);
   void client_control(void);

   // --------------------------------------------------------------------
   // ---   mwNetgameServer.cpp  -----------------------------------------
   // --------------------------------------------------------------------

   int  ServerInitNetwork(void);
   void ServerExitNetwork(void);
   void ServerSendTo(void *data, int len, int p);

   int ServerGetNewDynamicPort(void);

   int server_check_address(char * address);

   void headless_server_setup(void);

   void server_rewind(void);
   void server_create_new_state(void);
   void server_send_dif(int frame_num);
   void server_send_compressed_dif(int p, int src, int dst, char * dif);


   void server_proc_player_drop(void);
   void server_proc_limits(void);
   void server_reload(int level);


   void server_send_snfo_packet(void);
   void server_send_sjon_packet(char* address, int level, int frame, int player_num, int player_color);
   void server_send_sjrc_packet(int p);


   void server_proc_ping_packet(char *data, int p);
   void server_proc_pang_packet(char *data, int p);

   void server_proc_cdat_packet(int i);
   void server_proc_stak_packet(int i);


   void server_proc_cjon_packet(char* data, char* address);
   void server_proc_cjrc_packet(char* data, char* address);

   void server_proc_clog_packet(int i);


   void server_proc_rctl_packet(int i);

   void server_count_clients();

   void server_control();


   // --------------------------------------------------------------------
   // ---   mwNetgameSessionLog.cpp  -------------------------------------
   // --------------------------------------------------------------------
   void session_clear_entry(int i);

   void session_add_entry(const char* address, const char* hostname, int p, int active, int inactive_reason);


   void session_add_log(int i);
   void session_close(int p, int reason);

   void session_check_active(void);

   void session_save_active_at_level_done(void);
   void session_flush_active_at_server_exit(void);

   void session_tally(int i);


   // --------------------------------------------------------------------
   // ---   mwNetgameServerFileTransfer.cpp  -----------------------------
   // --------------------------------------------------------------------
   void server_proc_files_to_send(void);
   void server_send_file(int i);
   void server_add_file_to_send(const char * filename, int p);
   void server_proc_crfl_packet(int i);
   void server_proc_sfak_packet(int i);

   void server_send_srrf_packet(int p, int val);



};
extern mwNetgame mNetgame;

#endif // mwNetgame_H

