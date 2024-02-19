// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void);

   void initialize(void);
   int load_demo_file_array();
   int load_random_demo(void);
   void play_all_demos_and_save_stats(int x, int y);
   int key_check(void);
   void run_loop(int ti, int tf, int restore_level);
   void run_single_from_menu(void);
   void run_single_from_settings(void);
   void run_single_from_gate(int lev);
   void run_single_from_cmd_fn(const char* fn);
   void run_single_from_cmd_prompt(void);
   void run_continuous_random(void);
   void play_demo_for_stats(void);

   ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
   int num_demo_filenames;
   int demo_played[100];
   int files_for_random_loaded;
   int pass;
   int prev_index;
   int last_frame;

   int mode;
   int restore_mode;
   int restore_level;

   int countdown_val;
   int countdown_reset = 2400;

   int autoplay_enabled;
   int config_autoplay_enabled;

   float overlay_opacity;

   int demo_debug_complete_level_on_gate_with_fire;
   int demo_debug_running_demo_saves_level_data;
};
extern mwDemoMode mDemoMode;
