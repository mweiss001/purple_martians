// z_bullets.h

#include "pm.h"

// bullets
extern int pbullet[50][6];
extern int pm_bullet_collision_box;
extern struct ebullet ebullets[50];
struct ebullet
{
   int active;
   int shape;
   al_fixed fx;
   al_fixed fy;
   al_fixed fxinc;
   al_fixed fyinc;
};


void proc_pbullet_shoot(int p);
void proc_pbullet_collision(int p, int b);
void move_pbullets(void);
void draw_pbullets(void);
void proc_ebullet_collision(int p, int b);
void move_ebullets(void);
void draw_ebullets(void);
void clear_bullets(void);
