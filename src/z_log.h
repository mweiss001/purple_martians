// z_log.h


// ------------------------------------------------
// ----------------- logging ----------------------
// ------------------------------------------------


//#define NUM_LOG_CHAR  100000000
//#define NUM_LOG_LINES 2000000

// this is too much man! causes weird issues
//#define NUM_LOG_CHAR  500000000
//#define NUM_LOG_LINES 5000000

#define NUM_LOG_CHAR  100000000
#define NUM_LOG_LINES 1000000


extern char log_msg[NUM_LOG_CHAR]; // for logging
extern int log_msg_pos;
extern char log_lines[NUM_LOG_LINES][100]; // for log file viewer
extern int log_lines_int[NUM_LOG_LINES][3]; // for log file viewer


extern int LOG_NET;
extern int LOG_NET_join;
extern int LOG_NET_player_array;
extern int LOG_NET_bandwidth;
extern int LOG_NET_cdat;
extern int LOG_NET_stdf;
extern int LOG_NET_stdf_all_packets;

extern int LOG_NET_dif_applied;
extern int LOG_NET_dif_not_applied;

extern int LOG_NET_client_ping;
extern int LOG_NET_client_timer_adj;
extern int LOG_NET_server_rx_stak;













extern int LOG_TMR_cpu;
extern int LOG_TMR_move_tot;
extern int LOG_TMR_move_all;
extern int LOG_TMR_move_enem;

extern int LOG_TMR_bmsg_add;
extern int LOG_TMR_bmsg_draw;


extern int LOG_TMR_scrn_overlay;


extern int LOG_TMR_draw_tot;
extern int LOG_TMR_draw_all;
extern int LOG_TMR_sdif;
extern int LOG_TMR_cdif;
extern int LOG_TMR_rwnd;


extern int LOG_TMR_client_timer_adj;
extern int LOG_TMR_client_ping;



extern int autosave_log_on_program_exit;
extern int autosave_log_on_game_exit;
extern int autosave_log_on_level_done;

extern int autosave_game_on_game_exit;
extern int autosave_game_on_level_done;


void log_bandwidth_stats(int p);
void log_reason_for_player_quit(int p);
void add_log_TMR(double dt, const char *tag, int p);
void log_time_date_stamp(void);
void log_versions(void);
void log_player_array(void);
void log_player_array2(void);
void log_ending_stats(int p);
void log_ending_stats_server(void);
void erase_log(void);
void save_log_file(void);
void add_log_entry_sdat_rx_and_game_move_entered(int type, int player);
void add_log_entry2(int type, int player, const char *txt);
void add_log_entry_position_text(int type, int player, int width, int pos, const char *txt, const char *border, const char *fill);
void add_log_entry_centered_text(int type, int player, int width, const char *txt, const char *border, const char *fill);
void add_log_entry_header(int type, int player, const char *txt, int blank_lines);
int fill_filename_array(ALLEGRO_FS_ENTRY *fs, void * extra);
int log_file_viewer(int type);


void load_profile_graph(int choose);



int load_log_lines_array_from_static_file(const char* f);

void log_ping_graph(int num_lines);
void log_client_server_sync_graph(int num_lines);
void log_bandwidth_graph(int num_lines, int both);
void show_code_stats(void);
