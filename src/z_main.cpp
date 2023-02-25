// z_main.cpp

#include "pm.h"
#include "z_main.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_args.h"
#include "mwConfig.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "z_loop.h"
#include "mwLevel.h"
#include "mwGameMovesArray.h"


void final_wrapup(void)
{
   mwD.save_display_window_position();

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

#include "mwPlayers.h"
void fast_exit(int why)
{
   if (why != 0) mPlayer.loc[mPlayer.active_local_player].quit_reason = why; // don't overwrite if not zero
   if (mLog.autosave_log_on_program_exit) mLog.save_log_file();
   if (mLog.autosave_game_on_game_exit) mwGMA.blind_save_game_moves(3);
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

#include "mwProgramState.h"
void set_and_get_versions(void)
{
   // set version name
   sprintf(mwPS.pm_version_string, PM_VERSION);
   printf("\nPurple Martians Version %s\n", mwPS.pm_version_string);

   // get allegro version
   uint32_t version = al_get_allegro_version();
   int major = version >> 24;
   int minor = (version >> 16) & 255;
   int revision = (version >> 8) & 255;
   int release = version & 255;

   sprintf(mwPS.al_version_string, "Allegro Version: %d.%d.%d.%d", major, minor, revision, release);
   printf("%s\n", mwPS.al_version_string);


   // get hostname
   char msg[256];
   sprintf(msg, "hostname");
   FILE *fp;
   fp = popen(msg,"r");
   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      mwPS.local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   mwPS.local_hostname[loop] = 0;
   fclose(fp);

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
   mConfig.load();
   show_system_id();
   get_desktop_resolution();

   srand(time(NULL));

   // --- event queue ----------------
   mwEQ.event_queue = al_create_event_queue();
   if(!mwEQ.event_queue)
   {
      mI.m_err("Failed to create event queue.\n");
      return 0;
   }

   // --- display --------------------
   if (!mwD.init_display())
   {
      mI.m_err("Failed to initialize display.\n");
      return 0;
   }

   al_register_event_source(mwEQ.event_queue, al_get_display_event_source(display));

   // --- allegro add ons ------------
   if(!al_init_native_dialog_addon())
   {
      mI.m_err("Failed to initialize native dialog addon.\n");
      return 0;
   }

   if(!al_init_primitives_addon())
   {
      mI.m_err("Failed to initialize primitives addon.\n");
      return 0;
   }

   if(!al_init_image_addon())
   {
      mI.m_err("Failed to initialize image addon.\n");
      return 0;
   }

   if(!al_init_font_addon())
   {
      mI.m_err("Failed to initialize font addon.\n");
      return 0;
   }

   if(!al_init_ttf_addon())
   {
      mI.m_err("Failed to initialize ttf addon.\n");
      return 0;
   }
   mF.load_fonts();

   // --- keyboard -------------------
   if (!al_install_keyboard())
   {
      mI.m_err("Failed to install keyboard.\n");
      return 0;
   }
   //else printf("installed keyboard\n");
   al_register_event_source(mwEQ.event_queue, al_get_keyboard_event_source());


   // --- mouse ----------------------
   if (!al_install_mouse())
   {
      mI.m_err("Failed to install mouse.\n");
      return 0;
   }
   //else printf("installed mouse\n");
   al_register_event_source(mwEQ.event_queue, al_get_mouse_event_source());
   al_hide_mouse_cursor(display);


   // --- joystick -------------------
   if (!al_install_joystick())
   {
      mI.m_err("Failed to install joystick.\n");
      return 0;
   }
   //else printf("installed joystick\n");
   int nj = al_get_num_joysticks();
   //printf("found %d joystick(s)\n", nj);
   if (nj > 0)
   {
      mI.joy0 = al_get_joystick(0);
      //printf("j0 - %s\n", al_get_joystick_name(joy0));

      if (nj > 1)
      {
         mI.joy1 = al_get_joystick(1);
         //printf("j1 - %s\n", al_get_joystick_name(joy1));
      }
      al_register_event_source(mwEQ.event_queue, al_get_joystick_event_source());
   }

   mwB.load_tiles();
   al_set_display_icon(display, mwB.tile[401]);

   mwEQ.create_timers();

   mSound.load_sound();

   // init players
   for (int p=1; p<NUM_PLAYERS; p++) mPlayer.syn[p].color = 0; // all but player[0] which we got from config file
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1;

   mwDM.demo_mode_enabled = mwDM.demo_mode_config_enable; // set only at startup from cofing file

   mLevel.set_start_level();

   return 1;
}



int main(int argument_count, char **argument_array)
{
   proc_command_line_args1(argument_count, argument_array); // these args get processed before initial setup is called
   if (initial_setup())
   {
      proc_command_line_args2(argument_count, argument_array); // these args get processed after initial setup is called
      mwPS.program_state = 1;
      main_loop();
   }
   if (mLog.autosave_log_on_program_exit) mLog.save_log_file();
   final_wrapup();
   exit(0);
}
