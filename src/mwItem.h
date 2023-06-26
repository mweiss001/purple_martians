// mwItem.h

class mwItem
{
   public:

   mwItem(); // default constructor
   void initialize(void);

   int item[500][16];    // item ints
   float itemf[500][4];  // item floats

   int item_num_of_type[30];
   int item_first_num[30];
   char item_name[30][40];
   char pmsgtext[500][500];
   int item_tile[30];


   int item_secondary67(int type);
   int item_secondary67_hires(int type);

   void draw_items(void);
   void draw_item(int i, int custom, int cx, int cy);
   int draw_bonus(int i, int x, int y, int shape);

   void move_items();
   int is_item_stuck_to_wall(int i);
   int player_drop_item(int p, int i);
   void proc_player_carry(int p);

   void proc_item_collision(int p, int i);
   void proc_bonus_collision(int p, int i);
   void proc_mine_collision(int p, int i);
   void proc_sproingy_collision(int p, int i);

   // mwItemStartExitGate
   int draw_start(int i, int x, int y, int shape);
   int draw_exit(int i, int x, int y, int shape);
   void proc_start_collision(int p, int i);
   void proc_exit_collision(int p, int i);


   void proc_gate_collision(int p, int i);
   int draw_gate(int i, int x, int y, int custom);


   void set_gate_level_icon_position(int i);

   void draw_gate_info(int i);


   char* chrms(int time, char* ft);


   void draw_line(int x1, int x2, int y, const char * txt1, const char * txt2, int col);
   void show_page(int page, int xc, int bs, int by, int lev, int col);


   // mwItemBombRocket.cpp
   int draw_bomb(int i, int x, int y, int shape);
   int draw_lit_bomb(int i);
   void proc_lit_bomb(int i);
   void proc_bomb_collision(int p, int i);

   void proc_rocket_collision(int p, int i);
   void proc_lit_rocket(int i);
   int draw_rocket(int i, int x, int y, int shape);
   void draw_rocket_lines(int i);

   int seq_color(int mod, int c1, int c2);
   int seq_color2(void);
   int seq_color3(void);
   void bomb_block_crosshairs(int e, int f);
   void bomb_blocks(int i, int t, int dr, float x, float y);
   void bomb_crosshairs(float x, float y);
   void bomb_enemies(int i, int t, int dr, float x, float y);
   void bomb_players(int i, int t, int dr, float x, float y);

   // mwItemDoor.cpp
   int draw_door(int i, int x, int y, int custom);
   void proc_door_collision(int p, int i);
   void proc_player_door_move(int p);
   void change_linked_door_color_and_shape(int door);

   // mwItemKeySwitch.cpp
   int draw_key(int i, int x, int y, int tile);
   void proc_key_collision(int p, int i);
   void proc_moving_key(int i);
   void proc_key_block_range(int i, int action);
   void proc_switch_collision(int p, int i);
   void proc_switch_block_range(int i, int action);




   void proc_hider(int i);
   int draw_hider(int i, int x, int y, int tile);
   void erase_hider_areas(void);





   // mwItemMessage.cpp
   int draw_message(int i, int x, int y, int custom);
   void draw_pop_message(int i, int custom, int xpos_c, int ypos, int cursor_pos, int cursor_blink, char *f);
   void proc_pmsg(int i);

   void proc_pmsg_collision(int i);
   void proc_pmsg_reset_timer(int i);
   int edit_pmsg_text(int c, int new_msg);

   int get_frame_size(int num);
   void set_frame_size(int num, int frame_size);


   // mwItemTrigger.cpp
   int draw_orb(int i, int x, int y);
   void proc_orb(int i);
   int proc_orb_shot_collision(int i);
   void proc_orb_collision(int p, int i);

   int draw_trigger(int i, int x, int y, int custom);
   void proc_trigger(int i);
   void set_item_trigger_location_from_lift(int i, int a20);
   void detect_trigger_collisions(int i);

   int draw_block_manip(int i, int x, int y);
   void proc_block_manip(int i);

   int draw_block_damage(int i, int x, int y, int custom);
   void proc_block_damage(int i);
   void set_item_damage_location_from_lift(int i, int a20);
   void proc_item_damage_collisions(int i);

   int draw_timer(int i, int x, int y, int custom);
   void proc_timer(int i);

   void set_timer_flags(int &pack, int state, int t1_mode, int t2_mode, int t1_op_mode, int t2_op_mode, int cnt);
   void get_timer_flags(int pack, int &state, int &t1_mode, int &t2_mode, int &t1_op_mode, int &t2_op_mode, int &cnt);

   float get_timer_ratio_for_event(int ev);


//   int draw_wrap_rect(int i, int x, int y, int custom);
//   void wrap_rect_helper(int i, float &x, float &y, float xi, float yi);
//   void proc_wrap_rect(int i);
//
//   void proc_wrap_line(int i);
//   int draw_wrap_line(int i, int x, int y, int custom);



   // mwItemEditorFnx.cpp
   int item_data(int x, int y);
   void show_all_items(void);
   int sort_item(int set_pos);
   int get_empty_item(void);
   int get_empty_item(int type);
   void check_item(int i, int ct);
   void test_items(void);
   void erase_item(int num);
   int create_trigger(int i);
   int create_timer(int i);
   int create_block_manip(int i);
   int create_block_damage(int i);
   int create_start_block(int c);
   int create_exit(int c);
   void show_all_pmsg(void);
   int create_pmsg(int c);
   int create_door(int type);
   int create_item(int type);

   int create_hider(int i);


};
extern mwItem mItem;
