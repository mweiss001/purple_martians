// mwLift.h





//
//struct lift2
//{
//   float x;
//   float y;
//   float xinc;
//   float yinc;
//
//   float w;
//   float h;
//   float winc;
//   float hinc;
//
//   int active;
//   int flags;
//   int mode;
//   int val1;
//   int val2;
//   int color;
//   int current_step;
//   int num_steps;
//   int limit_counter;
//   int limit_type;
//   char lift_name[40];
//};
//
//struct lift_step2
//{
//   int type;
//   int x;
//   int y;
//   int w;
//   int h;
//   int val;
//};
//
//






class mwLift
{
   public:

   mwLift(); // default constructor
   void initialize(void);


//   struct lift2 lifts[NUM_LIFTS];
//   struct lift_step2 lift_steps[NUM_LIFTS][40];

   char lift_step_type_name[10][10];


};
extern mwLift mwLifts;



