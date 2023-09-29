// mwWindowGroupEdit.cpp
#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwLevel.h"



// structure for predefined control things that can be group edited
struct ge_datum
{
   char name[40];
   char snam[8];
   int vartyp;     // 2=item, 3=mEnemy.Ei, 4=mEnemy.Ef, 5=initial direction, 6=initial position, 8 = sec xc
   int varnum;     // index of the variable
   int valid;      // does it apply to items in list
   int collapsed;  // how it is drawn
   int adj_mode;
   float adj_min;
   float adj_max;
   float min_allowed;
   float max_allowed;
};
struct ge_datum ge_data[100];

/*

vartype 2
int item[][varnum]

vartype 3
int Ei[][varnum]

vartype 4
float Ef[][varnum]

vartype 5 - initial direction
vartype 6 - initial position

vartype 8 - sec xc
vartype 9 - sec y2


vartype 7 - vinepod flags
varnum = flag bits



*/


// mapping table of controls to objects and types
int cot[100][4][20] = {0}; // control, object_type, type,  mapping table

void mwWindow::ge_init_data(void)
{
   for (int i=0; i<100; i++)
   {
      ge_data[i].vartyp = 0;
      ge_data[i].varnum = 0;
      ge_data[i].valid = 0;
      ge_data[i].collapsed = 0;
      ge_data[i].adj_mode = 0;
      ge_data[i].adj_min = 0;
      ge_data[i].adj_max = 0;
      ge_data[i].min_allowed = 0;
      ge_data[i].max_allowed = 0;
      sprintf(ge_data[i].name, " ");
      sprintf(ge_data[i].snam, " ");
   }
   int i = 0;

   sprintf(ge_data[i].name, "Initial Position");
   sprintf(ge_data[i].snam, "ipos");
   ge_data[i].vartyp = 6;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 0;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 0;


   cot[i][3][1] = 1; // bouncer
   cot[i][3][2] = 1; // cannon
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][6] = 1; // flapper
   cot[i][3][7] = 1; // vinepod
   cot[i][3][8] = 1; // trakbot
   cot[i][3][9] = 1; // cloner
   cot[i][3][19] = 1; // crew

   cot[i][2][1]  = 1; // door
   cot[i][2][2]  = 1; // bonus
   cot[i][2][3]  = 1; // exit
   cot[i][2][4]  = 1; // key
   cot[i][2][5]  = 1; // start
   cot[i][2][6]  = 1; // orb
   cot[i][2][7]  = 1; // mine
   cot[i][2][8]  = 1; // bomb
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][11] = 1; // rocket
   cot[i][2][12] = 1; // warp
   cot[i][2][13] = 1; // timer
   cot[i][2][14] = 1; // switch
   cot[i][2][15] = 1; // sproingy
   cot[i][2][16] = 1; // block manip
   cot[i][2][17] = 1; // block damage


   i++;
   sprintf(ge_data[i].name, "Initial Direction");
   sprintf(ge_data[i].snam, "idir");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 2;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // block walker
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][6] = 1; // flapper
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Initial Direction");
   sprintf(ge_data[i].snam, "idir");
   ge_data[i].vartyp = 5;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 0;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 0;
   cot[i][3][1] = 1; // bouncer
   cot[i][3][2] = 1; // cannon

   i++;
   sprintf(ge_data[i].name, "Initial Direction");
   sprintf(ge_data[i].snam, "idir");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 8;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 8;
   cot[i][3][8]  = 1; // trakbot

   i++;
   sprintf(ge_data[i].name, "X speed");
   sprintf(ge_data[i].snam, "xspd");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "X speed");
   sprintf(ge_data[i].snam, "xspd");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 2;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   cot[i][3][8]  = 1; // trakbot

   i++;
   sprintf(ge_data[i].name, "X Speed");
   sprintf(ge_data[i].snam, "xspd");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 4;
   ge_data[i].min_allowed = 1;
   ge_data[i].max_allowed = 20;
   cot[i][3][6] = 1; // flapper

   i++;
   sprintf(ge_data[i].name, "X Acceleration");
   sprintf(ge_data[i].snam, "xacl");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = .1;
   ge_data[i].adj_max = 3;
   ge_data[i].min_allowed = .01;
   ge_data[i].max_allowed = 5;
   cot[i][3][6] = 1; // flapper

   i++;
   sprintf(ge_data[i].name, "X ground speed (1/x)");
   sprintf(ge_data[i].snam, "xspd");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 4;
   ge_data[i].adj_min = 1;
   ge_data[i].adj_max = 12;
   ge_data[i].min_allowed = 1;
   ge_data[i].max_allowed = 12;
   cot[i][3][5] = 1; // jumpworm

   i++;
   sprintf(ge_data[i].name, "X jump speed");
   sprintf(ge_data[i].snam, "xspd");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   cot[i][3][5]  = 1; // jumpworm

   i++;
   sprintf(ge_data[i].name, "Y speed");
   sprintf(ge_data[i].snam, "yspd");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 3;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][6] = 1; // flapper
   cot[i][3][8] = 1; // trakbot
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Wall Jump Boost");
   sprintf(ge_data[i].snam, "wljb");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 8;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2;
   cot[i][3][5] = 1; // jumpworm

   i++;
   sprintf(ge_data[i].name, "Drop Mode");
   sprintf(ge_data[i].snam, "drpm");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2;
   cot[i][3][8] = 1; // trakbot

   i++;
   sprintf(ge_data[i].name, "Turn Before Hole");
   sprintf(ge_data[i].snam, "tb4h");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 10;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 10;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 180;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Jump Before Hole");
   sprintf(ge_data[i].snam, "jb4h");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 11;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 10;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 180;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Jump Before Wall");
   sprintf(ge_data[i].snam, "jb4w");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 12;
   ge_data[i].adj_min = 30;
   ge_data[i].adj_max = 60;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 120;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Jump Under Width");
   sprintf(ge_data[i].snam, "juwd");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 30;
   ge_data[i].adj_max = 60;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 500;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Jump Wait");
   sprintf(ge_data[i].snam, "juwt");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 30;
   ge_data[i].adj_max = 60;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 500;
   cot[i][3][3] = 1; // archwagon
   cot[i][3][4] = 1; // blokwalk
   cot[i][3][5] = 1; // jumpworm
   cot[i][3][19] = 1; // crew

   i++;
   sprintf(ge_data[i].name, "Flap Height");
   sprintf(ge_data[i].snam, "flph");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 21;
   ge_data[i].adj_min = 10;
   ge_data[i].adj_max = 40;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 200;
   cot[i][3][6] = 1; // flapper

   i++;
   sprintf(ge_data[i].name, "Flap Speed");
   sprintf(ge_data[i].snam, "flps");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 10;
   ge_data[i].adj_min = 1.5;
   ge_data[i].adj_max = 4;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 20;
   cot[i][3][6] = 1; // flapper

   i++;
   sprintf(ge_data[i].name, "Height Above Player");
   sprintf(ge_data[i].snam, "hapl");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 10;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 80;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 200;
   cot[i][3][6] = 1; // flapper

   i++;
   sprintf(ge_data[i].name, "Seek Count");
   sprintf(ge_data[i].snam, "skct");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 8;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 100;
   cot[i][3][1] = 1; // bouncer
   cot[i][3][2] = 1; // cannon

   i++;
   sprintf(ge_data[i].name, "Seek Speed");
   sprintf(ge_data[i].snam, "sksp");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 40;
   cot[i][3][1] = 1; // bouncer
   cot[i][3][2] = 1; // cannon

   i++;
   sprintf(ge_data[i].name, "Extra Hits To Kill");
   sprintf(ge_data[i].snam, "xhtk");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 3;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 20;
   cot[i][3][2] = 1; // cannon

   i++;
   sprintf(ge_data[i].name, "Shot Speed");
   sprintf(ge_data[i].snam, "shsp");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .1;
   ge_data[i].max_allowed = 80;
   cot[i][3][2] = 1; // cannon
   cot[i][3][3] = 1; // archwagon
   cot[i][3][6] = 1; // flapper
   cot[i][3][7] = 1; // vinepod
   cot[i][3][8] = 1; // trakbot

   i++;
   sprintf(ge_data[i].name, "Shot Retrigger");
   sprintf(ge_data[i].snam, "shrt");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 15;
   ge_data[i].adj_min = 80;
   ge_data[i].adj_max = 200;
   ge_data[i].min_allowed = 2;
   ge_data[i].max_allowed = 1000;
   cot[i][3][2] = 1; // cannon
   cot[i][3][3] = 1; // archwagon
   cot[i][3][6] = 1; // flapper
   cot[i][3][8] = 1; // trakbot

   i++;
   sprintf(ge_data[i].name, "Shot Proximity");
   sprintf(ge_data[i].snam, "shpr");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 17;
   ge_data[i].adj_min = 80;
   ge_data[i].adj_max = 200;
   ge_data[i].min_allowed = 2;
   ge_data[i].max_allowed = 2000;
   cot[i][3][2] = 1; // cannon
   cot[i][3][3] = 1; // archwagon
   cot[i][3][6] = 1; // flapper
   cot[i][3][8] = 1; // trakbot



   i++;
   sprintf(ge_data[i].name, "Item Secondary x1");
   sprintf(ge_data[i].snam, "scx1");
   ge_data[i].vartyp = 2;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2000;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2000;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage


   i++;
   sprintf(ge_data[i].name, "Item Secondary y1");
   sprintf(ge_data[i].snam, "scy1");
   ge_data[i].vartyp = 2;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2000;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2000;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage

   i++;
   sprintf(ge_data[i].name, "Item Secondary w");
   sprintf(ge_data[i].snam, "scwi");
   ge_data[i].vartyp = 2;
   ge_data[i].varnum = 8;
   ge_data[i].adj_min = 8;
   ge_data[i].adj_max = 1000;
   ge_data[i].min_allowed = 8;
   ge_data[i].max_allowed = 1000;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage

   i++;
   sprintf(ge_data[i].name, "Item Secondary h");
   sprintf(ge_data[i].snam, "sche");
   ge_data[i].vartyp = 2;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 8;
   ge_data[i].adj_max = 1000;
   ge_data[i].min_allowed = 8;
   ge_data[i].max_allowed = 1000;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage


   i++;
   sprintf(ge_data[i].name, "Item Secondary xc");
   sprintf(ge_data[i].snam, "scxc");
   ge_data[i].vartyp = 8;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 20;
   ge_data[i].adj_max = 1980;
   ge_data[i].min_allowed = 20;
   ge_data[i].max_allowed = 1980;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage


   i++;
   sprintf(ge_data[i].name, "Item Secondary y2");
   sprintf(ge_data[i].snam, "scy2");
   ge_data[i].vartyp = 9;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 20;
   ge_data[i].adj_max = 1980;
   ge_data[i].min_allowed = 20;
   ge_data[i].max_allowed = 1980;
   cot[i][2][9]  = 1; // trigger
   cot[i][2][10] = 1; // message
   cot[i][2][13] = 1; // timer
   cot[i][2][17] = 1; // block damage

   i++;
   sprintf(ge_data[i].name, "Extend Time");
   sprintf(ge_data[i].snam, "extm");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 17;
   ge_data[i].adj_min = 10;
   ge_data[i].adj_max = 100;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 800;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Pause Time");
   sprintf(ge_data[i].snam, "paus");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 10;
   ge_data[i].adj_max = 100;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 800;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Control Point Multiplier");
   sprintf(ge_data[i].snam, "ctpm");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 1;
   ge_data[i].adj_max = 4;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 8;
   cot[i][3][7]  = 1; // vinepod



   i++;
   sprintf(ge_data[i].name, "Show Path");
   sprintf(ge_data[i].snam, "shpt");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_SHOW_PATH;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Invincible Initial Position");
   sprintf(ge_data[i].snam, "invi");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_INV_INIT;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Invincible Extended Position");
   sprintf(ge_data[i].snam, "inve");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_INV_EXTN;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Invincible While Moving");
   sprintf(ge_data[i].snam, "invm");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_INV_MOVE;
   cot[i][3][7]  = 1; // vinepod


   i++;
   sprintf(ge_data[i].name, "Use Spline");
   sprintf(ge_data[i].snam, "ussp");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_USE_SPLINE;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Use Trigger");
   sprintf(ge_data[i].snam, "ustr");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_USE_TRIGGER;
   cot[i][3][7]  = 1; // vinepod

   i++;
   sprintf(ge_data[i].name, "Fire Shot");
   sprintf(ge_data[i].snam, "frsh");
   ge_data[i].vartyp = 7;
   ge_data[i].varnum = PM_ENEMY_VINEPOD_FIRE_SHOT;
   cot[i][3][7]  = 1; // vinepod

   //printf("i:%d\n", i);


}

// iterate the object list to find out what controls are valid
void mwWindow::ge_set_valid_controls(void)
{
   // set all controls valid by default
   for (int i=0; i<100; i++) if (ge_data[i].vartyp) ge_data[i].valid = 1;

   // remove controls as soon as they don't apply to all objects in list
   for (int i=0; i<NUM_OBJ; i++)
      if (mWM.obj_list[i][0]) // iterate all objects in list
      {
         int obt = mWM.obj_list[i][0]; // object type
         int num = mWM.obj_list[i][1]; // object num
         int typ = 0;
         if (obt == 2) typ = mItem.item[num][0];
         if (obt == 3) typ = mEnemy.Ei[num][0];
         for (int i=0; i<100; i++) // iterate control mapping table
            if (cot[i][obt][typ] == 0) ge_data[i].valid = 0; // remove controls that do not match object
      }
}


void mwWindow::ge_clear_obj_list(void)
{
   for (int o=0; o<NUM_OBJ; o++) mWM.obj_list[o][0] = 0;
}

void mwWindow::ge_collapse_all_controls(void)
{
   for (int i=0; i<100; i++) if (ge_data[i].vartyp) ge_data[i].collapsed = 1;
}

void mwWindow::ge_expand_all_controls(void)
{
   for (int i=0; i<100; i++) if (ge_data[i].vartyp) ge_data[i].collapsed = 0;
}



void mwWindow::ge_add_to_obj_list(int t, int a)
{
   // check if already in list
   int duplicate = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if ((mWM.obj_list[i][0] == t) && (mWM.obj_list[i][1] == a)) duplicate = 1;
   if (!duplicate)
   {
      for (int i=0; i<NUM_OBJ; i++)
         if (mWM.obj_list[i][0] == 0)
         {
            mWM.obj_list[i][0] = t;
            mWM.obj_list[i][1] = a;
            i = NUM_OBJ;
         }
   }
}

void mwWindow::ge_remove_obj_list_item(int a)
{
   // slide down to close hole
   for (int i=a; i<NUM_OBJ-1; i++)
   {
      mWM.obj_list[i][0] = mWM.obj_list[i+1][0];
      mWM.obj_list[i][1] = mWM.obj_list[i+1][1];
      mWM.obj_list[i][2] = mWM.obj_list[i+1][2];
   }
}

void mwWindow::ge_remove_obj_list_filtered_items(void)
{
   for (int i=0; i<NUM_OBJ; i++)
      if (mWM.obj_list[i][0])
      {
         int type = mWM.obj_list[i][0];
         int num = mWM.obj_list[i][1];
         if ((type == 2) && (mWM.obj_filter[type][mItem.item[num][0]] == 0)) ge_remove_obj_list_item(i);
         if ((type == 3) && (mWM.obj_filter[type][ mEnemy.Ei[num][0]] == 0)) ge_remove_obj_list_item(i);
      }
}

void mwWindow::ge_swap_obj_list_items(int i1, int i2)
{
   int t0 = mWM.obj_list[i1][0];
   int t1 = mWM.obj_list[i1][1];
   int t2 = mWM.obj_list[i1][2];

   mWM.obj_list[i1][0] = mWM.obj_list[i2][0];
   mWM.obj_list[i1][1] = mWM.obj_list[i2][1];
   mWM.obj_list[i1][2] = mWM.obj_list[i2][2];

   mWM.obj_list[i2][0] = t0;
   mWM.obj_list[i2][1] = t1;
   mWM.obj_list[i2][2] = t2;
}

void mwWindow::ge_object_initial_position_random(int typ, int num)
{
   int x, y, empt = 0, lb = 0;
   while ((!empt) && (lb < 10000))
   {
      lb++;
      x = rand() % 100;
      y = rand() % 100;
      empt = 1;
      if ((x < mWM.bx1) || (x >= mWM.bx2+1)) empt = 0;
      if ((y < mWM.by1) || (y >= mWM.by2+1)) empt = 0;
      if (!mLevel.is_block_empty(x, y, 1, 1, 1)) empt = 0;
   }
   if (empt)
   {
      if (typ == 2)
      {
         mItem.itemf[num][0] = mItem.item[num][4] = x*20;
         mItem.itemf[num][1] = mItem.item[num][5] = y*20;
      }
      if (typ == 3)
      {
         mEnemy.Ef[num][0] = x*20;
         mEnemy.Ef[num][1] = y*20;
      }
   }
   else printf("could not find empty space for object\n");
}

int mwWindow::ge_draw_list_items(int x1, int y1, int ni)
{
   int xt = x1; // to keep track of pos as we expand in the x axis
   if (ni == 0)
   {
      al_draw_text(mFont.pr8, mColor.pc[15], xt, y1, 0, "<empty>");
      xt+=56;
   }
   else
   {
      // column headers
      al_draw_text(mFont.pr8, mColor.pc[15], xt, y1, 0, " #");  xt+=24;
      al_draw_text(mFont.pr8, mColor.pc[15], xt, y1, 0, "S");   xt+=16;
      al_draw_text(mFont.pr8, mColor.pc[15], xt, y1, 0, "num"); xt+=32;
      for (int ge_num=0; ge_num<100; ge_num++)
         if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
         {
            al_draw_textf(mFont.pr8, mColor.pc[15], xt, y1, 0, "%.4s", ge_data[ge_num].snam);
            xt+=40;
         }

      // rows
      for (int i=0; i<NUM_OBJ; i++)
         if (mWM.obj_list[i][0])
         {
            y1+=8;
            int type = mWM.obj_list[i][0];
            int num = mWM.obj_list[i][1];
            int col = 15;
            if (mWM.obj_list[i][2]) col = mColor.flash_color;

            xt = x1; // to keep track of pos as we expand in the x axis

            al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",i); xt+=24; // list item number

            int tn = 0;
            if (type == 2) tn = mItem.item_tile[mItem.item[num][0]];
            if (type == 3) tn = mEnemy.enemy_tile[mEnemy.Ei[num][0]];
            al_draw_scaled_bitmap(mBitmap.tile[tn], 0, 0, 20, 20, xt, y1, 8, 8, 0); xt+=16; // bitmap

            al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%3d", num); xt+=32; // object number

            // valid variables
            for (int ge_num=0; ge_num<100; ge_num++)
               if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
                  {
                     int vt = ge_data[ge_num].vartyp;
                     int vn = ge_data[ge_num].varnum;

                     if (vt == 2) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",   mItem.item[num][vn]);
                     if (vt == 3) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",   mEnemy.Ei[num][vn]);
                     if (vt == 4) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2.1f", mEnemy.Ef[num][vn]);
                     //if (vt == 5) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",   mEnemy.Ei[num][2]);

                     if (vt == 7)  // vinepod flag
                     {
                        if (mEnemy.Ei[num][20] & vn) al_draw_text(mFont.pr8, mColor.pc[col], xt, y1, 0, "on");
                        else                         al_draw_text(mFont.pr8, mColor.pc[col], xt, y1, 0, "off");
                     }

                     if (vt == 8) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",   mItem.item[num][6] + mItem.item[num][8]/2); // secondary x center
                     if (vt == 9) al_draw_textf(mFont.pr8, mColor.pc[col], xt, y1, 0, "%2d",   mItem.item[num][7] + mItem.item[num][9]);   // secondary y2

                     xt+=40;
                  }
         }
   }
   return xt;
}

void mwWindow::ge_show_obj_list(int x, int y, int *ew, int *eh, int d)
{
   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (mWM.obj_list[i][0]) ni++;

   int fs = 12;
   int y1 = y + fs;

   int x1 = x+fs;

   int yf1 = y;
   int yf2 = y + (ni+1)*8 + fs*2;
   int x2 = ge_draw_list_items(x1, y1, ni);
   if (x2 < x1+180) x2=x1+180; // minimum size

   *ew = (x2+fs) - x ; // actual width
   *eh = yf2 - y;      // actual height

   // draw frame around list
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1-fs+q, yf1+q, x2+fs-q, yf2-q, mColor.pc[12+32+(q*ci)], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], (x1+x2)/2, yf1+2, ALLEGRO_ALIGN_CENTER, "Group Edit Object List");
//   al_draw_textf(mFont.pr8, mColor.pc[15], x2-24, yf1+2,                    0, "%2d", ni);
   if ((!d) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > yf1) && (mInput.mouse_y < yf2)) // detect if mouse is on list
   {
      for (int o=0; o<NUM_OBJ; o++) mWM.obj_list[o][2] = 0; // clear all highlights
      int mpl = ((mInput.mouse_y - yf1 + fs)/8)-4;             // get raw list item
      if ((mpl < -1) || (mpl > ni-1)) mpl = -1;         // ensure valid list item
      if (mpl != -1)                                    // mouse is on valid list item
      {
         mWM.obj_list[mpl][2] = 1;                          // mark this item as highlighted
         al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, mColor.pc[10], 1); // draw rectangle around list item
         if (mInput.mouse_b[1][0])
         {
            if (mInput.SHFT()) // remove item from list
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               ge_remove_obj_list_item(mpl);
            }
            else // keep mouse for drag swap
            {
               int old_mpl = mpl;
               while (mInput.mouse_b[1][0])
               {
                  mWM.redraw_level_editor_background();
                  mScreen.draw_scaled_level_region_to_display(1);
                  mWM.cycle_windows(1); // draw only

                  mpl = ((mInput.mouse_y - yf1 + fs)/8)-4;             // get raw list item
                  if ((mpl < -1) || (mpl > ni-1)) mpl = -1;     // ensure valid list item
                  if (mpl != -1) al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, mColor.pc[10], 1); // if mouse is on valid list item, draw rectangle around list item pointer
               }
               if (mpl != -1) ge_swap_obj_list_items(old_mpl, mpl); // if mouse is on valid list item, do the swap
            } // mouse b1 held
         } // mInput.mouse_b[1][0] pressed
      } // mouse on valid list item
   } // mouse on obj list
}





void mwWindow::ge_set_var(int gvt, int gvn, int num, float val)
{
   if (gvt == 2) mItem.item[num][gvn] = val;
   if (gvt == 3) mEnemy.Ei[num][gvn]  = val;
   if (gvt == 4)
   {
      mEnemy.Ef[num][gvn] = val;
      //if (gvn == 9) mEnemy.recalc_pod(num);
      if ((gvn == 5) && ((mEnemy.Ei[num][0] == 4) || (mEnemy.Ei[num][0] == 6))) mEnemy.scale_bouncer_and_cannon_speed(num); // cannon and bouncer speed only
   }
   if (gvt == 8) mItem.item[num][6] = val - mItem.item[num][8]/2; // given xc, set x1 (w is different for every obj) // x1 = center - 1/2 width
   if (gvt == 9) mItem.item[num][7] = val - mItem.item[num][9];   // given y2, set y1 (h is different for every obj) // y1 = y2 - height
}

float mwWindow::ge_get_var(int gvt, int gvn, int num)
{
   float val = 0;
   if (gvt == 2) val = mItem.item[num][gvn];
   if (gvt == 3) val = mEnemy.Ei[num][gvn];
   if (gvt == 4) val = mEnemy.Ef[num][gvn];
   if (gvt == 8) val = mItem.item[num][6] + mItem.item[num][8]/2; // xc = x + w/2
   if (gvt == 9) val = mItem.item[num][7] + mItem.item[num][9];   // y2 = y1 + h
   return val;
}



int mwWindow::ge_show_controls(int x, int y, int *ew, int *eh, int hidden, int d)
{
   char msg[1024];
   int nc = 0; // number of valid controls
   int by = y;
   int bts = 16;
   int y_spacing = 2; // spacing between groups

   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (mWM.obj_list[i][0]) ni++;

   if (ni)
   {
      ge_set_valid_controls();

      for (int ge_num=0; ge_num<100; ge_num++)
         if (ge_data[ge_num].valid)
         {
            nc++;
            int gvt = ge_data[ge_num].vartyp;
            int gvn = ge_data[ge_num].varnum;
            int xa = x;

            int x1 = xa;
            int x2 = x1 + 320;
            int x3 = x2 - 60;
            int x4 = x1 + 160;

            int fy1 = by-2; // for frame drawing
            int fy2 = by + bts; // for frame drawing

            // get frame size
            if (!ge_data[ge_num].collapsed)
            {
               if ((gvt == 2) || (gvt == 3) || (gvt == 4) || (gvt == 8) || (gvt == 9)) fy2 += bts * 3;
               else fy2 += bts;
            }


            // frame control section
            int fx1 = x1+1;
            int fx2 = x2+1;
            int fcol = 13;
            if ((mInput.mouse_x > fx1) && (mInput.mouse_x < fx2) && (mInput.mouse_y > fy1) && (mInput.mouse_y < fy2)) fcol = 10;

            al_draw_filled_rectangle(fx1, fy1, fx2, fy2, mColor.pc[fcol+208]);
            al_draw_rectangle(fx1, fy1, fx2, fy2, mColor.pc[fcol], 1);

            sprintf(msg, "%s", ge_data[ge_num].name);
            //adjust button size to text
            int sl = (strlen(msg)+2)*4;
            int tx1 = x4 - sl + 3;
            int tx2 = x4 + sl - 1;
            mWidget.toggle(tx1, by, tx2, bts, 0,0,0,0, 0,0,0,0, 1,0,0,d, ge_data[ge_num].collapsed, msg, msg,  15, 15, 12, 12);
            mWidget.toggle(x2-12, by, x2-4, bts, 0,0,0,0, 0,-1,0,0, 1,0,1,d, ge_data[ge_num].collapsed, "-", "+",  15, 15, -1, -1);

            if (!ge_data[ge_num].collapsed)
            {
               if ((gvt == 2) || (gvt == 3) || (gvt == 4) || (gvt == 8) || (gvt == 9))
               {
                  // find min max and average
                  float mins = 9999;
                  float maxs = -9999;
                  float tally = 0;

                  for (int i=0; i<NUM_OBJ; i++)
                     if (mWM.obj_list[i][0])
                     {
                        float val = ge_get_var(gvt, gvn, mWM.obj_list[i][1]);
                        if (val < mins) mins = val;
                        if (val > maxs) maxs = val;
                        tally += val;
                     }
                  float avg = tally / (float) ni;

                  //sprintf(msg, "Current - min:%2.1f max:%2.1f avg:%2.1f", mins, maxs, avg);
                  sprintf(msg, "min:%2.1f max:%2.1f avg:%2.1f", mins, maxs, avg);
                  mWidget.buttont(x1+6, by, x2-4, bts, 0,0,0,0,  0,13,15,0,1,0,1,d, msg);

                  float mna = ge_data[ge_num].min_allowed;
                  float mxa = ge_data[ge_num].max_allowed;

                  mWidget.sliderf(x1+6,     by, x4-1,   bts,   0,0,0,0,  0,3,15,0,1,0,0,d, ge_data[ge_num].adj_min, mxa, mna, .1, "Min:");
                  mWidget.sliderf(x4+1,       by, x2-4, bts,   0,0,0,0,  0,3,15,0,1,0,1,d, ge_data[ge_num].adj_max, mxa, mna, .1, "Max:");

                  mWidget.buttonp(    x1+6, by, x3-1, bts, 100,0,0,0,  0, 4,15,0,1,1,0,d, ge_data[ge_num].adj_mode); // Action type


                  if (mWidget.buttont(x3+1,   by, x2-4, bts,   0,0,0,0,  0,10,15,0,1,0,1,d, "Do It!"))
                  {
                     float mn = ge_data[ge_num].adj_min;
                     float mx = ge_data[ge_num].adj_max;
                     if (ge_data[ge_num].adj_mode == 0) // randomize
                     {
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mWM.obj_list[i][0])
                              ge_set_var(gvt, gvn, mWM.obj_list[i][1], mMiscFnx.mdw_rnd(mn, mx));
                     }
                     if (ge_data[ge_num].adj_mode == 1) // increment
                     {
                        float inc = (mx-mn)/ni;
                        float val = mn;
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mWM.obj_list[i][0])
                           {
                              ge_set_var(gvt, gvn, mWM.obj_list[i][1], val);
                              val += inc;
                           }
                     }
                     if (ge_data[ge_num].adj_mode == 2) // set all to min
                     {
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mWM.obj_list[i][0])
                              ge_set_var(gvt, gvn, mWM.obj_list[i][1], mn);
                     }
                     if (ge_data[ge_num].adj_mode == 3) // set all to first obj in list
                     {
                        float val = ge_get_var(gvt, gvn, mWM.obj_list[0][1]);
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mWM.obj_list[i][0])
                              ge_set_var(gvt, gvn, mWM.obj_list[i][1], val);
                     }


                  } // end of Do It!
               } // end of types 2, 3, 4
               else if (gvt == 5) // initial direction
               {
                  if (mWidget.buttont(x1+10, by, x4-30, bts, 0,0,0,0,  0,10,15,0,1,0,0,d, "Random"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mWM.obj_list[i][0])
                        {
                           int num = mWM.obj_list[i][1];
                           mEnemy.set_new_initial_direction(num, rand() % 2000, rand() % 2000); // random
                        }
                  }
                  if (mWidget.buttont(x4-28, by, x2, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "Aim at Start Block"))
                  {
                     int sbx = 0, sby = 0; // start block x and y
                     for (int c=0; c<500; c++)
                        if (mItem.item[c][0] == 5)
                        {
                           sbx = mItem.item[c][4];
                           sby = mItem.item[c][5];
                        }
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mWM.obj_list[i][0])
                        {
                           int num = mWM.obj_list[i][1];
                           mEnemy.set_new_initial_direction(num, sbx, sby); // point at start block
                        }
                  } // end of aim at start block
               } // end of type 5
               else if (gvt == 6) // initial position
               {
                  if (mWidget.buttont(x1+10, by, x2, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "Randomize Position Within Selection"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mWM.obj_list[i][0])
                        {
                           int typ = mWM.obj_list[i][0];
                           int num = mWM.obj_list[i][1];
                           ge_object_initial_position_random(typ, num);
                        }
                  }
               }
               else if (gvt == 7) // vinepod flags
               {
                  int fx3 = x1 + 100;
                  int fx4 = x1 + 200;
                  if (mWidget.buttont(x1+6, by, fx3-4, bts, 0,0,0,0,  0,10,15,0,1,0,0,d, "all off"))
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mWM.obj_list[i][0]) mEnemy.Ei[mWM.obj_list[i][1]][20] &= ~ gvn;
                  if (mWidget.buttont(fx3+4, by, fx4-4, bts, 0,0,0,0,  0,10,15,0,1,0,0,d, "all on"))
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mWM.obj_list[i][0]) mEnemy.Ei[mWM.obj_list[i][1]][20] |= gvn;
                  if (mWidget.buttont(fx4+4, by, x2-4, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "random"))
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                     {
                        if (mMiscFnx.mdw_rnd(0, 10) > 5) mEnemy.Ei[mWM.obj_list[i][1]][20] |= gvn;
                        else                             mEnemy.Ei[mWM.obj_list[i][1]][20] &= ~ gvn;
                     }
               }




            } // end of not collapsed
            by+=y_spacing+1;
         } // end of valid ge_num
   } // end of more than 0 items in list

   *ew = 322; // fixed width for now
   *eh = by-y-y_spacing;
   return nc;
}

void mwWindow::ge_add_selection_to_list(int set_filters)
{
   // add everything in selection to list...
   int rx1 = mWM.bx1*20;
   int ry1 = mWM.by1*20;
   int rx2 = mWM.bx2*20+20;
   int ry2 = mWM.by2*20+20;

   for (int b=0; b<100; b++) // add enemies in selection
      if ((mEnemy.Ei[b][0]) && (mEnemy.Ef[b][0] >= rx1) && (mEnemy.Ef[b][0] < rx2) && (mEnemy.Ef[b][1] >= ry1) && (mEnemy.Ef[b][1] < ry2)) ge_add_to_obj_list(3, b);

   for (int b=0; b<500; b++) // add items in selection
      if ((mItem.item[b][0]) && (mItem.item[b][4] >= rx1) && (mItem.item[b][4] < rx2) && (mItem.item[b][5] >= ry1) && (mItem.item[b][5] < ry2)) ge_add_to_obj_list(2, b);

   // set filters to what is actually in the list
   if (set_filters)
   {
      for (int i=0; i<NUM_OBJ; i++)
      if (mWM.obj_list[i][0])
      {
         int typ = mWM.obj_list[i][0];
         int num = mWM.obj_list[i][1];
         if (typ == 2) mWM.obj_filter[typ][mItem.item[num][0]] = 1;
         if (typ == 3) mWM.obj_filter[typ][ mEnemy.Ei[num][0]] = 1;
      }
   }
}

void mwWindow::ge_process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      if (mWM.mW[5].show_sel_frame) // get new selection rectangle
      {
         mWM.get_new_box();
         if (mInput.SHFT()) ge_add_selection_to_list(1); // add everything in selection to list and set filters...
      }
      else
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

         // is mouse on item
         for (int i=0; i<500; i++)
         {
            int type = mItem.item[i][0];
            if ((type) && (mWM.obj_filter[2][type])) // filter for this type of item
            {
               if ((mWM.gx == mItem.item[i][4]/20) && (mWM.gy == mItem.item[i][5]/20)) ge_add_to_obj_list(2, i);
            }
         }
         // is mouse on enemy
         for (int e=0; e<100; e++)
         {
            int type = mEnemy.Ei[e][0];
            if ((type) && (mWM.obj_filter[3][type])) // filter for this type of enemy
            {
               if ((mWM.gx == mEnemy.Ef[e][0]/20) && (mWM.gy == mEnemy.Ef[e][1]/20)) ge_add_to_obj_list(3, e);
            }
         }
      }
   }
   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mWM.set_windows(1);
   }
}
