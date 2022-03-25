// zmain.cpp

#include "pm.h"

// --------------- Global Variables ---------------
// all global variables should be declared here and externed in pm.h








// temp testing variable
int tx1=0;
int ty1=0;
int ttc1=0;
float ttfloat1=0;

int tx2=0;
int ty2=0;
int ttc2=0;
float ttfloat2=0;



// ------------------------------------------------
// ----------------- netgame ----------------------
// ------------------------------------------------
int ima_server = 0;
int ima_client = 0;
char m_serveraddress[256] = "192.168.1.2";
int TCP = 0;

int stdf_freq = 40;
int zlib_cmp = 7;
int control_lead_frames = 3;
int server_lead_frames = 1;

int deathmatch_pbullets = 0;
int deathmatch_pbullets_damage = 5;
int suicide_pbullets = 0;

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










// ------------------------------------------------
// ----- visual level select ----------------------
// ------------------------------------------------
ALLEGRO_BITMAP * grid_bmp = NULL;
ALLEGRO_BITMAP * level_icon_bmp[NUM_LEV];
int le[NUM_LEV]; // level exists array
int num_levs;
int sel_x, sel_y, sel_size;
int grid_cols, grid_rows, grid_size, grid_width, grid_height;
int load_visual_level_select_done = 0;


// frame_speed, frames per second, frame_num stuff
int speed_testing = 0;
int actual_fps;
int last_frames_skipped = 0;
int frames_skipped_last_second;
int last_fps_frame_num = 0;
int draw_frame;
int frame_speed = 40;
int frame_num;

// global game control
int level_done_trig = 0;
int level_done_proc = 0;
int next_level = 0;
int game_exit = 1;

// some global strings
char level_filename[80];
char local_hostname[80];

char pm_version_string[80];
char al_version_string[80];


char global_string[20][25][80];
char msg[1024];
char color_name[16][20];

// for log file viewer code to find most recent log file
ALLEGRO_FS_ENTRY *filenames[1000];
int num_filenames;

// animation sequence array
int zz[20][NUM_ANS];

// ------------------------------------------------
// ----- level editor unsorted --------------------
// ------------------------------------------------
char sel_filename[500];
int ty = 46;   // button start
int bts = 12;  // button spacing

// level editor start block UL corner
int wx=0;
int wy=0;
int pop_msg_viewer_pos;
int Redraw = 1;
int Num_legend_lines = 2;
int Viewer_lock = 0;

// ------------------------------------------------
// ----- return values from getxy and getbox ------
// ------------------------------------------------
int bx1, bx2, by1, by2;
int get100_x, get100_y;

// ------------------------------------------------
// ------------ game moves array ------------------
// ------------------------------------------------
int game_moves[GAME_MOVES_SIZE][4];
int game_move_entry_pos = 0;
int game_move_current_pos = 0; // for savegame running only

// ------------------------------------------------
// ------------- screen messages ------------------
// ------------------------------------------------
int bottom_msg_on = 1;
int bottom_msg = 0;
int bmsg_index = 0;
ALLEGRO_BITMAP *bmsg_bmp[20] = {NULL};
ALLEGRO_BITMAP *bmsg_bmp2[20] = {NULL};
int game_event_retrigger_holdoff[10];
ALLEGRO_BITMAP *bmsg_temp;


// ------------------------------------------------
// -----------status and selection window----------
// ------------------------------------------------
// status window
int status_window_active = 1;
int status_window_x = 9999;
int status_window_y = 35;
int status_window_w = 320;
int status_window_h = 43;
int draw_item_num;
int draw_item_type;
int point_item_num;
int point_item_type;

// select window
int select_window_active = 1;
int select_window_x = 9999;
int select_window_y = 100;
int select_window_w = 322;
int select_window_h;
int select_window_text_y;
int select_window_block_on = 1;
int swnbl;
int swnbl_cur = 0;
int swbl[NUM_SPRITES][2];

int select_window_block_y;
int btext_draw_flag;
int select_window_special_on = 1;
int select_window_num_special_lines = 3;
int select_window_special_y;
int stext_draw_flag;
int sw_mouse_gone = 0;

// ------------------------------------------------
// ---------------zoom full screen ----------------
// ------------------------------------------------
int stx=10;  // zfs selection window
int sty=10;
int sux=40;
int suy=30;

int copy_blocks=1;
int copy_enemies=1;
int copy_items=1;
int copy_lifts=1;
int copy_mode = 0;
int brf_mode =0;

int ft_level_header[20];
int ft_l[100][100];
int ft_item[500][16];
char* ft_pmsg[500];
int ft_Ei[100][32];
al_fixed ft_Efi[100][16];

char ft_ln[NUM_LIFTS][80];
int ft_lift[NUM_LIFTS][4];
int ft_ls[NUM_LIFTS][40][4];


// ------------------------------------------------
// ----------------- demo mode --------------------
// ------------------------------------------------
ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
int demo_played[100];
int num_demo_filenames = 0;
int demo_mode_on = 0;
int demo_mode_countdown;

// ------------------------------------------------
// ----------------- mouse and keys ---------------
// ------------------------------------------------

// serial key check
char skc[64];
int skc_index = 0;

bool key[ALLEGRO_KEY_MAX];
int Key_pressed_ASCII;

float mouse_loop_pause = 0;

int mouse_x = 0;
int mouse_y = 0;
int mouse_z = 0;
int mouse_dx = 0;
int mouse_dy = 0;
int mouse_dz = 0;
int mouse_b1 = 0;
int mouse_b2 = 0;
int mouse_b3 = 0;
int mouse_b4 = 0;

int KEY_F1_held = 0;
int KEY_F2_held = 0;
int KEY_F3_held = 0;
int KEY_F4_held = 0;
int KEY_F5_held = 0;
int KEY_F6_held = 0;
int KEY_F7_held = 0;
int KEY_F8_held = 0;
int KEY_F9_held = 0;
int KEY_F10_held = 0;
int KEY_F11_held = 0;
int KEY_F12_held = 0;
int KEY_PRTSCR_held = 0;


// ------------------------------------------------
// ----------------- setup ------------------------
// ------------------------------------------------

ALLEGRO_TIMER * fps_timer;  // used to control the speed of the game
ALLEGRO_TIMER * mnu_timer;  // used to control the speed of the menu
ALLEGRO_TIMER * sec_timer;  // used to count the actual frames per second


ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_COLOR palette_color[256];
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *f1 = NULL;
ALLEGRO_FONT *f2 = NULL;
ALLEGRO_FONT *f3 = NULL;

ALLEGRO_JOYSTICK *joy0 = NULL;
ALLEGRO_JOYSTICK *joy1 = NULL;

// ------------------------------------------------
// ----------------- bitmaps ----------------------
// ------------------------------------------------

ALLEGRO_BITMAP *tilemap = NULL;
ALLEGRO_BITMAP *ptilemap = NULL;
ALLEGRO_BITMAP *dtilemap = NULL;
ALLEGRO_BITMAP *M_tilemap = NULL;
ALLEGRO_BITMAP *M_ptilemap = NULL;
ALLEGRO_BITMAP *M_dtilemap = NULL;


ALLEGRO_BITMAP *tile[NUM_SPRITES] = {NULL};

int sa[NUM_SPRITES][2]; // shape attributes

ALLEGRO_BITMAP *player_tile[16][32] = {NULL};
ALLEGRO_BITMAP *door_tile[2][16][8] = {NULL};


ALLEGRO_BITMAP *level_background = NULL;
ALLEGRO_BITMAP *level_buffer = NULL;

ALLEGRO_BITMAP *dtemp = NULL; // temp draw
ALLEGRO_BITMAP *lefsm = NULL; // level editor fullscreen map

ALLEGRO_BITMAP *mp = NULL;     //  mouse_pointer
ALLEGRO_BITMAP *ft_bmp = NULL;  //  file temp paste bmp

ALLEGRO_BITMAP *logo_ichael = NULL;
ALLEGRO_BITMAP *logo_avid = NULL;
ALLEGRO_BITMAP *logo_eiss = NULL;
int logo_text_bitmaps_create = 1;

ALLEGRO_BITMAP *text_demo = NULL;
ALLEGRO_BITMAP *text_mode = NULL;
int text_demomode_bitmaps_create = 1;

ALLEGRO_BITMAP *text_title = NULL;
int text_title_bitmaps_create = 1;
int text_title_draw_color = -1;


// ------------------------------------------------
// ----------------- logging ----------------------
// ------------------------------------------------
char log_msg[100000000]; // for logging
int log_msg_pos = 0;

char log_lines[1000000][100]; // for log file viewer
int log_lines_int[1000000][3]; // for log file viewer

int log_timer; // to measure chase and lock time


int L_LOGGING_NETPLAY = 0;
int L_LOGGING_NETPLAY_JOIN = 0;
int L_LOGGING_NETPLAY_bandwidth = 0;
int L_LOGGING_NETPLAY_cdat = 0;
int L_LOGGING_NETPLAY_game_move = 0;
int L_LOGGING_NETPLAY_sdat = 0;
int L_LOGGING_NETPLAY_sdak = 0;
int L_LOGGING_NETPLAY_stdf = 0;
int L_LOGGING_NETPLAY_stdf_all_packets = 0;
int L_LOGGING_NETPLAY_stdf_when_to_apply = 0;
int L_LOGGING_NETPLAY_show_dif1 = 0;
int L_LOGGING_NETPLAY_show_dif2 = 0;

int auto_save_game_on_exit = 0;
int auto_save_game_on_level_done = 0;


// ------------------------------------------------
// ---------------- sound -------------------------
// ------------------------------------------------
ALLEGRO_VOICE *voice = NULL;
ALLEGRO_MIXER *mn_mixer = NULL;
ALLEGRO_MIXER *se_mixer = NULL;
ALLEGRO_MIXER *st_mixer = NULL;
ALLEGRO_SAMPLE *snd[20];
ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
ALLEGRO_AUDIO_STREAM *pm_theme_stream;
int fuse_loop_playing;
int sample_delay[8];
int se_scaler=5;
int st_scaler=5;
int lit_item;
int sound_on = 1;



// ------------------------------------------------
// ---------------- players -----------------------
// ------------------------------------------------
struct player players[NUM_PLAYERS];
struct player1 players1[NUM_PLAYERS];
int active_local_player = 0;


// ------------------------------------------------
// ---------------- lifts -----------------------
// ------------------------------------------------
struct lift lifts[NUM_LIFTS];
struct lift_step lift_steps[NUM_LIFTS][40];
int num_lifts;
char lift_step_type_name[10][10];


// ------------------------------------------------
// ---------------- level -------------------------
// ------------------------------------------------
int l[100][100];
int start_level;
int play_level;
int valid_level_loaded;
int level_num; // used by level editor and load file to keep track of last level loaded
int resume_allowed=0;
int number_of_starts;


// items
int item[500][16];      // item ints
al_fixed itemf[500][4]; // item fixeds
int item_num_of_type[20];
int item_first_num[20];
char item_name[20][40];
char *pmsg[500] = { NULL };


// enemies
int Ei[100][32];        // enemy ints
al_fixed Efi[100][16];  // enemy fixeds
int e_num_of_type[50];
int e_first_num[50];
char enemy_name[20][40];
int num_enemy;
int enemy_tile[20];

// PDE
int PDEi[100][32];
al_fixed PDEfx[100][16];
char PDEt[100][20][40];

// bullets
int pbullet[50][6];
int e_bullet_active[50];
int e_bullet_shape[50];
al_fixed e_bullet_fx[50];
al_fixed e_bullet_fy[50];
al_fixed e_bullet_fxinc[50];
al_fixed e_bullet_fyinc[50];
int pm_bullet_collision_box = 8;








// ------------------------------------------------
// ---------------- display -----------------------
// ------------------------------------------------


int desktop_width;
int desktop_height;


int disp_x_curr; // either wind in windowed mode or full fullscreen mode)
int disp_y_curr;
int disp_w_curr;
int disp_h_curr;

int disp_x_wind; // windowed
int disp_y_wind;
int disp_w_wind;
int disp_h_wind;

int disp_x_full; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
int disp_y_full;
int disp_w_full;
int disp_h_full;

int SCREEN_W;
int SCREEN_H;
int WX;
int WY;
int fullscreen = 1;

// used to only redraw a region of background to increase fps
int level_display_region_x;
int level_display_region_y;
int level_display_region_w;
int level_display_region_h;

int display_transform_double = 1;

int level_editor_running = 0;
int help_screens_running = 0;
int visual_level_select_running = 0;

int show_debug_overlay = 0;

int show_player_join_quit_timer = 0;
int show_player_join_quit_player = 0;
int show_player_join_quit_jq = 0;

float scale_factor = 1.0;
float scale_factor_current  = 1.0;
float scale_factor_inc = 0.03;
int show_scale_factor;



int show_splash_screen = 1;
int splash_screen_done = 0;

int mdw_an_seq = 0;   // mdw animation sequence number
float points[9][8];   // for mdw logo
int mdw_map_logo_x = 100;
int mdw_map_logo_y = 140;
int mdw_map_logo_th = 1;
float mdw_map_logo_scale = .3;

float mdw_splash_logo_x;
float mdw_splash_logo_y;
int mdw_splash_logo_th;
float mdw_splash_logo_scale;

float mdw_logo_scale_dec;
float mdw_logo_x_dec;
float mdw_logo_y_dec;



// position and size of map on menu screen
int menu_map_x;
int menu_map_y;
int menu_map_size;

int db;  // level editor zoom fullscreen map double
int txc; // center of right hand side panel in level editor

// game map
int game_map_on;
int map_x = BORDER_WIDTH;
int map_y = BORDER_WIDTH;
int map_size = 0;
int new_size = 0;


void final_wrapup(void)
{
   save_display_window_position();

   al_destroy_audio_stream(pm_theme_stream);
/*
   printf("al_uninstall_audio()\n");
   al_uninstall_audio();

   printf("al_destroy_font()\n");
   al_destroy_font(font);
   al_destroy_font(f1);
   al_destroy_font(f2);
   al_destroy_font(f3);

   printf("al_shutdown_ttf_addon()\n");
   al_shutdown_ttf_addon();

   printf("al_shutdown_font_addon()\n");
   al_shutdown_font_addon();

   printf("al_shutdown_image_addon()\n");
   al_shutdown_image_addon();

   printf("al_shutdown_native_dialog_addon()\n");
   al_shutdown_native_dialog_addon();

   printf("al_shutdown_primitives_addon()\n");
   al_shutdown_primitives_addon();

   printf("al_unregister_event_source(event_queue, al_get_keyboard_event_source())\n");
   al_unregister_event_source(event_queue, al_get_keyboard_event_source());

   printf("al_uninstall_keyboard()\n");
   al_uninstall_keyboard();

   printf("al_unregister_event_source(event_queue, al_get_mouse_event_source())\n");
   al_unregister_event_source(event_queue, al_get_mouse_event_source());

   printf("al_uninstall_mouse()\n");
   al_uninstall_mouse();

   printf("al_unregister_event_source(event_queue, al_get_joystick_event_source())\n");
   al_unregister_event_source(event_queue, al_get_joystick_event_source());

   printf("al_uninstall_joystick()\n");
   al_uninstall_joystick();

   printf("al_unregister_event_source(event_queue, al_get_timer_event_source(mnu_timer));\n");
   al_unregister_event_source(event_queue, al_get_timer_event_source(mnu_timer));

   printf("al_unregister_event_source(event_queue, al_get_display_event_source(display))\n");
   al_unregister_event_source(event_queue, al_get_display_event_source(display));

   printf("al_destroy_display()\n");
   al_destroy_display(display);

   printf("al_destroy_event_queue(event_queue)\n");
   al_destroy_event_queue(event_queue);

  // printf("\nBefore al_uninstall_system()\n");
   al_uninstall_system();
  // printf("\nAfter al_uninstall_system()\n");

  */
   al_uninstall_system();
}

void fast_exit(int why)
{
   // don't overwrite if not zero
   if (why != 0) players1[active_local_player].quit_reason = why;

   if (ima_client) log_ending_stats();
   if (ima_server) log_ending_stats_server();

   save_log_file();
   blind_save_game_moves(3);
   final_wrapup();
   exit(0);
}


void show_system_id()
{
   int j = al_get_system_id();
   if (j == ALLEGRO_SYSTEM_ID_UNKNOWN)     printf("System ID: Unknown system\n");
   if (j == ALLEGRO_SYSTEM_ID_XGLX)        printf("System ID: Xglx\n");
   if (j == ALLEGRO_SYSTEM_ID_WINDOWS)     printf("System ID: Windows\n");
   if (j == ALLEGRO_SYSTEM_ID_MACOSX)      printf("System ID: macOS\n");
   if (j == ALLEGRO_SYSTEM_ID_ANDROID)     printf("System ID: Android\n");
   if (j == ALLEGRO_SYSTEM_ID_IPHONE)      printf("System ID: iOS\n");
   if (j == ALLEGRO_SYSTEM_ID_GP2XWIZ)     printf("System ID: GP2XWIZ\n");
   if (j == ALLEGRO_SYSTEM_ID_RASPBERRYPI) printf("System ID: Raspberry Pi\n");
   if (j == ALLEGRO_SYSTEM_ID_SDL)         printf("System ID: SDL\n");
}



void set_and_get_versions(void)
{
   // set version name
   sprintf(pm_version_string, "7.10");

   sprintf(msg, "Purple Martians Version %s", pm_version_string);
   printf("\n%s\n", msg);

   // get allegro version
   uint32_t version = al_get_allegro_version();
   int major = version >> 24;
   int minor = (version >> 16) & 255;
   int revision = (version >> 8) & 255;
   int release = version & 255;

   sprintf(al_version_string, "Allegro Version: %d.%d.%d.%d", major, minor, revision, release);
   printf("%s\n", al_version_string);

   get_hostname();
}


void get_desktop_resolution()
{
   ALLEGRO_MONITOR_INFO aminfo;
   al_get_monitor_info(0 , &aminfo);
   desktop_width  = aminfo.x2 - aminfo.x1;
   desktop_height = aminfo.y2 - aminfo.y1;
   printf("Desktop Resolution: %dx%d\n", desktop_width, desktop_height);

   disp_x_full = 0; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
   disp_y_full = 0;
   disp_w_full = desktop_width;
   disp_h_full = desktop_height;
}


int initial_setup(void)
{

   //al_set_config_value(al_get_system_config(), "trace", "level", "debug");

   al_init();
   set_and_get_versions();
   get_config_values();
   show_system_id();
   get_desktop_resolution();



// --- event queue ----------------
   event_queue = al_create_event_queue();
   if(!event_queue)
   {
      m_err("Failed to create event queue.\n");
      return -1;
   }


// --- display --------------------
   if (!init_display()) return 0;
   al_register_event_source(event_queue, al_get_display_event_source(display));


// --- allegro add ons ------------
   if(!al_init_native_dialog_addon())
   {
      m_err("Failed to initialize native dialog addon.\n");
      return -1;
   }









   if(!al_init_primitives_addon())
   {
      m_err("Failed to initialize primitives addon.\n");
      return -1;
   }

   if(!al_init_font_addon())
   {
      m_err("Failed to initialize font addon.\n");
      return -1;
   }

   if(!al_init_ttf_addon())
   {
      m_err("Failed to initialize ttf addon.\n");
      return -1;
   }
   load_fonts();


// --- keyboard -------------------
   if (!al_install_keyboard())
   {
      m_err("Failed to install keyboard.\n");
      return -1;
   }
   //else printf("installed keyboard\n");
   al_register_event_source(event_queue, al_get_keyboard_event_source());


// --- mouse ----------------------
   if (!al_install_mouse())
   {
      m_err("Failed to install mouse.\n");
      return -1;
   }
   //else printf("installed mouse\n");
   al_register_event_source(event_queue, al_get_mouse_event_source());
   al_hide_mouse_cursor(display);


// --- joystick -------------------
   if (!al_install_joystick())
   {
      m_err("Failed to install joystick.\n");
      return -1;
   }
   //else printf("installed joystick\n");
   int nj = al_get_num_joysticks();
   //printf("found %d joystick(s)\n", nj);
   if (nj > 0)
   {
      joy0 = al_get_joystick(0);
      //printf("j0 - %s\n", al_get_joystick_name(joy0));

      if (nj > 1)
      {
         joy1 = al_get_joystick(1);
         //printf("j1 - %s\n", al_get_joystick_name(joy1));
      }
      al_register_event_source(event_queue, al_get_joystick_event_source());
   }


   if(!al_init_image_addon())
   {
      m_err("Failed to initialize image addon.\n");
      return -1;
   }
   load_tiles();
   al_set_display_icon(display, tile[401]);



// --- timers ---------------------
   // create timers
   fps_timer = al_create_timer(1/(float)frame_speed);
   sec_timer = al_create_timer(1);
   //mnu_timer = al_create_timer(.01);
   mnu_timer = al_create_timer(.008);

   // register timer event source
   al_register_event_source(event_queue, al_get_timer_event_source(mnu_timer));

   // start timers
   al_start_timer(fps_timer);
   al_start_timer(sec_timer);
   al_start_timer(mnu_timer);

   load_sound();

   seed_mdw();  // for mdw logo
   fill_mdw();

   // init all players
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
   players[0].active = 1;

   //zero_level_data();
   //reset_animation_sequence_frame_nums(0);
   //printf("end of initial setup\n");
   return 1;
}


void game_menu(void)
{
   int top_menu_sel = 3;
   if (!splash_screen_done)
   {
      splash_screen();
      splash_screen_done = 1;
   }
   //printf("game menu\n");
   load_level(start_level, 0);
   do
   {
      //printf("post load level\n");
      top_menu_sel = zmenu(7, top_menu_sel, 10);

      if ((top_menu_sel == 4) && (resume_allowed)) // resume game
      {
         game_loop(7); // resume
      }
      if (top_menu_sel == 3) // start new game
      {
         play_level = start_level;
         game_loop(1); // single player game
         if (resume_allowed) top_menu_sel = 4;
      }
      if (top_menu_sel == 2) // start level
      {
         int pl = visual_level_select();
         if (pl)
         {
            play_level = pl;
            set_start_level(pl);
            printf("lev selected level:%d\n", play_level);
            top_menu_sel = 3;
         }
         else
         {
            top_menu_sel = 15; // dummy mode to redraw
         }
      }

      if (top_menu_sel == 5) // host network game
      {
         play_level = start_level;
         game_loop(2); // server game
      }
      if (top_menu_sel == 6) // join network game
      {
         game_loop(3); // client game
      }

      if (top_menu_sel == 9) demo_mode(); // demo mode

      if (top_menu_sel == 8) // level editor
      {
         play_level = edit_menu(start_level);

         // restore menu items
         set_start_level(play_level);
         set_speed();

         game_loop(1); // single player game
      }

      if (top_menu_sel == 10) help(""); // help

      if (top_menu_sel == 15) // dummy mode to redraw
      {
         load_level(start_level, 0);
         top_menu_sel = 2;
      }


      if (top_menu_sel == 7) // options menu
      {
         int options_menu_sel = 2;
         do
         {
            options_menu_sel = zmenu(8, options_menu_sel, 30);
            if (options_menu_sel == 3)
            {
               int netgame_menu_sel = 2;
               do
               {
                  netgame_menu_sel = zmenu(4, netgame_menu_sel, 30);
                  if (netgame_menu_sel == 3) // edit server name
                  {
                     edit_server_name();
                     save_config();
                  }
                  if (netgame_menu_sel == 4) // toggle TCP/UDP
                  {
                     TCP = !TCP;
                     save_config();
                  }
                  if (netgame_menu_sel == 5) // toggle deathmatch bullets
                  {
                     deathmatch_pbullets = !deathmatch_pbullets;
                     save_config();
                  }
                  if (netgame_menu_sel == 6) // toggle suicide bullets
                  {
                     suicide_pbullets = !suicide_pbullets;
                     save_config();
                  }
                  if ( (netgame_menu_sel >= 100) && (netgame_menu_sel < 200) ) // right
                  {
                     netgame_menu_sel -= 100;
                     if (netgame_menu_sel == 7) // damage inc
                     {
                        if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) deathmatch_pbullets_damage +=10;
                        else deathmatch_pbullets_damage++;
                        if (deathmatch_pbullets_damage > 100) deathmatch_pbullets_damage = 100;
                        save_config();
                     }
                  }
                  if ( (netgame_menu_sel >= 200) && (netgame_menu_sel < 300) )  // left
                  {
                     netgame_menu_sel -= 200;
                     if (netgame_menu_sel == 7) // damage dec
                     {
                        if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) deathmatch_pbullets_damage -=10;
                        else deathmatch_pbullets_damage--;
                        if (deathmatch_pbullets_damage < -10) deathmatch_pbullets_damage = -10;
                        save_config();
                     }
                  }

               }  while (netgame_menu_sel != 2); // end of netgame options menu
            }

            if (options_menu_sel == 10) // logging options menu
            {
               int logging_menu_sel = 2;
               do
               {
                  logging_menu_sel = zmenu(3, logging_menu_sel, 30);
                  if (logging_menu_sel == 3) // all on
                  {
                     L_LOGGING_NETPLAY=1;
                     L_LOGGING_NETPLAY_JOIN=1;
                     L_LOGGING_NETPLAY_bandwidth=1;
                     L_LOGGING_NETPLAY_cdat=1;
                     L_LOGGING_NETPLAY_game_move=1;
                     L_LOGGING_NETPLAY_sdat=1;
                     L_LOGGING_NETPLAY_sdak=1;
                     L_LOGGING_NETPLAY_stdf=1;
                     L_LOGGING_NETPLAY_stdf_all_packets=1;
                     L_LOGGING_NETPLAY_stdf_when_to_apply=1;
                     L_LOGGING_NETPLAY_show_dif1=1;
                     L_LOGGING_NETPLAY_show_dif2=1;
                     auto_save_game_on_level_done=1;
                     auto_save_game_on_exit=1;
                     save_config();
                  }

                  if (logging_menu_sel == 4 ) // all off
                  {
                     L_LOGGING_NETPLAY=0;
                     L_LOGGING_NETPLAY_JOIN=0;
                     L_LOGGING_NETPLAY_bandwidth=0;
                     L_LOGGING_NETPLAY_cdat=0;
                     L_LOGGING_NETPLAY_game_move=0;
                     L_LOGGING_NETPLAY_sdat=0;
                     L_LOGGING_NETPLAY_sdak=0;
                     L_LOGGING_NETPLAY_stdf=0;
                     L_LOGGING_NETPLAY_stdf_all_packets=0;
                     L_LOGGING_NETPLAY_stdf_when_to_apply=0;
                     L_LOGGING_NETPLAY_show_dif1=0;
                     L_LOGGING_NETPLAY_show_dif2=0;
                     auto_save_game_on_level_done=0;
                     auto_save_game_on_exit=0;
                     save_config();
                  }

                  if (logging_menu_sel == 5)
                  {
                     L_LOGGING_NETPLAY= !L_LOGGING_NETPLAY;
                     save_config();
                  }
                  if (logging_menu_sel == 6)
                  {
                     L_LOGGING_NETPLAY_JOIN= !L_LOGGING_NETPLAY_JOIN;
                     save_config();
                  }
                  if (logging_menu_sel == 7)
                  {
                     L_LOGGING_NETPLAY_bandwidth= !L_LOGGING_NETPLAY_bandwidth;
                     save_config();
                  }
                  if (logging_menu_sel == 8)
                  {
                     L_LOGGING_NETPLAY_cdat= !L_LOGGING_NETPLAY_cdat;
                     save_config();
                  }
                  if (logging_menu_sel == 9)
                  {
                     L_LOGGING_NETPLAY_game_move= !L_LOGGING_NETPLAY_game_move;
                     save_config();
                  }

                  if (logging_menu_sel == 10)
                  {
                    L_LOGGING_NETPLAY_sdat = !L_LOGGING_NETPLAY_sdat;
                     save_config();
                  }
                  if (logging_menu_sel == 11)
                  {
                     L_LOGGING_NETPLAY_sdak= !L_LOGGING_NETPLAY_sdak;
                     save_config();
                  }
                  if (logging_menu_sel == 12)
                  {
                     L_LOGGING_NETPLAY_stdf= !L_LOGGING_NETPLAY_stdf;
                     save_config();
                  }
                  if (logging_menu_sel == 13)
                  {
                     L_LOGGING_NETPLAY_stdf_all_packets= !L_LOGGING_NETPLAY_stdf_all_packets;
                     save_config();
                  }
                  if (logging_menu_sel == 14)
                  {
                     L_LOGGING_NETPLAY_stdf_when_to_apply= !L_LOGGING_NETPLAY_stdf_when_to_apply;
                     save_config();
                  }
                  if (logging_menu_sel == 15)
                  {
                     L_LOGGING_NETPLAY_show_dif1= !L_LOGGING_NETPLAY_show_dif1;
                     save_config();
                  }
                  if (logging_menu_sel == 16)
                  {
                     L_LOGGING_NETPLAY_show_dif2 = !L_LOGGING_NETPLAY_show_dif2;
                     save_config();
                  }
                  if (logging_menu_sel == 17)
                  {
                     auto_save_game_on_level_done= !auto_save_game_on_level_done;
                     save_config();
                  }
                  if (logging_menu_sel == 18)
                  {
                     auto_save_game_on_exit= !auto_save_game_on_exit;
                     save_config();
                  }
                  if (logging_menu_sel == 19)
                  {
                     log_file_viewer(1);
                  }
               }  while (logging_menu_sel != 2); // end of netgame options menu
            }

            if (options_menu_sel == 7) sound_toggle();
            if (options_menu_sel == 11) splash_toggle();
            if (options_menu_sel == 12) save_gm();
            if (options_menu_sel == 13) // run game
            {
               if (load_gm("-"))
               {
                  game_loop(9); // demo game

                  // reset player data
                  for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
                  players[0].active = 1;
                  active_local_player = 0;
                  get_config_values(); // restore player color from config file
                  erase_log();
               }
            }

            if (options_menu_sel == 5) // controller setup menu
            {
               set_key_menu(9, 0, 10); //set controller menu keys
               int p1_menu_sel = 2;
               do
               {
                  p1_menu_sel = zmenu(9, p1_menu_sel, 30);
                  if (p1_menu_sel == 3) test_keys();

                  if (p1_menu_sel == 4) get_all_keys(0);

                  if (p1_menu_sel == 5) // set all to joy1
                  {
                     players1[0].up_key = 128;
                     players1[0].down_key = 129;
                     players1[0].right_key = 131;
                     players1[0].left_key = 130;
                     players1[0].jump_key = 133;
                     players1[0].fire_key = 132;
                     players1[0].menu_key = 135;
                  }

                  if (p1_menu_sel == 6) // set all to joy2
                  {
                     players1[0].up_key = 148;
                     players1[0].down_key = 149;
                     players1[0].right_key = 151;
                     players1[0].left_key = 150;
                     players1[0].jump_key = 153;
                     players1[0].fire_key = 152;
                     players1[0].menu_key = 155;
                  }

                  if (p1_menu_sel == 7) // set all to arrows
                  {
                     players1[0].up_key = 84;
                     players1[0].down_key = 85;
                     players1[0].right_key = 83;
                     players1[0].left_key = 82;
                     players1[0].jump_key = 75;
                     players1[0].fire_key = 3;
                     players1[0].menu_key = 59;
                  }

                  if (p1_menu_sel == 8) // set all to IJKL
                  {
                     players1[0].up_key = 9;
                     players1[0].down_key = 11;
                     players1[0].right_key = 12;
                     players1[0].left_key = 10;
                     players1[0].jump_key = 75;
                     players1[0].fire_key = 3;
                     players1[0].menu_key = 59;
                  }

                  if (p1_menu_sel == 10) players1[0].up_key =    my_readkey();
                  if (p1_menu_sel == 11) players1[0].down_key =  my_readkey();
                  if (p1_menu_sel == 12) players1[0].right_key = my_readkey();
                  if (p1_menu_sel == 13) players1[0].left_key =  my_readkey();
                  if (p1_menu_sel == 14) players1[0].jump_key =  my_readkey();
                  if (p1_menu_sel == 15) players1[0].fire_key =  my_readkey();
                  if (p1_menu_sel == 16) players1[0].menu_key =  my_readkey();

                  set_key_menu(9, 0, 10); //set controller menu keys
                  save_config();

                  if ( (p1_menu_sel >= 100) && (p1_menu_sel < 200) ) p1_menu_sel -= 100; // right
                  if ( (p1_menu_sel >= 200) && (p1_menu_sel < 300) ) p1_menu_sel -= 200; // left

               }  while (p1_menu_sel != 2);
            }


            if ( (options_menu_sel >= 100) && (options_menu_sel < 200) )  // right
            {
               options_menu_sel -= 100;

               if (options_menu_sel == 4) // color inc
               {
                  int p = 0;
                  if (++players[p].color > 15) players[p].color = 1;
                  save_config();
               }

               if (options_menu_sel == 6) // frame_speed ++
               {
                  if (++frame_speed > 500) frame_speed = 500;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol ++
               {
                  if (++se_scaler > 9) se_scaler = 9;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol ++
               {
                  if (++st_scaler > 9) st_scaler = 9;
                  set_st_scaler();
               }
            }


            if ( (options_menu_sel >= 200) && (options_menu_sel < 300) ) //left
            {
               options_menu_sel -= 200;

               if (options_menu_sel == 4) // color dec
               {
                  int p = 0;
                  if (--players[p].color < 1) players[p].color = 15;
                  save_config();
               }
               if (options_menu_sel == 6) // frame_speed --
               {
                  if (--frame_speed < 1) frame_speed = 1;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol --
               {
                  if (--se_scaler < 0) se_scaler = 0;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol --
               {
                  if (--st_scaler < 0) st_scaler = 0;
                  set_st_scaler();
               }
            }
         }  while (options_menu_sel != 2); // end of options menu
         al_rest(0.1);
      }
      if ( (top_menu_sel >= 100) && (top_menu_sel < 200) )
      {
         // right
         top_menu_sel -= 100;

         if (top_menu_sel == 2) // start level inc
         {
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) start_level +=100;
            else if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) start_level +=10;
            else start_level++;
            if (start_level > 399) start_level = 399;
            set_start_level(start_level);
            load_level(start_level, 0);
         }
      }
      if ( (top_menu_sel >= 200) && (top_menu_sel < 300) )
      {
         // left
         top_menu_sel -= 200;

         if (top_menu_sel == 2) // start level dec
         {
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) start_level -=100;
            else if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) start_level -=10;
            else start_level--;
            if (start_level < 1) start_level = 1;
            set_start_level(start_level);
            load_level(start_level, 0);
         }
      }
   } while (top_menu_sel != 1); // end of game menu
}


int main(int argument_count, char **argument_array)
{
// --------------------------------------------------------------------------------------------
// these flags get processed before initial setup is called
// --------------------------------------------------------------------------------------------

#ifndef RELEASE
   if (argument_count == 2) // example 'pmwin x'
   {
      int ret = 0;
      if (strcmp(argument_array[1],"-x") == 0 )
      {
          ret = copy_files_to_clients(2); // src only
          exit(0);
      }
      if (strcmp(argument_array[1],"-t") == 0 )
      {
          ret = copy_files_to_clients(1); // pm.exe and levels only
          exit(0);
      }
      if (strcmp(argument_array[1],"-u") == 0 )
      {
         ret = copy_files_to_clients(0); // all files
         exit(0);
      }
      sprintf(msg, "ret:%d\n",ret); // to suppress warning only!
   }
#endif

   if (initial_setup())
   {


// --------------------------------------------------------------------------------------------
// these flags get processed after allegro is initialized
// --------------------------------------------------------------------------------------------


   //spline_test();
   //spline_adjust();

   //scaled_tile_test();

      if (argument_count == 2) // example 'pmwin arg1'
      {

         // run level editor -- eg: 'pm.exe -e'
         if (strcmp(argument_array[1],"-e") == 0 )
         {
            play_level = start_level;
            set_start_level(play_level);
            printf("running level editor for level:%d\n", play_level);
            play_level = edit_menu(play_level);
            set_start_level(play_level);
            fast_exit(0);
         }


         if (strcmp(argument_array[1],"-h") == 0 )  // help
         {
            help("Command Line");
            fast_exit(0);
         }


         #ifndef RELEASE
//         // run packet test server
//         void nc_server(void);
//         if (strcmp(argument_array[1],"-x") == 0 )
//         {
//            nc_server();
//            exit(0);
//         }
//         // run packet test client
//         if (strcmp(argument_array[1],"-z") == 0 )
//         {
//            nc_client();
//            exit(0);
//         }


         if (strcmp(argument_array[1],"-l") == 0 )  // log file viewer
         {
            log_file_viewer(1);
            fast_exit(0);
         }

         if (strcmp(argument_array[1],"-lr") == 0 )  // log file viewer most recent
         {
            log_file_viewer(2);
            fast_exit(0);
         }

         if (strcmp(argument_array[1],"-g") == 0 )  // temp testing
         {
             temp_test();
             fast_exit(0);
         }
         #endif

         // no server specified; use the one from the config file
         if (strcmp(argument_array[1],"-c") == 0 )
         {
            show_splash_screen = 0;
            game_loop(3); // client game
            fast_exit(0);
         }
         // no start level specified; use play level from config file
         if (strcmp(argument_array[1],"-s") == 0 )
         {
            show_splash_screen = 0;
            play_level = start_level;
            game_loop(2); // server game
            fast_exit(0);
         }

         // keep this last so if no other single flag matches try to run like it an int level...
         // start game on specified level -- eg: 'pm.exe 211'
         int pl = atoi(argument_array[1]);
         if ((pl > 0) && (pl < 400))
         {
            show_splash_screen = 0;
            play_level = pl;
            set_start_level(pl);
            printf("started game on level:%d\n", play_level);
            game_loop(1); // single player game
         }
         else printf("%s could not be parsed to an integer level number\n", argument_array[1]);

      } // end of argument_count == 2


      if (argument_count == 3) // example 'pmwin arg1 arg2'
      {
         // start game on specified level -- eg: 'pm.exe -p 211'
         if (strcmp(argument_array[1],"-p") == 0 )
         {
            show_splash_screen = 0;
            int pl = atoi(argument_array[2]);
            if ((pl > 0) && (pl < 400))
            {
               play_level = pl;
               set_start_level(pl);
               printf("started game on level:%d\n", play_level);
               game_loop(1); // single player game
            }
            else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
         }

         // run level editor for specified level -- eg: 'pm.exe -e 211'
         if (strcmp(argument_array[1],"-e") == 0 )
         {
            int pl = atoi(argument_array[2]);
            if ((pl > 0) && (pl < 400))
            {
               play_level = pl;
               set_start_level(pl);
               printf("running level editor for level:%d\n", pl);
               pl = edit_menu(pl);
               set_start_level(pl);
               fast_exit(0);
            }
            else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
         }

         // run saved game from file -- eg: 'pm.exe -f mz23.gm'
         if (strcmp(argument_array[1],"-f") == 0 )
         {
            show_splash_screen = 0;
            sprintf(msg, "savegame/%s", argument_array[2] );
            if (load_gm(msg))
            {
               printf("running game file:%s\n", argument_array[2]);
               game_loop(9); // demo game
               fast_exit(0);
            }
         }
         if (strcmp(argument_array[1],"-c") == 0 )
         {
            show_splash_screen = 0;
            sprintf(m_serveraddress, "%s", argument_array[2]);
            save_config();
            game_loop(3); // client game
            fast_exit(0);
         }
         if (strcmp(argument_array[1],"-s") == 0 )
         {
            show_splash_screen = 0;
            play_level = atoi(argument_array[2]);
            game_loop(2); // server game
            fast_exit(0);
         }
      } // end of argument_count == 3
      game_menu(); // this is where it all happens
   } // end of if initial setup

   blind_save_game_moves(2);
   save_log_file();
   final_wrapup();
   exit(0);
}


int copy_files_to_clients(int exe_only)
{
   int ret;
   char sys_cmd[500];
   char client[20][255];
   int num_clients = 0;

//   sprintf(client[num_clients++], "\\\\E6400\\pm_client1");  //  E6400
//   sprintf(client[num_clients++], "\\\\E6410\\pm_client2");  // win7 portable dev system
//   sprintf(client[num_clients++], "\\\\4230j\\pm_client3");  // win7 acer laptop
//   sprintf(client[num_clients++], "\\\\E6430\\pm_client4");  // win7 studio pc
//   sprintf(client[num_clients++], "\\\\E6420\\pm_client5");  // win7 2560x1600 (my room)
//   sprintf(client[num_clients++], "\\\\pfv\\pm_client6");    // XP 1600x1200
//   sprintf(client[num_clients++], "\\\\m-4230-3\\pm_client7"); // ubuntu acer laptop
//   sprintf(client[num_clients++], "\\\\DESKTOP-DBNSJH8\\pm_client8"); // win 10 EID work laptop
//   sprintf(client[num_clients++], "\\\\e4230f\\pm_client9"); // acer laptop
//   sprintf(client[num_clients++], "\\\\4230a\\pm_client10"); // acer laptop
//   sprintf(client[num_clients++], "\\\\insp9400\\pm_client11"); // dell insp 9400 (backup machine)
//   sprintf(client[num_clients++], "\\\\m7667\\pm_client12"); // core2 2G 32bit GeForce 7500 LE
//   sprintf(client[num_clients++], "\\\\nv59\\pm_client13");  // gateway nv59 i3 4G 1600x1200
//   sprintf(client[num_clients++], "\\\\y510\\pm_client14");  // y510 XP SP3


   sprintf(client[num_clients++], "\\\\4230j\\pm_client3");  // 4230j win 7


//   sprintf(client[num_clients++], "\\\\zi3\\pm_client99");  // zaiden


   if (exe_only == 1)
   {

      printf("copying exe to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "copy pm.exe %s\\pm.exe /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         sprintf(sys_cmd, "copy levels\\*.pml %s\\levels ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);


/*
         sprintf(sys_cmd, "copy pm.cfg %s\\pm.cfg /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         */

      }
   }
   else if (exe_only == 2)
   {
      printf("copying src to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "copy src\\*.cpp %s\\src ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         sprintf(sys_cmd, "copy src\\*.h %s\\src ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         sprintf(sys_cmd, "copy levels\\*.pml %s\\levels ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
      }
   }
   else
   {
      printf("copying all files to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "xcopy *.* %s /E /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
      }
   }
   return ret;
}
