// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void);

   void initialize(void);
   int load_random_demo(void);
   void key_check(int p);

   void demo_record(void);

   int load_demo_file_array();




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
   int demo_debug_super_fast_mode_F2;


   int player_sections[20][20] = {0};

   void set_player_section(int i, int p, int c, int f1, int f2, int y, int end_type);

   void fill_player_sections(void);

   void get_more_player_section_info(void);

   void find_ack_moves(int i);


   int draw_section_details(int i, int x, int y, int single_line);

   void draw_timeline(void);


   void play_all_demos_and_save_stats(int x, int y);


   void seek_to_frame(int frame, int draw);

   char current_loaded_demo_file[200];

   int record_player_number = 0;
   int record_player_color = 8;
   int play = 0;
   int level_done_frame = -1;

   int current_section = 0;


   void find_level_done(void);

   int load_demo_record(void);
   void start_record(void);


   void demo_remove(int i);
   void demo_swap(int i, int j);
   void demo_sort(void);



   // position and size of the scrollbar track list rectangle (timeline)
   int sb_x1;
   int sb_x2;
   int sb_w;

   int sb_y2;
   int sb_h;
   int sb_y1;

   int sb_spacing = 4;
   int sb_size = 20;










};
extern mwDemoMode mDemoMode;


