#include "pm.h"
#include "mwEditSelection.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwEnemy.h"
#include "mwEventQueue.h"
#include "mwLoadSelectionDialog.h"
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


int mwEditSelection::load_selection_prompt()
{
   return mLoadSelectionDialog.run();
//   return 1;
   // char sel_filename[500];
   // sprintf(sel_filename,"sel\\");
   // if (mMiscFnx.mw_file_select("Load Selection", sel_filename, ".sel", 0)) return load_selection(sel_filename);
   // return 0;
}

int mwEditSelection::load_selection(const char* filename)
{
   printf("load_selection(%s)\n", filename);

   FILE *fp = fopen(filename, "rb");
   if (!fp)
   {
      char msg[1024];
      sprintf(msg, "Error opening %s", filename);
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

      // set all lifts to step 0
      ftLift.lift_setup();

      draw_fsel();

   }
   return 1;
}


void mwEditSelection::fill_ft_variables_from_selection(int save_to_disk)
{
   int eib=0;
   int iib=0;
   int lib=0;
   int x1 = mLevelEditor.selection.x1*20;
   int y1 = mLevelEditor.selection.y1*20;
   int x2 = mLevelEditor.selection.x2*20+20;
   int y2 = mLevelEditor.selection.y2*20+20;

   clear_ft_variables();

   // blocks
   for (int x=0; x<(mLevelEditor.selection.x2-mLevelEditor.selection.x1+1); x++)
      for (int y=0; y<(mLevelEditor.selection.y2-mLevelEditor.selection.y1+1); y++)
         if ( (x >= 0) && (x < 100) && (y >= 0) && (y < 100) && (mLevelEditor.selection.x1+x >= 0) && (mLevelEditor.selection.x1+x < 100) && (mLevelEditor.selection.y1+y >= 0) && (mLevelEditor.selection.y1+y < 100) )
         {
            if (mEditorMain.obj_filter[1][1][0])                                         ftLevel.l[x][y] = mLevel.l[mLevelEditor.selection.x1+x][mLevelEditor.selection.y1+y];                      // get block and flags
            if ((!mEditorMain.obj_filter[1][1][0]) && (mEditorMain.obj_filter[1][2][0])) ftLevel.l[x][y] = mLevel.l[mLevelEditor.selection.x1+x][mLevelEditor.selection.y1+y] & PM_BTILE_ALL_FLAGS; // get flags only
         }

   itemIDTranslations.clear();
   // items
   for (int b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mEditorMain.obj_filter[2][mItem.item[b][0]][0]) && (mItem.item[b][4] >= x1) && (mItem.item[b][4] < x2) && (mItem.item[b][5] >= y1) && (mItem.item[b][5] < y2))
      {
         int c = iib++; // destination

         // save link from old item number to new
         itemIDTranslations.push_back({b, c});

         // copy all 16 variables
         for (int y=0; y<16; y++) ftItem.item[c][y] = mItem.item[b][y];

         // shift x, y (now relative to the selection window ul corner)
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

   // change door refs in items
   for (int b=0; b<iib; b++)
      if (ftItem.item[b][0] == PM_ITEM_TYPE_DOOR && ftItem.item[b][8] == 1) // type linked dest item
      {
         // change link to new door
         for (auto lit : itemIDTranslations)
            if (ftItem.item[b][9] == lit.oldItemNumber) ftItem.item[b][9] = lit.newItemNumber;
      }

   // enemies
   for (int b=0; b<100; b++) // check for enemies in box
      if ((mEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[b][0]][0]) && (mEnemy.Ef[b][0] >= x1) && (mEnemy.Ef[b][0] < x2) && (mEnemy.Ef[b][1] >= y1) && (mEnemy.Ef[b][1] < y2))
      {
         //printf("copying enemy:%d to ft\n", b);
         int c = eib++;
         for (int y=0; y<32; y++) ftEnemy.Ei[c][y] = mEnemy.Ei[b][y];
         for (int y=0; y<16; y++) ftEnemy.Ef[c][y] = mEnemy.Ef[b][y];

         // shift x, y (now relative to the selection window ul corner)
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
   liftIDTranslations.clear();
   if (mEditorMain.obj_filter[4][1][0])
      for (int b=0; b<NUM_LIFTS; b++) // source, if in selection
         if ((mLift.cur[b].active) && (mLift.cur[b].x >= x1) && (mLift.cur[b].x < x2) && (mLift.cur[b].y >= y1) && (mLift.cur[b].y < y2))
         {
            int c = lib++; // destination
            ftLift.copy_lift_from_other_instance(c, mLift, b);

            // save link from old lift number to new
            liftIDTranslations.push_back({b, c});

            // shift x, y of move steps (now relative to the selection window ul corner)
            for (int s=0; s<ftLift.cur[c].num_steps; s++)
               if ((ftLift.stp[c][s].type & 31) == 1)
               {
                  ftLift.stp[c][s].x -= x1;
                  ftLift.stp[c][s].y -= y1;
               }
         }

   // change lift refs in items (only trigger and block damage)
   for (int b=0; b<iib; b++)
      if (((ftItem.item[b][0] == PM_ITEM_TYPE_TRIGGR) && (ftItem.item[b][3] & PM_ITEM_TRIGGER_LIFT_ON)) || ((ftItem.item[b][0] == PM_ITEM_TYPE_BLKDMG) && (ftItem.item[b][3] & PM_ITEM_DAMAGE_LIFT_ON)))
      {
         // change link to new lift
         for (auto lit : liftIDTranslations)
            if (ftItem.item[b][10] == lit.oldLiftNumber) ftItem.item[b][10] = lit.newLiftNumber;
      }


   ft_level_header[3] = iib; // num_of_items
   ft_level_header[4] = eib; // num_of_enemies
   ft_level_header[5] = lib; // num_of_lifts
   ft_level_header[8] =  mLevelEditor.selection.x2-mLevelEditor.selection.x1+1; // width
   ft_level_header[9] =  mLevelEditor.selection.y2-mLevelEditor.selection.y1+1; // height

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

   int sw = ft_level_header[8];
   int sh = ft_level_header[9];

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

         if (ftItem.item[b][0] == 5) // start
         {
            clt_last += mTriggerEvent.add_item_link_translation(2, b, 9, ftItem.item[b][1], clt, clt_last);
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
   liftIDTranslations.clear();
   if (mEditorMain.obj_filter[4][1][0])
   {
      for (b=0; b<ft_level_header[5]; b++)
      {
         int l = mLift.get_empty_lift();
         if (l > -1)
         {
            int lim = 0;
            mLift.copy_lift_from_other_instance(l, ftLift, b);

            liftIDTranslations.push_back({b, l});

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
                     if ((clt[i][0] == 4) && (clt[i][1] == b) && (clt[i][2] == s)) // found match
                        mLift.stp[l][s].val = clt[i][4];  // new event

            }
            mLift.set_lift_to_step(l, 0);
            if (lim) mLift.erase_lift(l);
         }
      }
   } // end of if copy lifts

   // enemies
   for (b=0; b<100; b++) // iterate enemies in ft
      if ((ftEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][ftEnemy.Ei[b][0]][0])) // if active attempt to copy this enemy
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
   itemIDTranslations.clear();

   for (b=0; b<500; b++)
      if ((ftItem.item[b][0]) && (mEditorMain.obj_filter[2][ftItem.item[b][0]][0]))
      {
         //int copied = 0;
         for (c=0; c<500; c++) // search for empty place to copy to
         {
            if (mItem.item[c][0] == 0) // found empty
            {
               itemIDTranslations.push_back({b, c});

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

               // is this block damage or trigger that has a link to a lift (mItem.item[c][10])
               if (((mItem.item[c][0] == PM_ITEM_TYPE_TRIGGR) && (mItem.item[c][3] & PM_ITEM_TRIGGER_LIFT_ON)) || ((mItem.item[c][0] == PM_ITEM_TYPE_BLKDMG) && (mItem.item[c][3] & PM_ITEM_DAMAGE_LIFT_ON)))
               {
                  // change link to new lift
                  for (auto lit : liftIDTranslations)
                     if (mItem.item[c][10] == lit.oldLiftNumber) mItem.item[c][10] = lit.newLiftNumber;
               }

               if (copy_mode != 2) // only do translations for copy, not move
               {
                  for (int i=0; i<clt_last; i++)                   // does this copy item have an entry in the clt table?
                     if ((clt[i][0] == 2) && (clt[i][1] == b)) // found index of source item table
                     {
                        mItem.item[c][clt[i][2]] = clt[i][4];  // new event
                        // printf("new item event!!\n");
                     }
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

   // change door refs
   for (int c=0; c<500; c++) // search for empty place to copy to
      if (mItem.item[c][0] == PM_ITEM_TYPE_DOOR && mItem.item[c][8] == 1) // type linked dest item
      {
         // change link to new door
         for (auto lit : itemIDTranslations)
            if (mItem.item[c][9] == lit.oldItemNumber) mItem.item[c][9] = lit.newItemNumber;
      }

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
   if (mEditorMain.obj_filter[1][1][0])
      for (int x=mLevelEditor.selection.x1; x<mLevelEditor.selection.x2+1; x++)
         for (int y=mLevelEditor.selection.y1; y<mLevelEditor.selection.y2+1; y++) mLevel.l[x][y]=0;

   // items
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0]) && (mEditorMain.obj_filter[2][mItem.item[i][0]][0]))
         if ((mItem.item[i][4] >= x1) && (mItem.item[i][4] < x2) && (mItem.item[i][5] >= y1) && (mItem.item[i][5] < y2)) mItem.clear_item(i);

   // enemies
   for (int e=0; e<100; e++)
      if ((mEnemy.Ei[e][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[e][0]][0]))
         if ((mEnemy.Ef[e][0] >= x1) && (mEnemy.Ef[e][0] < x2) && (mEnemy.Ef[e][1] >= y1) && (mEnemy.Ef[e][1] < y2)) mEnemy.clear_enemy(e);

   // lifts
   if (mEditorMain.obj_filter[4][1][0])
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
      // both blocks and flags
      if ((mEditorMain.obj_filter[1][1][0]) && (mEditorMain.obj_filter[1][2][0]))
      {
         mLevel.l[x][y] = tn;
         return;
      }

      // blocks only
      if ((mEditorMain.obj_filter[1][1][0]) && (!mEditorMain.obj_filter[1][2][0]))
      {
         mLevel.l[x][y] &= ~PM_BTILE_TILENUM_MASK; // clear lower bits
         int tileNum =  tn & PM_BTILE_TILENUM_MASK;
         mLevel.l[x][y] |= tileNum; // merge tileNum
         return;

      }
      // flags only
      if ((!mEditorMain.obj_filter[1][1][0]) && (mEditorMain.obj_filter[1][2][0]))
      {
         mLevel.l[x][y] &= PM_BTILE_TILENUM_MASK; // clear upper bits
         int flags    = tn & ~PM_BTILE_TILENUM_MASK;
         mLevel.l[x][y] |= flags; // merge flags
         return;
      }
   }
}


void mwEditSelection::draw_fsel(void)
{

   int sw = ft_level_header[8];
   int sh = ft_level_header[9];


   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(sw*20, sh*20);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   // draw blocks
   if (mEditorMain.obj_filter[1][1][0])
   {
      for (int x=0; x<sw; x++)
         for (int y=0; y<sh; y++)
            if (mEditorMain.show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(ftLevel.l[x][y], x*20, y*20);
   }

   // draw items
   for (int i=0; i<500; i++)
      if ((ftItem.item[i][0]) && (mEditorMain.obj_filter[2][ftItem.item[i][0]][0])) ftItem.draw_item(i, 1, ftItem.item[i][4], ftItem.item[i][5]);

   // draw enemies
   for (int e=0; e<100; e++)
      if ((ftEnemy.Ei[e][0]) && (mEditorMain.obj_filter[3][ftEnemy.Ei[e][0]][0])) ftEnemy.draw_enemy(e, 0, 0, 0);

   // draw lifts
   if (mEditorMain.obj_filter[4][1][0])
      for (int l=0; l<ft_level_header[5]; l++)
         ftLift.draw_lift(l, mwRect<float>::fromX1Y1WH(ftLift.cur[l].x, ftLift.cur[l].y, ftLift.cur[l].w, ftLift.cur[l].h));
}

void mwEditSelection::process_mouse_on_background(void)
{
   if (mInput.mouse_b[1][0])
   {
      if (copy_mode)
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         do_copy(mLevelEditor.gx, mLevelEditor.gy);
      }
      else mLevelEditor.get_new_selection("selection"); // get new selection
   }
   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mLevelEditor.set_mode(1);
   }
}

void mwEditSelection::sel_to_ft(char * b) // for load
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

void mwEditSelection::ft_to_sel(char * b) // for save
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

void mwEditSelection::draw_buttons(int x1, int x2, int &ya, int d)
{
   if (copy_mode)
   {
      if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   10, 0, 15, 0, 0, "Paste Selection", d)) copy_mode = 0;
   }
   else
   {
      if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   9, 0, 15, 0, 0, "Copy Selection", d))
      {
         copy_mode = 1;
         fill_ft_variables_from_selection(0);
         draw_fsel();
      }
   }
   if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   9, 0, 15, 0, 0, "Move Selection", d))
   {
      copy_mode = 2;
      fill_ft_variables_from_selection(0);
      draw_fsel();
      do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }
   if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   9, 0, 15, 0, 0, "Clear Selection", d))
   {
      do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }
   ya+=8; // spacing between groups

   if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   8, 0, 15, 0, 0, "Save To Disk",    d)) fill_ft_variables_from_selection(1);
   if (mWidget.mButton(0,x1,x2, ya, 16,  1,2,0,1,   8, 0, 15, 0, 0, "Load From Disk",  d))
   {
      if (load_selection_prompt()) copy_mode = 1;
   }
}



void mwEditSelection::show_details(int x1, int x2, int &ya, int fc)
{
   int rx1 = mLevelEditor.selection.x1*20;    // source x
   int ry1 = mLevelEditor.selection.y1*20;    // source y
   int rx2 = mLevelEditor.selection.x2*20+20;
   int ry2 = mLevelEditor.selection.y2*20+20;
   int iib=0;
   int eib=0;
   int lib=0;

   // count items in box
   for (int b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mEditorMain.obj_filter[2][mItem.item[b][0]][0]) && (mItem.item[b][4] >= rx1) && (mItem.item[b][4] < rx2) && (mItem.item[b][5] >= ry1) && (mItem.item[b][5] < ry2)) iib++;

   // count enemies in box
   for (int b=0; b<100; b++)
      if ((mEnemy.Ei[b][0]) && (mEditorMain.obj_filter[3][mEnemy.Ei[b][0]][0]) && (mEnemy.Ef[b][0] >= rx1) && (mEnemy.Ef[b][0] < rx2) && (mEnemy.Ef[b][1] >= ry1) && (mEnemy.Ef[b][1] < ry2)) eib++;

   // count lifts in box
   for (int d=0; d<NUM_LIFTS; d++)
      if ((mLift.cur[d].active) && (mEditorMain.obj_filter[4][1][0]) && (mLift.cur[d].x >= rx1) && (mLift.cur[d].x < rx2) && (mLift.cur[d].y >= ry1) && (mLift.cur[d].y < ry2)) lib++;

   int sx = mLevelEditor.selection.x1;
   int sy = mLevelEditor.selection.y1;
   int sw = mLevelEditor.selection.w + 1;
   int sh = mLevelEditor.selection.h + 1;

   if (copy_mode)
   {
      sx = 0;
      sy = 0;
      sw = ft_level_header[8];
      sh = ft_level_header[9];

      iib = ft_level_header[3];
      eib = ft_level_header[4];
      lib = ft_level_header[5];
   }
   int xc = (x1+x2)/2;
   al_draw_line(x1, ya, x2, ya, mColor.pc[fc], 1);
   al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[fc], 1);
   al_draw_text( mFont.pr8, mColor.pc[15], xc, ya+2,  ALLEGRO_ALIGN_CENTER, "Selection Details");
   ya+=11;
   al_draw_line(x1, ya+19, x2, ya+19, mColor.pc[fc], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya+2,  ALLEGRO_ALIGN_CENTER, "x:%-2d    y:%-2d", sx, sy);
   al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya+10, ALLEGRO_ALIGN_CENTER, "w:%-2d    h:%-2d", sw, sh);
   ya+=19;
   al_draw_line(x1, ya+27, x2, ya+27, mColor.pc[fc], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya+2,   ALLEGRO_ALIGN_CENTER, "%d Enemies", eib);
   al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya+10,  ALLEGRO_ALIGN_CENTER, "%d Items",   iib);
   al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya+18,  ALLEGRO_ALIGN_CENTER, "%d Lifts",   lib);
   ya+=35;
}

void mwEditSelection::draw(mwWindow &w)
{
   int d = w.disable_input;
   int x1 = w.rect.x1 + 1;
   int x2 = w.rect.x2 - 1;
   int ya = w.rect.y1 + 2;

   if (mWidget.mButton(0, x2-12, x2-4,   1, ya, 9,    0, 0, 0, 1,   0, 0, 15, 0, 0, "?",    d)) mHelp.help("Edit Selection");
   ya += 14;
   mEditSelection.show_details(x1, x2, ya, w.color);
   mEditSelection.draw_buttons(x1, x2, ya, d);

   // adjust height
   w.rect.setX1Y1X2Y2(w.rect.x1, w.rect.y1, w.rect.x2, ya-1);
}


void mwEditSelection::draw_level_editor_background_overlays(int mouse_on_window)
{
   // show selection
   if (!copy_mode) mLevelEditor.show_selection_rect(mLevelEditor.selection, 14, "selection");

   if (!mouse_on_window)
   {
      if (copy_mode)
      {
         if (ft_bmp)
         {
            al_draw_bitmap(ft_bmp, mLevelEditor.gx*20, mLevelEditor.gy*20, 0);
            mLevelEditor.show_selection_rect(mwRect<int>::fromX1Y1WH(mLevelEditor.gx, mLevelEditor.gy, ft_level_header[8]-1, ft_level_header[9]-1), 10, "paste");
         }
         else copy_mode = 0;
      }
   }
}

