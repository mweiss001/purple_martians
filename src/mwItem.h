// mwItem.h

#ifndef MWITEM_H
#define MWITEM_H
#include "mwMessageText.h"

// forward declaration
class mwLevel;

class mwItem
{
   private:
   mwLevel& m_parent; // Store a reference to parent

   public:

   mwItem(mwLevel& parent) : m_parent(parent) { initialize(); } // default constructor


   void initialize();


   int item[500][16];    // item ints
   float itemf[500][4];  // item floats

   int item_num_of_type[30];
   int item_first_num[30];
   char item_name[30][40];
   int item_tile[30];


   mwMessageText mMessageText;

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

   bool is_item_being_carried(int i);
   bool is_item_moveable(int i, int type);

   bool is_item_carryable(int i);
   bool is_item_carryable_through_door(int i);


   void proc_item_collision(int p, int i);
   void proc_bonus_collision(int p, int i);
   void proc_mine_collision(int p, int i);
   void proc_sproingy_collision(int p, int i);
   float get_sproingy_jump_height(int num);
   int get_sp(float jh);





   // mwItemStartExitGate
   int draw_start(int i, int x, int y, int shape);
   void proc_start_collision(int p, int i);
   void set_player_start_pos(int p);

   int find_gate_for_level(int level);
   int find_gate_for_player(int p);

   void get_gate_tile_index_and_seq(int i, int &index, int &seq);


   void proc_start(int i);


   int draw_exit(int i, int x, int y, int shape);
   void proc_exit_collision(int p, int i);


   void proc_gate_collision(int p, int i);

   void draw_level_icon(int i);

   void draw_gate_tile(int i, int size);

   int draw_gate(int i, int x, int y, int custom);

   void draw_gate_hover_info(int i, int page = -1);

   void draw_level_icon_direct_to_screen(int i);


   char * chrd(int v, char* ft);
   char * chrd(int v1, int v2, char* ft);

   char* chrms(int time, char* ft);

   void draw_line(int x1, int x2, int y, const char * txt1, const char * txt2, int col);
   void show_page(int i, int page, int xc, int bs, int by);



   // mwItemBombRocket.cpp
   int draw_bomb(int i, int x, int y, int shape);
   int draw_lit_bomb(int i);
   void proc_lit_bomb(int i);
   void proc_bomb_collision(int p, int i);

   void proc_rocket_collision(int p, int i);
   void proc_lit_rocket(int i);
   int draw_rocket(int i, int x, int y, int shape);
   void draw_rocket_lines(int i);

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


//   void draw_door_tile(int x, int y, int set, int col, int ans, float scale, int door_entry_type, bool in, bool out);

   void draw_door_tile(int x, int y, int draw_type, int col, int ans, float scale, float rot);


   int find_source_door(int i);




   // mwItemKeySwitch.cpp
   int draw_key(int i, int x, int y, int tile);
   void proc_key_collision(int p, int i);
   void proc_moving_key(int i);
   void proc_key_block_range(int i, int action);
   void proc_switch_collision(int p, int i);
   void proc_switch_block_range(int i, int action);
   void set_switch_tile(int i);

   // hider
   void proc_hider(int i);
   int draw_hider(int i, int x, int y, int tile);
   void erase_hider_areas();







   // mwItemMessage.cpp
   int draw_pmsg(int i, int x, int y, int custom);
   void draw_message(int i, int custom, int xpos_c = 0, int ypos = 0, int cursor_pos = 0, int cursor_blink = 0, char *f = NULL);
   void proc_pmsg(int i);

   void proc_pmsg_collision(int i);
   void proc_pmsg_reset_timer(int i);
   void proc_pmsg_countdown_timer(int i);

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
   void set_trigger_location_from_lift(int i);
   void detect_trigger_collisions(int i);


   int draw_alter_tiles(int i, int x, int y);
   void proc_alter_tiles(int i);


   int draw_damage_area(int i, int x, int y, int custom);
   void proc_damage_area(int i);
   void set_damage_area_location_from_lift(int i);
   void proc_damage_area_collisions(int i);



   int draw_timer(int i, int x, int y, int custom);
   void proc_timer(int i);

//   void set_timer_flags(int &pack, int state, int t1_mode, int t2_mode, int t1_op_mode, int t2_op_mode, int cnt);
//   void get_timer_flags(int pack, int &state, int &t1_mode, int &t2_mode, int &t1_op_mode, int &t2_op_mode, int &cnt);

   void set_timer_flags(int &pack, int state, int t1_mode, int t2_mode, int t1_op_mode, int t2_op_mode, int cnt, int draw_mode);
   void get_timer_flags(int pack, int &state, int &t1_mode, int &t2_mode, int &t1_op_mode, int &t2_op_mode, int &cnt, int &draw_mode);



   float get_timer_ratio_for_event(int ev);


   // mwItemEditorFnx.cpp
   int item_data(int x, int y);
   void show_all_items();
   int sort_item();
   int get_empty_item();
   int get_empty_item(int type);
   void test_items();

   void clear_items();

   void clear_item(int i);

   int create_trigger(int i);
   int create_timer(int i);
   int create_alter_tiles(int i);
   int create_damage_area(int i);
   int create_start(int c);
   int create_exit(int c);
   void show_all_messages();
   int create_message(int c);
   int create_door(int type);
   int create_item(int type);

   int create_hider(int i);

};

#endif


/*

item variable descriptions

list of items

#define PM_ITEM_TYPE_DOOR        1
#define PM_ITEM_TYPE_BONUS       2
#define PM_ITEM_TYPE_EXIT        3
#define PM_ITEM_TYPE_KEY         4
#define PM_ITEM_TYPE_START       5
#define PM_ITEM_TYPE_ORB         6
#define PM_ITEM_TYPE_MINE        7
#define PM_ITEM_TYPE_BOMB        8
#define PM_ITEM_TYPE_TRIGGER     9
#define PM_ITEM_TYPE_MESSAGE    10
#define PM_ITEM_TYPE_ROCKET     11
#define PM_ITEM_TYPE_TIMER      13
#define PM_ITEM_TYPE_SWITCH     14
#define PM_ITEM_TYPE_SPRING     15
#define PM_ITEM_TYPE_ALTERTILES 16
#define PM_ITEM_TYPE_DAMAGEAREA 17
#define PM_ITEM_TYPE_GATE       18
#define PM_ITEM_TYPE_HIDER      19

#define PM_ITEM_TYPE_EXPIRING   66

#define PM_ITEM_TYPE_LIT_ROCKET 98
#define PM_ITEM_TYPE_LIT_BOMB   99



// common
item[][0] = active and type
item[][1] = bitmap or ans
item[][2] = common flags

item[][4] = x pos
item[][5] = y pos

item[][14] = time to live
item[][15] = tag with cloner item id





// type specific

---------------------------------------------------------------
-- 1 - PM_ITEM_TYPE_DOOR
---------------------------------------------------------------

item[][1] draw type
#define PM_ITEM_DOOR_DRAW_HIDE      0
#define PM_ITEM_DOOR_DRAW_STATIC    1
#define PM_ITEM_DOOR_DRAW_WARP      2
#define PM_ITEM_DOOR_DRAW_PORTAL    3

item[][2] common item flags
item[][3] unused
item[][4] x
item[][5] y
item[][6] color

item[][7]  move type (0=INSTANT 1=TRAVEl)
item[][8]  unused
item[][9]  linked destination item (-1 is none)
item[][10] key held flag

item[][11] enter mode
#define PM_ITEM_DOOR_ENTER_IMMEDIATE  0
#define PM_ITEM_DOOR_ENTER_WITH_UP    1
#define PM_ITEM_DOOR_ENTER_WITH_DOWN  2

item[][12] draw connecting line
#define PM_ITEM_DOOR_LINE_NEVER   0
#define PM_ITEM_DOOR_LINE_ALWAYS  1
#define PM_ITEM_DOOR_LINE_TOUCH   2

item[][13] scale (100 = 1.0)


---------------------------------------------------------------
-- 2 - PM_ITEM_TYPE_BONUS
---------------------------------------------------------------
item[][6] bonus type (0 - Health   1 - Purple Coin)
item[][7] health bonus amount


---------------------------------------------------------------
-- 3 - PM_ITEM_TYPE_EXIT
---------------------------------------------------------------
item[][8] exit with x enemies left



---------------------------------------------------------------
-- 4 - PM_ITEM_TYPE_KEY
---------------------------------------------------------------
item[][1]  tile
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] rotation for key move  float rot = (float) item[i][10] / 1000;
item[][11] counter for key move
item[][12] matching keyed blocks only




---------------------------------------------------------------
-- 5 - PM_ITEM_TYPE_START
---------------------------------------------------------------
item[][1] = tiles
item[][6] mode
item[][7] start index
item[][8] no backsies
item[][9] trigger








---------------------------------------------------------------
-- 6 - PM_ITEM_TYPE_ORB
---------------------------------------------------------------
item[][1] = bitmap

item[][3] = orb flags
#define PM_ITEM_ORB_STATE       0b0000000000000001
#define PM_ITEM_ORB_PREV_STATE  0b0000000000000010
#define PM_ITEM_ORB_TGON        0b0000000000000100
#define PM_ITEM_ORB_TGOF        0b0000000000001000
#define PM_ITEM_ORB_TRIG_TOUCH  0b0000000000010000
#define PM_ITEM_ORB_TRIG_UP     0b0000000000100000
#define PM_ITEM_ORB_TRIG_DOWN   0b0000000001000000
#define PM_ITEM_ORB_TRIG_SHOT   0b0000000010000000
#define PM_ITEM_ORB_TRIG_CURR   0b0000000100000000
#define PM_ITEM_ORB_TRIG_PREV   0b0000001000000000
#define PM_ITEM_ORB_ROTB        0b1100000000000000

item[][6]  = mode
item[][7]  = timer val
item[][8]  = timer count

item[][10] = pm_event STATE ON
item[][11] = pm_event STATE OFF
item[][12] = pm_event TGON
item[][13] = pm_event TGOF









---------------------------------------------------------------
-- 7 - PM_ITEM_TYPE_MINE
---------------------------------------------------------------
item[][1] = tile
item[][8] mine damage



---------------------------------------------------------------
-- 8 - PM_ITEM_TYPE_BOMB
---------------------------------------------------------------
item[][1] = bitmap
item[][6]  = exploding mode (0-inactive) (1-lit) (2-exploding) (3-armed)
item[][7]  = damage range
item[][8]  = fuse length counter
item[][9]  = fuse length value
item[][10] = unused as far as I can tell
item[][11] = sticky
item[][12] = type (0-fuse) (1-detonator)
item[][13] = player that lit



---------------------------------------------------------------
-- 9 - PM_ITEM_TYPE_TRIGGER
---------------------------------------------------------------
item[][3] = trigger flags
#define PM_ITEM_TRIGGER_PLAYER   0b000000000000000000000001
#define PM_ITEM_TRIGGER_ENEMY    0b000000000000000000000010
#define PM_ITEM_TRIGGER_ITEM     0b000000000000000000000100
#define PM_ITEM_TRIGGER_PSHOT    0b000000000000000000001000
#define PM_ITEM_TRIGGER_ESHOT    0b000000000000000000010000
#define PM_ITEM_TRIGGER_CURR     0b000000000000000000100000
#define PM_ITEM_TRIGGER_PREV     0b000000000000000001000000
#define PM_ITEM_TRIGGER_TGON     0b000000000000000010000000
#define PM_ITEM_TRIGGER_TGOF     0b000000000000000100000000
#define PM_ITEM_TRIGGER_LIFT_ON  0b000000000000001000000000
#define PM_ITEM_TRIGGER_LIFT_XC  0b000000000000010000000000
#define PM_ITEM_TRIGGER_LIFT_XF  0b000000000000100000000000
#define PM_ITEM_TRIGGER_LIFT_XL  0b000000000001000000000000
#define PM_ITEM_TRIGGER_LIFT_YC  0b000000000010000000000000
#define PM_ITEM_TRIGGER_LIFT_YF  0b000000000100000000000000
#define PM_ITEM_TRIGGER_LIFT_YL  0b000000001000000000000000
#define PM_ITEM_TRIGGER_DRAW_ON  0b000000010000000000000000
#define PM_ITEM_TRIGGER_COLOR    0b000111100000000000000000

item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)

item[][10] = trigger field lift number

item[][11] = pm_event CURR ON
item[][12] = pm_event CURR OFF
item[][13] = pm_event TGON
item[][14] = pm_event TGOF





---------------------------------------------------------------
-- 10 - PM_ITEM_TYPE_MESSAGE
---------------------------------------------------------------
item[][1] event trigger

item[][3] flags
#define PM_ITEM_MESSAGE_SHOW_ALWAYS   0b00000000000000001
#define PM_ITEM_MESSAGE_SHOW_SCROLL   0b00000000000000010
#define PM_ITEM_MESSAGE_FRAME0        0b00000000000001000
#define PM_ITEM_MESSAGE_FRAME1        0b00000000000010000
#define PM_ITEM_MESSAGE_FRAME2        0b00000000000100000
#define PM_ITEM_MESSAGE_FRAME4        0b00000000001000000
#define PM_ITEM_MESSAGE_FRAME12       0b00000000010000000

item[][4]  scroll x
item[][5]  scroll y

item[][6]  msg x
item[][7]  msg y
item[][8]  msg w
item[][9]  msg h

item[][12] timer count and val  (packed)
item[][13] text and frame color (packed)


---------------------------------------------------------------
-- 11 - PM_ITEM_TYPE_ROCKET
---------------------------------------------------------------
item[][1] = bitmap
item[][6]  = steerability
item[][7]  = damage range
item[][8]  = max speed
item[][9]  = acceleration
item[][10] = rotation
item[][11] = current speed
item[][13] = player that lit




---------------------------------------------------------------
-- 12 - not used
---------------------------------------------------------------


---------------------------------------------------------------
-- 13 - PM_ITEM_TYPE_TIMER
---------------------------------------------------------------
item[][3]  = flags
#define PM_ITEM_TIMER_COUNT         0b11111111111111110000000000000000
#define PM_ITEM_TIMER_T1_MODE       0b00000000000000001110000000000000
#define PM_ITEM_TIMER_T2_MODE       0b00000000000000000001110000000000
#define PM_ITEM_TIMER_T1_OP_MODE    0b00000000000000000000001000000000
#define PM_ITEM_TIMER_T2_OP_MODE    0b00000000000000000000000100000000
#define PM_ITEM_TIMER_DRAW_MODE     0b00000000000000000000000000000010
#define PM_ITEM_TIMER_STATE_CURR    0b00000000000000000000000000000001

item[][6]  = display x
item[][7]  = display y
item[][8]  = display w
item[][9]  = display h

item[][10] = t1 reset val
item[][11] = t2 reset val
item[][12] = t1 i/p event
item[][13] = t1 o/p event
item[][14] = t2 i/p event
item[][15] = t2 o/p event




---------------------------------------------------------------
-- 14 - PM_ITEM_TYPE_SWITCH
---------------------------------------------------------------
item[][1]  tile
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] color flags
item[][11] lockout


---------------------------------------------------------------
-- 15 - PM_ITEM_TYPE_SPRING
---------------------------------------------------------------
item[][1] tile
item[][6] jump length (only used in level editor)
item[][7] sproinginess

---------------------------------------------------------------
-- 16 - PM_ITEM_TYPE_ALTERTILES
---------------------------------------------------------------
item[][1] = pm_event_trigger

item[][3] = mode
mode 1 set all to block 1
mode 2 set all block 2 to block 1
mode 3 toggle block 2 to block 1
mode 4 copy area
mode 5 cycle 3
mode 6 cycle 4
mode 7 cycle 4 but don't loop, one way only

item[][6]  = field x (2000)
item[][7]  = field y (2000)
item[][8]  = field w (2000)
item[][9]  = field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color
item[][13] = copy x or block 3
item[][14] = copy y or block 4



---------------------------------------------------------------
-- 17 - PM_ITEM_TYPE_DAMAGE
---------------------------------------------------------------
item[][1]  = event trigger number
item[][3]  = damage flags
#define PM_ITEM_DAMAGE_PLAYER   0b00000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b00000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b00000000000000100
#define PM_ITEM_DAMAGE_PSHOT    0b00000000000001000
#define PM_ITEM_DAMAGE_ESHOT    0b00000000000010000
#define PM_ITEM_DAMAGE_CURR     0b00000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b00000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b00000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b00000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b00000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b00000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b00000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b00001000000000000

item[][6]  = field x
item[][7]  = field y
item[][8]  = field w
item[][9]  = field h
item[][10] = lift number
item[][11] = mode
item[][12] = player damage
item[][13] = draw type


---------------------------------------------------------------
-- 18 - PM_ITEM_TYPE_GATE
---------------------------------------------------------------
item[][1] = draw mode
0 Draw Mode:Legacy
1 Draw Mode:Gold Colored Door
2 Draw Mode:Wooden Door
3 Draw Mode:Metal Door
4 Draw Mode:Portcullis

item[][6]  = level
item[][7]  = current display page
item[][10] = level icon size
item[][11] = hover level icon size



---------------------------------------------------------------
-- 19 - PM_ITEM_TYPE_HIDER
---------------------------------------------------------------
item[][1]  = pm_event_trigger
item[][3]  = mode
item[][6]  = block range x
item[][7]  = block range y
item[][8]  = block range w
item[][9]  = block range h
item[][10] = active










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
























