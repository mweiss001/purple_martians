// mwShots.h

struct eshot
{
   int active;
   int shape;
   float x;
   float y;
   float xinc;
   float yinc;
};

struct pshot
{
   int active;
   int player;
   float x;
   float y;
   float xinc;
   float yinc;
};

class mwShots
{
   public:

   struct eshot e[50];
   struct pshot p[50];

   int deathmatch_shots;
   int deathmatch_shot_damage;
   int suicide_shots;

   void clear_shots(void);

   void proc_player_shoot(int p);

   void proc_pshot_collision(int p, int b);
   void move_pshots(void);
   void draw_pshots(void);

   void proc_eshot_collision(int p, int b);
   void move_eshots(void);
   void draw_eshots(void);

   void fire_enemy_shotz(int e, int shot_ans, float px, float py);
   void fire_enemy_shota(int e, int shot_ans, int p);
   void fire_enemy_x_shot(int e, int p);

};
extern mwShots mShot;



