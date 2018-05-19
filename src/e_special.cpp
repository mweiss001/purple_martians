// e_special.cpp (20100220 cleanup)
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

   al_draw_textf(font, palette_color[9],  swx1+2,   swy1+2, 0, "Status Window    level:%d ",level_num);
   al_draw_textf(font, palette_color[15], swx1+186, swy1+2, 0, "%d ",level_num);


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
            help((char *)"Status Window");
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





      // draw special blocka
      for (c=0; c<16*select_window_num_special_lines; c++)
      {
         if (c < 100) a = PDEi[c][1]; // bmp or ans
         else a = 0;
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(memory_bitmap[b], swx1+(c-((c/16)*16) )*20+1, swy1+14+select_window_special_y+1+(c/16*20), 0 );
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
         al_draw_bitmap(memory_bitmap[swbl[c][0]], swx1+(c-((c/16)*16) )*20+1, swy1+select_window_block_y+1+14+(c/16*20), 0 );


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
                  help((char *)"Selection Window");
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
                  if (strcmp(PDEt[ret][x],"<end>") == 0) tl = x;
               if (tl<5) tl = 5;

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
                        case 1: create_obj(2, 1, 0); break; // type 200 - door
                        case 2: create_obj(2, 5, 0); break; // type 201 - start
                        case 3: create_obj(2, 3, 0); break; // type 202 - exit
                        case 4: create_obj(2, 4, 0); break; // type 203 - key
                        case 5: create_obj(3, 7, 0); break; // type 204 - pod
                        case 7: create_obj(2, 10,0); break; // type 206 - msg
                        case 8: create_obj(3, 9, 0); break; // type 207 - cloner
                        case 9: if (create_lift()) object_viewer(4, num_lifts-1); break; // type 208 - lift
                        case 10: create_door(1); break; // type 209 - 1 way al_fixed exit door
                        case 11: create_door(2); break; // type 210 - 1 way linked exit door
                        case 12: create_door(3); break; // type 211 - 2 way door set


                     }
                     draw_big(1);
                     return 1001;
                  } // end of if creator   extern int sw_mouse_gone;

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

            al_draw_text (font, palette_color[15], swx1+2, syt+14, 0, "-----------------------");
            al_draw_textf(font, palette_color[15], swx1+2, syt+22, 0, "Block %d - %s ", ret, t);
            al_draw_text (font, palette_color[15], swx1+2, syt+30, 0, "-----------------------");

            if (mouse_b1)
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
   for (int c=0; c<NUM_SPRITES; c++)
      if ((sa[c][0] == 1) && (sa[c][1] == 1)) // if block and locked
            swbl[swbn++][0] = c;   // put shape # in list and inc counter
   for (int c=0; c<NUM_SPRITES; c++)
      if ((sa[c][0] == 1) && (sa[c][1] == 0)) // if block and unlocked
            swbl[swbn++][0] = c;   // put shape # in list and inc counter

   swnbl = (swbn / 16) + 1;
   if (swnbl_cur == 0) swnbl_cur = swnbl; // initial only
}



