// e_editor_main.cpp
#include "pm.h"

void em_check_s_window_pos(int reset_pos)
{
   int reset_status = 0;
   int reset_select = 0;

   int status_window_x2 = status_window_x + status_window_w;
   int status_window_y2 = status_window_y + status_window_h;
   if ((status_window_x < 0) || (status_window_x2 > SCREEN_W)) reset_status = 1;
   if ((status_window_y < 0) || (status_window_y2 > SCREEN_H)) reset_status = 1;
   if (reset_status)
   {
      status_window_x = 10;
      status_window_y = 10;
   }
   int select_window_x2 = select_window_x + select_window_w;
   int select_window_y2 = select_window_y + select_window_h;
   if ((select_window_x < 0) || (select_window_x2 > SCREEN_W)) reset_select = 1;
   if ((select_window_y < 0) || (select_window_y2 > SCREEN_H)) reset_select = 1;
   if (reset_select)
   {
      select_window_x = SCREEN_W - (select_window_w + 10);
      select_window_y = 10;
   }
}

void em_process_status_window(int draw_only, int gx, int gy, int* mpow)
{
   int swx1 = status_window_x;
   int swy1 = status_window_y;
   int swh = status_window_h;
   int sww = status_window_w;
   int swx2 = swx1 + sww;
   int swy2 = swy1 + swh;

   al_set_target_backbuffer(display);

   // erase background
   al_draw_filled_rectangle(swx1-2, swy1-2, swx2+2, swy2+2, palette_color[0]);

   // draw item area
   al_draw_rectangle(swx1, swy1 + 12, swx1 + 160, swy2, palette_color[9], 1);
   al_draw_text(font, palette_color[15], swx1 + 24,  swy1 + 14, 0, "Draw Item   ");
   al_draw_text(font, palette_color[14], swx1 + 100, swy1 + 14, 0, "mouse");
   al_draw_text(font, palette_color[14], swx1 + 143, swy1 + 14, 0, "b1");
   em_draw_item_info(                    swx1 + 2,   swy1 + 21, 9, draw_item_type, draw_item_num);

   // draw item flags
   if ((draw_item_type == 1) && (show_flag_details)) draw_flags(swx1+4, swy1+47, &draw_item_num, mpow, 0, 1, 0);


   // view item area
   al_draw_rectangle(swx1 + 160, swy1 + 12, swx2, swy2, palette_color[9], 1);
   al_draw_text(font, palette_color[15], swx1 + 184, swy1 + 14, 0, "View Item ");
   al_draw_text(font, palette_color[14], swx1 + 261, swy1 + 14, 0, "mouse");
   al_draw_text(font, palette_color[14], swx1 + 303, swy1 + 14, 0, "b2");
   em_draw_item_info(                    swx1 + 162, swy1 + 21, 9, point_item_type, point_item_num);



//
//   // draw buttons based on object type
//   if (point_item_type > 1)
//   {
//      int type=0;
//      if (point_item_type == 2) type = item[point_item_num][0];
//      if (point_item_type == 3) type = Ei[point_item_num][0];
//
//      int x1 = swx1+162;
//      int y1 = swy1+46;
//      int w=0, h=0;
//      ovw_get_size(point_item_type, type, &w, &h);
//      int x2 = x1 + w;
//      int y2 = y1 + h;
//
//      al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);
//      //printf("x1:%d y1:%d x2:%d y2:%d w:%d h:%d\n", ov_window_x1, ov_window_y1, ov_window_x2, ov_window_y2, ov_window_w, ov_window_h);
//
//      obj_buttons(point_item_type, point_item_num, x1, x2, y1, 0, 16);
//
//   }
//
//








   // point item flags
   if ((point_item_type == 1) && (show_flag_details)) draw_flags(swx1+164, swy1+47, &point_item_num, mpow, 1, 0, 1);

   // title bar background color
   al_draw_filled_rectangle(swx1, swy1, swx2, swy1 + 11, palette_color[9+192]);

   // frame title bar
   al_draw_rectangle(swx1, swy1, swx2, swy1+11, palette_color[9], 1);

   al_draw_textf(font, palette_color[9],  swx1+2,   swy1+2, 0, "Status Window    level:%d ",last_level_loaded);
   al_draw_textf(font, palette_color[15], swx1+186, swy1+2, 0, "%d ",last_level_loaded);


   al_draw_textf(font, palette_color[15], swx1+222, swy1+2, 0, "x:%-2d y:%-2d ", gx, gy);
   al_draw_text( font, palette_color[9],  swx1+222, swy1+2, 0, "x:");
   al_draw_text( font, palette_color[9],  swx1+262, swy1+2, 0, "y:");

   al_draw_text(font, palette_color[9], swx2-10, swy1+2, 0, "X");
   al_draw_text(font, palette_color[9], swx2-24, swy1+2, 0, "?");

   // faded frame
   int th = 1;
   for (int a=0; a<th; a++)
      al_draw_rectangle(swx1-a, swy1-a, swx2+a, swy2+a, palette_color[9+a*(256/th)], 1);

   if (draw_only == 0)
   {
      // is mouse on entire window
      if ((mouse_x > swx1) && (mouse_x < swx2) && (mouse_y > swy1) && (mouse_y < swy2)) *mpow = 1;


      if ( (mouse_x > swx1+310) && (mouse_x < swx1+320) && (mouse_y > swy1) && (mouse_y < swy1+12) )
      {
         al_draw_text(font, palette_color[14], swx2-10, swy1+2, 0, "X");
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            status_window_active = 0;
         }
      }

      if ( (mouse_x > swx1+296) && (mouse_x < swx1+304) && (mouse_y > swy1) && (mouse_y < swy1+12) )
      {
         al_draw_text(font, palette_color[14], swx2-24, swy1+2, 0, "?");
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            help("Status Window");
         }
      }


      if ((mouse_x > swx1) && (mouse_x < swx1+296) && (mouse_y > swy1) && (mouse_y < swy1+12))
      {
         al_draw_rectangle(swx1, swy1, swx2, swy1+11,  palette_color[14], 1);
         if (mouse_b1) // title bar move
         {
            int mx = (mouse_x-swx1); // x offset
            int my = (mouse_y-swy1); // y offset
            while (mouse_b1)
            {
               em_redraw_background(gx, gy);
               int junk = 0;
               em_process_select_window(1, &junk); // draw only
               em_process_status_window(1, gx, gy, &junk); // draw only
               status_window_x = mouse_x-mx;
               status_window_y = mouse_y-my;
            }
            em_check_s_window_pos(0);
            save_config();
         }
      }
   }
}

void em_process_select_window(int draw_only, int* mpow)
{
   int x;

   int swx1 = select_window_x;
   int swy1 = select_window_y;
   int swh = select_window_h;
   int sww = select_window_w;
   int swx2 = swx1 + sww;
   int swy2 = swy1 + swh;

   int c = 13;            // first y line of sub-windows;
   select_window_h = 200; // for now

   // set special start y
   if (select_window_special_on)
   {
      select_window_special_y = c;
      c = 16 + c + select_window_num_special_lines*20;
   }
   // set special start y
   if (select_window_block_on)
   {
      select_window_block_y = c;
      c = 16 + c + swnbl_cur*20;
   }
   select_window_text_y = c;
   select_window_h = select_window_text_y;

   int sys = swy1 + select_window_special_y;
   int syb = swy1 + select_window_block_y;
   int syt = swy1 + select_window_text_y;
   int sxw = swx1 + select_window_w-1;
   int vx = (mouse_x-select_window_x-2)/20; // column


   al_set_target_backbuffer(display);
   // erase background
   al_draw_filled_rectangle(swx1-1, swy1-1, swx2+1, swy2+1, palette_color[0]);

   // main top bar frame and text
   // title bar background color
   al_draw_filled_rectangle(swx1, swy1, swx2, swy1 + 11, palette_color[9+192]);
   al_draw_rectangle(swx1, swy1, swx2, swy1 + 12, palette_color[9], 1);
   al_draw_text(font, palette_color[9], swx1+2,  swy1+2, 0, "Selection Window");
   al_draw_text(font, palette_color[9], sxw-145, swy1+2, 0, "Blocks  Special  X");
   al_draw_text(font, palette_color[9], sxw-21,  swy1+2, 0, "?");

   // special top bar frame and text
   if (select_window_special_on)
   {
      al_draw_filled_rectangle(            swx1,    sys, swx2, sys+12, palette_color[9+192]);
      al_draw_rectangle       (            swx1,    sys, swx2, sys+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], swx1+2,  sys+2, 0, "Special Items");
      al_draw_text(font, palette_color[9], swx2-9,  sys+2, 0, "X");
      al_draw_text(font, palette_color[9], swx2-25, sys+2, 0, "-");
      al_draw_text(font, palette_color[9], swx2-41, sys+2, 0, "+");

      // draw special block
      for (c=0; c<16*select_window_num_special_lines; c++)
      {
         int tn = PDEi[c][1]; // default is the tile in PDEi[c][1]
         if (tn > 999) tn = zz[5][tn-1000]; // ans
         al_draw_bitmap(tile[tn], swx1+(c-((c/16)*16) )*20+1, swy1+14+select_window_special_y+1+(c/16*20), 0 );
      }
   }
   // blocks top bar frame and text
   if (select_window_block_on)
   {
      al_draw_filled_rectangle(swx1, syb, swx2, syb+12, palette_color[9+192]);
      al_draw_rectangle(       swx1, syb, swx2, syb+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], swx1+2,  syb+2, 0, "Block Selection ");
      al_draw_text(font, palette_color[9], swx2-9,  syb+2, 0, "X");
      al_draw_text(font, palette_color[9], swx2-25, syb+2, 0, "-");
      al_draw_text(font, palette_color[9], swx2-41, syb+2, 0, "+");

      for (c=0; c<16*swnbl_cur; c++)
         al_draw_bitmap(btile[swbl[c][0] & 1023], swx1+(c-((c/16)*16) )*20+1, swy1+select_window_block_y+1+14+(c/16*20), 0 );
   }

   // frame the whole thing
   al_draw_rectangle(swx1, swy1, swx2, swy2, palette_color[9], 1);

   if (draw_only == 0)
   {
      // check for mouse on whole window
      if ((mouse_x > swx1) && (mouse_x < swx2) && (mouse_y > swy1) && (mouse_y < swy2))
      {
         *mpow = 1;
         sw_mouse_gone = 0;

         // check for mouse on top title bar
         if (mouse_y < 14 + swy1)
         {
            al_draw_rectangle(swx1, swy1, swx2, swy1 + 12, palette_color[14], 1);
            al_draw_text(font, palette_color[14], swx1+2, swy1+2, 0, "Selection Window");

            // 'X' to close whole selection window
            if ((mouse_x > sxw-8) && (mouse_x < sxw))
            {
               al_draw_text(font, palette_color[14], sxw-9, select_window_y+2, 0, "X");
               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();  // wait for release
                  select_window_active = 0;
               }
            }
            // '?' button
            if ((mouse_x > sxw-21) && (mouse_x < sxw-13))
            {
               al_draw_text(font, palette_color[14], sxw-21, swy1+2,  0,"?" );
               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();  // wait for release
                  help("Selection Window");
               }
            }

            // 'Special' button
            if ((mouse_x > sxw-81) && (mouse_x < sxw-25))
            {
               al_draw_text(font, palette_color[14], sxw-81, swy1+2, 0, "Special");
               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();  // wait for release
                  select_window_special_on = !select_window_special_on;
               }
            }

            // 'Block' button
            if ((mouse_x > sxw-144) && (mouse_x < sxw-88))
            {
               al_draw_text(font, palette_color[14], sxw-145, swy1+2, 0, "Blocks");
               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();  // wait for release
                  select_window_block_on = !select_window_block_on;
               }
            }
            // title bar drag move!
            if (mouse_b1)
            {
               int mx = (mouse_x-swx1); // x offset
               int my = (mouse_y-swy1); // y offset
               while (mouse_b1)
               {
                  em_redraw_background(0, 0);
                  int junk = 0;
                  em_process_status_window(1, 0, 0, &junk); // draw only
                  em_process_select_window(1, &junk); // draw only
                  select_window_x = mouse_x-mx;
                  select_window_y = mouse_y-my;
               }
               em_check_s_window_pos(0);
               save_config();
            }
         } // end of if mouse on title bar

         // check for mouse on special window title bar
         if (select_window_special_on)
         {
            if ((mouse_y > sys) && (mouse_y < 14 + sys)) // on s title bar
            {
               al_draw_rectangle       (swx1, sys, swx2, sys+12, palette_color[14], 1);
               al_draw_text(font, palette_color[14], swx1+2,  sys+2, 0, "Special Items");
               if ((mouse_x > swx2-8) && (mouse_x < swx2))  // close special sub window
               {
                  al_draw_text(font, palette_color[14], swx2-9,  sys+2, 0, "X");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release
                     select_window_special_on = 0;
                  }
               }
               if ((mouse_x > swx2-25) && (mouse_x < swx2-17))  // Special button
               {
                  al_draw_text(font, palette_color[14], swx2-25,  sys+2, 0, "-");
                  if (mouse_b1)
                     {
                        while (mouse_b1) proc_controllers();  // wait for release
                        if (--select_window_num_special_lines < 1 )
                        {
                           select_window_num_special_lines++;
                           select_window_special_on = 0;
                        }
                     }
               }
               if ((mouse_x > swx2-41) && (mouse_x < swx2-33))  // Special button
               {
                  al_draw_text(font, palette_color[14], swx2-41,  sys+2, 0, "+");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release
                     if (++select_window_num_special_lines > 4 ) select_window_num_special_lines = 4;
                  }
               }
            }
         } // end of if special select window on

         // check for mouse on block window title bar
         if (select_window_block_on)
         {
            if ( (mouse_y > syb) && (mouse_y < 14 + syb) ) // on title bar
            {
               al_draw_rectangle       (swx1, syb, swx2, syb+12, palette_color[14], 1);
               al_draw_text(font, palette_color[14], swx1+2,  syb+2, 0, "Block Selection");
               if ((mouse_x > swx2-8) && (mouse_x < swx2))  // close special sub window
               {
                  al_draw_text(font, palette_color[14], swx2-9,  syb+2, 0, "X");
                  if (mouse_b1)
                     {
                        while (mouse_b1) proc_controllers();  // wait for release
                        select_window_block_on = 0;
                     }
               }
               if ((mouse_x > swx2-25) && (mouse_x < swx2-17))  // - button
               {
                  al_draw_text(font, palette_color[14], swx2-25,  syb+2, 0, "-");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release

                     if (--swnbl_cur < 1 )
                     {
                        swnbl_cur++;
                        select_window_block_on = 0;
                     }
                  }
               }
               if ((mouse_x > swx2-41) && (mouse_x < swx2-33))  // + button
               {
                  al_draw_text(font, palette_color[14], swx2-41,  syb+2, 0, "+");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release
                     if (++swnbl_cur > swnbl) swnbl_cur = swnbl;
                  }
               }
            }
         } // end of if (select_window_block_on)

         // check for mouse on special window
         if ( (select_window_special_on) && (mouse_y > 15 + sys) && (mouse_y < 16 + sys + select_window_num_special_lines * 20) )
         {
            int vy = (mouse_y-sys-15)/20; // row
            int ret = vy*16+vx;
            int tl = 0; // text lines
            if (ret < 100) // dont try to show anything above PDE[99]
            {
               // set  text length (number of lines)
               for (x=0; x<20; x++)
                  if (strncmp(PDEt[ret][x],"<end>", 5) == 0) tl = x;
               if (tl<5) tl = 5;

                // remove line endings
               for (x=0; x<20; x++)
                  for (int z=0; z<40; z++)
                  {
                     if (PDEt[ret][x][z] == 10) PDEt[ret][x][z] = 32;
                     if (PDEt[ret][x][z] == 13) PDEt[ret][x][z] = 32;
                  }

               // erase and frame
               al_draw_filled_rectangle(swx1, syt, swx2, 12+syt+3+(8*tl), palette_color[0]);
               al_draw_rectangle(swx1, syt, swx2, 12+syt+3+(8*tl), palette_color[9], 1);

               // title and frame
               al_draw_rectangle(swx1, syt, swx2, syt+12, palette_color[9], 1);
               al_draw_text(font, palette_color[9], swx1+2, syt+2, 0, "Description ");

               // draw text for this pde
               stext_draw_flag=1;
               for (x=0; x<tl; x++)
                  al_draw_text(font, palette_color[15], swx1+2, swy1 + select_window_text_y+14+(x*8), 0, PDEt[ret][x]);

               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();     // wait for release
                  int pn = PDEi[ret][0];
                  if (pn < 200)
                  {
                     draw_item_type = 5;
                     draw_item_num = ret;
                  }
                  if (pn > 199) // Creator
                  {
                     switch (pn)
                     {
                        case 200: create_obj(2, 1, 0);  break; // type 200 - door
                        case 201: create_obj(2, 5, 0);  break; // type 201 - start
                        case 202: create_obj(2, 3, 0);  break; // type 202 - exit
                        case 203: create_obj(2, 4, 0);  break; // type 203 - key
                        case 204: create_obj(3, 7, 0);  break; // type 204 - pod
                        case 206: create_obj(2, 10,0);  break; // type 206 - msg
                        case 207: create_obj(3, 9, 0);  break; // type 207 - cloner
                        case 208: if (create_lift()) object_viewerw(4, num_lifts-1); break; // type 208 - lift
                        case 209: create_door(1);       break; // type 209 - 1 way al_fixed exit door
                        case 210: create_door(2);       break; // type 210 - 1 way linked exit door
                        case 211: create_door(3);       break; // type 211 - 2 way door set
                        case 212: create_obj(3, 10, 0); break; // type 212 - field
                        case 213: create_obj(2, 9, 0);  break; // type 213 - trigger
                        case 214: create_obj(2, 16, 0); break; // type 214 - block manip
                        case 215: create_obj(2, 17, 0); break; // type 215 - block damage
                     }
                  } // end of if creator
               } // end of if (mouse_b & 1)
            } // end of ret < 99
         }  // end of mouse on special
         else stext_draw_flag = 0; // mouse not on special
         // check for mouse on block window
         if ( (select_window_block_on) && (mouse_y > 14 + syb) && (mouse_y < 14 + syb + swnbl_cur * 20))
         {
            int vy = (mouse_y-syb-14)/20; // row
            int ret = vy*16+vx;
            int tl = 3; // text lines
            int syt2 = syt+15+(8*tl);
            if (show_flag_details) syt2 += 140;

            ret = swbl[ret][0];

            // erase
            al_draw_filled_rectangle(swx1, syt, swx2, syt2, palette_color[0]);

            // frame
            al_draw_rectangle(swx1, syt, swx2, syt2, palette_color[9], 1);

            // title and frame
            al_draw_rectangle(swx1, syt, swx2, syt+12, palette_color[9], 1);
            al_draw_text(font, palette_color[9], swx1+2, syt+2,  0, "Description");

            // draw text for this block
            btext_draw_flag=1;
            em_get_text_description_of_block_based_on_flags(ret);
            al_draw_text (font, palette_color[15], swx1+2, syt+14, 0, "---------------------");
            al_draw_textf(font, palette_color[15], swx1+2, syt+22, 0, "Block %d - %s ", ret&1023, msg);
            al_draw_text (font, palette_color[15], swx1+2, syt+30, 0, "---------------------");

            int junk;
            if (show_flag_details) draw_flags(swx1+4, syt+38, &ret, &junk, 1, 0, 1);

            if ((mouse_b1) || (mouse_b2))
            {
               while (mouse_b1) proc_controllers(); // wait for release
               draw_item_type = 1;
               draw_item_num = ret;
            }
         } // end of mouse on block
         else btext_draw_flag = 0; // mouse not on block
      } // end of if mouse on whole window
      else
      {
         sw_mouse_gone++;
         stext_draw_flag = 0;
         btext_draw_flag = 0;
      }
   }
}

void em_set_swbl(void)
{
   swbn = 0;
   for (int c=0; c<NUM_SPRITES; c++)
   {
      swbl[c][0] = swbl[c][1] = 0;                    // erase
      if (sa[c][0] & PM_BTILE_SHOW_SELECT_WIN)
      {
         swbl[swbn][0] = c | sa[c][0];                // add to list with default flags
         swbl[swbn][0] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear flag
         swbn++;
      }
   }
   swnbl = (swbn / 16) + 1;
   if (swnbl_cur == 0) swnbl_cur = swnbl; // initial only
}

void em_set_block_range(int bx1, int by1, int bx2, int by2)
{
   int draw_item_flags = draw_item_num & PM_BTILE_MOST_FLAGS;
   int fsd[20][20] = {0};

   // purple pipe with open center
   fsd[0][0] = 576; // trigger blocks start
   fsd[0][1] = 592; // trigger block end
   fsd[0][9] = -1;  // default shape
   fsd[0][10] = 576; // upper left corner
   fsd[0][11] = 577; // upper right corner
   fsd[0][12] = 578; // lower left corner
   fsd[0][13] = 579; // lower right corner
   fsd[0][14] = 580; // left vertical through
   fsd[0][15] = 581; // right vertical through
   fsd[0][16] = 582; // upper horizontal through
   fsd[0][17] = 583; // lower horizontal through

   // purple pipe with solid center
   fsd[1][0] = 512; // trigger blocks start
   fsd[1][1] = 528; // trigger block end
   fsd[1][9] = 528;  // default shape
   fsd[1][10] = 512; // upper left corner
   fsd[1][11] = 513; // upper right corner
   fsd[1][12] = 514; // lower left corner
   fsd[1][13] = 515; // lower right corner
   fsd[1][14] = 516; // left vertical through
   fsd[1][15] = 517; // right vertical through
   fsd[1][16] = 518; // upper horizontal through
   fsd[1][17] = 519; // lower horizontal through

   // wires
   fsd[2][0] = 605; // trigger blocks start
   fsd[2][1] = 624; // trigger block end
   fsd[2][9] = -1;  // default shape
   fsd[2][10] = 608; // upper left corner
   fsd[2][11] = 609; // upper right corner
   fsd[2][12] = 610; // lower left corner
   fsd[2][13] = 611; // lower right corner
   fsd[2][14] = 612; // left vertical through
   fsd[2][15] = 612; // right vertical through
   fsd[2][16] = 614; // upper horizontal through
   fsd[2][17] = 614; // lower horizontal through

   // brick with corners
   fsd[3][0] = 640; // trigger blocks start
   fsd[3][1] = 656; // trigger block end
   fsd[3][9] = 656; // default shape
   fsd[3][10] = 640; // upper left corner
   fsd[3][11] = 641; // upper right corner
   fsd[3][12] = 642; // lower left corner
   fsd[3][13] = 643; // lower right corner
   fsd[3][14] = 644; // left vertical through
   fsd[3][15] = 645; // right vertical through
   fsd[3][16] = 646; // upper horizontal through
   fsd[3][17] = 647; // lower horizontal through

   // rainbows
   fsd[4][0] = 672; // trigger blocks start
   fsd[4][1] = 679; // trigger block end
   fsd[4][9] = -1;  // default shape
   fsd[4][10] = 672; // upper left corner
   fsd[4][11] = 673; // upper right corner
   fsd[4][12] = 674; // lower left corner
   fsd[4][13] = 675; // lower right corner
   fsd[4][14] = 676; // left vertical through
   fsd[4][15] = 677; // right vertical through
   fsd[4][16] = 678; // upper horizontal through
   fsd[4][17] = 679; // lower horizontal through

   // rainbows 2
   fsd[5][0] = 704; // trigger blocks start
   fsd[5][1] = 711; // trigger block end
   fsd[5][9] = -1;  // default shape
   fsd[5][10] = 704; // upper left corner
   fsd[5][11] = 705; // upper right corner
   fsd[5][12] = 706; // lower left corner
   fsd[5][13] = 707; // lower right corner
   fsd[5][14] = 708; // left vertical through
   fsd[5][15] = 709; // right vertical through
   fsd[5][16] = 710; // upper horizontal through
   fsd[5][17] = 711; // lower horizontal through

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++)
         fsd[i][j] |= draw_item_flags;


   int fsx[20][5] = {0};

   // purple pipes
   fsx[0][0] = 576; // trigger blocks start
   fsx[0][1] = 592; // trigger block end
   fsx[0][2] = 582; // middle
   fsx[0][3] = 590; // left end
   fsx[0][4] = 588; // right end

   // wires
   fsx[1][0] = 608; // trigger blocks start
   fsx[1][1] = 625; // trigger block end
   fsx[1][2] = 614; // middle
   fsx[1][3] = 622; // left end
   fsx[1][4] = 620; // right end

   // lined platform
   fsx[2][0] = 161; // trigger blocks start
   fsx[2][1] = 163; // trigger block end
   fsx[2][2] = 162; // middle
   fsx[2][3] = 161; // left end
   fsx[2][4] = 163; // right end

   // brown bricks
   fsx[3][0] = 177; // trigger blocks start
   fsx[3][1] = 179; // trigger block end
   fsx[3][2] = 178; // middle
   fsx[3][3] = 177; // left end
   fsx[3][4] = 179; // right end

   // semi-solid screen
   fsx[4][0] = 33; // trigger blocks start
   fsx[4][1] = 39; // trigger block end
   fsx[4][2] = 37; // middle
   fsx[4][3] = 36; // left end
   fsx[4][4] = 38; // right end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsx[i][j] |= draw_item_flags;


   int fsy[20][5] = {0};

   // purple pipes
   fsy[0][0] = 576; // trigger blocks start
   fsy[0][1] = 592; // trigger block end
   fsy[0][2] = 580; // middle
   fsy[0][3] = 591; // upper end
   fsy[0][4] = 589; // lower end

   // wires
   fsy[1][0] = 608; // trigger blocks start
   fsy[1][1] = 625; // trigger block end
   fsy[1][2] = 612; // middle
   fsy[1][3] = 623; // upper end
   fsy[1][4] = 621; // lower end

   // semi-solid screen
   fsy[4][0] = 33; // trigger blocks start
   fsy[4][1] = 39; // trigger block end
   fsy[4][2] = 34; // middle
   fsy[4][3] = 33; // upper end
   fsy[4][4] = 35; // lower end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsy[i][j] |= draw_item_flags;


   if ((bx2-bx1==1) && (by2-by1==1)) l[bx1][by1] = draw_item_num; // single block 1 x 1

   if ((bx2-bx1==1) && (by2-by1>1)) // vertical line 1 x >1
   {
      int a = bx1;
      for (int b=by1; b<by2; b++) // cycle the range
      {
         l[a][b] = draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((draw_item_num&1023) >= (fsy[x][0]&1023)) && ((draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                  l[a][b] = fsy[x][2]; // default
                  if (b == by1)   l[a][b] = fsy[x][3]; // left end cap
                  if (b == by2-1) l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((bx2-bx1>1) && (by2-by1==1)) // horizontal line >1 x 1
   {
      int b = by1;
      for (int a=bx1; a<bx2; a++) // cycle the range
      {
         l[a][b] = draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((draw_item_num&1023) >= (fsx[x][0]&1023)) && ((draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                  l[a][b] = fsx[x][2]; // default
                  if (a == bx1)   l[a][b] = fsx[x][3]; // left end cap
                  if (a == bx2-1) l[a][b] = fsx[x][4]; // right end cap
               }
            }
         }
      }
   }
   if ((bx2-bx1>1) && (by2-by1>1)) // box shape with corners >1 x >1
   {
      int special_handler = 0;
      for (int a=bx1; a<bx2; a++)       // cycle the range
         for (int b=by1; b<by2; b++)
            for (int x=0; x<20; x++)
               if (fsd[x][0]&1023)
               {
                  if (((draw_item_num&1023) >= (fsd[x][0]&1023)) && ((draw_item_num&1023) <= (fsd[x][1]&1023)))
                  {
                     special_handler = 1;

                     if (fsd[x][9] != -1) l[a][b] = fsd[x][9]; // default block

                     if (b == by1  ) l[a][b] = fsd[x][16];         // upper horizontal through
                     if (b == by2-1) l[a][b] = fsd[x][17];         // lower horizontal through


                     if (a == bx1)
                     {
                        if (b == by1) l[a][b] = fsd[x][10];        // upper-right corner
                        else if (b == by2-1) l[a][b] = fsd[x][12]; // lower-right corner
                        else l[a][b] = fsd[x][14];                 // right vertical through
                     }
                     if (a == bx2-1)
                     {
                        if (b == by1) l[a][b] = fsd[x][11];        // upper-left corner
                        else if (b == by2-1) l[a][b] = fsd[x][13]; // lower-left corner
                        else l[a][b] = fsd[x][15];                 // left vertical through
                     }
                  }
                  if (!special_handler) l[a][b] = draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners*/
}

void em_get_new_box(int* bx1, int*by1, int*bx2, int*by2) // keep the mouse !!
{
   *bx2 = *bx1; // set to initial
   *by2 = *by1;
   int gx=0, gy=0, hx=0, hy=0;
   while (mouse_b1)
   {
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
      *bx2 = gx; // set with mouse pointer
      *by2 = gy;
      if (*bx2 < *bx1) *bx2 = *bx1; // enforce x2 >= x1
      if (*by2 < *by1) *by2 = *by1;
      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();
      al_draw_rectangle(*bx1*20, *by1*20, *bx2*20+21, *by2*20+21, palette_color[10], 1); // show the selection rectangle
//      al_draw_textf(font, palette_color[15], *bx1*20, *by1*20-18, 0, " x1:%d y2:%d ", *bx1, *by1);
//      al_draw_textf(font, palette_color[15], *bx1*20, *by1*20-10, 0, " x2:%d y2:%d ", *bx2, *by2);
      get_new_screen_buffer(3, 0, 0);
   }
   // swap bx1 and bx2 if neccesary
   int z; // for swap
   if (*bx1 > *bx2) { z = *bx1; *bx1 = *bx2; *bx2 = z; }
   if (*by1 > *by2) { z = *by1; *by1 = *by2; *by2 = z;}
   // always set second to one more
   *bx2 = *bx2 +1;
   *by2 = *by2 +1;
}

char* em_get_text_description_of_block_based_on_flags(int flags)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PBUL)   sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void em_show_draw_item_cursor(int x, int y)
{
   if (point_item_type > -1) // if mouse pointer on window, do not show draw item
   {
      int type = draw_item_type;
      int num = draw_item_num;
      switch (type)
      {
         case 1: // block
            al_draw_bitmap(btile[num&1023], x*20, y*20, 0);
         break;
         case 2: // item
            draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = PDEi[num][1]; // bmp or ans
            if (a > 999) a = zz[5][a-1000]; // ans
            al_draw_bitmap(tile[a], x*20, y*20, 0);
         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[10], 0);
   }
}

void em_draw_item_info(int x, int y, int color, int type, int num)
{
   int a, b;
   switch (type)
   {
      case 1:
         al_draw_bitmap(btile[num&1023], x, y, 0);
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", em_get_text_description_of_block_based_on_flags(num) );
      break;
      case 2:
         draw_item(num, 1, x, y);
         a = item[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", item_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a], item_num_of_type[a]);
      break;
      case 3:
         draw_enemy(num, 1, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", enemy_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a], e_num_of_type[a]);
      break;
      case 4:
      {
         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );
         al_draw_text(font, palette_color[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);
      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(tile[b], x, y, 0);
         a = Ei[num][0]; // type
         al_draw_text(font, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}

void em_redraw_background(int gx, int gy)
{
   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();
   init_level_background();
   get_new_background(0);
   draw_lifts();
   draw_items();
   draw_enemies();
   em_show_draw_item_cursor(gx, gy);
   get_new_screen_buffer(3, 0, 0);
}

void em_find_point_item(int gx, int gy, int hx, int hy)
{
   // find point item
   point_item_type = 1; // block by default
   point_item_num = l[gx][gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[max_ob][2];                // array of objects found
   for (int a=0; a<max_ob; a++)          // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((item[i][0]) && (ob < max_ob))
      {
         int x = item[i][4];
         int y = item[i][5];
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((Ei[e][0]) && (ob < max_ob))
      {
         int x = al_fixtoi(Efi[e][0]);
         int y = al_fixtoi(Efi[e][1]);
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<num_lifts; l++) // check for lifts
   {
      int x = lifts[l].x1;
      int y = lifts[l].y1;
      if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
      {
          mo[ob][0] = 4;
          mo[ob][1] = l;
          ob++;
      }
   }
   //al_draw_textf(font, palette_color[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(font, palette_color[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mouse_x % 20;         // mouse position relative to block boundary
      int ss = 20/ob;                // step space
      int of = mm / ss;              // convert to offset into ob array
      point_item_type = mo[of][0];
      point_item_num  = mo[of][1];
      //al_draw_textf(font, palette_color[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }
}

void em_process_mouse_b1(int gx, int gy)
{
   int bx1, bx2, by1, by2;
   // don't allow drag draw selection unless draw type is block
   if (draw_item_type != 1) while (mouse_b1) proc_controllers();

   int din = draw_item_num; // shorter variable name
   switch (draw_item_type)
   {
      case 1:  // block
      {
         bx1 = gx;
         by1 = gy;
         em_get_new_box(&bx1, &by1, &bx2, &by2);
         em_set_block_range(bx1, by1, bx2, by2);
      }
      break;
      case 2:  // item
      {
         int type = item[din][0];
         int ofx = gx*20 - item[din][4]; // get offset of move in 2000 format
         int ofy = gy*20 - item[din][5];
         int c = get_empty_item(type); // get a place to put it
         if (c == -1)  break;
         for (int b=0; b<16; b++) item[c][b] = item[din][b]; // copy from draw item
         item[c][4] += ofx; // adjust with offsets
         item[c][5] += ofy;
         if ((type == 4) || (type == 9) || (type == 16) || (type == 17)) // move range for key, trig, manip, damage
         {
            item[c][6] += ofx; // adjust with offsets
            item[c][7] += ofy;
         }
         if (type == 10)
         {
            item[c][10] += ofx; // adjust with offsets
            item[c][11] += ofy;
            strcpy(pmsgtext[c], pmsgtext[din]); // msg
         }
         sort_item();
      }
      break;
      case 3:    // enemy
      {
         int type = Ei[din][0];

         int ofx = gx*20 - al_fixtoi(Efi[din][0]); // get offset of move in 2000 format
         int ofy = gy*20 - al_fixtoi(Efi[din][1]);

         int c = get_empty_enemy(type); // get a place to put it
         if (c == -1)  break;
         for (int x=0; x<32; x++) Ei[c][x]  = Ei[din][x];
         for (int x=0; x<16; x++) Efi[c][x] = Efi[din][x];

         Efi[c][0] += al_itofix(ofx);  // apply offsets
         Efi[c][1] += al_itofix(ofy);


         if (type == 7) // podzilla
         {
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
            //if (al_show_native_message_box(display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
            {
                Efi[c][5] = Efi[din][5] + al_itofix(ofx);
                Efi[c][6] = Efi[din][6] + al_itofix(ofy);
            }
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
            //if (al_show_native_message_box(display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
            {
               Ei[c][11] = Ei[din][11] + ofx;
               Ei[c][12] = Ei[din][12] + ofy;
            }
            recalc_pod(c);
         }
         if (type == 9) // cloner
         {
            //if (al_show_native_message_box(display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
            {
               Ei[c][15] = Ei[din][15] + ofx;
               Ei[c][16] = Ei[din][16] + ofy;
               Ei[c][17] = Ei[din][17] + ofx;
               Ei[c][18] = Ei[din][18] + ofy;
            }
            //if (al_show_native_message_box(display, "Move?", "Move cloner's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
            {
               Ei[c][11] = Ei[din][11] + ofx;
               Ei[c][12] = Ei[din][12] + ofy;
            }
         }

         if (type == 10) // field
         {
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
            // (al_show_native_message_box(display, "Move?", "Move field's damage and trigger fields too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
            {
               Ei[c][11] = Ei[din][11] + ofx;
               Ei[c][12] = Ei[din][12] + ofy;
               Ei[c][15] = Ei[din][15] + ofx;
               Ei[c][16] = Ei[din][16] + ofy;
            }
         }
         sort_enemy();
      }
      break;
      case 5:    // Special
      if ((PDEi[din][0] > 99) && (PDEi[din][0] < 200)) // PDE item
      {
         int d = get_empty_item(); // get a place to put it
         if (d == -1)  break;
         // copy from pde
         for (int x=0; x<16; x++) // item
            item[d][x] = PDEi[din][x];
         item[d][0] -= 100;
         item[d][4] = gx*20;
         item[d][5] = gy*20;
         sort_item();
      }
      if (PDEi[din][0] < 99) // PDE enemy
      {
         int d = get_empty_enemy(); // get a place to put it
         if (d == -1)  break;
         for (int x=0; x<32; x++) Ei[d][x]  = PDEi[din][x];
         for (int x=0; x<16; x++) Efi[d][x] = PDEfx[din][x];
         Efi[d][0] = al_itofix(gx*20);  // set new x,y
         Efi[d][1] = al_itofix(gy*20);
         sort_enemy();
      }
      break;
   } // end of switch case
}

int em_process_mouse_b2(int gx, int gy)
{
   int quit = 0;
   switch (point_item_type)
   {
      case 1:
         sprintf(global_string[2][2], "Copy Block    ");
         sprintf(global_string[2][3], "              ");
         sprintf(global_string[2][4], "                ");
      break;
      case 2:
         sprintf(global_string[2][2], "Copy %s  ",  item_name[item[point_item_num][0]]);
         sprintf(global_string[2][3], "View %s  ",  item_name[item[point_item_num][0]]);
         sprintf(global_string[2][4], "Delete %s ", item_name[item[point_item_num][0]]);
      break;
      case 3:
         sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[point_item_num][0]]);
         sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[point_item_num][0]]);
         sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[point_item_num][0]]);
      break;
      case 4:
         sprintf(global_string[2][2], "              ");
         sprintf(global_string[2][3], "View Lift '%s'",   lifts[point_item_num].lift_name);
         sprintf(global_string[2][4], "Delete Lift '%s'", lifts[point_item_num].lift_name);
      break;
   }

   switch (pmenu(2, 0))
   {
      case 2:  // copy
         if (point_item_type < 4)
         {
            draw_item_type = point_item_type;
            draw_item_num  = point_item_num;
         }
      break;
      case 3:  // view
         if (point_item_type > 1) object_viewerw(point_item_type, point_item_num);
      break;
      case 4:  // delete
         switch (point_item_type)
         {
            case 1: // delete
                 l[gx][gy] = 0;
            break;
            case 2: // zero item
               if ((draw_item_type == 2) && (draw_item_num == point_item_num)) // are you deleting the draw item?
               {
                  draw_item_type = 1;
                  draw_item_num = 0;
               }
               erase_item(point_item_num);
               sort_item();
            break;
            case 3: // zero enemy
               if ((draw_item_type == 3) && (draw_item_num == point_item_num)) // are you deleting the draw item?
               {
                  draw_item_type = 1;
                  draw_item_num = 0;
               }
               erase_enemy(point_item_num);
               sort_enemy();
            break;
            case 4: // zero lift
               erase_lift(point_item_num);
            break;
         }
      break;
      case 5: break; // menu divider
      case 6:   // zoom full screen
      {
         int draw_item;
         if (draw_item_type == 1) draw_item = draw_item_num;
         else draw_item = 0;
         zoom_full_screen(draw_item);
      }
      break;
      case 7: group_edit(); break;
      case 8:
         status_window_active = 1;
         em_check_s_window_pos(0);
      break;
      case 9:
         select_window_active = 1;
         em_check_s_window_pos(0);
      break;

      case 11: // new level
      if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
      {
         zero_level_data();
         save_level_prompt();
      }
      load_level(last_level_loaded, 0); // blind load
      break;
      case 12: // load level
         load_level_prompt();
         sort_enemy();
         sort_item();
      break;
      case 13: save_level_prompt(); break; // save level
      case 14: if (save_level_prompt()) quit=1; break; // save and exit
      case 15: help("Level Editor Basics"); break;// help
      case 16: quit = 1; break; // exit
      case 18: predefined_enemies(); break;
      case 19: global_level(); break;
      case 20: level_viewer(); break;
      case 21: animation_sequence_editor(); break;
      case 22: copy_tiles(); break;
      case 23: edit_btile_attributes(); break;

   } // end of switch case
   return quit;
}

int em_process_keypress(void)
{
   int quit = 0;
   if (key[ALLEGRO_KEY_B])
   {
      while (key[ALLEGRO_KEY_B]) proc_controllers();
      printf("load bookmark\n");
      if (bookmark_level == last_level_loaded)
      {
         if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewerw(2, bookmark_num);
         if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewerw(3, bookmark_num);
         if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewerw(4, bookmark_num);
      }
   }
   if (key[ALLEGRO_KEY_I])
   {
      while (key[ALLEGRO_KEY_I]) proc_controllers();
      show_all_items();
   }
   if (key[ALLEGRO_KEY_E])
   {
      while (key[ALLEGRO_KEY_E]) proc_controllers();
      show_all_enemies();
   }
   if (key[ALLEGRO_KEY_L])
   {
      while (key[ALLEGRO_KEY_L]) proc_controllers();
      show_all_lifts();
   }
   if (key[ALLEGRO_KEY_S])
   {
      while (key[ALLEGRO_KEY_S]) proc_controllers();
      level_check();
   }
   if (key[ALLEGRO_KEY_P])
   {
      while (key[ALLEGRO_KEY_P]) proc_controllers();
      show_all_pmsg();
   }
   if (key[ALLEGRO_KEY_ESCAPE])
   {
      while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
      if (al_show_native_message_box(display, "Save?", "Save before exit?", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1) save_level(last_level_loaded);
      quit = 1;
   }
   return quit;
}

int edit_menu(int el)
{
   mW[1].set_pos(100, 100);
   mW[1].set_size(100, 100);
   mW[1].set_title("win1");
   mW[1].active = 1;
   mW[1].index = 1;
   mW[1].layer = 0;

   mW[2].set_pos(300, 100);
   mW[2].set_size(100, 100);
   mW[2].set_title("win2");
   mW[2].active = 1;
   mW[2].index = 2;
   mW[2].layer = 1;

   mW[3].set_pos(500, 100);
   mW[3].set_size(82, 100);
   mW[3].set_title("Filters");
   mW[3].active = 1;
   mW[3].resizable = 1;
   mW[3].index = 3;
   mW[3].layer = 2;


   mW[4].set_pos(700, 100);
   mW[4].set_size(100, 100);
   mW[4].set_title("win4");
   mW[4].active = 1;
   mW[4].index = 4;
   mW[4].layer = 3;

   if (!el) load_level_prompt(); // load prompt
   else load_level(el, 0);       // blind load
   al_show_mouse_cursor(display);
   level_editor_running = 1;
   resume_allowed = 0;
   draw_item_type = 1;
   draw_item_num  = 0;
   em_check_s_window_pos(0);
   load_PDE();
   sort_enemy();
   sort_item();
   em_set_swbl();
   set_frame_nums(0);
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0; // clear_key array
   int quit=0, mouse_on_window=0;
   int gx=0, gy=0, hx=0, hy=0;
   if (autoload_bookmark)
   {
      printf("load bookmark\n");
      if (bookmark_level == last_level_loaded)
      {
         if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewerw(2, bookmark_num);
         if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewerw(3, bookmark_num);
         if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewerw(4, bookmark_num);
      }
   }
   while (!quit)
   {
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
      em_redraw_background(gx, gy);


      mouse_on_window = 0;


      // find max layer
      int max_layer = 0;
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;


      // cycle layers down from max, drawing and detecting mouse as we go
      for (int b = max_layer; b>=0; b--)
         for (int a=0; a<NUM_MW; a++) // draw any window at that layer
            if ((mW[a].active) && (mW[a].layer == b))
            {
               mW[a].draw();
               if (mW[a].detect_mouse()) mouse_on_window = a;
            }

      // set all window to not have focus
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].have_focus = 0;

      // this one does
      if (mouse_on_window) mW[mouse_on_window].set_focus(mouse_on_window);


      // process window that has focus
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].have_focus)) mW[a].process();




      if (status_window_active) em_process_status_window(0, gx, gy, &mouse_on_window);
      if (select_window_active) em_process_select_window(0, &mouse_on_window);
      if (!mouse_on_window) // mouse pointer is not on window
      {
         em_find_point_item(gx, gy, hx, hy);
         if (mouse_b1) em_process_mouse_b1(gx, gy);
         if (mouse_b2) quit = em_process_mouse_b2(gx, gy);
      }
      else point_item_type = -1; // to mark that
      quit = em_process_keypress();
   }
   level_editor_running = 0;
   al_hide_mouse_cursor(display);
   return last_level_loaded;
}

