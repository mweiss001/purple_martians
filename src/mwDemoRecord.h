// mwDemoRecord.h
#include "mwWindowManager.h"
class mwDemoRecord
{
   public:
   mwWindowManager mWM = mwWindowManager(8);

   void draw_mainW(mwWindow w);
   void draw_GMList(mwWindow &w);
   void draw_transport_controls(mwWindow w);
   void draw_timeline(mwWindow w);
   void draw_current_section(mwWindow w);
   void draw_file_details(mwWindow w);
   void draw_range_tools(mwWindow w);
   void draw_record_settings(mwWindow w);


   void draw_transport_controls_seek(mwRect<int> rect, int d);

   void draw_transport_controls_speed(mwRect<int> rect, int d);


   int draw_GMList_controls(mwWindow &w);


   void proc_timeline_context_menu(int gmInfo_index, int mouse_frame);

   void draw_timeline_section_text(mwRect<int> rect, int gmInfo_index);


   bool draw_timeline_tracks(int x1, int x2, int y1, float bts, float ls, bool smallText, bool sizeOnly, int &w, int &h, int &gmInfo_index, int display_only);

   void draw_timeline_time_box(float xc, int y1, int y2, int y_offset, int f, int color);


   void change_player_color_and_name_dialog(int gmInfoIndex);

   void delete_section(int gmInfo_index);

   void sh(bool inc_dec, float per);

   void set_window_positions(int set);

   int lnk_arr[10000] = {0};
   int lnk_entry_pos = 0;

   char last_loaded_demo_file[1024];

   int fhs[10000][4] = {0};
   int fhs_indx = 0;

   int play = 0;
   int record = 0;
   int record_punch_in_armed = 0;

   int current_section = 0;

   int time_format = 1;

   int show_cpu_graph = 0;
   int show_player_grid = 0;
   int show_windows_in_play = 1;
   int show_windows_in_rec = 0;



   int gm_list_all = 0;
   int gm_list_mono = 0;
   int gm_list_simple = 0;
   int gm_list_start_index = 0;
   int gm_list_edit_index = -1;



   int background_x_justify = 2; //  0-center 1-left 2-right
   int background_y_justify = 1; //  0-center 1-top 2-bottom


   int rt_start_frame = 0;
   int rt_end_frame = 0;
   int rt_all_players = 0;
   int rt_player = 0;
   int rt_player_mode = 0;



   int timeline_level_done_color = 11;
   int timeline_mouse_pos_color = 14;
   int timeline_player_deaths_color = 10;
   int timeline_purple_coins_color = 8;
   int timeline_enemy_hit_color = 14;
   int timeline_display_time_labels = -4;
   int timeline_show_player_icons = 1;

   int timeline_last_frame = 0;





   void gdt();
   char * gettf(int frame, char* ft);


   void init();

   void set_timeline_colors(int level_done, int mouse_pos, int player_deaths, int purple_coins, int enemies_hit, int time_labels, int player_icons);


   void redraw_callback();

   void demo_record();
   int load_demo_record(bool get_new);
   void refresh();
   void reload();

   void start_record();
   void stop_transport();

   void set_active_section(int i);

   void range_tools_set_fire_moves(int f, int clear_set);
   void range_tools_do(int action);
   void proc_range_tools_menu();

   void proc_gm_list_menu(int gi, int x, int y);
   void edit_gm(int gi, int x, int y);
   void proc_edit_gm_type_menu(int & t);
   void change_player_num_menu(int & p);

   void find_fire_held_sections(int p);
   void mark_player_carry(int p);
   void mark_player_shot_used(int p, int f, int type);
   void remove_unused_fire_held_sections(int test);

   void proc_file_details_menu();

   void load_lnk_arr();
   void proc_cpu_time(double frame_start_timestamp);
   void proc_section_details_menu();

   void add_new_section_dialog();

   void proc_transport_menu();


   void save_mWM();
   bool load_mWM();



};
extern mwDemoRecord mDemoRecord;



