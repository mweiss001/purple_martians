// mwTriggerEvent.cpp

#include "pm.h"
#include "mwTriggerEvent.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwLift.h"
#include "mwDisplay.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwWindowManager.h"
#include "mwScreen.h"
#include "mwMiscFnx.h"
#include "mwLoop.h"



mwTriggerEvent mTriggerEvent;


mwTriggerEvent::mwTriggerEvent()
{
   initialize();
}

void mwTriggerEvent::initialize(void)
{
   for (int x=0; x<1000; x++) event[x] = 0;
}


void mwTriggerEvent::show_event_line(int x, int &y, int ev, int type, int v1, int v2)
{
   if (type == 2) // item
   {
      if (mItem.item[v1][0] == 6)  al_draw_textf(mFont.pr8, mColor.pc[7],  x, y, 0, "ev:%2d - item:%3d [orb] ", ev, v1);
      if (mItem.item[v1][0] == 9)  al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "ev:%2d - item:%3d [trg] ", ev, v1);
      if (mItem.item[v1][0] == 10) al_draw_textf(mFont.pr8, mColor.pc[11], x, y, 0, "ev:%2d - item:%3d [msg] ", ev, v1);
      if (mItem.item[v1][0] == 16) al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "ev:%2d - item:%3d [bm]  ", ev, v1);
      if (mItem.item[v1][0] == 17) al_draw_textf(mFont.pr8, mColor.pc[10], x, y, 0, "ev:%2d - item:%3d [bd]  ", ev, v1);

      if ((mItem.item[v1][0] == 13) && (v2 == 1)) al_draw_textf(mFont.pr8, mColor.pc[10], x, y, 0, "ev:%2d - item:%3d [t1 i/p]  ", ev, v1);
      if ((mItem.item[v1][0] == 13) && (v2 == 2)) al_draw_textf(mFont.pr8, mColor.pc[10], x, y, 0, "ev:%2d - item:%3d [t1 o/p]  ", ev, v1);
      if ((mItem.item[v1][0] == 13) && (v2 == 3)) al_draw_textf(mFont.pr8, mColor.pc[10], x, y, 0, "ev:%2d - item:%3d [t2 i/p]  ", ev, v1);
      if ((mItem.item[v1][0] == 13) && (v2 == 4)) al_draw_textf(mFont.pr8, mColor.pc[10], x, y, 0, "ev:%2d - item:%3d [t2 o/p]  ", ev, v1);
   }
   if (type == 3) // enemy
   {
      if (mEnemy.Ei[v1][0] == 9)                al_draw_textf(mFont.pr8, mColor.pc[7],  x, y, 0, "ev:%2d - enem:%3d [clonr] ", ev, v1);
      if ((mEnemy.Ei[v1][0] == 7) && (v2 == 0)) al_draw_textf(mFont.pr8, mColor.pc[7],  x, y, 0, "ev:%2d - enem:%3d [vp i/p] ", ev, v1);
      if ((mEnemy.Ei[v1][0] == 7) && (v2 == 1)) al_draw_textf(mFont.pr8, mColor.pc[7],  x, y, 0, "ev:%2d - enem:%3d [vp o/p] ", ev, v1);
   }

   if (type == 4) // lift
   {
      al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "ev:%2d - lift:%3d step:%d", ev, v1, v2);
   }
   y+=8;
}


void mwTriggerEvent::show_all_events(void)
{
   int x = 0, y = 20;
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_textf(mFont.pr8, mColor.pc[15], 0, 0, 0, "All non-zero event references");

   for (int i=0; i<500; i++)
   {
      if (mItem.item[i][0] == 6) // orb
      {
         if (mItem.item[i][10]) show_event_line(x, y, mItem.item[i][10], 2, i, 0);
         if (mItem.item[i][11]) show_event_line(x, y, mItem.item[i][11], 2, i, 0);
         if (mItem.item[i][12]) show_event_line(x, y, mItem.item[i][12], 2, i, 0);
         if (mItem.item[i][13]) show_event_line(x, y, mItem.item[i][13], 2, i, 0);
      }
      if (mItem.item[i][0] == 9) // trigger
      {
         if (mItem.item[i][11]) show_event_line(x, y, mItem.item[i][11], 2, i, 0);
         if (mItem.item[i][12]) show_event_line(x, y, mItem.item[i][12], 2, i, 0);
         if (mItem.item[i][13]) show_event_line(x, y, mItem.item[i][13], 2, i, 0);
         if (mItem.item[i][14]) show_event_line(x, y, mItem.item[i][14], 2, i, 0);
      }
      if (mItem.item[i][0] == 13) // timer
      {
         if (mItem.item[i][12]) show_event_line(x, y, mItem.item[i][12], 2, i, 1); // t1 i/p
         if (mItem.item[i][13]) show_event_line(x, y, mItem.item[i][13], 2, i, 2); // t1 o/p
         if (mItem.item[i][14]) show_event_line(x, y, mItem.item[i][14], 2, i, 3); // t2 i/p
         if (mItem.item[i][15]) show_event_line(x, y, mItem.item[i][15], 2, i, 4); // t2 o/p

      }
      if ((mItem.item[i][0] == 10) && (mItem.item[i][1])) show_event_line(x, y, mItem.item[i][1], 2, i, 0); // message
      if ((mItem.item[i][0] == 16) && (mItem.item[i][1])) show_event_line(x, y, mItem.item[i][1], 2, i, 0); // block manip
      if ((mItem.item[i][0] == 17) && (mItem.item[i][1])) show_event_line(x, y, mItem.item[i][1], 2, i, 0); // block damage
   }

   for (int l=0; l<NUM_LIFTS; l++) // lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // steps
            if ((((mLift.stp[l][s].type & 31) == 5) || ((mLift.stp[l][s].type & 31) == 6)) && (mLift.stp[l][s].val)) show_event_line(x, y, mLift.stp[l][s].val, 4, l, s);

   for (int e=0; e<100; e++) // enemies
   {
      if (mEnemy.Ei[e][0] == 9) // cloner
         if (mEnemy.Ei[e][8]) show_event_line(x, y, mEnemy.Ei[e][8], 3, e, 0);

      if (mEnemy.Ei[e][0] == 7) // vinepod
      {
         if (mEnemy.Ei[e][18]) show_event_line(x, y, mEnemy.Ei[e][18], 3, e, 0);
         if (mEnemy.Ei[e][19]) show_event_line(x, y, mEnemy.Ei[e][19], 3, e, 1);
      }
   }
   al_flip_display();
   mInput.tsw(); // wait for keypress
}


// check the clt list for an existing translation from a source event
// return the destination translation used or 0 if no translation
int mwTriggerEvent::check_clt_for_source_event(int ev, int clt[][5], int clt_last)
{
   for (int i=0; i<clt_last; i++)
      if (clt[i][3] == ev) return clt[i][4];
   return 0;
}

int mwTriggerEvent::add_item_link_translation(int obt, int num, int ext, int src_ev, int clt[][5], int clt_last)
{
   if (src_ev) // do nothing for event zero
   {
      // check if this event already has a translation and get it if it does
      int dst_ev = check_clt_for_source_event(src_ev, clt, clt_last);
      if (!dst_ev) dst_ev = get_unused_pm_event_extended(clt, clt_last); // no existing translation found
      clt[clt_last][0] = obt;    // object type
      clt[clt_last][1] = num;    // item # in selection
      clt[clt_last][2] = ext;    // item var #
      clt[clt_last][3] = src_ev;
      clt[clt_last][4] = dst_ev;
      return 1;
   }
   return 0; // nothing added
}


// checks all objects for a reference to the passed event number
int mwTriggerEvent::is_pm_event_used(int ev)
{
   for (int i=0; i<500; i++) // iterate items
   {
      if (mItem.item[i][0] == 6) // orb
      {
         if (mItem.item[i][10] == ev) return 1;
         if (mItem.item[i][11] == ev) return 1;
         if (mItem.item[i][12] == ev) return 1;
         if (mItem.item[i][13] == ev) return 1;
      }
      if (mItem.item[i][0] == 9) // trigger
      {
         if (mItem.item[i][11] == ev) return 1;
         if (mItem.item[i][12] == ev) return 1;
         if (mItem.item[i][13] == ev) return 1;
         if (mItem.item[i][14] == ev) return 1;
      }
      if (mItem.item[i][0] == 13) // timer
      {
         if (mItem.item[i][12] == ev) return 1;
         if (mItem.item[i][13] == ev) return 1;
         if (mItem.item[i][14] == ev) return 1;
         if (mItem.item[i][15] == ev) return 1;
      }

      if ((mItem.item[i][0] == 10) && (mItem.item[i][1] == ev)) return 1; // message
      if ((mItem.item[i][0] == 16) && (mItem.item[i][1] == ev)) return 1; // bm
      if ((mItem.item[i][0] == 17) && (mItem.item[i][1] == ev)) return 1; // bd
   }

   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            if (((((mLift.stp[l][s].type & 31) == 5) || (mLift.stp[l][s].type & 31) == 6)) && (mLift.stp[l][s].val == ev)) return 1;

   for (int e=0; e<100; e++) // iterate enemies
   {
      if (mEnemy.Ei[e][0] == 9) // cloner
         if (mEnemy.Ei[e][8] == ev) return 1;

      if (mEnemy.Ei[e][0] == 7) // vinepod
         if ((mEnemy.Ei[e][18] == ev) || (mEnemy.Ei[e][19] == ev)) return 1;

   }
   return 0;
}

// checks objects and translation table
int mwTriggerEvent::get_unused_pm_event_extended(int clt[][5], int clt_last)
{
   int ev = 1; // starting event to test (don't ever use event 0)
   int done = 0;

   while (!done)
   {
      int used = 0;

      if (is_pm_event_used(ev)) used = 1; // first check all object that use events

      for (int i=0; i<clt_last; i++) // then also check the translation table
         if ((clt[i][3] == ev) || (clt[i][4] == ev)) used = 1;

      if (used == 0) return ev; // found one!
      else if (++ev > 999) done = 1;
   }
   return 0; // only if no unused can be found
}

// only checks objects
int mwTriggerEvent::get_unused_pm_event(void)
{
   int ev = 1; // don't ever use event 0
   int done = 0;
   while (!done)
   {
      if (!is_pm_event_used(ev)) return ev;
      else if (++ev > 999) done = 1;
   }
   return 0; // only if no unused can be found
}



void mwTriggerEvent::find_event_txrs_for_event(int ev, int &evan, int eva[][2])
/*
   finds all event transmitters that match ev
   fills array eva with x and x positions
*/
{
   for (int i=0; i<500; i++)
   {
      // trigger
      if ((mItem.item[i][0] == 9) && ((mItem.item[i][11] == ev) || (mItem.item[i][12] == ev) || (mItem.item[i][13] == ev) || (mItem.item[i][14] == ev)))
      {
         eva[evan][0] = mItem.item[i][4]+10;
         eva[evan][1] = mItem.item[i][5]+10;
         evan++;
      }

      // orb
      if ((mItem.item[i][0] == 6) && ((mItem.item[i][10] == ev) || (mItem.item[i][11] == ev) || (mItem.item[i][12] == ev) || (mItem.item[i][13] == ev)))
      {
         eva[evan][0] = mItem.item[i][4]+10;
         eva[evan][1] = mItem.item[i][5]+10;
         evan++;
      }

      // timer
      if ((mItem.item[i][0] == 13) && ((mItem.item[i][13] == ev) || (mItem.item[i][15] == ev)))
      {
         eva[evan][0] = mItem.item[i][4]+10;
         eva[evan][1] = mItem.item[i][5]+10;
         evan++;
      }
   }

   for (int e=0; e<100; e++) // iterate enemies
      if ((mEnemy.Ei[e][0] == 7) && (mEnemy.Ei[e][19] == ev)) // vinepod
      {
         eva[evan][0] = mEnemy.Ei[e][9] +10;
         eva[evan][1] = mEnemy.Ei[e][10]+10;
         evan++;
      }

   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            if (((mLift.stp[l][s].type & 31) == 6) && (mLift.stp[l][s].val == ev))
            {
               //printf("found a match with a lift:%d step:%d ", l, s);
               int pms = mLift.find_previous_move_step(l, s);
               eva[evan][0] = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
               eva[evan][1] = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;
               evan++;
            }
}


void mwTriggerEvent::find_event_rxrs_for_event(int ev, int &evan, int eva[][2])
/*
   finds all event receivers that match ev
   fills array eva with x and x positions
*/
{
   for (int i=0; i<500; i++)
   {
      // message, manip or damage
      if (((mItem.item[i][0] == 10) || (mItem.item[i][0] == 16) || (mItem.item[i][0] == 17)) && (mItem.item[i][1] == ev))
      {
         eva[evan][0] = mItem.item[i][4]+10;
         eva[evan][1] = mItem.item[i][5]+10;
         evan++;
      }

      // timer
      if ((mItem.item[i][0] == 13) && ((mItem.item[i][12] == ev) || (mItem.item[i][14] == ev)) )
      {
         eva[evan][0] = mItem.item[i][4]+10;
         eva[evan][1] = mItem.item[i][5]+10;
         evan++;
      }
   }

   for (int e=0; e<100; e++)
   {
      if ((mEnemy.Ei[e][0] == 9) && (mEnemy.Ei[e][8] == ev)) // cloner
      {
         eva[evan][0] = mEnemy.Ef[e][0]+10;
         eva[evan][1] = mEnemy.Ef[e][1]+10;
         evan++;
      }
      if ((mEnemy.Ei[e][0] == 7) && (mEnemy.Ei[e][18] == ev)) // vinepod
      {
         eva[evan][0] = mEnemy.Ef[e][0]+10;
         eva[evan][1] = mEnemy.Ef[e][1]+10;
         evan++;
      }
   }

   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            if (((mLift.stp[l][s].type & 31) == 5) && (mLift.stp[l][s].val == ev))
            {
               //printf("found a match with a lift:%d step:%d ", l, s);
               int pms = mLift.find_previous_move_step(l, s);
               eva[evan][0] = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
               eva[evan][1] = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;
               evan++;
            }
}

void mwTriggerEvent::find_and_show_event_links(int obj_type, int obj_num, int obj_ext)
{
   int eva[100][2] = { 0 };
   int evan = 0;
   int ev = 0;

   if (obj_type == 2) // items
   {
      int i = obj_num; // item index
      int itype = mItem.item[i][0];

      int x1 = mItem.item[i][4]+10;
      int y1 = mItem.item[i][5]+10;


      if (itype == 6) // orb
      {
         for (int en=10; en<14; en++) // iterate all 4 trigger events
         {
            ev = mItem.item[obj_num][en];
            if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);
         }
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }

      if (itype == 9) // trigger
      {
         for (int en=11; en<15; en++) // iterate all 4 trigger events
         {
            ev = mItem.item[obj_num][en];
            if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);
         }
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }
      if ((itype == 10) || (itype == 16) || (itype == 17))  // message, block manip or block damage
      {
         ev = mItem.item[i][1];
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }
      if (itype == 13)  // timer
      {
         ev = mItem.item[i][12]; // input links for timer 1
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);

         ev = mItem.item[i][14]; // input links for timer 2
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);

         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);

         evan = 0;
         ev = mItem.item[i][13]; // output links for timer 1
         if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);

         ev = mItem.item[i][15]; // output links for timer 2
         if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);

         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[11], 2);
      }
   }
   if (obj_type == 3)  // enemy
   {
      int e = obj_num; // item index
      int etype = mEnemy.Ei[e][0];

      int x1 = mEnemy.Ef[e][0]+10;
      int y1 = mEnemy.Ef[e][1]+10;

      if (etype == 9) // cloner
      {
         ev = mEnemy.Ei[e][8];
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }

      if (etype == 7) // vinepod input
      {
         ev = mEnemy.Ei[e][18];
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }


      if (etype == 7) // vinepod output
      {
         ev = mEnemy.Ei[e][19];
         if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }
   }

   if (obj_type == 4)  // lift step
   {
      int l = obj_num;
      int s = obj_ext;
      int stype = mLift.stp[l][s].type & 31;

      if (stype == 5) // step type "wait for trig"
      {
         // get position from previous move step
         int pms = mLift.find_previous_move_step(l, s);
         int x1 = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
         int y1 = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;

         ev = mLift.stp[l][s].val; // get event
         if (ev > 0) find_event_txrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);

      }
      if (stype == 6) // step type "send event"
      {
         // get position from previous move step
         int pms = mLift.find_previous_move_step(l, s);
         int x1 = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
         int y1 = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;

         ev = mLift.stp[l][s].val; // get event
         if (ev > 0) find_event_rxrs_for_event(ev, evan, eva);
         for (int i2=0; i2<evan; i2++)
            al_draw_line(x1, y1, eva[i2][0], eva[i2][1], mColor.pc[10], 2);
      }
   }
}



int mwTriggerEvent::is_mouse_on_trigger_item(int &obj_type, int &obj_num, int &obj_ext)
/*
// things that emit events
works for items (trigger, orb, timer) and lift step type 6
and vinepod now

*/
{
   int mouse_on_item = 0;

   for (int x=0; x<500; x++)
      if ((mItem.item[x][0] == 9) || (mItem.item[x][0] == 6) || (mItem.item[x][0] == 13))  // trigger, orb or timer
      {
         if ((mWM.gx == mItem.item[x][4]/20) && (mWM.gy == mItem.item[x][5]/20))
         {
            mouse_on_item = 1;
            obj_type = 2;
            obj_num = x;
            if (mItem.item[x][0] == 13)  // for timer, determine timer num based on mouse y position
            {
               int my = mWM.hy % 20;
               if (my < 10) obj_ext = 1;
               else obj_ext = 2;
            }
         }
      }

   for (int e=0; e<100; e++)
      if (mEnemy.Ei[e][0] == 7) // vinepod (extended pos)
      {
         if ((mWM.gx == mEnemy.Ei[e][9]/20) && (mWM.gy == mEnemy.Ei[e][10]/20))
         {
            mouse_on_item = 1;
            obj_type = 3;
            obj_num = e;
         }
      }

   for (int l=0; l<NUM_LIFTS; l++)
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++)
            if ((mLift.stp[l][s].type & 31) == 6)  // only step type 6 - send trigger event
            {
               // get previous move step
               int pms = mLift.find_previous_move_step(l, s);

               // is mouse is on step
               int x1 = mLift.stp[l][pms].x;
               int y1 = mLift.stp[l][pms].y;
               int x2 = mLift.stp[l][pms].w + x1;
               int y2 = mLift.stp[l][pms].h + y1;
               if ((mWM.hx > x1)  && (mWM.hx < x2) && (mWM.hy > y1)  && (mWM.hy < y2))
               {

                  mLift.set_lift_to_step(l, pms);

                  mouse_on_item = 1;
                  obj_type = 4;
                  obj_ext = l;
                  obj_num = s;
               }
            }

   return mouse_on_item;
}


int mwTriggerEvent::get_trigger_item(int obj_type, int obj_num, int obj_ext, int& ti_obj_type, int& ti_obj_num, int& ti_obj_ext)
/*
   prompts to select an object that has an event sender

   passed the object that we want to link to (the receiver) int obj_type, int obj_num, int obj_ext
   currently works for items (orb, trigger, timer) enemies (cloner and vinepod) and lift step type 5

   currently only looks for items (orb, trigger, timer 1 and 2)

   want to make work for list step type 6 also
*/

{
   while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
   mScreen.init_level_background(0);
   int tx = mDisplay.SCREEN_W/2;

   int quit=0;
   int mouse_on_item = 0;

   int x2=0, y2=0, itx=0, ity=0;


   // get object position for display
   if (obj_type == 2) // item
   {
      x2 = mItem.item[obj_num][4]+10;
      y2 = mItem.item[obj_num][5]+10;
   }

   if (obj_type == 3) // enemy
   {
      x2 = mEnemy.Ef[obj_num][0]+10;
      y2 = mEnemy.Ef[obj_num][1]+10;
   }

   if (obj_type == 4) // lift
   {
      int l = obj_ext;
      int s = obj_num;
      x2 = mLift.stp[l][s].x + mLift.stp[l][s].w / 2;
      y2 = mLift.stp[l][s].y + mLift.stp[l][s].h / 2;
//      int pms = mLift.find_previous_move_step(l, s);
//      x2 = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
//      y2 = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;
   }


   while(!quit)
   {
      mWM.redraw_level_editor_background(0);
      mMiscFnx.crosshairs_full(mWM.gx*20+10, mWM.gy*20+10, 15, 1);

      mouse_on_item = is_mouse_on_trigger_item(ti_obj_type, ti_obj_num, ti_obj_ext);

      if (mouse_on_item)
      {
         if (ti_obj_type == 2) // item
         {
            itx = mItem.item[ti_obj_num][4];
            ity = mItem.item[ti_obj_num][5];
            al_draw_line(x2, y2, itx+10, ity+10, mColor.pc[10], 2);
            if (mItem.item[ti_obj_num][0] == 13) // pointing at timer
            {
               int timer_num = ti_obj_ext;
               if (timer_num == 1)
               {
                  al_draw_rectangle(itx+1.5, ity+1.5, itx+19, ity+10, mColor.pc[9], 1);
                  al_draw_text(mFont.pixl,  mColor.pc[9],  itx+22, ity-2,  0, "timer 1");
               }
               if (timer_num == 2)
               {
                  al_draw_rectangle(itx+1.5, ity+10, itx+19, ity+19, mColor.pc[9], 1);
                  al_draw_text(mFont.pixl,  mColor.pc[9],  itx+22, ity+8,  0, "timer 2");
               }
            }
         }
         if (ti_obj_type == 3) // enemy
         {
            if (mEnemy.Ei[ti_obj_num][0] == 7) // vinepod
             {
               itx = mEnemy.Ei[ti_obj_num][9];
               ity = mEnemy.Ei[ti_obj_num][10];
               al_draw_line(x2, y2, itx+10, ity+10, mColor.pc[10], 2);
             }
         }

         if (ti_obj_type == 4) // lift
         {
            int l = ti_obj_ext;
            int s = ti_obj_num;
            int pms = mLift.find_previous_move_step(l, s);
            itx = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
            ity = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;
            al_draw_line(x2, y2, itx, ity, mColor.pc[10], 2);
         }
      }
      else
      {
         mTriggerEvent.find_and_show_event_links(obj_type, obj_num, obj_ext);
      }

      mScreen.get_new_screen_buffer(3, 0, 0);
      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, mColor.pc[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, mColor.pc[15], 1);

      al_draw_text(mFont.pr8,  mColor.pc[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Trigger Object");
      al_draw_text(mFont.pr8,  mColor.pc[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mFont.pr8,  mColor.pc[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mFont.pr8,  mColor.pc[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(mFont.pr8, mColor.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Trigger Object:%d", ti_obj_num);
      else               al_draw_textf(mFont.pr8, mColor.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Trigger Object Selected");


      while (mInput.mouse_b[1][0])
      {
         mEventQueue.proc();
         if (mouse_on_item) quit = 1;
         else quit = 2;
      }
      while ((mInput.mouse_b[2][0]) || (mInput.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mEventQueue.proc();
         quit = 2;
      }
   }

   if (quit == 2) return 0;
   return 1;
}



void mwTriggerEvent::set_event_num_in_sender(int obj_type, int obj_num, int obj_ext, int tog, int ev)
/*
   sets trigger event num in sender

   currently only works for items orb, trigger, timer

   uses tog and item num to set event num in appropriate variable number

*/
{
   if (obj_type == 2) // items
   {
      int type = mItem.item[obj_num][0];

      if (type == 6) // orb
      {
         mItem.item[obj_num][10] = 0;
         mItem.item[obj_num][11] = 0;
         mItem.item[obj_num][12] = 0;
         mItem.item[obj_num][13] = 0;
         if (tog) mItem.item[obj_num][12] = ev;
         else     mItem.item[obj_num][10] = ev;
      }
      if (type == 9) // trigger
      {
         mItem.item[obj_num][11] = 0;
         mItem.item[obj_num][12] = 0;
         mItem.item[obj_num][13] = 0;
         mItem.item[obj_num][14] = 0;
         if (tog) mItem.item[obj_num][13] = ev;
         else     mItem.item[obj_num][11] = ev;
      }
      if (type == 13) // timer
      {
         if (obj_ext == 1) mItem.item[obj_num][13] = ev;
         if (obj_ext == 2) mItem.item[obj_num][15] = ev;
      }
   }

   if (obj_type == 3) // enemy
   {
      int type = mEnemy.Ei[obj_num][0];
      if (type == 7) mEnemy.Ei[obj_num][19] = ev; // vinepod
   }

   if (obj_type == 4) // lift step
   {
      int l = obj_ext;
      int s = obj_num;
      mLift.stp[l][s].val = ev;
   }
}


void mwTriggerEvent::find_event_sender_for_obj(int obj_type, int obj_num, int obj_ext, int obj_ext2)
/*
   called from an object that needs a trigger event sender set
   works for timer, bd, bm, msg
   vinepod, cloner
   lifts

   prompts for a trigger sender
   finds unused event number
   sets that in both sender and receiver

*/
{
   if (obj_type == 2) // item
   {
      // item type of receiver
      int rx_type = mItem.item[obj_num][0];

      // prompt to choose sender obj
      int ti_obj_type, ti_obj_ext, ti_num;
      if (get_trigger_item(2, obj_num, rx_type, ti_obj_type, ti_num, ti_obj_ext ))
      {
         // get unused event
         int ev = get_unused_pm_event();

         // index of receiver event variable
         int rx_event_index = 1; // default for bm, bd and msg
         if (rx_type == 13) // timer
         {
            int timer_num = obj_ext;
            if (timer_num == 1) rx_event_index = 12; // timer 1
            if (timer_num == 2) rx_event_index = 14; // timer 2
         }

         // set event num in receiver
         mItem.item[obj_num][rx_event_index] = ev;

         // does receiver need a toogle or regular sender?
         int tog = 0;
         int timer_mode = obj_ext2;
         if ((rx_type == 16) && (mItem.item[obj_num][3] == 3))  tog = 1; // block manip mode 3 - toggle blocks
         if ((rx_type == 17) && (mItem.item[obj_num][11] == 1)) tog = 1; // block damage mode 1 - toggle damage
         if ((rx_type == 13) && (timer_mode == 1))              tog = 1; // timer mode 1 - free run after trigger

         // set event num in sender
         set_event_num_in_sender(ti_obj_type, ti_num, ti_obj_ext, tog, ev);

      }
   }

   if (obj_type == 3) // enemy
   {
      // enemy type of receiver
      int rx_type = mEnemy.Ei[obj_num][0];

      // prompt to choose sender obj
      int ti_obj_type, ti_obj_ext, ti_num;
      if (get_trigger_item(3, obj_num, 0, ti_obj_type, ti_num, ti_obj_ext ))
      {
         // get unused event
         int ev = get_unused_pm_event();

         // set event num in receiver
         if (rx_type == 7) mEnemy.Ei[obj_num][18] = ev; // vinepod
         if (rx_type == 9) mEnemy.Ei[obj_num][8] = ev; // cloner

         // set event num in sender
         set_event_num_in_sender(ti_obj_type, ti_num, ti_obj_ext, 1, ev);

      }
   }

   if (obj_type == 4) // lift
   {
      int ti_obj_type, ti_obj_ext, ti_num;
      if (mTriggerEvent.get_trigger_item(4, obj_num, 0, ti_obj_type, ti_num, ti_obj_ext))
      {
         int ev = mTriggerEvent.get_unused_pm_event();

         mLift.stp[obj_num][obj_ext].val = ev; // set event number in receiver

         // set event num in sender
         set_event_num_in_sender(ti_obj_type, ti_num, ti_obj_ext, 1, ev);

      }

   }






}
