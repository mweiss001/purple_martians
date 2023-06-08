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
   int prev_lev;
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

};
extern mwDemoMode mDemoMode;


