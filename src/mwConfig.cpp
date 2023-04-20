// mwConfig.cpp

#include "pm.h"
#include "mwConfig.h"
#include "mwLog.h"
#include "mwSound.h"
#include "mwSettings.h"
#include "mwPlayers.h"
#include "mwNetgame.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwShots.h"
#include "mwInput.h"

mwConfig mConfig;

void mwConfig::save(void)
{
   char msg[1024];
   ALLEGRO_CONFIG * cfg = al_load_config_file("pm.cfg");
   if(!cfg)
   {
      sprintf(msg, "error opening pm.cfg to save");
      mInput.m_err(msg);
      printf("%s\n", msg);
   }
   else
   {
      asci(SCREEN, mDisplay.disp_x_wind)
      asci(SCREEN, mDisplay.disp_y_wind)
      asci(SCREEN, mDisplay.disp_w_wind)
      asci(SCREEN, mDisplay.disp_h_wind)
      ascf(SCREEN, mDisplay.scale_factor)
      asci(SCREEN, mDisplay.fullscreen)
      asci(SCREEN, mDisplay.display_adapter_num)
      asci(SCREEN, mLogo.show_splash_screen)
      asci(SCREEN, mBottomMessage.bottom_msg_on)

      asci(SCREEN, mDisplay.saved_display_transform_double)
      asci(SCREEN, mDisplay.display_transform_double_max)

      asci(SCREEN, mLoop.eco_draw)

      asci(GAME, mPlayer.syn[0].color)
      asci(GAME, mLevel.start_level)
      asci(GAME, mDisplay.viewport_mode)
      asci(GAME, mDisplay.viewport_show_hyst)
      ascf(GAME, mDisplay.viewport_x_div)
      ascf(GAME, mDisplay.viewport_y_div)
      asci(GAME, mSettings.settings_current_page)
      asci(GAME, mLoop.speed_control_lock)

      asci(GAMECONTROLS, mPlayer.loc[0].up_key)
      asci(GAMECONTROLS, mPlayer.loc[0].down_key)
      asci(GAMECONTROLS, mPlayer.loc[0].left_key)
      asci(GAMECONTROLS, mPlayer.loc[0].right_key)
      asci(GAMECONTROLS, mPlayer.loc[0].jump_key)
      asci(GAMECONTROLS, mPlayer.loc[0].fire_key)
      asci(GAMECONTROLS, mPlayer.loc[0].menu_key)
      asci(SOUND, mSound.sound_on)
      asci(SOUND, mSound.se_scaler)
      asci(SOUND, mSound.st_scaler)

      al_set_config_value(cfg, "NETWORK", "server_IP", mNetgame.m_serveraddress);
      asci(NETWORK, mNetgame.TCP)
      asci(NETWORK, mShot.deathmatch_shots)
      asci(NETWORK, mShot.deathmatch_shot_damage)
      asci(NETWORK, mShot.suicide_shots)

      asci(NETWORK, mPlayer.loc[0].server_state_freq_mode)



      asci(LOGGING, mLog.LOG_NET)
      asci(LOGGING, mLog.LOG_NET_join)
      asci(LOGGING, mLog.LOG_NET_player_array)
      asci(LOGGING, mLog.LOG_NET_bandwidth)
      asci(LOGGING, mLog.LOG_NET_cdat)
      asci(LOGGING, mLog.LOG_NET_stdf)
      asci(LOGGING, mLog.LOG_NET_stdf_all_packets)

      asci(LOGGING, mLog.LOG_NET_dif_applied)
      asci(LOGGING, mLog.LOG_NET_dif_not_applied)

      asci(LOGGING, mLog.LOG_NET_client_ping)
      asci(LOGGING, mLog.LOG_NET_client_timer_adj)
      asci(LOGGING, mLog.LOG_NET_server_rx_stak)

      asci(LOGGING, mLog.LOG_TMR_cpu)
      asci(LOGGING, mLog.LOG_TMR_move_tot)
      asci(LOGGING, mLog.LOG_TMR_move_all)
      asci(LOGGING, mLog.LOG_TMR_move_enem)
      asci(LOGGING, mLog.LOG_TMR_bmsg_add)
      asci(LOGGING, mLog.LOG_TMR_bmsg_draw)
      asci(LOGGING, mLog.LOG_TMR_scrn_overlay)



      asci(LOGGING, mLog.LOG_TMR_draw_tot)
      asci(LOGGING, mLog.LOG_TMR_draw_all)
      asci(LOGGING, mLog.LOG_TMR_sdif)
      asci(LOGGING, mLog.LOG_TMR_cdif)
      asci(LOGGING, mLog.LOG_TMR_rwnd)
      asci(LOGGING, mLog.LOG_TMR_client_timer_adj)
      asci(LOGGING, mLog.LOG_TMR_client_ping)





      asci(LOGGING, mLog.autosave_log_on_program_exit)
      asci(LOGGING, mLog.autosave_log_on_game_exit)
      asci(LOGGING, mLog.autosave_log_on_level_done)

      asci(DEMO, mDemoMode.demo_mode_config_enable)
      asci(DEMO, mLog.autosave_game_on_level_done)
      asci(DEMO, mLog.autosave_game_on_game_exit)
      ascf(DEMO, mDemoMode.demo_mode_overlay_opacity)

      asci(BMSG, mBottomMessage.num_lines)

      ascf(BMSG, mBottomMessage.io)
      ascf(BMSG, mBottomMessage.fo)
      ascf(BMSG, mBottomMessage.ihs)
      ascf(BMSG, mBottomMessage.ivs)
      ascf(BMSG, mBottomMessage.fhs)
      ascf(BMSG, mBottomMessage.fvs)

      asci(BMSG, mBottomMessage.disp_player)
      asci(BMSG, mBottomMessage.disp_enemy)
      asci(BMSG, mBottomMessage.disp_item)
      asci(BMSG, mBottomMessage.disp_health)


      // cpu
      asci(OVERLAY, mSettings.overlay_grid[0][0]);
      asci(OVERLAY, mSettings.overlay_grid[0][1]);
      asci(OVERLAY, mSettings.overlay_grid[0][2]);
      asci(OVERLAY, mSettings.overlay_grid[0][3]);

      // display
      asci(OVERLAY, mSettings.overlay_grid[1][0]);
      asci(OVERLAY, mSettings.overlay_grid[1][1]);
      asci(OVERLAY, mSettings.overlay_grid[1][2]);
      asci(OVERLAY, mSettings.overlay_grid[1][3]);

      // draw profile
      asci(OVERLAY, mSettings.overlay_grid[2][0]);
      asci(OVERLAY, mSettings.overlay_grid[2][1]);
      asci(OVERLAY, mSettings.overlay_grid[2][2]);
      asci(OVERLAY, mSettings.overlay_grid[2][3]);

      // debug grid
      asci(OVERLAY, mSettings.overlay_grid[3][0]);
      asci(OVERLAY, mSettings.overlay_grid[3][1]);
      asci(OVERLAY, mSettings.overlay_grid[3][2]);
      asci(OVERLAY, mSettings.overlay_grid[3][3]);

      // sync graph
      asci(OVERLAY, mSettings.overlay_grid[4][0]);
      asci(OVERLAY, mSettings.overlay_grid[4][1]);
      asci(OVERLAY, mSettings.overlay_grid[4][2]);
      asci(OVERLAY, mSettings.overlay_grid[4][3]);

      // sync adjust
      asci(OVERLAY, mSettings.overlay_grid[5][0]);
      asci(OVERLAY, mSettings.overlay_grid[5][1]);
      asci(OVERLAY, mSettings.overlay_grid[5][2]);
      asci(OVERLAY, mSettings.overlay_grid[5][3]);

      // state freq adj
      asci(OVERLAY, mSettings.overlay_grid[6][0]);
      asci(OVERLAY, mSettings.overlay_grid[6][1]);
      asci(OVERLAY, mSettings.overlay_grid[6][2]);
      asci(OVERLAY, mSettings.overlay_grid[6][3]);

      // bandwidth stats
      asci(OVERLAY, mSettings.overlay_grid[7][0]);
      asci(OVERLAY, mSettings.overlay_grid[7][1]);
      asci(OVERLAY, mSettings.overlay_grid[7][2]);
      asci(OVERLAY, mSettings.overlay_grid[7][3]);

      // misc
      asci(OVERLAY, mSettings.overlay_grid[8][0]);
      asci(OVERLAY, mSettings.overlay_grid[8][1]);
      asci(OVERLAY, mSettings.overlay_grid[8][2]);
      asci(OVERLAY, mSettings.overlay_grid[8][3]);

      asci(OVERLAY, mSettings.number_of_debug_overlay_modes);

      asci(LEVEL_EDITOR, mLoop.autosave_level_editor_state);

   }
   al_save_config_file("pm.cfg", cfg);
   al_destroy_config(cfg);
}

void mwConfig::load(void)
{
   // this reads values and validates
   // after that, it immediately calls save in case values were not found and defaults were used

   char msg[1024];
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

   agci(SCREEN, mDisplay.disp_x_wind, 100)
   agci(SCREEN, mDisplay.disp_y_wind, 100)
   agci(SCREEN, mDisplay.disp_w_wind, 800)
   agci(SCREEN, mDisplay.disp_h_wind, 600)
   agci(SCREEN, mDisplay.fullscreen, 1)
   agci(SCREEN, mDisplay.display_adapter_num, 0)
   agci(SCREEN, mLogo.show_splash_screen, 1)
   agci(SCREEN, mDisplay.display_transform_double_max, 3)
   agci(SCREEN, mDisplay.saved_display_transform_double, 0)

   agcf(SCREEN, mDisplay.scale_factor, 1.0)
   //mDisplay.set_scale_factor(1.0, 1); // reset to 1.0 with every program start
   mDisplay.set_scale_factor(mDisplay.scale_factor, 1);

   agci(SCREEN, mLogo.show_splash_screen, 1)
   if (!mLogo.show_splash_screen) mLogo.splash_screen_done = 1;

   agci(SCREEN, mBottomMessage.bottom_msg_on, 1)

   agci(SCREEN, mLoop.eco_draw, 0)


   agci(GAME, mLevel.start_level, 1)

   agci(GAME, mPlayer.syn[0].color, 8)

   agci(GAME, mDisplay.viewport_mode, 1)
   agci(GAME, mDisplay.viewport_show_hyst, 0)
   agcf(GAME, mDisplay.viewport_x_div, 0.33)
   agcf(GAME, mDisplay.viewport_y_div, 0.33)
   agci(GAME, mSettings.settings_current_page, 0)
   agci(GAME, mLoop.speed_control_lock, 1)

   agci(GAMECONTROLS, mPlayer.loc[0].up_key,    ALLEGRO_KEY_UP)
   agci(GAMECONTROLS, mPlayer.loc[0].down_key,  ALLEGRO_KEY_DOWN)
   agci(GAMECONTROLS, mPlayer.loc[0].left_key,  ALLEGRO_KEY_LEFT)
   agci(GAMECONTROLS, mPlayer.loc[0].right_key, ALLEGRO_KEY_RIGHT)
   agci(GAMECONTROLS, mPlayer.loc[0].jump_key,  ALLEGRO_KEY_SPACE)
   agci(GAMECONTROLS, mPlayer.loc[0].fire_key,  ALLEGRO_KEY_ALT)
   agci(GAMECONTROLS, mPlayer.loc[0].menu_key,  ALLEGRO_KEY_ESCAPE)

   agci(SOUND, mSound.sound_on, 1)
   agci(SOUND, mSound.se_scaler, 5)
   agci(SOUND, mSound.st_scaler, 2)

   val = al_get_config_value(cfg, "NETWORK", "server_IP");
   if (!val) sprintf(mNetgame.m_serveraddress, "192.168.1.3");
   else sprintf(mNetgame.m_serveraddress, "%s", val);

   agci(NETWORK, mNetgame.TCP, 0)
   agci(NETWORK, mShot.deathmatch_shots, 0)
   agci(NETWORK, mShot.deathmatch_shot_damage, 5)
   agci(NETWORK, mShot.suicide_shots, 0)

   agci(NETWORK, mPlayer.loc[0].server_state_freq_mode, 1)


   agci(LOGGING, mLog.LOG_NET, 0)
   agci(LOGGING, mLog.LOG_NET_player_array, 0)
   agci(LOGGING, mLog.LOG_NET_join, 0)
   agci(LOGGING, mLog.LOG_NET_bandwidth, 0)
   agci(LOGGING, mLog.LOG_NET_cdat, 0)
   agci(LOGGING, mLog.LOG_NET_stdf, 0)
   agci(LOGGING, mLog.LOG_NET_stdf_all_packets, 0)

   agci(LOGGING, mLog.LOG_NET_dif_applied, 0)
   agci(LOGGING, mLog.LOG_NET_dif_not_applied, 0)

   agci(LOGGING, mLog.LOG_NET_client_ping, 0)
   agci(LOGGING, mLog.LOG_NET_client_timer_adj, 0)
   agci(LOGGING, mLog.LOG_NET_server_rx_stak, 0)


   agci(LOGGING, mLog.LOG_TMR_cpu, 0)
   agci(LOGGING, mLog.LOG_TMR_move_tot, 0)
   agci(LOGGING, mLog.LOG_TMR_move_all, 0)
   agci(LOGGING, mLog.LOG_TMR_move_enem, 0)
   agci(LOGGING, mLog.LOG_TMR_bmsg_add, 0)
   agci(LOGGING, mLog.LOG_TMR_bmsg_draw, 0)
   agci(LOGGING, mLog.LOG_TMR_scrn_overlay, 0)


   agci(LOGGING, mLog.LOG_TMR_draw_tot, 0)
   agci(LOGGING, mLog.LOG_TMR_draw_all, 0)
   agci(LOGGING, mLog.LOG_TMR_sdif, 0)
   agci(LOGGING, mLog.LOG_TMR_cdif, 0)
   agci(LOGGING, mLog.LOG_TMR_rwnd, 0)

   agci(LOGGING, mLog.LOG_TMR_client_timer_adj, 0)
   agci(LOGGING, mLog.LOG_TMR_client_ping, 0)



   agci(LOGGING, mLog.autosave_log_on_program_exit, 0)
   agci(LOGGING, mLog.autosave_log_on_game_exit, 0)
   agci(LOGGING, mLog.autosave_log_on_level_done, 0)

   agci(DEMO, mDemoMode.demo_mode_config_enable, 1)
   agci(DEMO, mLog.autosave_game_on_level_done, 0)
   agci(DEMO, mLog.autosave_game_on_game_exit, 0)
   agcf(DEMO, mDemoMode.demo_mode_overlay_opacity, 0.1)


   agci(BMSG, mBottomMessage.num_lines, 8)

   agcf(BMSG, mBottomMessage.io, 1.0)
   agcf(BMSG, mBottomMessage.fo, 0.1)
   agcf(BMSG, mBottomMessage.ihs, 0.5)
   agcf(BMSG, mBottomMessage.ivs, 0.5)
   agcf(BMSG, mBottomMessage.fhs, 0.1)
   agcf(BMSG, mBottomMessage.fvs, 0.1)

   agci(BMSG, mBottomMessage.disp_player, 0)
   agci(BMSG, mBottomMessage.disp_enemy, 0)
   agci(BMSG, mBottomMessage.disp_item, 0)
   agci(BMSG, mBottomMessage.disp_health, 0)





   // cpu
   agci(OVERLAY, mSettings.overlay_grid[0][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[0][1], 1);
   agci(OVERLAY, mSettings.overlay_grid[0][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[0][3], 0);

   // display
   agci(OVERLAY, mSettings.overlay_grid[1][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[1][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[1][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[1][3], 0);

   // draw profile
   agci(OVERLAY, mSettings.overlay_grid[2][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[2][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[2][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[2][3], 0);

   // debug grid
   agci(OVERLAY, mSettings.overlay_grid[3][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[3][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[3][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[3][3], 0);

   // sync graph
   agci(OVERLAY, mSettings.overlay_grid[4][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[4][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[4][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[4][3], 0);

   // sync adjust
   agci(OVERLAY, mSettings.overlay_grid[5][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[5][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[5][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[5][3], 0);

   // state freq adj
   agci(OVERLAY, mSettings.overlay_grid[6][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[6][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[6][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[6][3], 0);

   // bandwidth stats
   agci(OVERLAY, mSettings.overlay_grid[7][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[7][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[7][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[7][3], 0);

   // misc
   agci(OVERLAY, mSettings.overlay_grid[8][0], 0);
   agci(OVERLAY, mSettings.overlay_grid[8][1], 0);
   agci(OVERLAY, mSettings.overlay_grid[8][2], 0);
   agci(OVERLAY, mSettings.overlay_grid[8][3], 0);

   agci(OVERLAY, mSettings.number_of_debug_overlay_modes, 2);

   agci(LEVEL_EDITOR, mLoop.autosave_level_editor_state, 0);

   al_destroy_config(cfg);
   save();
}
