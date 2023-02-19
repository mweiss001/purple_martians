// z_lift.h

// ------------------------------------------------
// ---------------- lifts -----------------------
// ------------------------------------------------
extern char lift_step_type_name[10][10];
extern struct lift lifts[NUM_LIFTS];
extern struct lift_step lift_steps[NUM_LIFTS][40];

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


int construct_lift(int l, char* lift_name);
void clear_lift(int l);
int get_empty_lift(void);
int create_lift(void);
void erase_lift(int lift);
void delete_lift_step(int l, int step);
void lift_setup(void);

int construct_lift_step(int l, int s, int type, int x, int y, int w, int h, int val);
void show_all_lifts(void);
void lift_step_set_size_from_previous_move_step(int lift, int step);
int lift_find_previous_move_step(int lift, int step);
void move_lift_step(int lift, int step);
int get_new_lift_step(int lift, int step);
int insert_lift_step(int lift, int step);
void insert_steps_until_quit(int lift, int step);
void set_all_steps(int l, int s, int what);
void step_popup_menu(int lift, int step);
int draw_current_step_buttons(int x1, int x2, int y, int l, int s, int d);
void draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc, int d);
int draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step, int d);

void clear_lift_step(int l, int s);
int get_num_lifts(void);
int get_next_lift(int l);
int get_prev_lift(int l);


void set_lift_to_step(int l, int s);
int is_player_riding_lift(int l);
void draw_lift_line(int l);
void draw_lift(int l, int x1, int y1, int x2, int y2);
void draw_lifts();
void set_lift_xyinc(int l, int step);
int lift_check_prox(int l, int pd);
void move_lifts(int ignore_prox);
