// mwDemoMode.h

class mwDemoMode
{
   public:
   mwDemoMode(void);

   void initialize(void);
   int load_demo_file_array();
   int load_random_demo(void);
   void play_all_demos_and_save_stats(int x, int y);
   void gdt(void);
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


   char * gettf(int frame, char* ft);

   int lnk_arr[10000] = {0};
   int lnk_entry_pos = 0;

   int player_sections[20][20] = {0};
   char current_loaded_demo_file[256];

   int draw_overlay(void);

   void demo_record(void);
   int load_demo_record(void);
   void refresh(void);
   void reload(void);

   void seek_to_frame(int frame, int draw);

   void start_record(void);

   void set_active_section(int i);


   void mark_timeline_position(int frame, int x1, int y1, int sb_y2, int col);
   void set_player_section(int i, int p, int c, int f1, int f2, int y, int end_type);
   void fill_player_sections(void);
   void get_more_player_section_info(void);
   void find_ack_moves(int i);
   void find_level_done(void);


   void draw_gm_txt_lines(int x, int y);

   void proc_gm_list_menu(int gi);

   void edit_gm(int gi);


   void load_lnk_arr(void);



   void proc_cpu_time(double frame_start_timestamp);

   int draw_section_details(int i, int x, int y);
   void draw_section_details_one_line(int i, int x, int y);

   int draw_record_settings(int x, int y);
   void draw_timeline(void);

   int show_play_record_buttons(int x, int y);

   int play = 0;
   int record = 0;

   int record_player_number = 0;
   int record_player_color = 8;

   int record_punch_in_armed = 0;

   int level_done_frame = 0;
   int level_done_player = -1;

   int current_frame_num = 0;
   int current_section = 0;

   // position and size of the scrollbar track list rectangle (timeline)
   int sb_x1;
   int sb_x2;
   int sb_w;

   int sb_y2;
   int sb_h;
   int sb_y1;

   int sb_spacing = 4;
   int sb_size = 20;
   int sb_text = 1;


//   int sb_spacing = 1;
//   int sb_size = 6;
//   int sb_text = 0;


   int tm_frame = 1;
   int sh_cpu_graph = 0;
   int sh_level_done = 1;
   int sh_section_details = 1;

   int sh_gamemoves_list = 1;
   int gm_list_all = 0;
   int gm_list_lines = 20;

   int sh_player_grid = 0;
   int sh_rec_controls = 1;
   int sh_timeline = 1;


   int sh_overlay_in_play;
   int sh_overlay_in_rec;



};
extern mwDemoMode mDemoMode;


