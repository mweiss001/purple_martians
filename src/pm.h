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


extern int num_enemy;

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

extern ALLEGRO_BITMAP *text_title;
extern int text_title_bitmaps_create;
extern int text_title_draw_color;



extern int level_display_region_x;
extern int level_display_region_y;
extern int level_display_region_w;
extern int level_display_region_h;

extern char skc[64];
extern int skc_index;


extern int les; // level editor scale

extern int level_editor_running;
extern int help_screens_running;


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

extern int visual_level_select_running;

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


extern float mouse_loop_pause;

extern char local_hostname[80];
extern char version_string[80];

// #define CLONERLINES
// #define RELEASE

#define NETPLAY
#ifdef NETPLAY
void server_control(void);
void server_local_control(int);
int server_init(void);
void server_exit(void);
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
// void screen_shot_for_video(void);
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

extern int show_scale_factor;
extern int show_debug_overlay;


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




extern int zz[20][NUM_ANS];

extern int passcount;
extern int passcount_timer_fps;

extern int item_num_of_type[20];

extern int sa[NUM_SPRITES][2];

extern char level_filename[80];

extern char global_string[20][25][80];

extern char *pmsg[500];



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

extern int game_moves[1000000][4];
extern int game_move_entry_pos;
extern int game_move_current_pos;

extern char *key_names[];


extern int get100_x, get100_y;
extern int bx1, by1, bx2, by2;
extern int pop_msg_viewer_pos;


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

// ---------------------------------------------------------------------
// -------- new list of function prototypes 20180515 -------------------
// ---------------------------------------------------------------------

//e_bitmap.h - done
int select_bitmap_ans(int zzindx);
int animation_proc();
int select_bitmap_proc();
int copy_bitmap_proc();

// e_editor.h - done
void get_item_draw_shape(int i);
void draw_item_shape(int i, int x, int y);
void draw_enemy_shape(int e, int x, int y);
void draw_PDE_shape(int pde, int x, int y);
void show_draw_item_cursor(void);
void set_block_range(void);
void get_new_box(void);
void update_editor_background(void);
int process_scrolledge(void);
void pointer_text(int x, int y, int ty);
void do_brf(int x, int y, int flood_block);
int zoom_full_screen(int wx, int wy, int draw_item);
void draw_item_info(int x, int y, int color, int type, int num);
int edit_menu(int el);

// e_fnx.h - done
int mouse_changed(void);
al_fixed get_sproingy_jump_height(int num);
void set_xyinc_rot(int EN, int x2, int y2);
void set_rocket_rot(int num, int x2, int y2);
void set_wx(int x, int y);
void set_wx_from_start_block(void);
void show_big(void);
void draw_big(int draw_lifts);
void draw_cloner_boxes(int num);
void draw_bs(int cc);
int getbox(char *txt, int obj_type, int sub_type, int num );
int getxy(char *txt, int obj_type, int sub_type, int num );
int get_item(char *txt, int obj_type, int sub_type, int num );
void initialize_zz(void);

// glt.h - done
void show_block_list(void);
void global_level();

// e_item.h - done
void crosshairs(int mx, int my, int x, int y, int color);
void crosshairs_nodb(int mx, int my, int x, int y, int db, int color);
void title(char *txt, int y, int tc, int fc);
void test_items(void);
int sort_item(void);
void show_all_items(void);
void title_obj(int obj_type, int sub_type, int num, int legend_highlight, int highlight_color);
int move_obt_with_map(int obt, int type, int num);
void object_viewer(int obt, int num);

// e_lev.h - done
int lev_show_level_data(int x_pos, int y_pos);
void mark_rect(int sel, int color);
void show_cur(void);
void show_msel(void);
void compare_all(void);
void lev_draw(int full);
void level_viewer(void);
void show_cur_vs(int cur, int x1, int y1, int size, int fc);
void load_visual_level_select(void);
int visual_level_select(void);

// e_menu.h - done
int pmenu(int menu_num);
void text_setup();

// e_nev.h - done
void erase_item(int num);
int get_empty_item(void);
int get_empty_item(int type);
int get_empty_enemy(void);
int get_empty_enemy(int type);
int move_trigger_box(int num, int type);
void recalc_pod(int EN);
int move_pod_extended(int num);
void show_all_enemies(void);
void sort_enemy(void);
void create_cloner(void);
int create_pod(void);
int create_key(int c);
int create_start_block(int c);
int create_exit(int c);
void show_all_pmsg(void);
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore, int rot);
void display_pop_message(int c, char *f, int xpos_c, int ypos, int redraw_map, int show_line_breaks);
int edit_pmsg_text(int c, int new_msg);
int create_pmsg(int c);
int create_obj(int obt, int sub_type, int sent_num);
int create_door(int type);
int create_item(int type);

// e_nlv.h - done
void edit_server_name(void);
int edit_lift_name(int lift, int step_ty, int bts);
void show_all_lifts(void);
void erase_lift(int lift);
void delete_lift_step(int lift, int step);
void lift_setup(void);
void draw_step_button(int xa, int xb, int ty, int ty2, int lift, int step, int rc);
void draw_steps(int step_ty, int lift, int current_step, int highlight_step);
void highlight_current_lift(int lift);
void draw_lift_mp(int lift);
int create_lift(void);
void move_lift_step(int lift, int step);
void redraw_get_new_lift_step_menu(int sty, int step, int highlight);
int get_new_lift_step(int lift, int step);
int insert_lift_step(int lift, int step);
void insert_steps_until_quit(int lift, int step);
void step_popup_menu(int lift, int step);
void set_bts(int lift);
void redraw_lift_viewer(int lift, int step);
int lift_editor(int lift);


// e_pde.h - done
int load_PDE();
void save_PDE();
int bottom_menu(int menu_num);
int edit_int(int x, int y, int val, int inc, int lv, int uv);
al_fixed edit_fix(int x, int y, al_fixed val);
void PDE_swap(int s1, int s2);
void PDE_sort(void);
void predefined_enemies(void);

// e_sel.h - done
void clear_ft(void);
int load_selection(void);
void save_selection(int save);
int enforce_limit(int val, int ll, int ul);
int check_limit(int val, int ll, int ul);
void do_fcopy(int qx1, int qy1);
void do_rnd(void);
void do_clear(void);
void draw_fsel(void);

// e_sliders.h - done
void update_var(int bn, int type, int num, float f);
void fill_smsg_button(int bn, int obt, int type, int num);
void fill_smsg_slider(int bn, int type, int num);
void mdw_colsel(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
int mdw_button(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);
void mdw_slider(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );

// e_special.h
void check_s_window_pos(int reset_pos);
int process_status_window(int draw_only);
int process_select_window(int draw_only);
void set_swbl(void);


#include <libnet.h>
#include <zlib.h>

// n_client.h - done
int ClientInit(char *serveraddress);
int ClientCheckResponse(void);
int ClientReceive(void *data);
void ClientSend(void *data, int len);
void client_flush(void);
void ClientExit(void);
void nc_client(void);
int client_init_driver(void);
int client_init_join(void);
void client_exit(void);
int client_init(void);
void read_game_step_from_packet(int x, int clf_check);
void client_timer_adjust(void);
int process_chdf_packet(void);
void client_apply_diff();
void client_block_until_good_chdf_received(void);
void process_bandwidth_counters(int p);
void client_control(void);
void client_local_control(int p);

// n_network.h
extern int NetworkDriver;
int NetworkInit();

// n_packet.h
extern char packetbuffer[1024];
extern int packetsize;

void Packet(char *id);
int PacketRead(char *id);

void PacketAddByte(char b);
char PacketGetByte(void);

void PacketAdd2Bytes(int);
int Packet2ByteRead(void);

void PacketAdd3Bytes(int);
int Packet3ByteRead(void);

void PacketAdd4Bytes(int);
int Packet4ByteRead(void);

void PacketAddString(char*);
void PacketReadString(char*);

//n_server.h
#define MAX_CLIENTS 32
extern NET_CONN *ListenConn;
extern NET_CONN *ClientConn[MAX_CLIENTS];
extern int ClientNum;

int ServerInit(void);
void ServerExit(void);
int ServerListen(void);
int ServerReceive(void *data, int *sender);
void ServerBroadcast(void *data, int len);
void ServerSendTo(void *data, int len, int who);


// yfilecom.h - done
void make_filename(int x);
int load_level_prompt(void);
int save_level_prompt(void);
int save_sprit(void);
int load_sprit(void);
void zero_level_data(void);
int load_level(int level_to_load, int display);
int save_level(int level_to_save);
int mw_file_select(const char * title, char * fn, const char * ext, int save);

// zbullets.h - done
void proc_pbullets(void);
void draw_pbullets(void);
void proc_ebullets(void);
void draw_ebullets(void);
void clear_bullets(void);

// zcontrol.h - done
int getJoystickNum(ALLEGRO_JOYSTICK* joy);
int get_scan_code_from_joystick(int joy, int b_a, int num);
int my_readkey(void);
void get_all_keys(int p);
void test_keys(void);
void set_start_level(int s);
void set_passcount_timer_fps(int x);
void set_speed(void);
void set_comp_move_from_controls(int p);
void clear_keys(int p);
void set_controls_from_comp_move(int g);
void set_comp_move_from_player_key_check(int p);
void player_key_check(int p);
void function_key_check(void);
void rungame_key_check(int p, int ret);
void add_game_move(int pc, int type, int data1, int data2);
void proc_game_move(void);
void serial_key_check(int key);
void set_controls_from_game_move(int p);
int proc_events(ALLEGRO_EVENT ev, int ret);
int proc_controllers(void);

// zemove.h - done
int enemy_data(int x_pos, int y_pos);
void get_enemy_draw_shape(int e);
void draw_enemy(void);
void enemy_collision(void);
void enemy_flapper(void);
void enemy_block_walker(void);
void enemy_cloner(void);
void set_trakbot_mode(int EN, int mode);
void enemy_trakbot(void);
void enemy_podzilla(void);
void enemy_cannon(void);
void enemy_bouncer(void);
void enemy_archwagon(void);
al_fixed mdw_fixmul(al_fixed a, al_fixed b, float f_round);
void enemy_deathcount(void);
void enemy_move(void);
void enemy_killed(int EN);
void enemy_player_hit_proc(int EN);

// zfile.h - done
char* cmtos(int cm);
void save_gm_txt(char *sfname);
void save_gm_gm(char *sfname);
void save_gm(void);
void blind_save_game_moves(int d);
int load_gm(char *sfname);

// zfnx.h - done
int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);
void erase_last_bmsg(void);
void slide_bmsg(void);
void tsw(void);
void reset_animation_sequence_passcounts(int pc);
void update_animation(void);
al_fixed get_rot_from_xyinc(int EN);
al_fixed get_rot_from_PXY(int EN, int p);
void seek_set_xyinc(int EN);
void seek_set_xyinc(int EN, int x, int y);
int find_closest_player_flapper(int EN, int dir);
int find_closest_player_quad(int EN, int quad, int prox);
int find_closest_player(int EN);
void fire_enemy_bulleta(int EN, int bullet_ans, int p);
void fire_enemy_x_bullet(int EN, int p);
int is_right_solid(int solid_x, int solid_y, int lift_check);
int is_left_solid(int solid_x, int solid_y, int lift_check);
int is_down_solid(int solid_x, int solid_y, int lift_check);
int is_up_solid(int solid_x, int solid_y, int lift_check);
al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
void state_to_chunk(char * b);
void chnk_to_state(char * b);
void get_chunk_dif(char *a, char *b, char *c, int size);
void apply_chunk_dif(char *a, char *c, int size);
void reset_states(void);
void show_chunk_dif(char *a, char *b);
int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra);
void demo_mode(void);
void temp_test(void);

// zitem.h - done
int item_data(int x_pos, int y_pos);
void change_linked_door_color_and_shape(int door);
void remove_block(int x, int y);
void draw_pop_message(int c);
int small_letter(ALLEGRO_BITMAP *bmp, int x, int col, char *t);
void draw_small_text(ALLEGRO_BITMAP * bmp, int x, int y, int col, int textnum);
void draw_door(int c, int x, int y);
void draw_items(void);
void proc_item_move(void);
int player_drop_item(int p);
void proc_player_carry(void);
void proc_item_collision();
void do_bomb_damage(int i);
void proc_lit_bomb(void);
void proc_lit_rocket(void);

// zlifts.h - done
int construct_lift(int l, char* lift_name, int width, int height, int color, int num_steps);
void clear_lift(int l);
int construct_lift_step(int lift, int step, int x, int y, int val, int type);
void clear_lift_step(int lift, int step);
void set_lift(int lift, int step);
void draw_lift_lines(void);
void draw_lifts(void);
void set_lift_xyinc(int d, int step);
void move_lifts(int ignore_prox);

// zlog.h - done
void log_bandwidth_stats(int p);
void log_reason_for_client_quit(int p);
void log_player_array(void);
void log_ending_stats(void);
void log_ending_stats_server(void);
void save_log_file(void);
void add_log_entry_sdat_rx_and_game_move_entered(int type, int player);
void add_log_entry2(int type, int player, char *txt);
void add_log_entry_position_text(int type, int player, int width, int pos, char *txt, char *border, char *fill);
void add_log_entry_centered_text(int type, int player, int width, char *txt, char *border, char *fill);
void add_log_entry_header(int type, int player, char *txt, int blank_lines);
int fill_filename_array(ALLEGRO_FS_ENTRY *fs, void * extra);
int log_file_viewer(int type);

// zlogo.h - done
void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, char * txt);
void draw_title(int tx, int ty, int tw, int th, int color);
void draw_demo_mode_overlay(void);
void draw_large_2lines(ALLEGRO_FONT *tf, char * m1, char * m2, int color, float opa );
void idw(int txt, int x, int y, float x_scale, float y_scale);
void splash_screen(void);
void splash_toggle(void);
void mspline(float *par, ALLEGRO_COLOR col, int thickness);
void mfspline(float *par, int col, int thickness);
void draw_mdw(int x, int y, float x_scale, float y_scale, int line_thickness);
void seed_mdw(void);
void fill_mdw(void);
void mdw_an3(int x, int y, float sc, int th);
int mdw_an2(void);
void mdw_an(void);
void spline_test(void);
void redraw_spline(int s);
void spline_adjust(void);

// zloop.h - done
void add_screen_msg(char *txt, int x, int y, int delay, int ssn, int z1, int z2, int z3, int z4);
void draw_screen_msg(void);
void new_bmsg(char *nb);
void event(int ev, int x, int y, int z1, int z2, int z3, int z4);
void proc_frame_delay(void);
void proc_player_health(void);
void proc_level_done(void);
void pm_main(void);

// zmain.h - done
void final_wrapup(void);
void fast_exit(int why);
void get_hostname(void);
void save_config(void);
void get_config_values(void);
void make_palette(void);
void m_err(char * err_msg);
void window_title(void);
int initial_setup(void);
void game_menu(void);
int main(int argument_count, char **argument_array);
int copy_files_to_clients(int exe_only);

// zmenu.h - done
int load_help(void);
void chop_first_x_char(char *str, int n);
void help(char *topic);
int zmenu(int menu_num, int menu_pos, int y);
void menu_setup(void);
void set_key_menu(int menu, int p, int start_row);

// zplayer.h - done
void get_player_start_pos(int p);
void player_move(void);
void draw_player(int p);
void draw_players(void);
void get_players_shape(int p);
void fill_door_bitmap(void);
void fill_player_bitmap(void);
void init_player(int p, int t);

// zscrn.h - done
void show_bitmap_flags(int flags);
void show_pixel_format(int df);
void show_display_flags(int flags);
void show_display_options(void);
void show_display_orienation(void);
void show_fullscreen_modes(void);
void create_bmp(void);
void show_resize_msg(int type);
void load_fonts(void);
void auto_set_les(void);
void set_ortho(void);
void rebuild_bitmaps(void);
void show_disp_values(void);
void proc_screen_change(int new_sw, int new_sh, int new_sx, int new_sy, int new_fs);
void test_window_size_and_pos(void);
int init_screen(void);
void get_new_background(int full);
void stimp(void);
void stamp(void);
void get_new_screen_buffer(void);
void set_map_var(void);
void set_scale_factor(int instant);
void init_l2000(void);
void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);
void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor);
void draw_level(void);
void frame_and_title(int show_players);
void proc_scale_factor_change(void);
void rtextout_centre(ALLEGRO_BITMAP *dbmp, char *txt1, int x, int y, int col, float scale, int rot, float op);
void mtextout(char *txt1, int x, int y, float x_scale, float y_scale, int col);
void mtextout_centre(char *txt1, int x, int y, float x_scale, float y_scale, int col);
void show_level_done(int keypress);
void set_map_position(void);
void next_map_mode(void);
void next_map_size(void);
void draw_map(void);
void draw_percent_bar(int cx, int y, int width, int height, int percent);
void draw_screen_overlay(void);
void show_player_join_quit(void);
void draw_fps_display(int show_type);
void draw_speed_test_data(void);
void draw_top_display(void);
void draw_bottom_msg(void);

// zsound.h - done
void stop_sound(void);
void proc_sound(void);
void load_sound(void);
void set_se_scaler(void);
void set_st_scaler(void);
void sound_toggle(void);





















