// mwPMEvent.cpp

#include "pm.h"
#include "mwPMEvent.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwLift.h"
#include "mwDisplay.h"
#include "mwItems.h"
#include "mwInput.h"


mwPMEvent mwPME;


mwPMEvent::mwPMEvent()
{
   initialize();
}

void mwPMEvent::initialize(void)
{
   for (int x=0; x<1000; x++) event[x] = 0;
}




void mwPMEvent::show_event_line(int x, int &y, int ev, int type, int v1, int v2)
{
   if (type == 1) // item
   {
      if (mItem.item[v1][0] == 6)  al_draw_textf(mF.pr8, mC.pc[7],  x, y, 0, "ev:%2d - item:%3d [orb] ", ev, v1);
      if (mItem.item[v1][0] == 9)  al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "ev:%2d - item:%3d [trg] ", ev, v1);
      if (mItem.item[v1][0] == 16) al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "ev:%2d - item:%3d [bm]  ", ev, v1);
      if (mItem.item[v1][0] == 17) al_draw_textf(mF.pr8, mC.pc[10], x, y, 0, "ev:%2d - item:%3d [bd]  ", ev, v1);
   }
   if (type == 2) // lift
   {
      al_draw_textf(mF.pr8, mC.pc[15], x, y, 0, "ev:%2d - lift:%3d step:%d", ev, v1, v2);
   }
   y+=8;
}


void mwPMEvent::show_all_events(void)
{
   int x = 0, y = 20;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_textf(mF.pr8, mC.pc[15], 0, 0, 0, "All non-zero event references");

   for (int i=0; i<500; i++)
   {
      if (mItem.item[i][0] == 6) // orb
      {
         if (mItem.item[i][10]) show_event_line(x, y, mItem.item[i][10], 1, i, 0);
         if (mItem.item[i][11]) show_event_line(x, y, mItem.item[i][11], 1, i, 0);
         if (mItem.item[i][12]) show_event_line(x, y, mItem.item[i][12], 1, i, 0);
         if (mItem.item[i][13]) show_event_line(x, y, mItem.item[i][13], 1, i, 0);
      }
      if (mItem.item[i][0] == 9) // trigger
      {
         if (mItem.item[i][11]) show_event_line(x, y, mItem.item[i][11], 1, i, 0);
         if (mItem.item[i][12]) show_event_line(x, y, mItem.item[i][12], 1, i, 0);
         if (mItem.item[i][13]) show_event_line(x, y, mItem.item[i][13], 1, i, 0);
         if (mItem.item[i][14]) show_event_line(x, y, mItem.item[i][14], 1, i, 0);
      }


      if ((mItem.item[i][0] == 16) && (mItem.item[i][1])) show_event_line(x, y, mItem.item[i][1], 1, i, 0);
      if ((mItem.item[i][0] == 17) && (mItem.item[i][1])) show_event_line(x, y, mItem.item[i][1], 1, i, 0);
   }

   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            if (((mLift.stp[l][s].type & 31) == 5) && (mLift.stp[l][s].val)) show_event_line(x, y, mLift.stp[l][s].val, 2, l, s);




   al_flip_display();
   mI.tsw(); // wait for keypress
}











int mwPMEvent::add_item_link_translation(int sel_item_num, int sel_item_var, int sel_item_ev, int clt[][4], int clt_last)
{
   if (sel_item_ev)
   {
      // check if this event already has a translation and get it if it does
      int ev2 = check_clt_for_event(sel_item_ev, clt, clt_last);

      if (ev2) // existing translation found
      {
         clt[clt_last][0] = sel_item_num;   // item # in selection
         clt[clt_last][1] = sel_item_var;   // item var #
         clt[clt_last][2] = sel_item_ev;    // original link
         clt[clt_last][3] = ev2; // new link

         printf("et %d %d %d %d\n", clt[clt_last][0], clt[clt_last][1], clt[clt_last][2], clt[clt_last][3]);

      }
      else // no existing translation found
      {
         ev2 = get_unused_pm_event_extended(clt, clt_last);
         clt[clt_last][0] = sel_item_num;   // item # in selection
         clt[clt_last][1] = sel_item_var;   // item var #
         clt[clt_last][2] = sel_item_ev;    // original link
         clt[clt_last][3] = ev2; // new link
         printf("ne %d %d %d %d\n", clt[clt_last][0], clt[clt_last][1], clt[clt_last][2], clt[clt_last][3]);
      }
   }
   else return 0; // nothing added
   return 1; // added
}




// check the clt list for an existing pm_event
// return the translation used or 0 if none found
int mwPMEvent::check_clt_for_event(int ev, int clt[][4], int clt_last)
{
   for (int i=0; i<clt_last; i++)
   {
      if (clt[i][2] == ev) return clt[i][3];
   }
   return 0;
}


int mwPMEvent::is_pm_event_used(int ev)
{
   for (int i=0; i<500; i++)
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


      if ((mItem.item[i][0] == 16) && (mItem.item[i][1] == ev)) return 1;
      if ((mItem.item[i][0] == 17) && (mItem.item[i][1] == ev)) return 1;
   }

   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (mLift.cur[l].active)
         for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            if (((mLift.stp[l][s].type & 31) == 5) && (mLift.stp[l][s].val == ev)) return 1;

   return 0;
}

int mwPMEvent::get_unused_pm_event_extended(int clt[][4], int clt_last)
{
   int ev = 1; // starting event to test (don't ever use event 0)
   int done = 0;

   while (!done)
   {
      int used = 0;

      if (is_pm_event_used(ev)) used = 1; // first check if used in main item item array

      for (int i=0; i<clt_last; i++) if (clt[i][3] == ev) used = 1; // then check the translation table also

      if (used == 0) return ev; // found one!
      else if (++ev > 999) done = 1;
   }
   return 0; // only if no unused can be found
}



int mwPMEvent::get_unused_pm_event(void)
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


void mwPMEvent::find_and_show_event_links(int type, int i, int num2)
{
   if (type == 1) // trigger
   {
      int x1 = mItem.item[i][4]+10;
      int y1 = mItem.item[i][5]+10;
      for (int en=11; en<15; en++) // iterate all 4 trigger events
      {
         int ev = mItem.item[i][en];
         if (ev > 0) // don't do anything for link if zero
         {
            for (int i2=0; i2<500; i2++)
               if ( ((mItem.item[i2][0] == 16) || (mItem.item[i2][0] == 17)) && (mItem.item[i2][1] == ev))
               {
                  // found a match with manip or damage
                  int x2 = mItem.item[i2][4]+10;
                  int y2 = mItem.item[i2][5]+10;
                  al_draw_line(x1, y1, x2, y2, mC.pc[10], 2);
               }
            for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
               if (mLift.cur[l].active)
                  for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
                     if (((mLift.stp[l][s].type & 31) == 5) && (mLift.stp[l][s].val == ev))
                     {
                        //printf("found a match with a lift:%d step:%d ", l, s);
                        int pms = mLift.lift_find_previous_move_step(l, s);
                        int x2 = mLift.stp[l][pms].x + mLift.stp[l][pms].w / 2;
                        int y2 = mLift.stp[l][pms].y + mLift.stp[l][pms].h / 2;

                        //printf("pms:%d x:%d y:%d\n", pms, x2, y2);

                        al_draw_line(x1, y1, x2, y2, mC.pc[10], 2);
                     }
         }
      }
   }

   if (type == 2)  // block manip or block damage
   {
      int x1 = mItem.item[i][4]+10;
      int y1 = mItem.item[i][5]+10;
      int ev = mItem.item[i][1];
      if (ev > 0) // don't do anything for link if zero
      {
         for (int i2=0; i2<500; i2++)
         {
            // this finds matching triggers for bm and bd
            if ((mItem.item[i2][0] == 9) && ((mItem.item[i2][11] == ev) || (mItem.item[i2][12] == ev) || (mItem.item[i2][13] == ev) || (mItem.item[i2][14] == ev)))
            {
               // found a match with a trigger
               int x2 = mItem.item[i2][4]+10;
               int y2 = mItem.item[i2][5]+10;
               al_draw_line(x1, y1, x2, y2, mC.pc[10], 2);
            }
         }
      }
   }


   if (type == 3)  // lift
   {
      int lift = i;
      int step = num2;
      int stype = mLift.stp[lift][step].type & 31;

      // get position from previous move step
      int pms = mLift.lift_find_previous_move_step(lift, step);
      int x1 = mLift.stp[lift][pms].x + mLift.stp[lift][pms].w / 2;
      int y1 = mLift.stp[lift][pms].y + mLift.stp[lift][pms].h / 2;

//      printf("current lift:%d step:%d stype:%d  ", i, step, stype );
//      printf("pms:%d x:%d y:%d\n", pms, x1, y1);

      if (stype == 5) // only if this step is "wait for trig"
      {
         int ev = mLift.stp[lift][step].val; // get event
         if (ev > 0) // don't do anything for link if zero
         {
            for (int i2=0; i2<500; i2++)
            {
               if ((mItem.item[i2][0] == 9) && ((mItem.item[i2][11] == ev) || (mItem.item[i2][12] == ev) || (mItem.item[i2][13] == ev) || (mItem.item[i2][14] == ev)))
               {
                  // found a match with a trigger
                  int x2 = mItem.item[i2][4]+10;
                  int y2 = mItem.item[i2][5]+10;

//                  printf("found event:%d match with trigger:%d x:%d y:%d\n", ev, i2, x2, y2);
//                  printf("x1:%d y1:%d x2:%d y2:%d\n", x1, y1, x2, y2);

                  al_draw_line(x1, y1, x2, y2, mC.pc[10], 2);
               }

            }
         }
      }
   }
}




