// mwDemoRecord.h

class mwDemoRecord
{
   public:

   int lnk_arr[10000] = {0};
   int lnk_entry_pos = 0;

   int player_sections[20][20] = {0};
   char current_loaded_demo_file[256];

   int fhs[10000][4] = {0};
   int fhs_indx = 0;


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

   int tm_frame = 1;
   int sh_cpu_graph = 0;
   int sh_level_done = 1;
   int sh_section_details = 1;

   int sh_gamemoves_list = 1;
   int gm_list_all = 0;
   int gm_list_lines = 40;

   int gm_list_mono = 0;


   int sh_range_tools = 1;
   int rt_start_frame = 0;
   int rt_end_frame = 0;
   int rt_all_players = 0;


   int sh_player_grid = 0;
   int sh_rec_controls = 0;
   int sh_timeline = 1;

   int timeline_size = 4;

   int sh_overlay_in_play = 0;
   int sh_overlay_in_rec = 1;

   void gdt(void);
   char * gettf(int frame, char* ft);
   void seek_to_frame(int frame, int draw);

   int draw_overlay(void);

   void demo_record(void);
   int load_demo_record(void);
   void refresh(void);
   void reload(void);

   void start_record(void);
   void stop_transport(void);

   void set_active_section(int i);

   void range_tools_set_fire_moves(int f, int clear_set);
   void range_tools_do(int action);
   void proc_range_tools_menu(void);
   int draw_range_tools(int x1, int y1);

   void mark_timeline_position(int frame, int x1, int y1, int sb_y2, int col);
   void set_player_section(int i, int p, int c, int f1, int f2, int y, int end_type);
   void fill_player_sections(void);
   void get_more_player_section_info(void);
   void find_ack_moves(int i);
   void find_level_done(void);

   void draw_gm_txt_lines(int x, int y, int dump);
   void proc_gm_list_menu(int gi);
   void edit_gm(int gi);
   void copy_ptp(int sp, int dp);
   void proc_edit_gm_type_menu(int & t);
   void proc_edit_gm_player_num_menu(int & p);

   void find_fire_held_sections(int p);
   void mark_player_carry(int p);
   void mark_player_shot_used(int p, int f, int type);
   void remove_unused_fire_held_sections(int test);

   void proc_file_details_menu(void);
   int draw_file_details(int x1, int y1);

   void load_lnk_arr(void);
   void proc_cpu_time(double frame_start_timestamp);
   void proc_section_details_menu(void);

   int draw_section_details(int x, int y);
   void draw_section_details_one_line(int i, int x, int y);

   int draw_record_settings(int x, int y);
   void draw_timeline(void);

   void proc_transport_menu(void);
   int draw_transport_controls(int x, int y);

};
extern mwDemoRecord mDemoRecord;



