// mwLog.h
#ifndef mwLog_H
#define mwLog_H


//#define NUM_LOG_CHAR  100000000
//#define NUM_LOG_LINES 2000000

// this is too much man! causes weird issues
//#define NUM_LOG_CHAR  500000000
//#define NUM_LOG_LINES 5000000

#define NUM_LOG_CHAR  100000000
#define NUM_LOG_LINES 1000000


#define LOG_error                         9

#define LOG_NET                          10
#define LOG_NET_DIF_REWIND               20
#define LOG_NET_DIF_CREATE               21
#define LOG_NET_DIF_TRX                  22
#define LOG_NET_DIF_TRX_PACKET           23
#define LOG_NET_DIF_APPLY                24
#define LOG_NET_DIF_ACK                  25
#define LOG_NET_CDAT                     30
#define LOG_NET_TIMER_ADJUST             31
#define LOG_NET_CLIENT_PING              32
#define LOG_NET_FILE_TRANSFER            33
#define LOG_NET_ENDING_STATS             34
#define LOG_NET_BANDWIDTH                35
#define LOG_NET_SESSION                  37
#define LOG_NET_CSV                      38

#define LOG_NET_SUBTYPE_PLAYER_ACTIVE         1
#define LOG_NET_SUBTYPE_PLAYER_INACTIVE       2
#define LOG_NET_SUBTYPE_PLAYER_DIED           4
#define LOG_NET_SUBTYPE_NEXT_LEVEL            20
#define LOG_NET_SUBTYPE_LEVEL_STARTED         21
#define LOG_NET_SUBTYPE_SERVER_START          31
#define LOG_NET_SUBTYPE_SERVER_STOP           32
#define LOG_NET_SUBTYPE_SERVER_CJON           33
#define LOG_NET_SUBTYPE_CLIENT_START          40
#define LOG_NET_SUBTYPE_CLIENT_INIT           41
#define LOG_NET_SUBTYPE_CLIENT_CJON           42
#define LOG_NET_SUBTYPE_CLIENT_WAIT           43
#define LOG_NET_SUBTYPE_CLIENT_SJON           44
#define LOG_NET_SUBTYPE_CLIENT_STOP           45
#define LOG_NET_SUBTYPE_SERVER_CJRC           82
#define LOG_NET_SUBTYPE_SERVER_DROP           86
#define LOG_NET_SUBTYPE_CLIENT_DROP           88
#define LOG_NET_SUBTYPE_SERVER_RELOAD         90
#define LOG_NET_SUBTYPE_PACKET_BUF_FULL       94


#define LOG_OTH_PROGRAM_STATE      50
#define LOG_OTH_TRANSITIONS        51
#define LOG_OTH_LEVEL_DONE         52


#define LOG_TMR                    60
#define LOG_TMR_cpu                70
#define LOG_TMR_rebuild_bitmaps    72
#define LOG_TMR_move_tot           74
#define LOG_TMR_move_all           75
#define LOG_TMR_move_enem          76


#define LOG_TMR_draw_tot           80
#define LOG_TMR_draw_all           81
#define LOG_TMR_bmsg_add           84
#define LOG_TMR_bmsg_draw          85
#define LOG_TMR_scrn_overlay       87
#define LOG_TMR_sdif               90
#define LOG_TMR_cdif               91
#define LOG_TMR_rwnd               92
#define LOG_TMR_client_timer_adj   95
#define LOG_TMR_client_ping        96
#define LOG_TMR_proc_rx_buffer     97


#define LOG_ACTION_PRINT  0b001
#define LOG_ACTION_LOG    0b010

struct log_type
{
   int group; // 0=unused, 1=net, 2=timer, 3=other
   int action; // 001=print_to_console, 010=log_to_file
   char name[40];
};

class mwLog
{

private:

   void log_player_array(int type);
   void log_versions();
   void log_append_textf(int type, const char *format, ...);
   void log_append_text(int type, const char *txt);
   void log_add_prefixed_textf(int type, int player, const char *format, ...);
   void log_add_prefixed_text(int type, int player, const char *msg);
   void log_append_text_to_db(int type, const char *txt);
   void add_fwf(int type, int player, int width, int pos, const char *border, const char *fill, const char *format, ...);
   void add_fw(int type, int player, int width, int pos, const char *border, const char *fill, const char *txt);
   void add_headerf(int type, int player, int blank_lines, const char *format, ...);
   void add_header(int type, int player, int blank_lines, const char *txt);
   void log_time_date_stamp();

   char log_status_msg[NUM_LOG_CHAR];
   int log_status_msg_pos = 0;
   int log_status_msg_num_lines = 0;

   char log_net_msg[NUM_LOG_CHAR];
   int log_net_msg_pos = 0;
   int log_net_msg_num_lines = 0;

   int lp[8][2];

   ALLEGRO_FS_ENTRY *filenames[1000];
   int num_filenames;


   void erase_log();
   void erase_log_net();
   void erase_log_status();

   void save_log_file();
   void save_log_net_file();
   void save_log_status_file();

   void log_ending_stats_client(int type, int p);
   void log_ending_stats_server(int type);
   void log_bandwidth_stats(int type, int p);
   void log_reason_for_player_quit(int type, int p);


   char log_lines[NUM_LOG_LINES][100];  // for log file viewer
   int log_lines_int[NUM_LOG_LINES][3]; // for log file viewer

   char log_msg[NUM_LOG_CHAR];
   int log_msg_pos = 0;

public:

   mwLog(); // default constructor

   struct log_type log_types[100];

   void init_log_types();
   void clear_all_log_actions();
   void flush_logs();

   void log_error(const char *txt, bool dialog = 1);


   void add_log_status_db_rows();
   void add_log_net_db_row(int type, int sub_type, int client, const char *format, ...);
   void add_log_net_db_row2(int type, int sub_type, double agt, int frame, int player, int client, const char* msg);

   void add(int log_type, int sub_type, int p, float v0=0, float v1=0, float v2=0, float v3=0, float v4=0, float v5=0, float v6=0, float v7=0, float v8=0, float v9=0, const char* t1="", const char* t2="");

   void add_tmrf(int type, const char *format, ...);
   void add_tmr1(int type, const char *tag, double dt);
   void add_tmr(int type, const char *txt);

   int autosave_log_on_level_done = 0;
   int autosave_log_on_level_quit = 0;
   int autosave_log_on_program_exit = 0;


   // in mwLogGraph.cpp
   void run_profile_graph(int choose);
   int load_profile_graph(int choose);

   void run_ping_graph(int num_lines);
   int load_ping_graph(int num_lines);

   void run_client_server_sync_graph(int num_lines);
   int load_client_server_sync_graph(int num_lines);

   void run_bandwidth_graph(int num_lines, int both);
   int load_bandwidth_graph(int num_lines, int both);

   // in mwLogViewer.cpp
   int load_log_lines_array_from_static_file(const char* f);
   int log_file_viewer(int type);


};
extern mwLog mLog;

#endif // mwLog_H



