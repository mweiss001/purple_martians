// mwLift.h

struct lift
{
   float x;
   float y;
   float xinc;
   float yinc;
   float w;
   float h;
   float winc;
   float hinc;
   int active;
   int flags;
   int mode;
   int val1;
   int val2;
   int color;
   int current_step;
   int num_steps;
   int limit_counter;
   int limit_type;
   char lift_name[40];
};

struct lift_step
{
   int type;
   int x;
   int y;
   int w;
   int h;
   int val;
};

class mwLift
{
   public:

   mwLift(); // default constructor
   void initialize();

   struct lift       cur[NUM_LIFTS];
   struct lift_step  stp[NUM_LIFTS][40];
   char lift_step_type_name[10][10];

   // these are used in the game
   void move_lifts(int ignore_prox);
   void next_step(int l);


   void set_lift_to_step(int l, int s);
   int is_player_riding_lift(int l);
   int lift_check_prox(int l, int pd);
   void set_lift_xyinc(int l, int step);

   // these are used by both to draw
   void draw_lift_line(int l);
   void draw_lift(int l, int x1, int y1, int x2, int y2);
   void draw_lifts();

   // this is used when loading and saving level and after run lifts in level editor, sets all lifts to step 0
   void lift_setup();

   // used in level editor only
   int construct_lift(int l, char* lift_name);
   void clear_lift(int l);
   int get_empty_lift();
   int create_lift();
   void erase_lift(int lift);

   void clear_lift_step(int l, int s);
   void delete_lift_step(int l, int s);
   int construct_lift_step(int l, int s, int type, int x, int y, int w, int h, int val);

   void show_all_lifts();
   int get_num_lifts();
   int get_next_lift(int l);
   int get_prev_lift(int l);

   void set_size_from_previous_step(int l, int s);
   int find_previous_move_step(int l, int s);
   void move_lift_step(int l, int s);
   int get_new_lift_step(int l, int s);
   int insert_lift_step(int l, int s);
   void insert_steps_until_quit(int l, int s);
   void set_all_steps(int l, int s, int what);
   void step_popup_menu(int l, int s);
   int draw_current_step_buttons(int x1, int x2, int y, int l, int s, int d);
   void draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc, int d);
   int draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step, int d);

};
extern mwLift mLift;

