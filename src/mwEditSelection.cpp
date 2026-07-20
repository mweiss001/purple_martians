#include "pm.h"
#include "mwEditSelection.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwEnemy.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLevelEditor.h"
#include "mwLoop.h"

#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwTriggerEvent.h"
#include "mwWidget.h"
#include "mwWindowManager.h"

mwEditSelection mEditSelection;

mwEditSelection::mwEditSelection() { init(); }

void mwEditSelection::init()
{
   copy_mode = 0;
   brf_mode = 0;






}


void mwEditSelection::show_pointer_text(int x1, int x2, int y, int mouse_on_window)
{
   int rx1 = mLevelEditor.selection.x1*20;    // source x
   int ry1 = mLevelEditor.selection.y1*20;    // source y
   int rx2 = mLevelEditor.selection.x2*20+20;
   int ry2 = mLevelEditor.selection.y2*20+20;
   int eib=0;
   int iib=0;
   int lib=0;

   // count items in box
   for (int b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mEditorMain.obj_filter[2][mItem.item[b][0]]) && (mItem.item[b][4] >= rx1) && (mItem.item[b][4] < rx2) && (mItem.item[b][5] >= ry1) && (mItem.item[b][5] < ry2)) iib++;

   // count enemies in box
   for (int b=0; b<100; b++)
      if ((mEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[b][0]]) && (mEnemy.Ef[b][0] >= rx1) && (mEnemy.Ef[b][0] < rx2) && (mEnemy.Ef[b][1] >= ry1) && (mEnemy.Ef[b][1] < ry2)) eib++;

   // count lifts in box
   for (int d=0; d<NUM_LIFTS; d++)
      if ((mLift.cur[d].active) && (mEditorMain.obj_filter[4][1]) && (mLift.cur[d].x >= rx1) && (mLift.cur[d].x < rx2) && (mLift.cur[d].y >= ry1) && (mLift.cur[d].y < ry2)) lib++;

   int xc = (x1+x2)/2;
   int fc = 13;
   int tc = 15;

   al_draw_text( mFont.pr8, mColor.pc[tc], xc, y+1,  ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_rectangle(x1, y, x2, y+10, mColor.pc[fc], 1);
   y+=10;
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+1, ALLEGRO_ALIGN_CENTER, "  x:%-2d    y:%-2d  ", mLevelEditor.selection.x1, mLevelEditor.selection.y1);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+9, ALLEGRO_ALIGN_CENTER, "  w:%-2d    h:%-2d  ", mLevelEditor.selection.x2-mLevelEditor.selection.x1+1, mLevelEditor.selection.y2-mLevelEditor.selection.y1+1);
   al_draw_rectangle(x1, y, x2, y+18, mColor.pc[fc], 1);

   y+=18;
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+1, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+9, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+17, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
   al_draw_rectangle(x1, y, x2, y+26, mColor.pc[fc], 1);
}

void mwEditSelection::do_floodfill(int x, int y, int flood_block)
{
   int show_progress = 0;
   int f[100][100] = {0};   // array of blocks to mark
   int rb = mLevel.l[x][y];        // block num to replace
   f[x][y] = 1;             // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) // iterate already marked
            {
               if ((a >  0) && (mLevel.l[a-1][b]) == rb) // look left
               {
                  if (f[a-1][b] == 0) found++; // found unmarked
                  f[a-1][b] = 1; // mark it
               }
               if ((b >  0) && (mLevel.l[a][b-1]) == rb) // look up
               {
                  if (f[a][b-1] == 0) found++; // found unmarked
                  f[a][b-1] = 1; // mark it
               }
               if ((a < 99) && (mLevel.l[a+1][b]) == rb) // look right
               {
                  if (f[a+1][b] == 0) found++; // found unmarked
                  f[a+1][b] = 1; // mark it
               }
               if ((b < 99) && (mLevel.l[a][b+1]) == rb) // look down
               {
                  if (f[a][b+1] == 0) found++; // found unmarked
                  f[a][b+1] = 1; // mark it
               }
            }

      if (show_progress)
      {
         // just because I can and it looks cool
         //printf("times:%d found:%d\n", times, found);
         for (int a1=0; a1<100; a1++)
            for (int b1=0; b1<100; b1++)
               if (f[a1][b1]) set_block_with_flag_filters(a1, b1, flood_block);

         mScreen.init_level_background();
         mScreen.get_new_background(0);
         mScreen.draw_scaled_level_region_to_display();
         al_flip_display();
         al_rest(.04);
      }
   } while (found);
   if (!show_progress)
   {
      // or we could just do it instantly at the end
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) set_block_with_flag_filters(a, b, flood_block);
      mScreen.init_level_background();
   }
}



void mwEditSelection::clear_ft_variables(void)
{
   // ft_level_header
   for (int x=0; x<20; x++) ft_level_header[x] = 0;

   // level
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++) ftLevel.l[x][y] = 0;

   // items
   for (int i=0; i<500; i++) ftItem.clear_item(i);

   // enemies
   for (int e=0; e<100; e++) ftEnemy.clear_enemy(e);

   // lifts
   ftLift.clear_lifts();
}


int mwEditSelection::load_selection(void)
{
   char sel_filename[500];
   sprintf(sel_filename,"sel\\");
   if (mMiscFnx.mw_file_select("Load Selection", sel_filename, ".sel", 0))
   {
      FILE *fp = fopen(sel_filename, "rb");
      if (!fp)
      {
         char msg[1024];
         sprintf(msg, "Error opening %s", sel_filename);
         return 0;
      }
      else  // file open !
      {
         clear_ft_variables();

         // read the compressed data
         char cmp[SEL_SIZE];
         fread(cmp, sizeof(cmp), 1, fp);
         fclose(fp);

         // decompress cmp to ft
         char ft[SEL_SIZE];
         uLongf destLen = sizeof(ft);
         uncompress((Bytef*)ft, (uLongf*)&destLen, (Bytef*)cmp, sizeof(cmp));

         // copy to variables
         sel_to_ft(ft);

         ftLift.lift_setup(); // set all lifts to step 0
      }
   }
   return 1;
}


void mwEditSelection::fill_ft_variables_from_selection(int save_to_disk)
{
   int b, c, x, y;
   int eib=0;
   int iib=0;
   int lib=0;
   int x1 = mLevelEditor.selection.x1*20;
   int y1 = mLevelEditor.selection.y1*20;
   int x2 = mLevelEditor.selection.x2*20+20;
   int y2 = mLevelEditor.selection.y2*20+20;

   clear_ft_variables();

   // blocks
   for (x=0; x<(mLevelEditor.selection.x2-mLevelEditor.selection.x1+1); x++)
      for (y=0; y<(mLevelEditor.selection.y2-mLevelEditor.selection.y1+1); y++)
         if ( (x >= 0) && (x < 100) && (y >= 0) && (y < 100) && (mLevelEditor.selection.x1+x >= 0) && (mLevelEditor.selection.x1+x < 100) && (mLevelEditor.selection.y1+y >= 0) && (mLevelEditor.selection.y1+y < 100) )
         {
            if (mEditorMain.obj_filter[1][1])                                      ftLevel.l[x][y] = mLevel.l[mLevelEditor.selection.x1+x][mLevelEditor.selection.y1+y];                      // get block and flags
            if ((!mEditorMain.obj_filter[1][1]) && (mEditorMain.obj_filter[1][2])) ftLevel.l[x][y] = mLevel.l[mLevelEditor.selection.x1+x][mLevelEditor.selection.y1+y] & PM_BTILE_ALL_FLAGS; // get flags only
         }


   // items
   for (b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mEditorMain.obj_filter[2][mItem.item[b][0]]) && (mItem.item[b][4] >= x1) && (mItem.item[b][4] < x2) && (mItem.item[b][5] >= y1) && (mItem.item[b][5] < y2))
      {
         c = iib++;

         // copy all 16 variables
         for (y=0; y<16; y++) ftItem.item[c][y] = mItem.item[b][y];

         // set new x, y (now relative to the selection window ul corner)
         ftItem.item[c][4] -= x1;
         ftItem.item[c][5] -= y1;

         if (mItem.item_secondary67(mItem.item[b][0]))
         {
            ftItem.item[c][6] -= x1;
            ftItem.item[c][7] -= y1;
         }
         if (mItem.item[b][0] == 10) // message
         {
            strcpy(ftItem.pmsgtext[c], mItem.pmsgtext[b]);
         }
      }

   // enemies
   for (b=0; b<100; b++) // check for enemies in box
      if ((mEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[b][0]]) && (mEnemy.Ef[b][0] >= x1) && (mEnemy.Ef[b][0] < x2) && (mEnemy.Ef[b][1] >= y1) && (mEnemy.Ef[b][1] < y2))
      {
         //printf("copying enemy:%d to ft\n", b);
         c = eib++;
         for (y=0; y<32; y++) ftEnemy.Ei[c][y] = mEnemy.Ei[b][y];
         for (y=0; y<16; y++) ftEnemy.Ef[c][y] = mEnemy.Ef[b][y];

         ftEnemy.Ef[c][0]-= x1;
         ftEnemy.Ef[c][1]-= y1;

         if (ftEnemy.Ei[c][0] == 7) // vinepod
         {
            for (int i=3; i<13; i+=2)
            {
               ftEnemy.Ei[c][i+0]-= x1;
               ftEnemy.Ei[c][i+1]-= y1;
            }
         }
         if (ftEnemy.Ei[c][0] == 9 ) // cloner
         {
            ftEnemy.Ei[c][15]-= x1;
            ftEnemy.Ei[c][16]-= y1;
            ftEnemy.Ei[c][17]-= x1;
            ftEnemy.Ei[c][18]-= y1;
         }
      }

   // lifts
   if (mEditorMain.obj_filter[4][1])
      for (b=0; b<NUM_LIFTS; b++) // source, if in selection
         if ((mLift.cur[b].active) && (mLift.cur[b].x >= x1) && (mLift.cur[b].x < x2) && (mLift.cur[b].y >= y1) && (mLift.cur[b].y < y2))
         {
            c = lib++; // destination
            ftLift.copy_lift_from_other_instance(c, mLift, b);
            for (int s=0; s<ftLift.cur[c].num_steps; s++)  // shift move steps
               if ((ftLift.stp[c][s].type & 31) == 1)
               {
                  ftLift.stp[c][s].x -= x1;
                  ftLift.stp[c][s].y -= y1;
               }
         }

   ft_level_header[3] = iib; // num_of_items

   printf("num of items:%d",iib);

   ft_level_header[4] = eib; // num_of_enemies
   ft_level_header[5] = lib; // num_of_lifts

   sw = ft_level_header[8] =  mLevelEditor.selection.x2-mLevelEditor.selection.x1+1; // width
   sh = ft_level_header[9] =  mLevelEditor.selection.y2-mLevelEditor.selection.y1+1; // height

   //printf("finished copying to ft - i:%d e:%d l:%d\n", iib, eib, lib);

   ftLift.lift_setup(); // set all lifts to step 0

   if (save_to_disk)
   {
      char sel_filename[500];
      al_make_directory("sel"); // create if not already created
      sprintf(sel_filename, "sel\\");
      if (mMiscFnx.mw_file_select("Save Selection", sel_filename, ".sel", 1))
      {
         // put variables in sel
         char sel[SEL_SIZE];
         ft_to_sel(sel);

         // compress sel to cmp
         char cmp[SEL_SIZE];
         uLongf destLen= sizeof(cmp);
         compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)sel, sizeof(sel), -1);
         int cmp_size = destLen;

         // write cmp to file
         FILE *fp = fopen(sel_filename,"wb");
         fwrite(cmp, cmp_size, 1, fp);
         fclose(fp);

      }
   }
}

void mwEditSelection::do_copy(int qx1, int qy1)
{
   // printf("do fcopy\n");

   int b, c, x, y;
   int x3 = qx1*20;         // dest x
   int y3 = qy1*20;         // dest y

   int erase_out_of_bounds_main = 0;  // 0 = adjust

   // blocks
   for (x=0; x<sw; x++)
      for (y=0; y<sh; y++)
         if ((qx1+x >= 0) && (qx1+x < 100) && (qy1+y >= 0) && (qy1+y < 100))
            set_block_with_flag_filters(qx1+x, qy1+y, ftLevel.l[x][y]);


   // this section is to make any copied pm_event links have new unique pm_events and still linked properly
   int clt[100][5] = { 0 };
   int clt_last = 0; // index
   if (copy_mode != 2) // do not do any link translation for move, only for copy
   {

      //   clt[0] = obt (2=item, 3=enem, 4=lift)
      //   clt[1] = num
      //   clt[2] = ext (2,3 int var num  4=lift step index
      //   clt[3] = src_ev
      //   clt[4] = dst_ev

      for (b=0; b<500; b++) // iterate items in ft
      {
         if (ftItem.item[b][0] == 6) // orb
         {
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 10, ftItem.item[b][10], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 11, ftItem.item[b][11], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ftItem.item[b][12], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ftItem.item[b][13], clt, clt_last);
         }
         if (ftItem.item[b][0] == 9) // trigger
         {
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 11, ftItem.item[b][11], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ftItem.item[b][12], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ftItem.item[b][13], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 14, ftItem.item[b][14], clt, clt_last);
         }
         if (ftItem.item[b][0] == 13) // timer
         {
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ftItem.item[b][12], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ftItem.item[b][13], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 14, ftItem.item[b][14], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 15, ftItem.item[b][15], clt, clt_last);
         }
         if ((ftItem.item[b][0] == 10) || (ftItem.item[b][0] == 16) || (ftItem.item[b][0] == 17)) // message, bm or bd
         {
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 1, ftItem.item[b][1], clt, clt_last);
         }
      }
      for (b=0; b<100; b++) // iterate enemies in ft
      {
         if (ftEnemy.Ei[b][0] == 9) // cloner
         {
            clt_last += mTriggerEvent.add_item_link_translation(3, b, 8, ftEnemy.Ei[b][8], clt, clt_last);
         }
         if (ftEnemy.Ei[b][0] == 7) // vinepod
         {
            clt_last += mTriggerEvent.add_item_link_translation(3, b, 18, ftEnemy.Ei[b][18], clt, clt_last);
            clt_last += mTriggerEvent.add_item_link_translation(3, b, 19, ftEnemy.Ei[b][19], clt, clt_last);
         }

      }

      for (b=0; b<ft_level_header[5]; b++) // iterate lifts in ft
         for (y=0; y<ftLift.cur[b].num_steps; y++) // iterate lift steps in ft
         {
            int step_type = ftLift.stp[b][y].type & 31;
            int step_val  = ftLift.stp[b][y].val;

            if ((step_type == 5) || (step_type == 6)) clt_last += mTriggerEvent.add_item_link_translation(4, b, y, step_val, clt, clt_last);
         }
   }



   // lifts
   if (mEditorMain.obj_filter[4][1])
   {
      for (b=0; b<ft_level_header[5]; b++)
      {
         int l = mLift.get_empty_lift();
         if (l > -1)
         {
            int lim = 0;
            mLift.copy_lift_from_other_instance(l, ftLift, b);
            for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
            {
               if ((mLift.stp[l][s].type & 31) == 1) // shift move steps
               {
                  mLift.stp[l][s].x += x3;
                  mLift.stp[l][s].y += y3;
                  if (erase_out_of_bounds_main)
                  {
                     if (mMiscFnx.check_limit(mLift.stp[l][s].x, 0, 1980)) lim = 1;
                     if (mMiscFnx.check_limit(mLift.stp[l][s].y, 0, 1980)) lim = 1;
                  }
                  else // adjust if out of bounds
                  {
                     mLift.stp[l][s].x = mMiscFnx.enforce_limit(mLift.stp[l][s].x, 0, 1980);
                     mLift.stp[l][s].y = mMiscFnx.enforce_limit(mLift.stp[l][s].y, 0, 1980);
                  }
               }

               // do link translations
               if (copy_mode != 2) // do not do any link translation for move, only for copy
                  for (int i=0; i<clt_last; i++) // does this lift step have an entry in the clt table?
                     if ((clt[i][0] == 4) && (clt[i][1] == b) && (clt[i][2] == y)) // found match
                        mLift.stp[l][s].val = clt[i][4];  // new event

            }
            mLift.set_lift_to_step(l, 0);
            if (lim)  mLift.erase_lift(l);
         }
      }
   } // end of if copy lifts

   // enemies
   for (b=0; b<100; b++) // iterate enemies in ft
      if ((ftEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][ftEnemy.Ei[b][0]])) // if active attempt to copy this enemy
      {
         //int copied = 0;
         for (c=0; c<100; c++)
         {
            if (mEnemy.Ei[c][0] == 0) // found empty
            {
               int lim = 0;
               for (y=0; y<32; y++) mEnemy.Ei[c][y] = ftEnemy.Ei[b][y]; // copy 32 ints
               for (y=0; y<16; y++) mEnemy.Ef[c][y] = ftEnemy.Ef[b][y]; // copy 16 floats

               // apply offsets
               mEnemy.Ef[c][0] += x3;
               mEnemy.Ef[c][1] += y3;


               if (copy_mode != 2) // do not do any link translation for move, only for copy
                  for (int i=0; i<clt_last; i++) // does this enemy have an entry in the clt table?
                     if ((clt[i][0] == 3) && (clt[i][1] == b)) // found index
                        mEnemy.Ei[c][clt[i][2]] = clt[i][4];  // new event


               if (erase_out_of_bounds_main)
               {
                  if (mMiscFnx.check_limit(mEnemy.Ef[c][0], 0, 1980)) lim = 1;
                  if (mMiscFnx.check_limit(mEnemy.Ef[c][1], 0, 1980)) lim = 1;
               }
               else // adjust if out of bounds
               {
                  mEnemy.Ef[c][0] = mMiscFnx.enforce_limit(mEnemy.Ef[c][0], 0, 1980);
                  mEnemy.Ef[c][1] = mMiscFnx.enforce_limit(mEnemy.Ef[c][1], 0, 1980);
               }

               if (mEnemy.Ei[c][0] == 7) // vinepod
               {
                  for (int i=3; i<13; i+=2)
                  {
                     mEnemy.Ei[c][i+0]+= x3;
                     mEnemy.Ei[c][i+1]+= y3;
                  }
               }
               if (mEnemy.Ei[c][0] == 9) // cloner
               {
                  mEnemy.Ei[c][15]+= x3;
                  mEnemy.Ei[c][16]+= y3;
                  mEnemy.Ei[c][17]+= x3;
                  mEnemy.Ei[c][18]+= y3;
               }
               if (lim)
               {
                  //copied = -1;
                  for (y=0; y<32; y++) mEnemy.Ei[c][y] = 0;
                  for (y=0; y<16; y++) mEnemy.Ef[c][y] = 0;
               }
               c = 100; // end loop
            } // end of found empty
         }  // end if iterate real enemy array
      } // end of attempt copy


   // items
   for (b=0; b<500; b++)
      if ((ftItem.item[b][0]) && (mEditorMain.obj_filter[2][ftItem.item[b][0]]))
      {
         //int copied = 0;
         for (c=0; c<500; c++) // search for empty place to copy to
         {
            if (mItem.item[c][0] == 0) // found empty
            {
               //copied = 1000+c;
               int lim = 0;
               // copy all 16 variables
               for (y=0; y<16; y++)
                     mItem.item[c][y] = ftItem.item[b][y];

               // apply offsets
               mItem.item[c][4] += x3;
               mItem.item[c][5] += y3;

               if (erase_out_of_bounds_main)
               {
                  if (mMiscFnx.check_limit(mItem.item[c][4], 0, 1980)) lim = 1;
                  if (mMiscFnx.check_limit(mItem.item[c][5], 0, 1980)) lim = 1;
               }
               else // adjust if out of bounds
               {
                  mItem.item[c][4] = mMiscFnx.enforce_limit(mItem.item[c][4], 0, 1980);
                  mItem.item[c][5] = mMiscFnx.enforce_limit(mItem.item[c][5], 0, 1980);
               }

               if (copy_mode != 2) // do not do any link translation for move, only for copy
                  for (int i=0; i<clt_last; i++)                   // does this copy item have an entry in the clt table?
                     if ((clt[i][0] == 2) && (clt[i][1] == b)) // found index of source item table
                        {
                           mItem.item[c][clt[i][2]] = clt[i][4];  // new event

                           printf("new item event!!\n");

                        }

               // also adjust secondary locations
               if (mItem.item_secondary67(mItem.item[c][0]))
               {
                  mItem.item[c][6] += x3;
                  mItem.item[c][7] += y3;
               }
               if (mItem.item[c][0] == 5) // start
               {
                  // do something here to prevent exact duplicates
               }
               if (mItem.item[c][0] == 10) // message
               {
                  strcpy(mItem.pmsgtext[c], ftItem.pmsgtext[b]);
               }

               // limits exceeded; erase
               if (lim)
               {
                  printf("erase:%d\n",c);
                  mItem.clear_item(c);
                  //copied = -1;
               }
               c = 500; // end loop
            } // end of found empty
         }  // end if iterate real item array
      } // end of attempt copy
   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mScreen.init_level_background();

   if (copy_mode == 2) copy_mode = 1; // if move (copy_mode == 2), change to copy (copy_mode == 1)

}

void mwEditSelection::do_clear(void)
{
   int x1 = mLevelEditor.selection.x1*20;
   int y1 = mLevelEditor.selection.y1*20;
   int x2 = mLevelEditor.selection.x2*20+20;
   int y2 = mLevelEditor.selection.y2*20+20;

   // blocks
   if (mEditorMain.obj_filter[1][1])
      for (int x=mLevelEditor.selection.x1; x<mLevelEditor.selection.x2+1; x++)
         for (int y=mLevelEditor.selection.y1; y<mLevelEditor.selection.y2+1; y++) mLevel.l[x][y]=0;

   // items
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0]) && (mEditorMain.obj_filter[2][mItem.item[i][0]]))
         if ((mItem.item[i][4] >= x1) && (mItem.item[i][4] < x2) && (mItem.item[i][5] >= y1) && (mItem.item[i][5] < y2)) mItem.clear_item(i);

   // enemies
   for (int e=0; e<100; e++)
      if ((mEnemy.Ei[e][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[e][0]]))
         if ((mEnemy.Ef[e][0] >= x1) && (mEnemy.Ef[e][0] < x2) && (mEnemy.Ef[e][1] >= y1) && (mEnemy.Ef[e][1] < y2)) mEnemy.clear_enemy(e);

   // lifts
   if (mEditorMain.obj_filter[4][1])
      for (int l=NUM_LIFTS-1; l>=0; l--) // have to iterate backward because erase_lift changes list order
         if (mLift.cur[l].active)
            if ((mLift.cur[l].x >= x1) && (mLift.cur[l].x < x2) && (mLift.cur[l].y >= y1) && (mLift.cur[l].y < y2)) mLift.erase_lift(l);

   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mScreen.init_level_background();
}



void mwEditSelection::set_block_with_flag_filters(int x, int y, int tn)
{
   if ((x>=0) && (x<100) && (y>=0) && (y<100))
   {
      // blocks and flags
      if ((mEditorMain.obj_filter[1][1]) && (mEditorMain.obj_filter[1][2])) mLevel.l[x][y] = tn;

      // this is wrong!!!!
      // blocks only (same as block and flags?) mLevel.l[x][y] = tn;


      if ((mEditorMain.obj_filter[1][1]) && (!mEditorMain.obj_filter[1][2]))
      {
         mLevel.l[x][y] &= 0b11111111111111111111110000000000; // clear lower bits

         int mtn = tn & PM_BTILE_TILENUM_MASK; // strip flags from tile

         mLevel.l[x][y] |= mtn; // merge tileNum

      }

      // flags only
      if ((!mEditorMain.obj_filter[1][1]) && (mEditorMain.obj_filter[1][2]))
      {
         mLevel.l[x][y] &= PM_BTILE_TILENUM_MASK; // clear upper bits
         mLevel.l[x][y] &= ~PM_BTILE_ALL_FLAGS;   // clear flags in destination
         int flags = tn & PM_BTILE_ALL_FLAGS;     // get only flags from draw item
         mLevel.l[x][y] |= flags;                 // merge
      }

/*

      void mwTileSets::drawTile(int x, int y, int tileNum, int drawItemFlags, int drawTileMode)
      {
         // replace with tileNum and drawItemFlags
         if (drawTileMode == 1) mLevel.l[x][y] = tileNum | drawItemFlags;


         // change only tile portion (lower 10 bits)
         if (drawTileMode == 2)
         {
            mLevel.l[x][y] &= 0b11111111111111111111110000000000; // clear lower bits
            mLevel.l[x][y] |= tileNum; // merge tileNum
         }

         // change only flags portion (upper 22 bits)
         if (drawTileMode == 3)
         {
            mLevel.l[x][y] &= 0b00000000000000000000001111111111; // clear upper bits
            mLevel.l[x][y] |= drawItemFlags; // merge drawItemFlags
         }
      }
*/

   }
}

int mwEditSelection::draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 16;

   bts += mLoop.pct_y;

   int col = 9;
   char msg[20] = "Copy Selection";

   if (copy_mode)
   {
      col = 10;
      sprintf(msg, "Paste Selection");
   }

   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, msg))
   {
      if (copy_mode) copy_mode = 0;
      else
      {
         copy_mode = 1;
         fill_ft_variables_from_selection(0);
         draw_fsel();
      }
   }
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Move Selection"))
   {
      copy_mode = 2;
      fill_ft_variables_from_selection(0);
      draw_fsel();
      do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Clear Selection"))
   {
      do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }


   yfb+=bts/2; // spacing between groups

   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,6,15,0, 1,0,1,d, "Save To Disk")) fill_ft_variables_from_selection(1);
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,6,15,0, 1,0,1,d, "Load From Disk"))
   {
      if (load_selection())
      {
         copy_mode = 1;
         sw = ft_level_header[8];
         sh = ft_level_header[9];
         draw_fsel();
      }
   }
   if (mEditorMain.draw_item_type == 1) // don't even show these 3 buttons unless draw item type is block
   {
      yfb+=bts/2; // spacing between groups
      int tn = mEditorMain.draw_item_num & PM_BTILE_TILENUM_MASK;
      bts = 24;
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,7,15,0, 1,1,1,d, "     Fill"))
      {
         for (int x=mLevelEditor.selection.x1; x<mLevelEditor.selection.x2+1; x++)
            for (int y=mLevelEditor.selection.y1; y<mLevelEditor.selection.y2+1; y++)
               set_block_with_flag_filters(x, y, mEditorMain.draw_item_num);
         mScreen.init_level_background();
         al_set_target_backbuffer(mDisplay.display);
      }
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,7,15,0, 1,1,1,d, "     Frame"))
      {
         for (int x=mLevelEditor.selection.x1; x<mLevelEditor.selection.x2+1; x++)
         {
            set_block_with_flag_filters(x, mLevelEditor.selection.y1, mEditorMain.draw_item_num);
            set_block_with_flag_filters(x, mLevelEditor.selection.y2, mEditorMain.draw_item_num);
         }
         for (int y=mLevelEditor.selection.y1; y<mLevelEditor.selection.y2+1; y++)
         {
            set_block_with_flag_filters(mLevelEditor.selection.x1, y, mEditorMain.draw_item_num);
            set_block_with_flag_filters(mLevelEditor.selection.x2, y, mEditorMain.draw_item_num);
         }
         mScreen.init_level_background();
         al_set_target_backbuffer(mDisplay.display);
      }
      brf_mode ? col=10 : col=7;
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,col,15,0, 1,1,1,d, "     Floodfill")) brf_mode = !brf_mode;
   }
   return yfb;
}


void mwEditSelection::draw_fsel(void)
{
   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(sw*20, sh*20);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   // draw blocks
   if (mEditorMain.obj_filter[1][1])
   {
      for (int x=0; x<sw; x++)
         for (int y=0; y<sh; y++)
            if (mEditorMain.show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(ftLevel.l[x][y], x*20, y*20);
   }

   // draw items
   for (int i=0; i<500; i++)
      if ((ftItem.item[i][0]) && (mEditorMain.obj_filter[2][ftItem.item[i][0]])) ftItem.draw_item(i, 1, ftItem.item[i][4], ftItem.item[i][5]);

   // draw enemies
   for (int e=0; e<100; e++)
      if ((ftEnemy.Ei[e][0]) && (mEditorMain.obj_filter[3][ftEnemy.Ei[e][0]])) ftEnemy.draw_enemy(e, 0, 0, 0);

   // draw lifts
   if (mEditorMain.obj_filter[4][1])
      for (int l=0; l<ft_level_header[5]; l++)
         ftLift.draw_lift(l, mwRect<float>::fromX1Y1WH(ftLift.cur[l].x, ftLift.cur[l].y, ftLift.cur[l].w, ftLift.cur[l].h));
}

void mwEditSelection::process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      if (copy_mode)
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         do_copy(mLevelEditor.gx, mLevelEditor.gy);
      }
      if (brf_mode)
      {
         while (mInput.mouse_b[1][0])mEventQueue.proc(1);
         do_floodfill(mLevelEditor.gx, mLevelEditor.gy, mEditorMain.draw_item_num);
      }
      if ((!copy_mode) && (!brf_mode)) mLevelEditor.get_new_box("selection"); // get new selection
   }
   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mLevelEditor.set_mode(1);
   }
}

void mwEditSelection::sel_to_ft(char * b) // for load level
{
   int sz = 0, offset = 0;
   sz = sizeof(ft_level_header); memcpy(ft_level_header, b+offset, sz); offset += sz;
   sz = sizeof(ftLevel.l);       memcpy(ftLevel.l,       b+offset, sz); offset += sz;
   sz = sizeof(ftItem.item);     memcpy(ftItem.item,     b+offset, sz); offset += sz;
   sz = sizeof(ftEnemy.Ei);      memcpy(ftEnemy.Ei,      b+offset, sz); offset += sz;
   sz = sizeof(ftEnemy.Ef);      memcpy(ftEnemy.Ef,      b+offset, sz); offset += sz;
   sz = sizeof(ftLift.cur);      memcpy(ftLift.cur,      b+offset, sz); offset += sz;
   sz = sizeof(ftLift.stp);      memcpy(ftLift.stp,      b+offset, sz); offset += sz;
   sz = sizeof(ftItem.pmsgtext); memcpy(ftItem.pmsgtext, b+offset, sz); offset += sz;
}

void mwEditSelection::ft_to_sel(char * b) // for save level
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(ft_level_header); memcpy(b+offset, ft_level_header, sz);
   offset += sz; sz = sizeof(ftLevel.l);       memcpy(b+offset, ftLevel.l,       sz);
   offset += sz; sz = sizeof(ftItem.item);     memcpy(b+offset, ftItem.item,     sz);
   offset += sz; sz = sizeof(ftEnemy.Ei);      memcpy(b+offset, ftEnemy.Ei,      sz);
   offset += sz; sz = sizeof(ftEnemy.Ef);      memcpy(b+offset, ftEnemy.Ef,      sz);
   offset += sz; sz = sizeof(ftLift.cur);      memcpy(b+offset, ftLift.cur,      sz);
   offset += sz; sz = sizeof(ftLift.stp);      memcpy(b+offset, ftLift.stp,      sz);
   offset += sz; sz = sizeof(ftItem.pmsgtext); memcpy(b+offset, ftItem.pmsgtext, sz);
}


/*
int mwEditSelection::draw(int x1, int x2, int y1, int y2, int d, int have_focus)
{
   mMiscFnx.titlex("Edit Selection", 15, 13, x1, x2, y1+1);

   int by1 = y1+3;
   if (mWidget.buttont(x2-12, by1, x2-4, 9, 0,0,0,0, 0,-1,15,0, 0,0,0,d,"?")) mHelp.help("Edit Selection");

   int mow = mWM.is_mouse_on_any_window();
   mEditSelection.show_pointer_text(x1+1, x2-1, y1+20, mow);

   int sy2 = mEditSelection.draw_buttons(x1+1, x2-1, y1+80, d);

   int fc = 13;
   if (have_focus) fc = 10;
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fc], 1); // frame entire window

   return sy2;

   //set_size(w, sy2-y1-1);


}

*/

void mwEditSelection::draw(mwRect<int> &rect, int d, int have_focus)
{
   mMiscFnx.titlex("Edit Selection", 15, 13, rect.x1, rect.x2, rect.y1+1);

   int by1 = rect.y1+3;
   if (mWidget.buttont(rect.x2-12, by1, rect.x2-4, 9, 0,0,0,0, 0,-1,15,0, 0,0,0,d,"?")) mHelp.help("Edit Selection");

//   int mow = mLevelEditor.mWM.is_mouse_on_any_window();
   mEditSelection.show_pointer_text(rect.x1+1, rect.x2-1, rect.y1+20, mLevelEditor.mWM.mouse_on_window);


   // draw buttons and get y2
   int y2 = mEditSelection.draw_buttons(rect.x1+1, rect.x2-1, rect.y1+80, d);

   // adjust height
   rect.setX1Y1X2Y2(rect.x1, rect.y1, rect.x2, y2-1);

   // frame entire window
   int fc = 13;
   if (have_focus) fc = 10;
   rect.draw_rectangle(mColor.pc[fc], 1);

}



void mwEditSelection::draw_level_editor_background_overlays(int mouse_on_window)
{
   // show selection
   if (!copy_mode) mLevelEditor.show_selection_rect(mLevelEditor.selection, 14, "selection");

   // only show if mouse not on window
   if (!mouse_on_window)
   {
      if (brf_mode) mMiscFnx.crosshairs_full(mLevelEditor.gx*20+10, mLevelEditor.gy*20+10, 15, 1);
      if (copy_mode)
      {
         if (ft_bmp)
         {
            al_draw_bitmap(ft_bmp, mLevelEditor.gx*20, mLevelEditor.gy*20, 0);
            mLevelEditor.show_selection_rect(mwRect<int>::fromX1Y1WH(mLevelEditor.gx, mLevelEditor.gy, sw-1, sh-1), 10, "paste");
         }
         else copy_mode = 0;
      }
   }
}


























