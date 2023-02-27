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

class mwLog
{
   private:
   int lp[8][2];

   public:

   ALLEGRO_FS_ENTRY *filenames[1000];
   int num_filenames;

   char log_msg[NUM_LOG_CHAR];
   int log_msg_pos = 0;
   char log_lines[NUM_LOG_LINES][100];  // for log file viewer
   int log_lines_int[NUM_LOG_LINES][3]; // for log file viewer


   int LOG_NET = 0;
   int LOG_NET_join = 0;
   int LOG_NET_player_array = 0;
   int LOG_NET_bandwidth = 0;
   int LOG_NET_cdat = 0;
   int LOG_NET_stdf = 0;
   int LOG_NET_stdf_all_packets = 0;

   int LOG_NET_dif_applied = 0;
   int LOG_NET_dif_not_applied = 0;

   int LOG_NET_client_ping = 0;
   int LOG_NET_client_timer_adj = 0;
   int LOG_NET_server_rx_stak = 0;

   int LOG_TMR_cpu = 0;
   int LOG_TMR_move_tot = 0;
   int LOG_TMR_move_all = 0;
   int LOG_TMR_move_enem = 0;

   int LOG_TMR_bmsg_add = 0;
   int LOG_TMR_bmsg_draw = 0;
   int LOG_TMR_scrn_overlay = 0;

   int LOG_TMR_draw_tot = 0;
   int LOG_TMR_draw_all = 0;
   int LOG_TMR_sdif = 0;
   int LOG_TMR_cdif = 0;
   int LOG_TMR_rwnd = 0;

   int LOG_TMR_client_timer_adj = 0;
   int LOG_TMR_client_ping = 0;

   int autosave_log_on_program_exit = 0;
   int autosave_log_on_game_exit = 0;
   int autosave_log_on_level_done = 0;
   int autosave_game_on_game_exit = 0;
   int autosave_game_on_level_done = 0;


   void erase_log(void);
   void save_log_file(void);
   void add_log_entry2(int type, int player, const char *txt);
   void add_log_entry_position_text(int type, int player, int width, int pos, const char *txt, const char *border, const char *fill);
   void add_log_entry_centered_text(int type, int player, int width, const char *txt, const char *border, const char *fill);
   void add_log_entry_header(int type, int player, const char *txt, int blank_lines);

   void log_bandwidth_stats(int p);
   void log_reason_for_player_quit(int p);
   void add_log_TMR(double dt, const char *tag, int p);
   void log_time_date_stamp(void);
   void log_versions(void);
   void log_player_array(void);
   void log_player_array2(void);
   void log_ending_stats(int p);
   void log_ending_stats_server();

   // in mwLogGraph.cpp
   void run_profile_graph(int choose);
   int load_profile_graph(int choose);

   void run_ping_graph(int num_lines);
   int load_ping_graph(int num_lines);

   void run_client_server_sync_graph(int num_lines);
   int load_client_server_sync_graph(int num_lines);

   void run_bandwidth_graph(int num_lines, int both);
   int load_bandwidth_graph(int num_lines, int both);

   // not used
   void run_timestamp_graph(void);
   void log_timestamp_graph(int num_lines);


   // in mwLogViewer.cpp
   int load_log_lines_array_from_static_file(const char* f);
   int log_file_viewer(int type);


};
extern mwLog mLog;

#endif // mwLog_H



