// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void);

   void initialize(void);
   int load_random_demo(void);
   void key_check(int p);


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

   int countdown; // countdown
   int countdown_val;
   int countdown_reset;

   int autoplay_enabled;
   int config_autoplay_enabled;



   float overlay_opacity;

   int demo_debug_complete_level_on_gate_with_fire;
   int demo_debug_running_demo_saves_level_data;
   int demo_debug_convert_playback_to_record_with_fire;
   int demo_debug_super_fast_mode_F2;




};
extern mwDemoMode mDemoMode;


