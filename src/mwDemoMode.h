// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode();

   void initialize();
   int load_demo_file_array();
   int load_random_demo();
   void play_all_demos_and_save_stats(int x, int y);
   int key_check();
   void run_loop(int ti, int tf, int restore_level);
   void run_single_from_menu();
   void run_single_from_settings();
   void run_single_from_gate(int lev);
   void run_single_from_cmd_fn(const char* fn);
   void run_single_from_cmd_prompt();
   void run_continuous_random();
   void play_demo_for_stats();

   void continuous_random_next_level();


   static void seek_to_frame(int frame, int draw);


   ALLEGRO_FS_ENTRY *demo_FS_filenames[100]{};
   int num_demo_filenames{};
   int demo_played[100]{};
   int files_for_random_loaded{};
   int pass{};
   int prev_index{};
   int last_frame{};

   int play_mode{};



   void frame_advance();
   bool check_level_done();
   void common_exit();
   void check_valid_active_local_player();
   void set_active_local_player(int p);


   int countdown_val{};
   int countdown_reset{2400};
   int autoplay_enabled{};
   int config_autoplay_enabled{};

   float overlay_opacity{};

   int demo_debug_complete_level_on_gate_with_fire{};
   int demo_debug_running_demo_saves_level_data{};
};
extern mwDemoMode mDemoMode;
