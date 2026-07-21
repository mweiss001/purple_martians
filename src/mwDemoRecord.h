// mwDemoRecord.h

#include "mwWindowManager.h"

class mwDemoRecord
{
   public:
   mwWindowManager mWM = mwWindowManager(5);

   void draw_transport_controls(mwRect<int> rect, int disable_input, int have_focus);
   void draw_GMList(mwRect<int> rect, int disable_input, int have_focus);
   void draw_timeline(mwRect<int> rect, int disable_input, int have_focus);

   int record_mode_active{};

   int lnk_arr[10000] = {0};
   int lnk_entry_pos = 0;

   char current_loaded_demo_file[256];

   int fhs[10000][4] = {0};
   int fhs_indx = 0;

   int play = 0;
   int record = 0;

   int record_player_number = 0;
   int record_player_color = 8;
   int record_punch_in_armed = 0;
   int current_frame_num = 0;
   int current_section = 0;

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

   int sh_overlay_in_play = 0;
   int sh_overlay_in_rec = 1;

   void gdt();
   char * gettf(int frame, char* ft);


private:
   void seek_to_frame(int frame, int draw);
public:

   void init();

   void redraw_callback();

   int draw_overlay();

   void demo_record();
   int load_demo_record();
   void refresh();
   void reload();

   void start_record();
   void stop_transport();

   void set_active_section(int i);

   void range_tools_set_fire_moves(int f, int clear_set);
   void range_tools_do(int action);
   void proc_range_tools_menu();
   int draw_range_tools(int x1, int y1);

   void proc_gm_list_menu(int gi, int x, int y);
   void edit_gm(int gi, int x, int y);
   void copy_ptp(int sp, int dp);
   void proc_edit_gm_type_menu(int & t);
   void change_player_num_menu(int & p);

   void find_fire_held_sections(int p);
   void mark_player_carry(int p);
   void mark_player_shot_used(int p, int f, int type);
   void remove_unused_fire_held_sections(int test);

   void proc_file_details_menu();
   int draw_file_details(int x1, int y1);

   void load_lnk_arr();
   void proc_cpu_time(double frame_start_timestamp);
   void proc_section_details_menu();

   int draw_section_details(int x, int y);
   int draw_record_settings(int x, int y);

   void proc_transport_menu();

};
extern mwDemoRecord mDemoRecord;



