// mwConfig.cpp

#include "pm.h"
#include "mwConfig.h"
#include "mwLog.h"
#include "mwSound.h"
#include "mwSettings.h"
#include "mwPlayer.h"
#include "mwNetgame.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwShot.h"
#include "mwInput.h"
#include "mwVisualLevel.h"
#include "mwMain.h"
#include "mwScreen.h"
#include "mwGameMoves.h"


mwConfig mConfig;

void mwConfig::save_config(void)
{
   //printf("save cfg\n");

   mSettings.save_settings();

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
      asci(SCREEN, mDisplay.fullscreen_monitor_num)
      asci(SCREEN, mLogo.show_splash_screen)

      asci(SCREEN, mDisplay.saved_display_transform_double)
      asci(SCREEN, mDisplay.display_transform_double_max)

      asci(GAME, mPlayer.syn[0].color)
      asci(GAME, mLevel.start_level)

      asci(GAME, mPlayer.syn[0].overworld_last_touched_gate)


      asci(GAME, mMain.classic_mode)

      asci(GAME, mScreen.transition_num_steps)
      asci(GAME, mScreen.transition_delay)


      asci(GAME, mScreen.viewport_mode)
      asci(GAME, mScreen.viewport_show_hyst)
      ascf(GAME, mScreen.viewport_x_div)
      ascf(GAME, mScreen.viewport_y_div)

      asci(GAME, mScreen.viewport_look_player_motion)
      asci(GAME, mScreen.viewport_look_up_down)
      asci(GAME, mScreen.viewport_look_player_facing_left_right)
      asci(GAME, mScreen.viewport_look_rocket)

      asci(GAME, mLoop.speed_control_lock)
      asci(GAME, mLoop.frame_speed)


      asci(GAME, mLoop.reset_frame_speed_at_program_start)



      asci(GAME, mVisualLevel.max_level_num)
      asci(GAME, mVisualLevel.level_icon_size)


      asci(GAMECONTROLS, mPlayer.loc[0].up_key)
      asci(GAMECONTROLS, mPlayer.loc[0].down_key)
      asci(GAMECONTROLS, mPlayer.loc[0].left_key)
      asci(GAMECONTROLS, mPlayer.loc[0].right_key)
      asci(GAMECONTROLS, mPlayer.loc[0].jump_key)
      asci(GAMECONTROLS, mPlayer.loc[0].fire_key)
      asci(GAMECONTROLS, mPlayer.loc[0].menu_key)

      asci(FUNCTIONKEYS, mInput.function_key_fullscreen)
      asci(FUNCTIONKEYS, mInput.function_key_text_double)
      asci(FUNCTIONKEYS, mInput.function_key_debug_overlay)
      asci(FUNCTIONKEYS, mInput.function_key_zoom_in)
      asci(FUNCTIONKEYS, mInput.function_key_zoom_out)
      asci(FUNCTIONKEYS, mInput.function_key_printscreen)
      asci(FUNCTIONKEYS, mInput.function_key_fakekey)
      asci(FUNCTIONKEYS, mInput.function_key_force_save)
      asci(FUNCTIONKEYS, mInput.function_key_init_background)
      asci(FUNCTIONKEYS, mInput.function_key_speed_dec)
      asci(FUNCTIONKEYS, mInput.function_key_speed_inc)

      asci(SOUND, mSound.sound_on)
      asci(SOUND, mSound.se_scaler)
      asci(SOUND, mSound.st_scaler)

      al_set_config_value(cfg, "NETWORK", "server_IP", mNetgame.server_address);

      asci(NETWORK, mNetgame.server_port)


      asci(NETWORK, mPlayer.syn[0].player_vs_player_shots)
      asci(NETWORK, mPlayer.syn[0].player_vs_player_shot_damage)
      asci(NETWORK, mPlayer.syn[0].player_vs_self_shots)

      ascf(NETWORK, mNetgame.client_chase_offset)
      ascf(NETWORK, mNetgame.client_chase_offset_auto_offset)
      asci(NETWORK, mNetgame.client_chase_offset_mode)

      asci(LOGGING, mLog.autosave_log_on_level_done)
      asci(LOGGING, mLog.autosave_log_on_level_quit)
      asci(LOGGING, mLog.autosave_log_on_program_exit)


      asci(DEMO, mDemoMode.config_autoplay_enabled)

      asci(DEMO, mGameMoves.autosave_game_on_level_done)
      asci(DEMO, mGameMoves.autosave_game_on_level_quit)
      asci(DEMO, mGameMoves.autosave_game_on_program_exit)
      asci(DEMO, mGameMoves.server_send_gm_to_clients)



      ascf(DEMO, mDemoMode.overlay_opacity)


      asci(DEMO, mDemoMode.demo_debug_complete_level_on_gate_with_fire)
      asci(DEMO, mDemoMode.demo_debug_running_demo_saves_level_data)

      asci(BMSG, mBottomMessage.bottom_msg_on)
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
      asci(BMSG, mBottomMessage.disp_player_text_long)

      asci(SETTINGS, mSettings.current_page)
      asci(SETTINGS, mSettings.show_advanced)
      asci(SETTINGS, mSettings.show_debug)

      asci(OVERLAY, mSettings.number_of_debug_overlay_modes);

      asci(LEVEL_EDITOR, mLoop.autosave_level_editor_state);

   }
   al_save_config_file("pm.cfg", cfg);
   al_destroy_config(cfg);


}

void mwConfig::load_config(void)
{
   // this reads values and validates
   // after that, it immediately calls save in case values were not found and defaults were used

   mSettings.load_settings();

   // printf("load cfg\n");

   char msg[1024];
   ALLEGRO_CONFIG * cfg = NULL;
   cfg = al_load_config_file("pm.cfg");
   if (!cfg)
   {
      sprintf(msg, "error loading pm.cfg -- recreating");
      printf("%s\n", msg);
      cfg = al_create_config();
      al_save_config_file("pm.cfg", cfg);
   }

   const char* val;

   agci(SCREEN, mDisplay.disp_x_wind, 200)
   agci(SCREEN, mDisplay.disp_y_wind, 40)
   agci(SCREEN, mDisplay.disp_w_wind, 1280)
   agci(SCREEN, mDisplay.disp_h_wind, 1024)
   agci(SCREEN, mDisplay.fullscreen, 1)
   agci(SCREEN, mDisplay.fullscreen_monitor_num, 0)
   agci(SCREEN, mLogo.show_splash_screen, 1)
   agci(SCREEN, mDisplay.display_transform_double_max, 3)
   agci(SCREEN, mDisplay.saved_display_transform_double, 0)

   agcf(SCREEN, mDisplay.scale_factor, 1.0)
   //mDisplay.set_scale_factor(1.0, 1); // reset to 1.0 with every program start
   mDisplay.set_scale_factor(mDisplay.scale_factor, 1);

   agci(SCREEN, mLogo.show_splash_screen, 1)
   if (!mLogo.show_splash_screen) mLogo.splash_screen_done = 1;

   agci(GAME, mLevel.start_level, 1)
   agci(GAME, mPlayer.syn[0].overworld_last_touched_gate, 0)


   agci(GAME, mMain.classic_mode, 0)

   agci(GAME, mScreen.transition_num_steps, 80)
   agci(GAME, mScreen.transition_delay, 8)

   agci(GAME, mPlayer.syn[0].color, 8)

   agci(GAME, mScreen.viewport_mode, 1)
   agci(GAME, mScreen.viewport_show_hyst, 0)
   agcf(GAME, mScreen.viewport_x_div, 0.33)
   agcf(GAME, mScreen.viewport_y_div, 0.33)

   agci(GAME, mScreen.viewport_look_player_motion, 0)
   agci(GAME, mScreen.viewport_look_up_down, 1)
   agci(GAME, mScreen.viewport_look_player_facing_left_right, 0)
   agci(GAME, mScreen.viewport_look_rocket, 1)

   agci(GAME, mLoop.speed_control_lock, 1)
   agci(GAME, mLoop.reset_frame_speed_at_program_start, 1)

   if (mLoop.reset_frame_speed_at_program_start) mLoop.frame_speed = 40;
   else
   {
      agci(GAME, mLoop.frame_speed, 40)
   }

   agci(GAME, mVisualLevel.max_level_num, 100)
   agci(GAME, mVisualLevel.level_icon_size, 40)


   agci(GAMECONTROLS, mPlayer.loc[0].up_key,    ALLEGRO_KEY_UP)
   agci(GAMECONTROLS, mPlayer.loc[0].down_key,  ALLEGRO_KEY_DOWN)
   agci(GAMECONTROLS, mPlayer.loc[0].left_key,  ALLEGRO_KEY_LEFT)
   agci(GAMECONTROLS, mPlayer.loc[0].right_key, ALLEGRO_KEY_RIGHT)
   agci(GAMECONTROLS, mPlayer.loc[0].jump_key,  ALLEGRO_KEY_SPACE)
   agci(GAMECONTROLS, mPlayer.loc[0].fire_key,  ALLEGRO_KEY_ALT)
   agci(GAMECONTROLS, mPlayer.loc[0].menu_key,  ALLEGRO_KEY_ESCAPE)

   agci(FUNCTIONKEYS, mInput.function_key_zoom_out,        ALLEGRO_KEY_F5)
   agci(FUNCTIONKEYS, mInput.function_key_zoom_in,         ALLEGRO_KEY_F6)
   agci(FUNCTIONKEYS, mInput.function_key_debug_overlay,   ALLEGRO_KEY_F10)
   agci(FUNCTIONKEYS, mInput.function_key_text_double,     ALLEGRO_KEY_F11)
   agci(FUNCTIONKEYS, mInput.function_key_fullscreen,      ALLEGRO_KEY_F12)
   agci(FUNCTIONKEYS, mInput.function_key_printscreen,     0)
   agci(FUNCTIONKEYS, mInput.function_key_fakekey,         0)
   agci(FUNCTIONKEYS, mInput.function_key_force_save,      0)
   agci(FUNCTIONKEYS, mInput.function_key_init_background, 0)
   agci(FUNCTIONKEYS, mInput.function_key_speed_dec,       ALLEGRO_KEY_F7)
   agci(FUNCTIONKEYS, mInput.function_key_speed_inc,       ALLEGRO_KEY_F8)

   agci(SOUND, mSound.sound_on, 1)
   agci(SOUND, mSound.se_scaler, 3)
   agci(SOUND, mSound.st_scaler, 1)

   val = al_get_config_value(cfg, "NETWORK", "server_IP");
   if (!val) sprintf(mNetgame.server_address, "purplemartians.org");
   else sprintf(mNetgame.server_address, "%s", val);

   agci(NETWORK, mNetgame.server_port, 24785)

   agci(NETWORK, mPlayer.syn[0].player_vs_player_shots, 1)
   agci(NETWORK, mPlayer.syn[0].player_vs_player_shot_damage, 5)
   agci(NETWORK, mPlayer.syn[0].player_vs_self_shots, 1)

   agcf(NETWORK, mNetgame.client_chase_offset, -0.02)
   agcf(NETWORK, mNetgame.client_chase_offset_auto_offset, -0.005)
   agci(NETWORK, mNetgame.client_chase_offset_mode, 1)

   agci(LOGGING, mLog.autosave_log_on_level_done, 0)
   agci(LOGGING, mLog.autosave_log_on_level_quit, 0)
   agci(LOGGING, mLog.autosave_log_on_program_exit, 0)

   agci(DEMO, mDemoMode.config_autoplay_enabled, 1)

   agci(DEMO, mGameMoves.autosave_game_on_level_done, 0)
   agci(DEMO, mGameMoves.autosave_game_on_level_quit, 0)
   agci(DEMO, mGameMoves.autosave_game_on_program_exit, 0)
   agci(DEMO, mGameMoves.server_send_gm_to_clients, 0)


   agcf(DEMO, mDemoMode.overlay_opacity, 0)

   agci(DEMO, mDemoMode.demo_debug_complete_level_on_gate_with_fire, 0)
   agci(DEMO, mDemoMode.demo_debug_running_demo_saves_level_data, 0)

   agci(BMSG, mBottomMessage.bottom_msg_on, 1)
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
   agci(BMSG, mBottomMessage.disp_player_text_long, 1)

   agci(SETTINGS, mSettings.current_page, 0)
   agci(SETTINGS, mSettings.show_advanced, 0)
   agci(SETTINGS, mSettings.show_debug, 0)

   agci(OVERLAY, mSettings.number_of_debug_overlay_modes, 2);

   agci(LEVEL_EDITOR, mLoop.autosave_level_editor_state, 0);

   al_destroy_config(cfg);

   save_config(); // to save default values

}
