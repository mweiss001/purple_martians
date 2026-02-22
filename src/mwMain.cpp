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
#include "mwMiscFnx.h"
#include "mwSql.h"

mwMain mMain;

void mwMain::final_wrapup(void)
{
   if (!headless_server)
   {
      mDisplay.refresh_window_position_and_size();
      al_destroy_audio_stream(mSound.pm_theme_stream);
      al_inhibit_screensaver(false);
   }
   mConfig.save_config();
   al_uninstall_system();
}



void mwMain::fast_exit(int type)
{
   if (type == 2)  printf("\n\nReceived SIGINT - terminating...\n\n");
   if (type == 6)  printf("\n\nReceived SIGABRT - terminating...\n\n");
   if (type == 15) printf("\n\nReceived SIGTERM - terminating...\n\n");
   if (mLog.autosave_log_on_program_exit) mLog.flush_logs();
   final_wrapup();
   exit(type);
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

   // allegro version
   uint32_t version = al_get_allegro_version();
   int major = version >> 24;
   int minor = (version >> 16) & 255;
   int revision = (version >> 8) & 255;
   int release = version & 255;
   sprintf(mLoop.al_version_string, "Allegro Version: %d.%d.%d.%d", major, minor, revision, release);
   printf("%s\n", mLoop.al_version_string);

   std::cout << "GCC Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << " - ";

   long standard = __cplusplus;
   // Mapping the macro values to standard names
   if (standard == 199711L) std::cout << "C++98/C++03" << std::endl;
   else if (standard == 201103L) std::cout << "C++11" << std::endl;
   else if (standard == 201402L) std::cout << "C++14" << std::endl;
   else if (standard == 201703L) std::cout << "C++17" << std::endl;
   else if (standard == 202002L) std::cout << "C++20" << std::endl;
   else if (standard == 202302L) std::cout << "C++23" << std::endl;
   else std::cout << "Unknown or experimental C++ version: " << standard << std::endl;


   // get hostname
   FILE *fp = popen("hostname", "r");
   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      mLoop.local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   mLoop.local_hostname[loop] = 0;
   pclose(fp);
//   printf("Hostname:%s\n", mLoop.local_hostname);

//   std::string t2 = std::format("{:%Y%m%d %H%M%S}", std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()));
//   std::cout << "GMT Time: " << t2 << std::endl;



}

int mwMain::initial_setup(void)
{
   //al_set_config_value(al_get_system_config(), "trace", "level", "debug");

   if (!al_init())
   {
      printf("Failed to initialize allegro.\n");
      return 0;
   }

   set_exe_path();
   set_and_get_versions();

   al_make_directory("data"); // create if not already created

   mSql.init();

   mConfig.load_config();

   show_system_id();
   srand(time(NULL));

   // --- event queue ----------------
   mEventQueue.event_queue = al_create_event_queue();
   if(!mEventQueue.event_queue)
   {
      printf("Failed to create event queue.\n");
      return 0;
   }

   // --- timers -------------
   mEventQueue.create_timers();

   // --- display --------------------
   if (headless_server)
   {
      mDisplay.no_display = 1;
      printf("Starting Headless Server.\n");
      mSound.sound_on = 0;
      mBitmap.load_sprit(); // get animation sequences and shape attributes
   }
   else if (!mDisplay.init_display())
   {
      printf("Failed to initialize display.\n");
      return 0;
   }



   mBitmap.create_bitmaps();

   if(!al_init_image_addon())
   {
      mInput.m_err("Failed to initialize image addon.\n");
      return 0;
   }
   if(!al_init_primitives_addon())
   {
      mInput.m_err("Failed to initialize primitives addon.\n");
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
   mBitmap.load_tiles();


   // --- things not to load for headless server ---------------
   if (!mDisplay.no_display)
   {
      al_inhibit_screensaver(true);

      al_register_event_source(mEventQueue.event_queue, al_get_display_event_source(mDisplay.display));

      // --- allegro add ons ------------
      if(!al_init_native_dialog_addon())
      {
         mInput.m_err("Failed to initialize native dialog addon.\n");
         return 0;
      }

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

      al_set_display_icon(mDisplay.display, mBitmap.tile[401]);

      mSound.load_sound();
   }


   // init players
   mPlayer.set_default_player_colors();
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1;

   mDemoMode.autoplay_enabled = mDemoMode.config_autoplay_enabled; // set only at startup from config file

   mLevel.setup_data();

   if (classic_mode) mLevel.set_start_level();
   else mLevel.set_start_level(1);

   return 1;
}

int mwMain::pm_main(int argument_count, char **argument_array)
{
   proc_command_line_args1(argument_count, argument_array); // these args get processed before initial setup is called
   if (initial_setup())
   {
      proc_command_line_args2(argument_count, argument_array); // these args get processed after initial setup is called

      if (mLoop.state[0] == PM_PROGRAM_STATE_QUIT) // nothing set by command line args
      {
         mLoop.state[1] = mLoop.state[0] = PM_PROGRAM_STATE_MENU; // set up for menu
      }
      mLoop.main_loop();
   }
   if (mLog.autosave_log_on_program_exit) mLog.flush_logs();

   final_wrapup();
   exit(0);
}
