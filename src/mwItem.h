// mwItem.h

#ifndef MWITEM_H
#define MWITEM_H


class mwItem
{
   public:

   mwItem(); // default constructor
   void initialize();


   int item[500][16];    // item ints
   float itemf[500][4];  // item floats

   int item_num_of_type[30];
   int item_first_num[30];
   char item_name[30][40];
   char pmsgtext[500][500];
   int item_tile[30];


   int item_secondary67(int type);
   int item_secondary67_hires(int type);

   void draw_items();
   void draw_item(int i, int custom, int cx, int cy);
   int draw_bonus(int i, int x, int y, int shape);
   void draw_purple_coin_screen_direct(int i);

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
   void proc_start_collision(int p, int i);
   void set_player_start_pos(int p);

   int draw_exit(int i, int x, int y, int shape);
   void proc_exit_collision(int p, int i);



   void proc_gate_collision(int p, int i);
   int draw_gate(int i, int x, int y, int custom);
   void set_gate_level_icon_position(int i);
   void draw_gate_info(int i);

   char * chrd(int v, char* ft);
   char * chrd(int v1, int v2, char* ft);

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
   int seq_color2();
   int seq_color3();
   void bomb_block_crosshairs(int e, int f);
   void bomb_blocks(int i, int t, int dr, float x, float y);
   void bomb_crosshairs(float x, float y);
   void bomb_enemies(int i, int t, int dr, float x, float y);
   void bomb_players(int i, int t, int dr, float x, float y);

   // mwItemDoor.cpp
   int draw_door(int i, int x, int y, int custom);
   void proc_door_collision(int p, int i);
   void proc_player_door_move(int p);
   void change_linked_door_color_and_draw_type(int door);

   // mwItemKeySwitch.cpp
   int draw_key(int i, int x, int y, int tile);
   void proc_key_collision(int p, int i);
   void proc_moving_key(int i);
   void proc_key_block_range(int i, int action);
   void proc_switch_collision(int p, int i);
   void proc_switch_block_range(int i, int action);


   // hider
   void proc_hider(int i);
   int draw_hider(int i, int x, int y, int tile);
   void erase_hider_areas();







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


   // mwItemEditorFnx.cpp
   int item_data(int x, int y);
   void show_all_items();
   int sort_item(int set_pos);
   int get_empty_item();
   int get_empty_item(int type);
   void test_items();
   void clear_item(int i);

   int create_trigger(int i);
   int create_timer(int i);
   int create_block_manip(int i);
   int create_block_damage(int i);
   int create_start_block(int c);
   int create_exit(int c);
   void show_all_pmsg();
   int create_pmsg(int c);
   int create_door(int type);
   int create_item(int type);

   int create_hider(int i);


};
extern mwItem mItem;

#endif


/*

item variable descriptions

list of items


[1]  - door
[2]  - bonus
[3]  - exit
[4]  - key
[5]  - start
[6]  - orb
[7]  - mine
[8]  - bomb
[9]  - trigger
[10] - pop-up msg
[11] - rocket
[12] - not used
[13] - timer
[14] - switch
[15] - sproingy
[16] - block manip
[17] - block damage
[18] - gate
[19] - hider

[98] - lit rocket
[99] - lit bomb

// common
item[][0] = active and type
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4] = x pos
item[][5] = y pos
item[][14] = time to live
item[][15] = tag with cloner item id

// type specific

[1] - door
item[][1]  draw type (0=hidden, 1=static door, 2=animated warp)
item[][6]  color
item[][7]  move type (0=auto, 1=force instant, 2=force move)
item[][8]  type (0=exit only, 1=linked dest)
item[][9]  linked destination item
item[][10] key held flag
item[][11] door entry type (0=immed, 1=up, 2=down)
item[][12] draw lines always, never, only when touched




[2] - bonus
item[][6] bonus type
1 - Health
2 - Free Man
3 - Purple Coin
item[][7] health bonus

[3] - exit
item[][8] exit with x enemies left

[4] - key
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] rotation for key move
item[][11] counter for key move
item[][12] matching keyed blocks only

[5] - start
item[][6] mode
item[][7] start index
item[][8] no backsies



[6] - orb


[7] - mine
item[][8] mine damage

[8] - bomb
item[][6]  mode (2=explosion)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value
item[][11]  sticky
item[][12]  timer / remote

[9]  - Trigger
item[][2] = draw_type (color)
item[][3] = flags
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] = trigger field lift number
item[][11] = CURR ON  pm_event
item[][12] = CURR OFF pm_event
item[][13] = TGON pm_event #
item[][14] = TGOF pm_event #

[10] - msg
item[][1] event trigger
item[][2] draw flags
item[][6]  msg x
item[][7]  msg y
item[][8]  msg w
item[][9]  msg h
item[][10]
item[][11]
item[][12] timer count and val  (packed)
item[][13] text and frame color (packed)

[11] - rocket
item[][6]  steerabaility
item[][7]  blast size
item[][8]  max speed
item[][9]  accel
item[][10] rocket rotation (scaled by 10)


[12] not used

[13] timer

[14]  - switch
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] color flags
item[][11]  lockout

[15] - sproingy
item[][6] jump length (only used in level editor)
item[][7] sproinginess

[16] - Block Manip
item[][1] = pm_event_trigger
item[][2] = draw on
item[][3] = mode
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color




[17] - Block Damage
item[][6]  = field x (2000)
item[][7]  = field y (2000)
item[][8]  = field w (2000)
item[][9]  = field h (2000)
item[][10] = lift number
item[][11] = mode
item[][12] = t1 val
item[][13] = count
item[][14] = t2 val
item[][15] = damage

[18] - gate
item[][6] level num
item[][7] player touching
item[][8] display page
item[][9]
item[][10]
item[][11]
item[][12]

[19] hider



[98] - lit rocket
[99] - lit bomb
item[][6]  mode (1=lit, 2=explosion, 3=remote detonator)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value

item[][11] sticky
item[][12] timer | remote
item[][13] last player to touch

*/
























