// e_pde.cpp

#include "pm.h"

void check_s_window_pos(int reset_pos)
{
   int swx1 = status_window_x;
   int swy1 = status_window_y;
   int swh = status_window_h;
   int sww = status_window_w;
   int swx2 = swx1 + sww;
   int swy2 = swy1 + swh;
   if ((swx2 > SCREEN_W) || (swx1 < 0)) reset_pos = 1;
   if ((swy2 > SCREEN_H) || (swy1 < 0)) reset_pos = 1;

   swx1 = select_window_x;
   swy1 = select_window_y;
   swh = select_window_h;
   sww = select_window_w;
   swx2 = swx1 + sww;
   swy2 = swy1 + swh;
   if ((swx2 > SCREEN_W) || (swx1 < 0)) reset_pos = 1;
   if ((swy2 > SCREEN_H) || (swy1 < 0)) reset_pos = 1;

   if (reset_pos)
   {
      status_window_x = SCREEN_W-(sww+10);
      status_window_y = 10;
      select_window_x = SCREEN_W-(sww+10);
      select_window_y = status_window_y + status_window_h + 10;
   }
}

int process_status_window(int draw_only)
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
   al_draw_text(font, palette_color[15], swx1 + 24,  swy1 + 14,  0, "Draw Item   ");
   al_draw_text(font, palette_color[14], swx1 + 100, swy1 + 14,  0, "mouse");
   al_draw_text(font, palette_color[14], swx1 + 143, swy1 + 14,  0, "b1");
   draw_item_info(swx1+2,   swy1+21, 9, draw_item_type, draw_item_num);

   // view item area
   al_draw_rectangle(swx1 + 160, swy1 + 12, swx2, swy2, palette_color[9], 1);
   al_draw_text(font, palette_color[15], swx1 + 184, swy1 + 14,  0, "View Item ");
   al_draw_text(font, palette_color[14], swx1 + 261, swy1 + 14,  0, "mouse");
   al_draw_text(font, palette_color[14], swx1 + 303, swy1 + 14,  0, "b2");
   draw_item_info(swx1+162, swy1+21, 9, point_item_type, point_item_num);


   // title bar background color
   al_draw_filled_rectangle(swx1, swy1, swx2, swy1 + 11, palette_color[9+192]);

   // frame title bar
   al_draw_rectangle(swx1, swy1, swx2, swy1+11, palette_color[9], 1);

   al_draw_textf(font, palette_color[9],  swx1+2,   swy1+2, 0, "Status Window    level:%d ",last_level_loaded);
   al_draw_textf(font, palette_color[15], swx1+186, swy1+2, 0, "%d ",last_level_loaded);


   int x100 = mouse_x/20 + wx;
   int y100 = mouse_y/20 + wy;
   if (x100>99) x100 = 99;
   if (y100>99) y100 = 99;

   al_draw_textf(font, palette_color[15], swx1+222, swy1+2, 0, "x:%-2d y:%-2d ", x100, y100);
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
      if ( (mouse_x > swx1+310) && (mouse_x < swx1+320) && (mouse_y > swy1) && (mouse_y < swy1+12) )
      {
         al_draw_text(font, palette_color[14], swx2-10, swy1+2, 0, "X");
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            status_window_active = 0;
            return 1001;
         }
      }

      if ( (mouse_x > swx1+296) && (mouse_x < swx1+304) && (mouse_y > swy1) && (mouse_y < swy1+12) )
      {
         al_draw_text(font, palette_color[14], swx2-24, swy1+2, 0, "?");
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            help("Status Window");
            return 1001;
         }
      }

      if ((mouse_x > swx1) && (mouse_x < swx1+296) && (mouse_y > swy1) && (mouse_y < swy1+12))
      {
         al_draw_rectangle(swx1, swy1, swx2, swy1+11,  palette_color[14], 1);
         if (mouse_b1) // title bar move
         {
            int tx = (mouse_x-swx1); // x offset
            int ty = (mouse_y-swy1); // y offset
            while (mouse_b1)
            {
               proc_controllers();
               update_editor_background();
               process_select_window(1); // draw only
               process_status_window(1); // draw only
               al_flip_display();
               al_clear_to_color(al_map_rgb(0,0,0));
               status_window_x = mouse_x-tx;
               status_window_y = mouse_y-ty;
            }
            check_s_window_pos(0);
            return 1001;
         }
      }
   }
   return 1;
}

int process_select_window(int draw_only)
{
   int x;

   int swx1 = select_window_x;
   int swy1 = select_window_y;
   int swh = select_window_h;
   int sww = select_window_w;
   int swx2 = swx1 + sww;
   int swy2 = swy1 + swh;

   int c, a, b=0;
   select_window_h = 200; // for now
   c = 13;  // first y line of sub-windows
   set_swbl();  // set swbl
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
      al_draw_filled_rectangle(swx1, sys,  swx2,    sys+12, palette_color[9+192]);
      al_draw_rectangle       (swx1, sys,  swx2,    sys+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], swx1+2,  sys+2, 0, "Special Items");
      al_draw_text(font, palette_color[9], swx2-9,  sys+2, 0, "X");
      al_draw_text(font, palette_color[9], swx2-25, sys+2, 0, "-");
      al_draw_text(font, palette_color[9], swx2-41, sys+2, 0, "+");





      // draw special block
      for (c=0; c<16*select_window_num_special_lines; c++)
      {
         if (c < 100) a = PDEi[c][1]; // bmp or ans
         else a = 0;
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(tile[b], swx1+(c-((c/16)*16) )*20+1, swy1+14+select_window_special_y+1+(c/16*20), 0 );
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
         al_draw_bitmap(btile[swbl[c][0]], swx1+(c-((c/16)*16) )*20+1, swy1+select_window_block_y+1+14+(c/16*20), 0 );


   }

   // frame the whole thing
//   al_draw_rectangle(swx1, swy1, swx2, swy2, palette_color[9], 1);

   // faded frame
   int th = 1;
   for (int a=0; a<th; a++)
      al_draw_rectangle(swx1-a, swy1-a, swx2+a, swy2+a, palette_color[9+a*(256/th)], 1);

   if (draw_only == 0)
   {
      // check for mouse on whole window
      if ((mouse_x > swx1) && (mouse_x < swx2) && (mouse_y > swy1) && (mouse_y < swy2))
      {
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
                  return 1001;
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
                  return 1001;
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
                  return 1001;
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
                  return 1001;
               }
            }

            // title bar drag move!
            if (mouse_b1)
            {
               int tx = (mouse_x-swx1); // x offset
               int ty = (mouse_y-swy1); // y offset
               while (mouse_b1)
               {
                  proc_controllers();
                  update_editor_background();
                  process_status_window(1); // draw only
                  process_select_window(1); // draw only
                  al_flip_display();
                  al_clear_to_color(al_map_rgb(0,0,0));
                  select_window_x = mouse_x-tx;
                  select_window_y = mouse_y-ty;
               }
               check_s_window_pos(0);
               return 1001;
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
                     return 1001;
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
                        return 1001;
                     }
               }
               if ((mouse_x > swx2-41) && (mouse_x < swx2-33))  // Special button
               {
                  al_draw_text(font, palette_color[14], swx2-41,  sys+2, 0, "+");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release
                     if (++select_window_num_special_lines > 4 )
                           select_window_num_special_lines = 4;
                     return 1001;
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
                        return 1001;
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
                     return 1001;
                  }
               }
               if ((mouse_x > swx2-41) && (mouse_x < swx2-33))  // + button
               {
                  al_draw_text(font, palette_color[14], swx2-41,  syb+2, 0, "+");
                  if (mouse_b1)
                  {
                     while (mouse_b1) proc_controllers();  // wait for release
                     if (++swnbl_cur > swnbl) swnbl_cur = swnbl;
                     return 1001;
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
                  while (mouse_b1) proc_controllers(); // wait for release
                  if (PDEi[ret][0] < 200) return 3000+ret; // PUT ITEM OR ENEMY

                  if (PDEi[ret][0] > 199) // Creator
                  {
                     switch (PDEi[ret][0]-199)
                     {
                        case 1:  create_obj(2, 1, 0); break; // type 200 - door
                        case 2:  create_obj(2, 5, 0); break; // type 201 - start
                        case 3:  create_obj(2, 3, 0); break; // type 202 - exit
                        case 4:  create_obj(2, 4, 0); break; // type 203 - key
                        case 5:  create_obj(3, 7, 0); break; // type 204 - pod
                        case 7:  create_obj(2, 10,0); break; // type 206 - msg
                        case 8:  create_obj(3, 9, 0); break; // type 207 - cloner
                        case 9: if (create_lift()) object_viewer(4, num_lifts-1); break; // type 208 - lift
                        case 10: create_door(1); break; // type 209 - 1 way al_fixed exit door
                        case 11: create_door(2); break; // type 210 - 1 way linked exit door
                        case 12: create_door(3); break; // type 211 - 2 way door set
                        case 13: create_obj(3, 10, 0); break; // type 212 - field
                        case 14: create_obj(2, 9, 0); break;  // type 213 - trigger
                        case 15: create_obj(2, 16, 0); break;  // type 214 - block manip
                        case 16: create_obj(2, 17, 0); break;  // type 215 - block damage
                     }
                     draw_big(1);
                     return 1001;
                  } // end of if creator

               } // end of if (mouse_b & 1)
            }
         }  // end of mouse on special
         else // mouse not on special
         {
            if (stext_draw_flag)
            {
               stext_draw_flag = 0;
               return 1001;
            }
         }

         // check for mouse on block window
         if ( (select_window_block_on) && (mouse_y > 14 + syb) && (mouse_y < 14 + syb + swnbl_cur * 20))
         {
            int vy = (mouse_y-syb-14)/20; // row
            int ret = vy*16+vx;
            int tl = 3; // text lines
            ret = swbl[ret][0];

            // erase
            al_draw_filled_rectangle(swx1, syt, swx2, 12+syt+3+(8*tl), palette_color[0]);

            // frame
            al_draw_rectangle(swx1, syt, swx2, 12+syt+3+(8*tl), palette_color[9], 1);

            // title and frame
            al_draw_rectangle(swx1, syt, swx2, syt+12, palette_color[9], 1);
            al_draw_text(font, palette_color[9], swx1+2, syt+2,  0, "Description");

            // draw text for this pde
            char t[80];
            btext_draw_flag=1;

            if ((ret > 127 ) && (ret < NUM_SPRITES )) sprintf(t,"solid");
            if ((ret > 95 ) && (ret < 128 ))          sprintf(t,"breakable");
            if ((ret > 63 ) && (ret < 96 ))           sprintf(t,"bombable");
            if ((ret > 31  ) && (ret < 64 ))          sprintf(t,"semi-solid ");
            if (ret < 32 )                            sprintf(t,"empty");

            al_draw_text (font, palette_color[15], swx1+2, syt+14, 0, "---------------------");
            al_draw_textf(font, palette_color[15], swx1+2, syt+22, 0, "Block %d - %s ", ret, t);
            al_draw_text (font, palette_color[15], swx1+2, syt+30, 0, "---------------------");

            if ((mouse_b1) || (mouse_b2))
            {
               while (mouse_b1) proc_controllers(); // wait for release
               return ret;
            }
         } // end of mouse on block
         else // mouse not on block
         {
            if (btext_draw_flag)
            {
               btext_draw_flag = 0;
               return 1001;
            }
         }
      } // end of if mouse on whole window


      else
      {
         sw_mouse_gone++;
         if (stext_draw_flag)
         {
            stext_draw_flag = 0;
            return 1001;
         }
         if (btext_draw_flag)
         {
            btext_draw_flag = 0;
            return 1001;
         }
      }
   }

return 999;
}

void set_swbl(void)
{
   int swbn = 0;
   // erase array
   for (int c=0; c<NUM_SPRITES; c++)
   {
      swbl[c][0] = 0;
      swbl[c][1] = 0;
   }
/*
   for (int c=0; c<NUM_SPRITES; c++)
      if ((sa[c][0] == 1) && (sa[c][1] == 1)) // if block and locked
            swbl[swbn++][0] = c;   // put shape # in list and inc counter
   for (int c=0; c<NUM_SPRITES; c++)
      if ((sa[c][0] == 1) && (sa[c][1] == 0)) // if block and unlocked
            swbl[swbn++][0] = c;   // put shape # in list and inc counter
*/

/*

   for (int c=0; c<NUM_SPRITES; c++)
      if (sa[c][1] == 1) swbl[swbn++][0] = c;   // put shape # in list and inc counter
*/


   for (int c=0; c<NUM_SPRITES; c++)
      if (sa[c][0] & PM_BTILE_SHOW_SELECT_WIN) swbl[swbn++][0] = c;   // put shape # in list and inc counter


   swnbl = (swbn / 16) + 1;
   if (swnbl_cur == 0) swnbl_cur = swnbl; // initial only
}














int load_PDE()
{
   FILE *filepntr;
   int PDE_load_error;
   int loop, ch, c, x;
   char buff[80];
   PDE_load_error = 0;
   if (!al_filename_exists("bitmaps/pde.pm"))
   {
      sprintf(msg, "Can't find pde.pm");
      PDE_load_error = 2;
   }
   if (!PDE_load_error) // file exists
      if ((filepntr=fopen("bitmaps/pde.pm","r")) == NULL)
      {
         sprintf(msg, "Error opening pde.pm");
         m_err(msg);
         PDE_load_error = 3;
      }
   if (!PDE_load_error) // file exists and is open!
   {
      for (c=0; c<100; c++) // read PDE enemy al_fixed
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
            PDEfx[c][x] = atoi(buff);  // al_fixed

            if (ch == EOF)
            {
               sprintf(msg, "Error reading al_fixed in PDE");
               m_err(msg);
               PDE_load_error = 4;
            }
         }
      for (c=0; c < 100; c++)  // enemy ints
         for (x=0; x<32; x++)
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
            PDEi[c][x] = atoi(buff);
            if (ch == EOF)
            {
               sprintf(msg,  "Error reading ints in PDE" );
               m_err(msg);
               PDE_load_error = 5;
            }
         }
      for (c=0; c < 100; c++)  // enemy text
         for (x=0; x<20; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               PDEt[c][x][loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            PDEt[c][x][loop] = (char)NULL;
            if (ch == EOF)
            {
               sprintf(msg,"Error reading text at %d %d %d in PDE", loop, c, x);
               m_err(msg);
               PDE_load_error = 6;
            }
         }
      fclose(filepntr);
   }
   if (PDE_load_error)
   {
      sprintf(msg,"PDI load error %d", PDE_load_error);
      m_err(msg);
      return 0;
   }
   return 1;
}

void save_PDE()
{
   FILE *filepntr;
   int c, x;
   filepntr = fopen("bitmaps/pde.pm","w");
   for (c=0; c < 100; c++)  // enemy al_fixed
      for (x=0; x<16; x++)
         fprintf(filepntr,"%d\n",PDEfx[c][x]);

   for (c=0; c < 100; c++) // enemy int
      for (x=0; x<32; x++)
         fprintf(filepntr,"%d\n",PDEi[c][x]);

   for (c=0; c < 100; c++) // enemy text
      for (x=0; x<20; x++)
         fprintf(filepntr,"%s\n",PDEt[c][x]);
   fclose(filepntr);
}


// this is a non blocking, pass through function and should be called in a loop
int bottom_menu(int menu_num)
{
   int selection=0;
   if (mouse_y > 186) selection = (mouse_x / 40);   // highlight only
   for (int c=0; c<8; c++)
   {
      int d = 9+64;
      if (c == selection) d = 9;
      al_draw_textf(font, palette_color[d], c*40, 184, 0, " F%-1d  ", c+1);
      al_draw_text(font,  palette_color[d], c*40, 192, 0, global_string[menu_num][c]);
   }
   selection = 999; // normal return --  nothing happened
   if ((mouse_b1) && (mouse_y > 186))
   {
      selection = (mouse_x / 40);
      while (mouse_b1) proc_controllers();   // wait for release
   }
   if (selection == 7) selection = -1;
   if (key[ALLEGRO_KEY_F1]) selection = 0;
   if (key[ALLEGRO_KEY_F2]) selection = 1;
   if (key[ALLEGRO_KEY_F3]) selection = 2;
   if (key[ALLEGRO_KEY_F4]) selection = 3;
   if (key[ALLEGRO_KEY_F5]) selection = 4;
   if (key[ALLEGRO_KEY_F6]) selection = 5;
   if (key[ALLEGRO_KEY_F7]) selection = 6;
   if (key[ALLEGRO_KEY_F8]) selection = -1;
   if (key[ALLEGRO_KEY_ESCAPE])selection = -1;
   return selection;
}


int edit_int(int x, int y, int val, int inc, int lv, int uv)
{
   int imx = mouse_x * display_transform_double;
   int imy = mouse_y * display_transform_double;
   int old_mouse;
   al_hide_mouse_cursor(display);
   while (mouse_b1)
   {
      al_set_mouse_xy(display, 160, 100);
      proc_controllers();
      old_mouse = mouse_y;
      proc_controllers();

      al_draw_filled_rectangle(x, y, x+32, y+8, palette_color[0]);
      al_draw_textf(font, palette_color[10], x, y, 0, "%+-d ", val);
      al_flip_display();

      val = val - ((mouse_y - old_mouse) * inc);

      if (val > uv) val = uv;
      if (val < lv) val = lv;

   }
   al_set_mouse_xy(display, imx, imy);
   al_show_mouse_cursor(display);
   return val;
}


al_fixed edit_fix(int x, int y, al_fixed val)
{
   al_fixed fxinc = al_ftofix(.01);    // x inc
   al_fixed fyinc = al_ftofix(.1);     // y inc
   al_fixed flv   = al_ftofix(-2000);  // lv
   al_fixed fuv   = al_ftofix(2000);   // uv

   int imx = mouse_x * display_transform_double; // initial mouse position
   int imy = mouse_y * display_transform_double;
   int old_mouse_x;
   int old_mouse_y;
   al_hide_mouse_cursor(display);

   while (mouse_b1)
   {
      al_set_mouse_xy(display, 160, 100);
      proc_controllers();
      old_mouse_x = mouse_x;
      old_mouse_y = mouse_y;
      proc_controllers();

      al_draw_filled_rectangle(x, y, x+96, y+8, palette_color[0]);
      al_draw_textf(font, palette_color[10], x, y, 0, "%+-8.6f ",al_fixtof(val));
      al_flip_display();

      if (key[ALLEGRO_KEY_0]) val = al_ftofix(0);
      if (key[ALLEGRO_KEY_1]) val = al_ftofix(1);
      if (key[ALLEGRO_KEY_2]) val = al_ftofix(2);
      if (key[ALLEGRO_KEY_3]) val = al_ftofix(3);
      if (key[ALLEGRO_KEY_4]) val = al_ftofix(4);
      if (key[ALLEGRO_KEY_5]) val = al_ftofix(5);
      if (key[ALLEGRO_KEY_6]) val = al_ftofix(6);
      if (key[ALLEGRO_KEY_7]) val = al_ftofix(7);
      if (key[ALLEGRO_KEY_8]) val = al_ftofix(8);
      if (key[ALLEGRO_KEY_9]) val = al_ftofix(9);

      val += (mouse_x - old_mouse_x) * fxinc;
      val += (mouse_y - old_mouse_y) * fyinc;

      float sfr = 0.01; // round
      float fval = al_fixtof(val);
      fval = round(fval/sfr) * sfr;
      val = al_ftofix(fval);

      // check and enforce limits
      if (val > fuv) val = fuv;
      if (val < flv) val = flv;


   }
   al_set_mouse_xy(display, imx, imy); // set original mouse position
   al_show_mouse_cursor(display);
   return val;
}

void PDE_swap(int s1, int s2)
{
   for (int y=0; y<32; y++)
   {
      int temp    = PDEi[s1][y];
      PDEi[s1][y] = PDEi[s2][y];
      PDEi[s2][y] = temp;
   }
   for (int y=0; y<16; y++)
   {
      al_fixed temp   = PDEfx[s1][y];
      PDEfx[s1][y] = PDEfx[s2][y];
      PDEfx[s2][y] = temp;
   }
   for (int y=0; y<20; y++)
   {
      char stemp[80];
      strcpy(stemp,       PDEt[s1][y] );
      strcpy(PDEt[s1][y], PDEt[s2][y] );
      strcpy(PDEt[s2][y], stemp);
   }
}


void PDE_sort(void)
{
   // first just sort by type
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if ( PDEi[x][0] > PDEi[x+1][0]) // sort by type
         {
            PDE_swap(x, x + 1);
            swap_flag++; // if any swaps
         }
      }
   }

   // then sort by text line
   swap_flag = 1;
   int do_swap = 0;
   while (swap_flag)
   {
      do_swap = 0;
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if ((PDEi[x][0] > 199) && ( PDEi[x][0] < 300))  // if both creators
            if ((PDEi[x+1][0] > 199) && ( PDEi[x+1][0] < 300))
               if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1])) > 0)
                  do_swap = 1;

         if ((PDEi[x][0] > 99) && ( PDEi[x][0] < 200))  // if both items
            if ((PDEi[x+1][0] > 99) && ( PDEi[x+1][0] < 200))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if ((PDEi[x][0] > 0) && ( PDEi[x][0] < 100))  // if both enemies
            if ((PDEi[x+1][0] > 0) && ( PDEi[x+1][0] < 100))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if (do_swap) // do the swap
         {
            do_swap = 0;
            swap_flag++; // if any swaps
            PDE_swap(x, x + 1);
         } // end of swap
      } // end of for x
   } // end of while swap flag
}



void predefined_enemies(void)
{
   while (mouse_b2) proc_controllers();
   if (load_PDE())
   {
      int x,y, EN = 0, redraw = 1, menu_sel;

      int quit = 0;
      while(!quit)
      {
         if (redraw)
         {
            int a,b=0;
            int rt = PDEi[EN][0];
            redraw = 0;

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

            a = PDEi[EN][1]; // bmp or ans
            if (a < NUM_SPRITES) b = a; // bmp
            if (a > 999) b = zz[5][a-1000]; // ans

            al_draw_bitmap(tile[b], 0,0,0);

            for (x=0; x<20; x++)
               al_draw_text(font, palette_color[15], 0, 20+(x*8), 0, PDEt[EN][x]);

            if (rt < 99)
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Predefined Enemy %d", EN);
               for (x=0; x<16; x++) // three columns
               {
                  al_draw_textf(font, palette_color[15], 256, 20+(x*8), 0, "Ef%-2d %+-f", x, al_fixtof(PDEfx[EN][x]));
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "Ei%-3d %+-d", x, PDEi[EN][x]);
                  al_draw_textf(font, palette_color[15], 500, 20+(x*8), 0, "Ei%-3d %+-d", x+16, PDEi[EN][x+16]);
               }
            }

            if ((rt > 99) && (rt < 200))
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Predefined Item %d", EN);
               for (x=0; x<16; x++)
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "I%-2d %+-d", x, PDEi[EN][x]);
            }
            if (rt > 199)
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Special Creator %d", EN);
               for (x=0; x<16; x++) // three columns
               {
                  al_draw_textf(font, palette_color[15], 256, 20+(x*8), 0, "Ef%-2d %+-f", x, al_fixtof(PDEfx[EN][x]));
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "Ei%-2d %+-d", x, PDEi[EN][x]);
                  al_draw_textf(font, palette_color[15], 500, 20+(x*8), 0, "Ei%-2d %+-d", x+16, PDEi[EN][x+16]);
               }
            }
         }
         menu_sel = (bottom_menu(5));   // call the menu handler
         al_flip_display();
         proc_controllers();
         al_rest(0.01);


         if ((key[ALLEGRO_KEY_RCTRL]) && (key[ALLEGRO_KEY_S])) // sort
         {
            while (key[ALLEGRO_KEY_S]) proc_controllers();
            PDE_sort();

            for (int x=0; x<50; x++) PDE_swap(x, x + 50); // move all to + 50

            int insert_pos = 0; // creators to 0
            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if (rt > 199) PDE_swap(x, insert_pos++);
            }

            insert_pos = 16; // items to 16   while (key[KEY_ESC]) proc_controllers();

            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if ((rt > 99) && (rt < 200)) PDE_swap(x, insert_pos++);
            }

            insert_pos = 32; // enemies to 32
            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if ((rt) && (rt < 99)) PDE_swap(x, insert_pos++);
            }
            redraw = 1;
         }


         // --------------  edit text
         if ((mouse_b1) && (mouse_x < 240) && (mouse_y > 20) && (mouse_y < 180))
         {
            int line_length = 30;
            int tx = mouse_x/8;
            int ty = (mouse_y-20)/8;
            int text_edit_quit = 0;
            while (!text_edit_quit)
            {
               // redraw
               // erase background
               al_draw_filled_rectangle(0, 20, 256, 180, palette_color[0]);
               al_draw_rectangle(0, 20, 256, 180, palette_color[14], 1);
               al_draw_text(font, palette_color[14], 128, 12,  ALLEGRO_ALIGN_CENTER, "Text Edit Mode");

               // redraw all the text
               for (x=0; x<20; x++)
               {
                  redraw = 0;
                  al_draw_text(font, palette_color[15], 0, 20+(x*8), 0, PDEt[EN][x]);
               }
               // mark the text entry position
               al_draw_filled_rectangle((tx*8), 20+(ty*8), (tx*8)+8, 20+(ty*8)+8, palette_color[138]);
               msg[0] = PDEt[EN][ty][tx];
               if (msg[0] == (char)NULL) msg[0] = 32;
               msg[1] = (char)NULL;
               al_draw_text(font, palette_color[10], (tx*8), 20+(ty*8), 0, msg);
               al_flip_display();


               proc_controllers();
               int k = Key_pressed_ASCII;
               if ((k>31) && (k<127)) // insert if alphanumeric or return
               {
                  int z = strlen(PDEt[EN][ty]);
                  if (z > line_length) z = line_length;
                  for (x=z; x>tx; x--)
                     PDEt[EN][ty][x] = PDEt[EN][ty][x-1];
                  PDEt[EN][ty][tx] = k;
                  if (++tx > line_length) // end of line?
                  {
                     PDEt[EN][ty][tx] = (char)NULL; // terminate the line
                     ty++;  // LF
                     tx = 0; // CR
                  }
               }

               if (key[ALLEGRO_KEY_BACKSPACE])
               {
                  while (key[ALLEGRO_KEY_BACKSPACE]) proc_controllers();
                  if (--tx<0) tx = 0;
                  int z = strlen(PDEt[EN][ty]);
                  for (x=tx; x<z; x++)
                     PDEt[EN][ty][x] = PDEt[EN][ty][x+1];
               }
               if (key[ALLEGRO_KEY_ENTER])
               {
                  while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
                  for (y=19; y>ty; y--)  // slide all down
                     strcpy(PDEt[EN][y],PDEt[EN][y-1]);
                  if (strlen(PDEt[EN][ty]) == 999) // cursor past end of line
                     PDEt[EN][ty+1][0] = (char)NULL; // next line empty
                  if ((signed int)strlen(PDEt[EN][ty]) >= tx) // cursor not past end of line
                  {
                     for (x=0; x <= 30-tx; x++)         // split line at tx
                         PDEt[EN][ty+1][x] = PDEt[EN][ty+1][tx+x];

                     PDEt[EN][ty][tx] = (char)NULL;  // terminate top line
                     tx = 0;
                     ty++;
                  }
               }
               if (key[ALLEGRO_KEY_DELETE])
               {
                  while (key[ALLEGRO_KEY_DELETE]) proc_controllers();
                  if (PDEt[EN][ty][tx] == (char)NULL)
                  {
                     for (x=0; x<=30-tx; x++) // get portion from line below
                         PDEt[EN][ty][tx+x] = PDEt[EN][ty+1][x];
                     for (y=ty+1; y<19; y++)  // slide all up
                        strcpy(PDEt[EN][y],PDEt[EN][y+1]);
                     PDEt[EN][19][0] = (char)NULL; // last line empty
                  }
                  else
                  {
                     for (x = tx; x < (int)strlen(PDEt[EN][ty]); x++)
                        PDEt[EN][ty][x] = PDEt[EN][ty][x+1];
                  }
               }

               if (key[ALLEGRO_KEY_RIGHT])
               {
                  while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
                  if (++tx > line_length-1) tx = line_length-1;
               }
               if (key[ALLEGRO_KEY_LEFT])
               {
                  while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
                  if (--tx < 0) tx = 0;
               }
               if (key[ALLEGRO_KEY_UP])
               {
                  while (key[ALLEGRO_KEY_UP]) proc_controllers();
                  if (--ty < 0) ty = 0;
               }
               if (key[ALLEGRO_KEY_DOWN])
               {
                  while (key[ALLEGRO_KEY_DOWN]) proc_controllers();
                  if (++ty > 19) ty = 19;
               }
               if ((mouse_b1) && (mouse_x < 250) && (mouse_y > 20) && (mouse_y < 180))
               {
                  ty = (mouse_y-20)/8;
                  tx = mouse_x/8;
               }
               if (tx > (signed int)strlen(PDEt[EN][ty])) tx = strlen(PDEt[EN][ty]);

               while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
               {
                  proc_controllers();
                  text_edit_quit = 1;
               }
            }
            redraw = 1;
         } // end of edit text



         if (key[ALLEGRO_KEY_RIGHT])
         {
            while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
            EN +=1;
            if (EN > 99) EN = 99;
            redraw =1;
         }

         if (key[ALLEGRO_KEY_LEFT])
         {
            while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
            EN -=1;
            if (EN < 0) EN = 0;
            redraw =1;
         }




         if (key[ALLEGRO_KEY_PGUP])
         {
            while (key[ALLEGRO_KEY_PGUP]) proc_controllers();
            EN +=10;
            if (EN > 99) EN = 99;
            redraw =1;
         }
         if (key[ALLEGRO_KEY_PGDN])
         {
            while (key[ALLEGRO_KEY_PGDN]) proc_controllers();
            EN -=10;
            if (EN < 0) EN = 0;
            redraw =1;
         }
         if ((key[ALLEGRO_KEY_RCTRL]) && (key[ALLEGRO_KEY_DELETE])) // DELETE PD
         {
            while (key[ALLEGRO_KEY_DELETE]) proc_controllers();
            for (int y=0; y<32; y++) PDEi[EN][y] = 0;
            for (int y=0; y<16; y++) PDEfx[EN][y] = al_itofix(0);
            for (int y=0; y<20; y++) strcpy(PDEt[EN][y],"");
            PDEi[EN][0] = 9999; // mark as empty
            redraw =1;
            //PDE_sort();
         }



         if (menu_sel == 1) // prev PDE
         {
            if (--EN < 0) EN = 0;
            redraw =1;
         }
         if (menu_sel == 2) // next PDE
         {
            if (++EN > 99) EN = 99;
            redraw =1;
         }
         if (menu_sel == 5) // save PDE
         {
            save_PDE();
            redraw =1;
         }
         if (menu_sel == 6) // load PDE
         {
            load_PDE();
            redraw =1;
         }

         if ((mouse_y > 20) && (mouse_y < 148) && (mouse_b1))  // edit variables
         {
            y = (mouse_y-20)/8;
            if ((mouse_x > 256) && (mouse_x < 400)) PDEfx[EN][y]   = edit_fix(296, 20+(y*8), PDEfx[EN][y]);
            if ((mouse_x > 400) && (mouse_x < 500)) PDEi[EN][y]    = edit_int(440, 20+(y*8), PDEi[EN][y], 1, -1, 20000);
            if ((mouse_x > 500) && (mouse_x < 600)) PDEi[EN][y+16] = edit_int(540, 20+(y*8), PDEi[EN][y+16], 1, -1, 20000);
            redraw=1;
        }

         while (key[ALLEGRO_KEY_ESCAPE])
         {
            quit = 1;
            proc_controllers();
         }
         if (menu_sel == -1) quit = 1;
      }
   }
}
























