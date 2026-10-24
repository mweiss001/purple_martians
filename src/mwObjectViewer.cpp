#include "pm.h"
#include "mwObjectViewer.h"

#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwLevel.h"
#include "mwLevelEditor.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwScreen.h"
#include "mwTriggerEvent.h"
#include "mwWidget.h"
#include "mwWindowManager.h"

mwObjectViewer mObjectViewer;

void mwObjectViewer::init()
{
   obt = 0;
   num = 0;
   viewer_lock = 0;
   snap = 20;
}


#define MAP_LOCK_KEY ALLEGRO_KEY_Z


int mwObjectViewer::create_obj(int obt, int type, int num)
{
   if (obt == 2) // items
   {
      int ret = mLevel.mItem.create_item(type);
      if (ret > 500)
      {
         al_show_native_message_box(mDisplay.display,
               "Error", "No creator exists for the current item type",
               "Copy from an existing item of that type, or get one from the selection window",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
      }
      else num = ret;
   }
   if (obt == 3)
   {
      if (type == 7)
      {
         int e = mLevel.mEnemy.create_vinepod();
         if ((e>=0) && (e<99) && (mLevel.mEnemy.Ei[e][0] == 13))
         {
            object_viewer(3, e);
            num = e;
         }
      }
      if (type == 9)
      {
         int e = mLevel.mEnemy.create_cloner();
         if ((e>=0) && (e<99) && (mLevel.mEnemy.Ei[e][0] == 9))
         {
            object_viewer(3, e);
            num = e;
         }
      }
   }
   if (obt == 4) mLevel.mLift.create_lift();
   return num;  // return number of created obj or sent_num if bad create
}
void mwObjectViewer::ov_set_width(mwWindow & ww)
{

   int type=0, w=300;

   if (obt == 2) type = mLevel.mItem.item[num][0];
   if (obt == 3) type = mLevel.mEnemy.Ei[num][0];

   if ((obt == 2) && (type == 1 )) w = 210; // door
   if ((obt == 2) && (type == 2 )) w = 200; // bonus
   if ((obt == 2) && (type == 3 )) w = 200; // exit
   if ((obt == 2) && (type == 4 )) w = 200; // key
   if ((obt == 2) && (type == 5 )) w = 200; // start
   if ((obt == 2) && (type == 6 )) w = 280; // orb
   if ((obt == 2) && (type == 7 )) w = 200; // mine
   if ((obt == 2) && (type == 8 )) w = 260; // bomb
   if ((obt == 2) && (type == 9 )) w = 280; // trigger
   if ((obt == 2) && (type == 10)) w = 260; // message
   if ((obt == 2) && (type == 11)) w = 260; // rocket
   if ((obt == 2) && (type == 13)) w = 260; // timer
   if ((obt == 2) && (type == 14)) w = 200; // switch
   if ((obt == 2) && (type == 15)) w = 240; // sproingy
   if ((obt == 2) && (type == 16)) w = 280; // bm
   if ((obt == 2) && (type == 17)) w = 290; // bd
   if ((obt == 2) && (type == 18)) w = 280; // gate
   if ((obt == 2) && (type == 19)) w = 260; // hider

   if (obt == 3) w = 300; // all enemies

   if ((obt == 3) && (type == 9 )) w = 320; // cloner

   // if ((obt == 3) && (type == 1 )) w = 300; // bouncer
   // if ((obt == 3) && (type == 2 )) w = 300; // cannon
   // if ((obt == 3) && (type == 3 )) w = 300; // archwagon
   // if ((obt == 3) && (type == 4 )) w = 300; // blokwalk
   // if ((obt == 3) && (type == 5 )) w = 300; // jumpworm
   // if ((obt == 3) && (type == 6 )) w = 300; // flapper
   // if ((obt == 3) && (type == 7 )) w = 300; // vinepod
   // if ((obt == 3) && (type == 8 )) w = 300; // trakbot
   // if ((obt == 3) && (type == 9 )) w = 300; // cloner

   if (obt == 4)                   w = 300; // lift

   ww.rect.setWidth(w);
}





void mwObjectViewer::ov_draw_overlay_rectangle_and_crosshairs(int x1, int y1, int w, int h, int color, int crosshairs)
{
   float thickness = 2.0;
   int x2 = x1+w;
   int y2 = y1+h;
   al_draw_rectangle(x1+1, y1+1, x2-1, y2-1, mColor.pc[color], thickness);


   if (crosshairs)
   {
      al_draw_textf(mFont.pixl, mColor.pc[15], x1, y1-22, 0, "x1:%d y1:%d", x1, y1);
      al_draw_textf(mFont.pixl, mColor.pc[15], x1, y1-16, 0, "x2:%d y2:%d", x2, y2);
      al_draw_textf(mFont.pixl, mColor.pc[15], x1, y1-10, 0, "w:%d h:%d", x2-x1, y2-y1);

      int xc = (x1+x2)/2;
      int yc = (y1+y2)/2;
      al_draw_line(0, yc, 1999, yc, mColor.pc[color], thickness);
      al_draw_line(xc, 0, xc, 1999, mColor.pc[color], thickness);
   }
}

void mwObjectViewer::ov_draw_overlays(int legend_highlight)
{
   al_set_target_bitmap(mBitmap.level_buffer);

   if (obt == 4)  // lifts
   {
      int lift = num;
      int step = mLevel.mLift.cur[lift].current_step;

      for (int s=0; s<mLevel.mLift.cur[lift].num_steps; s++) // iterate steps
         mTriggerEvent.find_and_show_event_links(4, lift, s);


      if ((mLevel.mLift.stp[lift][step].type & 31) != 1) step = mLevel.mLift.find_previous_move_step(lift, step);

      int color = (mLevel.mLift.stp[lift][step].type >> 28) & 15;

      int x1 = mLevel.mLift.stp[lift][step].x-1;
      int y1 = mLevel.mLift.stp[lift][step].y-1;

      int w = mLevel.mLift.stp[lift][step].w;
      int h = mLevel.mLift.stp[lift][step].h;

      int x2 = x1 + w + 3;
      int y2 = y1 + h + 3;
      int xc = (x1 + x2) / 2;
      int yc = (y1 + y2) / 2;

      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1);

      al_draw_line(xc, 0, xc, y1, mColor.pc[color], 1);
      al_draw_line(xc, y2, xc, 2000, mColor.pc[color], 1);
      al_draw_line(0, yc, x1, yc, mColor.pc[color], 1);
      al_draw_line(x2, yc, 2000, yc, mColor.pc[color], 1);

   }
   if (obt == 3)  // enemies
   {
      int type = mLevel.mEnemy.Ei[num][0];
      int obj_x = mLevel.mEnemy.Ef[num][0]+10;
      int obj_y = mLevel.mEnemy.Ef[num][1]+10;

      int color = 13;
      if (legend_highlight == 1) color = mColor.flash_color;
      mMiscFnx.crosshairs_full(obj_x, obj_y, color, 1);

      if ((type == 2) || (type == 3) || (type == 6) || (type == 8)) // // draw yellow shot prox circle for cannon, archwagon, flapper, trakbot
      {
         (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
         al_draw_circle(obj_x, obj_y, mLevel.mEnemy.Ei[num][17], mColor.pc[color], 1);
      }
      if (type == 6) // flapper
      {
         (legend_highlight == 3) ? color = mColor.flash_color : color = 10;

         // draw red height above player line
         int hab = mLevel.mEnemy.Ei[num][20];
         al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, mColor.pc[color], 3);

         // draw flap height
         int fh = mLevel.mEnemy.Ei[num][21];
         al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, mColor.pc[12], 1);
         al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, mColor.pc[12], 1);
      }
      if (type == 7)
      {
         mLevel.mEnemy.draw_vinepod_controls(num, legend_highlight);
         mTriggerEvent.find_and_show_event_links(obt, num, 0);
      }
      if (type == 9) // cloner
      {
         int color2 = 11;
         if (legend_highlight == 2) color2 = mColor.flash_color;
         ov_draw_overlay_rectangle_and_crosshairs(mLevel.mEnemy.Ei[num][15], mLevel.mEnemy.Ei[num][16], mLevel.mEnemy.Ei[num][19], mLevel.mEnemy.Ei[num][20], color2, 1);

         int color3 = 10;
         if (legend_highlight == 3) color3 = mColor.flash_color;
         ov_draw_overlay_rectangle_and_crosshairs(mLevel.mEnemy.Ei[num][17], mLevel.mEnemy.Ei[num][18], mLevel.mEnemy.Ei[num][19], mLevel.mEnemy.Ei[num][20], color3, 1);
         mTriggerEvent.find_and_show_event_links(obt, num, 0);
      }
   }
   if (obt == 2)  // items
   {
      int type = mLevel.mItem.item[num][0];
      int obj_x = mLevel.mItem.item[num][4]+10;
      int obj_y = mLevel.mItem.item[num][5]+10;
      int color = 13;
      if (legend_highlight == 1) color = mColor.flash_color;
      mMiscFnx.crosshairs_full(obj_x, obj_y, color, 1);
      switch (type)
      {
         case 1: // door
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            if (mLevel.mItem.item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((mLevel.mItem.item[i][0] == 1) && (mLevel.mItem.item[i][9] == num))
                  {
                     num_src++;
                     int x2 = mLevel.mItem.item[i][4]+10;
                     int y2 = mLevel.mItem.item[i][5]+10;
                     mMiscFnx.crosshairs_full(x2, y2, color, 1);
                     al_draw_line(obj_x, obj_y, x2, y2, mColor.pc[color], 1);
                  }
            }
            if (mLevel.mItem.item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = mLevel.mItem.item[num][9];
               int x2 = mLevel.mItem.item[di][4]+10;
               int y2 = mLevel.mItem.item[di][5]+10;
               mMiscFnx.crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, mColor.pc[color], 1);
            }
         }
         break;
         case 4: // key
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            mLevel.mItem.proc_key_block_range(num, 2); // show blocks that will be affected
         break;

         case 5: // start
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;

         case 14: // switch
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            mLevel.mItem.proc_switch_block_range(num, 2); // show blocks that will be affected
         break;
         case 8: // bomb
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            al_draw_circle(obj_x, obj_y, mLevel.mItem.item[num][7], mColor.pc[color], 1); // draw yellow bomb damage
         break;
         case 9: // trigger
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 6: // orb
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 10: // pmsg
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            int mx = mLevel.mItem.item[num][6];
            int my = mLevel.mItem.item[num][7];
            int mw = mLevel.mItem.item[num][8];
            int mh = mLevel.mItem.item[num][9];
            ov_draw_overlay_rectangle_and_crosshairs(mx, my, mw, mh, color, 1);
            al_draw_textf(mFont.pr8, mColor.pc[15], mx+mw/2, my-10,   ALLEGRO_ALIGN_CENTRE, "x:%d y:%d", mx, my);
            al_draw_textf(mFont.pr8, mColor.pc[15], mx+mw/2, my+mh+2, ALLEGRO_ALIGN_CENTRE, "w:%d h:%d", mw, mh);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         }
         break;
         case 11: // rocket
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            al_draw_circle(obj_x, obj_y, mLevel.mItem.item[num][7], mColor.pc[color], 1); // draw yellow bomb damage
         break;
         case 13: // timer
            (legend_highlight == 2) ? color = mColor.flash_color : color = 11;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 15: // sproingy
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            int y = mLevel.mItem.get_sproingy_jump_height(num);
            mMiscFnx.crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
            (legend_highlight == 2) ? color = mColor.flash_color : color = 12;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            if (mLevel.mItem.item[num][3] == 4)
            {
               (legend_highlight == 3) ? color = mColor.flash_color : color = 11;
               ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][13], mLevel.mItem.item[num][14], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            }
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 17: // block damage
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 19: // hider
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7], mLevel.mItem.item[num][8], mLevel.mItem.item[num][9], color, 1);
         break;

      } // end of switch case
   }
}
void mwObjectViewer::ov_process_mouse_on_background()
{
   int lift=0, step=0;
   if (obt == 4)
   {
      lift = num;
      step = mLevel.mLift.cur[lift].current_step;
   }

   mouse_on_obj = 0;

   // vinepod only
   mouse_on_vpodx = 0;
   mouse_on_vpod1 = 0;
   mouse_on_vpod2 = 0;
   mouse_on_tb_ul = 0;
   mouse_on_tb_lr = 0;

   // cloner only
   mouse_on_csb_ul = 0;
   mouse_on_csb_lr = 0;
   mouse_on_cdb_ul = 0;

   // enemy shot prox
   mouse_on_esp = 0;

   mouse_on_sp = 0;
   mouse_on_bmb = 0;
   mouse_on_bms = 0;

   mouse_on_item_sec_ul = 0;
   mouse_on_item_sec_lr = 0;

   mouse_on_lift = 0;

   mouse_move = 0;
   mouse_adj = 0;

   int mst = 6;      // mouse select threshold
   int msn = 10-mst; // mouse select negative threshold
   int msp = 10+mst; // mouse select positive threshold


   // -----------------------------------------------------------
   // --  detect if mouse pointer is on enemy
   // -----------------------------------------------------------
   for (int e=0; e<100; e++)
      if (mLevel.mEnemy.Ei[e][0])
      {
         int ex = mLevel.mEnemy.Ef[e][0];
         int ey = mLevel.mEnemy.Ef[e][1];
         int type = mLevel.mEnemy.Ei[e][0];

         // check to see if we can set this object to be the current object
         if ((mLevelEditor.hx>ex+msn) && (mLevelEditor.hx<ex+msp) && (mLevelEditor.hy>ey+msn) && (mLevelEditor.hy<ey+msp) && (!viewer_lock) && (!mInput.key[MAP_LOCK_KEY][0]) && (mEditorMain.obj_filter[3][type][0]))
         {
            // set this enemy to current object
            obt = 3;
            num = e;
            mouse_move = 1;
            mouse_on_obj = 1;
         }

         // if this object is already current object
         if ((mLevelEditor.hx>ex+msn) && (mLevelEditor.hx<ex+msp) && (mLevelEditor.hy>ey+msn) && (mLevelEditor.hy<ey+msp) && (obt == 3) && (num == e))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is enemy, check for secondaries
   if (obt == 3)
   {
      int e = num;
      int type = mLevel.mEnemy.Ei[e][0];
      if ((type == 2) || (type == 3) || (type == 6) || (type == 8)) // cannon, archwagon, cannon, flapper and trakbot shot prox
      {
         float x0 = mLevel.mEnemy.Ef[e][0]+10;                       // center of enemy
         float y0 = mLevel.mEnemy.Ef[e][1]+10;
         float fx = mLevelEditor.hx;                                   // hi-res mouse pos
         float fy = mLevelEditor.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
         float bdr = mLevel.mEnemy.Ei[e][17];                        // prox radius
         float dif = dst-bdr;                                 // difference
         if ((dif < 3 ) && (dif > -3))                        // mouse is on prox circle
         {
            mouse_on_esp = 1;
            mouse_move = 1;
         }
      }

      if (type == 7) // vinepod
      {
         // vinepod trigger box
         int x1 = mLevel.mEnemy.Ei[e][11];
         int y1 = mLevel.mEnemy.Ei[e][12];
         int x2 = mLevel.mEnemy.Ei[e][11]+mLevel.mEnemy.Ei[e][13]+20;
         int y2 = mLevel.mEnemy.Ei[e][12]+mLevel.mEnemy.Ei[e][14]+20;
         if ((mLevelEditor.hx>x1-mst) && (mLevelEditor.hx<x1+mst) && (mLevelEditor.hy>y1-mst) && (mLevelEditor.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_tb_ul = 1;
            mouse_move = 1;
         }
         if ((mLevelEditor.hx>x2-mst) && (mLevelEditor.hx<x2+mst) && (mLevelEditor.hy>y2-mst) && (mLevelEditor.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_tb_lr = 1;
            mouse_adj = 1;
         }

         // vinepod extended position
         int px = mLevel.mEnemy.Ei[e][9];
         int py = mLevel.mEnemy.Ei[e][10];
         if ((mLevelEditor.hx>px+msn) && (mLevelEditor.hx<px+msp) && (mLevelEditor.hy>py+msn) && (mLevelEditor.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_vpodx = 1;
         }

         if (mLevel.mEnemy.Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
         {
            // vinepod control point 1
            px = mLevel.mEnemy.Ei[e][5];
            py = mLevel.mEnemy.Ei[e][6];
            if ((mLevelEditor.hx>px+msn) && (mLevelEditor.hx<px+msp) && (mLevelEditor.hy>py+msn) && (mLevelEditor.hy<py+msp))
            {
               mouse_move = 1;
               mouse_on_vpod1 = 1;
            }

            // vinepod control point 2
            px = mLevel.mEnemy.Ei[e][7];
            py = mLevel.mEnemy.Ei[e][8];
            if ((mLevelEditor.hx>px+msn) && (mLevelEditor.hx<px+msp) && (mLevelEditor.hy>py+msn) && (mLevelEditor.hy<py+msp))
            {
               mouse_move = 1;
               mouse_on_vpod2 = 1;
            }
         }
      }

      if (type == 9) // cloner source and destination boxes
      {
         int w  = mLevel.mEnemy.Ei[e][19];  // width
         int h  = mLevel.mEnemy.Ei[e][20];  // height
         int x3 = mLevel.mEnemy.Ei[e][17];  // dest box
         int y3 = mLevel.mEnemy.Ei[e][18];
         int x1 = mLevel.mEnemy.Ei[e][15];  // source box
         int y1 = mLevel.mEnemy.Ei[e][16];
         int x2 = x1 + w - 1;
         int y2 = y1 + h - 1;
         if ((mLevelEditor.hx>x1-mst) && (mLevelEditor.hx<x1+mst) && (mLevelEditor.hy>y1-mst) && (mLevelEditor.hy<y1+mst)) // source upper left corner (move)
         {
            mouse_on_csb_ul = 1;
            mouse_move = 1;
         }
         if ((mLevelEditor.hx>x2-mst) && (mLevelEditor.hx<x2+mst) && (mLevelEditor.hy>y2-mst) && (mLevelEditor.hy<y2+mst)) // source lower right corner (re-size)
         {
            mouse_on_csb_lr = 1;
            mouse_adj = 1;
         }
         if ((mLevelEditor.hx>x3-mst) && (mLevelEditor.hx<x3+mst) && (mLevelEditor.hy>y3-mst) && (mLevelEditor.hy<y3+mst)) // destination upper left corner (move)
         {
            mouse_on_cdb_ul = 1;
            mouse_move = 1;
         }
      }
   }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on item
   // -----------------------------------------------------------
   for (int b=0; b<500; b++)
      if (mLevel.mItem.item[b][0])
      {
         int ix = mLevel.mItem.item[b][4];
         int iy = mLevel.mItem.item[b][5];
         int type = mLevel.mItem.item[b][0];
         // check to see if we can set this object to be the current object
         if ((mLevelEditor.hx>ix+msn) && (mLevelEditor.hx<ix+msp) && (mLevelEditor.hy>iy+msn) && (mLevelEditor.hy<iy+msp) && (!viewer_lock) && (!mInput.key[MAP_LOCK_KEY][0]) && (mEditorMain.obj_filter[2][type][0]))
         {
            // set this item to current object
            obt = 2;
            num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         // if this object is already current object
         if ((mLevelEditor.hx>ix+msn) && (mLevelEditor.hx<ix+msp) && (mLevelEditor.hy>iy+msn) && (mLevelEditor.hy<iy+msp) && (obt == 2) && (num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is item, check for secondaries
   if (obt == 2)
   {
      int type = mLevel.mItem.item[num][0];

      if ((type == 8) || (type == 11)) // bomb or rocket
      {
         float x0 = (float) mLevel.mItem.item[num][4]+10; // get center of item location
         float y0 = (float) mLevel.mItem.item[num][5]+10;
         float fx = (float) mLevelEditor.hx;
         float fy = (float) mLevelEditor.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
         float bdr = (float) mLevel.mItem.item[num][7]; // bomb damage radius
         float dif = dst-bdr; // difference
         if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
         {
            mouse_on_bmb = 1;
            mouse_move = 1;
         }
      }
      if (type == 15) // sproingy
      {
         int x1 = mLevel.mItem.item[num][4];
         int y1 = mLevel.mItem.item[num][5];
         int y2 = y1 - mLevel.mItem.get_sproingy_jump_height(num);
         if ((mLevelEditor.hx>x1+msn) && (mLevelEditor.hx<x1+msp) && (mLevelEditor.hy>y2+msn) && (mLevelEditor.hy<y2+msp))
         {
            mouse_on_sp = 1;
            mouse_move = 1;
         }
      }
      if (type == 16) // block manip source
      {
         int x1 = mLevel.mItem.item[num][13];
         int y1 = mLevel.mItem.item[num][14];
         if ((mLevelEditor.hx>x1-mst) && (mLevelEditor.hx<x1+msp) && (mLevelEditor.hy>y1-mst) && (mLevelEditor.hy<y1+mst))
         {
            mouse_on_bms = 1;
            mouse_move = 1;
         }
      }
      if (mLevel.mItem.item_secondary67(type))
      {
         int x1 =      mLevel.mItem.item[num][6];
         int y1 =      mLevel.mItem.item[num][7];
         int x2 = x1 + mLevel.mItem.item[num][8];
         int y2 = y1 + mLevel.mItem.item[num][9];
         if ((mLevelEditor.hx>x1-mst) && (mLevelEditor.hx<x1+mst) && (mLevelEditor.hy>y1-mst) && (mLevelEditor.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_item_sec_ul = 1;
            mouse_move = 1;
         }
         if ((mLevelEditor.hx>x2-mst) && (mLevelEditor.hx<x2+mst) && (mLevelEditor.hy>y2-mst) && (mLevelEditor.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_item_sec_lr = 1;
            mouse_adj = 1;
         }
      }
   }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on lift
   // -----------------------------------------------------------
   mouse_on_lift = 0;
   if (mEditorMain.obj_filter[4][1][0])
   {
      // no lock...check all lifts and steps
      if ((!mInput.key[MAP_LOCK_KEY][0]) && (!viewer_lock))
      {
         for (int x=0; x<NUM_LIFTS; x++)  // cycle lifts
            if (mLevel.mLift.cur[x].active)
               for (int y=0; y<mLevel.mLift.cur[x].num_steps; y++)  // cycle steps
                  if ((mLevel.mLift.stp[x][y].type & 31) == 1) // look for move step
                  {
                     int w =  mLevel.mLift.stp[x][y].w / 2;
                     int h =  mLevel.mLift.stp[x][y].h / 2;
                     int nx = mLevel.mLift.stp[x][y].x + w;
                     int ny = mLevel.mLift.stp[x][y].y + h;
                     if ((mLevelEditor.hx > nx - w)  && (mLevelEditor.hx < nx + w) && (mLevelEditor.hy > ny - h)  && (mLevelEditor.hy < ny + h)) // is mouse on this step ?
                     {
                        mouse_on_lift = 1;
                        obt = 4;
                        num = x;
                        lift = x;
                        step = y;
                        mLevel.mLift.set_lift_to_step(lift, step);   // set current step in current lift
                     }
                  }
      }


      // locked, but locked to lift
      if (((mInput.key[MAP_LOCK_KEY][0]) || (viewer_lock)) && (obt == 4))
      {
         int x = lift; // check only current lift

         // lock current step also, only detect mouse on current step if current step is move step
         int y = mLevel.mLift.cur[x].current_step; // check only current step
         if ((mLevel.mLift.stp[x][y].type & 31) == 1) // check only move step
         {
            int w =  mLevel.mLift.stp[x][y].w / 2;
            int h =  mLevel.mLift.stp[x][y].h / 2;
            int nx = mLevel.mLift.stp[x][y].x + w;
            int ny = mLevel.mLift.stp[x][y].y + h;
            if ((mLevelEditor.hx > nx - w)  && (mLevelEditor.hx < nx + w) && (mLevelEditor.hy > ny - h)  && (mLevelEditor.hy < ny + h)) // is mouse on this step ?
            {
               mouse_on_lift = 1;
               step = y;
            }
         }


      /*
         // don't lock to current step, detect mouse on any move step and set that step as current step
         for (int y=0; y<mLevel.mLift.cur[x].num_steps; y++)  // cycle steps
            if ((mLevel.mLift.stp[x][y].type & 31) == 1) // look for move step
            {
               int w =  mLevel.mLift.stp[x][y].w / 2;
               int h =  mLevel.mLift.stp[x][y].h / 2;
               int nx = mLevel.mLift.stp[x][y].x + w;
               int ny = mLevel.mLift.stp[x][y].y + h;
               if ((mLevelEditor.hx > nx - w)  && (mLevelEditor.hx < nx + w) && (mLevelEditor.hy > ny - h)  && (mLevelEditor.hy < ny + h)) // is mouse on this step ?
               {
                  mouse_on_lift = 1;
                  step = y;
                  mLevel.mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
*/



      }
   }
   if (mouse_on_lift)
   {
      mouse_on_obj = 1;
      if (mInput.mouse_b[2][0]) mLevel.mLift.step_popup_menu(lift, step);

      // is mouse on lower right adjustable corner
      int x2 = mLevel.mLift.stp[lift][step].x + mLevel.mLift.stp[lift][step].w;
      int y2 = mLevel.mLift.stp[lift][step].y + mLevel.mLift.stp[lift][step].h;
      if ((mLevelEditor.hx > x2-8) && (mLevelEditor.hy > y2-8)) mouse_adj = 1;
      else mouse_move = 1;
   }


   // -----------------------------------------------------------
   // --  set mouse cursor
   // -----------------------------------------------------------
   if (mouse_move)     al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else                al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);




   if (mInput.mouse_b[3][0])
   {
      while (mInput.mouse_b[3][0])
      {
         if (mouse_on_obj)
         {
            if (obt == 3) // move enemy
            {
               if (mInput.key[ALLEGRO_KEY_UP]   [3]) ov_move_enemy(num,  0, -1);
               if (mInput.key[ALLEGRO_KEY_DOWN] [3]) ov_move_enemy(num,  0,  1);
               if (mInput.key[ALLEGRO_KEY_RIGHT][3]) ov_move_enemy(num,  1,  0);
               if (mInput.key[ALLEGRO_KEY_LEFT] [3]) ov_move_enemy(num, -1,  0);
            }
         }
         if (mLevel.mItem.item_secondary67_hires(mLevel.mItem.item[num][0])) // hi-res adjust
         {
            if (mouse_on_item_sec_ul) ov_b3_arrow_nudge(mLevel.mItem.item[num][6], mLevel.mItem.item[num][7]); // move item secondary
            if (mouse_on_item_sec_lr) ov_b3_arrow_nudge(mLevel.mItem.item[num][8], mLevel.mItem.item[num][9]); // resize item secondary
         }

         if (mouse_on_vpod1)  ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][5],  mLevel.mEnemy.Ei[num][6]);
         if (mouse_on_vpod2)  ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][7],  mLevel.mEnemy.Ei[num][8]);
         if (mouse_on_vpodx)  ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][9],  mLevel.mEnemy.Ei[num][10]);
         if (mouse_on_tb_ul)  ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][11], mLevel.mEnemy.Ei[num][12]);
         if (mouse_on_tb_lr)  ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][13], mLevel.mEnemy.Ei[num][14]);

         if (mouse_on_csb_ul) ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][15], mLevel.mEnemy.Ei[num][16]);
         if (mouse_on_cdb_ul) ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][17], mLevel.mEnemy.Ei[num][18]);
         if (mouse_on_csb_lr) ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][19], mLevel.mEnemy.Ei[num][20]);

         if (mouse_on_esp)    ov_b3_arrow_nudge(mLevel.mEnemy.Ei[num][17], mLevel.mEnemy.Ei[num][17]);
         if (mouse_on_bmb)    ov_b3_arrow_nudge(mLevel.mItem.item[num][7], mLevel.mItem.item[num][7]);
         if (mouse_on_sp)     ov_b3_arrow_nudge(mLevel.mItem.item[num][7], mLevel.mItem.item[num][7]);
         mLevelEditor.redraw_background();
      }
   }





   if (mInput.mouse_b[1][0])
   {
      // hi-res mouse pos relative to level
      int mx = mLevelEditor.hx-10;
      int my = mLevelEditor.hy-10;

      // snap to pos
      mMiscFnx.mw_round(mx, snap);
      mMiscFnx.mw_round(my, snap);


      int lsox=0, lsoy=0;
      int lspx=0, lspy=0;
      if (mouse_on_lift)
      {
         if (mouse_move)
         {
            // snap pos
            mMiscFnx.mw_round(mLevel.mLift.stp[lift][step].x, snap);
            mMiscFnx.mw_round(mLevel.mLift.stp[lift][step].y, snap);

            // get offset from where mouse was initially clicked to lift step upper left
            lsox = mx - mLevel.mLift.stp[lift][step].x;
            lsoy = my - mLevel.mLift.stp[lift][step].y;
         }
         if (mouse_adj)
         {
            // snap size
            mMiscFnx.mw_round(mLevel.mLift.stp[lift][step].w, snap);
            mMiscFnx.mw_round(mLevel.mLift.stp[lift][step].h, snap);

            // enforce minimum size
            if (mLevel.mLift.stp[lift][step].w < 4) mLevel.mLift.stp[lift][step].w = 4;
            if (mLevel.mLift.stp[lift][step].h < 4) mLevel.mLift.stp[lift][step].h = 4;

            // get offset from where mouse was initially clicked to lift step lower right
            lspx = mx - (mLevel.mLift.stp[lift][step].x + mLevel.mLift.stp[lift][step].w);
            lspy = my - (mLevel.mLift.stp[lift][step].y + mLevel.mLift.stp[lift][step].h);
         }
      }


      while (mInput.mouse_b[1][0])
      {
         // hi-res mouse pos relative to level
         mx = mLevelEditor.hx-10;
         my = mLevelEditor.hy-10;

         // snap to pos
         mMiscFnx.mw_round(mx, snap);
         mMiscFnx.mw_round(my, snap);

//         al_draw_textf(mFont.pixl, mColor.pc[15], mInput.mouse_x, mInput.mouse_y-32, ALLEGRO_ALIGN_CENTRE, "x:%d y:%d", mx, my);

         //printf("x:%d y:%d\n", mx, my);


         if (mouse_on_obj)
         {
            if (obt == 2) ov_move_item( num, mx - mLevel.mItem.item[num][4], my - mLevel.mItem.item[num][5]);
            if (obt == 3) ov_move_enemy(num, mx - mLevel.mEnemy.Ef[num][0],  my - mLevel.mEnemy.Ef[num][1]);
            if (obt == 4) // lift
            {
               if (mouse_move)
               {
                  // set new position
                  mLevel.mLift.stp[lift][step].x = mx-lsox;
                  mLevel.mLift.stp[lift][step].y = my-lsoy;
                  mLevel.mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
               if (mouse_adj)
               {
                  // set new size
                  mLevel.mLift.stp[lift][step].w = (mx-lspx) - mLevel.mLift.stp[lift][step].x;
                  mLevel.mLift.stp[lift][step].h = (my-lspy) - mLevel.mLift.stp[lift][step].y;

                  // enforce minimum size
                  if (mLevel.mLift.stp[lift][step].w < 4) mLevel.mLift.stp[lift][step].w = 4;
                  if (mLevel.mLift.stp[lift][step].h < 4) mLevel.mLift.stp[lift][step].h = 4;

                  mLevel.mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
         }
         if (mouse_on_vpod1)
         {
            mLevel.mEnemy.Ei[num][5] = mx;
            mLevel.mEnemy.Ei[num][6] = my;
         }
         if (mouse_on_vpod2)
         {
            mLevel.mEnemy.Ei[num][7] = mx;
            mLevel.mEnemy.Ei[num][8] = my;
         }
         if (mouse_on_vpodx)
         {
            mLevel.mEnemy.Ei[num][9]  = mx;
            mLevel.mEnemy.Ei[num][10] = my;
         }
         if (mouse_on_tb_ul) // move vinepod trigger box from ul
         {
            mLevel.mEnemy.Ei[num][11] = mx;
            mLevel.mEnemy.Ei[num][12] = my;
         }
         if (mouse_on_tb_lr)  // resize vinepod trigger box from lr
         {
            // prevent lr corner from being less than ul corner
            if (mx < mLevel.mEnemy.Ei[num][11]) mx = mLevel.mEnemy.Ei[num][11];
            if (my < mLevel.mEnemy.Ei[num][12]) my = mLevel.mEnemy.Ei[num][12];

            // set new size
            mLevel.mEnemy.Ei[num][13] = mx - mLevel.mEnemy.Ei[num][11];
            mLevel.mEnemy.Ei[num][14] = my - mLevel.mEnemy.Ei[num][12];
         }

         if (mouse_on_csb_ul) // move cloner source box from ul
         {
            mLevel.mEnemy.Ei[num][15] = mx;
            mLevel.mEnemy.Ei[num][16] = my;
         }
         if (mouse_on_cdb_ul) // move cloner destination box from ul
         {
            mLevel.mEnemy.Ei[num][17] = mx;
            mLevel.mEnemy.Ei[num][18] = my;
         }
         if (mouse_on_csb_lr) // resize cloner boxes from source box lr
         {
            // prevent lr corner from being less than ul corner
            if (mx < mLevel.mEnemy.Ei[num][15]+20) mx = mLevel.mEnemy.Ei[num][15]+20;
            if (my < mLevel.mEnemy.Ei[num][16]+20) my = mLevel.mEnemy.Ei[num][16]+20;

            // set new size
            mLevel.mEnemy.Ei[num][19] = mx - mLevel.mEnemy.Ei[num][15];
            mLevel.mEnemy.Ei[num][20] = my - mLevel.mEnemy.Ei[num][16];
         }

         if (mouse_on_item_sec_ul) // move item secondary
         {
            if (!mLevel.mItem.item_secondary67_hires(mLevel.mItem.item[num][0])) // force snap 20 for these item types
            {
               mLevel.mItem.item[num][6] = mLevelEditor.gx*20;
               mLevel.mItem.item[num][7] = mLevelEditor.gy*20;
            }
            else
            {
               mLevel.mItem.item[num][6] = mx;
               mLevel.mItem.item[num][7] = my;
            }
         }
         if (mouse_on_item_sec_lr) // resize item secondary
         {
            int min_size = snap;
            if (min_size < 4) min_size = 4;
            if (!mLevel.mItem.item_secondary67_hires(mLevel.mItem.item[num][0])) // force snap 20 for these item types
            {
               mLevel.mItem.item[num][8] = (mLevelEditor.gx+1)*20 - mLevel.mItem.item[num][6];
               mLevel.mItem.item[num][9] = (mLevelEditor.gy+1)*20 - mLevel.mItem.item[num][7];
               min_size = 20;
            }
            else
            {
               mLevel.mItem.item[num][8] = mx - mLevel.mItem.item[num][6];
               mLevel.mItem.item[num][9] = my - mLevel.mItem.item[num][7];
            }
            // don't allow sizes to be less than min_size
            if (mLevel.mItem.item[num][8] < min_size) mLevel.mItem.item[num][8] = min_size;
            if (mLevel.mItem.item[num][9] < min_size) mLevel.mItem.item[num][9] = min_size;
         }
         if (mouse_on_sp) // adjust sproingy jump height
         {
            float y0 = mLevel.mItem.item[num][5]+10;
            float fy = mLevelEditor.hy;
            mLevel.mItem.item[num][7] = mLevel.mItem.get_sp(y0-fy);

            // bounds check
            if (mLevel.mItem.item[num][7] < 40) mLevel.mItem.item[num][7] = 40;
            if (mLevel.mItem.item[num][7] > 200) mLevel.mItem.item[num][7] = 200;
         }
         if (mouse_on_bmb) // adjust bomb blast radius
         {
            float x0 = mLevel.mItem.item[num][4]+10; // get center of item location
            float y0 = mLevel.mItem.item[num][5]+10;
            float fx = mLevelEditor.hx;
            float fy = mLevelEditor.hy;
            float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            mLevel.mItem.item[num][7] = dist;
         }
         if (mouse_on_bms) // move block manip source
         {
            mLevel.mItem.item[num][13] = mLevelEditor.gx*20;
            mLevel.mItem.item[num][14] = mLevelEditor.gy*20;
         }
         if (mouse_on_esp) // adjust enemy shot prox
         {
            float x0 = mLevel.mEnemy.Ef[num][0]+10; // get center of object location
            float y0 = mLevel.mEnemy.Ef[num][1]+10;
            float fx = mLevelEditor.hx;
            float fy = mLevelEditor.hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            mLevel.mEnemy.Ei[num][17] = dst;
         }
         mLevelEditor.redraw_background();
      } // end of while mInput.mouse_b[1][0] pressed

      // snap non hires item secondaries to 20
      if ((mouse_on_obj) && (obt == 2) && (mLevel.mItem.item_secondary67(mLevel.mItem.item[num][0])) && (!mLevel.mItem.item_secondary67_hires(mLevel.mItem.item[num][0])) )
      {
         mMiscFnx.mw_round(mLevel.mItem.item[num][6], 20);
         mMiscFnx.mw_round(mLevel.mItem.item[num][7], 20);
      }

   } // end of if mInput.mouse_b[1][0] pressed

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mLevelEditor.set_mode(1);
   }
}



void mwObjectViewer::ov_b3_arrow_nudge(int& x, int& y)
{
   if (mInput.key[ALLEGRO_KEY_LEFT][3])  x--;
   if (mInput.key[ALLEGRO_KEY_RIGHT][3]) x++;
   if (mInput.key[ALLEGRO_KEY_UP][3])    y--;
   if (mInput.key[ALLEGRO_KEY_DOWN][3])  y++;
}



void mwObjectViewer::ov_move_item(int num, int x_offset, int y_offset)
{
   mLevel.mItem.itemf[num][0] = mLevel.mItem.item[num][4] += x_offset;
   mLevel.mItem.itemf[num][1] = mLevel.mItem.item[num][5] += y_offset;

   if ((mInput.SHFT()) && (mLevel.mItem.item_secondary67(mLevel.mItem.item[num][0]))) // move secondaries also
   {
      mLevel.mItem.item[num][6] += x_offset;
      mLevel.mItem.item[num][7] += y_offset;
   }
}


void mwObjectViewer::ov_move_enemy(int num, int x_offset, int y_offset)
{
   mLevel.mEnemy.Ef[num][0] += x_offset;
   mLevel.mEnemy.Ef[num][1] += y_offset;
   if (mLevel.mEnemy.Ei[num][0] == 7) // vinepod
   {
      // vinepod also stores this as initial position
      mLevel.mEnemy.Ei[num][3] = mLevel.mEnemy.Ef[num][0];
      mLevel.mEnemy.Ei[num][4] = mLevel.mEnemy.Ef[num][1];

      if (mInput.SHFT()) // vinepod secondaries
      {
         mLevel.mEnemy.Ei[num][5]  += x_offset;
         mLevel.mEnemy.Ei[num][6]  += y_offset;
         mLevel.mEnemy.Ei[num][7]  += x_offset;
         mLevel.mEnemy.Ei[num][8]  += y_offset;
         mLevel.mEnemy.Ei[num][9 ] += x_offset;
         mLevel.mEnemy.Ei[num][10] += y_offset;
         mLevel.mEnemy.Ei[num][11] += x_offset;
         mLevel.mEnemy.Ei[num][12] += y_offset;
      }
   }
   if ((mLevel.mEnemy.Ei[num][0] == 9) && (mInput.SHFT())) // cloner secondaries
   {
      mLevel.mEnemy.Ei[num][15] += x_offset; // source
      mLevel.mEnemy.Ei[num][16] += y_offset;
      mLevel.mEnemy.Ei[num][17] += x_offset; // dest
      mLevel.mEnemy.Ei[num][18] += y_offset;
   }
}


void mwObjectViewer::ov_check_if_valid()
{
   // check if the current object is valid
   if (obt==0)                               mLevelEditor.set_mode(1);
   if ((obt==2) && (!mLevel.mItem.item[num][0]))    mLevelEditor.set_mode(1);
   if ((obt==3) && (!mLevel.mEnemy.Ei[num][0]))     mLevelEditor.set_mode(1);
   if ((obt==4) && (!mLevel.mLift.cur[num].active)) mLevelEditor.set_mode(1);
}

void mwObjectViewer::ov_process_keypress()
{
   char msg[1024];
   int type=0, lift=0, step=0;
   if (obt == 2) type = mLevel.mItem.item[num][0];
   if (obt == 3) type = mLevel.mEnemy.Ei[num][0];
   if (obt == 4)
   {
      lift = num;
      step = mLevel.mLift.cur[num].current_step;
      mLevel.mLift.set_lift_to_step(lift, step);   // set current step in current lift
      mLevel.mLift.cur[num].current_step = step;
   }

   while (mInput.key[ALLEGRO_KEY_DELETE][0]) { mEventQueue.proc(1); mb = 20; }
   while (mInput.key[ALLEGRO_KEY_RIGHT][0])  { mEventQueue.proc(1); mb = 21; }
   while (mInput.key[ALLEGRO_KEY_LEFT][0])   { mEventQueue.proc(1); mb = 22; }

   switch(mb)
   {
      case 18: // move
      {
         if (obt==2)
         {
            sprintf(msg,"%s", mLevel.mItem.item_name[type]);
            mMiscFnx.getxy(msg, obt, type, num);
         }
         if (obt==3)
         {
            sprintf(msg,"%s", mLevel.mEnemy.enemy_name[type][0]);
            mMiscFnx.getxy(msg, obt, type, num);
         }
         if (obt==4)
         {
            while (!mInput.key[ALLEGRO_KEY_ESCAPE][0])
            {
               if (mLevelEditor.redraw_background())
               {
                  for (int t=0; t<2; t++) mLevel.mLift.move_lifts(1);  // move lifts for 2 frames
                  ov_draw_overlays(0);
                  mScreen.draw_scaled_level_region_to_display();
                  mLevelEditor.mWM.cycle_windows(1); // draw only
               }
            }
            while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1); // wait for release
            mLevel.mLift.lift_setup(); // reset all lifts to step 0
         }
      }
      break;
      case 19:
         num = create_obj(obt, type, num);
      break;
      case 20: // delete
         if (obt == 2)
         {
            mLevel.mItem.clear_item(num);
            mLevel.mItem.sort_item(1);
            if (num >= mLevel.mItem.item_first_num[type]+ mLevel.mItem.item_num_of_type[type]) num--;
            ov_check_if_valid();
         }
         if (obt == 3)
         {
            mLevel.mEnemy.Ei[num][0] = 0;
            mLevel.mEnemy.sort_enemy();
            if (num >= mLevel.mEnemy.e_first_num[type]+mLevel.mEnemy.e_num_of_type[type]) num--;
            ov_check_if_valid();
         }
         if (obt == 4)
         {
            mLevel.mLift.erase_lift(num);
            if (--num < 0) num = 0;      // set to prev lift or zero
            ov_check_if_valid();
         }
      break;
      case 21: // next
         if ((obt==2) && (++num >= mLevel.mItem.item_first_num[type] + mLevel.mItem.item_num_of_type[type])) num--;
         if ((obt==3) && (++num >= mLevel.mEnemy.e_first_num[type] + mLevel.mEnemy.e_num_of_type[type])) num--;
         if (obt==4) num = mLevel.mLift.get_next_lift(num);
      break;
      case 22: // previous
         if ((obt==3) && (--num < mLevel.mEnemy.e_first_num[type])) num++;
         if ((obt==2) && (--num < mLevel.mItem.item_first_num[type])) num++;
         if (obt==4) num = mLevel.mLift.get_prev_lift(num);
      break;
   } // end of switch (mb)
}


// 71
void mwObjectViewer::odbi(int d, int xa, int xb, int &ya, int bts, int col, int &var, int ul, int ll, int sl_inc, int st1_inc, int st2_inc, const char* txt)
{
   mWidget.mStepSliderInt(0, xa, xb,  1, ya, bts-1,  2,2,1,1,   col, col, 15,15,15,0,0,  var,   ul,   ll,  sl_inc, st1_inc, st2_inc, txt, 1, d);
   ya+=bts+1;
}
// 45
void mwObjectViewer::odb0(int d, int xa, int xb, int &ya, int bts, int col, int &var, int ul, int ll, int sl_inc, int st1_inc, int st2_inc, const char* txt, const char* txt0)
{
   mWidget.mStepSliderInt0(0, xa, xb,  1, ya, bts-1,  2,2,1,1,   col, col, 15,15,15,0,0,  var,   ul,   ll,  sl_inc, st1_inc, st2_inc, txt, txt0, 1, d);
   ya+=bts+1;
}

// 34
void mwObjectViewer::odbf(int d, int xa, int xb, int &ya, int bts, int col, float &var, float ul, float ll, float sl_inc, float st1_inc, float st2_inc, const char* txt)
{
   mWidget.mStepSliderFloat(0, xa, xb,  1, ya, bts-1,  2,2,1,1,   col, col, 15,15,15,0,0,  var,   ul,   ll,  sl_inc, st1_inc, st2_inc, txt, 1, d);
   ya+=bts+1;
}


// 26
void mwObjectViewer::odbt(int d, int xa, int xb, int &ya, int bts, int colt, int &var, int col0, int col1,  const char* txt0, const char* txt1)
{
   mWidget.mButtonToggle(0, xa, xb,  ya, bts,    1, 2, 0, 0,     0, 0, var, 0, 1, txt0, txt1, col0, col1, colt, colt, 0, 0, d);
}

// 30
void mwObjectViewer::odbt(int d, int xa, int xb, int &ya, int bts, int &var, int bitflag, const char* txt0, const char* txt1, int bcol0, int bcol1, int tcol0, int tcol1)
{
   mWidget.mButtonToggleFlag(0, xa, xb,  ya, bts,    1, 2, 0, 0,    0, 0, var, bitflag,  txt0, txt1, bcol0, bcol1, tcol0, tcol1, 0, 0, d);
}



// 42
bool mwObjectViewer::odbb(int d, int xa, int xb, int &ya, int bts, int col, const char* txt)
{
   return mWidget.mButton(0,xa,xb,  ya,bts,   0,2,3,1,   col,col,15,15,0,  txt, d);
}

// 16
bool mwObjectViewer::odbc(int d, int xa, int xb, int &ya, int bts, int bcol, int tcol, int type, int& v1, int v2, int v3)
{
   return mWidget.mButtonCustom(0, xa, xb, ya, bts, 1, 2, 0, 1,   bcol, 0, tcol, 0, 0, type, v1, v2, v3, d);
}

// 27
bool mwObjectViewer::odbp(int d, int xa, int xb, int &ya, int bts, int col, int type, int& v1)
{
   return mWidget.mButtonPD(0, xa, xb, ya, bts,   1, 2, col, col, col, 15, 1, type, v1, d);
}








void mwObjectViewer::ov_draw_buttons(int x1, int y1, int x2, int y2, int d)
{
   int n = num;
   int type=0;
   if (obt == 2) type = mLevel.mItem.item[n][0];
   if (obt == 3) type = mLevel.mEnemy.Ei[n][0];


   mb = 0;


   // button x position
   int xa = x1+1;
   int xb = x2-1;
   int ya = y1+38;
   // split into thirds
   int x13 = xa + 1 * (xb-xa) / 3; // 1/3
   int x23 = xa + 2 * (xb-xa) / 3; // 2/3
   // split into quarters
   int x14 = xa + 1 * (xb-xa) / 4; // 1/4
   // split into sevenths
   int x27 = xa + 2 * (xb-xa) / 7; // 2/7
   int x57 = xa + 5 * (xb-xa) / 7; // 5/7

   int bts=16;
   int dim=128;


   // --------------------------------------------------
   // --- Common ---------------------------------------
   // --------------------------------------------------

   int by1 = y1+2;

   if (mWidget.mButton(0, x2-12, x2-4,   1, by1, 9,     0, 0, 0, 3,    0, 0, 15, 14, 0,   "?", d)) mHelp.help("Viewer Basics");
   if (mWidget.mButton(0, xa, x27-1,  1, ya, bts-2,    1, 2, 0, 1,    9, 9, 15, 0, 0, "Prev", d)) mb = 22;
   mWidget.mButtonToggle(0, x27+1, x57-1,  1, ya, bts-2,    1, 2, 0, 0,     0, 0, viewer_lock, 0, 1, "Unlocked", "Locked", 6, 7, 15, 15, 0, 0, 0);
   if (mWidget.mButton(0, x57+1,   xb,  1, ya, bts-2,    1, 2, 0, 1,    9, 9, 15, 0, 0, "Next", d)) mb = 21;
   ya+=bts;

   if (obt == 4) { if (mWidget.mButton(0, xa, x13-1,  1, ya, bts-2,    1, 2, 0, 1,    13, 13, 15, 0, 0, "Run Lifts", d)) mb = 18; }
   else          { if (mWidget.mButton(0, xa, x13-1,  1, ya, bts-2,    1, 2, 0, 1,    13, 13, 15, 0, 0, "Move",      d)) mb = 18; }
   if (mWidget.mButton(0, x13+1, x23-1,  1, ya, bts-2,    1, 2, 0, 1,    14, 14, 15, 0, 0, "Create", d)) mb = 19;
   if (mWidget.mButton(0, x23+1, xb,     1, ya, bts-2,    1, 2, 0, 1,    10, 10, 15, 0, 0, "Delete", d)) mb = 20;
   ya+=bts;

   if (mWidget.mButton(0, xa, x14-1,     1, ya, bts-2,    1, 2, 0, 1,    1, 1, 15, 0, 0, "Help", d)) mHelp.help("Viewer Basics");
   mWidget.mButtonCustom(0, xa, xb, 1, ya, bts-2, 1, 2, 0, 1,   1, 0, 15, 0, 0, 57, n, obt, 0, d); // object type specific help
   ya+=bts+4;


   // --------------------------------------------------
   // --- Lifts ----------------------------------------
   // --------------------------------------------------
   if (obt == 4) // lifts
   {
      int step = mLevel.mLift.cur[n].current_step;
      odbp(d, xa, xb, ya, bts, 13, 500, mLevel.mLift.cur[n].mode); // lift mode
      odbp(d, xa, xb, ya, bts, 13, 504, mLevel.mLift.cur[n].draw_mode); // draw mode
      if ((mLevel.mLift.cur[n].draw_mode == 10) || (mLevel.mLift.cur[n].draw_mode == 13)) // single block or tileset
      {
         ya+=4;
         odbc(d, xa, xb, ya, bts, 8, 15, 320, n, 0, 0); // set single block
         ya+=4;
      }
      if (( mLevel.mLift.cur[n].draw_mode == 11) || (mLevel.mLift.cur[n].draw_mode == 12)) // 3 block platform or column
      {
         ya+=4;
         odbc(d, xa, xb, ya, bts, 8, 15, 321, n, 0, 0); // set start block
         ya+=4;
      }




      if (mLevel.mLift.cur[n].mode)
         odbi(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ei[n][8],      2000,        1,        1,        1,       10,      "Reset Timer:");

      mWidget.mButton(0, xa, xb,   1, ya, bts-2,   0, 2, 0, 21,   13, 15, 15,  0, 0, "Lift Name:", 1);
      if (mWidget.mButton(3, xa+84, 0,   1, ya+1, 12,   1, 1, 3, 3,   0, 15, 15,  10, 0, mLevel.mLift.cur[n].lift_name, 0)) mMiscFnx.edit_string_simple(xa+84, ya+1, mLevel.mLift.cur[n].lift_name, sizeof(mLevel.mLift.cur[n].lift_name));

      ya+=bts;

      // list of step buttons
      // --------------------------------------------------------------------------------
      int ysb = ya; // y pos of step buttons

      // process mouse on list of step buttons
      int step_pointer = -99;
      if ((mInput.mouse_x > xa + 10) && (mInput.mouse_x < xb - 10)) // is mouse on step buttons?
      {
         int step0_y_offset = ysb + 14 + bts;
         int mouse_step = (mInput.mouse_y - step0_y_offset) / bts;               // calculate step that mouse is on
         if ((mouse_step >= 0) && (mouse_step < mLevel.mLift.cur[n].num_steps))   // is this a valid step?
         {
            step_pointer = mouse_step;  // set step pointer to this step
            if (mInput.mouse_b[1][0])
            {
               step = mouse_step; // set current step to this step
               mLevel.mLift.cur[n].current_step = step; // set current step in lift
            }
            if (mInput.mouse_b[2][0]) mLevel.mLift.step_popup_menu(n, step_pointer); // step pop-up menu for this step
         }
      }

      // draw the list of steps  - this has to go after, because it can eat the mouse clicks needed for previous section
      int ycs = ysb + mLevel.mLift.draw_steps(xa, xb, ysb, n, step, step_pointer, d);
      ycs +=bts;

      // draw buttons for the current step button and get y position for next item (lift)
      int yld = ycs + mLevel.mLift.draw_current_step_buttons(xa, xb, ycs, n, step, d);


      mLevelEditor.mWM.mW[7].rect.h = yld - mLevelEditor.mWM.mW[7].rect.y1-1; // global variable for height of ovw when variable due to lift
      mLevelEditor.mWM.mW[7].set_size(mLevelEditor.mWM.mW[7].rect.w, mLevelEditor.mWM.mW[7].rect.h);

      yld += bts;

      // draw current lift under step list buttons
      if (bts == 16) // only if max button size
      {
         int x1 = xa+10;
         int y1 = yld; // only see in 2 highest screen modes
         int s = step;

         // if step is not a move step, find prev that is
         if ((mLevel.mLift.stp[n][s].type & 31) != 1) s = mLevel.mLift.find_previous_move_step(n, s);

         mwRect<float> lr = mwRect<float>::fromX1Y1WH(x1, y1, mLevel.mLift.stp[n][s].w, mLevel.mLift.stp[n][s].h);
         mLevel.mLift.draw_lift(n, lr);
      }
   }


   // --------------------------------------------------
   // --- Enemies --------------------------------------
   // --------------------------------------------------
   if (obt == 3)    // enemies
   {
      switch (type) // enemy subtypes
      {
         case 1: // bouncer
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][5],        12,        0,     0.01,     0.01,      0.1,      "Speed:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ei[n][8],       100,        0,        1,        1,        0,      "Seek Count:");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 12, "Set Initial Direction"))  mMiscFnx.getxy("Initial Direction", 96, 4, n);
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 8, 9, 1313, n, 5, 0); // main shape
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 8, 9, 1313, n, 6, 0); // seek shape
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;
         case 2: // cannon
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][5],        12,        0,     0.01,     0.01,      0.1,      "Speed:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ei[n][8],       100,        0,        1,        1,        0,      "Seek Count:");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 12, "Set Initial Direction"))  mMiscFnx.getxy("Initial Direction", 96, 4, n);
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  8, mLevel.mEnemy.Ei[n][9],        40,        0,        1,        1,        0,      "Extra Hits To Kill:");
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][7],        20,      0.8,      0.1,      0.1,        1,      "Shot Speed:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][17],     2000,       20,        1,        1,       10,      "Shot Proximity:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][15],      200,        1,        1,        1,       10,      "Retrigger Time:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;
         case 3: // archwag
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][6],         9,      0.7,     0.01,     0.01,      0.1,     "X-Speed:");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][3],        16,      0.7,     0.01,     0.01,      0.1,     "Y-Speed:");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][2],        13, 13,  "Initial Direction:Left ",    "Initial Direction:Right");
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][8],        13, 13,  "Follow Mode",                "Bounce Mode");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][10],      100,        0,        1,        1,       10,      "Turn Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][11],      100,        0,        1,        1,       10,      "Jump Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][12],      100,        0,        1,        1,       10,      "Jump Before Wall:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][7],       600,        0,        1,        1,       10,      "Jump Under Width:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][6],       500,        0,        1,        1,       10,      "Jump Timer Wait:", "Off");
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][7],        20,      0.8,      0.1,      0.1,        1,      "Shot Speed:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][17],     2000,       20,        1,        1,       10,      "Shot Proximity:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][15],      200,        1,        1,        1,       10,      "Retrigger Time:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;
         case 4: // block walker
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][6],         9,      0.7,     0.01,     0.01,      0.1,     "X-Speed:");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][3],        16,      0.7,     0.01,     0.01,      0.1,     "Y-Speed:");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][2],        13, 13,  "Initial Direction:Left ",    "Initial Direction:Right");
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][8],        13, 13,  "Follow Mode",                "Bounce Mode");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][10],      100,        0,        1,        1,       10,      "Turn Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][11],      100,        0,        1,        1,       10,      "Jump Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][12],      100,        0,        1,        1,       10,      "Jump Before Wall:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][7],       600,        0,        1,        1,       10,      "Jump Under Width:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][6],       500,        0,        1,        1,       10,      "Jump Timer Wait:", "Off");
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 8, 15, 318, n, 0, 0); // get block
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 8, "Default 'H' Block")) mLevel.mEnemy.Ei[n][13] = 148 | PM_BTILE_ALL_SOLID | PM_BTILE_BREAKABLE_PSHOT;
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
         break;
         case 5: // jumpworm
            odbi(d, xa, xb, ya, bts, 11, mLevel.mEnemy.Ei[n][4],        12,        1,        1,        1,        0,     "X-Speed (Ground):1/");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][6],        12,      0.4,     0.01,     0.01,      0.1,     "X-Speed (jump):");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][3],        12,      0.4,     0.01,     0.01,      0.1,     "Y-Speed:");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][2],        13, 13,  "Initial Direction:Left ",    "Initial Direction:Right");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][10],      100,        0,        1,        1,       10,      "Turn Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][11],      100,        0,        1,        1,       10,      "Jump Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][12],      100,        0,        1,        1,       10,      "Jump Before Wall:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][8],       160,        0,        1,        1,       10,      "Wall Jump Boost:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][7],       600,        0,        1,        1,       10,      "Jump Under Width:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][6],       500,        0,        1,        1,       10,      "Jump Timer Wait:", "Off");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;
         case 6: // flapper
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][5],         8,      0.5,     0.01,     0.01,      0.1,     "X-Speed:");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][6],         1,     0.01,     0.01,     0.01,      0.1,     "X-Accel:");
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][2],        12, 12,  "Initial Direction:Left ",    "Initial Direction:Right");
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ef[n][3],         5,      0.0,     0.01,     0.01,      0.1,      "Y-Speed:");
            odbf(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ef[n][10],        8,      0.5,      0.1,     0.01,      0.1,      "Flap Speed:");
            odbi(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ei[n][21],       80,        0,        1,        1,       10,      "Flap Height:");
            odbi(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ei[n][20],     1000,    -1000,        1,        1,       10,      "Height Above Player:");
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][7],        20,      0.8,      0.1,      0.1,        1,      "Shot Speed:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][17],     2000,       20,        1,        1,       10,      "Shot Proximity:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][15],      200,        1,        1,        1,       10,      "Retrigger Time:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;

         case 7: // vinepod
         {
            if (odbb(d, xa, xb, ya, bts, 10, "Set Extended Position")) mMiscFnx.getxy("Vinepod Extended Position", 90, 13, n);
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][17],      400,        2,        1,        1,       10,      "Extend Time:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][9],       400,        0,        1,        1,       10,      "Pause:");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_FIRE_SHOT, "Fire Shot:OFF", "Fire Shot:ON ", 11+dim, 11, 15+dim, 15);
            if (mLevel.mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_FIRE_SHOT)
            odbf(d, xa, xb, ya, bts, 11, mLevel.mEnemy.Ef[n][7],        20,      0.8,      0.1,      0.1,        1,      "Shot Speed:");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts,  2, mLevel.mEnemy.Ei[n][19],       99,        0,        1,        1,       10,      "Output Event:", "OFF");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_SHOW_PATH, "Hide Path","Show Path" ,                            14+dim, 14, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_INIT, "Invincible At Rest:OFF" , "Invincible At Rest:ON ",  14+dim, 14, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_EXTN, "Invincible Extended:OFF", "Invincible Extended:ON ", 14+dim, 14, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_MOVE, "Invincible Moving:OFF"  , "Invincible Moving:ON ",   14+dim, 14, 15+dim, 15);
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(3, n, 0, 0);
            odb0(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ei[n][18],       99,        0,        1,        1,       10,      "Event Trigger:", "OFF");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_USE_TRIGGER, "Use Internal Trigger:OFF", "Use Internal Trigger:ON ",   14+dim, 14, 15+dim, 15);
            if (mLevel.mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_USE_TRIGGER)
            {
               if (odbb(d, xa, xb, ya, bts, 14, "Set Internal Trigger Box")) mMiscFnx.get_block_range("Trigger Box", mLevel.mEnemy.Ei[n][11], mLevel.mEnemy.Ei[n][12], mLevel.mEnemy.Ei[n][13], mLevel.mEnemy.Ei[n][14], 2);
            }
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_USE_SPLINE, "Use Spline:OFF", "Use Spline:ON ",   3+dim, 3, 15+dim, 15 );
            if (mLevel.mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
            {
               odbf(d, xa, xb, ya, bts,  3, mLevel.mEnemy.Ef[n][6],        8,         1,      0.1,      0.1,        1,      "Control Point Scale:");
               if (odbb(d, xa, xb, ya, bts, 6, "Set Control Point 1")) mMiscFnx.getxy("Vinepod Control Point 1", 91, 13, n);
               if (odbb(d, xa, xb, ya, bts, 7, "Set Control Point 2")) mMiscFnx.getxy("Vinepod Control Point 2", 92, 13, n);
               if (odbb(d, xa, xb, ya, bts, 3, "Set Thirds")) mLevel.mEnemy.vinepod_set_cp_thirds(n);
            }

            ya+=4; // spacer
            static int xfrm = 1;
            static int rot = 90;
            static float sca = 1.0;
            int c = 6;
            odbt(d, xa, xb, ya, bts, 15, xfrm,        c+64, c,  "Transform Tools:Off", "Transform Tools:On");
            if (xfrm)
            {
               ya+=2; // spacer
               int xh = 70;
               if (odbb(d, xa, xa+xh-2, ya, bts, c, "Rotate")) mLevel.mEnemy.vinepod_rotate(n, mLevel.mEnemy.deg_to_rad(rot));
               ya-=bts;
               odbi(d, xa+xh+2, xb, ya, bts-1,  c, rot,                    180,    -180,        1,        1,       10,      "Degrees:");
               ya+=1; // spacer
               if (odbb(d, xa, xa+xh-2, ya, bts, c, "Scale")) mLevel.mEnemy.vinepod_scale(n, sca);
               ya-=bts;
               odbf(d, xa+xh+2, xb, ya, bts-1,  c, sca,                     10,    0.01,     0.01,      0.1,       1,      "");
               ya+=1; // spacer
               xh = (xb-xa)/2;
               if (odbb(d, xa, xa+xh-2, ya, bts, c, "Mirror X")) mLevel.mEnemy.vinepod_mirror_x(n);
               ya-=bts;
               if (odbb(d, xa+xh+2, xb, ya, bts, c, "Mirror Y")) mLevel.mEnemy.vinepod_mirror_y(n);
            }
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         }
         break;
         case 8: // trakbot
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][2],        10,      0.5,     0.01,     0.01,      0.1,      "X-Speed:");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][3],        10,      0.5,     0.01,     0.01,      0.1,      "Y-Speed:");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 12, "Initial Direction"))
            {
               mLevel.mEnemy.Ei[n][5]++;
               if ((mLevel.mEnemy.Ei[n][5] < 0) || (mLevel.mEnemy.Ei[n][5] > 7)) mLevel.mEnemy.Ei[n][5] = 0; // enforce limits
               mLevel.mEnemy.set_trakbot_mode(num, mLevel.mEnemy.Ei[n][5]);
            }
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][7],    6+64, 6,  "Drop Mode:Off", "Drop Mode:On ");
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ef[n][7],        20,      0.8,      0.1,      0.1,        1,      "Shot Speed:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][17],     2000,       20,        1,        1,       10,      "Shot Proximity:");
            odbi(d, xa, xb, ya, bts,  9, mLevel.mEnemy.Ei[n][15],      200,        1,        1,        1,       10,      "Retrigger Time:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;

         case 9: // cloner
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][30],    12, 10,  "Not Invincible", "Invincible!");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(3, n, 0, 0);
            odb0(d, xa, xb, ya, bts, 13, mLevel.mEnemy.Ei[n][8],        99,        0,        1,        1,       10,      "Event Trigger:", "none");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts, 14, mLevel.mEnemy.Ei[n][9],      4800,        0,        1,        1,       10,      "Created Time To Live:", "-");
            odb0(d, xa, xb, ya, bts, 14, mLevel.mEnemy.Ei[n][10],      600,        0,        1,        1,       10,      "Max Created Objects:", "-");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 11, "Set Source Area")) mMiscFnx.get_block_range("Cloner Source Area", mLevel.mEnemy.Ei[n][15], mLevel.mEnemy.Ei[n][16], mLevel.mEnemy.Ei[n][19], mLevel.mEnemy.Ei[n][20], 1);
            if (odbb(d, xa, xb, ya, bts, 10, "Set Destination")) mMiscFnx.getxy("Cloner Destination", 98, 9, n);
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 13, 1081, mLevel.mEnemy.Ei[n][4]); // show boxes
            odbp(d, xa, xb, ya, bts, 13, 1082, mLevel.mEnemy.Ei[n][5]); // draw mode
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][29],       20,        0,        1,        1,       10,      "Collision Box:");
            odbf(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ef[n][4] ,       10,        0,     0.01,     0.01,      0.1,      "Health Decrement:" );
            odbi(d, xa, xb, ya, bts,  4, mLevel.mEnemy.Ei[n][25],       50,        0,        1,        1,       10,      "Health Bonus:" );
         break;

         case 19: // crew
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][6],         9,      0.7,     0.01,     0.01,      0.1,     "X-Speed:");
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][3],        16,      0.7,     0.01,     0.01,      0.1,     "Y-Speed:");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][2],        13, 13,  "Initial Direction:Left ",    "Initial Direction:Right");
            odbt(d, xa, xb, ya, bts, 15, mLevel.mEnemy.Ei[n][8],        13, 13,  "Follow Mode",                "Bounce Mode");
            ya+=4; // spacer
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][10],      100,        0,        1,        1,       10,      "Turn Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][11],      100,        0,        1,        1,       10,      "Jump Before Hole:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][12],      100,        0,        1,        1,       10,      "Jump Before Wall:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][7],       600,        0,        1,        1,       10,      "Jump Under Width:", "Off");
            odb0(d, xa, xb, ya, bts,  7, mLevel.mEnemy.Ei[n][6],       500,        0,        1,        1,       10,      "Jump Timer Wait:", "Off");
            ya+=4; // spacer
            mWidget.mColorSelect(0, xa, xb, ya, bts, 1, mLevel.mEnemy.Ei[num][3], 0, "Select Crew Color", d);
            ya+=4; // spacer
            odbf(d, xa, xb, ya, bts, 12, mLevel.mEnemy.Ef[n][8],       1.0,      0.2,     0.05,     0.01,      0.1,      "Scale:");
         break;

      }
   }

   // --------------------------------------------------
   // --- Items ----------------------------------------
   // --------------------------------------------------
   if (obt == 2) // items
   {
      switch (type) // item subtypes
      {
         case 1: // door
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][8], 12, 12,  "Door Type:Exit Only",  "Door Type:Normal");
            if (mLevel.mItem.item[n][8]) // regular door (not exit only)
            {
               // check for bad link
               // int link = mItem.item[num][9];
               // if (mItem.item[link][0] != 1) // link is not door
               // {
               //    mItem.item[num][9] = num;  // link to self
               //    mItem.item[num][11] = 1;   // trigger with up
               // }

               ya+=4; // spacer
               char msg[80];
               sprintf(msg, "Destination Door:%d", mLevel.mItem.item[num][9]);
               if (odbb(d, xa, xb, ya, bts, 11, msg))
               {
                  int i = mMiscFnx.get_item(2, 1, num );
                  if (i > -1) mLevel.mItem.item[num][9] = i;
               }
               odbp(d, xa, xb, ya, bts, 11, 1050, mLevel.mItem.item[n][11]); // door entry type
               odbt(d, xa, xb, ya, bts,  15, mLevel.mItem.item[n][7], 11, 11, "Move Type:Instant", "Move Type:Travel ");
               odbp(d, xa, xb, ya, bts, 11, 1051, mLevel.mItem.item[n][12]); // exit link show
            }
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 13, 1052, mLevel.mItem.item[n][1]); // draw type
            mWidget.mColorSelect(0, xa, xb, ya, bts, 1, mLevel.mItem.item[n][6], 0, "Select Door Color", d);
            if (odbb(d, xa, xb, ya, bts, 13, "Change Linked to Match")) mLevel.mItem.change_linked_door_color_and_draw_type(n);
         break;
         case 2: // bonus
            if (mLevel.mItem.item[n][6] == 0) // health bonus
            {
               odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
               ya+=4; // spacer
               odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][6], 11, 8,  "Type:Health Bonus", "Type:Purple Coin");
               odbi(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][7], 100, 0, 1,  1, 10, "Health:");
               mLevel.mItem.item[n][1] = 1035;
               mLevel.mItem.item[n][2] = 1;
               if (mLevel.mItem.item[n][7] == 10) mLevel.mItem.item[n][1] = 1023;
               if (mLevel.mItem.item[n][7] == 50) mLevel.mItem.item[n][1] = 1027;
            }
            else if (mLevel.mItem.item[n][6] == 1) // purple coin
            {
               odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][3], 13, 13,  "Stationary",    "Fall");
               ya+=4; // spacer
               odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][6], 11, 8,  "Type:Health Bonus", "Type:Purple Coin");
               mLevel.mItem.item[n][1] = 197;
               mLevel.mItem.item[n][2] = 0;
            }
         break;
         case 3: // exit
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbb(1, xa, xb, ya, bts, 12, "Enemy Count Lock");
            odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][8], 100, 0, 1,  1, 10, "");
         break;
         case 4: // key
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][3],  13, 13,  "Stationary",    "Fall");
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 13, 102, mLevel.mItem.item[n][1]); // key color
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 10, "Get New Block Range")) mMiscFnx.get_block_range("Block Range", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][12],  7, 7,  "Remove All Blocks", "Remove Only Matching");
         break;
         case 5: // start
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 11, 1078, mLevel.mItem.item[n][6]); // start mode
            odbi(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][7], 9, 0, 1,  1, 0, "Start Index:");
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][8],  11, 11, "Increase Index Only:OFF", "Increase Index Only:ON");
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 13, "Set Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
            odb0(d, xa, xb, ya, bts,  13, mLevel.mItem.item[n][9],  99, 0,  1,   1, 0, "Event Trigger:", "Off");
         break;
         case 6: // orb
         {
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 7, 15, 94, n, 0, 0); // rotation
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 12, 15, 90, n, 0, 0); // orb trigger type
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 8, 92, mLevel.mItem.item[n][6]); // mode
            int MODE = mLevel.mItem.item[n][6];
            if ((MODE == 1) || (MODE == 3)) mLevel.mItem.item[n][2] &= ~PM_ITEM_ORB_STATE; // MODE:Stick On  or MODE:Timed On  - set initial state OFF
            if ((MODE == 2) || (MODE == 4)) mLevel.mItem.item[n][2] |=  PM_ITEM_ORB_STATE; // MODE:Stick Off or MODE:Timed Off - set initial state ON
            if (MODE == 0) // only show initial state for toggle mode
               odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][2], PM_ITEM_ORB_STATE, "Initial State:OFF","Initial State:ON ", 8, 8, 15, 15);
            if ((MODE == 3) || (MODE == 4)) // only show timer for mode 3 and 4
               odbi(d, xa, xb, ya, bts, 8, mLevel.mItem.item[n][7], 400, 0, 1,  1, 10, "Time:");
            ya+=4; // spacer
            odbb(1, xa, xb, ya, bts, 11, "Continuously Set Event When:");
            odb0(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][10], 99, 0, 1,  1, 0,  "On:",  "-");
            odb0(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][11], 99, 0, 1,  1, 0,  "Off:", "-");
            ya+=4; // spacer
            odbb(1, xa, xb, ya, bts, 12, "Set Event When Switching:");
            odb0(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][12], 99, 0, 1,  1, 0,  "On:",  "-");
            odb0(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][13], 99, 0, 1,  1, 0,  "Off:", "-");
         }         break;
         case 7: // mine
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 10, mLevel.mItem.item[n][8], 200, 0, 1,  1, 10, "Damage:");
         break;
         case 8: // bomb
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[num][12],        12, 12,  "Type:Fuse Timer",    "Type:Remote Detonator");
            if (mLevel.mItem.item[num][12]) mLevel.mItem.item[num][1] = 537; // remote detonator
            else // fuse
            {
               mLevel.mItem.item[num][1] = 464;
               odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][9], 2000, 1, 1,  1, 10, "Fuse Time:");
            }
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][11],   4, 4,  "Sticky:Off", "Sticky:On");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 14, mLevel.mItem.item[n][7], 1200, 20, 1,  1, 10, "Damage Range:");
         break;

         case 10: // message
         {
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][2], PM_ITEM_PMSG_SHOW_ALWAYS , "Show Always:OFF", "Show Always:ON", 12+dim, 12, 15+dim, 15);
            ya+=4; // spacer
            if (!(mLevel.mItem.item[n][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
            {
               odbi(d, xa, xb, ya, bts, 8, mLevel.mItem.item[n][12], 400, 0, 1,  1, 10, "Display time:");
               ya+=4; // spacer
               odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][2], PM_ITEM_PMSG_SHOW_SCROLL , "Show Scroll:OFF", "Show Scroll:ON", 9+dim, 9, 15+dim, 15);
               ya+=4; // spacer
               if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
               odb0(d, xa, xb, ya, bts,  13, mLevel.mItem.item[n][1],  99, 0,  1,   1, 0, "Event Trigger:", "Off");
               ya+=4; // spacer
            }
            if (odbb(d, xa, xb, ya, bts, 10, "Draw New Message Area")) mMiscFnx.get_block_range("Message Area", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            int q = 10;
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][6], 1990, 0, 1,  1, 10, "x:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][7], 1990, 0, 1,  1, 10, "y:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][8], 1990, 0, 1,  1, 10, "w:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][9], 1990, 0, 1,  1, 10, "h:");
            ya+=4; // spacer
            int frame_size = mLevel.mItem.get_frame_size(num);
            if (odbp(d, xa, xb, ya, bts, 3, 7, frame_size)) mLevel.mItem.set_frame_size(num, frame_size);
            ya+=4; // spacer
            int tc=0, fc=0;
            mMiscFnx.get_int_3216(mLevel.mItem.item[n][13], tc, fc);
            mWidget.mColorSelect(0, xa, xb, ya, bts, 1, tc, 0, "Select Text Color", d);
            mWidget.mColorSelect(0, xa, xb, ya, bts, 1, fc, 0, "Select Frame Color", d);
            mMiscFnx.set_int_3216(mLevel.mItem.item[n][13], tc, fc);
            if (odbb(d, xa, xb, ya, bts, 13, "Edit Message")) mLevel.mItem.edit_pmsg_text(n, 0);
            ya+=bts*2; // leave space for OK and Cancel buttons
            // draw the current one last to ensure it is on top
            mObjectViewer.pop_msg_viewer_pos = ya+bts/2+2;


            char pcc[500];
            sprintf(pcc, "%s", mLevel.mItem.mMessageText.get_text(n).c_str());

            mLevel.mItem.draw_message(n, 1, (xa+xb)/2, mObjectViewer.pop_msg_viewer_pos, 0, 0, pcc ); // show the message
            ya+=bts*8;
         }
         break;
         case 11: // rocket
            odbp(d, xa, xb, ya, bts, 13, 1023, mLevel.mItem.item[n][3]); // stat | fall | ride through door
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 10, "Set Initial Direction")) mMiscFnx.getxy("Initial Direction", 97, 11, n);
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][8], 20,  1, 1,  1, 0, "Maximum Speed:");
            odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][9], 200, 1, 1,  1, 0, "Acceleration:");
            odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][6], 100, 1, 1,  1, 0, "Steerability:");
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 14, mLevel.mItem.item[n][7], 1200, 20, 1,  1, 10, "Damage Range:");
         break;
         case 14: // switch
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            if (odbb(d, xa, xb, ya, bts, 10, "Get New Block Range")) mMiscFnx.get_block_range("Block Range", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][10], 0b0001, "Green:OFF",  "Green:ON",   9+dim,  9, 15+dim, 15);
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][10], 0b0010, "Red:OFF",    "Red:ON",    10+dim, 10, 15+dim, 15);
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][10], 0b0100, "Blue:OFF",   "Blue:ON",   12+dim, 12, 15+dim, 15);
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][10], 0b1000, "Purple:OFF", "Purple:ON",  8+dim,  8, 15+dim, 15);
            mLevel.mItem.set_switch_tile(n);
         break;
         case 15: // sproingy
            odbp(d, xa, xb, ya, bts, 13, 1022, mLevel.mItem.item[n][3]); // stat | fall | carry | carry through door
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][7], 200, 40, 1,  1, 10, "Sproinginess:");
         break;
         case 13: // timer
         {
            int state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time;
            mLevel.mItem.get_timer_flags(mLevel.mItem.item[n][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time);
            int q = 11;
            if (mLevel.mItem.item[n][2]) q = 10;
            odbt(d, xa, xb, ya, bts,  15, mLevel.mItem.item[n][2], q, q, "Draw Mode:Hidden", "Draw Mode:Progress Bar");
            if (mLevel.mItem.item[n][2])
            {
               if (odbb(d, xa, xb, ya, bts, q, "Get New Display Area")) mMiscFnx.get_block_range("Display Area", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
               odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][6], 1990, 0, 1,  1, 10, "x:");
               odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][7], 1990, 0, 1,  1, 10, "y:");
               odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][8], 1990, 0, 1,  1, 10, "w:");
               odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][9], 1990, 0, 1,  1, 10, "h:");
            }
            ya+=4; q = 13; // spacer
            odb0(d, xa, xb, ya, bts,  q, mLevel.mItem.item[n][10],  1000, 0, 1,   1, 10, "Timer 1:", "Off");
            if (mLevel.mItem.item[n][10])
            {
               odbp(d, xa, xb, ya, bts, q, 160, t1_mode); // timer mode
               if (odbb(d, xa, xb, ya, bts, q, "Set Input Event from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 1, t1_mode);
               odb0(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][12],  99, 0, 1,   1, 0, "Input Event:", "Off");
               odb0(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][13],  99, 0, 1,   1, 0, "Output Event:", "Off");
               odbt(d, xa, xb, ya, bts,  15, t1_op_mode, q, q, "Output Mode:One Time", "Output Mode:Continuous");
            }
            ya+=4; q = 14; // spacer
            odb0(d, xa, xb, ya, bts,  q, mLevel.mItem.item[n][11],  1000, 0, 1,   1, 10, "Timer 2:", "Off");
            if (mLevel.mItem.item[n][11])
            {
               odbp(d, xa, xb, ya, bts, q, 160, t2_mode); // timer mode
               if (odbb(d, xa, xb, ya, bts, q, "Set Input Event from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 1, t2_mode);
               odb0(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][14],  99, 0, 1,   1, 0, "Input Event:", "Off");
               odb0(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][15],  99, 0, 1,   1, 0, "Output Event:", "Off");
               odbt(d, xa, xb, ya, bts,  15, t2_op_mode, q, q, "Output Mode:One Time", "Output Mode:Continuous");
            }
            state = 1;
            time = mLevel.mItem.item[n][10]; // always start with full t1_timer
            mLevel.mItem.set_timer_flags(mLevel.mItem.item[n][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time);
         }
         break;
         case 9: // trigger
         {
            if (odbb(d, xa, xb, ya, bts, 14, "Get New Trigger Field")) mMiscFnx.get_block_range("Trigger Rectangle", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            int q = 14;
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][6], 1990, 0, 1,  1, 10, "x:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][7], 1990, 0, 1,  1, 10, "y:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][8], 1990, 0, 1,  1, 10, "w:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][9], 1990, 0, 1,  1, 10, "h:");
            ya+=4; // spacer
            // draw trigger field on/off with optional color select if on
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF","Draw Trigger Field:ON", 15+dim, mLevel.mItem.item[n][2], 15+dim, 15);
            if (mLevel.mItem.item[n][3] & PM_ITEM_TRIGGER_DRAW_ON)
               mWidget.mColorSelect(0, xa, xb, ya, bts, 1, mLevel.mItem.item[n][2], 0, "Select Trigger Field Color", d);
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF       ", "Triggered by Players:ON        ", 9+dim, 9, 15+dim, 15);
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF       ", "Triggered by Enemies:ON        ", 9+dim, 9, 15+dim, 15) ;
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF         ", "Triggered by Items:ON          ", 9+dim, 9, 15+dim, 15 );
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_PSHOT,  "Triggered by Player's Shots:OFF", "Triggered by Player's Shots:ON ", 9+dim, 9, 15+dim, 15 );
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_ESHOT,  "Triggered by Enemy's Shots:OFF ", "Triggered by Enemy's Shots:ON  ", 9+dim, 9, 15+dim, 15 );
            ya+=4; // spacer
            odbb(1, xa, xb, ya, bts, 11, "Continuously Set Event When:");
            odb0(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][11], 99, 0, 1,  1, 0,  "On:",  "-");
            odb0(d, xa, xb, ya, bts, 11, mLevel.mItem.item[n][12], 99, 0, 1,  1, 0,  "Off:", "-");
            ya+=4; // spacer
            odbb(1, xa, xb, ya, bts, 12, "Set Event When Switching:");
            odb0(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][13], 99, 0, 1,  1, 0,  "On:",  "-");
            odb0(d, xa, xb, ya, bts, 12, mLevel.mItem.item[n][14], 99, 0, 1,  1, 0,  "Off:", "-");
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts,  mLevel.mItem.item[n][3], PM_ITEM_TRIGGER_LIFT_ON, "Follows Lift:OFF","Follows Lift:ON ", 6+dim, 6, 15+dim, 15);
            if (mLevel.mItem.item[n][3] & PM_ITEM_TRIGGER_LIFT_ON)
            {
               odbi(d, xa, xb, ya, bts, 6, mLevel.mItem.item[n][10], 39, 0, 1,  1, 0, "Lift Number:");
               odbc(d, xa, xb, ya, bts, 6, 15, 401, n, 0, 0); // X Alignment
               odbc(d, xa, xb, ya, bts, 6, 15, 402, n, 0, 0); // Y Alignment
            }
         }
         break;
         case 16: // block manip
         {
            if (odbb(d, xa, xb, ya, bts, 12, "Get New Block Manip Field")) mMiscFnx.get_block_range("Block Manip Rectangle", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][2],   15+96, mLevel.mItem.item[n][12],  "Draw Block Manip Field:OFF", "Draw Block Manip Field:ON");
            if (mLevel.mItem.item[n][2]) mWidget.mColorSelect(0, xa, xb, ya, bts, 1, mLevel.mItem.item[n][12], 0, "Select Block Manip Field Color", d);
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 14, 301, mLevel.mItem.item[n][3]); // mode
            if (mLevel.mItem.item[n][3]) // all modes except 0
            {
               ya+=2; // spacer
               if (mLevel.mItem.item[n][3] == 4)
               {
                  if (odbb(d, xa, xb, ya, bts, 11, "Set Copy Source")) mMiscFnx.getxy("Copy Source", 95, 0, n);
                  ya+=2; // spacer
               }
               else
               {
                  ya+=2; // spacer
                  odbc(d, xa, xb, ya, bts, 8, 15, 310, n, 10, 1); // Block 1
                  ya+=6; // spacer
                  if (mLevel.mItem.item[n][3] > 1)
                  {
                     odbc(d, xa, xb, ya, bts, 8, 15, 310, n, 11, 2); // Block 2
                     ya+=6; // spacer
                  }
                  if (mLevel.mItem.item[n][3] > 4)
                  {
                     odbc(d, xa, xb, ya, bts, 8, 15, 310, n, 12, 3); // Block 3
                     ya+=6; // spacer
                  }
                  if (mLevel.mItem.item[n][3] > 5)
                  {
                     odbc(d, xa, xb, ya, bts, 8, 15, 310, n, 14, 4); // Block 4
                     ya+=6; // spacer
                  }
               }
               if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
               odb0(d, xa, xb, ya, bts,  13, mLevel.mItem.item[n][1],  99, 0,  1,   1, 0, "Event Trigger:", "Off");
            }
         }
         break;
         case 17: // block damage
         {
            if (odbb(d, xa, xb, ya, bts, 10, "Get New Block Damage Field")) mMiscFnx.get_block_range("Block Damage Rectangle", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            int q = 10;
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][6], 1990, 0, 1,  1, 10, "x:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][7], 1990, 0, 1,  1, 10, "y:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][8], 1990, 0, 1,  1, 10, "w:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][9], 1990, 0, 1,  1, 10, "h:");
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 8, 404, mLevel.mItem.item[n][2]); // draw type
            ya+=4; // spacer
            odbc(d, xa, xb, ya, bts, 8, 15, 96, n, 0, 0); // draw rotation
            ya+=4; // spacer
            odbp(d, xa, xb, ya, bts, 7, 402, mLevel.mItem.item[n][11]); // mode
            int MODE = mLevel.mItem.item[n][11];
            if (MODE == 0) mLevel.mItem.item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
            if (MODE == 2) mLevel.mItem.item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
            if (MODE == 3) mLevel.mItem.item[num][3] &=  ~PM_ITEM_DAMAGE_CURR; // set damage off
            ya+=4; // spacer
            if (MODE == 1) // toggle
               odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_CURR, "Initial State:OFF","Initial State:ON ", q+dim, q, 15+dim, 15);
            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               ya+=4; // spacer
               if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
               odb0(d, xa, xb, ya, bts,  13, mLevel.mItem.item[n][1],  99, 0,  1,   1, 0, "Event Trigger:", "Off");
            }
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_PLAYER,  "Affects Players:OFF",        "Affects Players:ON", 10+dim, 10, 15+dim, 15);
            if (mLevel.mItem.item[n][3] & PM_ITEM_DAMAGE_PLAYER)
            {
               odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_INSTGIB, "Player Instant Death:OFF",   "Player Instant Death:ON", 10+dim, 10, 15+dim, 15);
               if (!(mLevel.mItem.item[n][3] & PM_ITEM_DAMAGE_INSTGIB))  odbi(d, xa, xb, ya, bts, 10, mLevel.mItem.item[n][15], 2000, -2000, 1,  1, 10, "Player Damage:");
            }
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_ENEMY,   "Affects Enemies:OFF",        "Affects Enemies:ON", 10+dim, 10, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_ITEM,    "Affects Items:OFF",          "Affects Items:ON", 10+dim, 10, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_PSHOT,   "Affects Player's Shots:OFF", "Affects Player's Shots:ON", 10+dim, 10, 15+dim, 15);
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_ESHOT,   "Affects Enemy's Shots:OFF",  "Affects Enemy's Shots:ON", 10+dim, 10, 15+dim, 15);
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_LIFT_ON, "Follows Lift:OFF",           "Follows Lift:ON ", 6+dim, 6, 15+dim, 15);
            if (mLevel.mItem.item[n][3] & PM_ITEM_DAMAGE_LIFT_ON)
            {
               odbi(d, xa, xb, ya, bts, 6, mLevel.mItem.item[n][10], 39, 0, 1,  1, 0, "Lift Number:");
               odbt(d, xa, xb, ya, bts, mLevel.mItem.item[n][3], PM_ITEM_DAMAGE_LIFT_MS, "Mirror Lift:OFF",           "Mirror Lift:ON ", 6+dim, 6, 15+dim, 15);
               if (!(mLevel.mItem.item[n][3] & PM_ITEM_DAMAGE_LIFT_MS))
               {
                  odbc(d, xa, xb, ya, bts, 6, 15, 411, n, 0, 0); // X Alignment
                  odbc(d, xa, xb, ya, bts, 6, 15, 412, n, 0, 0); // Y Alignment
               }
            }
         }
         break;
         case 18: // gate
         {
            odbp(d, xa, xb, ya, bts, 8, 405, mLevel.mItem.item[n][1]); // draw mode
            ya+=4; // spacer
            odbi(d, xa, xb, ya, bts, 10, mLevel.mItem.item[n][6], 99, 1, 1,  1, 10, "Level:");

            ya+=4; // spacer
            int q=12; // mode color
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][10], 400, 20, 1,  1, 10, "Level Icon size:");

            ya+=4; // spacer
            q = 13;
            odbt(d, xa, xb, ya, bts, 15, mLevelEditor.show_gate_info,        q, q,  "Hide Hover Info Pages",        "Show Hover Info Pages");
            if (mLevelEditor.show_gate_info) mLevelEditor.show_gate_info_item = n;

            char msg[200];
            sprintf(msg, "Gate Info Page:%d", mLevelEditor.show_gate_info_page);
            if (odbb(d, xa, xb, ya, bts, q, msg)) if (++mLevelEditor.show_gate_info_page > 3) mLevelEditor.show_gate_info_page = 0;

            ya+=4; // spacer
            //odbt(d, xa, xb, ya, bts, mItem.item[n][3], PM_ITEM_GATE_HOVER_PAGE_DIR, "Hover Pages Direction:Below","Hover Pages Direction:Above", q, q, 15, 15);
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][11], 400, 20, 1,  1, 10, "Hover Level Icon size:");



         }
         break;
         case 19: // hider
         {
            if (odbb(d, xa, xb, ya, bts, 10, "Get New Hidden Area")) mMiscFnx.get_block_range("Hidden Area", mLevel.mItem.item[n][6], mLevel.mItem.item[n][7], mLevel.mItem.item[n][8], mLevel.mItem.item[n][9], 1);
            int q = 10;
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][6], 1990, 0, 1,  1, 10, "x:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][7], 1990, 0, 1,  1, 10, "y:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][8], 1990, 0, 1,  1, 10, "w:");
            odbi(d, xa, xb, ya, bts, q, mLevel.mItem.item[n][9], 1990, 0, 1,  1, 10, "h:");
            ya+=4; // spacer
            q=7; // mode color
            odbp(d, xa, xb, ya, bts, q, 403, mLevel.mItem.item[n][3]); // hider mode
            int MODE = mLevel.mItem.item[n][3];
            if (MODE == 0) mLevel.mItem.item[num][2] = 0;
            if (MODE == 1) mLevel.mItem.item[num][2] = 1;
            if (MODE == 2) mLevel.mItem.item[num][2] = 0;
            if (MODE == 3) mLevel.mItem.item[num][2] = 1;
            if ((MODE == 4) || (MODE == 5))
            {
               odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][2],  q, q,  "Initial State:Show", "Initial State:Hide ");
            }
            if ((MODE == 2) || (MODE == 3) || (MODE == 4) || (MODE == 5))
            {
               ya+=4; // spacer
               if (odbb(d, xa, xb, ya, bts, 13, "Set Event Trigger from Object")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
               odb0(d, xa, xb, ya, bts,  13, mLevel.mItem.item[n][1],  99, 0,  1,   1, 0, "Event Trigger:", "Off");
            }
            ya+=4; // spacer
            odbt(d, xa, xb, ya, bts, 15, mLevel.mItem.item[n][10],   10+96, 10,  "Blind Erase:OFF", "Blind Erase:ON");


         }
         break;
     }
   }
   // set height
   if (obt != 4)
   {
      int llo = (3 - num_legend_lines)*8; // legend line offset
      mLevelEditor.mWM.mW[7].rect.h = ya-y1+32-llo;
      mLevelEditor.mWM.mW[7].set_size(mLevelEditor.mWM.mW[7].rect.w, mLevelEditor.mWM.mW[7].rect.h);
   }
}












void mwObjectViewer::ov_title(int x1, int x2, int y1, int y2, int color)
{
   int xc = (x1+x2)/2;
   int yt = y1+14;

   // legend line text
   char lmsg[6][80];
   for (int x=0; x<6; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[6];
   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused
   legend_color[5] = 0;   // unused

   num_legend_lines = 0;

   // flash color for location
   legend_line_highlight == 1 ? legend_color[1] = mColor.flash_color : legend_color[1] = 13;

   // title bar
   window_title[0] = 0;

   if (obt == 4)  // lifts
   {
      sprintf(window_title, "Viewer - Lift:%d", num);
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      al_draw_textf(mFont.pr8, mColor.pc[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "Lift %d of %d", num+1, mLevel.mLift.get_num_lifts());
   }
   if (obt == 3)  // enemies
   {
      int type = mLevel.mEnemy.Ei[num][0];
      sprintf(window_title, "Viewer - Enemy:%d", num);
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      mLevel.mEnemy.draw_enemy(num, 1, xc-92, yt+1);
      al_draw_textf(mFont.pr8, mColor.pc[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "%s %d of %d", mLevel.mEnemy.enemy_name[type][0],1+num - mLevel.mEnemy.e_first_num[type],mLevel.mEnemy.e_num_of_type[type]);

      // legend line 1 is "[object] Location" by default
      sprintf(lmsg[1],"%s Location", mLevel.mEnemy.enemy_name[type][0]);

      // default number of legend lines
      num_legend_lines = 2;


      switch (type)
      {
         case 1: // bouncer
         break;
         case 2: // cannon
            num_legend_lines = 3;
            sprintf(lmsg[2],"Bullet Proximity");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 3: // archwagon
            num_legend_lines = 3;
            sprintf(lmsg[2],"Bullet Proximity");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 4: // blockwalk
         break;
         case 5: // jumpworm
         break;
         case 6: // flapper
            num_legend_lines = 4;
            sprintf(lmsg[2],"Bullet Proximity");
            sprintf(lmsg[3],"Height Above Player");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
            legend_line_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 10;
         break;
         case 7: // vinepod
         {
            sprintf(lmsg[2],"Extended Position");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
            int legend_count = 3;
            if (mLevel.mEnemy.Ei[num][20] & PM_ENEMY_VINEPOD_USE_TRIGGER)
            {
               sprintf(lmsg[legend_count],"Trigger Box");
               legend_line_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 14;
               legend_count++;
            }
            if (mLevel.mEnemy.Ei[num][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
            {
               sprintf(lmsg[legend_count],"Control Point 1");
               legend_line_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 6;
               legend_count++;
               sprintf(lmsg[legend_count],"Control Point 2");
               legend_line_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 7;
               legend_count++;
            }
            num_legend_lines = legend_count;
         }
         break;
         case 8: // trakbot
            num_legend_lines = 3;
            sprintf(lmsg[2],"Bullet Proximity");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 9: // cloner
            num_legend_lines = 4;
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 11;
            legend_line_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 10;
         break;
      }
   }
   if (obt == 2)  // items
   {
      int type = mLevel.mItem.item[num][0];
      sprintf(window_title, "Viewer - Item:%d", num);
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      mLevel.mItem.draw_item(num, 1, xc-94, yt+1);
      al_draw_textf(mFont.pr8, mColor.pc[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "%s %d of %d", mLevel.mItem.item_name[type], 1+num - mLevel.mItem.item_first_num[type], mLevel.mItem.item_num_of_type[type]);

      // legend line 1 is "[object] Location" by default
      sprintf(lmsg[1],"%s Location", mLevel.mItem.item_name[type]);

      // default number of legend lines
      num_legend_lines = 2;


      switch (type)
      {
         case 1: // door
         {
            num_legend_lines = 3;
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
            if (mLevel.mItem.item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((mLevel.mItem.item[i][0] == 1) && (mLevel.mItem.item[i][9] == num))
                  {
                     num_src++;
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (mLevel.mItem.item[num][8] == 1)  // draw destination
            {
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2: // bonus
         break;
         case 3: // exit
         break;
         case 4: // key
            num_legend_lines = 3;
            sprintf(lmsg[2],"Block Range");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 5: // start
         break;
         case 6: // orb
         break;
         case 7: // mine
         break;
         case 8: // bomb
            num_legend_lines = 3;
            sprintf(lmsg[2],"Damage Range");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 9: // trigger
            num_legend_lines = 3;
            sprintf(lmsg[2],"Trigger Field");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 10: // message
            num_legend_lines = 3;
            sprintf(lmsg[1],"Scroll Location");
            sprintf(lmsg[2],"Message Area");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 11: // rocket
            num_legend_lines = 3;
            sprintf(lmsg[2],"Damage Range");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 13: // timer
            num_legend_lines = 3;
            sprintf(lmsg[2],"Display Area");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 14: // switch
            num_legend_lines = 3;
            sprintf(lmsg[2],"Block Range");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 15: // sproingy
            num_legend_lines = 3;
            sprintf(lmsg[2],"Jump Height");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 16: // block manip
            num_legend_lines = 3;
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 12;
            sprintf(lmsg[2],"Manip Field");
            if (mLevel.mItem.item[num][3] == 4)
            {
               num_legend_lines = 4;
               sprintf(lmsg[3],"Source Field");
               legend_line_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 11;
            }
         break;
         case 17: // block damage
            num_legend_lines = 3;
            sprintf(lmsg[2],"Damage Area");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 18: // gate
         break;
         case 19: // hider
            num_legend_lines = 3;
            sprintf(lmsg[2],"Hidden Area");
            legend_line_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;

      } // end of switch case
   }  // end of items

   if (num_legend_lines > 0)
   {
      al_draw_text(mFont.pr8, mColor.pc[legend_color[0]], xc, y2-37 + (4-num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(xc-100, y2-38+ (4-num_legend_lines)*8, xc+100, y2-1, mColor.pc[color], 1); // big frame
      al_draw_rectangle(xc-100, y2-38+ (4-num_legend_lines)*8, xc+100, y2-28+ (4-num_legend_lines)*8, mColor.pc[color], 1); // top frame
   }

   for (int x=1; x<num_legend_lines; x++)// draw text lines
      al_draw_text(mFont.pr8, mColor.pc[legend_color[x]], xc, y2-26+(3-num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);
}




































void mwObjectViewer::object_viewer(int o, int n)
{
   obt = o;
   num = n;
   mLevelEditor.set_mode(4); // object viewer
   al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
}


void mwObjectViewer::draw(mwWindow & w)
{
   ov_check_if_valid();
   ov_set_width(w);
   ov_title(w.rect.x1, w.rect.x2, w.rect.y1, w.rect.y2, w.color);
   w.set_title(window_title);
   ov_draw_buttons(w.rect.x1, w.rect.y1, w.rect.x2, w.rect.y2, w.disable_input);
}

void mwObjectViewer::draw_level_editor_background_overlays(int mouse_on_window)
{
   // if current object is message, show all messages
   if ((obt == 2) && (mLevel.mItem.item[num][0] == 10))
   {
      for (int i=0; i<500; i++)
         if (mLevel.mItem.item[i][0] == 10) mLevel.mItem.draw_message(i, 0);
   }

   // if mouse on legend line, show highlight
   legend_line_highlight = 0;
   int y1_legend = mLevelEditor.mWM.mW[7].rect.y2 - 34 + (5-num_legend_lines)*8; // legend pos
   int y2_legend = y1_legend + (num_legend_lines-1)*8;
   if ((mInput.mouse_x > mLevelEditor.mWM.mW[7].rect.x1) && (mInput.mouse_x < mLevelEditor.mWM.mW[7].rect.x2) && (mInput.mouse_y > y1_legend) && (mInput.mouse_y < y2_legend)) // is mouse on legend
      legend_line_highlight = ((mInput.mouse_y - y1_legend) / 8) + 1; // which legend line are we on?

   ov_draw_overlays(legend_line_highlight);
}

