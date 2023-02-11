// z_config.cpp

#include "pm.h"
#include "z_log.h"
#include "z_sound.h"
#include "z_settings.h"
#include "z_player.h"
#include "n_netgame.h"



#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define agci(section, var, def) val = al_get_config_value(cfg, STRINGIFY(section), STRINGIFY(var)); if (!val) var = def;  else var = atoi(val);
#define asci(section, var) sprintf(msg, "%d", var); al_set_config_value(cfg, STRINGIFY(section), STRINGIFY(var), msg);
#define agcf(section, var, def) val = al_get_config_value(cfg, STRINGIFY(section), STRINGIFY(var)); if (!val) var = def;  else var = atof(val);
#define ascf(section, var) sprintf(msg, "%f", var); al_set_config_value(cfg, STRINGIFY(section), STRINGIFY(var), msg);

/*
https://godbolt.org/
gcc-E
main()
{
    agci(SCREEN, disp_x_wind, 100)
    asci(SCREEN, disp_x_wind)
    agcf(SCREEN, scale_factor, 0.75)
    ascf(SCREEN, scale_factor)
}
main()
{
    val = al_get_config_value(cfg, "SCREEN", "disp_x_wind"); if (!val) disp_x_wind = 100; else disp_x_wind = atoi(val);
    sprintf(msg, "%d", disp_x_wind); al_set_config_value(cfg, "SCREEN", "disp_x_wind", msg);
    val = al_get_config_value(cfg, "SCREEN", "scale_factor"); if (!val) scale_factor = 0.75; else scale_factor = atof(val);
    sprintf(msg, "%0.3f", scale_factor); al_set_config_value(cfg, "SCREEN", "scale_factor", msg);
}
*/



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
      asci(SCREEN, disp_x_wind)
      asci(SCREEN, disp_h_wind)
      asci(SCREEN, disp_w_wind)
      asci(SCREEN, disp_h_wind)
      ascf(SCREEN, scale_factor)
      asci(SCREEN, fullscreen)
      asci(SCREEN, display_adapter_num)
      asci(SCREEN, show_splash_screen)
      asci(SCREEN, bottom_msg_on)

      asci(SCREEN, saved_display_transform_double)
      asci(SCREEN, display_transform_double_max)

      asci(SCREEN, eco_draw)



      asci(GAME, players[0].color)
      asci(GAME, start_level)
      asci(GAME, viewport_mode)
      asci(GAME, viewport_show_hyst)
      ascf(GAME, viewport_x_div)
      ascf(GAME, viewport_y_div)
      asci(GAME, settings_current_page)
      asci(GAME, speed_control_lock)

      asci(GAMECONTROLS, players1[0].up_key)
      asci(GAMECONTROLS, players1[0].down_key)
      asci(GAMECONTROLS, players1[0].left_key)
      asci(GAMECONTROLS, players1[0].right_key)
      asci(GAMECONTROLS, players1[0].jump_key)
      asci(GAMECONTROLS, players1[0].fire_key)
      asci(GAMECONTROLS, players1[0].menu_key)
      asci(SOUND, sound_on)
      asci(SOUND, se_scaler)
      asci(SOUND, st_scaler)

      al_set_config_value(cfg, "NETWORK", "server_IP", m_serveraddress);
      asci(NETWORK, TCP)
      asci(NETWORK, deathmatch_pbullets)
      asci(NETWORK, deathmatch_pbullets_damage)
      asci(NETWORK, suicide_pbullets)

      asci(NETWORK, players1[0].server_state_freq_mode)



      asci(LOGGING, LOG_NET)
      asci(LOGGING, LOG_NET_join)
      asci(LOGGING, LOG_NET_player_array)
      asci(LOGGING, LOG_NET_bandwidth)
      asci(LOGGING, LOG_NET_cdat)
      asci(LOGGING, LOG_NET_stdf)
      asci(LOGGING, LOG_NET_stdf_all_packets)

      asci(LOGGING, LOG_NET_dif_applied)
      asci(LOGGING, LOG_NET_dif_not_applied)

      asci(LOGGING, LOG_NET_client_ping)
      asci(LOGGING, LOG_NET_client_timer_adj)
      asci(LOGGING, LOG_NET_server_rx_stak)

      asci(LOGGING, LOG_TMR_cpu)
      asci(LOGGING, LOG_TMR_move_tot)
      asci(LOGGING, LOG_TMR_move_all)
      asci(LOGGING, LOG_TMR_move_enem)
      asci(LOGGING, LOG_TMR_bmsg_add)
      asci(LOGGING, LOG_TMR_bmsg_draw)
      asci(LOGGING, LOG_TMR_scrn_overlay)



      asci(LOGGING, LOG_TMR_draw_tot)
      asci(LOGGING, LOG_TMR_draw_all)
      asci(LOGGING, LOG_TMR_sdif)
      asci(LOGGING, LOG_TMR_cdif)
      asci(LOGGING, LOG_TMR_rwnd)
      asci(LOGGING, LOG_TMR_client_timer_adj)
      asci(LOGGING, LOG_TMR_client_ping)





      asci(LOGGING, autosave_log_on_program_exit)
      asci(LOGGING, autosave_log_on_game_exit)
      asci(LOGGING, autosave_log_on_level_done)

      asci(DEMO, demo_mode_config_enable)
      asci(DEMO, autosave_game_on_level_done)
      asci(DEMO, autosave_game_on_game_exit)
      ascf(DEMO, demo_mode_overlay_opacity)


      // cpu
      asci(OVERLAY, overlay_grid[0][0]);
      asci(OVERLAY, overlay_grid[0][1]);
      asci(OVERLAY, overlay_grid[0][2]);
      asci(OVERLAY, overlay_grid[0][3]);

      // display
      asci(OVERLAY, overlay_grid[1][0]);
      asci(OVERLAY, overlay_grid[1][1]);
      asci(OVERLAY, overlay_grid[1][2]);
      asci(OVERLAY, overlay_grid[1][3]);

      // draw profile
      asci(OVERLAY, overlay_grid[2][0]);
      asci(OVERLAY, overlay_grid[2][1]);
      asci(OVERLAY, overlay_grid[2][2]);
      asci(OVERLAY, overlay_grid[2][3]);

      // debug grid
      asci(OVERLAY, overlay_grid[3][0]);
      asci(OVERLAY, overlay_grid[3][1]);
      asci(OVERLAY, overlay_grid[3][2]);
      asci(OVERLAY, overlay_grid[3][3]);

      // sync graph
      asci(OVERLAY, overlay_grid[4][0]);
      asci(OVERLAY, overlay_grid[4][1]);
      asci(OVERLAY, overlay_grid[4][2]);
      asci(OVERLAY, overlay_grid[4][3]);

      // sync adjust
      asci(OVERLAY, overlay_grid[5][0]);
      asci(OVERLAY, overlay_grid[5][1]);
      asci(OVERLAY, overlay_grid[5][2]);
      asci(OVERLAY, overlay_grid[5][3]);

      // state freq adj
      asci(OVERLAY, overlay_grid[6][0]);
      asci(OVERLAY, overlay_grid[6][1]);
      asci(OVERLAY, overlay_grid[6][2]);
      asci(OVERLAY, overlay_grid[6][3]);

      // bandwidth stats
      asci(OVERLAY, overlay_grid[7][0]);
      asci(OVERLAY, overlay_grid[7][1]);
      asci(OVERLAY, overlay_grid[7][2]);
      asci(OVERLAY, overlay_grid[7][3]);

      // misc
      asci(OVERLAY, overlay_grid[8][0]);
      asci(OVERLAY, overlay_grid[8][1]);
      asci(OVERLAY, overlay_grid[8][2]);
      asci(OVERLAY, overlay_grid[8][3]);

      asci(OVERLAY, number_of_debug_overlay_modes);

      asci(LEVEL_EDITOR, autosave_level_editor_state);

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

   agci(SCREEN, disp_x_wind, 100)
   agci(SCREEN, disp_y_wind, 100)
   agci(SCREEN, disp_w_wind, 800)
   agci(SCREEN, disp_h_wind, 600)
   agci(SCREEN, fullscreen, 1)
   agci(SCREEN, display_adapter_num, 0)
   agci(SCREEN, show_splash_screen, 1)
   agci(SCREEN, display_transform_double_max, 3)
   agci(SCREEN, saved_display_transform_double, 0)

   agcf(SCREEN, scale_factor, 1.0)
   set_scale_factor(/*scale_factor*/ 1.0, 1);

   agci(SCREEN, show_splash_screen, 1)
   if (!show_splash_screen) splash_screen_done = 1;

   agci(SCREEN, bottom_msg_on, 1)


   agci(SCREEN, eco_draw, 0)




   agci(GAME, start_level, 1)
   set_start_level(start_level);

   agci(GAME, players[0].color, 8)
   set_player_color(0, players[0].color);

   agci(GAME, viewport_mode, 1)
   agci(GAME, viewport_show_hyst, 0)
   agcf(GAME, viewport_x_div, 0.33)
   agcf(GAME, viewport_y_div, 0.33)
   agci(GAME, settings_current_page, 0)
   agci(GAME, speed_control_lock, 1)

   agci(GAMECONTROLS, players1[0].up_key,    ALLEGRO_KEY_UP)
   agci(GAMECONTROLS, players1[0].down_key,  ALLEGRO_KEY_DOWN)
   agci(GAMECONTROLS, players1[0].left_key,  ALLEGRO_KEY_LEFT)
   agci(GAMECONTROLS, players1[0].right_key, ALLEGRO_KEY_RIGHT)
   agci(GAMECONTROLS, players1[0].jump_key,  ALLEGRO_KEY_SPACE)
   agci(GAMECONTROLS, players1[0].fire_key,  ALLEGRO_KEY_ALT)
   agci(GAMECONTROLS, players1[0].menu_key,  ALLEGRO_KEY_ESCAPE)

   agci(SOUND, sound_on, 1)
   agci(SOUND, se_scaler, 5)
   agci(SOUND, st_scaler, 2)

   val = al_get_config_value(cfg, "NETWORK", "server_IP");
   if (!val) sprintf(m_serveraddress, "192.168.1.3");
   else sprintf(m_serveraddress, "%s", val);

   agci(NETWORK, TCP, 0)
   agci(NETWORK, deathmatch_pbullets, 0)
   agci(NETWORK, deathmatch_pbullets_damage, 5)
   agci(NETWORK, suicide_pbullets, 0)

   agci(NETWORK, players1[0].server_state_freq_mode, 1)


   agci(LOGGING, LOG_NET, 0)
   agci(LOGGING, LOG_NET_player_array, 0)
   agci(LOGGING, LOG_NET_join, 0)
   agci(LOGGING, LOG_NET_bandwidth, 0)
   agci(LOGGING, LOG_NET_cdat, 0)
   agci(LOGGING, LOG_NET_stdf, 0)
   agci(LOGGING, LOG_NET_stdf_all_packets, 0)

   agci(LOGGING, LOG_NET_dif_applied, 0)
   agci(LOGGING, LOG_NET_dif_not_applied, 0)

   agci(LOGGING, LOG_NET_client_ping, 0)
   agci(LOGGING, LOG_NET_client_timer_adj, 0)
   agci(LOGGING, LOG_NET_server_rx_stak, 0)


   agci(LOGGING, LOG_TMR_cpu, 0)
   agci(LOGGING, LOG_TMR_move_tot, 0)
   agci(LOGGING, LOG_TMR_move_all, 0)
   agci(LOGGING, LOG_TMR_move_enem, 0)
   agci(LOGGING, LOG_TMR_bmsg_add, 0)
   agci(LOGGING, LOG_TMR_bmsg_draw, 0)
   agci(LOGGING, LOG_TMR_scrn_overlay, 0)


   agci(LOGGING, LOG_TMR_draw_tot, 0)
   agci(LOGGING, LOG_TMR_draw_all, 0)
   agci(LOGGING, LOG_TMR_sdif, 0)
   agci(LOGGING, LOG_TMR_cdif, 0)
   agci(LOGGING, LOG_TMR_rwnd, 0)

   agci(LOGGING, LOG_TMR_client_timer_adj, 0)
   agci(LOGGING, LOG_TMR_client_ping, 0)



   agci(LOGGING, autosave_log_on_program_exit, 0)
   agci(LOGGING, autosave_log_on_game_exit, 0)
   agci(LOGGING, autosave_log_on_level_done, 0)

   agci(DEMO, demo_mode_config_enable, 1)
   agci(DEMO, autosave_game_on_level_done, 0)
   agci(DEMO, autosave_game_on_game_exit, 0)
   agcf(DEMO, demo_mode_overlay_opacity, 0.1)






   // cpu
   agci(OVERLAY, overlay_grid[0][0], 0);
   agci(OVERLAY, overlay_grid[0][1], 1);
   agci(OVERLAY, overlay_grid[0][2], 0);
   agci(OVERLAY, overlay_grid[0][3], 0);

   // display
   agci(OVERLAY, overlay_grid[1][0], 0);
   agci(OVERLAY, overlay_grid[1][1], 0);
   agci(OVERLAY, overlay_grid[1][2], 0);
   agci(OVERLAY, overlay_grid[1][3], 0);

   // draw profile
   agci(OVERLAY, overlay_grid[2][0], 0);
   agci(OVERLAY, overlay_grid[2][1], 0);
   agci(OVERLAY, overlay_grid[2][2], 0);
   agci(OVERLAY, overlay_grid[2][3], 0);

   // debug grid
   agci(OVERLAY, overlay_grid[3][0], 0);
   agci(OVERLAY, overlay_grid[3][1], 0);
   agci(OVERLAY, overlay_grid[3][2], 0);
   agci(OVERLAY, overlay_grid[3][3], 0);

   // sync graph
   agci(OVERLAY, overlay_grid[4][0], 0);
   agci(OVERLAY, overlay_grid[4][1], 0);
   agci(OVERLAY, overlay_grid[4][2], 0);
   agci(OVERLAY, overlay_grid[4][3], 0);

   // sync adjust
   agci(OVERLAY, overlay_grid[5][0], 0);
   agci(OVERLAY, overlay_grid[5][1], 0);
   agci(OVERLAY, overlay_grid[5][2], 0);
   agci(OVERLAY, overlay_grid[5][3], 0);

   // state freq adj
   agci(OVERLAY, overlay_grid[6][0], 0);
   agci(OVERLAY, overlay_grid[6][1], 0);
   agci(OVERLAY, overlay_grid[6][2], 0);
   agci(OVERLAY, overlay_grid[6][3], 0);

   // bandwidth stats
   agci(OVERLAY, overlay_grid[7][0], 0);
   agci(OVERLAY, overlay_grid[7][1], 0);
   agci(OVERLAY, overlay_grid[7][2], 0);
   agci(OVERLAY, overlay_grid[7][3], 0);

   // misc
   agci(OVERLAY, overlay_grid[8][0], 0);
   agci(OVERLAY, overlay_grid[8][1], 0);
   agci(OVERLAY, overlay_grid[8][2], 0);
   agci(OVERLAY, overlay_grid[8][3], 0);

   agci(OVERLAY, number_of_debug_overlay_modes, 2);

   agci(LEVEL_EDITOR, autosave_level_editor_state, 0);

   al_destroy_config(cfg);
   save_config();
}

