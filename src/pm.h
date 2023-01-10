// pm.h - header file for both the game and the level editor

#define PM_VERSION "7.24.1"






class mWindow
{
   public:
   int x1, y1, x2, y2, w, h;
   int index;
   int active;
   int have_focus;
   int layer;
   int moveable;
   int moving;
   int resizable;
   int hidden;
   int color;
   char title[80];

   // mW[1] - status window only
   int draw_item_num;
   int draw_item_type;
   int point_item_type;
   int point_item_num;
   int show_flag_details;
   int show_non_default_blocks;

   // mW[2] - select window only
   int select_window_block_on;
   int select_window_special_on;
   int select_window_num_special_lines;
   int swbn;  // number of blocks
   int swnbl; // number of block lines (just swbn / 16 + 1)
   int swnbl_cur; // current number of lines shown

   // mW[3] - filter window only
   int collapsed;
   int filter_mode;

   // mW[4] - zfs window only
   int copy_mode;
   int brf_mode;
   int sw;
   int sh;

   // mW[5] - group edit list only
   int show_sel_frame;

   // mW[6] - group edit controls only

   // mW[7] - object viewer window only
   int obt;
   int num;
   int mb;
   int viewer_lock;
   int legend_line;
   int num_legend_lines;
   int pop_msg_viewer_pos;

   // mW[8] - top menu only
   int level_editor_mode;


   // mW[9] - tile helper only
   int th_add_del;
   int th_match;
   int th_group;
   int th_sel;


   mWindow(); // default constructor
   void set_pos(int, int);
   void set_size(int, int);
   void draw(int draw_only);
   int detect_mouse(void);
   void set_title(const char*);
   void process_mouse(void);
   void check_offscreen(void);
   void set_focus(int);
};

#define NUM_MW 10
extern mWindow mW[NUM_MW];

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

#define PM_COMPMOVE_LEFT   0b0000000000000001
#define PM_COMPMOVE_RIGHT  0b0000000000000010
#define PM_COMPMOVE_UP     0b0000000000000100
#define PM_COMPMOVE_DOWN   0b0000000000001000
#define PM_COMPMOVE_JUMP   0b0000000000010000
#define PM_COMPMOVE_FIRE   0b0000000000100000
#define PM_COMPMOVE_MENU   0b0000000001000000

#define PM_ENEMY_VINEPOD_SHOW_PATH   0b00000000000000001
#define PM_ENEMY_VINEPOD_INV_INIT    0b00000000000000010
#define PM_ENEMY_VINEPOD_INV_EXTN    0b00000000000000100
#define PM_ENEMY_VINEPOD_INV_MOVE    0b00000000000001000

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

#define PM_ITEM_ORB_STATE       0b0000000000000001
#define PM_ITEM_ORB_PREV_STATE  0b0000000000000010
#define PM_ITEM_ORB_TGON        0b0000000000000100
#define PM_ITEM_ORB_TGOF        0b0000000000001000
#define PM_ITEM_ORB_TRIG_TOUCH  0b0000000000010000
#define PM_ITEM_ORB_TRIG_UP     0b0000000000100000
#define PM_ITEM_ORB_TRIG_DOWN   0b0000000001000000
#define PM_ITEM_ORB_TRIG_BULLET 0b0000000010000000
#define PM_ITEM_ORB_TRIG_CURR   0b0000000100000000
#define PM_ITEM_ORB_TRIG_PREV   0b0000001000000000
#define PM_ITEM_ORB_ROTB        0b1100000000000000

#define PM_ITEM_PMSG_SHOW_ALWAYS   0b00000000000000001
#define PM_ITEM_PMSG_SHOW_SCROLL   0b00000000000000010
#define PM_ITEM_PMSG_TRIGGER_BOX   0b00000000000000100

#define PM_ITEM_PMSG_FRAME0        0b00000000000001000
#define PM_ITEM_PMSG_FRAME1        0b00000000000010000
#define PM_ITEM_PMSG_FRAME2        0b00000000000100000
#define PM_ITEM_PMSG_FRAME4        0b00000000001000000
#define PM_ITEM_PMSG_FRAME12       0b00000000010000000












class mwGraph
{
   public:

   int graph_x1;
   int graph_y1;
   int graph_x2;
   int graph_y2;
   int graph_w;
   int graph_h;

   int plot_x1;
   int plot_y1;
   int plot_x2;
   int plot_y2;
   int plot_w;
   int plot_h;


   int  title_draw_on;

   int  title_draw_style;


   int  title_draw_size;
   int  title_draw_y;
   char title_text[1024];
   int  title_text_color;
   int  title_frame_color;
   void draw_title(int set_size_only);

   void draw_series_legend(void);


   int series_legend_type;
   int series_legend_draw_on;
   int series_legend_size;
   int series_legend_x1;
   int series_legend_x2;
   int series_legend_w;
   int series_legend_y1;
   int series_legend_y2;

   int series_legend_force_solid_lines;

   int series_legend_show_counts;

   int plot_show_performance;


   void set_title(const char* text, int _type, int text_color, int frame_color);

   void set_x_axis_legend(const char* text, int font, int text_color, int frame_color);
   void set_y_axis_legend(const char* name, const char* units, int font, int text_color, int frame_color);

   void set_x_axis_labels(int type, int font, int tick_size, int color);
   void set_y_axis_labels(int type, int font, int tick_size, int color);

   void set_series(int s, const char* text, int color1, int color2);
   void set_series_legend_type(int type);


   int linked_group_id;
   double x_axis_cursor_pos;


   int  x_axis_slave;
   int  x_axis_type;
   int  x_axis_draw_size;

   int x_axis_grid_draw_on;
   int x_axis_grid_color;
   int x_axis_grid_label_draw_on;
   int x_axis_grid_label_font;
   int x_axis_grid_label_color;
   int x_axis_grid_label_draw_size;
   int x_axis_grid_label_tick_size;
   int x_axis_grid_label_text_size;

   int  x_axis_legend_draw_on;
   int  x_axis_legend_draw_size;
   int  x_axis_legend_font;
   int  x_axis_legend_draw_y;
   char x_axis_legend_text[1024];
   int  x_axis_legend_text_color;
   int  x_axis_legend_frame_color;
   double x_axis_divider;
   char x_axis_units_text[80];




   int x_axis_scrollbar_draw_on;
   int x_axis_scrollbar_size;
   int x_axis_scrollbar_draw_size;
   int x_axis_scrollbar_x1;
   int x_axis_scrollbar_x2;
   int x_axis_scrollbar_w;
   int x_axis_scrollbar_bar_x1;
   int x_axis_scrollbar_bar_x2;
   int x_axis_scrollbar_y1;
   int x_axis_scrollbar_y2;
   bool x_axis_lock_scroll;
   bool x_axis_lock_zoom;

   void x_axis_draw(void);
   void x_axis_get_size_and_arrange_pos(void);
   void x_axis_draw_legend(int set_size_only);
   void x_axis_draw_gridlines_and_labels(int set_size_only);
   int  x_axis_draw_scrollbar(int set_size_only);
   void x_axis_proc_scrollbar(int draw_only);


   int  y_axis_slave;

   int y_axis_type;
   int y_axis_draw_size;

   int y_axis_grid_draw_on;
   int y_axis_grid_color;
   int y_axis_grid_label_draw_on;
   int y_axis_grid_label_font;
   int y_axis_grid_label_color;
   int y_axis_grid_label_draw_size;
   int y_axis_grid_label_tick_size;
   int y_axis_grid_label_text_size;

   int  y_axis_legend_draw_on;
   int  y_axis_legend_draw_size;
   int  y_axis_legend_font;
   int  y_axis_legend_draw_x;
   char y_axis_legend_name[1024];
   char y_axis_legend_units[1024];
   int  y_axis_legend_text_color;
   int  y_axis_legend_frame_color;
   double y_axis_divider;



   int y_axis_scrollbar_draw_on;
   int y_axis_scrollbar_size;
   int y_axis_scrollbar_draw_size;
   int y_axis_scrollbar_x1;
   int y_axis_scrollbar_x2;
   int y_axis_scrollbar_w;
   int y_axis_scrollbar_bar_y1;
   int y_axis_scrollbar_bar_y2;
   int y_axis_scrollbar_y1;
   int y_axis_scrollbar_y2;
   int y_axis_scrollbar_h;
   bool y_axis_lock_scroll;
   int y_axis_zoom_lock;

   void y_axis_draw(void);
   void y_axis_get_size_and_arrange_pos(void);
   void y_axis_set_pos(void);
   void y_axis_draw_legend(int set_size_only);
   void y_axis_draw_gridlines_and_labels(int set_size_only);
   int  y_axis_draw_scrollbar(int set_size_only);
   void y_axis_proc_scrollbar(int draw_only);


   struct data_series
   {
      double data[10000][2];
      int num_data;
      int active;
      int color1;
      int color2;
      char name[256];
      int min_visible_index;
      int max_visible_index;
   };
   data_series series[20] = {0};

   float plot_line_size;
   float plot_point_size;

   void set_series_min_and_max_visible_indexes(int s);

   // min, max, and range for entire set of data
   double x_data_min;
   double x_data_max;
   double x_data_rng;
   double y_data_min;
   double y_data_max;
   double y_data_rng;

   // min, max, and range for axis
   double x_axis_min;
   double x_axis_max;
   double x_axis_rng;
   double y_axis_min;
   double y_axis_max;
   double y_axis_rng;

   double x_axis_rng_min;
   double y_axis_rng_min;


   int  convert_sxy_to_gxy(      int sx, int sy, double &gx, double &gy);
   int  convert_gxy_to_sxy(double gx, double gy, double &sx, double &sy);


   mwGraph(); // default constructor
   void initialize(void);
   void set_graph_pos(int x1, int y1, int x2, int y2);

   void draw_point_data(int x, int y, double mx, double my, int color, ALLEGRO_FONT *f, int s);

   void draw_graph(int draw_only);
   void proc_graph(void);

//   void draw_plot(int type);
//   void process_mouse_on_plot(void);
   void proc_plot_area(int draw_only);

   void add_data_point(int series, double x, double y);
   void calc_data_range(void);

   void autorange_axis(int x, int y);
   void set_range_axis(double x_min, double x_max, double y_min, double y_max);
   void enforce_axis_limits(void);
   void enforce_axis_limits(int type);

   char* x_axis_get_val_text(double val, int units);
   char* y_axis_get_val_text(double val, int units);

   int find_closest_point_to_mouse(int &s, int &i);





};
extern mwGraph mG[10];



























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



extern int program_state;
extern int new_program_state;
extern int old_program_state;
extern int program_update;
extern int program_update_1s;
extern int top_menu_sel;
extern int main_loop_exit;

extern int pm_event[1000];

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

extern int deathmatch_pbullets;
extern int deathmatch_pbullets_damage;
extern int suicide_pbullets;


#define STATE_SIZE 112928

// server's copies of client states
extern char srv_client_state[8][2][STATE_SIZE];
extern int srv_client_state_frame_num[8][2];

// server's copy of last stdf state
extern char srv_stdf_state[4][STATE_SIZE];
extern int srv_stdf_state_frame_num[4];

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
extern int last_fps_frame_num;
extern int frame_speed;
extern int frame_num;

// global game control
extern int next_level;

// some global strings
extern char level_filename[80];
extern char local_hostname[80];
extern char pm_version_string[80];
extern char al_version_string[80];
extern char global_string[20][25][80];
extern char msg[1024];
extern char color_name[16][20];

// animation sequence array
extern int zz[20][NUM_ANS];


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
// ----- level editor unsorted --------------------
// ------------------------------------------------

extern int swbl[NUM_SPRITES][2];

extern int bx1;  // selection window
extern int by1;
extern int bx2;
extern int by2;

extern int gx; // mouse position relative to scaled level background
extern int gy;
extern int hx;
extern int hy;

#define NUM_OBJ 600
// list of objects to edit as a group
extern int obj_list[NUM_OBJ][3];
extern int obj_filter[5][20];



// ------------------------------------------------
// ----------------- demo mode --------------------
// ------------------------------------------------
extern ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
extern int demo_played[100];
extern int num_demo_filenames;
extern int demo_mode_loaded;


extern int demo_mode_on;
extern int demo_mode_countdown;
extern int demo_mode_countdown_val;
extern int demo_mode_countdown_reset;
extern int demo_mode_enabled;
extern int demo_mode_last_frame;



// ------------------------------------------------
// ----------------- mouse and keys ---------------
// ------------------------------------------------

#define MAP_LOCK_KEY ALLEGRO_KEY_Z

extern char skc[64]; // serial key check
extern int skc_index;


extern char *key_names[];
extern bool key[ALLEGRO_KEY_MAX][4];
extern int key_pressed_ASCII;

extern float mouse_loop_pause;

extern int mouse_x;
extern int mouse_y;
extern int mouse_z;
extern int mouse_dx;
extern int mouse_dy;
extern int mouse_dz;
extern bool mouse_b[5][4];



// ------------------------------------------------
// ----------------- setup ------------------------
// ------------------------------------------------
extern ALLEGRO_TIMER * fps_timer;
extern ALLEGRO_TIMER * sec_timer;
extern ALLEGRO_TIMER * mnu_timer;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_COLOR palette_color[256];
extern int flash_color;
extern int flash_color2;
extern int flash_counter;
extern ALLEGRO_FONT *font0;
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

extern ALLEGRO_BITMAP *ft_bmp;  //  file temp paste bmp

extern ALLEGRO_BITMAP *logo_ichael;
extern ALLEGRO_BITMAP *logo_avid;
extern ALLEGRO_BITMAP *logo_eiss;
extern int logo_text_bitmaps_create;

extern ALLEGRO_BITMAP *text_title;
extern int text_title_bitmaps_create;
extern int text_title_draw_color;

extern ALLEGRO_BITMAP *large_text_overlay_bitmap;
extern int large_text_overlay_state;







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


extern int L_LOGGING_NETPLAY;
extern int L_LOGGING_NETPLAY_JOIN;
extern int L_LOGGING_NETPLAY_PLAYER_ARRAY;
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
   int control_method; // 0 = local, 1 = file play, 2 = remote view; 3 = server_local; 4 = client_local
   int paused;
   int paused_type; // 1 = death, 2 = door move
   int paused_mode;
   int paused_mode_count;

   int level_done_mode;
   int level_done_timer;
   int level_done_ack;
   int level_done_x;
   int level_done_y;
   int level_done_player;


   al_fixed PX, PY;       // players position
   al_fixed xinc, yinc;   // players momentum
   al_fixed right_xinc, left_xinc;

   al_fixed LIFE;
   al_fixed old_LIFE;

   int on_ladder;
   int on_rope;
   int player_ride;
   int carry_item;


   int shape; // index to player_tile
   int color; // used to draw frames and stuff in players color
   al_fixed draw_rot;
   al_fixed draw_scale;
   int door_draw_rot_num_steps;
   al_fixed door_draw_rot;
   al_fixed door_draw_rot_inc;
   int marked_door;
   int door_item;
   int door_xinc;
   int door_yinc;
   int door_num_steps;


   int spawn_point_index;

   int left_right; // determines the direction the player is facing


   // flags that indicate a control has been activated or held
   int up, down, left, right, jump, fire, fire_held, menu;

   int bullet_wait_counter, request_bullet, bullet_wait, bullet_speed;




   int stat_respawns;
   int stat_bullets_fired;
   int stat_enemy_hits;
   int stat_player_hits;
   int stat_self_hits;
   int stat_purple_coins;

   int stat_enemy_exploded;
   int stat_player_exploded;
   int stat_self_exploded;

   al_fixed stat_LIFE_inc;
   al_fixed stat_LIFE_dec;
   al_fixed stat_LIFE_wasted;

   int spare_int1;
   int spare_int2;

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
   int frames_skipped_last_sec;
   int frames_skipped_last_sec_tally;

   int timer_adjust;
   int timer_adjust_last_sec;
   int timer_adjust_last_sec_tally;


   int who; // for network id of clients
   char hostname[16];

   int server_last_stak_rx_frame_num; // used by server to see if client is still responding
   int client_last_stdf_rx_frame_num; // used by client to see if server is still responding


   int server_send_dif; // used by server to determine which frame to send dif


   int client_base_resets;

   int client_last_dif_applied;

   int client_sync;
   int server_sync;

   double ping;
   double ping_avg;

   double dsync;

   double client_chase_offset;



   int sc_sync; // packet stdf from s to c  sfn-cfn
   int cs_sync; // packet stak from c to s  cfn-sfn
   int rt_sync; // round trip to server back to server through client via stdf and stak

   int last_gm_index;
   int last_gm_frame_num;
   int last_gm_comp_move;


   double client_chase_fps;
   int server_game_move_sync;

   int client_cdat_packets_tx;



   int made_active_holdoff;
   int sync_stabilization_holdoff;

   int join_frame;
   int quit_frame;
   int quit_reason;


   // server rewind and stdf frequencies
   int s1;
   int s2;






   // used only to display on server screen overlay in client grid
   int num_dif_packets;
   int cmp_dif_size;

   int late_cdats;
   int late_cdats_last_sec;
   int late_cdats_last_sec_tally;


   double game_move_dsync; // current value

   double game_move_dsync_max;

   double game_move_dsync_avg_last_sec;
   double game_move_dsync_avg_last_sec_tally;
   double game_move_dsync_avg_last_sec_count;








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





extern struct packet_buffer packet_buffers[200];
struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int who;
   int packetsize;
   char data[1024];
};


extern double timestamp_frame_start;



extern int timestamps_index;
extern struct timestamp timestamps[10000];
struct timestamp
{
   int frame0;
   int frame1;
   int frame2;
   int type;
   double t0;
   double t1;
   double t2;
};


extern double ping_array[8];
extern int    ping_num_filled;
extern int    ping_index;






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



// bullets
extern int pbullet[50][6];
extern int pm_bullet_collision_box;

extern struct ebullet ebullets[50];
struct ebullet
{
   int active;
   int shape;
   al_fixed fx;
   al_fixed fy;
   al_fixed fxinc;
   al_fixed fyinc;
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

extern int thl[100][100]; // tile helper

extern int warp_level_location;

extern int start_level;
extern int play_level;
extern int valid_level_loaded;
extern int last_level_loaded;
extern int resume_allowed;
extern int number_of_starts;
extern int number_of_purple_coins;



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
extern int display_adapter_num;
extern float WX_shift_speed;


// used to only redraw a region of background to increase fps
extern int level_display_region_x;
extern int level_display_region_y;
extern int level_display_region_w;
extern int level_display_region_h;

extern int display_transform_double;
extern int saved_display_transform_double;
extern int show_dtd;







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

extern int scale_factor_holdoff;




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

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// -------- Function prototypes ----------------------------------------
// ---------------------------------------------------------------------
// -------- (grouped by source file) -----------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

// e_bitmap.h
void color_shiftc(ALLEGRO_BITMAP *b, int sc, int cs, int x, int y);
void color_shift4(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3, int cs4);
void color_shift3(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3);
void color_shift2(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2);
void color_shift(ALLEGRO_BITMAP *b, int sc, int cs);
void colorize_tile(void);
void combine_tile(void);
int select_bitmap(int tn);
int select_bitmap_ans(int zzindx);
void animation_sequence_editor(void);
void redraw_grid(int x, int y, int current_selection);
void draw_flag_text(int x, int y, int ys, int col, int last_flag_show);
int draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int last_flag_show);
void draw_and_proc_flag_rects_for_sa(int tn, int x, int y, int w, int h, int ys);
void draw_flags(int x1, int y1, int* num, int *mpow, int view_only, int clear_background, int ignore_mpow);
void draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight);
void edit_btile_attributes(void);
void draw_gridlines_and_frame(int x1, int y1, int x2, int y2, int fd, int fc, int fw, int gd, int gc, int gw);
int draw_and_process_button(int x, int y, const char * text, int c1, int c2, int center);
void copy_tiles(void);

// e_editor_main.h
void em_set_swbl(void);
void em_set_block_range(void);
char* em_get_text_description_of_block_based_on_flags(int flags);
void em_show_draw_item_cursor(void);
void em_show_item_info(int x, int y, int color, int type, int num);
void em_find_point_item(void);
void em_process_mouse(void);
void em_process_keypress(void);
int edit_menu(int el);

// e_editor_zfs.h
void zfs_pointer_text(int x1, int x2, int y, int mouse_on_window);
void zfs_do_brf(int x, int y, int flood_block);
void zfs_clear_ft(void);
int zfs_load_selection(void);
void zfs_save_selection(int save);
void zfs_do_fcopy(int qx1, int qy1);
void zfs_do_clear(void);
void set_block_with_flag_filters(int x, int y, int tn);
int zfs_draw_buttons(int x3, int x4, int yfb, int have_focus, int moving);
void zfs_draw_item_ft(int i);
void zfs_draw_enemy_ft(int e);
void zfs_draw_lifts_ft();
void zfs_draw_fsel(void);
void zfs_process_mouse(void);

// e_enemy.h
void erase_enemy(int e);
int get_empty_enemy(void);
int get_empty_enemy(int type);
void recalc_pod(int e);
void get_pod_extended_position(int e, int *x, int *y);
void show_level_data(void);
void show_all_enemies(void);
void sort_enemy(void);
int create_cloner(void);
int create_pod(void);
int create_vinepod(void);

// e_fnx.h
int exit_level_editor_dialog(void);
void draw_block_non_default_flags(int tn, int x, int y);
int enforce_limit(int val, int ll, int ul);
int check_limit(int val, int ll, int ul);
void swap_int(int *i1, int* i2);
float mdw_rnd(float rmin, float rmax);
void set_int_3216(int &I32, int H16, int L16);
void get_int_3216(int I32, int &H16, int &L16);
void printBits(size_t const size, void const * const ptr);
al_fixed get_sproingy_jump_height(int num);
int get_sp(float jh);
void set_xyinc_rot(int EN, int x2, int y2);
void set_rocket_rot(int num, int x2, int y2);
int get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2, int type);
void draw_vinepod_controls(int num, int legend_highlight);
int getxy(const char *txt, int obj_type, int sub_type, int num);
void show_event_line(int x, int &y, int ev, int type, int v1, int v2);
void show_all_events(void);
int add_item_link_translation(int sel_item_num, int sel_item_var, int sel_item_ev, int clt[][4], int clt_last);
void clear_pm_events(void);
int check_clt_for_event(int ev, int clt[][4], int clt_last);
int is_pm_event_used(int ev);
int get_unused_pm_event_extended(int clt[][4], int clt_last);
int get_unused_pm_event(void);
void find_and_show_event_links(int type, int i, int num2);
int get_trigger_item(int obj_type, int sub_type, int num);
int get_item(int obj_type, int sub_type, int num);
void crosshairs_full(int x, int y, int color, int line_width);
void titlex(const char *txt, int tc, int fc, int x1, int x2, int y);
void scale_bouncer_and_cannon_speed(int e);

// e_glt.h
void show_block_list(void);
void remove_unused_tiles(void);
void global_level(void);

// e_group_edit_windows.h
void ge_init_data(void);
void ge_set_valid_controls(void);
void ge_add_to_obj_list(int t, int i);
void ge_remove_obj_list_item(int o);
void ge_remove_obj_list_filtered_items(void);
void ge_swap_obj_list_items(int i1, int i2);
void ge_enemy_initial_position_random(int e, int csw);
void ge_item_initial_position_random(int i, int csw);
int ge_draw_list_items(int x1, int y1, int flash_color, int ni);
void ge_show_obj_list(int x, int y, int *ew, int *eh, int have_focus, int moving);
int ge_show_controls(int x, int y, int *ew, int *eh, int have_focus, int moving, int hidden, int draw_only);
void ge_add_selection_to_list(int set_filters);
void ge_process_mouse(void);

// e_item.h
void show_all_items(void);
int sort_item(int set_pos);
int get_empty_item(void);
int get_empty_item(int type);
void check_item(int i, int ct);
void test_items(void);
void erase_item(int num);
int create_trigger(int i);
int create_block_manip(int i);
int create_block_damage(int i);
int create_start_block(int c);
int create_exit(int c);
void show_all_pmsg(void);
int create_pmsg(int c);
int create_door(int type);
int create_item(int type);

// e_lift.h
al_fixed lift_get_distance_to_previous_move_step(int lift, int step);
void show_all_lifts(void);
void lift_step_set_size_from_previous_move_step(int lift, int step);
int lift_find_previous_move_step(int lift, int step);
void erase_lift(int lift);
void delete_lift_step(int l, int step);
void lift_setup(void);
int create_lift(void);
void move_lift_step(int lift, int step);
int get_new_lift_step(int lift, int step);
int insert_lift_step(int lift, int step);
void insert_steps_until_quit(int lift, int step);
void set_all_steps(int l, int s, int what);
void step_popup_menu(int lift, int step);
int draw_current_step_buttons(int x1, int x2, int y, int l, int s, int d);
void draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc, int d);
int draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step, int d);

// e_mWindows.h
void cm_get_block_position_on_map();
void cm_process_scrolledge(void);
void cm_show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text);
void cm_get_new_box(void);
void cm_redraw_level_editor_background(int mode);
void cm_process_mouse(void);
void cm_process_keypress(void);
void cm_redraw_level_editor_background(void);
void cm_process_menu_bar(int have_focus, int moving, int draw_only);
int cm_draw_filter_buttons(int x1, int x2, int y1, int mode, int have_focus, int moving);
void cm_draw_status_window(int x1, int x2, int y1, int y2, int have_focus, int moving);
void cm_draw_selection_window(int x1, int x2, int y1, int y2, int have_focus, int moving);
void set_windows(int mode);
int is_mouse_on_any_window(void);
int mw_get_max_layer(void);
int mw_cycle_windows(int draw_only);

// e_object_viewer_windows.h
int create_obj(int obt, int type, int num);
void ovw_get_size(void);
void set_switch_tile(int i);
void ovw_title(int x1, int x2, int y1, int y2, int legend_highlight);
void ovw_draw_buttons(int x1, int y1, int x2, int y2, int have_focus, int moving, int draw_only);
void ovw_draw_overlays(int legend_highlight);
void ovw_process_mouse(void);
void ovw_set_to_0(void);
void ovw_check_if_valid(int type);
void ovw_process_keypress(void);
void object_viewerw(int obt, int num);

// e_pde.h
int load_PDE();
void save_PDE();
void PDE_swap(int s1, int s2);
void PDE_sort(void);
void PDE_edit_text(int EN);
void predefined_enemies(void);


// e_sliders.h
void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
void draw_slider_text(int x1, int y1, int x2, int y2, int q2, int q5);
float get_slider_position(float sdx, float sul, float sll, int x1, int y1, int x2, int y2);
float get_slider_position2(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2);
float get_slider_position3(float f, float sul, float sll, float sinc, int q4, int x1, int y1, int x2, int y2);
float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);
float draw_slider(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, float sdx, float sul, float sll, int order);
void mdw_slider0(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2);
void mdw_slideri(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt);
void mdw_sliderf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 al_fixed &var, float sul, float sll, float sinc, const char *txt);
void mdw_sliderd(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt);
void set_trigger_event(int i, int ev0, int ev1, int ev2, int ev3);
int get_frame_size(int num);
void set_frame_size(int num, int frame_size);
int mdw_button(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);

int mdw_buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
int mdw_buttont_nb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);



int mdw_buttontt(int x1, int &y1, int x2, int bts, int tn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
void mdw_buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var);
void mdw_colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
int mdw_toggle(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);
int mdw_togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);

// e_tile_helper.h
void th_replace(int type);
int th_draw_buttons(int x3, int x4, int yfb, int have_focus, int moving);
int th_compare_tile(int rb, int cb, int group);
void th_find_connected(int x, int y, int group);
void th_process_mouse(void);

// e_visual_level.h
int lev_show_level_data(int x_pos, int y_pos);
void mark_rect(int sel, int color);
void show_cur(void);
void show_msel(void);
void compare_curr(int sel);
void compare_all(void);
void lev_draw(int full);
void level_viewer(void);
void show_cur_vs(int cur, int x1, int y1, int size, int fc);
void load_visual_level_select(void);
int visual_level_select(void);

// n_client.h
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
void client_read_game_move_from_packet(int x);
void client_process_stdf_packet(double timestamp);
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



//n_server.h


void init_timestamps(void);
void add_timestamp(int type, int f1, int f2, double t1, double t2);
int get_timestamp(int f, int type, double &res);
int get_delta(int f0, int type0, int f1, int type1, double &res);
int get_newest_timestamp(int type, double &res);


int ServerInitNetwork(void);
void ServerExitNetwork(void);
void ServerListen(void);
int ServerReceive(void *data, int *sender);
void ServerBroadcast(void *data, int len);
void ServerSendTo(void *data, int len, int who, int player);
void server_flush(void);
int  server_init(void);
void server_exit(void);

void server_send_stdf(int p);
void server_send_stdf(void);
void server_rewind(void);


void server_send_sdat(void);
void server_proc_player_drop(void);
void server_proc_cdat_packet(double timestamp);
void server_proc_stak_packet(void);
void server_proc_sdak_packet(void);
void server_proc_cjon_packet(int who);
void server_control();

void server_fast_packet_loop(void);


// z_args.cpp
void proc_command_line_args1(int argument_count, char **argument_array);
void proc_command_line_args2(int argument_count, char **argument_array);
int copy_files_to_clients(int exe_only);
void temp_test(void);


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
void load_config(void);

// z_control.h
int getJoystickNum(ALLEGRO_JOYSTICK* joy);
int get_scan_code_from_joystick(int joy, int button, int num);
int my_readkey(void);
void clear_keys(void);
void get_all_keys(int p);
void test_keys(void);
void set_start_level(int s);
void set_speed(void);
void clear_controls(int p);



void set_controls_from_comp_move(int p, int comp_move);


void set_comp_move_from_player_key_check(int p);
void set_controls_from_player_key_check(int p);
void function_key_check(void);
void rungame_key_check(int p);

void add_game_move2(int frame, int type, int data1, int data2);
void add_game_move(int frame, int type, int data1, int data2);

void proc_player_state_game_move(int x);

void proc_game_moves_array(void);

void serial_key_check(int key);

void proc_events(ALLEGRO_EVENT ev);
void proc_player_input(void);
void proc_controllers(void);

// z_display.h
void show_bitmap_flags(int flags);
void show_pixel_format(int df);
void show_display_flags(int flags);
void show_display_options(void);
void show_display_orienation(void);
void show_fullscreen_modes(void);
void auto_set_display_transform_double(void);
void set_saved_display_transform(int sdt);
void cycle_display_transform(void);
void set_display_transform();
void show_disp_values(int fs, int disp, int curr, int wind, int full, char *head);
int init_display(void);
void proc_display_change(void);
void save_display_window_position(void);
void proc_display_change_tofs(void);
void proc_display_change_fromfs(void);

// z_enemy.h
int enemy_data(int x_pos, int y_pos);
void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color);
void draw_enemy(int e, int custom, int cx, int cy);
void draw_enemies(void);
void proc_enemy_collision_with_pbullet(int e);
void move_enemies();
void enemy_deathcount(int e);
void enemy_player_hit_proc(int e);
void enemy_killed(int e);
void enemy_flapper(int e);
int is_player_in_trigger_box(int x1, int y1, int x2, int y2);
void cloner_create(int e);
void enemy_cloner(int e);
void set_trakbot_mode(int e, int mode);
void enemy_trakbot(int e);
void enemy_podzilla(int e);
void enemy_vinepod(int e);
void bouncer_cannon_common(int e);
void enemy_cannon(int e);
void enemy_bouncer(int e);
void enemy_archwagon(int e);
void walker_archwagon_common(int e);
void enemy_block_walker(int e);
void enemy_jumpworm(int e);

// z_file.h
void make_filename(int x);
int load_level_prompt();
int save_level_prompt();
void save_sprit(void);
void load_sprit(void);
void save_mW(void);
void load_mW(void);
int load_tiles(void);
void zero_level_data(void);
void level_check(void);
int load_level(int level_to_load, int load_only);
int save_level(int level_to_save);
int mw_file_select(const char * title, char * fn, const char * ext, int save);
char* cmtos(int cm);
void save_gm_txt(char *sfname);
void save_gm_gm(char *sfname);
void save_gm();
void blind_save_game_moves(int d);
int load_gm(const char *sfname);

// z_fnx.h

int mw_draw_line2(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int &line_color_offset);
int mw_draw_line3(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int c2_val, int c2_col, int &line_color_offset);

int round20(int val);
void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle);
void change_block(int x, int y, int block);
void clear_game_moves(void);
void get_hostname(void);
void process_flash_color(void);
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
int find_closest_player_cannon(int e, int dist);
int find_closest_player(int EN);
void fire_enemy_bulletz(int EN, int bullet_ans, al_fixed px, al_fixed py);
void fire_enemy_bulleta(int EN, int bullet_ans, int p);
void fire_enemy_x_bullet(int EN, int p);
int is_solid(int b, int c, int type);
int is_solidu(int b, int c, int type);
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

// z_game_menu.cpp
void game_menu(void);


// z_item.h
int item_data(int x, int y);
void change_linked_door_color_and_shape(int door);
void draw_pop_message(int i, int custom, int xpos_c, int ypos, int cursor_pos, int cursor_blink, char *f);
void draw_door(int i, int x, int y, int custom);
int seq_color(int mod, int c1, int c2);
int seq_color2(void);
int seq_color3(void);
void bomb_block_crosshairs(int e, int f);
void bomb_blocks(int i, int t, int dr, al_fixed fx, al_fixed fy);
void bomb_crosshairs(float x, float y);
void bomb_enemies(int i, int t, int dr, al_fixed x, al_fixed y);
void bomb_players(int i, int t, int dr, al_fixed x, al_fixed y);
void proc_lit_bomb(int i);
void draw_lit_bomb(int i);
void draw_rocket_lines(int i);
void draw_item(int i, int custom, int cx, int cy);
void draw_items(void);
void proc_pmsg_reset_timer(int i);
void proc_pmsg(int i);
int is_item_stuck_to_wall(int i);
void proc_switch_block_range(int i, int action);
void proc_key_block_range(int i, int action);
void proc_moving_key(int i);
void move_items();
int player_drop_item(int p);
void proc_player_carry(int p);
int proc_orb_bullet_collision(int i);
void proc_orb(int i);
void draw_orb(int i, int x, int y);
void proc_orb_collision(int p, int i);
void proc_door_collision(int p, int i);
void proc_start_collision(int p, int i);
void proc_bonus_collision(int p, int i);
void proc_exit_collision(int p, int i);
void proc_key_collision(int p, int i);
void proc_mine_collision(int p, int i);
void proc_bomb_collision(int p, int i);
void proc_rocket_collision(int p, int i);
void proc_warp_collision(int p, int i);
void proc_switch_collision(int p, int i);
void proc_sproingy_collision(int p, int i);
void proc_item_collision(int p, int i);
void proc_lit_rocket(int i);
void proc_trigger(int i);
void set_item_trigger_location_from_lift(int i, int a20);
void detect_trigger_collisions(int i);
void draw_trigger(int i, int x, int y);
void proc_block_manip(int i);
void draw_block_manip(int i, int x, int y);
void set_item_damage_location_from_lift(int i, int a20);
void proc_item_damage_collisions(int i);
void draw_block_damage(int i, int x, int y, int custom);
void proc_block_damage(int i);

// z_lift.h
int construct_lift(int l, char* lift_name);
void clear_lift(int l);
int construct_lift_step(int l, int s, int type, int x, int y, int w, int h, int val);
void clear_lift_step(int l, int s);
void set_lift_to_step(int l, int s);
int is_player_riding_lift(int l);
void draw_lift_line(int l);
void draw_lift(int l, int x1, int y1, int x2, int y2);
void draw_lifts();
void set_lift_xyinc(int d, int step);
int lift_check_prox(int l, int pd);
void move_lifts(int ignore_prox);

// z_log.h
void log_bandwidth_stats(int p);
void log_reason_for_player_quit(int p);
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

// z_logo.h
void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt);
void draw_title(int tx, int ty, int ttw, int tth, int color);
void draw_large_text_overlay(int type, int color);
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


// z_main.h
void final_wrapup(void);
void fast_exit(int why);
void show_system_id();
void set_and_get_versions(void);
void get_desktop_resolution();
int initial_setup(void);
int main(int argument_count, char **argument_array);


// z_loop.h
void proc_events(ALLEGRO_EVENT ev);
void proc_keys_held(void);
void proc_event_queue(void);
void draw_frame(void);
void move_frame(int t);
void loop_frame(int);
int has_player_acknowledged(int p);
int have_all_players_acknowledged(void);
void proc_level_done_mode(void);
void proc_program_state(void);
void proc_timer_adjust(void);
int proc_frame_skip(void);
void main_loop(void);

// z_menu.h
int load_help(void);
void chop_first_x_char(char *str, int n);
void help(const char *topic);
int tmenu(int menu_num, int menu_pos, int x, int y);
int zmenu(int menu_num, int menu_pos, int y);
int pmenu(int menu_num, int bg_color);
void menu_setup(void);
void set_key_menu(int menu, int p, int start_row);
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore);
int edit_pmsg_text(int c, int new_msg);
void edit_server_name(void);
int edit_lift_name(int lift, int step_ty, int bts, char *fst);

// z_ping_buffer.h
void ping_buffer_clear(void);
void ping_array_add(double ping);






// z_player.h
void set_player_start_pos(int p, int cont);
void proc_player_health(int p);
void proc_player_xy_move_test(int p);
void proc_player_xy_move(int p);
void proc_player_paused(int p);
void reset_player_scale_and_rot(int p);
int is_player_riding_rocket(int p);
void proc_player_stuck_in_blocks(int p);
void proc_player_riding_rocket(int p);
void proc_player_bounds_check(int p);
void proc_player_collisions(int p);
int is_player_within_ladder_reach(int p);
int is_player_within_rope_reach(int p);
void proc_player_rope_move(int p);
void proc_player_ladder_move(int p);
void proc_player_rope(int p);
void proc_player_ladder(int p);
void move_players(void);
void draw_player(int p);
void draw_players(void);
void get_players_shape(int p);
int is_player_color_used(int color);
void init_player(int p, int t);
void fill_player_tile(void);

// z_screen.h
void load_fonts(void);
void create_bitmaps(void);
void rebuild_bitmaps(void);
void get_new_background(int full);
void stimp(void);
void stamp(void);
void get_new_screen_buffer(int type, int x, int y);
void set_map_var(void);
void set_scale_factor(float new_scale_factor, int instant);
void mark_non_default_block(int x, int y);
void init_level_background(int);
void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);
void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor);
void draw_level(void);
void frame_and_title(int show_players);
void proc_scale_factor_change(void);
void rtextout_centre(ALLEGRO_BITMAP *dbmp, char *txt1, int x, int y, int col, float scale, int rot, float op);
void mtextout(char *txt1, int x, int y, float x_scale, float y_scale, int col);
void mtextout_centre(const char *txt1, int x, int y, float x_scale, float y_scale, int col);
void draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc);
void draw_percent_bar(int cx, int y, int width, int height, int percent);
void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent);
void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end);

// z_screen_overlay.h
int dif_from_now_to_nl();
void show_player_stat_box(int tx, int y, int p);
void show_level_done(void);
void draw_screen_overlay(void);
void show_player_join_quit(void);
void draw_fps_display(int show_type);
void draw_speed_test_data(void);
void draw_top_display(void);
void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4);
void dtextout(const char *txt1, int x, int y, int col);
void clear_bmsg(void);
int bmsg_show_text(const char *txt, int col, int bmsg_length);
int bmsg_draw_tile(int tn, int bmsg_length);
int bmsg_draw_tile2(int tn, int bmsg_length, int xo, int yo);
int bmsg_draw_player(int p, int bmsg_length);
int bmsg_draw_enemy(int e_type, int bmsg_length);
int bmsg_show_health(int h, int bmsg_length);
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











