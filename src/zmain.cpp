#include "pm.h"


// --------------- Global Variables ---------------
// all global variables should be declared here and externed in pm.h

ALLEGRO_TIMER * fps_timer;
ALLEGRO_TIMER * sec_timer;
ALLEGRO_TIMER * mnu_timer;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_COLOR palette_color[256];
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *f1 = NULL;
ALLEGRO_FONT *f2 = NULL;
ALLEGRO_FONT *f3 = NULL;

ALLEGRO_JOYSTICK *joy0 = NULL;
ALLEGRO_JOYSTICK *joy1 = NULL;

ALLEGRO_VOICE *voice = NULL;
ALLEGRO_MIXER *mn_mixer = NULL;
ALLEGRO_MIXER *se_mixer = NULL;
ALLEGRO_MIXER *st_mixer = NULL;

ALLEGRO_SAMPLE *snd[20];
int num_sounds = 9;
ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
ALLEGRO_AUDIO_STREAM *pm_theme_stream;

int lit_item;
int fuse_loop_playing;
int sample_delay[8];
int se_scaler=5;
int st_scaler=5;

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

int level_display_region_x;
int level_display_region_y;
int level_display_region_w;
int level_display_region_h;

char skc[64];
int skc_index = 0;

int les = 3; // level editor scale
int level_editor_running = 0;
int help_screens_running = 0;

bool key[ALLEGRO_KEY_MAX];

int Key_pressed_ASCII;

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



int visual_level_select_running = 0;



ALLEGRO_FS_ENTRY *filenames[1000];
int num_filenames;



ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
int demo_played[100];
int num_demo_filenames = 0;
int demo_mode_on = 0;
int demo_mode_countdown;


int disp_x_curr; // either wind in windowed mode or full fullscreen mode)
int disp_y_curr;
int disp_w_curr;
int disp_h_curr;

int disp_x_wind; // use when restoring from fullscreen
int disp_y_wind;
int disp_w_wind;
int disp_h_wind;

int disp_x_full = 0;
int disp_y_full = 0;
int disp_w_full;
int disp_h_full;

int SCREEN_W, SCREEN_H;

int WX, WY;

int fullscreen = 1;

// last screen pos, to tell if it changed
int l_spx;
int l_spy;

char local_hostname[80];
char version_string[80];

int TCP = 0;

int L_LOGGING = 0;
int L_LOGGING_NETPLAY = 0;
int L_LOGGING_NETPLAY_JOIN = 0;
int L_LOGGING_NETPLAY_bandwidth = 0;
int L_LOGGING_NETPLAY_client_timer_adjust = 0;
int L_LOGGING_NETPLAY_cdat = 0;
int L_LOGGING_NETPLAY_game_move = 0;
int L_LOGGING_NETPLAY_sdat = 0;
int L_LOGGING_NETPLAY_sdak = 0;
int L_LOGGING_NETPLAY_chdf = 0;
int L_LOGGING_NETPLAY_chdf_all_packets = 0;
int L_LOGGING_NETPLAY_chdf_when_to_apply = 0;
int L_LOGGING_NETPLAY_show_dif1 = 0;
int L_LOGGING_NETPLAY_show_dif2 = 0;


// server chdf
char client_chdf[8][2][CHUNK_SIZE];
int client_chdf_id[8][2]; // passcount id

// client chdf
char chdf[CHUNK_SIZE];         // for client chdf building
int chdf_pieces[16];

char clientl_chdf[CHUNK_SIZE]; // last ack state for diffing
int clientl_chdf_id;           // passcount id of last state

char dif[CHUNK_SIZE];
int dif_id[2]; //   (0 = src, 1 = dst)

int zlib_cmp = 7;
int chdf_freq = 40;
int control_lead_frames = 3;
int server_lead_frames = 1;

int deathmatch_pbullets = 0;
int deathmatch_pbullets_damage = 5;
int suicide_pbullets = 0;


float mouse_loop_pause = 0;

char msg[256];


struct player players[NUM_PLAYERS];
struct player1 players1[NUM_PLAYERS];

int l[100][100];    // level
int Ei[100][32];    // enemy ints
al_fixed Efi[100][16]; // enemy fixeds
int item[500][16];  // item ints
al_fixed itemf[500][4]; // item al_fixed_points

int num_lifts;
struct lift lifts[NUM_LIFTS];
struct lift_step lift_steps[NUM_LIFTS][40];


int db;  // level editor zoom fullscreen double
int md;         // menu map double
int map_double; // level editor map double
int map_x = BORDER_WIDTH;
int map_y = BORDER_WIDTH;
int map_size = 0;
int new_size = 0;
int txc;


int Redraw = 1;
int Num_legend_lines = 2;
int Viewer_lock = 0;

int menu_map_x;
int menu_map_y;
int menu_map_size;

int item_num_of_type[20];

int ssfnsn = 0; // screen shot file name sequence number
int making_video = 0;
int speed_testing = 0;


struct screen_msg screen_msgs[100];

char log_msg[100000000]; // for logging
int log_msg_pos = 0;
char log_lines[1000000][100]; // for log file viewer
int log_lines_int[1000000][3]; // for log file viewer


int log_timer;

int KEY_1_b = 1;
int KEY_2_b = 1;
int KEY_3_b = 0;
int KEY_4_b = 0;
int KEY_5_b = 0;
int KEY_6_b = 1;
int KEY_7_b = 1;
int KEY_8_b = 1;
int KEY_9_b = 1;

int KEY_1_held = 0;
int KEY_2_held = 0;
int KEY_3_held = 0;
int KEY_4_held = 0;
int KEY_5_held = 0;
int KEY_6_held = 0;
int KEY_7_held = 0;
int KEY_8_held = 0;
int KEY_9_held = 0;


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


int test_int = 3;

int actual_fps;
int last_frames_skipped = 0;
int frames_skipped_last_second;
int last_fps_passcount = 0;

int ima_server = 0;
int ima_client = 0;
int active_local_player = 0;


char m_serveraddress[256] = "192.168.1.2";

int game_moves[1000000][4];
int game_move_entry_pos = 0;
int game_move_current_pos = 0; // for savegame running only


char enemy_name[50][32];
char lift_step_type_name[10][10];

char color_name[16][20];

int pm_bullet_collision_box = 8;
int map_on = 0;
int show_debug_overlay = 0;

int show_player_join_quit_timer = 0;
int show_player_join_quit_player = 0;
int show_player_join_quit_jq = 0;


float scale_factor = 1.0;
float scale_factor_current  = 1.0;
float scale_factor_inc = 0.03;
int show_scale_factor;

ALLEGRO_BITMAP *tilemap = NULL;
ALLEGRO_BITMAP *ptilemap = NULL;
ALLEGRO_BITMAP *dtilemap = NULL;
ALLEGRO_BITMAP *M_tilemap = NULL;
ALLEGRO_BITMAP *M_ptilemap = NULL;
ALLEGRO_BITMAP *M_dtilemap = NULL;

ALLEGRO_BITMAP *memory_bitmap[NUM_SPRITES];
ALLEGRO_BITMAP *player_bitmap[16][32];
ALLEGRO_BITMAP *door_bitmap[2][16][8];
ALLEGRO_BITMAP *l2000 = NULL;

ALLEGRO_BITMAP *level_buffer = NULL;
ALLEGRO_BITMAP *dtemp = NULL; // temp draw
ALLEGRO_BITMAP *lefsm = NULL; // level editor fullscreen map

ALLEGRO_BITMAP *mp = NULL;     //  mouse_pointer
ALLEGRO_BITMAP *ft_bmp = NULL;  //  file temp paste bmp

int show_splash_screen = 1;
int splash_screen_done = 0;

int auto_save_game_on_exit = 0;
int auto_save_game_on_level_done = 0;

int mdw_an_seq = 0; // mdw animation sequence number
float points[10][8]; // for mdw logo
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

char global_string[20][25][80];

int tmy;   // menu pos
int tmtx, tmty; // text position
int mx, my;
float steps;

int sa[NUM_SPRITES][2];

int level_num;
char level_filename[80];

int level_header[20];
char *pmsg[500] = { NULL };

int zz[20][NUM_ANS];
int passcount;
int draw_frame;

int speed = 40;
int play_level;
int start_mode = 1;
int valid_level_loaded = 0;

int resume_allowed=0;
int top_menu_sel = 3;
int level_time;
int LIVES;

int start_level=1;
int sound_on = 1;
int passcount_timer_fps= 40;

int pbullet[50][6];

int e_bullet_active[50], e_bullet_shape[50];
al_fixed e_bullet_fx[50], e_bullet_fy[50], e_bullet_fxinc[50], e_bullet_fyinc[50];

int level_done = 0;
int game_exit = 1;
int num_enemy;

// counters and temp string
char b_msg[40][80];
int bottom_msg=0;
int game_map_on = 0;

// enemies
int e_num_of_type[50];
int e_first_num[50];
int PDEi[100][32];
al_fixed PDEfx[100][16];
char PDEt[100][20][40];
char eftype_desc[50][16][40];
char eitype_desc[50][32][40];

// items
int item_first_num[20];
int item_coloc_num;
int item_coloc_x;
int item_coloc_y;
char item_edit_text[80];
char item_desc[20][5][40];

int swbl[NUM_SPRITES][2];
int swbn;

int ty = 46;   // button start
int tw = 94;   // button width
int bts = 12;  // button spacing

char sel_filename[500];

int exit_link = 0;
int bx1, bx2, by1, by2;
int slx0=0, sly0=0, slx1=20, sly1=20;

int bmp_index = 255; // used by edit menu
int zzindx = 3;
int b1_color = 3, bs_win = 0;

int get100_x, get100_y; // used by edit functions

// zoom full screen stuff
int stx=10;  // selection window
int sty=10;
int sux=40;
int suy=30;

int copy_blocks=1;
int copy_enemies=1;
int copy_items=1;
int copy_lifts=1;
int copy_mode = 0;
int brf_mode =0;

int lc;

int ft_level_header[20];
int ft_l[100][100];
int ft_item[500][16];
char* ft_pmsg[500];
int ft_Ei[100][32];
al_fixed ft_Efi[100][16];

char ft_ln[NUM_LIFTS][80];
int ft_lift[NUM_LIFTS][4];
int ft_ls[NUM_LIFTS][40][4];

// gui stuff
int text_draw_flag=0;

int wx=0;
int wy=0;

int draw_item_num;
int draw_item_type;
int point_item_num;
int point_item_type;

int old_line_draw_mode;

int line_draw_mode;
int grid_flag = 0;

int pop_msg_viewer_pos;

// status window
int status_window_active = 1;
int status_window_x = 9999;
int status_window_y = 35;
int status_window_w = 320;
int status_window_h = 43;

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

int select_window_block_y;
int btext_draw_flag;
int select_window_special_on = 1;
int select_window_num_special_lines = 3;
int select_window_special_y;
int stext_draw_flag;
int sw_mouse_gone = 0;


void final_wrapup(void)
{
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


void get_hostname(void)
{
   sprintf(msg, "hostname");
   FILE *fp;
   fp = popen(msg,"r");

   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   local_hostname[loop] = 0;
   fclose(fp);
}

void save_config(void)
{
   //printf("save config values\n");
   ALLEGRO_CONFIG * cfg = al_load_config_file("pm.cfg");
   if(!cfg)
   {
      sprintf(msg, "error opening pm.cfg to save");
      m_err(msg);
      printf("%s\n", msg);
   }
   else
   {
      sprintf(msg, "%d", disp_w_wind);
      al_set_config_value(cfg, "SCREEN", "disp_w_wind", msg);

      sprintf(msg, "%d", disp_h_wind);
      al_set_config_value(cfg, "SCREEN", "disp_h_wind", msg);

      sprintf(msg, "%d", disp_x_wind);
      al_set_config_value(cfg, "SCREEN", "disp_x_wind", msg);

      sprintf(msg, "%d", disp_y_wind);
      al_set_config_value(cfg, "SCREEN", "disp_y_wind", msg);

      sprintf(msg, "%d", fullscreen);
      al_set_config_value(cfg, "SCREEN", "fullscreen", msg);

      sprintf(msg, "%f", scale_factor);
      al_set_config_value(cfg, "SCREEN", "scale_factor", msg);

      sprintf(msg, "%d", show_splash_screen);
      al_set_config_value(cfg, "SCREEN", "show_splash_screen", msg);
      if (show_splash_screen) sprintf(global_string[8][11], "Splash Screen:ON ");
      else sprintf(global_string[8][11], "Splash Screen:OFF");

      sprintf(msg, "%d",players[0].color );
      al_set_config_value(cfg, "GAME", "color", msg);

      sprintf(msg, "%d", start_level);
      al_set_config_value(cfg, "GAME", "start_level", msg);
      sprintf(global_string[7][2], "Start Level (%d)", start_level);

      sprintf(msg, "%d", players1[0].up_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_up_key", msg);

      sprintf(msg, "%d", players1[0].down_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_down_key", msg);

      sprintf(msg, "%d", players1[0].left_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_left_key", msg);

      sprintf(msg, "%d", players1[0].right_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_right_key", msg);

      sprintf(msg, "%d", players1[0].jump_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_jump_key", msg);

      sprintf(msg, "%d", players1[0].fire_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_fire_key", msg);

      sprintf(msg, "%d", players1[0].menu_key);
      al_set_config_value(cfg, "GAMECONTROLS", "p0_menu_key", msg);

      sprintf(msg, "%d", sound_on);
      al_set_config_value(cfg, "SOUND", "sound_on", msg);
      if (sound_on) sprintf(global_string[8][7],"Sound:On");
      else sprintf(global_string[8][7],"Sound:Off");

      sprintf(msg, "%d", se_scaler);
      al_set_config_value(cfg, "SOUND", "se_scaler", msg);
      sprintf(global_string[8][8],"Sound Effects Volume:%1d", se_scaler );

      sprintf(msg, "%d", st_scaler);
      al_set_config_value(cfg, "SOUND", "st_scaler", msg);
      sprintf(global_string[8][9],"Sound Track Volume:%1d", st_scaler );

      al_set_config_value(cfg, "NETWORK", "server_IP", m_serveraddress);
      sprintf(global_string[4][3],"Server:%s", m_serveraddress);

      sprintf(msg, "%d", TCP);
      al_set_config_value(cfg, "NETWORK", "TCP", msg);
      if (TCP) strcpy (global_string[4][4], "Packet Type:TCP");
      else strcpy (global_string[4][4], "Packet Type:UDP");

      sprintf(msg, "%d", deathmatch_pbullets);
      al_set_config_value(cfg, "NETWORK", "deathmatch_pbullets", msg);
      if (deathmatch_pbullets) strcpy (global_string[4][5], "Deathmatch Bullets:On ");
      else strcpy (global_string[4][5], "Deathmatch Bullets:Off");

      sprintf(msg, "%d", deathmatch_pbullets_damage);
      al_set_config_value(cfg, "NETWORK", "deathmatch_pbullets_damage", msg);
      sprintf(global_string[4][7], "Deathmatch Bullet Damage:%-2d", deathmatch_pbullets_damage);

      sprintf(msg, "%d", suicide_pbullets);
      al_set_config_value(cfg, "NETWORK", "suicide_pbullets", msg);
      if (suicide_pbullets) strcpy (global_string[4][6], "Suicide Bullets:On ");
      else strcpy (global_string[4][6], "Suicide Bullets:Off");

      sprintf(msg, "%d", control_lead_frames);
      al_set_config_value(cfg, "NETWORK", "control_lead_frames", msg);

      sprintf(msg, "%d", server_lead_frames);
      al_set_config_value(cfg, "NETWORK", "server_lead_frames", msg);

      sprintf(msg, "%d", chdf_freq);
      al_set_config_value(cfg, "NETWORK", "chdf_freq", msg);

      sprintf(msg, "%d", zlib_cmp);
      al_set_config_value(cfg, "NETWORK", "zlib_cmp", msg);


      sprintf(msg, "%d", L_LOGGING);
      al_set_config_value(cfg, "LOGGING", "LOGGING", msg);
      if (L_LOGGING)
         sprintf(global_string[3][ 5],"[ON ]:LOGGING                            ");
      else
         sprintf(global_string[3][ 5],"[OFF]:LOGGING                            ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY", msg);
      if (L_LOGGING_NETPLAY)
         sprintf(global_string[3][ 6],"[ON ]:LOGGING_NETPLAY                    ");
      else
         sprintf(global_string[3][ 6],"[OFF]:LOGGING_NETPLAY                    ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_JOIN);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_JOIN", msg);
      if (L_LOGGING_NETPLAY_JOIN)
         sprintf(global_string[3][ 7],"[ON ]:LOGGING_NETPLAY_JOIN               ");
      else
         sprintf(global_string[3][ 7],"[OFF]:LOGGING_NETPLAY_JOIN               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_bandwidth);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_bandwidth", msg);
      if (L_LOGGING_NETPLAY_bandwidth)
         sprintf(global_string[3][ 8],"[ON ]:LOGGING_NETPLAY_bandwidth          ");
      else
         sprintf(global_string[3][ 8],"[OFF]:LOGGING_NETPLAY_bandwidth          ");


      sprintf(msg, "%d", L_LOGGING_NETPLAY_client_timer_adjust);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_client_timer_adjust", msg);
      if (L_LOGGING_NETPLAY_client_timer_adjust)
         sprintf(global_string[3][ 9],"[ON ]:LOGGING_NETPLAY_client_timer_adjust");
      else
         sprintf(global_string[3][ 9],"[OFF]:LOGGING_NETPLAY_client_timer_adjust");


      sprintf(msg, "%d", L_LOGGING_NETPLAY_cdat);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_cdat", msg);
      if (L_LOGGING_NETPLAY_cdat)
         sprintf(global_string[3][10],"[ON ]:LOGGING_NETPLAY_cdat               ");
      else
         sprintf(global_string[3][10],"[OFF]:LOGGING_NETPLAY_cdat               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_game_move);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_game_move", msg);
      if (L_LOGGING_NETPLAY_game_move)
         sprintf(global_string[3][11],"[ON ]:LOGGING_NETPLAY_game_move          ");
      else
         sprintf(global_string[3][11],"[OFF]:LOGGING_NETPLAY_game_move          ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_sdat);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdat", msg);
      if (L_LOGGING_NETPLAY_sdat)
         sprintf(global_string[3][12],"[ON ]:LOGGING_NETPLAY_sdat               ");
      else
         sprintf(global_string[3][12],"[OFF]:LOGGING_NETPLAY_sdat               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_sdak);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdak", msg);
      if (L_LOGGING_NETPLAY_sdak)
         sprintf(global_string[3][13],"[ON ]:LOGGING_NETPLAY_sdak               ");
      else
         sprintf(global_string[3][13],"[OFF]:LOGGING_NETPLAY_sdak               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_chdf);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf", msg);
      if (L_LOGGING_NETPLAY_chdf)
         sprintf(global_string[3][14],"[ON ]:LOGGING_NETPLAY_chdf               ");
      else
         sprintf(global_string[3][14],"[OFF]:LOGGING_NETPLAY_chdf               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_chdf_all_packets);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf_all_packets", msg);
      if (L_LOGGING_NETPLAY_chdf_all_packets)
         sprintf(global_string[3][15],"[ON ]:LOGGING_NETPLAY_chdf_all_packets   ");
      else
         sprintf(global_string[3][15],"[OFF]:LOGGING_NETPLAY_chdf_all_packets   ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_chdf_when_to_apply);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf_when_to_apply", msg);
      if (L_LOGGING_NETPLAY_chdf_when_to_apply)
         sprintf(global_string[3][16],"[ON ]:LOGGING_NETPLAY_chdf_when_to_apply ");
      else
         sprintf(global_string[3][16],"[OFF]:LOGGING_NETPLAY_chdf_when_to_apply ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_show_dif1);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif1", msg);
      if (L_LOGGING_NETPLAY_show_dif1)
         sprintf(global_string[3][17],"[ON ]:LOGGING_NETPLAY_show_dif1          ");
      else
         sprintf(global_string[3][17],"[OFF]:LOGGING_NETPLAY_show_dif1          ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_show_dif2);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif2", msg);
      if (L_LOGGING_NETPLAY_show_dif2)
         sprintf(global_string[3][18],"[ON ]:LOGGING_NETPLAY_show_dif2          ");
      else
         sprintf(global_string[3][18],"[OFF]:LOGGING_NETPLAY_show_dif2          ");

      sprintf(msg, "%d", auto_save_game_on_level_done);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_level_done", msg);
      if (auto_save_game_on_level_done)
         sprintf(global_string[3][19],"[ON ]:Auto Save Game On Level Done       ");
      else
         sprintf(global_string[3][19],"[OFF]:Auto Save Game On Level Done       ");

      sprintf(msg, "%d", auto_save_game_on_exit);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_exit", msg);
      if (auto_save_game_on_exit)
         sprintf(global_string[3][20],"[ON ]:Auto Save Game On Exit             ");
      else
         sprintf(global_string[3][20],"[OFF]:Auto Save Game On Exit             ");

   }

   al_save_config_file("pm.cfg", cfg);
   al_destroy_config(cfg);
}





void get_config_values(void)
{
   // this only reads the values and validates
   // after that, it immediately calls save_config();
   // to save and update menu text

   //printf("get config values\n");

   //show_disp_values();

   ALLEGRO_CONFIG * cfg = NULL;
   cfg = al_load_config_file("pm.cfg");
   if(!cfg)
   {
      sprintf(msg, "error loading pm.cfg -- creating new pm.cfg");
      //m_err(msg);
      printf("%s\n", msg);
      cfg = al_create_config();
      al_save_config_file("pm.cfg", cfg);
   }


   const char* val;

   val = al_get_config_value(cfg, "SCREEN", "disp_x_wind");
   if (!val) disp_x_wind = 100;
   else disp_x_wind = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "disp_y_wind");
   if (!val) disp_y_wind = 100;
   else disp_y_wind = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "disp_w_wind");
   if (!val) disp_w_wind = 800;
   else disp_w_wind = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "disp_h_wind");
   if (!val) disp_h_wind = 600;
   else disp_h_wind = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "fullscreen");
   if (!val) fullscreen = 1;
   else fullscreen = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "scale_factor");
   if (!val) scale_factor = 1.0;
   else scale_factor = atof(val);
   set_scale_factor(1);


   val = al_get_config_value(cfg, "SCREEN", "show_splash_screen");
   if (!val) show_splash_screen = 1;
   else show_splash_screen = atoi(val);
   if (!show_splash_screen) splash_screen_done = 1;


   val = al_get_config_value(cfg, "GAME", "start_level");
   if (!val) start_level = 1;
   else
   {
      start_level = atoi(val);
      if (start_level < 1) start_level = 1;
      if (start_level > 399) start_level = 399;
   }

   val = al_get_config_value(cfg, "GAME", "color");
   if (!val) players[0].color = 8;
   else
   {
      players[0].color = atoi(val);
      if (players[0].color < 1) players[0].color = 1;
      if (players[0].color > 15) players[0].color = 15;
   }
   players[0].bitmap_index = players[0].color - 1;


//
//
//new default keys:
//arrows, x, z
//
//p0_up_key=84
//p0_down_key=85
//p0_left_key=82
//p0_right_key=83
//p0_jump_key=24
//p0_fire_key=26
//p0_menu_key=59



   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_up_key");
   if (!val) players1[0].up_key = ALLEGRO_KEY_UP;
   else players1[0].up_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_down_key");
   if (!val) players1[0].down_key = ALLEGRO_KEY_DOWN;
   else players1[0].down_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_left_key");
   if (!val) players1[0].left_key = ALLEGRO_KEY_LEFT;
   else players1[0].left_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_right_key");
   if (!val) players1[0].right_key = ALLEGRO_KEY_RIGHT;
   else players1[0].right_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_jump_key");
   if (!val) players1[0].jump_key = ALLEGRO_KEY_X;
   else players1[0].jump_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_fire_key");
   if (!val) players1[0].fire_key = ALLEGRO_KEY_Z;
   else players1[0].fire_key = atoi(val);

//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_up_key");
//   if (!val) players1[0].up_key = ALLEGRO_KEY_I;
//   else players1[0].up_key = atoi(val);
//
//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_down_key");
//   if (!val) players1[0].down_key = ALLEGRO_KEY_K;
//   else players1[0].down_key = atoi(val);
//
//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_left_key");
//   if (!val) players1[0].left_key = ALLEGRO_KEY_J;
//   else players1[0].left_key = atoi(val);
//
//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_right_key");
//   if (!val) players1[0].right_key = ALLEGRO_KEY_L;
//   else players1[0].right_key = atoi(val);
//
//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_jump_key");
//   if (!val) players1[0].jump_key = ALLEGRO_KEY_SPACE;
//   else players1[0].jump_key = atoi(val);
//
//   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_fire_key");
//   if (!val) players1[0].fire_key = ALLEGRO_KEY_C;
//   else players1[0].fire_key = atoi(val);
//



   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_menu_key");
   if (!val) players1[0].menu_key = ALLEGRO_KEY_ESCAPE;
   else players1[0].menu_key = atoi(val);


   val = al_get_config_value(cfg, "SOUND", "sound_on");
   if (!val) sound_on = 1;
   else sound_on = atoi(val);

   val = al_get_config_value(cfg, "SOUND", "se_scaler");
   if (!val) se_scaler = 5;
   else se_scaler = atoi(val);

   val = al_get_config_value(cfg, "SOUND", "st_scaler");
   if (!val) st_scaler = 5;
   else st_scaler = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "server_IP");
   if (!val) sprintf(m_serveraddress, "192.168.1.3");
   else sprintf(m_serveraddress, "%s", val);

   val = al_get_config_value(cfg, "NETWORK", "TCP");
   if (!val) TCP = 0;
   else TCP = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "deathmatch_pbullets");
   if (!val) deathmatch_pbullets = 0;
   else deathmatch_pbullets = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "deathmatch_pbullets_damage");
   if (!val) deathmatch_pbullets_damage = 5;
   else deathmatch_pbullets_damage = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "suicide_pbullets");
   if (!val) suicide_pbullets = 0;
   else suicide_pbullets = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "control_lead_frames");
   if (!val) control_lead_frames = 3;
   else control_lead_frames = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "server_lead_frames");
   if (!val) server_lead_frames = 1;
   else server_lead_frames = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "chdf_freq");
   if (!val) chdf_freq = 5;
   else chdf_freq = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "zlib_cmp");
   if (!val) zlib_cmp = 7;
   else zlib_cmp = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_level_done");
   if (!val) auto_save_game_on_level_done = 0;
   else auto_save_game_on_level_done = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_exit");
   if (!val) auto_save_game_on_exit = 0;
   else auto_save_game_on_exit = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING");
   if (!val) L_LOGGING = 0;
   else L_LOGGING = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY");
   if (!val) L_LOGGING_NETPLAY = 0;
   else L_LOGGING_NETPLAY = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_JOIN");
   if (!val) L_LOGGING_NETPLAY_JOIN = 0;
   else L_LOGGING_NETPLAY_JOIN = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_bandwidth");
   if (!val) L_LOGGING_NETPLAY_bandwidth = 0;
   else L_LOGGING_NETPLAY_bandwidth = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_client_timer_adjust");
   if (!val) L_LOGGING_NETPLAY_client_timer_adjust = 0;
   else L_LOGGING_NETPLAY_client_timer_adjust = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_cdat");
   if (!val) L_LOGGING_NETPLAY_cdat = 0;
   else L_LOGGING_NETPLAY_cdat = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_game_move");
   if (!val) L_LOGGING_NETPLAY_game_move = 0;
   else L_LOGGING_NETPLAY_game_move = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdat");
   if (!val) L_LOGGING_NETPLAY_sdat = 0;
   else L_LOGGING_NETPLAY_sdat = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdak");
   if (!val) L_LOGGING_NETPLAY_sdak = 0;
   else L_LOGGING_NETPLAY_sdak = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf");
   if (!val) L_LOGGING_NETPLAY_chdf = 0;
   else L_LOGGING_NETPLAY_chdf = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf_all_packets");
   if (!val) L_LOGGING_NETPLAY_chdf_all_packets = 0;
   else L_LOGGING_NETPLAY_chdf_all_packets = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_chdf_when_to_apply");
   if (!val) L_LOGGING_NETPLAY_chdf_when_to_apply = 0;
   else L_LOGGING_NETPLAY_chdf_when_to_apply = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif1");
   if (!val) L_LOGGING_NETPLAY_show_dif1 = 0;
   else L_LOGGING_NETPLAY_show_dif1 = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif2");
   if (!val) L_LOGGING_NETPLAY_show_dif2 = 0;
   else L_LOGGING_NETPLAY_show_dif2 = atoi(val);

   al_destroy_config(cfg);
   save_config();

}


void make_palette(void)
{
   // printf("make palette\n");

   palette_color[0]  = al_map_rgb(     0,   0,    0);   //       black
   palette_color[1]  = al_map_rgb(191, 108, 232); //    alt      purple 1
   palette_color[2]  = al_map_rgb(136, 32,  172); //    alt      purple 2
   palette_color[3]  = al_map_rgb(60,  127, 255); //    alt      blue
   palette_color[4]  = al_map_rgb(224, 28,  72);  //    alt      red
   palette_color[5]  = al_map_rgb(255, 0,   232); //    pink
   palette_color[6]  = al_map_rgb(255, 191, 127); //    taan
   palette_color[7]  = al_map_rgb(255, 127, 0);   //    orange
   palette_color[8]  = al_map_rgb(127, 0,   255); //    original purple
   palette_color[9]  = al_map_rgb(     0,   255,  127); //       alt    green
   palette_color[10] = al_map_rgb(255, 0,   0);   //    red
   palette_color[11] = al_map_rgb(     0,   255,  0);   //       green
   palette_color[12] = al_map_rgb(     0,   0,    255); //       blue
   palette_color[13] = al_map_rgb(     0,   255,  255); //       lt     blue
   palette_color[14] = al_map_rgb(255, 255, 0);   //    yellow
   palette_color[15] = al_map_rgb(255, 255, 255); //    white

   // special case reversed white (0)
   for (int x=1; x<16; x++)
   {
      int c = (x+1)*16 - 1;
      palette_color[x*16]  = al_map_rgb(c, c, c);
   }

   // all the other base colors from 1-15
   for (int a=1; a<16; a++) // iterate base colors
   {
      // extract r, g, b in float format
      float r, g, b;
      al_unmap_rgb_f(palette_color[a], &r, &g, &b);

      for (int x=1; x<16; x++)
      {
         float nr = r * (1 - ((float)x/15));
         float ng = g * (1 - ((float)x/15));
         float nb = b * (1 - ((float)x/15));
         palette_color[a+x*16]  = al_map_rgb_f(nr, ng, nb);
      }
   }

//   for (int x=0; x<16; x++)
//   {
//      unsigned char ur, ug, ub;
//      al_unmap_rgb(palette_color[a+x*16], &ur, &ug, &ub);
//      printf("%2d %3d r:%3d g:%3d b:%3d \n", x, a+x*16, ur, ug, ub );
//   }
//   for (int x=0; x<16; x++)
//   al_draw_line(10, 10+x*2, 200, 10+x*2, palette_color[10+x*16], 2);
//   int sz = 16;
//   al_draw_rectangle(0, 0, 17*sz, 17*sz, palette_color[15], 2);
//
//   for (int a=0; a<16; a++)
//      for (int b=0; b<16; b++)
//         al_draw_filled_rectangle(a*sz, b*sz, a*sz+sz, b*sz+sz, palette_color[b*16+a]);
//   al_draw_text(font, palette_color[9], 400, 400, ALLEGRO_ALIGN_CENTRE, "Hello World");

}



void m_err(char * err_msg)
{
   fprintf(stderr, "%s", err_msg);
   al_show_native_message_box(display, "Error", "Error:", err_msg, NULL,  ALLEGRO_MESSAGEBOX_ERROR );
}

void window_title(void)
{
   sprintf(msg, "Purple Martians");
//   sprintf(msg, "Purple Martians %s   [%d x %d]", version_string, SCREEN_W, SCREEN_H);
//   sprintf(msg, "Purple Martians %s   S[%d x %d]  A[%d x %d]   [%d]", version_string, SCREEN_W, SCREEN_H,  disp_w_curr, disp_h_curr, les);
   al_set_window_title(display, msg);
}

int initial_setup(void)
{
   al_init();

   // set version name
   sprintf(version_string, "Version 7.05");
   sprintf(msg, "Purple Martians %s", version_string);
   printf("\n%s\n", msg);
//   int title_len = strlen(msg);
//   for (int i=0; i<title_len; i++)  printf("-");
//   printf("\n");

   if (L_LOGGING)
   {
      #ifdef LOGGING
      add_log_entry_centered_text(20, 0, 76, (char*)"", (char*)"+", (char*)"-");
      add_log_entry_position_text(20, 0, 76, 10, msg, (char*)"|", (char*)" ");
      add_log_entry_centered_text(20, 0, 76, (char*)"", (char*)"+", (char*)"-");
      #endif
   }
   // get allegro version
   uint32_t version = al_get_allegro_version();
   int major = version >> 24;
   int minor = (version >> 16) & 255;
   int revision = (version >> 8) & 255;
   int release = version & 255;
   sprintf(msg, "Allegro version: %d.%d.%d.%d", major, minor, revision, release);
   printf("%s\n", msg);
   if (L_LOGGING)
   {
      #ifdef LOGGING
      add_log_entry_position_text(20, 0, 76, 10, msg, (char*)"|", (char*)" ");
      #endif
   }

   get_hostname();
//   sprintf(msg, "Local hostname:     [%s]",local_hostname);
//   printf("%s\n", msg);
//   #ifdef LOGGING
//   add_log_entry_position_text(20, 0, 76, 10, msg, "|", " ");
//   #endif

   menu_setup();

   get_config_values();

   if (!init_screen()) return 0;

//   for (int i=0; i<title_len; i++)  printf("-");
//   printf("\n");

   if (L_LOGGING)
   {
      #ifdef LOGGING
      add_log_entry_centered_text(20, 0, 76, (char*)"", (char*)"+", (char*)"-");
      #endif
   }


/*
   // show time and date stamp
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);

   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   sprintf(msg, "Date and time:      [%s]",tmsg);
   printf("%s\n", msg);

   #ifdef LOGGING
   add_log_entry_position_text(20, 0, 76, 10, msg, "|", " ");
   add_log_entry_centered_text(20, 0, 76, "", "+", "-");
   #endif
*/


   make_palette();


   if(!al_init_native_dialog_addon())
   {
      m_err((char*)"Failed to initialize native dialog addon.\n");
      return -1;
   }
   //else printf("init native_dialog addon\n");

   if(!al_init_image_addon())
   {
      m_err((char*)"Failed to initialize image addon.\n");
      return -1;
   }
   //else printf("init image addon\n");

   if(!al_init_primitives_addon())
   {
      m_err((char*)"Failed to initialize primitives addon.\n");
      return -1;
   }
   //else printf("init primitives addon\n");

   if(!al_init_font_addon())
   {
      m_err((char*)"Failed to initialize font addon.\n");
      return -1;
   }
   //else printf("init font addon\n");


   if(!al_init_ttf_addon())
   {
      m_err((char*)"Failed to initialize ttf addon.\n");
      return -1;
   }
   //else printf("init ttf addon\n");


   load_fonts();




   if (!al_install_keyboard())
   {
      m_err((char*)"Failed to install keyboard.\n");
      return -1;
   }
   //else printf("installed keyboard\n");

   event_queue = al_create_event_queue();
   if(!event_queue)
   {
      m_err((char*)"Failed to create event queue.\n");
      return -1;
   }
   //else printf("created event queue\n");

   al_register_event_source(event_queue, al_get_keyboard_event_source());
   //printf("registered keyboard event source\n");


   al_register_event_source(event_queue, al_get_display_event_source(display));
   //printf("registered display event source\n");

   if (!al_install_mouse())
   {
      m_err((char*)"Failed to install mouse.\n");
      return -1;
   }
   //else printf("installed mouse\n");
   al_hide_mouse_cursor(display);
   al_register_event_source(event_queue, al_get_mouse_event_source());


   load_sprit();
   al_set_display_icon(display, memory_bitmap[401]);
   fill_player_bitmap();
   fill_door_bitmap();

   seed_mdw();  // for mdw logo
   fill_mdw();

   if (!al_install_joystick())
   {
      m_err((char*)"Failed to install joystick.\n");
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

   if (sound_on) load_sound();

   // create timers
   fps_timer = al_create_timer(1/(float)passcount_timer_fps);
   sec_timer = al_create_timer(1);
   mnu_timer = al_create_timer(.01);

   // register timer event source
   al_register_event_source(event_queue, al_get_timer_event_source(mnu_timer));

   // start timers
   al_start_timer(fps_timer);
   al_start_timer(sec_timer);
   al_start_timer(mnu_timer);

   // init all players
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);

   // set all but 0 to inactive
   for (int p=1; p<NUM_PLAYERS; p++) players[p].active = 0;
   zero_level_data();
   reset_animation_sequence_passcounts(0);
   //printf("end of initial setup\n");
   return 1;
}


void game_menu(void)
{

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
      top_menu_sel = zmenu(7, top_menu_sel, tmy);

      // this must be before 3 because sometimes 3 calls 4 immed
      if ((top_menu_sel == 4) && (resume_allowed)) // resume game
      {
         start_mode = 0;
         al_set_timer_count(fps_timer, passcount); // sync timer_passcount to actual
         game_exit = 0;
         pm_main();
      }
      if (top_menu_sel == 3) // start new game
      {
         play_level = start_level;
         start_mode = 1; // load level and start
         game_exit = 0;
         pm_main();
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
            resume_allowed = 0;
            top_menu_sel = 15; // dummy mode to redraw
         }
      }

      #ifdef NETPLAY
      if (top_menu_sel == 5) // host network game
      {
         play_level = start_level;
         if (server_init())
         {
            start_mode = 1; // load level and start
            game_exit = 0;
            pm_main();
            server_exit();
         }
         else server_exit();
         resume_allowed = 0;
      }
      if (top_menu_sel == 6) // join network game
      {
         if (client_init())
         {
            start_mode = 1; // load level and start
            game_exit = 0;
            pm_main();
            client_exit();
         }
         else client_exit();
         resume_allowed = 0;
      }
      #endif

      if (top_menu_sel == 9) demo_mode(); // demo mode

      if (top_menu_sel == 8) // level editor
      {
         play_level = edit_menu(start_level);
         start_mode = 1;
         game_exit = 0;

         // restore menu items
         set_start_level(play_level);
         set_speed();

         pm_main();
      }

      if (top_menu_sel == 10) help((char*)""); // help


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
            options_menu_sel = zmenu(8, options_menu_sel, tmy + 20);
            if (options_menu_sel == 3)
            {
               int netgame_menu_sel = 2;
               do
               {
                  netgame_menu_sel = zmenu(4, netgame_menu_sel, tmy + 20);
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
                  logging_menu_sel = zmenu(3, logging_menu_sel, tmy + 20);
                  if (logging_menu_sel == 3) // all on
                  {
                     L_LOGGING=1;
                     L_LOGGING_NETPLAY=1;
                     L_LOGGING_NETPLAY_JOIN=1;
                     L_LOGGING_NETPLAY_bandwidth=1;
                     L_LOGGING_NETPLAY_client_timer_adjust=1;
                     L_LOGGING_NETPLAY_cdat=1;
                     L_LOGGING_NETPLAY_game_move=1;
                     L_LOGGING_NETPLAY_sdat=1;
                     L_LOGGING_NETPLAY_sdak=1;
                     L_LOGGING_NETPLAY_chdf=1;
                     L_LOGGING_NETPLAY_chdf_all_packets=1;
                     L_LOGGING_NETPLAY_chdf_when_to_apply=1;
                     L_LOGGING_NETPLAY_show_dif1=1;
                     L_LOGGING_NETPLAY_show_dif2=1;
                     auto_save_game_on_level_done=1;
                     auto_save_game_on_exit=1;
                     save_config();
                  }

                  if (logging_menu_sel == 4 ) // all off
                  {
                     L_LOGGING=0;
                     L_LOGGING_NETPLAY=0;
                     L_LOGGING_NETPLAY_JOIN=0;
                     L_LOGGING_NETPLAY_bandwidth=0;
                     L_LOGGING_NETPLAY_client_timer_adjust=0;
                     L_LOGGING_NETPLAY_cdat=0;
                     L_LOGGING_NETPLAY_game_move=0;
                     L_LOGGING_NETPLAY_sdat=0;
                     L_LOGGING_NETPLAY_sdak=0;
                     L_LOGGING_NETPLAY_chdf=0;
                     L_LOGGING_NETPLAY_chdf_all_packets=0;
                     L_LOGGING_NETPLAY_chdf_when_to_apply=0;
                     L_LOGGING_NETPLAY_show_dif1=0;
                     L_LOGGING_NETPLAY_show_dif2=0;
                     auto_save_game_on_level_done=0;
                     auto_save_game_on_exit=0;
                     save_config();
                  }

                  if (logging_menu_sel == 5)
                  {
                     L_LOGGING= !L_LOGGING;
                     save_config();
                  }
                  if (logging_menu_sel == 6)
                  {
                     L_LOGGING_NETPLAY= !L_LOGGING_NETPLAY;
                     save_config();
                  }
                  if (logging_menu_sel == 7)
                  {
                     L_LOGGING_NETPLAY_JOIN= !L_LOGGING_NETPLAY_JOIN;
                     save_config();
                  }
                  if (logging_menu_sel == 8)
                  {
                     L_LOGGING_NETPLAY_bandwidth= !L_LOGGING_NETPLAY_bandwidth;
                     save_config();
                  }
                  if (logging_menu_sel == 9)
                  {
                     L_LOGGING_NETPLAY_client_timer_adjust= !L_LOGGING_NETPLAY_client_timer_adjust;
                     save_config();
                  }
                  if (logging_menu_sel == 10)
                  {
                     L_LOGGING_NETPLAY_cdat= !L_LOGGING_NETPLAY_cdat;
                     save_config();
                  }
                  if (logging_menu_sel == 11)
                  {
                     L_LOGGING_NETPLAY_game_move= !L_LOGGING_NETPLAY_game_move;
                     save_config();
                  }

                  if (logging_menu_sel == 12)
                  {
                    L_LOGGING_NETPLAY_sdat = !L_LOGGING_NETPLAY_sdat;
                     save_config();
                  }
                  if (logging_menu_sel == 13)
                  {
                     L_LOGGING_NETPLAY_sdak= !L_LOGGING_NETPLAY_sdak;
                     save_config();
                  }
                  if (logging_menu_sel == 14)
                  {
                     L_LOGGING_NETPLAY_chdf= !L_LOGGING_NETPLAY_chdf;
                     save_config();
                  }
                  if (logging_menu_sel == 15)
                  {
                     L_LOGGING_NETPLAY_chdf_all_packets= !L_LOGGING_NETPLAY_chdf_all_packets;
                     save_config();
                  }
                  if (logging_menu_sel == 16)
                  {
                     L_LOGGING_NETPLAY_chdf_when_to_apply= !L_LOGGING_NETPLAY_chdf_when_to_apply;
                     save_config();
                  }
                  if (logging_menu_sel == 17)
                  {
                     L_LOGGING_NETPLAY_show_dif1= !L_LOGGING_NETPLAY_show_dif1;
                     save_config();
                  }
                  if (logging_menu_sel == 18)
                  {
                     L_LOGGING_NETPLAY_show_dif2 = !L_LOGGING_NETPLAY_show_dif2;
                     save_config();
                  }
                  if (logging_menu_sel == 19)
                  {
                     auto_save_game_on_level_done= !auto_save_game_on_level_done;
                     save_config();
                  }
                  if (logging_menu_sel == 20)
                  {
                     auto_save_game_on_exit= !auto_save_game_on_exit;
                     save_config();
                  }
               }  while (logging_menu_sel != 2); // end of netgame options menu
            }

            if (options_menu_sel == 7) sound_toggle();
            if (options_menu_sel == 11) splash_toggle();
            if (options_menu_sel == 12) save_gm();
            if (options_menu_sel == 13) // run game
            {
               if (load_gm((char*)"-"))
               {
                  players[0].control_method = 1;
                  start_mode = 2; // load level and start, but skip game array erasing
                  game_exit = 0;
                  pm_main();
                  players[0].control_method = 0;

                  // reset player data
                  for (int p=0; p<NUM_PLAYERS; p++)
                  {
                      players[p].active = 0;
                      players[p].control_method = 0;
                  }
                  players[0].active = 1;
                  active_local_player = 0;
               }
            }

            if (options_menu_sel == 5) // controller setup menu
            {
               set_key_menu(9, 0, 10); //set controller menu keys
               int p1_menu_sel = 2;
               do
               {
                  p1_menu_sel = zmenu(9, p1_menu_sel, tmy+16);
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
                     players[p].bitmap_index = players[p].color - 1;
                  save_config();
               }

               if (options_menu_sel == 6) // speed ++
               {
                  if (++speed > 500) speed = 500;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol ++
               {
                  extern int se_scaler;
                  if (++se_scaler > 9) se_scaler = 9;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol ++
               {
                  extern int st_scaler;
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
                     players[p].bitmap_index = players[p].color - 1;
                  save_config();
               }
               if (options_menu_sel == 6) // speed --
               {
                  if (--speed < 1) speed = 1;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol --
               {
                  extern int se_scaler;
                  if (--se_scaler < 0) se_scaler = 0;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol --
               {
                  extern int st_scaler;
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
            resume_allowed = 0;
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
            resume_allowed = 0;
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
          ret = copy_files_to_clients(1); // pm.exe only
          exit(0);
      }
      if (strcmp(argument_array[1],"-u") == 0 )
      {
         ret = copy_files_to_clients(0); // all files
         exit(0);
      }
      printf("ret:%d\n",ret);
   }
#endif

   if (initial_setup())
   {

// --------------------------------------------------------------------------------------------
// these flags get processed after allegro is initialized
// --------------------------------------------------------------------------------------------

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
            help((char*)"Command Line");
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

         #ifdef NETPLAY
         if (strcmp(argument_array[1],"-c") == 0 )
         {
            show_splash_screen = 0;
            // no server specified; use the one from the config file
            printf("started client - looking for server IP:%s\n", m_serveraddress);
            if (client_init())
            {
               start_mode = 1; // load level and start
               game_exit = 0;
               pm_main();
            }
            client_exit();
            fast_exit(0);

         }

         if (strcmp(argument_array[1],"-s") == 0 )
         {
            show_splash_screen = 0;
            // no start level specified; use play level from config file
            play_level = start_level;
            if (server_init())
            {
               start_mode = 1; // load level and start
               game_exit = 0;
               pm_main();
            }
            server_exit();
            fast_exit(0);
         }
         #endif


         // keep this last so if no other single flag matches try to run like it an int level...
         // start game on specified level -- eg: 'pm.exe 211'
         int pl = atoi(argument_array[1]);
         if ((pl > 0) && (pl < 400))
         {
            show_splash_screen = 0;
            play_level = pl;
            set_start_level(pl);
            printf("started game on level:%d\n", play_level);
            start_mode = 1; // load level and start
            game_exit = 0;
            pm_main();
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
               start_mode = 1; // load level and start
               game_exit = 0;
               pm_main();
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
               players[0].control_method = 1;
               start_mode = 2; // load level and start, but skip game array erasing
               game_exit = 0;
               pm_main();
               fast_exit(0);
            }
         }

//         #ifndef RELEASE
//         // run saved game file and record video
//         if (strcmp(argument_array[1],"-v") == 0 )
//         {
//            if (load_gm(argument_array[2]))
//            {
//               printf("running game file:%s\n", argument_array[2]);
//               players[0].control_method = 1;
//               start_mode = 2; // load level and start, but skip game array erasing
//               game_exit = 0;
//               making_video = 1;
//
//               char sys_cmd[500];
//               sprintf(sys_cmd, "del screenshots\\frame*.*");
//               printf("%s\n",sys_cmd);
//               system(sys_cmd);
//
//               sprintf(sys_cmd, "del out.mp4");
//               printf("%s\n",sys_cmd);
//               system(sys_cmd);
//
//               pm_main();
//
//               sprintf(sys_cmd, "screenshots\\ffmpeg.exe -framerate 40 -start_number 000 -i \"screenshots\\frame%%05d.bmp\" -c:v h264 -r 40 -pix_fmt yuv420p out.mp4");
//               printf("%s\n",sys_cmd);
//               system(sys_cmd);
//            }
//         }
//         #endif

         #ifdef NETPLAY
         if (strcmp(argument_array[1],"-c") == 0 )
         {
            show_splash_screen = 0;
            sprintf(m_serveraddress, "%s", argument_array[2]);
            printf("started client - looking for passed server IP:%s\n", m_serveraddress);
            save_config();
            //set_config_string("NETWORK", "server_IP", m_serveraddress);
            if (client_init())
            {
               start_mode = 1; // load level and start
               game_exit = 0;
               pm_main();
             }
             client_exit();
             fast_exit(0);
         }
         if (strcmp(argument_array[1],"-s") == 0 )
         {
            show_splash_screen = 0;
            play_level = atoi(argument_array[2]);
            if (server_init())
            {
               start_mode = 1; // load level and start
               game_exit = 0;
               pm_main();
            }
            server_exit();
            fast_exit(0);
         }
         #endif

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
   sprintf(client[num_clients++], "\\\\m-4230-3\\pm_client7"); // ubuntu acer laptop
   sprintf(client[num_clients++], "\\\\DESKTOP-DBNSJH8\\pm_client8"); // win 10 EID work laptop
//   sprintf(client[num_clients++], "\\\\e4230f\\pm_client9"); // acer laptop
//   sprintf(client[num_clients++], "\\\\4230a\\pm_client10"); // acer laptop
//   sprintf(client[num_clients++], "\\\\insp9400\\pm_client11"); // dell insp 9400 (backup machine)
//   sprintf(client[num_clients++], "\\\\m7667\\pm_client12"); // core2 2G 32bit GeForce 7500 LE
//   sprintf(client[num_clients++], "\\\\nv59\\pm_client13");  // gateway nv59 i3 4G 1600x1200
   //sprintf(client[num_clients++], "\\\\y510\\pm_client14");  // y510 XP SP3


//   sprintf(client[num_clients++], "\\\\zi3\\pm_client99");  // zaiden


   if (exe_only == 1)
   {

      printf("copying exe to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "copy pm.exe %s\\pm.exe /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
         sprintf(sys_cmd, "copy pm.cfg %s\\pm.cfg /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
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
