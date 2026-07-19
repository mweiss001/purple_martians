#include "pm.h"
#include "mwEditorMain.h"

#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditSelection.h"
#include "mwEnemy.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGlobalLevelTool.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwMiscFnx.h"
#include "mwObjectViewer.h"
#include "mwScreen.h"
#include "mwSelectionWindow.h"
#include "mwTileEditor.h"
#include "mwTileSets.h"
#include "mwTriggerEvent.h"
#include "mwVisualLevel.h"
#include "mwWidget.h"
#include "mwWindowManager.h"


mwEditorMain mEditorMain;

mwEditorMain::mwEditorMain() { init(); }

void mwEditorMain::init()
{

   draw_item_type = 1;
   draw_item_num  = 0;


   show_flag_details = 1;
   show_non_default_blocks = 1;
   show_tile_overlays = 1;


   // set all filters on
   for (int i=0; i<5; i++)
      for (int j=0; j<20; j++) obj_filter[i][j] = 1;

   collapsed = 0;
   filter_mode = 1;




}





char* mwEditorMain::get_text_description_of_block_based_on_flags(int flags, char * msg)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PSHOT)  sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void mwEditorMain::show_draw_item_cursor(void)
{
   int x = mWM.gx;
   int y = mWM.gy;


//   al_draw_textf(mFont.pr8, mColor.pc[15], 100, 100, 0, "point_item_type:%d  gx:%d  gy:%d", mWM.mW[1].point_item_type, mWM.gx, mWM.gy);



   if (point_item_type > -1) // if mouse pointer on window, do not show draw item
   {
      int type = draw_item_type;
      int num  = draw_item_num;
      switch (type)
      {
         case 1: // block
            if (show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(num, x*20, y*20);
            else al_draw_bitmap(mBitmap.tile[num & PM_BTILE_TILENUM_MASK], x*20, y*20, 0);
         break;
         case 2: // item
            mItem.draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            mEnemy.draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = mSelectionWindow.pdes[num].ia[1]; // bmp or ans
            if (a > 999) a = mBitmap.zz[5][a-1000]; // ans
            al_draw_bitmap(mBitmap.sprite[a], x*20, y*20, 0);
         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+21, y*20+21, mColor.pc[15], 1);
   }
}

void mwEditorMain::show_item_info(int x, int y, int color, int type, int num)
{
   char msg[1024];
   int a, b;
   switch (type)
   {
      case 1:
         if (show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(num, x, y);
         else al_draw_bitmap(mBitmap.tile[num & PM_BTILE_TILENUM_MASK], x, y, 0);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "Block #%d",num & PM_BTILE_TILENUM_MASK);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%s", get_text_description_of_block_based_on_flags(num, msg));
      break;
      case 2:
         mItem.draw_item(num, 1, x, y);
         a = mItem.item[num][0]; // type
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "%s", mItem.item_name[a]);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%d of %d", 1+num - mItem.item_first_num[a], mItem.item_num_of_type[a]);
      break;
      case 3:
         mEnemy.draw_enemy(num, 1, x, y);
         a = mEnemy.Ei[num][0]; // type
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "%s", mEnemy.enemy_name[a][0]);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%d of %d", 1+num - mEnemy.e_first_num[a], mEnemy.e_num_of_type[a]);
      break;
      case 4:
      {
         int col = mLift.cur[num].color;
         int width = mLift.cur[num].w;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, mColor.pc[col+((9-a)*16)], 1 );
         al_draw_text(mFont.pr8, mColor.pc[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, mLift.cur[num].lift_name);
      }
      break;
      case 5:
         a = mSelectionWindow.pdes[num].ia[1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = mBitmap.zz[5][a-1000]; // ans
         al_draw_bitmap(mBitmap.sprite[b], x, y, 0);
         al_draw_text(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%s", mSelectionWindow.pdes[num].name.c_str());
      break;
   }
}

void mwEditorMain::find_point_item(void)
{
   // find point item
   point_item_type = 1; // block by default
   point_item_num = mLevel.l[mWM.gx][mWM.gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[20][2];                    // array of objects found
   for (int a=0; a<max_ob; a++)      // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((mItem.item[i][0]) && (ob < max_ob))
      {
         int x = mItem.item[i][4];
         int y = mItem.item[i][5];
         if ( (mWM.hx >= x) && (mWM.hx <= x+19) && (mWM.hy > y) && (mWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((mEnemy.Ei[e][0]) && (ob < max_ob))
      {
         int x = mEnemy.Ef[e][0];
         int y = mEnemy.Ef[e][1];
         if ( (mWM.hx >= x) && (mWM.hx <= x+19) && (mWM.hy > y) && (mWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<NUM_LIFTS; l++) // check for lifts
      if (mLift.cur[l].active)
      {
         int x = mLift.cur[l].x;
         int y = mLift.cur[l].y;
         if ( (mWM.hx >= x) && (mWM.hx <= x+19) && (mWM.hy > y) && (mWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 4;
             mo[ob][1] = l;
             ob++;
         }
      }
   //al_draw_textf(mFont.pr8, mColor.pc[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(mFont.pr8, mColor.pc[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mInput.mouse_x % 20;     // mouse position relative to block boundary
      int ss = 20/ob;                   // step space
      int of = mm / ss;                 // convert to offset into ob array
      point_item_type = mo[of][0];
      point_item_num  = mo[of][1];
      //al_draw_textf(mFont.pr8, mColor.pc[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }

//   al_draw_textf(mFont.pr8, mColor.pc[15], 100, 100, 0, "point_item_type:%d  gx:%d  gy:%d", mWM.mW[1].point_item_type, mWM.gx, mWM.gy);




}

void mwEditorMain::process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      // don't allow drag draw selection unless draw type is block
      if (draw_item_type != 1) while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

      int din = draw_item_num; // shorter variable name
      switch (draw_item_type)
      {
         case 1: // block
         {
            if (mWM.get_new_box("preview", true))
            {
               mTileSets.drawRect(0);
               mScreen.init_level_background();
               al_set_target_backbuffer(mDisplay.display);
            }
         }
         break;
         case 2: // item
         {
            int type = mItem.item[din][0];
            int ofx = mWM.gx*20 - mItem.item[din][4]; // get offset of move in 2000 format
            int ofy = mWM.gy*20 - mItem.item[din][5];
            int c = mItem.get_empty_item(); // get a place to put it
            //printf("din:%d c:%d\n", din, c);
            if (c == -1)  break;
            for (int b=0; b<16; b++) mItem.item[c][b] = mItem.item[din][b]; // copy from draw item

            mItem.item[c][4] += ofx; // adjust with offsets
            mItem.item[c][5] += ofy;

            if (mItem.item_secondary67(type))
            {
               mItem.item[c][6] += ofx; // adjust with offsets
               mItem.item[c][7] += ofy;
            }
            if (type == 10) strcpy(mItem.pmsgtext[c], mItem.pmsgtext[din]); // message
            mItem.sort_item(1);
         }
         break;
         case 3: // enemy
         {
            int type = mEnemy.Ei[din][0];

            int ofx = mWM.gx*20 - mEnemy.Ef[din][0]; // get offset of move in 2000 format
            int ofy = mWM.gy*20 - mEnemy.Ef[din][1];

            int c = mEnemy.get_empty_enemy(type); // get a place to put it
            if (c == -1)  break;
            for (int x=0; x<32; x++) mEnemy.Ei[c][x] = mEnemy.Ei[din][x];
            for (int x=0; x<16; x++) mEnemy.Ef[c][x] = mEnemy.Ef[din][x];

            mEnemy.Ef[c][0] += ofx;  // apply offsets
            mEnemy.Ef[c][1] += ofy;

            if (type == 7) // vinepod
            {
               for (int i=3; i<13; i+=2) // all spline and trigger x y also
               {
                  mEnemy.Ei[c][i+0] = mEnemy.Ei[din][i+0] + ofx;
                  mEnemy.Ei[c][i+1] = mEnemy.Ei[din][i+1] + ofy;
               }
            }
            if (type == 9) // cloner
            {
               //if (al_show_native_message_box(mDisplay.display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if (mInput.SHFT()) // move stuff also
               {
                  mEnemy.Ei[c][15] = mEnemy.Ei[din][15] + ofx;
                  mEnemy.Ei[c][16] = mEnemy.Ei[din][16] + ofy;
                  mEnemy.Ei[c][17] = mEnemy.Ei[din][17] + ofx;
                  mEnemy.Ei[c][18] = mEnemy.Ei[din][18] + ofy;
               }
            }
            mEnemy.sort_enemy();
         }
         break;
         case 5: // PDE
            if ((mSelectionWindow.pdes[din].ia[0] > 99) && (mSelectionWindow.pdes[din].ia[0] < 200)) // PDE item
            {
               int d = mItem.get_empty_item(); // get a place to put it
               if (d == -1)  break;
               // copy from pde
               for (int x=0; x<16; x++) // item
                  mItem.item[d][x] = mSelectionWindow.pdes[din].ia[x];
               mItem.item[d][0] -= 100;
               mItem.item[d][4] = mWM.gx*20;
               mItem.item[d][5] = mWM.gy*20;
               if (mItem.item[d][0] == 4)
               {
                  mItem.itemf[d][0] = mItem.item[d][4];
                  mItem.itemf[d][1] = mItem.item[d][5];
                  mMiscFnx.get_block_range("Block Range", mItem.item[d][6], mItem.item[d][7], mItem.item[d][8], mItem.item[d][9], 1);
               }
               mItem.sort_item(1);
            }
            if (mSelectionWindow.pdes[din].ia[0] < 99) // PDE enemy
            {
               int d = mEnemy.get_empty_enemy(); // get a place to put it
               if (d == -1)  break;
               for (int x=0; x<32; x++) mEnemy.Ei[d][x] = mSelectionWindow.pdes[din].ia[x];
               for (int x=0; x<16; x++) mEnemy.Ef[d][x] = mSelectionWindow.pdes[din].fa[x];
               mEnemy.Ef[d][0] = mWM.gx*20;  // set new x,y
               mEnemy.Ef[d][1] = mWM.gy*20;
               mEnemy.sort_enemy();
            }
         break;
      } // end of switch case
   } // end of mInput.mouse_b[1][0]
   if (mInput.mouse_b[2][0])
   {
      switch (point_item_type)
      {
         case 1:
            sprintf(mMenu.menu_string[2], "Copy Block    ");
            sprintf(mMenu.menu_string[3], "              ");
            sprintf(mMenu.menu_string[4], "                ");
         break;
         case 2:
            sprintf(mMenu.menu_string[2], "Copy %s  ",  mItem.item_name[mItem.item[point_item_num][0]]);
            sprintf(mMenu.menu_string[3], "View %s  ",  mItem.item_name[mItem.item[point_item_num][0]]);
            sprintf(mMenu.menu_string[4], "Delete %s ", mItem.item_name[mItem.item[point_item_num][0]]);
         break;
         case 3:
            sprintf(mMenu.menu_string[2], "Copy %s  ",  (const char *)mEnemy.enemy_name[mEnemy.Ei[point_item_num][0]][0]);
            sprintf(mMenu.menu_string[3], "View %s  ",  (const char *)mEnemy.enemy_name[mEnemy.Ei[point_item_num][0]][0]);
            sprintf(mMenu.menu_string[4], "Delete %s ", (const char *)mEnemy.enemy_name[mEnemy.Ei[point_item_num][0]][0]);
         break;
         case 4:
            sprintf(mMenu.menu_string[2], "              ");
            sprintf(mMenu.menu_string[3], "View Lift '%s'",   mLift.cur[point_item_num].lift_name);
            sprintf(mMenu.menu_string[4], "Delete Lift '%s'", mLift.cur[point_item_num].lift_name);
         break;

         default:
            sprintf(mMenu.menu_string[2], "              ");
            sprintf(mMenu.menu_string[3], "              ");
            sprintf(mMenu.menu_string[4], "                ");
         break;

      }

      switch (mMenu.pmenu(2, 0, -20, 2))
      {
         case 2:  // copy
            if (point_item_type > 0 && point_item_type < 4)
            {
               draw_item_type = point_item_type;
               draw_item_num  = point_item_num;
            }
         break;
         case 3:  // view
            if (point_item_type > 1) mObjectViewer.object_viewer(point_item_type, point_item_num);
         break;
         case 4:  // delete
            switch (point_item_type)
            {
               case 1: // delete block
                    mLevel.l[mWM.gx][mWM.gy] = 0;
               break;
               case 2: // delete item
                  if ((draw_item_type == 2) && (draw_item_num == point_item_num)) // are you deleting the draw item?
                  {
                     draw_item_type = 1;
                     draw_item_num = 0;
                  }
                  mItem.clear_item(point_item_num);
                  mItem.sort_item(1);
               break;
               case 3: // delete enemy
                  if ((draw_item_type == 3) && (draw_item_num == point_item_num)) // are you deleting the draw item?
                  {
                     draw_item_type = 1;
                     draw_item_num = 0;
                  }
                  mEnemy.clear_enemy(point_item_num);
                  mEnemy.sort_enemy();
               break;
               case 4: // delete lift
                  mLift.erase_lift(point_item_num);
               break;
            }
         break;
         case 5: break; // menu divider
         case 6: mWM.set_level_editor_mode(2); break;   // edit selection
         case 7: mWM.set_level_editor_mode(3); break;   // group edit
         case 8: mWM.set_level_editor_mode(9); break;   // tile helper
         case 9: mWM.mW[1].active = 1; break; // status_window
         case 10: mWM.mW[2].active = 1; break; // select_window
         case 12: // new level
         if (al_show_native_message_box(mDisplay.display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            mLevel.zero_level_data();
            mLevel.save_level_prompt();
         }
         mLevel.load_level(mLevel.last_level_loaded, 0, 1); // blind load
         break;
         case 13: // load level
            mLevel.load_level_prompt();
            mEnemy.sort_enemy();
            mItem.sort_item(1);
         break;
         case 14: mLevel.save_level(mLevel.last_level_loaded); break; // save level
         case 15: mWM.active = 0; break; // save and exit
         case 16: mHelp.help("Level Editor Basics"); break;// help
         case 17: mWM.active = 0; break; // exit
      } // end of switch case
   } // end of mInput.mouse_b[2][0]
}





void mwEditorMain::draw_status_window(mwRect<int> rect, int d, int have_focus)
{
   int color = 9;

   int x1 = rect.x1;
   int y1 = rect.y1;
   int x2 = rect.x2;
   int y2 = rect.y2;


   int y3 = y1 + 12; // start of second line
   int y4 = y3 + 12; // start of items


   // frame entire window
   rect.draw_rectangle(mColor.pc[color], 1);


   // make rect for draw item
   mwRect<int> draw_item_rect = mwRect<int>::fromX1Y1X2Y2(x1, y4-2, x1+160, y2);

   // frame draw item
   draw_item_rect.draw_rectangle(mColor.pc[color], 1);


   // make rect for view item
   mwRect<int> view_item_rect = mwRect<int>::fromX1Y1X2Y2(x1+160, y4-2, x2, y2);

   // frame view item
   view_item_rect.draw_rectangle(mColor.pc[color], 1);


   // make rect for title bar
   mwRect<int> title_bar_rect = mwRect<int>::fromX1Y1X2Y2(x1, y1, x2, y1+11);

   // title bar background color
   title_bar_rect.draw_filled_rectangle(mColor.pc[9+192]);

   // detect mouse on title bar
   int title_frame_color = color;
   if ((have_focus) && title_bar_rect.contains(mInput.mouse_x, mInput.mouse_y)) title_frame_color = 14; // highlight to indicate move is possible

   // frame title bar
   title_bar_rect.draw_rectangle(mColor.pc[title_frame_color], 1);


   al_draw_textf(mFont.pr8, mColor.pc[9],  x1+2,   y1+2, 0, "Status Window   level:%d ", mLevel.last_level_loaded);
   al_draw_textf(mFont.pr8, mColor.pc[15], x1+178, y1+2, 0, "%d ", mLevel.last_level_loaded);

   int mow = mWM.is_mouse_on_any_window();
   if (mow)
   {
      al_draw_textf(mFont.pr8, mColor.pc[15], x1+222, y1+2, 0, "x:-- y:-- ");
      point_item_type = -1;
   }
   else
   {
      al_draw_textf(mFont.pr8, mColor.pc[15], x1+222, y1+2, 0, "x:%-2d y:%-2d ", mWM.gx, mWM.gy);
      find_point_item();
   }

   al_draw_text( mFont.pr8, mColor.pc[9],  x1+222, y1+2, 0, "x:");
   al_draw_text( mFont.pr8, mColor.pc[9],  x1+262, y1+2, 0, "y:");

   int by1 = y1+2;
   if (mWidget.buttont(x2-10, by1, x2-2,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mWM.mW[1].active = 0;
   if (mWidget.buttont(x2-22, by1, x2-14, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) mHelp.help("Status Window");

   // this ugly hack is to prevent the draw item block flag toggles from capturing the mouse when moving selection windows over them
   if (mWM.mW[2].moving) d = 1;
   else d = 0;


   // second line with controls
   by1 = y3+1;

   // tile draw mode (tile | flag | both
   al_draw_textf(mFont.pr8, mColor.pc[9],  x1+2, by1, 0, "Tile Draw Mode:");
   mWidget.buttonp(x1+122, by1, x1+122+32, 9, 600, 0,0,0,   0,-1,9,0,  0,0,0,d, draw_tile_mode);


   // draw vline at middle to separate draw and show controls
   int x3 = x1 + 160; // x middle
   al_draw_line(x3, y3-1, x3, y4-1, mColor.pc[color], 1);
   x3+=2;


   // show flags toggle
   mWidget.mCheckBox(1, x3, 50, 1, by1-1, 9, -1, show_flag_details, "flags", color, 15, d);
   x3+=64;

   // show non default blocks toggle
   if (mWidget.mCheckBox(1, x3, 50, 1, by1-1, 9, -1, show_non_default_blocks, "ndf", color, 15, d))
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }
   x3+=48;

   // force showing of tile overlay toggle
   if (mWidget.mCheckBox(1, x3, 50, 1, by1-1, 9, -1, show_tile_overlays, "ovr", color, 15, d))
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }


   // draw item area
   al_draw_text(mFont.pr8, mColor.pc[15], x1 + 24,  y4, 0, "Draw Item   ");
   al_draw_text(mFont.pr8, mColor.pc[14], x1 + 100, y4, 0, "mouse");
   al_draw_text(mFont.pr8, mColor.pc[14], x1 + 143, y4, 0, "b1");
   show_item_info(                     x1 + 2,   y4+7, 9, draw_item_type, draw_item_num);
   if (draw_item_type == 1 && show_flag_details) mBitmapTools.draw_flags(x1+4, y4+34, draw_item_num, status_window_has_mouse, d, 1, 0); // flags


   // view item area
   al_draw_text(mFont.pr8, mColor.pc[15], x1 + 184, y4, 0, "View Item ");
   al_draw_text(mFont.pr8, mColor.pc[14], x1 + 261, y4, 0, "mouse");
   al_draw_text(mFont.pr8, mColor.pc[14], x1 + 303, y4, 0, "b2");
   show_item_info(                     x1 + 162, y4+7, 9, point_item_type, point_item_num);
   if (point_item_type == 1 && show_flag_details) mBitmapTools.draw_flags(x1+164, y4+34, point_item_num, mow, 1, 0, 1); // flags
}


void mwEditorMain::draw_filter_window(mwRect<int> &rect, int d)
{

   int fs = 12;   // frame size
   int y = rect.y1+fs; // button y position

   int bts = 12;    // button size

   int tc1 = 15;    // text color 1
   int tc2 = 15;    // text color 2
   int fc1 = 15+64; // frame color 1
   int fc2 = 4;     // frame color 2

   int tl=0; // text_lines
   if (filter_mode == 1) tl = 28*bts-bts/2;
   if (filter_mode == 2) tl = 30*bts-bts;
   if (filter_mode == 3) tl = 32*bts-bts;

   if (collapsed) tl = -bts+2;

   int y2 = rect.y1+tl+fs*2-2; // pre calc

   // adjust size
   rect.setX1Y1X2Y2(rect.x1, rect.y1, rect.x2, y2);

   // erase background
   rect.draw_filled_rectangle(mColor.pc[0]);

   // draw frame around filter buttons
   int ci = 16; // color inc
   for (int q=0; q<fs; q++)
      rect.draw_rectangle(mColor.pc[12+32+(q*ci)], 1, -q);

   al_draw_text(mFont.pr8, mColor.pc[15], rect.XCenter(), rect.y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   int by1 = rect.y1+2;
   mWidget.toggle(rect.x2-10, by1, rect.x2-2, 8, 0,0,0,0, 0,0,0,0, 1,0,0,d, collapsed,  "-", "+", tc1, tc2, -1, -1);






   // detect mouse click before toggles, but don't do anything until after the toggles change
   int refresh_selection = 0;
   if ((filter_mode == 3) && (mEditSelection.copy_mode) && (mInput.mouse_b[1][0])) refresh_selection = 1;






   if (!collapsed)
   {
      if (filter_mode > 2) // add blocks and flags
      {
         mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[1][1],  "Blocks", "Blocks", tc1, tc2, fc1, fc2);
         mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[1][2],  "Flags",  "Flags",  tc1, tc2, fc1, fc2);
      }
      if (filter_mode > 1) // add lifts
      {
         mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[4][1],  "Lifts",  "Lifts",  tc1, tc2, fc1, fc2);
         y+=bts/2;
      }

      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][1],  "Bouncr", "Bouncr", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][2],  "Cannon", "Cannon", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][3],  "Arcwgn", "Arcwgn", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][4],  "BlkWlk", "BlkWlk", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][5],  "Jmpwrm", "Jmpwrm", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][6],  "Flappr", "Flappr", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][7],  "VinPod", "VinPod", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][8],  "Trakbt", "Trakbt", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][9],  "Cloner", "Cloner", tc1, tc2, fc1, fc2);

      y+=bts/2;
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][1],  "Door",   "Door",   tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][2],  "Bonus",  "Bonus",  tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][3],  "Exit",   "Exit",   tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][4],  "Key",    "Key",    tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][5],  "Start",  "Start",  tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][6],  "Orb",    "Orb",    tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][7],  "Mine",   "Mine",   tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][8],  "Bomb",   "Bomb",   tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][9],  "Triggr", "Triggr", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][10], "Messge", "Messge", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][11], "Rocket", "Rocket", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][13], "Timer",  "Timer",  tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][14], "Switch", "Switch", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][15], "Spring", "Spring", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][16], "Blk Mn", "Blk Mn", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][17], "Blk Dm", "Blk Dm", tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][18], "Gate",   "Gate",   tc1, tc2, fc1, fc2);
      mWidget.toggle(rect.x1+fs, y, rect.x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][19], "Hider",  "Hider",  tc1, tc2, fc1, fc2);
   }


   if (!d && mInput.mouse_b[2][0] && rect.contains(mInput.mouse_x, mInput.mouse_y))
   {
      refresh_selection = 1;

      sprintf(mMenu.menu_string[0],"Filters");
      sprintf(mMenu.menu_string[1],"---------------");
      sprintf(mMenu.menu_string[2],"All On");
      sprintf(mMenu.menu_string[3],"All Items On");
      sprintf(mMenu.menu_string[4],"All Enemies On");
      sprintf(mMenu.menu_string[5],"All Off");
      sprintf(mMenu.menu_string[6],"All Items Off");
      sprintf(mMenu.menu_string[7],"All Enemies Off");
      sprintf(mMenu.menu_string[8],"end");
      switch (mMenu.pmenu(6, 13, -20, 2))
      {
         case 2:
            for (int i=0; i<5; i++)
               for (int j=0; j<20; j++)
                  mEditorMain.obj_filter[i][j] = 1;
            break;
         case 3:
            for (int j=0; j<20; j++)
               mEditorMain.obj_filter[2][j] = 1;
            break;
         case 4:
            for (int j=0; j<20; j++)
               mEditorMain.obj_filter[3][j] = 1;
            break;
         case 5:
            for (int i=0; i<5; i++)
               for (int j=0; j<20; j++)
                  mEditorMain.obj_filter[i][j] = 0;
            break;
         case 6:
            for (int j=0; j<20; j++)
               mEditorMain.obj_filter[2][j] = 0;
            break;
         case 7:
            for (int j=0; j<20; j++)
               mEditorMain.obj_filter[3][j] = 0;
            break;
      }

      if ((mEditorMain.filter_mode == 3) && (mEditSelection.copy_mode))
      {
         mEditSelection.fill_ft_variables_from_selection(0);
         mEditSelection.draw_fsel();
      }
   }


   if (refresh_selection) mEditSelection.draw_fsel();







}









void mwEditorMain::cm_process_menu_bar(int d)
{
   char msg[1024];
   al_set_target_backbuffer(mDisplay.display);

   mWM.mW[8].set_pos(0, 0);
   mWM.mW[8].set_size(mDisplay.SCREEN_W, BORDER_WIDTH);

   int x1 = BORDER_WIDTH;
   int y1 = 0;
   int by1 = y1+4;
   int bts = 10;
   if (mWidget.buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "File"))
   {
      strcpy (mMenu.menu_string[0],"File"); // PD sub menu
      strcpy (mMenu.menu_string[1],"New");
      strcpy (mMenu.menu_string[2],"Load");
      strcpy (mMenu.menu_string[3],"Reload");
      strcpy (mMenu.menu_string[4],"Save");
      strcpy (mMenu.menu_string[5],"Save As");
      strcpy (mMenu.menu_string[6],"Exit");
      strcpy (mMenu.menu_string[7],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1)
      {
         if (al_show_native_message_box(mDisplay.display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            mLevel.zero_level_data();
            mLevel.save_level_prompt();
         }
         mLevel.load_level(mLevel.last_level_loaded, 0, 0);
      }
      if (ret == 2) mVisualLevel.visual_level_select(400);
      if (ret == 3) mLevel.load_level(mLevel.last_level_loaded, 0, 0);
      if (ret == 4) mLevel.save_level(mLevel.last_level_loaded);
      if (ret == 5) mLevel.save_level_prompt();
      if (ret == 6) mWM.active = 0;
      al_set_target_backbuffer(mDisplay.display);
   }
   x1 += 44;
   if (mWidget.buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "View"))
   {
      strcpy (mMenu.menu_string[0],"View");
      strcpy (mMenu.menu_string[1],"Toggle Fullscreen       F12");
      strcpy (mMenu.menu_string[2],"Zoom Out                 F5");
      strcpy (mMenu.menu_string[3],"Zoom In                  F6");
      strcpy (mMenu.menu_string[4],"Reset Zoom            F5+F6");
      strcpy (mMenu.menu_string[5],"Zoom Fit Level Vertical   ");
      sprintf(mMenu.menu_string[6],"Text Double:Auto");
      sprintf(mMenu.menu_string[7],"Text Double:1");
      sprintf(mMenu.menu_string[8],"Text Double:2");
      sprintf(mMenu.menu_string[9],"Text Double:3");

      if (mLoop.autosave_level_editor_state) sprintf(mMenu.menu_string[10],"Autosave State:ON ");
      else                                  sprintf(mMenu.menu_string[10],"Autosave State:OFF");
      sprintf(mMenu.menu_string[11],"Reset State");

      strcpy (mMenu.menu_string[12],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mDisplay.toggle_fullscreen();
      if (ret == 2) mDisplay.set_scale_factor(mDisplay.scale_factor * .90, 0);
      if (ret == 3) mDisplay.set_scale_factor(mDisplay.scale_factor * 1.1, 0);
      if (ret == 4) mDisplay.set_scale_factor(1.0, 1);
      if (ret == 5) mDisplay.set_scale_factor((float)(mDisplay.SCREEN_H - BORDER_WIDTH*2)/2000, 1);
      if (ret == 6) mDisplay.set_saved_display_transform(0);
      if (ret == 7) mDisplay.set_saved_display_transform(1);
      if (ret == 8) mDisplay.set_saved_display_transform(2);
      if (ret == 9) mDisplay.set_saved_display_transform(3);
      if (ret == 10) mLoop.autosave_level_editor_state = ! mLoop.autosave_level_editor_state;
      if (ret == 11) { mWM.initialize_windows(); mWM.save_mW(); }
   }
   x1 += 44;

   if (mWidget.buttont(x1, by1, x1+40, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Lists"))
   {
      strcpy (mMenu.menu_string[0],"Lists");
      strcpy (mMenu.menu_string[1],"List all Items");
      strcpy (mMenu.menu_string[2],"List all Enemies");
      strcpy (mMenu.menu_string[3],"List all Lifts");
      strcpy (mMenu.menu_string[4],"List all pmsg");
      strcpy (mMenu.menu_string[5],"List all Events");
      strcpy (mMenu.menu_string[6],"Level Check");
      strcpy (mMenu.menu_string[7],"Show Level Data");
      strcpy (mMenu.menu_string[8],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mItem.show_all_items();
      if (ret == 2) mEnemy.show_all_enemies();
      if (ret == 3) mLift.show_all_lifts();
      if (ret == 4) mItem.show_all_pmsg();
      if (ret == 5) mTriggerEvent.show_all_events();
      if (ret == 6) mLevel.level_check();
      if (ret == 7) mLevel.show_level_data(40, 40, 1);

   }
   x1 += 52;


   if (mWidget.buttont(x1, by1, x1+64, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Advanced"))
   {
      strcpy (mMenu.menu_string[0],"Advanced");
      strcpy (mMenu.menu_string[1],"Global Level Tool");
      strcpy (mMenu.menu_string[2],"Level Viewer!");
      strcpy (mMenu.menu_string[3],"Animation Sequence Editor");
      strcpy (mMenu.menu_string[4],"Copy Tiles");
      strcpy (mMenu.menu_string[5],"Tile Editor");
      strcpy (mMenu.menu_string[6],"Default Flag Editor");
      strcpy (mMenu.menu_string[7],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mGlobalLevelTool.execute();
      if (ret == 2) mVisualLevel.level_viewer();
      if (ret == 3) mBitmapTools.animation_sequence_editor();
      if (ret == 4) mBitmapTools.copy_tiles();
      if (ret == 5)
      {
         mwTileEditor te;
         te.edit_tile(0);
      }
      if (ret == 6) mBitmapTools.edit_tile_flags();


   }
   x1 += 76;

   if (mWidget.buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Help"))
   {
      strcpy (mMenu.menu_string[0],"Help");
      strcpy (mMenu.menu_string[1],"Level Editor Basics");
      strcpy (mMenu.menu_string[2],"Credits");
      strcpy (mMenu.menu_string[3],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mHelp.help("Level Editor Basics");
      if (ret == 2) mHelp.help("Credits");
   }
   x1 += 44;

   x1+= 12;

   if (mWM.level_editor_mode == 1) sprintf(msg, "Mode:Main Edit");
   if (mWM.level_editor_mode == 2) sprintf(msg, "Mode:Edit Selection");
   if (mWM.level_editor_mode == 3) sprintf(msg, "Mode:Group Edit");
   if (mWM.level_editor_mode == 4) sprintf(msg, "Mode:Object Viewer");
   if (mWM.level_editor_mode == 9) sprintf(msg, "Mode:Tile Helper");

   if (mWidget.buttont(x1, by1, x1+140, bts, 0,0,0,0, 0,-1,15,0, 0,1,0,d, msg))
   {
      strcpy (mMenu.menu_string[0],msg);
      strcpy (mMenu.menu_string[1],"Mode:Main Edit");
      strcpy (mMenu.menu_string[2],"Mode:Edit Selection");
      strcpy (mMenu.menu_string[3],"Mode:Group Edit");
      strcpy (mMenu.menu_string[4],"Mode:Object Viewer");
      strcpy (mMenu.menu_string[5],"Mode:Tile Helper");
      strcpy (mMenu.menu_string[6],"end");
      int ret = mMenu.tmenu(1, x1+4, by1-1);
      if (ret == 1) mWM.set_level_editor_mode(1);
      if (ret == 2) mWM.set_level_editor_mode(2);
      if (ret == 3) mWM.set_level_editor_mode(3);
      if (ret == 4) mWM.set_level_editor_mode(4);
      if (ret == 5) mWM.set_level_editor_mode(9);
   }

   x1+=180;

   bts = 12;
   int yt = by1-2;
   if (mWM.mW[7].active) mWidget.slideri(x1, yt, x1+80, bts, 0,0,0,0,   0,12,15,15,  1,0,0,d, mObjectViewer.snap, 20, 1, 1,  "Snap:");


   // status display in the lower right border
   int y2 = mDisplay.SCREEN_H-BORDER_WIDTH+3;
   x1 = mDisplay.SCREEN_W-172;
   al_draw_textf(mFont.pr8, mColor.pc[9],  x1+2,  y2, 0, "Level:");
   al_draw_textf(mFont.pr8, mColor.pc[15], x1+50, y2, 0, "%d ", mLevel.last_level_loaded);
   x1 += 80;

   al_draw_text( mFont.pr8, mColor.pc[9],  x1,    y2, 0, "x:");
   al_draw_text( mFont.pr8, mColor.pc[9],  x1+40, y2, 0, "y:");
   if (mWM.is_mouse_on_any_window()) al_draw_textf(mFont.pr8, mColor.pc[15], x1, y2, 0, "  --   -- ");
   else                          al_draw_textf(mFont.pr8, mColor.pc[15], x1, y2, 0, "  %-2d   %-2d ", mWM.gx, mWM.gy);

   x1 = mDisplay.SCREEN_W-400;
   al_draw_textf(mFont.pr8, mColor.pc[9],  x1,    y2, 0, "Zoom:");
   al_draw_textf(mFont.pr8, mColor.pc[15],  x1+40, y2, 0, "%3.1f", mDisplay.scale_factor);
   x1 +=80;

   al_draw_textf(mFont.pr8, mColor.pc[9],  x1,   y2, 0, "Text Double:");
   if (mDisplay.saved_display_transform_double == 0) al_draw_textf(mFont.pr8, mColor.pc[15],  x1+96,   y2, 0, "Auto");
   else                                         al_draw_textf(mFont.pr8, mColor.pc[15],  x1+96,   y2, 0, "%d", mDisplay.saved_display_transform_double);
}



void mwEditorMain::draw_level_editor_background_overlays(int mouse_on_window)
{

   if (!mouse_on_window) mEditorMain.show_draw_item_cursor();



}










