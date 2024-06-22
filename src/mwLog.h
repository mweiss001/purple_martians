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

#define LOG_error                  9
#define LOG_NET                    10
#define LOG_NET_join_details       12
#define LOG_NET_session            14
#define LOG_NET_ending_stats       22
#define LOG_NET_bandwidth          23

#define LOG_NET_stdf_rewind        25
#define LOG_NET_stdf_create        26
#define LOG_NET_stdf               27
#define LOG_NET_stdf_packets       28

#define LOG_NET_dif_apply          30

#define LOG_NET_stak               33
#define LOG_NET_cdat               35
#define LOG_NET_client_ping        36
#define LOG_NET_timer_adjust       37

#define LOG_NET_file_transfer      39


#define LOG_TMR                    44
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


#define LOG_OTH_program_state      50
#define LOG_OTH_transitions        51
#define LOG_OTH_level_done         52
#define LOG_OTH_move               53
#define LOG_OTH_draw               54

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
   public:

   mwLog(); // default constructor

   int lp[8][2];

   struct log_type log_types[100];

   void init_log_types(void);

   void set_log_type_action(int type, int action);
   void clear_all_log_actions(void);

   void log_append_textf(int type, const char *format, ...);
   void log_append_text(int type, const char *txt);

   void log_add_prefixed_textf(int type, int player, const char *format, ...);
   void log_add_prefixed_text(int type, int player, const char *msg);

   void add_fwf(int type, int player, int width, int pos, const char *border, const char *fill, const char *format, ...);
   void add_fw(int type, int player, int width, int pos, const char *border, const char *fill, const char *txt);

   void add_headerf(int type, int player, int blank_lines, const char *format, ...);
   void add_header(int type, int player, int blank_lines, const char *txt);

   void log_time_date_stamp(void);
   void log_versions(void);

   ALLEGRO_FS_ENTRY *filenames[1000];
   int num_filenames;

   char log_msg[NUM_LOG_CHAR];
   int log_msg_pos = 0;
   char log_lines[NUM_LOG_LINES][100];  // for log file viewer
   int log_lines_int[NUM_LOG_LINES][3]; // for log file viewer


   void add_tmrf(int type, const char *format, ...);
   void add_tmr1(int type, const char *tag, double dt);
   void add_tmr(int type, const char *txt);


   int autosave_log_on_level_done = 0;
   int autosave_log_on_level_quit = 0;
   int autosave_log_on_program_exit = 0;

   void erase_log(void);
   void save_log_file(void);

   void log_player_array(int type);
   void log_player_array2(int type);
   void log_player_array3(int type);

   void log_ending_stats_client(int type, int p);
   void log_ending_stats_server(int type);

   void log_bandwidth_stats(int type, int p);
   void log_reason_for_player_quit(int type, int p);
   void add_log_TMR(double dt, const char *tag, int p);

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



