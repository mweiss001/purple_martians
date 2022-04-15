// zcfg.cpp

#include "pm.h"

void save_config(void)
{
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

      sprintf(msg, "%d", stdf_freq);
      al_set_config_value(cfg, "NETWORK", "stdf_freq", msg);

      sprintf(msg, "%d", zlib_cmp);
      al_set_config_value(cfg, "NETWORK", "zlib_cmp", msg);



      sprintf(msg, "%d", L_LOGGING_NETPLAY);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY", msg);
      if (L_LOGGING_NETPLAY)
         sprintf(global_string[3][ 5],"[ON ]:LOGGING_NETPLAY                    ");
      else
         sprintf(global_string[3][ 5],"[OFF]:LOGGING_NETPLAY                    ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_JOIN);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_JOIN", msg);
      if (L_LOGGING_NETPLAY_JOIN)
         sprintf(global_string[3][ 6],"[ON ]:LOGGING_NETPLAY_JOIN               ");
      else
         sprintf(global_string[3][ 6],"[OFF]:LOGGING_NETPLAY_JOIN               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_bandwidth);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_bandwidth", msg);
      if (L_LOGGING_NETPLAY_bandwidth)
         sprintf(global_string[3][ 7],"[ON ]:LOGGING_NETPLAY_bandwidth          ");
      else
         sprintf(global_string[3][ 7],"[OFF]:LOGGING_NETPLAY_bandwidth          ");


      sprintf(msg, "%d", L_LOGGING_NETPLAY_cdat);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_cdat", msg);
      if (L_LOGGING_NETPLAY_cdat)
         sprintf(global_string[3][8],"[ON ]:LOGGING_NETPLAY_cdat               ");
      else
         sprintf(global_string[3][8],"[OFF]:LOGGING_NETPLAY_cdat               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_game_move);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_game_move", msg);
      if (L_LOGGING_NETPLAY_game_move)
         sprintf(global_string[3][9],"[ON ]:LOGGING_NETPLAY_game_move          ");
      else
         sprintf(global_string[3][9],"[OFF]:LOGGING_NETPLAY_game_move          ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_sdat);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdat", msg);
      if (L_LOGGING_NETPLAY_sdat)
         sprintf(global_string[3][10],"[ON ]:LOGGING_NETPLAY_sdat               ");
      else
         sprintf(global_string[3][10],"[OFF]:LOGGING_NETPLAY_sdat               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_sdak);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_sdak", msg);
      if (L_LOGGING_NETPLAY_sdak)
         sprintf(global_string[3][11],"[ON ]:LOGGING_NETPLAY_sdak               ");
      else
         sprintf(global_string[3][11],"[OFF]:LOGGING_NETPLAY_sdak               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_stdf);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf", msg);
      if (L_LOGGING_NETPLAY_stdf)
         sprintf(global_string[3][12],"[ON ]:LOGGING_NETPLAY_stdf               ");
      else
         sprintf(global_string[3][12],"[OFF]:LOGGING_NETPLAY_stdf               ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_stdf_all_packets);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf_all_packets", msg);
      if (L_LOGGING_NETPLAY_stdf_all_packets)
         sprintf(global_string[3][13],"[ON ]:LOGGING_NETPLAY_stdf_all_packets   ");
      else
         sprintf(global_string[3][13],"[OFF]:LOGGING_NETPLAY_stdf_all_packets   ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_stdf_when_to_apply);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf_when_to_apply", msg);
      if (L_LOGGING_NETPLAY_stdf_when_to_apply)
         sprintf(global_string[3][14],"[ON ]:LOGGING_NETPLAY_stdf_when_to_apply ");
      else
         sprintf(global_string[3][14],"[OFF]:LOGGING_NETPLAY_stdf_when_to_apply ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_show_dif1);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif1", msg);
      if (L_LOGGING_NETPLAY_show_dif1)
         sprintf(global_string[3][15],"[ON ]:LOGGING_NETPLAY_show_dif1          ");
      else
         sprintf(global_string[3][15],"[OFF]:LOGGING_NETPLAY_show_dif1          ");

      sprintf(msg, "%d", L_LOGGING_NETPLAY_show_dif2);
      al_set_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif2", msg);
      if (L_LOGGING_NETPLAY_show_dif2)
         sprintf(global_string[3][16],"[ON ]:LOGGING_NETPLAY_show_dif2          ");
      else
         sprintf(global_string[3][16],"[OFF]:LOGGING_NETPLAY_show_dif2          ");

      sprintf(msg, "%d", auto_save_game_on_level_done);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_level_done", msg);
      if (auto_save_game_on_level_done)
         sprintf(global_string[3][17],"[ON ]:Auto Save Game On Level Done       ");
      else
         sprintf(global_string[3][17],"[OFF]:Auto Save Game On Level Done       ");

      sprintf(msg, "%d", auto_save_game_on_exit);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_exit", msg);
      if (auto_save_game_on_exit)
         sprintf(global_string[3][18],"[ON ]:Auto Save Game On Exit             ");
      else
         sprintf(global_string[3][18],"[OFF]:Auto Save Game On Exit             ");


      sprintf(msg, "%d", status_window_x);
      al_set_config_value(cfg, "LEVEL_EDITOR", "status_window_x", msg);

      sprintf(msg, "%d", status_window_y);
      al_set_config_value(cfg, "LEVEL_EDITOR", "status_window_y", msg);

      sprintf(msg, "%d", select_window_x);
      al_set_config_value(cfg, "LEVEL_EDITOR", "select_window_x", msg);

      sprintf(msg, "%d", select_window_y);
      al_set_config_value(cfg, "LEVEL_EDITOR", "select_window_y", msg);

      sprintf(msg, "%d", show_non_default_blocks);
      al_set_config_value(cfg, "LEVEL_EDITOR", "show_non_default_blocks", msg);

      sprintf(msg, "%d", show_flag_details);
      al_set_config_value(cfg, "LEVEL_EDITOR", "show_flag_details", msg);


      sprintf(msg, "%d", autoload_bookmark);
      al_set_config_value(cfg, "LEVEL_EDITOR", "autoload_bookmark", msg);

      sprintf(msg, "%d", bookmark_level);
      al_set_config_value(cfg, "LEVEL_EDITOR", "bookmark_level", msg);

      sprintf(msg, "%d", bookmark_obj);
      al_set_config_value(cfg, "LEVEL_EDITOR", "bookmark_obj", msg);

      sprintf(msg, "%d", bookmark_num);
      al_set_config_value(cfg, "LEVEL_EDITOR", "bookmark_num", msg);


   }

   al_save_config_file("pm.cfg", cfg);
   al_destroy_config(cfg);
}

void get_config_values(void)
{
   menu_setup();

   // this only reads the values and validates
   // after that, it immediately calls save_config() to save and update menu text

   ALLEGRO_CONFIG * cfg = NULL;
   cfg = al_load_config_file("pm.cfg");
   if(!cfg)
   {
      sprintf(msg, "error loading pm.cfg -- creating new pm.cfg");
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

   val = al_get_config_value(cfg, "NETWORK", "stdf_freq");
   if (!val) stdf_freq = 5;
   else stdf_freq = atoi(val);

   val = al_get_config_value(cfg, "NETWORK", "zlib_cmp");
   if (!val) zlib_cmp = 7;
   else zlib_cmp = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_level_done");
   if (!val) auto_save_game_on_level_done = 0;
   else auto_save_game_on_level_done = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_exit");
   if (!val) auto_save_game_on_exit = 0;
   else auto_save_game_on_exit = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY");
   if (!val) L_LOGGING_NETPLAY = 0;
   else L_LOGGING_NETPLAY = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_JOIN");
   if (!val) L_LOGGING_NETPLAY_JOIN = 0;
   else L_LOGGING_NETPLAY_JOIN = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_bandwidth");
   if (!val) L_LOGGING_NETPLAY_bandwidth = 0;
   else L_LOGGING_NETPLAY_bandwidth = atoi(val);

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

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf");
   if (!val) L_LOGGING_NETPLAY_stdf = 0;
   else L_LOGGING_NETPLAY_stdf = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf_all_packets");
   if (!val) L_LOGGING_NETPLAY_stdf_all_packets = 0;
   else L_LOGGING_NETPLAY_stdf_all_packets = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_stdf_when_to_apply");
   if (!val) L_LOGGING_NETPLAY_stdf_when_to_apply = 0;
   else L_LOGGING_NETPLAY_stdf_when_to_apply = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif1");
   if (!val) L_LOGGING_NETPLAY_show_dif1 = 0;
   else L_LOGGING_NETPLAY_show_dif1 = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOGGING_NETPLAY_show_dif2");
   if (!val) L_LOGGING_NETPLAY_show_dif2 = 0;
   else L_LOGGING_NETPLAY_show_dif2 = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "status_window_x");
   if (!val) status_window_x = 10;
   else status_window_x = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "status_window_y");
   if (!val) status_window_y = 10;
   else status_window_y = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "select_window_x");
   if (!val) select_window_x = 10;
   else select_window_x = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "select_window_y");
   if (!val) select_window_y = 10;
   else select_window_y = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "show_non_default_blocks");
   if (!val) show_non_default_blocks = 0;
   else show_non_default_blocks = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "show_flag_details");
   if (!val) show_flag_details = 0;
   else show_flag_details = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "autoload_bookmark");
   if (!val) autoload_bookmark = 0;
   else autoload_bookmark = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "bookmark_level");
   if (!val) bookmark_level = 0;
   else bookmark_level = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "bookmark_obj");
   if (!val) bookmark_obj = 0;
   else bookmark_obj = atoi(val);

   val = al_get_config_value(cfg, "LEVEL_EDITOR", "bookmark_num");
   if (!val) bookmark_num = 0;
   else bookmark_num = atoi(val);



   al_destroy_config(cfg);
   save_config();
}

