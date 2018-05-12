// pm.h - header file for both the game and the level editor

//#define VERSION_HACK

#include <stdio.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


void options_menu(void);

void rebuild_bitmaps(void);
void set_ortho(void);

int log_file_viewer(int);


void temp_test(void);
void spline_test(void);

void get_config_values(void);
void splash_screen(void);
void splash_toggle(void);

void draw_large_2lines(ALLEGRO_FONT *tf, char * m1, char * m2, int color, float opa );

void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, char * txt);


extern int num_enemy;


void draw_speed_test_data(void);
void show_player_join_quit(void);

extern int KEY_1_b;
extern int KEY_2_b;
extern int KEY_3_b;
extern int KEY_4_b;
extern int KEY_5_b;
extern int KEY_6_b;
extern int KEY_7_b;
extern int KEY_8_b;
extern int KEY_9_b;

extern int WX, WY;



int mw_file_select(const char * title, char * filename, const char * ext, int sve);

void check_s_window_pos(int);

extern ALLEGRO_TIMER * fps_timer;
extern ALLEGRO_TIMER * sec_timer;
extern ALLEGRO_TIMER * mnu_timer;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_COLOR palette_color[256];
extern ALLEGRO_FONT *font;
extern ALLEGRO_FONT *f1;
extern ALLEGRO_FONT *f2;
extern ALLEGRO_FONT *f3;
extern ALLEGRO_JOYSTICK *joy0;
extern ALLEGRO_JOYSTICK *joy1;


extern ALLEGRO_VOICE *voice;
extern ALLEGRO_MIXER *mn_mixer;
extern ALLEGRO_MIXER *se_mixer;
extern ALLEGRO_MIXER *st_mixer;

extern ALLEGRO_SAMPLE *snd[20];
extern int num_sounds;
extern ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
extern ALLEGRO_AUDIO_STREAM *pm_theme_stream;


extern ALLEGRO_FS_ENTRY *filenames[1000];
extern int num_filenames;


extern ALLEGRO_BITMAP *logo_ichael;
extern ALLEGRO_BITMAP *logo_avid;
extern ALLEGRO_BITMAP *logo_eiss;
extern int logo_text_bitmaps_create;

extern ALLEGRO_BITMAP *text_demo;
extern ALLEGRO_BITMAP *text_mode;
extern int text_demomode_bitmaps_create;
void draw_demo_mode_overlay(void);

extern ALLEGRO_BITMAP *text_title;
extern int text_title_bitmaps_create;
extern int text_title_draw_color;
void draw_title(int tx, int ty, int tw, int th, int color);


extern int level_display_region_x;
extern int level_display_region_y;
extern int level_display_region_w;
extern int level_display_region_h;

extern char skc[64];
extern int skc_index;


extern int les; // level editor scale

extern int level_editor_running;
extern int help_screens_running;

void log_ending_stats_server();
void log_ending_stats();
void log_player_array();


void load_fonts(void);


void window_title(void);


extern int disp_x_curr; //either wind in windowed mode or 0 in fullscreen mode)
extern int disp_y_curr;

extern int disp_x_wind; // use when restoring from fullscreen
extern int disp_y_wind;

extern int disp_x_full;
extern int disp_y_full;

extern int disp_w_curr; //(either wind or full)
extern int disp_h_curr;

extern int disp_w_wind;
extern int disp_h_wind;

extern int disp_w_full;
extern int disp_h_full;

extern int l_spx, l_spy, fullscreen;

extern int SCREEN_W, SCREEN_H;


void proc_screen_change(int new_sw, int new_sh, int new_sx, int new_sy, int new_fs);

void show_resize_msg(void);

void m_err(char * err_msg);

extern int visual_level_select_running;

#define SOUND
#ifdef SOUND
void set_se_scaler(void);
void set_st_scaler(void);
#endif


void show_all_enemies(void);
void show_all_pmsg(void);
void show_all_items(void);
void show_all_lifts(void);


void stop_sound(void);


extern int fuse_loop_playing;
extern int phit_loop_playing;
extern int sample_delay[8];




extern bool key[ALLEGRO_KEY_MAX];

extern int Key_pressed_ASCII;


extern int mouse_x;
extern int mouse_y;
extern int mouse_z;
extern int mouse_dx;
extern int mouse_dy;
extern int mouse_dz;
extern int mouse_b1;
extern int mouse_b2;
extern int mouse_b3;
extern int mouse_b4;




extern int actual_fps;
extern int frames_skipped_last_second;
extern int last_frames_skipped;
extern int last_fps_passcount;


void save_config(void);




#define CHUNK_SIZE 104640


// server chdf
extern char client_chdf[8][2][CHUNK_SIZE];
extern int client_chdf_id[8][2]; // passcount id

// client chdf
extern char chdf[CHUNK_SIZE];          // for client chdf building
extern int chdf_pieces[16];
extern char clientl_chdf[CHUNK_SIZE];  // last ack state for diffing
extern int clientl_chdf_id;            // passcount id

extern char dif[CHUNK_SIZE];
extern int dif_id[2]; //   (0 = src, 1 = dst)

extern int chdf_freq;
extern int control_lead_frames;
extern int server_lead_frames;
extern int zlib_cmp;

extern int deathmatch_pbullets;
extern int deathmatch_pbullets_damage;
extern int suicide_pbullets;


void reset_states(void);
void chnk_to_state(char * b);
void state_to_chunk(char * b);
void show_chunk_dif(char * a, char * b);
void get_chunk_dif(char *a, char *b, char *c, int size);
void apply_chunk_dif(char *a, char *c, int size);





extern float mouse_loop_pause;

extern char local_hostname[80];
extern char version_string[80];

// #define CLONERLINES

// #define RELEASE


void fast_exit(int why);

#define NETPLAY

#ifdef NETPLAY
void client_control(void);
void server_control(void);

void client_local_control(int);
void server_local_control(int);

int client_init(void);
void client_exit(void);

int server_init(void);
void server_exit(void);

void client_flush(void);
void server_flush(void);

#endif


#define CONNECTION // tcp
#define CHANNEL    // udp
extern int TCP;


extern int L_LOGGING;
extern int L_LOGGING_NETPLAY;
extern int L_LOGGING_NETPLAY_JOIN;
extern int L_LOGGING_NETPLAY_bandwidth;
extern int L_LOGGING_NETPLAY_client_timer_adjust;
extern int L_LOGGING_NETPLAY_cdat;
extern int L_LOGGING_NETPLAY_game_move;
extern int L_LOGGING_NETPLAY_sdat;
extern int L_LOGGING_NETPLAY_sdak;
extern int L_LOGGING_NETPLAY_chdf;
extern int L_LOGGING_NETPLAY_chdf_all_packets;
extern int L_LOGGING_NETPLAY_chdf_when_to_apply;
extern int L_LOGGING_NETPLAY_show_dif1;
extern int L_LOGGING_NETPLAY_show_dif2;


#define NETPLAY_bandwidth_tracking

#define LOGGING
#define LOGGING_NETPLAY
#define LOGGING_NETPLAY_JOIN
#define LOGGING_NETPLAY_bandwidth
#define LOGGING_NETPLAY_client_timer_adjust
#define LOGGING_NETPLAY_cdat
#define LOGGING_NETPLAY_game_move
#define LOGGING_NETPLAY_sdat
#define LOGGING_NETPLAY_sdak
#define LOGGING_NETPLAY_chdf
#define LOGGING_NETPLAY_chdf_all_packets
#define LOGGING_NETPLAY_chdf_when_to_apply
#define LOGGING_NETPLAY_show_dif1
#define LOGGING_NETPLAY_show_dif2


// #define LOGGING_ZFS // zoom full screen



#define MAP_LOCK_KEY ALLEGRO_KEY_Z


#define NUM_SPRITES 1024
#define NUM_ANS 256
#define NUM_PLAYERS 8
#define BORDER_WIDTH 14

#define NUM_LIFTS 40



extern ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
extern int demo_played[100];
extern int num_demo_filenames;
extern int demo_mode_on;
extern int demo_mode_countdown;
void demo_mode(void);


extern int resume_allowed;
extern int start_mode;
extern int game_exit;



struct screen_msg
{
   int active;
   int delay;
   int original_x;
   int original_y;

   int x;
   int y;
   char text[80];

   int color;
   int color_inc;

   int color_inc_holdoff;
   int current_holdoff;

//    if color_inc = 0 no proc done; else
// countdown current_holdoff till 0; then reload and apply color_inc

   int ssn; //type of msg; used to select processing and initalization

   int current_step; // used by certain ssn's for multi-mode

   float size;
   float size_inc;

   float xinc; // doesn't do much good if x and y are ints!!
   float yinc;

   int rot;  /// (0-255)
   int rot_inc;

};
extern struct screen_msg screen_msgs[100];



extern int active_local_player;

struct player
{
   int active;  // all routines that process player will skip inactive
   int paused;
   int paused_type; // 1 = death, 2 = door move
   int paused_mode;
   int paused_mode_count;


   al_fixed PX, PY;       // players position
   al_fixed xinc, yinc;   // players momentum
   al_fixed right_xinc, left_xinc;

   al_fixed LIFE;
   al_fixed old_LIFE;
   int LIVES;

   int bitmap_index; // index to player_bitmap (player_bitmap[player.color][player.shape])
   int shape; // index to player_bitmap
   int color; // used to draw frames and stuff in players color


   int   door_draw_rot_num_steps; //20

   al_fixed door_draw_rot;
   al_fixed door_draw_rot_inc;
   al_fixed draw_rot;
   al_fixed draw_scale;

   int marked_door;
   int door_item;
   int door_xinc;
   int door_yinc;
   int door_num_steps;


   int left_right; // determines the direction the player is facing

   int player_ride, carry_item; //30

   // flags that indicate a control has been activated or held
   int up, down, left, right, jump, fire, fire_held, menu;

   // bullet stuff
   int bullet_wait_counter, request_bullet, bullet_wait, bullet_speed, num_bullets;

/*
   int num_bullets_fired;
   int num_bullets_hit_enemy; // num of times enemy hit with this player's bullet
   int num_bullets_hit_block; // num of times block hit by  this player's bullet
   int num_bullets_hit_other_player; // num of time other players hit with this players bullet
   int num_bullets_hit_themselves; // num of times this players hit themselves with bullets

   int num_enemy_hits;        // the number of times an enemy has hit this player
   int num_enemy_bullet_hits; // the number of times an emeny bullet has hit this player

   al_fixed LIFE_inc;
   al_fixed LIFE_dec;
*/

   int num_hits; // when players bullet hits enemy

//   int health_display_old; //45 moved to player1, does not need to be synced, left this here as a spacer
// converted to control_method
   int control_method; // 0 = local, 1 = file play, 2 = remote view; 3 = server_local; 4 = client_local


};
extern struct player players[NUM_PLAYERS];

struct player1
{
   // everything here will not be synced between server and client

   int health_display;
   int last_health_adjust;


   int up_key, down_key, left_key, right_key, jump_key, fire_key, menu_key;

   int comp_move, old_comp_move;

   // position and size of players on screen buffer (used to see if map is covering player)
   int sbx1,sby1,sbx2,sby2;


   // these are netgame and will be different from client to server

   int old_control_method; // 0 = local, 1 = file play, 2 = remote view; 3 = server_local; 4 = client_local

   int game_move_entry_pos;         // server only for client game_move data sync

   int server_last_sdat_sent_frame; // only server uses it, to keep track of when last sdat was sent to client
   int server_last_sdat_sent_start;
   int server_last_sdat_sent_num;

   int last_sdak_rx; // used by server to see if client is still alive


   int last_sdat_fpc;
   int last_sdat_lpc;



   int who; // for network id of clients

   char hostname[16];


   int server_sync;
   int sync_good_frames;

   int c_sync;
   int c_sync_min;
   int c_sync_err;



/*
   client_statistic[0] = 0; // sdat total
   client_statistic[1] = 0; // sdat skipped
   client_statistic[4] = 0;   // moves entered
   client_statistic[5] = 0;   // moves skipped
*/


   int cdat_packets_tx;
   int sdat_total;
   int sdat_skipped;
   int moves_entered;
   int moves_skipped;
   int moves_skipped_tally;
   int moves_skipped_last_tally;



   int serr_c_sync_err;
   int serr_display_timer;

   int fake_keypress_mode;


   int dif_corr;

   int made_active_holdoff;

   int join_chdf_sent;

   int frames_skipped;

   // client
   int num_dif_packets;
   int cmp_dif_size;







// common to server and client
   int join_frame;
   int quit_frame;
   int quit_reason;


   //server only (player 0)


   // next client to send chdf to
   int n_chdf;



   // used to add up until frame end
   int tx_current_bytes_for_this_frame;
   int tx_current_packets_for_this_frame;
   int tx_max_bytes_per_frame;
   int tx_max_packets_per_frame;
   int rx_current_bytes_for_this_frame;
   int rx_current_packets_for_this_frame;
   int rx_max_bytes_per_frame;
   int rx_max_packets_per_frame;


   // used to add up until tally end
   int tx_packets_tally;
   int tx_bytes_tally;
   int tx_max_packets_per_tally;
   int tx_max_bytes_per_tally;
   int rx_packets_tally;
   int rx_bytes_tally;
   int rx_max_packets_per_tally;
   int rx_max_bytes_per_tally;

   // used for display
   int tx_packets_per_tally;
   int tx_bytes_per_tally;
   int rx_packets_per_tally;
   int rx_bytes_per_tally;

   // grand totals
   int tx_total_bytes;
   int tx_total_packets;
   int rx_total_bytes;
   int rx_total_packets;













   int chdf_rx;
   int chdf_on_time;
   int chdf_late;















   // client only
   int client_sync_good;








};
extern struct player1 players1[NUM_PLAYERS];


struct lift
   {
      al_fixed fx;
      al_fixed fy;
      al_fixed fxinc;
      al_fixed fyinc;
      int x1;
      int y1;
      int x2;
      int y2;
      int width;
      int height;
      int color;
      int current_step;
      int num_steps;
      int limit_counter;
      int limit_type;
      char lift_name[40];
   };
extern struct lift lifts[NUM_LIFTS];


struct lift_step
   {
      int type;
      int val;
      int x;
      int y;
   };
extern struct lift_step lift_steps[NUM_LIFTS][40];




// stuff for recording video
void screen_shot_for_video(void);
extern int making_video;
extern int ssfnsn; // screen shot file name sequence number

extern int speed_testing;


extern int auto_save_game_on_exit;
extern int auto_save_game_on_level_done;

extern int show_splash_screen;
extern int splash_screen_done;

extern int mdw_an_seq; // mdw animation sequence number
extern float points[10][8]; // for mdw logo
extern int mdw_map_logo_x;
extern int mdw_map_logo_y;
extern int mdw_map_logo_th;
extern float mdw_map_logo_scale;
extern float mdw_splash_logo_x;
extern float mdw_splash_logo_y;
extern int mdw_splash_logo_th;
extern float mdw_splash_logo_scale;
extern float mdw_logo_scale_dec;
extern float mdw_logo_x_dec;
extern float mdw_logo_y_dec;
void mdw_an(void);
int mdw_an2(void);



extern int show_scale_factor;
extern int show_debug_overlay;



// start of global variables declared in main

extern char msg[256];

extern int l[100][100];

extern int Ei[100][32];
extern al_fixed Efi[100][16];
extern int item[500][16];
extern al_fixed itemf[500][4];


extern int db;  // level editor zoom fullscreen double
extern int md;         // menu map double
extern int map_double; // level editor map double

extern int game_map_on;
extern int map_x, map_y;
extern int map_size;
extern int new_size;


extern int Redraw;
extern int Num_legend_lines;
extern int Viewer_lock;

extern int start_level;
extern int play_level;

extern int valid_level_loaded;

extern int txc;



extern char enemy_name[50][32]; // for game
extern char eitype_desc[50][32][40]; // for level editor

extern char color_name[16][20];

extern char lift_step_type_name[10][10];



// ---------- all global ALLEGRO_BITMAP extern declarations here --------
extern ALLEGRO_BITMAP *tilemap;
extern ALLEGRO_BITMAP *ptilemap;
extern ALLEGRO_BITMAP *dtilemap;
extern ALLEGRO_BITMAP *M_tilemap;
extern ALLEGRO_BITMAP *M_ptilemap;
extern ALLEGRO_BITMAP *M_dtilemap;

extern ALLEGRO_BITMAP *memory_bitmap[NUM_SPRITES];
extern ALLEGRO_BITMAP *player_bitmap[16][32];
extern ALLEGRO_BITMAP *door_bitmap[2][16][8];
extern ALLEGRO_BITMAP *l2000;

extern ALLEGRO_BITMAP *level_buffer;
extern ALLEGRO_BITMAP *dtemp; // temp draw

extern ALLEGRO_BITMAP *lefsm; // level editor fullscreen map


extern ALLEGRO_BITMAP *mp;     //  mouse_pointer
extern ALLEGRO_BITMAP *ft_bmp;  //  file temp paste bmp

extern float points[10][8];
void seed_mdw(void);  // for mdw logo
void fill_mdw(void);
void draw_mdw(ALLEGRO_BITMAP *b, int x, int y, float x_scale, float y_scale, int line_thickness);



extern int zz[20][NUM_ANS];


extern int passcount;
extern int passcount_timer_fps;

void inc_timer_passcount(void);


extern int item_num_of_type[20];

extern int sa[NUM_SPRITES][2];

extern char level_filename[80];

extern char global_string[20][25][80];

extern char* pmsg[500];

int load_help(void);

extern char log_msg[100000000]; // for logging
extern int log_msg_pos;
extern char log_lines[1000000][100]; // for log file viewer
extern int log_lines_int[1000000][3]; // for log file viewer
extern int log_timer;


extern struct lift lifts[NUM_LIFTS];
extern struct lift_step lift_steps[NUM_LIFTS][40];
extern int num_lifts;


extern int ima_server;
extern int ima_client;





extern int show_player_join_quit_timer;
extern int show_player_join_quit_player;
extern int show_player_join_quit_jq;



void rtextout_centre(ALLEGRO_BITMAP *dbmp, char *txt1, int x, int y, int col, float scale, int rot, float op);

extern int game_moves[1000000][4];
extern int game_move_entry_pos;
extern int game_move_current_pos;

void add_game_move(int, int, int, int);


void set_comp_move_from_player_key_check(int); // but don't set controls !!!


void fill_door_bitmap(void);
void fill_player_bitmap(void);


void init_player(int p, int t);

void get_player_start_pos(int );


void add_log_entry(char *txt);
void add_log_entry2(int type, int player, char *txt);


void add_log_entry_header(int type, int player, char *txt, int);
void add_log_entry_sdat_rx_and_game_move_entered(int type, int player);
void add_log_entry_centered_text(int type, int player, int width, char *txt, char *border, char *fill);
void add_log_entry_position_text(int type, int player, int width, int pos, char *txt, char *border, char *fill);




int mouse_changed(void);


// game global variables

// ----------------game function prototypes----------------------


void level_editor_start(int);
void save_log_file(void);

void chop_first_x_char(char *str, int n);


// all from zplayer.cpp
void player_move();
void draw_players();
void get_players_shape(int);

extern char *key_names[];

int my_readkey(void);
void set_key_menu(int menu, int p, int start_row);
int zmenu(int, int, int);
void set_screens(int);
void next_map_mode(void);
void next_map_size(void);
void save_apl(void);
void event(int, int, int, int, int, int, int);
void fire_enemy_x_bullet(int EN, int p);
int is_right_solid(int, int, int);
int is_left_solid(int, int, int);
int is_up_solid(int, int, int);
int is_down_solid(int, int, int);
void seek_set_xyinc(int EN);
void seek_set_xyinc(int EN, int x, int y);
al_fixed get_rot_from_xyinc(int EN);
al_fixed get_rot_from_PXY(int EN, int p);


int find_closest_player(int EN);
void fire_enemy_bulleta(int EN, int , int p);
int find_closest_player_quad(int EN, int quad, int prox);
void draw_lift_lines(void);

int construct_lift(int, char *, int, int, int, int);
int construct_lift_step(int, int, int, int, int, int);

void reset_animation_sequence_passcounts(int);
void erase_last_bmsg(void);
void slide_bmsg(void);
void draw_bottom_msg(void);
void draw_top_display(void);
void draw_map(void);
void move_lifts(int ignore_prox);
int proc_controllers(void);
void get_new_background(void);
void get_new_screen_buffer(void);
void draw_items(void);
void draw_lifts(void);
void enemy_move(void);
void draw_enemy(void);
void enemy_collision(void);
void proc_lit_rocket(void);
void proc_player_carry(void);
void proc_item_collision(void);
void proc_item_move(void);
void proc_lit_bomb(void);

void show_level_done(int keypress);

void proc_scale_factor_change(void);


void set_trakbot_mode(int EN, int mode);




void stimp(void);
void stamp(void);

void init_l2000(void);


int load_sprit(void);
void set_map_var(void);
void menu_setup(void);
void set_speed(void);
void set_scale_factor(int instant);
void set_start_level(int start_level);
void load_keys(void);
void frame_and_title(int);
void draw_level(void);
void pm_main();
int load_game(void);
void save_game(void);
void help(char *);


void get_keys(void);

void get_all_keys(int p);
void test_keys(void);

void save_keys(void);

void blind_save_game_moves(int);
void save_gm(void);
int load_gm(char *);

void proc_level_done(void);


void set_controls_from_comp_move(int);
void set_comp_move_from_controls(int);

void set_passcount_timer_fps(int);

void sc_setup(void);
void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);



int item_sort(void);
void sort_enemy(void);
int save_level(int );
void load_sound();
void sound_toggle();
void proc_sound();

void run_dialog_netgame_conf(void);

void show_os_detected(void);
int init_screen(void);


void clear_bullets(void);


void update_animation(void);

// ----------------level editor function prototypes----------------------



extern int get100_x, get100_y;
extern int bx1, by1, bx2, by2;


extern int pop_msg_viewer_pos;













int quit_proc();
int load_sprit_proc();
int save_sprit_proc();

int replace_color_proc();
int select_all_proc();
int new_selection_proc();
int gridlines_proc();
int line_draw_proc();
int rect_draw_proc();
int rectfill_draw_proc();
int circle_draw_proc();
int circlefill_draw_proc();
int floodfill_draw_proc();


/*
int rb1_proc(int msg, DIALOG *d, int c);
int rb2_proc(int msg, DIALOG *d, int c);
int rb3_proc(int msg, DIALOG *d, int c);
int rb4_proc(int msg, DIALOG *d, int c);
int rb5_proc(int msg, DIALOG *d, int c);
int rb6_proc(int msg, DIALOG *d, int c);
int rb7_proc(int msg, DIALOG *d, int c);
int ar1_proc(int msg, DIALOG *d, int c);
int ar2_proc(int msg, DIALOG *d, int c);
int ar3_proc(int msg, DIALOG *d, int c);
int ar4_proc(int msg, DIALOG *d, int c);
int ar5_proc(int msg, DIALOG *d, int c);
int ar6_proc(int msg, DIALOG *d, int c);
*/

int bm_rot_proc();
int bm_flip_x_axis_proc();
int bm_flip_y_axis_proc();
int bm_scroll_up_proc();
int bm_scroll_down_proc();
int bm_scroll_left_proc();
int bm_scroll_right_proc();

// all from e_fnx.cpp
void initialize_zz(void);
void set_rocket_rot(int num, int x2, int y2);
void set_xyinc_rot(int EN, int x2, int y2);
void set_wx(int x, int y);
void set_wx_from_start_block(void);
void show_big(void);
void draw_big(int lifts);
void draw_bs(int cc);
int getbox(char *txt, int obj_type, int sub_type, int num );
int getxy(char *txt, int obj_type, int sub_type, int num );


al_fixed fixmod(al_fixed in, int mod);





void set_swbl();
//int draw_framed_shape(int x, int y, int shape, int frame_width, int ci);


// all from e_bitmap.cpp
/*
int draw_current_bitmap(int msg, DIALOG *d, int c);
int new_draw_csp(int msg, DIALOG *d, int c);
int show_draw_mode(int msg, DIALOG *d, int c);
int draw_animation(int msg, DIALOG *d, int c);
int color_bars(int msg, DIALOG *d, int c);
int fade_proc(int msg, DIALOG *d, int c);
*/


int select_bitmap_ans();
int animation_proc();
int select_bitmap_proc();
int copy_bitmap_proc();
void bitmap_main(void);
int edit_pallete_proc();


// all from e_editor.cpp
void tsw(void);

void get_item_draw_shape(int);
void draw_item_shape(int num, int x, int y);
void get_enemy_draw_shape(int e);  // the main final base one
void draw_enemy_shape(int e, int x, int y);

void draw_percent_bar(int, int, int, int, int);




void show_draw_item_cursor(void);
void set_block_range(void);
void update_editor_background(void);
int process_scrolledge(void);
void look_up(int tlc, int rb);
void look_down(int tlc, int rb);
int zoom_full_screen(int wx, int wy, int draw_item);
void draw_item_info(int x, int y, int color, int type, int num);
int edit_menu(int el);

// all from e_glt.cpp
void global_level();

// all from e_guifnx.cpp
int quit_proc();
int load_sprit_proc();
int save_sprit_proc();
int replace_color_proc();
int gridlines_proc();
int line_draw_proc();
int rect_draw_proc();
int rectfill_draw_proc();
int new_selection();
int select_all();
int rectfill_draw_proc();
int circle_draw_proc();
int circlefill_draw_proc();
int floodfill_draw_proc();
/*
int rb1_proc(int msg, DIALOG *d, int c);
int rb2_proc(int msg, DIALOG *d, int c);
int rb3_proc(int msg, DIALOG *d, int c);
int rb4_proc(int msg, DIALOG *d, int c);
int rb5_proc(int msg, DIALOG *d, int c);
int rb6_proc(int msg, DIALOG *d, int c);
int rb7_proc(int msg, DIALOG *d, int c);
int ar1_proc(int msg, DIALOG *d, int c);
int ar2_proc(int msg, DIALOG *d, int c);
int ar3_proc(int msg, DIALOG *d, int c);
int ar4_proc(int msg, DIALOG *d, int c);
int ar5_proc(int msg, DIALOG *d, int c);
int ar6_proc(int msg, DIALOG *d, int c);
*/

int bm_rot_proc();
int bm_flip_x_axis_proc();
int bm_flip_y_axis_proc();
int bm_scroll_up_proc();
int bm_scroll_down_proc();
int bm_scroll_left_proc();
int bm_scroll_right_proc();

// all from e_item.cpp

void draw_edit_item_shape(int x, int y, int obt, int sub_type, int num);
void draw_button(int b_num, char *msg, int tc, int rc);
void crosshairs(int mx, int my, int x, int y, int color); // funtion to draw rentangle and crosshairs
void title(char *txt, int y, int tc, int fc);
void title_obj(int obj_type, int sub_type, int num, int legend_highlight, int highlight_color);
void object_viewer(int obt, int num);
int item_data(int, int);
int enemy_data(int, int);


//int show_rungame_data(int x_pos, int y_pos);


// all from e_main.cpp

void final_wrapup(void);
int initial_setup(void);
int main(int argument_count, char **argument_array);

// all from e_nev.cpp
void erase_item(int num);
int get_empty_item(void);
int get_empty_item(int type); // finds, sets, sorts refinds
int get_empty_enemy(void);
int get_empty_enemy(int type); // finds, sets, sorts refinds
void recalc_pod(int num);
int move_trigger_box(int num, int type);
int move_pod_extended(int num);
void create_cloner(void);
int create_pod(void);


int edit_pmsg_text(int num, int new_msg);
void display_pop_message(int c, char *f, int xpos_c, int ypos, int redraw_map, int show_line_breaks);
int create_pmsg(int c);

int create_door(int);


int create_obj(int obt, int sub_type, int sent_num);
int create_item(int type);

void clear_lift(int l);
void clear_lift_step(int lift, int step);





// all from e_pde.cpp

int load_PDE();
int bottom_menu(int menu_num);
void save_PDE();
void predefined_enemies(void);

// all from e_sel.cpp

int load_selection(void);
void save_selection(int save);
void do_fcopy(int qx1, int qy1);
void do_clear(void);
void draw_fsel(void);



// all from e_sliders.cpp

void update_var(int bn, int type, int num, float f);

void fill_smsg(int bn, int num);

void mdw_colsel(int x1, int y1, int x2, int y2, int bn, int num,
              int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );

int mdw_button(int x1, int y1, int x2, int y2, int bn, int num,
                int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );

void mdw_slider(int x1, int y1, int x2, int y2, int bn, int num,
                 int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );




// all from e_special.cpp



extern int level_num;
extern int status_window_active;
extern int status_window_x;
extern int status_window_y;
extern int status_window_w;
extern int status_window_h;

extern int draw_item_num;
extern int draw_item_type;
extern int point_item_type;
extern int point_item_num;
extern int wx, wy;

extern int select_window_w;
extern int select_window_h;

extern int select_window_block_on;
extern int swnbl_cur;
extern int swnbl;

extern int select_window_block_y;
extern int select_window_special_on;
extern int select_window_num_special_lines;

extern int PDEi[100][32];
extern char PDEt[100][20][40];


extern int select_window_x;
extern int select_window_y;
extern int select_window_active;

extern int select_window_special_y;
extern int select_window_text_y;
extern int swbl[NUM_SPRITES][2];

extern int sw_mouse_gone;

extern int stext_draw_flag;
extern int btext_draw_flag;


int process_status_window(int);

void draw_select_window(void);
int process_select_window(int);


// all from e_lev.cpp
void level_viewer(void);



// all from yfilecom.cpp

void zero_level_data(void);
void make_filename(int x);
int load();
int save();
int load_sprit(void);
int load_level(int level_to_load, int display);
int save_level(int level_to_save);

int save_sprit(void);


#ifdef OLDFNXSTORAGE
int game_load_level(int level_to_load, int display);
#endif

// from bled

int block_editor_proc();

// all from e_nlv.cpp


void erase_lift(int lift);
void set_lift_xyinc(int d, int step); // used by move lift everywhere
//void draw_steps(int lift, int current_step, int highlight_step);
void draw_lift_mp(int lift); // draws the current lift on mp
int get_new_lift_step(int lift, int step, int x, int y);
void draw_step_button(int txc, int ty, int lift, int step, int tc, int rc);
int create_lift(void);
void lift_setup(void);
void set_lift(int lift, int step);
int lift_editor(int lift);


// from e_menu.cpp
int pmenu(int menu_num);
void text_setup();



