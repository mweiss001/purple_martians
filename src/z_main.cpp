// z_main.cpp

#include "pm.h"
#include "z_sound.h"
#include "z_log.h"
#include "z_player.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwBitmap.h"
#include "mwFont.h"

// --------------- Global Variables ---------------
// all global variables should be declared here and externed in pm.h

float tmaj_i = 0;

int program_state = 0;

int new_program_state = 0;
int old_program_state = 0;
int older_program_state = 0;
int program_update = 0;
int program_update_1s = 0;
int menu_update = 0;;
int top_menu_sel = 3;
int main_loop_exit = 0;
int pm_event[1000];

// temp testing variables
int pct_x = 0;
int pct_y = 0;

double t0=0;

int level_header[20] = {0};

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
int frame_speed = 40;
int frame_num;
int speed_control_lock = 1;
int eco_draw = 0;


// some global strings
char level_filename[80];
char local_hostname[80];

char pm_version_string[80];
char al_version_string[80];

char global_string[20][25][80];
char msg[1024];
char color_name[16][20];

// animation sequence array
int zz[20][NUM_ANS];

// ------------------------------------------------
// ------------ game moves array ------------------
// ------------------------------------------------
int game_moves[GAME_MOVES_SIZE][4];
int game_move_entry_pos = 0;
int game_move_current_pos = 0; // for savegame running only

int   game_event_retrigger_holdoff[10];
float game_event_retrigger_holdoff_tally[10];

// ------------------------------------------------
// ----------------- mouse and keys ---------------
// ------------------------------------------------

// serial key check
char skc[64];
int skc_index = 0;

bool key[ALLEGRO_KEY_MAX][4];
int key_pressed_ASCII;

int mouse_x = 0;
int mouse_y = 0;
int mouse_z = 0;
int mouse_dx = 0;
int mouse_dy = 0;
int mouse_dz = 0;

bool mouse_b[5][4] = { 0 };


// ------------------------------------------------
// ----------------- setup ------------------------
// ------------------------------------------------

ALLEGRO_TIMER * fps_timer;  // used to control the speed of the game
ALLEGRO_TIMER * mnu_timer;  // used to control the speed of the menu
ALLEGRO_TIMER * sec_timer;  // used to count the actual frames per second
ALLEGRO_TIMER * png_timer;  // used to control the speed client pings
ALLEGRO_TIMER * mou_timer;  // used to control hiding the mouse cursor


ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_COLOR palette_color[256];
int flash_color = 0;
int flash_color2 = 0;
int flash_counter = 0;

ALLEGRO_JOYSTICK *joy0 = NULL;
ALLEGRO_JOYSTICK *joy1 = NULL;

// ------------------------------------------------
// ----------------- bitmaps ----------------------
// ------------------------------------------------
int sa[NUM_SPRITES][2]; // shape attributes

ALLEGRO_BITMAP *ft_bmp = NULL;  //  file temp paste bmp

ALLEGRO_BITMAP *logo_ichael = NULL;
ALLEGRO_BITMAP *logo_avid = NULL;
ALLEGRO_BITMAP *logo_eiss = NULL;
int logo_text_bitmaps_create = 1;


ALLEGRO_BITMAP *text_title = NULL;
int text_title_bitmaps_create = 1;
int text_title_draw_color = -1;

ALLEGRO_BITMAP *large_text_overlay_bitmap;
int large_text_overlay_state = 0;

// ------------------------------------------------
// ---------------- lifts -----------------------
// ------------------------------------------------

struct lift lifts[NUM_LIFTS];
struct lift_step lift_steps[NUM_LIFTS][40];

int num_lifts;
char lift_step_type_name[10][10];

// bullets
int pbullet[50][6];
int pm_bullet_collision_box = 8;
struct ebullet ebullets[50];

// ------------------------------------------------
// ---------------- level -------------------------
// ------------------------------------------------
int l[100][100];
int thl[100][100] = {0}; // tile helper

int warp_level_location = 0;

int start_level;
int play_level;
int valid_level_loaded;
int last_level_loaded; // used by level editor and load file to keep track of last level loaded
int resume_allowed=0;
int number_of_starts;
int number_of_purple_coins;


// items
int item[500][16];      // item ints
al_fixed itemf[500][4]; // item fixeds
int item_num_of_type[30];
int item_first_num[30];
char item_name[30][40];
char pmsgtext[500][500] = {0};

// enemies
int Ei[100][32];        // enemy ints
al_fixed Efi[100][16];  // enemy fixeds
int e_num_of_type[50];
int e_first_num[50];
char enemy_name[100][2][40] = {0};
int num_enemy;
int enemy_tile[20];
int item_tile[20];

// PDE
int PDEi[100][32];
al_fixed PDEfx[100][16];
char PDEt[100][20][40];

int autosave_level_editor_state = 0;
int level_editor_running = 0;
int help_screens_running = 0;
int visual_level_select_running = 0;

int show_debug_overlay = 0;

int show_player_join_quit_timer = 0;
int show_player_join_quit_player = 0;
int show_player_join_quit_jq = 0;

// position and size of map on menu screen
int menu_map_x;
int menu_map_y;
int menu_map_size;


void final_wrapup(void)
{
   mwD.save_display_window_position();

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
   if (why != 0) players1[active_local_player].quit_reason = why; // don't overwrite if not zero
   if (autosave_log_on_program_exit) save_log_file();
   if (autosave_game_on_game_exit) blind_save_game_moves(3);
   final_wrapup();
   exit(0);
}

void show_system_id(void)
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

void set_exe_path(void)
{
   ALLEGRO_PATH *ep = al_get_standard_path(ALLEGRO_EXENAME_PATH);
   al_set_path_filename(ep, NULL);
   if (!al_change_directory(al_path_cstr(ep, ALLEGRO_NATIVE_PATH_SEP))) printf("Error Changing Current Directory!\n");
   //printf("Current Directory: %s\n", al_get_current_directory());
   al_destroy_path(ep);
}

void set_and_get_versions(void)
{
   // set version name
   sprintf(pm_version_string, PM_VERSION);

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

   get_hostname(0);
}

void get_desktop_resolution()
{
   ALLEGRO_MONITOR_INFO aminfo;
   al_get_monitor_info(0, &aminfo);
   mwD.desktop_width  = aminfo.x2 - aminfo.x1;
   mwD.desktop_height = aminfo.y2 - aminfo.y1;
   printf("Desktop Resolution: %dx%d\n", mwD.desktop_width, mwD.desktop_height);

   mwD.disp_x_full = 0; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
   mwD.disp_y_full = 0;
   mwD.disp_w_full = mwD.desktop_width;
   mwD.disp_h_full = mwD.desktop_height;
}

int initial_setup(void)
{
   //al_set_config_value(al_get_system_config(), "trace", "level", "debug");

   al_init();
   set_exe_path();
   set_and_get_versions();
   load_config();
   show_system_id();
   get_desktop_resolution();

   srand(time(NULL));


   // --- event queue ----------------
   event_queue = al_create_event_queue();
   if(!event_queue)
   {
      m_err("Failed to create event queue.\n");
      return 0;
   }

   // --- display --------------------
   if (!mwD.init_display())
   {
      m_err("Failed to initialize display.\n");
      return 0;
   }

   al_register_event_source(event_queue, al_get_display_event_source(display));

   // --- allegro add ons ------------
   if(!al_init_native_dialog_addon())
   {
      m_err("Failed to initialize native dialog addon.\n");
      return 0;
   }

   if(!al_init_primitives_addon())
   {
      m_err("Failed to initialize primitives addon.\n");
      return 0;
   }

   if(!al_init_image_addon())
   {
      m_err("Failed to initialize image addon.\n");
      return 0;
   }

   if(!al_init_font_addon())
   {
      m_err("Failed to initialize font addon.\n");
      return 0;
   }

   if(!al_init_ttf_addon())
   {
      m_err("Failed to initialize ttf addon.\n");
      return 0;
   }
   mF.initialize();

   // --- keyboard -------------------
   if (!al_install_keyboard())
   {
      m_err("Failed to install keyboard.\n");
      return 0;
   }
   //else printf("installed keyboard\n");
   al_register_event_source(event_queue, al_get_keyboard_event_source());


   // --- mouse ----------------------
   if (!al_install_mouse())
   {
      m_err("Failed to install mouse.\n");
      return 0;
   }
   //else printf("installed mouse\n");
   al_register_event_source(event_queue, al_get_mouse_event_source());
   al_hide_mouse_cursor(display);


   // --- joystick -------------------
   if (!al_install_joystick())
   {
      m_err("Failed to install joystick.\n");
      return 0;
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

   load_tiles();
   al_set_display_icon(display, mwB.tile[401]);

   // --- timers ---------------------
   // create timers
   fps_timer = al_create_timer(1/(float)frame_speed);
   sec_timer = al_create_timer(1);    // 1s
   mnu_timer = al_create_timer(.008); // 125 fps
   png_timer = al_create_timer(.5);   // 2 fps
   mou_timer = al_create_timer(5);    // 5s

   // register timer event source
   al_register_event_source(event_queue, al_get_timer_event_source(mnu_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(sec_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(png_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(mou_timer));

   // start timers
   al_start_timer(fps_timer);
   al_start_timer(sec_timer);
   al_start_timer(mnu_timer);
   al_start_timer(png_timer);
   al_start_timer(mou_timer);

   load_sound();

   // init players
   for (int p=1; p<NUM_PLAYERS; p++) players[p].color = 0; // all but player[0] which we got from config file
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
   players[0].active = 1;

   mwDM.demo_mode_enabled = mwDM.demo_mode_config_enable; // set only at startup from cofing file

   return 1;
}

int main(int argument_count, char **argument_array)
{
   proc_command_line_args1(argument_count, argument_array); // these args get processed before initial setup is called
   if (initial_setup())
   {

      proc_command_line_args2(argument_count, argument_array); // these args get processed after initial setup is called

      program_state = 1;

      main_loop();

   }
   if (autosave_log_on_program_exit) save_log_file();
   final_wrapup();
   exit(0);
}
