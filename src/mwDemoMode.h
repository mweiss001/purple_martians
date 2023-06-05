// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void);

   void initialize(void);
   void load_random_demo(void);
   void key_check(int p);




   ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
   int num_demo_filenames;
   int demo_played[100];

   int demo_mode_loaded;
   int demo_mode_on;



   int pass;
   int prev_lev;

   int restore_level;
   int restore_mode;


   int demo_mode_countdown;
   int demo_mode_countdown_val;
   int demo_mode_countdown_reset;
   int demo_mode_enabled;
   int demo_mode_last_frame;
   int demo_mode_config_enable;
   float demo_mode_overlay_opacity;

};
extern mwDemoMode mDemoMode;


