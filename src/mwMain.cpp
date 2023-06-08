// mwMain.cpp

#include "pm.h"
#include "mwMain.h"
#include "mwPlayer.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwConfig.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwGameMoves.h"

mwMain mMain;

void mwMain::final_wrapup(void)
{
   mDisplay.save_display_window_position();

   al_destroy_audio_stream(mSound.pm_theme_stream);
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

   printf("al_unregister_event_source(event_queue, al_get_display_event_source(mDisplay.display))\n");
   al_unregister_event_source(event_queue, al_get_display_event_source(mDisplay.display));

   printf("al_destroy_display()\n");
   al_destroy_display(mDisplay.display);

   printf("al_destroy_event_queue(event_queue)\n");
   al_destroy_event_queue(event_queue);

  // printf("\nBefore al_uninstall_system()\n");
   al_uninstall_system();
  // printf("\nAfter al_uninstall_system()\n");

  */
   al_uninstall_system();
}


void mwMain::fast_exit(int why)
{
   if (why != 0) mPlayer.loc[mPlayer.active_local_player].quit_reason = why; // don't overwrite if not zero
   if (mLog.autosave_log_on_program_exit) mLog.save_log_file();
   if (mLog.autosave_game_on_game_exit) mGameMoves.blind_save_game_moves(3);
   final_wrapup();
   exit(0);
}

void mwMain::show_system_id(void)
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

void mwMain::set_exe_path(void)
{
   ALLEGRO_PATH *ep = al_get_standard_path(ALLEGRO_EXENAME_PATH);
   al_set_path_filename(ep, NULL);
   if (!al_change_directory(al_path_cstr(ep, ALLEGRO_NATIVE_PATH_SEP))) printf("Error Changing Current Directory!\n");
   //printf("Current Directory: %s\n", al_get_current_directory());
   al_destroy_path(ep);
}

void mwMain::set_and_get_versions(void)
{
   // set version name
   sprintf(mLoop.pm_version_string, PM_VERSION);
   printf("\nPurple Martians Version %s\n", mLoop.pm_version_string);

   // get allegro version
   uint32_t version = al_get_allegro_version();
   int major = version >> 24;
   int minor = (version >> 16) & 255;
   int revision = (version >> 8) & 255;
   int release = version & 255;

   sprintf(mLoop.al_version_string, "Allegro Version: %d.%d.%d.%d", major, minor, revision, release);
   printf("%s\n", mLoop.al_version_string);


   // get hostname
   char msg[256];
   sprintf(msg, "hostname");
   FILE *fp;
   fp = popen(msg,"r");
   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      mLoop.local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   mLoop.local_hostname[loop] = 0;
   fclose(fp);

}

void mwMain::get_desktop_resolution()
{
   ALLEGRO_MONITOR_INFO aminfo;
   al_get_monitor_info(0, &aminfo);
   mDisplay.desktop_width  = aminfo.x2 - aminfo.x1;
   mDisplay.desktop_height = aminfo.y2 - aminfo.y1;
   printf("Desktop Resolution: %dx%d\n", mDisplay.desktop_width, mDisplay.desktop_height);

   mDisplay.disp_x_full = 0; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
   mDisplay.disp_y_full = 0;
   mDisplay.disp_w_full = mDisplay.desktop_width;
   mDisplay.disp_h_full = mDisplay.desktop_height;
}

int mwMain::initial_setup(void)
{
   //al_set_config_value(al_get_system_config(), "trace", "level", "debug");

   al_init();
   set_exe_path();
   set_and_get_versions();
   mConfig.load();
   show_system_id();
   get_desktop_resolution();

   srand(time(NULL));

   // --- event queue ----------------
   mEventQueue.event_queue = al_create_event_queue();
   if(!mEventQueue.event_queue)
   {
      mInput.m_err("Failed to create event queue.\n");
      return 0;
   }

   // --- display --------------------
   if (!mDisplay.init_display())
   {
      mInput.m_err("Failed to initialize display.\n");
      return 0;
   }

   al_register_event_source(mEventQueue.event_queue, al_get_display_event_source(mDisplay.display));

   // --- allegro add ons ------------
   if(!al_init_native_dialog_addon())
   {
      mInput.m_err("Failed to initialize native dialog addon.\n");
      return 0;
   }

   if(!al_init_primitives_addon())
   {
      mInput.m_err("Failed to initialize primitives addon.\n");
      return 0;
   }

   if(!al_init_image_addon())
   {
      mInput.m_err("Failed to initialize image addon.\n");
      return 0;
   }

   if(!al_init_font_addon())
   {
      mInput.m_err("Failed to initialize font addon.\n");
      return 0;
   }

   if(!al_init_ttf_addon())
   {
      mInput.m_err("Failed to initialize ttf addon.\n");
      return 0;
   }
   mFont.load_fonts();

   // --- keyboard -------------------
   if (!al_install_keyboard())
   {
      mInput.m_err("Failed to install keyboard.\n");
      return 0;
   }
   //else printf("installed keyboard\n");
   al_register_event_source(mEventQueue.event_queue, al_get_keyboard_event_source());


   // --- mouse ----------------------
   if (!al_install_mouse())
   {
      mInput.m_err("Failed to install mouse.\n");
      return 0;
   }
   //else printf("installed mouse\n");
   al_register_event_source(mEventQueue.event_queue, al_get_mouse_event_source());
   al_hide_mouse_cursor(mDisplay.display);


   // --- joystick -------------------
   if (!al_install_joystick())
   {
      mInput.m_err("Failed to install joystick.\n");
      return 0;
   }
   //else printf("installed joystick\n");
   int nj = al_get_num_joysticks();
   //printf("found %d joystick(s)\n", nj);
   if (nj > 0)
   {
      mInput.joy0 = al_get_joystick(0);
      //printf("j0 - %s\n", al_get_joystick_name(joy0));

      if (nj > 1)
      {
         mInput.joy1 = al_get_joystick(1);
         //printf("j1 - %s\n", al_get_joystick_name(joy1));
      }
      al_register_event_source(mEventQueue.event_queue, al_get_joystick_event_source());
   }

   mBitmap.load_tiles();
   al_set_display_icon(mDisplay.display, mBitmap.tile[401]);

   mEventQueue.create_timers();

   mSound.load_sound();

   // init players
   for (int p=1; p<NUM_PLAYERS; p++) mPlayer.syn[p].color = 0; // all but player[0] which we got from config file


   mPlayer.syn[1].color = 10;
   mPlayer.syn[2].color = 11;
   mPlayer.syn[3].color = 12;
   mPlayer.syn[4].color = 13;
   mPlayer.syn[5].color = 14;
   mPlayer.syn[6].color = 15;
   mPlayer.syn[7].color = 9;


   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1;

   mDemoMode.autoplay_enabled = mDemoMode. config_autoplay_enabled; // set only at startup from config file


   if (classic_mode) mLevel.set_start_level();
   else mLevel.set_start_level(1);


   mLevel.setup_data();



   return 1;
}



int mwMain::pm_main(int argument_count, char **argument_array)
{
   proc_command_line_args1(argument_count, argument_array); // these args get processed before initial setup is called
   if (initial_setup())
   {
      proc_command_line_args2(argument_count, argument_array); // these args get processed after initial setup is called
      if (mLoop.state[0] == 0) // nothing set by command line args
      {
         mLoop.state[1] = mLoop.state[0] = 1; // set up for menu
      }


      mLoop.main_loop();
   }
   if (mLog.autosave_log_on_program_exit) mLog.save_log_file();
   mConfig.save();
   final_wrapup();
   exit(0);
}
