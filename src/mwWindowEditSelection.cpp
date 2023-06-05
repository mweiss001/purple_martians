// mwWindowEditSelection.cpp

#include "pm.h"

#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"


int ft_level_header[20];
int ft_l[100][100];
int ft_item[500][16];
char ft_pmsgtext[500][500] = {0};

int   ft_Ei[100][32];
float ft_Ef[100][16];

char ft_ln[NUM_LIFTS][80];
int ft_lift[NUM_LIFTS][6];
int ft_ls[NUM_LIFTS][40][6];

ALLEGRO_BITMAP *ft_bmp = NULL;  //  file temp paste bmp


void mwWindow::es_pointer_text(int x1, int x2, int y, int mouse_on_window)
{
   int rx1 = mWM.bx1*20;    // source x
   int ry1 = mWM.by1*20;    // source y
   int rx2 = mWM.bx2*20+20;
   int ry2 = mWM.by2*20+20;
   int eib=0;
   int iib=0;
   int lib=0;

   // count items in box
   for (int b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mWM.obj_filter[2][mItem.item[b][0]]))
         if (mItem.item[b][4] >= rx1)
            if (mItem.item[b][4] < rx2)
               if (mItem.item[b][5] >= ry1)
                  if (mItem.item[b][5] < ry2)
                     iib++;

   // count enemies in box
   for (int b=0; b<100; b++)
      if ((mEnemy.Ei[b][0]) && (mWM.obj_filter[3][mEnemy.Ei[b][0]]))
         if (mEnemy.Ef[b][0] >= rx1)
            if (mEnemy.Ef[b][0] < rx2)
               if (mEnemy.Ef[b][1] >= ry1)
                  if (mEnemy.Ef[b][1] < ry2)
                     eib++;

   // count lifts in box
   if (mWM.obj_filter[4][1])
      for (int d=0; d<NUM_LIFTS; d++)
         if ((mLift.cur[d].active) && (mLift.cur[d].x >= rx1) && (mLift.cur[d].x < rx2) && (mLift.cur[d].y >= ry1) && (mLift.cur[d].y < ry2)) lib++;

   int xc = (x1+x2)/2;
   int fc = 13;
   int tc = 15;

   al_draw_text( mFont.pr8, mColor.pc[tc], xc, y+1,  ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_rectangle(x1, y, x2, y+10, mColor.pc[fc], 1);
   y+=10;
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+1, ALLEGRO_ALIGN_CENTER, "  x:%-2d    y:%-2d  ", mWM.bx1, mWM.by1);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+9, ALLEGRO_ALIGN_CENTER, "  w:%-2d    h:%-2d  ", mWM.bx2-mWM.bx1+1, mWM.by2-mWM.by1+1);
   al_draw_rectangle(x1, y, x2, y+18, mColor.pc[fc], 1);

   y+=18;
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+1, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+9, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y+17, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
   al_draw_rectangle(x1, y, x2, y+26, mColor.pc[fc], 1);
}

void mwWindow::es_do_brf(int x, int y, int flood_block)
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
         mScreen.draw_scaled_level_region_to_display(3);
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

void mwWindow::es_clear_ft(void)
{
   for (int x=0; x<20; x++) // ft_level_header
      ft_level_header[x] = 0;
   for (int x=0; x<100; x++) // ft_blocks
      for (int y=0; y<100; y++) ft_l[x][y] = 0;
   for (int c=0; c<500; c++) // ft_items
   {
      ft_pmsgtext[c][0] = 0;
      for (int x=0; x<16; x++) ft_item[c][x] = 0;
   }
   for (int c=0; c<100; c++) // ft_enemies
   {
      for (int x=0; x<32; x++) ft_Ei[c][x] = 0;
      for (int x=0; x<16; x++) ft_Ef[c][x] = 0;
   }
   for (int l=0; l<NUM_LIFTS; l++)  // ft_lifts
   {
     ft_ln[l][0] = 0; // erase lift name
     for (int x=0; x<6; x++) // lift var
        ft_lift[l][x] = 0;
     for (int s=0; s<40; s++) // step
        for (int x=0; x<4; x++) // step var
           ft_ls[l][s][x] = 0;
   }
}

int mwWindow::es_load_selection(void)
{
   char msg[1024];
   FILE *filepntr;
   int loop, ch, c, x, y;
   char sel_filename[500];
   char buff[2000];
   sprintf(sel_filename,"sel\\");
   if (mMiscFnx.mw_file_select("Load Selection", sel_filename, ".sel", 0))
   {
      es_clear_ft();
      if (!al_filename_exists(sel_filename))
      {
         sprintf(msg, "Can't Find %s ", sel_filename);
         mInput.m_err(msg);
         return 0;
      }
      if ((filepntr=fopen(sel_filename,"r")) == NULL)
      {
         sprintf(msg, "Error opening %s ", sel_filename);
         mInput.m_err(msg);
         return 0;
      }
      for (c=0; c<20; c++) // level header
      {
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = (char)NULL;
         ft_level_header[c] = atoi(buff);
      }
      for (c=0; c<ft_level_header[8]; c++)  // l[100][100]
         for (y=0; y<ft_level_header[9]; y++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_l[c][y] = atoi(buff);
         }
      for (c=0; c<ft_level_header[3]; c++)  // read item
      {
         for (x=0; x<16; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_item[c][x] = atoi(buff);
         }
         if (ft_item[c][0] == 10) // get pop msg
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               //if (ch == 126) ch = 13;
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            strcpy(ft_pmsgtext[c], buff);
         }
      }
      for (c=0; c < ft_level_header[4]; c++)  // enemy ints and floats
      {
         for (x=0; x<32; x++)  // first 32 ints
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_Ei[c][x] = atoi(buff);
         }
         for (x=0; x<16; x++)  // then 16 floats
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_Ef[c][x] = atof(buff);
         }
      }
      for (c=0; c<ft_level_header[5]; c++) // lifts
      {
         // get lift name
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
             buff[loop] = ch;
             loop++;
             ch = fgetc(filepntr);
         }
         buff[loop] = (char)NULL;
         strcpy(ft_ln[c], buff);

         // get lift data
         for (y=0; y<6; y++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_lift[c][y] = atoi(buff);
         }
         for (x=0; x<ft_lift[c][3]; x++) // get step data
            for (y=0; y<6; y++)
            {
               loop = 0;
               ch = fgetc(filepntr);
               while((ch != '\n') && (ch != EOF))
               {
                   buff[loop] = ch;
                   loop++;
                   ch = fgetc(filepntr);
               }
               buff[loop] = (char)NULL;
               ft_ls[c][x][y] = atoi(buff);
            }
      } // next lift
      fclose(filepntr);
   } // end of file select
   else return 0; // cancel pressed
   return 1;

}

void mwWindow::es_save_selection(int save)
{
   int b, c, x, y;
   int eib=0;
   int iib=0;
   int lib=0;
   int x1 = mWM.bx1*20;
   int y1 = mWM.by1*20;
   int x2 = mWM.bx2*20+20;
   int y2 = mWM.by2*20+20;


   es_clear_ft();

   // blocks
   for (x=0; x<(mWM.bx2-mWM.bx1+1); x++)
      for (y=0; y<(mWM.by2-mWM.by1+1); y++)
         if ( (x >= 0) && (x < 100) && (y >= 0) && (y < 100) && (mWM.bx1+x >= 0) && (mWM.bx1+x < 100) && (mWM.by1+y >= 0) && (mWM.by1+y < 100) )
         {
            if (mWM.obj_filter[1][1])                               ft_l[x][y] = mLevel.l[mWM.bx1+x][mWM.by1+y];                       // get block and flags
            if ((!mWM.obj_filter[1][1]) && (mWM.obj_filter[1][2])) ft_l[x][y] = mLevel.l[mWM.bx1+x][mWM.by1+y] & PM_BTILE_MOST_FLAGS; // get flags only
         }


   // items
   for (b=0; b<500; b++)
      if ((mItem.item[b][0]) && (mWM.obj_filter[2][mItem.item[b][0]]) && (mItem.item[b][4] >= x1) && (mItem.item[b][4] < x2) && (mItem.item[b][5] >= y1) && (mItem.item[b][5] < y2))
      {
         c = iib++;

         // copy all 16 variables
         for (y=0; y<16; y++) ft_item[c][y] = mItem.item[b][y];

         // set new x, y (now relative to the selection window ul corner)
         ft_item[c][4] -= x1;
         ft_item[c][5] -= y1;

         if (mItem.item_secondary67(mItem.item[b][0]))
         {
            ft_item[c][6] -= x1;
            ft_item[c][7] -= y1;
         }
         if (mItem.item[b][0] == 10) // message
         {
            strcpy(ft_pmsgtext[c], mItem.pmsgtext[b]);
         }
      }

   // enemies
   for (b=0; b<100; b++) // check for enemies in box
      if ((mEnemy.Ei[b][0]) && (mWM.obj_filter[3][mEnemy.Ei[b][0]]) && (mEnemy.Ef[b][0] >= x1) && (mEnemy.Ef[b][0] < x2) && (mEnemy.Ef[b][1] >= y1) && (mEnemy.Ef[b][1] < y2))
      {
         //printf("copying enemy:%d to ft\n", b);
         c = eib++;
         for (y=0; y<32; y++) ft_Ei[c][y] = mEnemy.Ei[b][y];
         for (y=0; y<16; y++) ft_Ef[c][y] = mEnemy.Ef[b][y];

         ft_Ef[c][0]-= x1;
         ft_Ef[c][1]-= y1;

         if (ft_Ei[c][0] == 7) // vinepod
         {
            for (int i=3; i<13; i+=2)
            {
               ft_Ei[c][i+0]-= x1;
               ft_Ei[c][i+1]-= y1;
            }
         }
         if (ft_Ei[c][0] == 9 ) // cloner
         {
            ft_Ei[c][15]-= x1;
            ft_Ei[c][16]-= y1;
            ft_Ei[c][17]-= x1;
            ft_Ei[c][18]-= y1;
         }
      }

   // lifts
   if (mWM.obj_filter[4][1])
      for (b=0; b<NUM_LIFTS; b++) // source, if in selection
         if ((mLift.cur[b].active) && (mLift.cur[b].x >= x1) && (mLift.cur[b].x < x2) && (mLift.cur[b].y >= y1) && (mLift.cur[b].y < y2))
         {
            c = lib++; // destination

            strcpy(ft_ln[c], mLift.cur[b].lift_name);

            ft_lift[c][0] = mLift.cur[b].mode;
            ft_lift[c][1] = mLift.cur[b].flags;
            ft_lift[c][2] = mLift.cur[b].color;
            ft_lift[c][3] = mLift.cur[b].num_steps;
            ft_lift[c][4] = mLift.cur[b].val1;
            ft_lift[c][5] = mLift.cur[b].val2;


            for (y = 0; y < mLift.cur[b].num_steps; y++) // copy steps
            {
               int vx = mLift.stp[b][y].x;
               int vy = mLift.stp[b][y].y;
               if ((mLift.stp[b][y].type & 31) == 1) // shift move steps
               {
                  vx -= x1;
                  vy -= y1;
               }
               ft_ls[c][y][0] = vx;
               ft_ls[c][y][1] = vy;
               ft_ls[c][y][2] = mLift.stp[b][y].w;
               ft_ls[c][y][3] = mLift.stp[b][y].h;
               ft_ls[c][y][4] = mLift.stp[b][y].val;
               ft_ls[c][y][5] = mLift.stp[b][y].type;
            }
         }

   ft_level_header[3] = iib; // num_of_items

  // printf("num of items:%d",iib);

   ft_level_header[4] = eib; // num_of_enemies
   ft_level_header[5] = lib; // num_of_lifts

   mWM.mW[4].sw = ft_level_header[8] =  mWM.bx2-mWM.bx1+1; // width
   mWM.mW[4].sh = ft_level_header[9] =  mWM.by2-mWM.by1+1; // height

   //printf("finished copying to ft - i:%d e:%d l:%d\n", iib, eib, lib);

   if (save)
   {
      char sel_filename[500];
      al_make_directory("sel"); // create if not already created
      sprintf(sel_filename, "sel\\");
      if (mMiscFnx.mw_file_select("Save Selection", sel_filename, ".sel", 1))
      {
         //printf("fn:%s\n", sel_filename);

         FILE * filepntr = fopen(sel_filename,"w");
         for (x=0; x<20; x++)
            fprintf(filepntr,"%d\n",ft_level_header[x]);
         for (c=0; c<(mWM.bx2-mWM.bx1+1); c++)  // selection of blocks
            for (x=0; x<(mWM.by2-mWM.by1+1); x++)
               fprintf(filepntr,"%d\n",ft_l[c][x]);

         for (c=0; c < ft_level_header[3]; c++) // items
         {
            for (x=0; x<16; x++)
               fprintf(filepntr,"%d\n",ft_item[c][x]);

            if (ft_item[c][0] == 10) // pop msg
            {
               y = 0;
               while (ft_pmsgtext[c][y] != (char)NULL)
               {
                  if (ft_pmsgtext[c][y] == 13) fprintf(filepntr,"%c", 126);
                  else fprintf(filepntr,"%c",ft_pmsgtext[c][y]);
                  y++ ;
               }
               fprintf(filepntr,"\n");
            }
         }
         for (c=0; c < ft_level_header[4]; c++) // enemy int and float
         {
            for (x=0; x<32; x++) fprintf(filepntr,"%d\n",ft_Ei[c][x]);
            for (x=0; x<16; x++) fprintf(filepntr,"%f\n",ft_Ef[c][x]);
         }
         for (c=0; c < ft_level_header[5]; c++) // lifts
         {
            fprintf(filepntr,"%s\n",ft_ln[c]);
            for (x=0; x<6; x++) fprintf(filepntr,"%d\n",ft_lift[c][x]);
            for (x=0; x<ft_lift[c][3]; x++)
               for (y=0; y<6; y++) fprintf(filepntr,"%d\n",ft_ls[c][x][y]);
         }
         fclose(filepntr);
      }
   }
}

void mwWindow::es_do_fcopy(int qx1, int qy1)
{
   // printf("do fcopy\n");

   int b, c, x, y;
   int x3 = qx1*20;         // dest x
   int y3 = qy1*20;         // dest y

   int erase_out_of_bounds_main = 0;  // 0 = adjust

   // blocks
   for (x=0; x<mWM.mW[4].sw; x++)
      for (y=0; y<mWM.mW[4].sh; y++)
         if ((qx1+x >= 0) && (qx1+x < 100) && (qy1+y >= 0) && (qy1+y < 100))
            set_block_with_flag_filters(qx1+x, qy1+y, ft_l[x][y]);


   // this section is to make any copied pm_event links have new unique pm_events and still linked properly
   int clt[100][5] = { 0 };
   int clt_last = 0; // index

//   clt[0] = obt (2=item, 3=enem, 4=lift)
//   clt[1] = num
//   clt[2] = ext (2,3 int var num  4=lift step index
//   clt[3] = src_ev
//   clt[4] = dst_ev

   for (b=0; b<500; b++) // iterate items in ft
   {
      if (ft_item[b][0] == 6) // orb
      {
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 10, ft_item[b][10], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 11, ft_item[b][11], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ft_item[b][12], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ft_item[b][13], clt, clt_last);
      }
      if (ft_item[b][0] == 9) // trigger
      {
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 11, ft_item[b][11], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ft_item[b][12], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ft_item[b][13], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 14, ft_item[b][14], clt, clt_last);
      }
      if (ft_item[b][0] == 13) // timer
      {
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 12, ft_item[b][12], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 13, ft_item[b][13], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 14, ft_item[b][14], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 15, ft_item[b][15], clt, clt_last);
      }
      if ((ft_item[b][0] == 10) || (ft_item[b][0] == 16) || (ft_item[b][0] == 17)) // message, bm or bd
      {
         clt_last += mTriggerEvent.add_item_link_translation(2, b, 1, ft_item[b][1], clt, clt_last);
      }
   }
   for (b=0; b<100; b++) // iterate enemies in ft
   {
      if (ft_Ei[b][0] == 9) // cloner
      {
         clt_last += mTriggerEvent.add_item_link_translation(3, b, 8, ft_Ei[b][8], clt, clt_last);
      }      if (ft_Ei[b][0] == 7) // vinepod
      {
         clt_last += mTriggerEvent.add_item_link_translation(3, b, 18, ft_Ei[b][18], clt, clt_last);
         clt_last += mTriggerEvent.add_item_link_translation(3, b, 19, ft_Ei[b][19], clt, clt_last);
      }

   }

   for (b=0; b<ft_level_header[5]; b++) // iterate lifts in ft
      for (y=0; y<ft_lift[b][3]; y++) // iterate lift steps in ft
      {
         int step_type = ft_ls[b][y][5] & 31;
         int step_val = ft_ls[b][y][4];
         if ((step_type == 5) || (step_type == 6)) clt_last += mTriggerEvent.add_item_link_translation(4, b, y, step_val, clt, clt_last);
      }

   // lifts
   if (mWM.obj_filter[4][1])
   {
      for (b=0; b<ft_level_header[5]; b++)
      {
         int l = mLift.get_empty_lift();
         if (l > -1)
         {
            int lim = 0;
            mLift.construct_lift(l, ft_ln[b]);
            mLift.cur[l].mode      = ft_lift[b][0];
            mLift.cur[l].flags     = ft_lift[b][1];
            mLift.cur[l].num_steps = ft_lift[b][3];
            mLift.cur[l].val1      = ft_lift[b][4];
            mLift.cur[l].val2      = ft_lift[b][5];


            for (y=0; y<ft_lift[b][3]; y++) // copy steps
            {
               int vx   = ft_ls[b][y][0];
               int vy   = ft_ls[b][y][1];
               int vw   = ft_ls[b][y][2];
               int vh   = ft_ls[b][y][3];
               int val  = ft_ls[b][y][4];
               int type = ft_ls[b][y][5];

               if ((type & 31) == 1) // shift  move steps
               {
                  // apply offsets
                  vx += x3;
                  vy += y3;

                  if (erase_out_of_bounds_main)
                  {
                     if (mMiscFnx.check_limit(vx, 0, 1980)) lim = 1;
                     if (mMiscFnx.check_limit(vy, 0, 1980)) lim = 1;
                  }
                  else // adjust if out of bounds
                  {
                     vx = mMiscFnx.enforce_limit(vx, 0, 1980);
                     vy = mMiscFnx.enforce_limit(vy, 0, 1980);
                  }
               }

               // does this lift step have an entry in the clt table?
               for (int i=0; i<clt_last; i++)
                  if ((clt[i][0] == 4) && (clt[i][1] == b) && (clt[i][2] == y)) // found match
                     val = clt[i][4];  // new event


               //printf("contructing step:%d\n", y);
               mLift.construct_lift_step(l, y, type, vx, vy, vw, vh, val);
            }
            mLift.set_lift_to_step(l, 0);
            if (lim)
            {
               mLift.erase_lift(l);
            }
         }
      }
   } // end of if copy lifts

   // enemies
   for (b=0; b<100; b++) // iterate enemies in ft
      if ((ft_Ei[b][0]) && (mWM.obj_filter[3][ft_Ei[b][0]])) // if active attempt to copy this enemy
      {
         //int copied = 0;
         for (c=0; c<100; c++)
         {
            if (mEnemy.Ei[c][0] == 0) // found empty
            {
               int lim = 0;
               for (y=0; y<32; y++) mEnemy.Ei[c][y] = ft_Ei[b][y]; // copy 32 ints
               for (y=0; y<16; y++) mEnemy.Ef[c][y] = ft_Ef[b][y]; // copy 16 floats

               // apply offsets
               mEnemy.Ef[c][0] += x3;
               mEnemy.Ef[c][1] += y3;


               // does this enemy have an entry in the clt table?
               for (int i=0; i<clt_last; i++)
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
      if ((ft_item[b][0]) && (mWM.obj_filter[2][ft_item[b][0]]))
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
                     mItem.item[c][y] = ft_item[b][y];

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

               // does this copy item have an entry in the clt table?
               for (int i=0; i<clt_last; i++)
                  if ((clt[i][0] == 2) && (clt[i][1] == b)) // found index of source item table
                     mItem.item[c][clt[i][2]] = clt[i][4];  // new event

               // also adjust secondary loctaions
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
                  strcpy(mItem.pmsgtext[c], ft_pmsgtext[b]);
               }

               // limits exceeded; erase
               if (lim)
               {
                  printf("erase:%d\n",c);
                  mItem.erase_item(c);
                  //copied = -1;
               }
               c = 500; // end loop
            } // end of found empty
         }  // end if iterate real item array
      } // end of attempt copy
   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mScreen.init_level_background();
}

void mwWindow::es_do_clear(void)
{
   int x1 = mWM.bx1*20;
   int y1 = mWM.by1*20;
   int x2 = mWM.bx2*20+20;
   int y2 = mWM.by2*20+20;

   // blocks
   if (mWM.obj_filter[1][1])
      for (int x=mWM.bx1; x<mWM.bx2+1; x++)
         for (int y=mWM.by1; y<mWM.by2+1; y++) mLevel.l[x][y]=0;

   // items
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0]) && (mWM.obj_filter[2][mItem.item[i][0]]))
         if ((mItem.item[i][4] >= x1) && (mItem.item[i][4] < x2) && (mItem.item[i][5] >= y1) && (mItem.item[i][5] < y2)) mItem.erase_item(i);

   // enemies
   for (int e=0; e<100; e++)
      if ((mEnemy.Ei[e][0]) && (mWM.obj_filter[3][mEnemy.Ei[e][0]]))
         if ((mEnemy.Ef[e][0] >= x1) && (mEnemy.Ef[e][0] < x2) && (mEnemy.Ef[e][1] >= y1) && (mEnemy.Ef[e][1] < y2))
         {
            for (int y=0; y<32; y++) mEnemy.Ei[e][y] = 0;
            for (int y=0; y<16; y++) mEnemy.Ef[e][y] = 0;
         }

   // lifts
   if (mWM.obj_filter[4][1])
      for (int l=NUM_LIFTS-1; l>=0; l--) // have to iterate backward beacuse erase_lift changes list order
         if (mLift.cur[l].active)
            if ((mLift.cur[l].x >= x1) && (mLift.cur[l].x < x2) && (mLift.cur[l].y >= y1) && (mLift.cur[l].y < y2)) mLift.erase_lift(l);

   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mScreen.init_level_background();
}

void mwWindow::set_block_with_flag_filters(int x, int y, int tn)
{
   if ((x>=0) && (x<100) && (y>=0) && (y<100))
   {
      // blocks and flags
      if ((mWM.obj_filter[1][1]) && (mWM.obj_filter[1][2])) mLevel.l[x][y] = tn;

      // flags only
      if ((!mWM.obj_filter[1][1]) && (mWM.obj_filter[1][2]))
      {
         int flags = tn & PM_BTILE_MOST_FLAGS; // get only flags from draw item
         mLevel.l[x][y] &= ~PM_BTILE_MOST_FLAGS;                       // clear flags in destination
         mLevel.l[x][y] |= flags;                                      // merge
      }
      // blocks only (same as block and flags?)
      if ((mWM.obj_filter[1][1]) && (!mWM.obj_filter[1][2])) mLevel.l[x][y] = tn;
   }
}

int mwWindow::es_draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 16;
   int col = 9;
   char msg[20] = "Copy Selction";

//   mWM.mW[4].copy_mode ? col=10 : col=9;
//   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, "Copy Selection"))

   if (mWM.mW[4].copy_mode)
   {
      col = 10;
      sprintf(msg, "Paste Selection");
   }

   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, msg))
   {
      if (mWM.mW[4].copy_mode) mWM.mW[4].copy_mode = 0;
      else
      {
         mWM.mW[4].copy_mode = 1;
         es_save_selection(0); // puts selection in ft_
         es_draw_fsel();
      }
   }
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Move Selection"))
   {
      mWM.mW[4].copy_mode = 1;
      es_save_selection(0); // puts selection in ft_
      es_draw_fsel();
      es_do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Clear Selection"))
   {
      es_do_clear();
      al_set_target_backbuffer(mDisplay.display);
   }


   yfb+=bts/2; // spacing between groups

   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,6,15,0, 1,0,1,d, "Save To Disk")) es_save_selection(1); // puts in ft_ and saves to disk
   if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,6,15,0, 1,0,1,d, "Load From Disk"))
   {
      if (es_load_selection())
      {
         mWM.mW[4].copy_mode = 1;
         mWM.mW[4].sw = ft_level_header[8];
         mWM.mW[4].sh = ft_level_header[9];
         es_draw_fsel();
      }
   }
   if (mWM.mW[1].draw_item_type == 1) // don't even show these 3 buttons unless draw item type is block
   {
      yfb+=bts/2; // spacing between groups
      int tn = mWM.mW[1].draw_item_num & 1023;
      bts = 24;
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,7,15,0, 1,1,1,d, "     Fill"))
      {
         for (int x=mWM.bx1; x<mWM.bx2+1; x++)
            for (int y=mWM.by1; y<mWM.by2+1; y++)
               set_block_with_flag_filters(x, y, mWM.mW[1].draw_item_num);
         mScreen.init_level_background();
         al_set_target_backbuffer(mDisplay.display);
      }
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,7,15,0, 1,1,1,d, "     Frame"))
      {
         for (int x=mWM.bx1; x<mWM.bx2+1; x++)
         {
            set_block_with_flag_filters(x, mWM.by1, mWM.mW[1].draw_item_num);
            set_block_with_flag_filters(x, mWM.by2, mWM.mW[1].draw_item_num);
         }
         for (int y=mWM.by1; y<mWM.by2+1; y++)
         {
            set_block_with_flag_filters(mWM.bx1, y, mWM.mW[1].draw_item_num);
            set_block_with_flag_filters(mWM.bx2, y, mWM.mW[1].draw_item_num);
         }
         mScreen.init_level_background();
         al_set_target_backbuffer(mDisplay.display);
      }
      mWM.mW[4].brf_mode ? col=10 : col=7;
      if (mWidget.buttontt(x3, yfb, x4, bts, tn,12,0,0, 0,col,15,0, 1,1,1,d, "     Floodfill")) mWM.mW[4].brf_mode = !mWM.mW[4].brf_mode;
//      if (mWidget.buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, "Block Floodfill")) mWM.mW[4].brf_mode = !mWM.mW[4].brf_mode;
   }
   return yfb;
}


void mwWindow::es_draw_item_ft(int i)
{
   int type = ft_item[i][0];
   int shape = ft_item[i][1];                            // get shape
   if (shape > 999) shape = mBitmap.zz[0][shape-1000];   // ans

   int x = ft_item[i][4];
   int y = ft_item[i][5];
   int custom = 1;

   int drawn = 0;
   if (type == 1)  drawn = mItem.draw_door         (i, x, y, custom);
   if (type == 2)  drawn = mItem.draw_bonus        (i, x, y, shape);
   if (type == 3)  drawn = mItem.draw_exit         (i, x, y, shape);
   if (type == 4)  drawn = mItem.draw_key          (i, x, y, shape);
   if (type == 5)  drawn = mItem.draw_start        (i, x, y, shape);
   if (type == 6)  drawn = mItem.draw_orb          (i, x, y);
   if (type == 8)  drawn = mItem.draw_bomb         (i, x, y, shape);
   if (type == 9)  drawn = mItem.draw_trigger      (i, x, y, custom);
   if (type == 10) drawn = mItem.draw_message      (i, x, y, custom);
   if (type == 11) drawn = mItem.draw_rocket       (i, x, y, shape);
   if (type == 13) drawn = mItem.draw_timer        (i, x, y, custom);
   if (type == 16) drawn = mItem.draw_block_manip  (i, x, y);
   if (type == 17) drawn = mItem.draw_block_damage (i, x, y, custom);
   if (type == 98) drawn = mItem.draw_rocket       (i, x, y, shape);
   if (type == 99) drawn = mItem.draw_lit_bomb     (i);

   // default draw if nothing else has drawn it up to now
   if (!drawn) al_draw_bitmap(mBitmap.tile[shape], x, y, 0);
}


void mwWindow::es_draw_enemy_ft(int e)
{
   int type = ft_Ei[e][0];
   int EXint = ft_Ef[e][0];
   int EYint = ft_Ef[e][1];
   int flags = 0;
   if (ft_Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
   if (ft_Ei[e][2] == 1) flags = 0;
   if (ft_Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
   if (ft_Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
   int tn = ft_Ei[e][1];

   float rot = ft_Ef[e][14];
   float sc  = ft_Ef[e][12];
   al_draw_scaled_rotated_bitmap(mBitmap.tile[tn], 10, 10, EXint+10, EYint+10, sc, sc, rot, flags);

   if (type == 9) // cloner
   {
      // source
      float sx1 = ft_Ei[e][15];
      float sy1 = ft_Ei[e][16];
      float sx2 = sx1 + ft_Ei[e][19];
      float sy2 = sy1 + ft_Ei[e][20];
      int sc1 = 11 + 128; // source box color

      // destination
      float dx1 = ft_Ei[e][17];
      float dy1 = ft_Ei[e][18];
      float dx2 = dx1 + ft_Ei[e][19];
      float dy2 = dy1 + ft_Ei[e][20];
      int dc1 = 10 + 128; // destination box color

      int q = ft_Ei[e][4]; // show box mode (0=none) (1 = src only) (2 = dst only) (3 = both)
      if ((q == 1) || (q == 3)) mMiscFnx.rectangle_with_diagonal_lines(sx1, sy1, sx2, sy2, 8, sc1, sc1+64, 0); // source
      if ((q == 2) || (q == 3)) mMiscFnx.rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64, 0); // destination

   }
}


void mwWindow::es_draw_lifts_ft()
{
   for (int l=0; l<ft_level_header[5]; l++)
   {
      int col = (ft_lift[l][1] >> 28) & 15;

      int x1 = ft_ls[l][0][0];
      int y1 = ft_ls[l][0][1];
      int x2 = x1 + ft_ls[l][0][2];
      int y2 = y1 + ft_ls[l][0][3];


      int a;
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mColor.pc[col + ((9 - a)*16)], 2 ); // faded outer shell
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mColor.pc[col] );                            // solid core
      al_draw_text(mFont.pr8, mColor.pc[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, ft_ln[l]); // name

   }
}


void mwWindow::es_draw_fsel(void)
{
   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(mWM.mW[4].sw*20, mWM.mW[4].sh*20);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   // draw blocks
   if (mWM.obj_filter[1][1])
   {
      for (int x=0; x<mWM.mW[4].sw; x++)
         for (int y=0; y<mWM.mW[4].sh; y++)
            if (mWM.mW[1].show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(ft_l[x][y], x*20, y*20);
   }

   // draw items
   for (int i=0; i<500; i++)
      if ((ft_item[i][0]) && (mWM.obj_filter[2][ft_item[i][0]])) es_draw_item_ft(i);

   // draw enemies
   for (int e=0; e<100; e++)
      if ((ft_Ei[e][0]) && (mWM.obj_filter[3][ft_Ei[e][0]])) es_draw_enemy_ft(e);

   // draw lifts
   if (mWM.obj_filter[4][1]) es_draw_lifts_ft();

}

void mwWindow::es_process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      if (mWM.mW[4].copy_mode)
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc();
         es_do_fcopy(mWM.gx, mWM.gy);
      }
      if (mWM.mW[4].brf_mode)
      {
         while (mInput.mouse_b[1][0])mEventQueue.proc();
         es_do_brf(mWM.gx, mWM.gy, mWM.mW[1].draw_item_num);
      }
      if ((!mWM.mW[4].copy_mode) && (!mWM.mW[4].brf_mode)) mWM.get_new_box(); // get new selection
   }

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc();
      mWM.set_windows(1);
   }
}
