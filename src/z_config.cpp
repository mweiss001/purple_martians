// z_config.cpp

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

      sprintf(msg, "%d", display_adapter_num);
      al_set_config_value(cfg, "SCREEN", "display_adapter_num", msg);

      sprintf(msg, "%f", scale_factor);
      al_set_config_value(cfg, "SCREEN", "scale_factor", msg);

      sprintf(msg, "%d", show_splash_screen);
      al_set_config_value(cfg, "SCREEN", "show_splash_screen", msg);

      sprintf(msg, "%d", saved_display_transform_double);
      al_set_config_value(cfg, "SCREEN", "saved_display_transform_double", msg);

      sprintf(msg, "%d",players[0].color );
      al_set_config_value(cfg, "GAME", "color", msg);

      sprintf(msg, "%d", start_level);
      al_set_config_value(cfg, "GAME", "start_level", msg);
      sprintf(global_string[7][2], "Start Level (%d)", start_level);

      sprintf(msg, "%d", demo_mode_config_enable);
      al_set_config_value(cfg, "GAME", "demo_mode_config_enable", msg);

      sprintf(msg, "%0.2f", demo_mode_overlay_opacity);
      al_set_config_value(cfg, "GAME", "demo_mode_overlay_opacity", msg);


      sprintf(msg, "%d", viewport_mode);
      al_set_config_value(cfg, "GAME", "viewport_mode", msg);
      sprintf(msg, "%d", viewport_show_hyst);
      al_set_config_value(cfg, "GAME", "viewport_show_hyst", msg);
      sprintf(msg, "%d", viewport_x_div);
      al_set_config_value(cfg, "GAME", "viewport_x_div", msg);
      sprintf(msg, "%d", viewport_y_div);
      al_set_config_value(cfg, "GAME", "viewport_y_div", msg);













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

      sprintf(msg, "%d", se_scaler);
      al_set_config_value(cfg, "SOUND", "se_scaler", msg);

      sprintf(msg, "%d", st_scaler);
      al_set_config_value(cfg, "SOUND", "st_scaler", msg);

      al_set_config_value(cfg, "NETWORK", "server_IP", m_serveraddress);

      sprintf(msg, "%d", TCP);
      al_set_config_value(cfg, "NETWORK", "TCP", msg);

      sprintf(msg, "%d", deathmatch_pbullets);
      al_set_config_value(cfg, "NETWORK", "deathmatch_pbullets", msg);

      sprintf(msg, "%d", deathmatch_pbullets_damage);
      al_set_config_value(cfg, "NETWORK", "deathmatch_pbullets_damage", msg);

      sprintf(msg, "%d", suicide_pbullets);
      al_set_config_value(cfg, "NETWORK", "suicide_pbullets", msg);

      sprintf(msg, "%d", zlib_cmp);
      al_set_config_value(cfg, "NETWORK", "zlib_cmp", msg);

      sprintf(msg, "%d", LOG_NET);
      al_set_config_value(cfg, "LOGGING", "LOG_NET", msg);

      sprintf(msg, "%d", LOG_NET_join);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_join", msg);

      sprintf(msg, "%d", LOG_NET_player_array);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_player_array", msg);

      sprintf(msg, "%d", LOG_NET_bandwidth);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_bandwidth", msg);

      sprintf(msg, "%d", LOG_NET_cdat);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_cdat", msg);

      sprintf(msg, "%d", LOG_NET_game_move);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_game_move", msg);

      sprintf(msg, "%d", LOG_NET_sdat);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_sdat", msg);

      sprintf(msg, "%d", LOG_NET_sdak);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_sdak", msg);

      sprintf(msg, "%d", LOG_NET_stdf);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_stdf", msg);

      sprintf(msg, "%d", LOG_NET_stdf_all_packets);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_stdf_all_packets", msg);

      sprintf(msg, "%d", LOG_NET_stdf_when_to_apply);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_stdf_when_to_apply", msg);

      sprintf(msg, "%d", LOG_NET_show_dif1);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_show_dif1", msg);

      sprintf(msg, "%d", LOG_NET_show_dif2);
      al_set_config_value(cfg, "LOGGING", "LOG_NET_show_dif2", msg);

      sprintf(msg, "%d", auto_save_game_on_level_done);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_level_done", msg);

      sprintf(msg, "%d", auto_save_game_on_exit);
      al_set_config_value(cfg, "LOGGING", "auto_save_game_on_exit", msg);
   }

   al_save_config_file("pm.cfg", cfg);
   al_destroy_config(cfg);
}

void load_config(void)
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

   val = al_get_config_value(cfg, "SCREEN", "display_adapter_num");
   if (!val) display_adapter_num = 0;
   else display_adapter_num = atoi(val);

   val = al_get_config_value(cfg, "SCREEN", "scale_factor");
   if (!val) set_scale_factor(1, 1);
   else set_scale_factor(atof(val), 1);

   val = al_get_config_value(cfg, "SCREEN", "show_splash_screen");
   if (!val) show_splash_screen = 1;
   else show_splash_screen = atoi(val);
   if (!show_splash_screen) splash_screen_done = 1;

   val = al_get_config_value(cfg, "SCREEN", "saved_display_transform_double");
   if (!val) saved_display_transform_double = 0;
   else saved_display_transform_double = atoi(val);

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

   val = al_get_config_value(cfg, "GAME", "demo_mode_config_enable");
   if (!val) demo_mode_config_enable = 1;
   else demo_mode_config_enable = atoi(val);


   val = al_get_config_value(cfg, "GAME", "demo_mode_overlay_opacity");
   if (!val) demo_mode_overlay_opacity = 0.1;
   else demo_mode_overlay_opacity = atof(val);


   val = al_get_config_value(cfg, "GAME", "viewport_mode");
   if (!val) viewport_mode = 1;
   else viewport_mode = atoi(val);

   val = al_get_config_value(cfg, "GAME", "viewport_show_hyst");
   if (!val) viewport_show_hyst = 0;
   else viewport_show_hyst = atoi(val);

   val = al_get_config_value(cfg, "GAME", "viewport_x_div");
   if (!val) viewport_x_div = 8;
   else viewport_x_div = atoi(val);

   val = al_get_config_value(cfg, "GAME", "viewport_y_div");
   if (!val) viewport_y_div = 12;
   else viewport_y_div = atoi(val);














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
   if (!val) players1[0].jump_key = ALLEGRO_KEY_SPACE;
   else players1[0].jump_key = atoi(val);

   val = al_get_config_value(cfg, "GAMECONTROLS", "p0_fire_key");
   if (!val) players1[0].fire_key = ALLEGRO_KEY_ALT;
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
   if (!val) st_scaler = 2;
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

   val = al_get_config_value(cfg, "NETWORK", "zlib_cmp");
   if (!val) zlib_cmp = 7;
   else zlib_cmp = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_level_done");
   if (!val) auto_save_game_on_level_done = 0;
   else auto_save_game_on_level_done = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "auto_save_game_on_exit");
   if (!val) auto_save_game_on_exit = 0;
   else auto_save_game_on_exit = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET");
   if (!val) LOG_NET = 0;
   else LOG_NET = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_player_array");
   if (!val) LOG_NET_player_array = 0;
   else LOG_NET_player_array = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_join");
   if (!val) LOG_NET_join = 0;
   else LOG_NET_join = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_bandwidth");
   if (!val) LOG_NET_bandwidth = 0;
   else LOG_NET_bandwidth = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_cdat");
   if (!val) LOG_NET_cdat = 0;
   else LOG_NET_cdat = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_game_move");
   if (!val) LOG_NET_game_move = 0;
   else LOG_NET_game_move = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_sdat");
   if (!val) LOG_NET_sdat = 0;
   else LOG_NET_sdat = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_sdak");
   if (!val) LOG_NET_sdak = 0;
   else LOG_NET_sdak = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_stdf");
   if (!val) LOG_NET_stdf = 0;
   else LOG_NET_stdf = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_stdf_all_packets");
   if (!val) LOG_NET_stdf_all_packets = 0;
   else LOG_NET_stdf_all_packets = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_stdf_when_to_apply");
   if (!val) LOG_NET_stdf_when_to_apply = 0;
   else LOG_NET_stdf_when_to_apply = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_show_dif1");
   if (!val) LOG_NET_show_dif1 = 0;
   else LOG_NET_show_dif1 = atoi(val);

   val = al_get_config_value(cfg, "LOGGING", "LOG_NET_show_dif2");
   if (!val) LOG_NET_show_dif2 = 0;
   else LOG_NET_show_dif2 = atoi(val);

   al_destroy_config(cfg);
   save_config();
}
