// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void) { demo_mode_countdown_reset = 2400; }

   void run(void);

   ALLEGRO_FS_ENTRY *demo_FS_filenames[100];
   int num_demo_filenames;
   int demo_played[100];

   int demo_mode_loaded;
   int demo_mode_on;
   int demo_mode_countdown;
   int demo_mode_countdown_val;
   int demo_mode_countdown_reset;
   int demo_mode_enabled;
   int demo_mode_last_frame;
   int demo_mode_config_enable;
   float demo_mode_overlay_opacity;
};
extern mwDemoMode mDemoMode;


