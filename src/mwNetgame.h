// mwNetgame.h

#define STATE_SIZE 112384
#define MAX_CLIENTS 32

struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int who;
   int packetsize;
   char data[1024];
};

class mwNetgame
{
   public:

   mwNetgame(); // constructor

   int NetworkDriver;
   int NetworkInit();

   struct packet_buffer packet_buffers[200];
   void init_packet_buffer(void);

   char packetbuffer[1024];
   int packetsize;
   int packetpos;
   void set_packetpos(int pos);
   int get_packetpos(void);
   void Packet(const char *id);
   int PacketRead(const char *id);
   void PacketAddByte(char b);
   char PacketGetByte(void);
   void PacketAddString(char*);
   void PacketReadString(char*);
   void PacketPut1ByteInt(int b);
   void PacketPut2ByteInt(int b);
   void PacketPut3ByteInt(int b);
   void PacketPut4ByteInt(int b);
   int PacketGet1ByteInt(void);
   int PacketGet2ByteInt(void);
   int PacketGet3ByteInt(void);
   int PacketGet4ByteInt(void);
   void PacketPutDouble(double);
   double PacketGetDouble(void);

   int ima_server = 0;
   int ima_client = 0;
   char m_serveraddress[256] = "192.168.1.2";
   int TCP = 0;
   int zlib_cmp = 7;

   double client_chase_offset = 0.0;
   double client_chase_offset_auto_offset = -0.02;
   int client_chase_offset_mode = 1; // 0 = manual, 1 = auto

   // server stdf frequency
   int server_state_freq = 2;
   int server_state_freq_mode = 1; // 0 = manual, 1 = auto


   // server's last 8 states for rewinding
   char srv_rewind_state[8][STATE_SIZE];
   int srv_rewind_state_frame_num[8];


   // server's copies of client states
   char srv_client_state[8][2][STATE_SIZE];
   int srv_client_state_frame_num[8][2];





   // local client's states
   char client_state_buffer[STATE_SIZE];  // buffer for building compressed dif from packet pieces
   int  client_state_buffer_pieces[16];   // to mark packet pieces as received
   char client_state_base[STATE_SIZE];    // last ack state
   int  client_state_base_frame_num;      // last ack state frame_num
   char client_state_dif[STATE_SIZE];     // uncompressed dif
   int  client_state_dif_src;             // uncompressed dif src frame_num
   int  client_state_dif_dst;             // uncompressed dif dst frame_num


   void game_vars_to_state(char * b);
   void state_to_game_vars(char * b);
   void get_state_dif(char *a, char *b, char *c, int size);
   void apply_state_dif(char *a, char *c, int size);
   void reset_states(void);
   void reset_client_state(int p); // server only



   // --------------------------------------------------------------------
   // ---   mwNetgameClient.cpp  -----------------------------------------
   // --------------------------------------------------------------------
   NET_CONN *ServerConn = NULL;
   NET_CHANNEL *ServerChannel = NULL;
   float tmaj_i;

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
   void client_process_sjon_packet(void);
   void client_read_game_move_from_packet(int x);
   void client_send_ping(void);
   void client_send_stak(void);
   void client_timer_adjust(void);
   void client_process_stdf_packet(double timestamp);
   void client_apply_dif();
   void client_block_until_initial_state_received(void);
   void client_process_sdat_packet(void);
   void client_process_serr_packet(void);
   void client_proc_player_drop(void);
   void client_control(void);
   void client_local_control(int p);
   void process_bandwidth_counters(int p);

   // --------------------------------------------------------------------
   // ---   mwNetgameServer.cpp  -----------------------------------------
   // --------------------------------------------------------------------
   int ClientNum;
   NET_CONN *ListenConn = NULL;                         // listening connection
   NET_CONN *ClientConn[MAX_CLIENTS] = {NULL, };        // array of connections for each client
   NET_CHANNEL *ListenChannel = NULL;                   // listen channel
   NET_CHANNEL *ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client

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


   int find_earliest_rewind_state(void);


   void show_rewind_states(const char *format, ...);


   void add_rewind_state(void);
   void set_rewind_state(void);


   void server_rewind(void);
   void server_send_sdat(void);
   void server_proc_player_drop(void);
   void server_proc_cdat_packet(double timestamp);
   void server_lock_client(int p);

   void server_proc_stak_packet(double timestamp);
   void server_proc_cjon_packet(int who);
   void server_control();
   int get_player_num_from_who(int who);
   void server_fast_packet_loop(void);
   void server_read_packet_buffer(void);

};
extern mwNetgame mNetgame;
