// pm.h - header file for both the game and the level editor

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

#include <libnet.h>
#include <zlib.h>


#define NUM_SPRITES 1024
#define NUM_ANS 256
#define NUM_PLAYERS 8
#define BORDER_WIDTH 14
#define NUM_LIFTS 40


#define PM_LIFT_TYPE_BITS        0b00000000000000000000000000011111
#define PM_LIFT_FLAG_BITS        0b00001111111111111111111111100000
#define PM_LIFT_COLOR_BITS       0b11110000000000000000000000000000
#define PM_LIFT_NO_DRAW          0b00001000000000000000000000000000
#define PM_LIFT_SOLID_PLAYER     0b00000100000000000000000000000000
#define PM_LIFT_SOLID_ENEMY      0b00000010000000000000000000000000
#define PM_LIFT_SOLID_ITEM       0b00000001000000000000000000000000
#define PM_LIFT_HIDE_LINES       0b00000000100000000000000000000000



#define PM_BTILE_MOST_FLAGS        0b00111111111111110000000000000000
#define PM_BTILE_ALL_FLAGS         0b01111111111111110000000000000000
#define PM_BTILE_ALL_SOLID         0b00000000000111110000000000000000
#define PM_BTILE_ALL_SEMI          0b00000000111001110000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000
#define PM_BTILE_SEMISOLID_PLAYER  0b00000000001000000000000000000000
#define PM_BTILE_SEMISOLID_ENEMY   0b00000000010000000000000000000000
#define PM_BTILE_SEMISOLID_ITEM    0b00000000100000000000000000000000
#define PM_BTILE_BOMBABLE          0b00000001000000000000000000000000
#define PM_BTILE_BREAKABLE_PBUL    0b00000010000000000000000000000000
#define PM_BTILE_BREAKABLE_EBUL    0b00000100000000000000000000000000
#define PM_BTILE_LADDER_MOVE       0b00001000000000000000000000000000
#define PM_BTILE_ROPE_MOVE         0b00010000000000000000000000000000
#define PM_BTILE_SECRET            0b00100000000000000000000000000000
#define PM_BTILE_SHOW_SELECT_WIN   0b01000000000000000000000000000000




#define PM_ENEMY_FIELD_DAMAGE_PLAYER  0b00000000000000000000000000000001
#define PM_ENEMY_FIELD_DAMAGE_ENEMY   0b00000000000000000000000000000010
#define PM_ENEMY_FIELD_DAMAGE_ITEM    0b00000000000000000000000000000100
#define PM_ENEMY_FIELD_DAMAGE_PBUL    0b00000000000000000000000000001000
#define PM_ENEMY_FIELD_DAMAGE_EBUL    0b00000000000000000000000000010000
#define PM_ENEMY_FIELD_TRIGGER_PLAYER 0b00000000000000000000000000100000
#define PM_ENEMY_FIELD_TRIGGER_ENEMY  0b00000000000000000000000001000000
#define PM_ENEMY_FIELD_TRIGGER_ITEM   0b00000000000000000000000010000000
#define PM_ENEMY_FIELD_TRIGGER_PBUL   0b00000000000000000000000100000000
#define PM_ENEMY_FIELD_TRIGGER_EBUL   0b00000000000000000000001000000000
#define PM_ENEMY_FIELD_DAMAGE_CURR    0b00000000000000000000010000000000
#define PM_ENEMY_FIELD_TRIGGER_CURR   0b00000000000000000000100000000000
#define PM_ENEMY_FIELD_TRIGGER_PREV   0b00000000000000000001000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_ON    0b00000000000000000010000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_XC    0b00000000000000000100000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_XF    0b00000000000000001000000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_XL    0b00000000000000010000000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_YC    0b00000000000000100000000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_YF    0b00000000000001000000000000000000
#define PM_ENEMY_FIELD_LIFT_TRG_YL    0b00000000000010000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_ON    0b00000000000100000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_XC    0b00000000001000000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_XF    0b00000000010000000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_XL    0b00000000100000000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_YC    0b00000001000000000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_YF    0b00000010000000000000000000000000
#define PM_ENEMY_FIELD_LIFT_DMG_YL    0b00000100000000000000000000000000




#define PM_ITEM_TRIGGER_PLAYER   0b00000000000000001
#define PM_ITEM_TRIGGER_ENEMY    0b00000000000000010
#define PM_ITEM_TRIGGER_ITEM     0b00000000000000100
#define PM_ITEM_TRIGGER_PBUL     0b00000000000001000
#define PM_ITEM_TRIGGER_EBUL     0b00000000000010000
#define PM_ITEM_TRIGGER_CURR     0b00000000000100000
#define PM_ITEM_TRIGGER_PREV     0b00000000001000000
#define PM_ITEM_TRIGGER_TGON     0b00000000010000000
#define PM_ITEM_TRIGGER_TGOF     0b00000000100000000
#define PM_ITEM_TRIGGER_LIFT_ON  0b00000001000000000
#define PM_ITEM_TRIGGER_LIFT_XC  0b00000010000000000
#define PM_ITEM_TRIGGER_LIFT_XF  0b00000100000000000
#define PM_ITEM_TRIGGER_LIFT_XL  0b00001000000000000
#define PM_ITEM_TRIGGER_LIFT_YC  0b00010000000000000
#define PM_ITEM_TRIGGER_LIFT_YF  0b00100000000000000
#define PM_ITEM_TRIGGER_LIFT_YL  0b01000000000000000
#define PM_ITEM_TRIGGER_DRAW_ON  0b10000000000000000

#define PM_ITEM_DAMAGE_PLAYER   0b000000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b000000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b000000000000000100
#define PM_ITEM_DAMAGE_PBUL     0b000000000000001000
#define PM_ITEM_DAMAGE_EBUL     0b000000000000010000
#define PM_ITEM_DAMAGE_CURR     0b000000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b000000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b000000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b000000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b000000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b000000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b000000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b000001000000000000
#define PM_ITEM_DAMAGE_TIMR_SN  0b000010000000000000
#define PM_ITEM_DAMAGE_TIMR_BN  0b000100000000000000
#define PM_ITEM_DAMAGE_TIMR_SP  0b001000000000000000
#define PM_ITEM_DAMAGE_TIMR_BP  0b010000000000000000
#define PM_ITEM_DAMAGE_INSTGIB  0b100000000000000000






// enemy debug stuff
//#define SHOW_POD_CLONER_TRIGGER_BOX
// #define SHOW_CANNON_COLLISION_BOX
//#define SHOW_FLAPPER_DEBUG
//#define SHOW_TRAKBOT_BULLET_TRIGGER_CIRCLE

// item debug stuff
// #define SHOW_BOMB_DAMAGE_BOX

// screen debug stuff
// #define SHOW_HYSTERESIS_WINDOW



//#define RELEASE



// ------------------------------------------------
// ------------------------------------------------
// ----------- Global variables -------------------
// ------------------------------------------------
// ------------------------------------------------
// --- declared in zmain.cpp and externed here ----
// ------------------------------------------------
// -- (I know globals are bad programming style) --
// ------------------------------------------------
// ------------------------------------------------
// ------------------------------------------------



extern int pm_event[1000];
//extern int copy_link_tranform[500][4];






// temp testing variable
extern int pct_x;
extern int pct_y;


extern int tx1;
extern int ty1;
extern int ttc1;
extern float ttfloat1;


extern int tx2;
extern int ty2;
extern int ttc2;
extern float ttfloat2;


//old#define PML_SIZE 370880

//#define PML_SIZE  371520
#define PML_SIZE 384960



extern int level_header[20];



// ------------------------------------------------
// ----------------- netgame ----------------------
// ------------------------------------------------
extern int ima_server;
extern int ima_client;
extern char m_serveraddress[256];
extern int TCP;

extern int stdf_freq;
extern int zlib_cmp;
extern int control_lead_frames;
extern int server_lead_frames;

extern int deathmatch_pbullets;
extern int deathmatch_pbullets_damage;
extern int suicide_pbullets;



//#define STATE_SIZE  105280
//#define STATE_SIZE 105920
#define STATE_SIZE 105952



// server's copies of client states
extern char srv_client_state[8][2][STATE_SIZE];
extern int srv_client_state_frame_num[8][2];

// local client's states
extern char client_state_buffer[STATE_SIZE];  // buffer for building compressed dif from packet pieces
extern int  client_state_buffer_pieces[16];   // to mark packet pieces as received
extern char client_state_base[STATE_SIZE];    // last ack state
extern int  client_state_base_frame_num;      // last ack state frame_num
extern char client_state_dif[STATE_SIZE];     // uncompressed dif
extern int  client_state_dif_src;             // uncompressed dif src frame_num
extern int  client_state_dif_dst;             // uncompressed dif dst frame_num



// ------------------------------------------------
// ----- visual level select ----------------------
// ------------------------------------------------
#define NUM_LEV 400
extern ALLEGRO_BITMAP * grid_bmp;
extern ALLEGRO_BITMAP * level_icon_bmp[NUM_LEV];
extern int le[NUM_LEV];
extern int num_levs;
extern int sel_x, sel_y, sel_size;
extern int grid_cols, grid_rows, grid_size, grid_width, grid_height;
extern int load_visual_level_select_done;



// frame_speed, frames per second, frame_num stuff
extern int speed_testing;
extern int actual_fps;
extern int last_frames_skipped;
extern int frames_skipped_last_second;
extern int last_fps_frame_num;
extern int draw_frame;
extern int frame_speed;
extern int frame_num;

// global game control
extern int game_exit;
extern int level_done_trig;
extern int level_done_proc;

extern int next_level;

// some global strings
extern char level_filename[80];
extern char local_hostname[80];
extern char pm_version_string[80];
extern char al_version_string[80];
extern char global_string[20][25][80];
extern char msg[1024];
extern char color_name[16][20];

// for log file viewer code to find most recent log file
extern ALLEGRO_FS_ENTRY *filenames[1000];
extern int num_filenames;

// animation sequence array
extern int zz[20][NUM_ANS];

// ------------------------------------------------
// ----- level editor unsorted --------------------
// ------------------------------------------------
extern char sel_filename[500];
extern int ty;   // button start
extern int bts;  // button spacing
extern int wx; // level editor start block UL corner
extern int wy;
extern int pop_msg_viewer_pos;
extern int Redraw;
extern int num_legend_lines;
extern int Viewer_lock;

// ------------------------------------------------
// ----- return values from getxy and getbox ------
// ------------------------------------------------
extern int bx1, bx2, by1, by2;
extern int get100_x, get100_y;

// ------------------------------------------------
// ------------ game moves array ------------------
// ------------------------------------------------
#define GAME_MOVES_SIZE 1000000

extern int game_moves[GAME_MOVES_SIZE][4];
extern int game_move_entry_pos;
extern int game_move_current_pos;

// ------------------------------------------------
// ------------- screen messages ------------------
// ------------------------------------------------
extern int bottom_msg_on;
extern int bottom_msg;
extern int bmsg_index;
extern ALLEGRO_BITMAP *bmsg_bmp[20];
extern ALLEGRO_BITMAP *bmsg_bmp2[20];
extern ALLEGRO_BITMAP *bmsg_temp;
extern int game_event_retrigger_holdoff[10];
extern float game_event_retrigger_holdoff_tally[10];





// ------------------------------------------------
// -----------status and selection window----------
// ------------------------------------------------

// object viewer window
extern int ov_window_active;
extern int ov_window_x;
extern int ov_window_y;
extern int ov_window_w;
extern int ov_window_h;





// status window
extern int status_window_active;
extern int status_window_x;
extern int status_window_y;
extern int status_window_w;
extern int status_window_h;

extern int draw_item_num;
extern int draw_item_type;
extern int point_item_type;
extern int point_item_num;

// select window
extern int select_window_active;
extern int select_window_x;
extern int select_window_y;
extern int select_window_w;
extern int select_window_h;
extern int select_window_text_y;
extern int select_window_block_on;
extern int swbn;
extern int swnbl;
extern int swnbl_cur;
extern int swbl[NUM_SPRITES][2];
extern int show_non_default_blocks;
extern int show_flag_details;

extern int autoload_bookmark;
extern int bookmark_level;
extern int bookmark_obj;
extern int bookmark_num;



extern int select_window_block_y;
extern int btext_draw_flag;
extern int select_window_special_on;
extern int select_window_num_special_lines;
extern int select_window_special_y;
extern int stext_draw_flag;
extern int sw_mouse_gone;







// ------------------------------------------------
// ---------------zoom full screen ----------------
// ------------------------------------------------
extern int stx;
extern int sty;
extern int sux;
extern int suy;

extern int copy_blocks;
extern int copy_enemies;
extern int copy_items;
extern int copy_lifts;
extern int copy_flags;
extern int copy_mode;
extern int brf_mode;



extern int ft_level_header[20];
extern int ft_l[100][100];
extern int ft_item[500][16];
extern char ft_pmsgtext[500][500];

extern int ft_Ei[100][32];
extern al_fixed ft_Efi[100][16];

extern char ft_ln[NUM_LIFTS][80];
extern int ft_lift[NUM_LIFTS][6];
extern int ft_ls[NUM_LIFTS][40][6];


// ------------------------------------------------
// ----------------- demo mode --------------------
// ------------------------------------------------
extern ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
extern int demo_played[100];
extern int num_demo_filenames;
extern int demo_mode_on;
extern int demo_mode_countdown;

// ------------------------------------------------
// ----------------- mouse and keys ---------------
// ------------------------------------------------

#define MAP_LOCK_KEY ALLEGRO_KEY_Z

extern char skc[64]; // serial key check
extern int skc_index;


extern char *key_names[];
extern bool key[ALLEGRO_KEY_MAX];
extern int Key_pressed_ASCII;

extern float mouse_loop_pause;

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

extern int KEY_F1_held;
extern int KEY_F2_held;
extern int KEY_F3_held;
extern int KEY_F4_held;
extern int KEY_F5_held;
extern int KEY_F6_held;
extern int KEY_F7_held;
extern int KEY_F8_held;
extern int KEY_F9_held;
extern int KEY_F10_held;
extern int KEY_F11_held;
extern int KEY_F12_held;
extern int KEY_PRTSCR_held;

extern int KEY_UP_held;
extern int KEY_DOWN_held;
extern int KEY_LEFT_held;
extern int KEY_RIGHT_held;



// ------------------------------------------------
// ----------------- setup ------------------------
// ------------------------------------------------
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

// ------------------------------------------------
// ----------------- bitmaps ----------------------
// ------------------------------------------------

extern ALLEGRO_BITMAP *tilemap;
extern ALLEGRO_BITMAP *btilemap;
extern ALLEGRO_BITMAP *ptilemap;
extern ALLEGRO_BITMAP *dtilemap;
extern ALLEGRO_BITMAP *M_tilemap;
extern ALLEGRO_BITMAP *M_btilemap;
extern ALLEGRO_BITMAP *M_ptilemap;
extern ALLEGRO_BITMAP *M_dtilemap;

extern ALLEGRO_BITMAP *tile[NUM_SPRITES];
extern ALLEGRO_BITMAP *btile[NUM_SPRITES];


extern int sa[NUM_SPRITES][2];

extern ALLEGRO_BITMAP *player_tile[16][32];
extern ALLEGRO_BITMAP *door_tile[2][16][8];

extern ALLEGRO_BITMAP *level_background;
extern ALLEGRO_BITMAP *level_buffer;

extern ALLEGRO_BITMAP *dtemp; // temp draw
extern ALLEGRO_BITMAP *lefsm; // level editor fullscreen map

extern ALLEGRO_BITMAP *mp;     //  mouse_pointer
extern ALLEGRO_BITMAP *ft_bmp;  //  file temp paste bmp

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








// ------------------------------------------------
// ----------------- logging ----------------------
// ------------------------------------------------
extern char log_msg[100000000]; // for logging
extern int log_msg_pos;
extern char log_lines[1000000][100]; // for log file viewer
extern int log_lines_int[1000000][3]; // for log file viewer
extern int log_timer;

extern int L_LOGGING_NETPLAY;
extern int L_LOGGING_NETPLAY_JOIN;
extern int L_LOGGING_NETPLAY_bandwidth;
extern int L_LOGGING_NETPLAY_cdat;
extern int L_LOGGING_NETPLAY_game_move;
extern int L_LOGGING_NETPLAY_sdat;
extern int L_LOGGING_NETPLAY_sdak;
extern int L_LOGGING_NETPLAY_stdf;
extern int L_LOGGING_NETPLAY_stdf_all_packets;
extern int L_LOGGING_NETPLAY_stdf_when_to_apply;
extern int L_LOGGING_NETPLAY_show_dif1;
extern int L_LOGGING_NETPLAY_show_dif2;

extern int auto_save_game_on_exit;
extern int auto_save_game_on_level_done;

#define NETPLAY_bandwidth_tracking


// ------------------------------------------------
// ---------------- players -----------------------
// ------------------------------------------------
extern struct player players[NUM_PLAYERS];
extern struct player1 players1[NUM_PLAYERS];
extern int active_local_player;

struct player // synced between server and client
{
   int active;  // all routines that process player will skip inactive
   int paused;
   int paused_type; // 1 = death, 2 = door move
   int paused_mode;
   int paused_mode_count;

   int spawn_point_index;


   al_fixed PX, PY;       // players position
   al_fixed xinc, yinc;   // players momentum
   al_fixed right_xinc, left_xinc;

   al_fixed LIFE;
   al_fixed old_LIFE;
   int LIVES;

   int on_ladder;

   int shape; // index to player_tile
   int color; // used to draw frames and stuff in players color

   int door_draw_rot_num_steps;
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

   int player_ride, carry_item;

   // flags that indicate a control has been activated or held
   int up, down, left, right, jump, fire, fire_held, menu;

   int bullet_wait_counter, request_bullet, bullet_wait, bullet_speed;


   int on_rope;

   int num_hits;       // when players bullet hits enemy
   int control_method; // 0 = local, 1 = file play, 2 = remote view; 3 = server_local; 4 = client_local
};

struct player1 // not synced between server and client
{
   int health_display;
   int last_health_adjust;
   int potential_bomb_damage;


   float field_damage_tally;
   int field_damage_holdoff;
   int field_damage_enemy_number;

   int up_key, down_key, left_key, right_key, jump_key, fire_key, menu_key;
   int comp_move, old_comp_move;

   int fake_keypress_mode;

   int frames_skipped;

   // position and size of players on screen buffer (used to see if map is covering player)
   int sbx1,sby1,sbx2,sby2;

   int who; // for network id of clients
   char hostname[16];

   int game_move_entry_pos;             // server only for client game_move data sync

   int server_last_sdat_sent_frame_num; // only server uses it, to keep track of when last sdat was sent to client

   int server_last_sdat_sent_start;     // used by server to prevent sending multiple sdats with same start and num
   int server_last_sdat_sent_num;

   int server_last_sdak_rx_frame_num; // used by server to see if client is still responding
   int client_last_sdat_rx_frame_num; // used by client to see if server is still responding

   int server_sdat_sync_freq;


   int client_sync;
   int server_sync;

   int client_game_move_sync;
   int client_game_move_sync_min;
   int client_game_move_sync_err;

   int server_game_move_sync;
   int server_game_move_sync_min;
   int server_game_move_sync_err;

   int client_cdat_packets_tx;
   int client_sdat_packets_rx;
   int client_sdat_packets_skipped;
   int moves_entered;
   int moves_skipped;
   int moves_skipped_tally;
   int moves_skipped_last_tally;

   // server error sync'd back to client
   int serr_c_sync_err;
   int serr_display_timer;

   int made_active_holdoff;
   int join_stdf_sent;

   int join_frame;
   int quit_frame;
   int quit_reason;

   int stdf_rx;
   int stdf_on_time;
   int stdf_late;
   int dif_corr;

   // server only - next client to send stdf to
   int n_stdf;

   // used only to display on server screen overlay in client grid
   int num_dif_packets;
   int cmp_dif_size;


   // bandwidth counters and tallies

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

};

// ------------------------------------------------
// ---------------- lifts -----------------------
// ------------------------------------------------
extern int num_lifts;
extern char lift_step_type_name[10][10];
extern struct lift lifts[NUM_LIFTS];
extern struct lift_step lift_steps[NUM_LIFTS][40];
struct lift
{
   al_fixed fx;
   al_fixed fy;
   al_fixed fxinc;
   al_fixed fyinc;

   al_fixed fw;
   al_fixed fh;
   al_fixed fwinc;
   al_fixed fhinc;

   int x1;
   int y1;
   int x2;
   int y2;
   int width;
   int height;
   int flags;
   int mode;
   int val1;
   int val2;
   int color;
   int current_step;
   int num_steps;
   int limit_counter;
   int limit_type;
   char lift_name[40];
};
struct lift_step
{
   int type;
   int x;
   int y;
   int w;
   int h;
   int val;
};

// ------------------------------------------------
// ---------------- sound -------------------------
// ------------------------------------------------
extern ALLEGRO_VOICE *voice;
extern ALLEGRO_MIXER *mn_mixer;
extern ALLEGRO_MIXER *se_mixer;
extern ALLEGRO_MIXER *st_mixer;
extern ALLEGRO_SAMPLE *snd[20];
extern ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
extern ALLEGRO_AUDIO_STREAM *pm_theme_stream;
extern int fuse_loop_playing;
extern int sample_delay[8];
extern int st_scaler;
extern int se_scaler;
extern int lit_item;
extern int sound_on;


// ------------------------------------------------
// ---------------- level -------------------------
// ------------------------------------------------
extern int l[100][100];
extern int start_level;
extern int play_level;
extern int valid_level_loaded;
extern int last_level_loaded;
extern int resume_allowed;
extern int number_of_starts;

// items
extern int item[500][16];      // item ints
extern al_fixed itemf[500][4]; // item fixeds
extern int item_num_of_type[30];
extern int item_first_num[30];
extern char item_name[30][40];
extern char pmsgtext[500][500];

// enemies
extern int Ei[100][32];        // enemy ints
extern al_fixed Efi[100][16];  // enemy fixeds
extern int e_num_of_type[50];
extern int e_first_num[50];
extern char enemy_name[20][40];
extern int num_enemy;

extern int item_tile[20];
extern int enemy_tile[20];

// PDE
extern int PDEi[100][32];
extern al_fixed PDEfx[100][16];
extern char PDEt[100][20][40];

// bullets
extern int pbullet[50][6];
extern int e_bullet_active[50];
extern int e_bullet_shape[50];
extern al_fixed e_bullet_fx[50];
extern al_fixed e_bullet_fy[50];
extern al_fixed e_bullet_fxinc[50];
extern al_fixed e_bullet_fyinc[50];
extern int pm_bullet_collision_box;


extern int desktop_width;
extern int desktop_height;

extern int disp_x_curr; // current (either wind in windowed mode or full in fullscreen mode)
extern int disp_y_curr;
extern int disp_w_curr;
extern int disp_h_curr;

extern int disp_x_wind; // windowed
extern int disp_y_wind;
extern int disp_w_wind;
extern int disp_h_wind;

extern int disp_x_full; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
extern int disp_y_full;
extern int disp_w_full;
extern int disp_h_full;



extern int SCREEN_W;
extern int SCREEN_H;
extern int WX;
extern int WY;
extern int fullscreen;

// used to only redraw a region of background to increase fps
extern int level_display_region_x;
extern int level_display_region_y;
extern int level_display_region_w;
extern int level_display_region_h;

extern int display_transform_double;

extern int level_editor_running;
extern int help_screens_running;
extern int visual_level_select_running;

extern int show_debug_overlay;

extern int show_player_join_quit_timer;
extern int show_player_join_quit_player;
extern int show_player_join_quit_jq;

extern float scale_factor;
extern float scale_factor_current;
extern float scale_factor_inc;
extern int show_scale_factor;

extern int show_splash_screen;
extern int splash_screen_done;

extern int mdw_an_seq;   // mdw animation sequence number
extern float points[9][8];  // for mdw logo
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

// position and size of map on menu screen
extern int menu_map_size;
extern int menu_map_x;
extern int menu_map_y;

extern int db;  // level editor zoom fullscreen map double
extern int txc; // center of right hand side panel in level editor

// game map
extern int game_map_on;
extern int map_x;
extern int map_y;
extern int map_size;
extern int new_size;


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// -------- Function prototypes ----------------------------------------
// ---------------------------------------------------------------------
// -------- (grouped by source file) -----------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

//e_bitmap.h
int select_bitmap(int);
int select_bitmap_ans(int zzindx);
void animation_sequence_editor(void);
void copy_tiles(void);
void edit_btile_attributes(void);


void draw_flag_text(int x, int y, int ys, int col, int last_flag_draw);

// this is the common one, called by all
// if mouse is on button, return button number
int draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int last_flag_draw);

// this is for copy bitmap and only only affects sa[][]
void draw_and_proc_flag_rects_for_sa(int tn, int x, int y, int w, int h, int ys);

// this is only for draw_item
void draw_and_proc_flag_rects_draw_item(int x, int y, int w, int h, int ys);



int draw_and_process_button(int x, int y, const char * text, int c1, int c2, int center);






// e_editor_main.h
void get_item_draw_shape(int i);
void draw_item_shape(int i, int x, int y);
void draw_enemy_shape(int e, int x, int y);
void draw_PDE_shape(int pde, int x, int y);
void show_draw_item_cursor(void);
void set_block_range(void);
void get_new_box(void);
void update_editor_background(void);
int process_scrolledge(void);

char* get_text_description_of_block_based_on_flags(int flags);
void draw_item_info(int x, int y, int color, int type, int num);
int edit_menu(int el);


// e_editor_zfs.h
void clear_ft(void);
int load_selection(void);
void save_selection(int save);
int enforce_limit(int val, int ll, int ul);
int check_limit(int val, int ll, int ul);
void do_fcopy(int qx1, int qy1);
void do_clear(void);
void draw_fsel(void);
void pointer_text(int x, int y, int ty);
void do_brf(int x, int y, int flood_block);
int zoom_full_screen(int wx, int wy, int draw_item);

// e_group_edit.cpp
void group_edit(void);


void ovw_get_block_position_on_map(int*x, int*y, int *hx, int *hy);





// e_fnx.h
void printBits(size_t const size, void const * const ptr);
al_fixed get_sproingy_jump_height(int num);
void set_xyinc_rot(int EN, int x2, int y2);
void set_rocket_rot(int num, int x2, int y2);
void set_wx(int x, int y);
void set_wx_from_start_block(void);
void show_big(void);
void draw_big(int draw_lifts);
void draw_cloner_boxes(int num);
void draw_bs(int cc);
int getbox(const char *txt, int obj_type, int sub_type, int num );


int get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2);



int getxy(const char *txt, int obj_type, int sub_type, int num );
void clear_pm_events(void);
int check_clt_for_event(int ev, int clt[][4], int clt_last);
int get_unused_pm_event_extended(int clt[][4], int clt_last);
int add_item_link_translation(int sel_item_num, int sel_item_var, int sel_item_ev, int clt[][4], int clt_last);
int is_pm_event_used(int ev);
int get_unused_pm_event(void);
int get_trigger_item(const char *txt, int obj_type, int sub_type, int num );
void find_and_show_event_links(int i);
int get_item(const char *txt, int obj_type, int sub_type, int num );

void crosshairs_full(int cx, int cy, int color, int line_width);
void crosshairs(int mx, int my, int x, int y, int color);
void crosshairs_nodb(int mx, int my, int x, int y, int db, int color);
void title(const char *txt, int y, int tc, int fc);

// e_glt.h
void show_block_list(void);
void global_level();

// e_item.h
void show_all_items(void);
int sort_item(void);
int get_empty_item(void);
int get_empty_item(int type);
void erase_item(int num);
void test_items(void);
int create_pmsg(int c);
void display_pop_message(int c, char *f, int xpos_c, int ypos, int redraw_map, int show_line_breaks);
void show_all_pmsg(void);
int create_key(int c);
int create_start_block(int c);
int create_exit(int c);
int create_door(int type);
int create_field(void);
int create_item(int type);

// e_enemy.h
void show_all_enemies(void);
void sort_enemy(void);
int get_empty_enemy(void);
int get_empty_enemy(int type);
int move_trigger_box(int num, int type);
void recalc_pod(int EN);
void get_pod_extended_position(int e, int *x, int *y);
int move_pod_extended(int num);
int create_cloner(void);
int create_pod(void);


// e_object_viewer.h
int create_obj(int obt, int sub_type, int sent_num);
int obj_buttons(int xa, int xb, int ty, int a, int bts, int obt, int num);
void title_obj(int obj_type, int sub_type, int num, int legend_highlight, int highlight_color);
int move_obt_with_map(int obt, int type, int num);
void object_viewer(int obt, int num);



// e_object_viewer_window.h
void object_viewerw(int obt, int num);




// e_lift.h
int lift_find_previous_move_step(int lift, int step);
al_fixed lift_get_distance_to_previous_move_step(int lift, int step);

void show_all_lifts(void);
void erase_lift(int lift);
void delete_lift_step(int lift, int step);
void lift_setup(void);
void draw_step_button(int xa, int xb, int ty, int ty2, int lift, int step, int rc);
int draw_steps(int step_ty, int lift, int current_step, int highlight_step);
void highlight_current_lift(int lift);
int create_lift(void);
void move_lift_step(int lift, int step);
int redraw_get_new_lift_step_menu(int sty, int step, int highlight);
int get_new_lift_step(int lift, int step);
int insert_lift_step(int lift, int step);
void insert_steps_until_quit(int lift, int step);
void step_popup_menu(int lift, int step);
void set_bts(int lift);
void redraw_lift_viewer(int lift, int step);
int lift_viewer(int lift);

// e_pde.h
int load_PDE();
void save_PDE();
int bottom_menu(int menu_num);
int edit_int(int x, int y, int val, int inc, int lv, int uv);
al_fixed edit_fix(int x, int y, al_fixed val);
void PDE_swap(int s1, int s2);
void PDE_sort(void);
void predefined_enemies(void);
void check_s_window_pos(int reset_pos);
int process_status_window(int draw_only);
int process_select_window(int draw_only);
void set_swbl(void);

// e_sliders.h
void update_var(int bn, int type, int num, float f);
void fill_smsg_button(int bn, int obt, int type, int num);
void fill_smsg_slider(int bn, int type, int num);
void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);

void mdw_slider     (int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );

void mdw_slider2_int(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt);

void mdw_slider2_fix(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 al_fixed &var, float sul, float sll, float sinc, const char *txt);

void mdw_slider2_flt(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt);

void mdw_slider0_int(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2);




int  mdw_button( int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
int  mdw_buttont(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
void mdw_buttonp(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var);






void mdw_colsel (int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 );
int mdw_toggle  (int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_color0, int frame_color1);
int mdw_togglf  (int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, int flags, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_color0, int frame_color1);



// e_visual_level.h
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



// n_client.h
int  ClientInitNetwork(const char *serveraddress);
void ClientExitNetwork(void);
int  ClientCheckResponse(void);
int  ClientReceive(void *data);
void ClientSend(void *data, int len);
void client_flush(void);
int  client_init_join(void);
void client_exit(void);
int  client_init(void);
void client_read_game_move_from_packet(int x, int clf_check);
int  client_process_stdf_packet(void);
void client_apply_diff();
void client_block_until_initial_state_received(void);
void client_process_sdat_packet(void);
void client_process_serr_packet(void);
void client_proc_player_drop(void);
void client_control(void);
void client_local_control(int p);
void process_bandwidth_counters(int p);

// n_network.h
extern int NetworkDriver;
int NetworkInit();

// n_packet.h
extern char packetbuffer[1024];
extern int packetsize;
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

//n_server.h
int ServerInitNetwork(void);
void ServerExitNetwork(void);
int ServerListen(void);
int ServerReceive(void *data, int *sender);
void ServerBroadcast(void *data, int len);
void ServerSendTo(void *data, int len, int who);
void server_flush(void);
int  server_init(void);
void server_exit(void);
void server_send_stdf(int p);
void server_send_stdf(void);
void server_send_sdat(void);
void server_proc_player_drop(void);
void server_proc_cdat_packet(void );
void server_proc_stak_packet(void );
void server_proc_sdak_packet(void );
void server_proc_CJON_packet(int who);
void server_control();
void server_local_control(int p);

// z_bullets.h
void proc_pbullet_shoot(int p);
void proc_pbullet_collision(int p, int b);
void move_pbullets(void);
void draw_pbullets(void);
void proc_ebullet_collision(int p, int b);
void move_ebullets(void);
void draw_ebullets(void);
void clear_bullets(void);

// z_config.cpp
void save_config(void);
void get_config_values(void);

// z_control.h
int getJoystickNum(ALLEGRO_JOYSTICK* joy);
int get_scan_code_from_joystick(int joy, int b_a, int num);
int my_readkey(void);
void clear_keys(void);
void get_all_keys(int p);
void test_keys(void);
void set_start_level(int s);
void set_speed(void);
void clear_controls(int p);
void set_controls_from_comp_move(int g);
void set_comp_move_from_player_key_check(int p);
void set_controls_from_player_key_check(int p);
void function_key_check(void);
void rungame_key_check(int p, int ret);
void add_game_move(int pc, int type, int data1, int data2);
void proc_player_state_game_move(int x);
void proc_game_move(void);
void serial_key_check(int key);
void set_controls_from_game_move(int p);
int proc_events(ALLEGRO_EVENT ev, int ret);
int proc_controllers(void);

// z_enemy.h
int enemy_data(int x_pos, int y_pos);
void get_enemy_draw_shape(int e);

void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color);
void draw_enemies(void);

//void proc_enemy_collision_with_player(int p);
void proc_enemy_collision_with_pbullet(void);
void enemy_flapper(int e);
void enemy_block_walker(int e);
void enemy_cloner(int e);
void set_trakbot_mode(int EN, int mode);
void enemy_trakbot(int e);
void enemy_podzilla(int e);
void enemy_cannon(int e);
void enemy_bouncer(int e);
void enemy_archwagon(int e);
void enemy_deathcount(int e);
void move_enemies(void);
void enemy_killed(int EN);
void enemy_player_hit_proc(int EN);

void enemy_field(int e);
void proc_field_collision(int t, int p, int x);
void draw_enemy_field(int e);

void detect_field_collisions(void);
void set_field_location_from_lift(int e, int dt, int a20);


// z_file.h
void save_sprit(void);
void load_sprit(void);
void make_filename(int x);
int load_level_prompt(void);
int save_level_prompt(void);
int load_tiles(void);
void zero_level_data(void);
void level_check(void);
int load_level(int level_to_load, int display);
int save_level(int level_to_save);
int mw_file_select(const char * title, char * fn, const char * ext, int save);
char* cmtos(int cm);
void save_gm_txt(char *sfname);
void save_gm_gm(char *sfname);
void save_gm(void);
void blind_save_game_moves(int d);
int load_gm(const char *sfname);

// z_fnx.h
int round20(int val);
void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle);
void clear_game_moves(void);
void get_hostname(void);
void make_palette(void);
void m_err(const char * err_msg);
void window_title(void);
int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);
void tsw(void);
void set_frame_nums(int fn);
void reset_animation_sequence_frame_nums(int fn);
void update_animation(void);
al_fixed get_rot_from_xyinc(int EN);
al_fixed get_rot_from_PXY(int EN, int p);
void seek_set_xyinc(int EN);
void seek_set_xyinc(int EN, int x, int y);
int find_closest_player_flapper(int EN, int dir);
int find_closest_player_quad(int EN, int quad, int prox);
int find_closest_player(int EN);
void fire_enemy_bulleta(int EN, int bullet_ans, int p);
void fire_enemy_bulletb(int EN, int bullet_ans, int p);
void fire_enemy_x_bullet(int EN, int p);
int is_right_solid(int solid_x, int solid_y, int lift_check, int type);
int is_left_solid(int solid_x, int solid_y, int lift_check, int type);
int is_down_solid(int solid_x, int solid_y, int lift_check, int type);
int is_up_solid(int solid_x, int solid_y, int lift_check, int type);
al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir);
void show_var_sizes(void);
void pml_to_var(char * b);
void var_to_pml(char * b);
void game_vars_to_state(char * b);
void state_to_game_vars(char * b);
void get_state_dif(char *a, char *b, char *c, int size);
void apply_state_dif(char *a, char *c, int size);
void reset_states(void);
void show_state_dif(char *a, char *b);
int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra);
void demo_mode(void);
void temp_test(void);

// z_item.h
int item_data(int x_pos, int y_pos);
void change_linked_door_color_and_shape(int door);
void remove_block(int x, int y);
void draw_pop_message(int c);
void draw_door(int c, int x, int y);
void draw_items(void);
void move_items(void);
int player_drop_item(int p);
void proc_player_carry(int p);
void proc_item_collision(int p, int x);
void proc_lit_bomb(int);
void proc_lit_rocket(int);

void process_trigger(int);
void draw_trigger(int);
void set_item_trigger_location_from_lift(int, int);
void detect_trigger_collisions(int i);

void proc_start_collision(int p, int i);


void process_block_manip(int i);
void draw_block_manip(int i);

void proc_item_damage_collisions(int i);
void set_item_damage_location_from_lift(int , int);
void process_block_damage(int i);
void draw_block_damage(int i);






// z_lift.h
int construct_lift(int l, char* lift_name);
void clear_lift(int l);
int construct_lift_step(int lift, int step, int type, int x, int y, int w, int h, int val);
void clear_lift_step(int lift, int step);
void set_lift_to_step(int lift, int step);
void draw_lift_lines(void);
void draw_lift(int l, int x1, int y1, int x2, int y2);
void draw_lifts(void);
void set_lift_xyinc(int d, int step);
void move_lifts(int ignore_prox);

// z_log.h
void log_bandwidth_stats(int p);
void log_reason_for_client_quit(int p);
void log_time_date_stamp(void);
void log_versions(void);
void log_player_array(void);
void log_ending_stats(void);
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

// z_logo.h
void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt);
void draw_title(int tx, int ty, int ttw, int tth, int color);
void draw_demo_mode_overlay(void);
void draw_large_2lines(ALLEGRO_FONT *tf, const char * m1, const char * m2, int color, float opa );
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

void scaled_tile_test(void);


// z_loop.h
void proc_frame_delay(void);
void proc_level_done(void);
void proc_start_mode(int start_mode);
void game_loop(int start_mode);

// z_main.h
void final_wrapup(void);
void fast_exit(int why);
int initial_setup(void);
void game_menu(void);
int main(int argument_count, char **argument_array);
int copy_files_to_clients(int exe_only);

// z_map.h
void set_map_position(void);
void next_map_mode(void);
void next_map_size(void);
void draw_map(void);

// z_menu.h
int load_help(void);
void chop_first_x_char(char *str, int n);
void help(const char *topic);
int zmenu(int menu_num, int menu_pos, int y);
void menu_setup(void);
void set_key_menu(int menu, int p, int start_row);
int pmenu(int menu_num, int bg_color);
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore, int rot);
int edit_pmsg_text(int c, int new_msg);
void edit_server_name(void);
int edit_lift_name(int lift, int step_ty, int bts, char *fst);

// z_player.h
void set_player_start_pos(int p, int cont);
int is_player_riding_rocket(int p);
void reset_player_scale_and_rot(int p);
void proc_player_health(int p);
void move_players(void);
void draw_player(int p);
void draw_players(void);
void get_players_shape(int p);
int is_player_color_used(int color);
void fill_door_tile(void);
void fill_player_tile(void);
void init_player(int p, int t);


// z_display.h





void show_bitmap_flags(int flags);
void show_pixel_format(int df);
void show_display_flags(int flags);
void show_display_options(void);
void show_display_orienation(void);
void show_fullscreen_modes(void);
void auto_set_display_transform_double(void);
void set_display_transform(void);



void show_disp_values(void);
void proc_display_change_tofs(void);
void proc_display_change_fromfs(void);
void proc_display_change(void);
int init_display(void);

void save_display_window_position(void);



// z_screen.h
void create_bmp(void);
void rebuild_bitmaps(void);
void load_fonts(void);
void get_new_background(int full);
void stimp(void);
void stamp(void);
void get_new_screen_buffer(int type, int x, int y);
void set_map_var(void);
void set_scale_factor(int instant);
void init_level_background(void);
void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);
void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor);
void draw_level(void);
void frame_and_title(int show_players);
void proc_scale_factor_change(void);
void rtextout_centre(ALLEGRO_BITMAP *dbmp, char *txt1, int x, int y, int col, float scale, int rot, float op);
void mtextout(char *txt1, int x, int y, float x_scale, float y_scale, int col);
void mtextout_centre(const char *txt1, int x, int y, float x_scale, float y_scale, int col);
void show_level_done(int keypress);
void draw_percent_bar(int cx, int y, int width, int height, int percent);
void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end);

// z_screen_overlay.h
void draw_screen_overlay(void);
void show_player_join_quit(void);
void draw_fps_display(int show_type);
void draw_speed_test_data(void);
void draw_top_display(void);

void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4);
//void dtextout(const char *txt1, int x, int y, int col);
void clear_bmsg(void);
//int bmsg_show_text(const char *txt, int col, int bmsg_length);
//int bmsg_draw_tile(int tn, int bmsg_length);
//int bmsg_draw_player(int p, int bmsg_length);
//int bmsg_draw_enemy(int e_type, int bmsg_length);
//int bmsg_show_health(int h, int bmsg_length);
void new_bmsg(int ev, int x, int y, int z1, int z2, int z3, int z4);
void draw_bmsg();


// z_sound.h
void start_music(int resume);
void stop_sound(void);
void proc_sound(void);
void load_sound(void);
void set_se_scaler(void);
void set_st_scaler(void);
void sound_toggle(void);


