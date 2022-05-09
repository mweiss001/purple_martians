// e_object_viewer_windows.cpp

#include "pm.h"


int create_obj(int obt, int sub_type, int sent_num)
{
   int num = sent_num; // default
   int ret;

   if (obt == 2) // items
   {
      ret = create_item(sub_type);
      if (ret > 500)
      {
         num = sent_num;
         al_show_native_message_box(display,
               "Error", "No creator exists for the current item type",
               "Copy from an existing item of that type, or get one from the selection window",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
      }
      else num = ret;
   }
   if (obt == 3)
   {
      if (sub_type == 7)
      {
         int e = create_pod();
         if ((e>=0) && (e<99) && (Ei[e][0] == 7))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
      if (sub_type == 9)
      {
         int e = create_cloner();
         if ((e>=0) && (e<99) && (Ei[e][0] == 9))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
      if (sub_type == 10)
      {
         int e = create_field();
         if ((e>=0) && (e<99) && (Ei[e][0] == 10))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
   }
   if (obt == 4) create_lift();
   return num;  // return number of created obj or sent_num if bad create
}

int ovw_get_size(int obt, int type, int*w, int*h)
{
   int ret = 0;

   if ((obt == 2) && (type == 1 )) { *w = 210; *h = 262; ret = 1;} // door
   if ((obt == 2) && (type == 2 )) { *w = 200; *h = 190; ret = 1;} // bonus
   if ((obt == 2) && (type == 3 )) { *w = 200; *h = 158; ret = 1;} // exit
   if ((obt == 2) && (type == 4 )) { *w = 200; *h = 182; ret = 1;} // key
   if ((obt == 2) && (type == 5 )) { *w = 200; *h = 174; ret = 1;} // start
   if ((obt == 2) && (type == 7 )) { *w = 200; *h = 158; ret = 1;} // mine
   if ((obt == 2) && (type == 8 )) { *w = 200; *h = 214; ret = 1;} // bomb
   if ((obt == 2) && (type == 9 )) { *w = 280; *h = 406; ret = 1;} // trigger
   if ((obt == 2) && (type == 10)) { *w = 220; *h = 390; ret = 1;} // message
   if ((obt == 2) && (type == 11)) { *w = 220; *h = 230; ret = 1;} // rocket
   if ((obt == 2) && (type == 12)) { *w = 220; *h = 214; ret = 1;} // warp
   if ((obt == 2) && (type == 14)) { *w = 200; *h = 142; ret = 1;} // switch
   if ((obt == 2) && (type == 15)) { *w = 240; *h = 166; ret = 1;} // sproingy
   if ((obt == 2) && (type == 16)) { *w = 280; *h = 358; ret = 1;} // bm
   if ((obt == 2) && (type == 17)) { *w = 290; *h = 358; ret = 1;} // bd


   if ((obt == 3) && (type == 3 )) { *w = 220; *h = 422; ret = 1;} // archwagon
   if ((obt == 3) && (type == 4 )) { *w = 220; *h = 302; ret = 1;} // bouncer
   if ((obt == 3) && (type == 6 )) { *w = 220; *h = 318; ret = 1;} // cannon
   if ((obt == 3) && (type == 7 )) { *w = 220; *h = 302; ret = 1;} // podzilla
   if ((obt == 3) && (type == 8 )) { *w = 220; *h = 342; ret = 1;} // trakbot
   if ((obt == 3) && (type == 9 )) { *w = 220; *h = 390; ret = 1;} // cloner
   if ((obt == 3) && (type == 11)) { *w = 220; *h = 350; ret = 1;} // block walker
   if ((obt == 3) && (type == 12)) { *w = 220; *h = 446; ret = 1;} // flapper

   if ((obt == 3) && (type == 10)) { *w = 280; *h = 350; ret = 1;} // field

   if (obt == 4)                   { *w = 300; *h = ov_window_lift_buttons_h; ret = 1;} // lift

   return ret;
}

void ovw_process_scrolledge(void)
{
   int bw = BORDER_WIDTH;
   int scrolledge = 10;
   int scroll_amount = 20;

   if (mouse_x < scrolledge) WX-=scroll_amount;           // scroll left
   if (mouse_x > SCREEN_W-scrolledge) WX+=scroll_amount;  // scroll right
   if (mouse_y < scrolledge) WY-=scroll_amount;           // scroll up
   if (mouse_y > SCREEN_H-scrolledge) WY+=scroll_amount;  // scroll down

      // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // used by get_new_background to only get what is needed
   level_display_region_x = WX;
   level_display_region_y = WY;
   level_display_region_w = SW;
   level_display_region_h = SH;

}

void ovw_get_block_position_on_map(int*x, int*y, int *hx, int *hy)
{
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mouse_x-BORDER_WIDTH;
   float my1 = mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (scale_factor_current * 20);
   float my2 = my1 / (scale_factor_current * 20);
   // get block position of WX
   float mx3 = (float)WX / 20;
   float my3 = (float)WY / 20;

   // add
   float mx4 = mx3 + mx2;
   float my4 = my3 + my2;

   *x = (int) mx4;
   *y = (int) my4;

   if (*x < 0)  *x = 0;
   if (*y < 0)  *y = 0;
   if (*x > 99) *x = 99;
   if (*y > 99) *y = 99;

   // hx, hy in 0-1999 scale
   // the mouse position past the border width is how far we are into the scaled map
   mx1 = mouse_x-BORDER_WIDTH;
   my1 = mouse_y-BORDER_WIDTH;

   // scale
   mx2 = mx1 / scale_factor_current;
   my2 = my1 / scale_factor_current;

   // get position of WX
   mx3 = (float)WX;
   my3 = (float)WY;

   // add
   mx4 = mx3 + mx2;
   my4 = my3 + my2;

   *hx = (int) mx4;
   *hy = (int) my4;

   if (*hx < 0)    *hx = 0;
   if (*hy < 0)    *hy = 0;
   if (*hx > 1999) *hx = 1999;
   if (*hy > 1999) *hy = 1999;
}

void ovw_proc_move_window(int obt, int num, int type)
{
   if ((mouse_x > ov_window_x1) && (mouse_x < ov_window_x2) && (mouse_y > ov_window_y1) && (mouse_y < ov_window_y1+14)) // is mouse on title bar
   {
      // draw rectangle around title bar to show it can be dragged
      // al_draw_rectangle(ov_window_x, ov_window_y, ov_window_x2, ov_window_y+14, palette_color[14], 1);
      // color text in title bar to show it can be moved
      //msg[0] = 0;
      //if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
      //if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
      //int ov_xc = (ov_window_x + ov_window_x2) / 2;
      //al_draw_text(font, palette_color[10], ov_xc, ov_window_y+2, ALLEGRO_ALIGN_CENTER,  msg);
      if (mouse_b1)
      {
         int mxo = mouse_x - ov_window_x1; // get offset from mouse position to window x, y
         int myo = mouse_y - ov_window_y1;
         while (mouse_b1)
         {
            ov_window_x1 = mouse_x - mxo;
            ov_window_y1 = mouse_y - myo;
            ov_window_x2 = ov_window_x1 + ov_window_w;
            ov_window_y2 = ov_window_y1 + ov_window_h;
            ovw_redraw_background(obt, num, type, 0, 1);
         }
      }
   }
}

int ovw_redraw_background(int obt, int num, int type, int legend_line, int show_window)
{
   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();
   if (obt == 4) init_level_background(); // to draw new lift lines
   get_new_background(0);
   draw_lifts();
   draw_items();
   draw_enemies();

   ovw_draw_overlays(obt, num, legend_line);

   // if current object is message, show all messages
   if ((obt == 2) && (type == 10))
   {
      for (int i=0; i<500; i++)
         if (item[i][0] == 10) draw_pop_message(i);
   }

   get_new_screen_buffer(3, 0, 0);

   int mb = 0;
   if (show_window)
   {
      mb = ovw_draw_buttons(obt, num, type);
      ovw_title(obt, num, 0); // draw button title, frame and legend lines
   }
   return mb;
}

void ovw_title(int obt, int num, int legend_highlight)
{
   int sub_type=0;
   if (obt == 2) sub_type = item[num][0];
   if (obt == 3) sub_type = Ei[num][0];
   int ov_xc = ov_window_x1 + ov_window_w/2;
   int yt = ov_window_y1+14;

   // legend line text
   char lmsg[5][80];
   for (int x=0; x<5; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[5];

   // default number of legend lines
   num_legend_lines = 2;

   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused

   if (legend_highlight == 1) legend_color[1] = flash_color;

   if (!legend_highlight)
   {
      // title bar
      msg[0] = 0;
      if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
      if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
      if (obt == 4) sprintf(msg, "Lift Viewer [%d]", num);
      for (int x=0; x<15; x++)
         al_draw_line(ov_window_x1, ov_window_y1+x, ov_window_x2, ov_window_y1+x, palette_color[13+(x*16)], 1);
      al_draw_text(font, palette_color[15], ov_xc, ov_window_y1+2, ALLEGRO_ALIGN_CENTER,  msg);
   }

   if (obt == 4)  // lifts
   {
      num_legend_lines = 0;
      al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
      al_draw_textf(font, palette_color[13], ov_xc, yt+8, ALLEGRO_ALIGN_CENTER, "Lift %d of %d",num+1, num_lifts);
   }
   if (obt == 3)  // enemies
   {
      if (!legend_highlight)
      {
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_enemy(num, 1, ov_xc-92, yt+1);
         sprintf(msg,"%s %d of %d", (const char *)enemy_name[sub_type],1+num - e_first_num[sub_type],e_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], ov_xc-69, yt+8, 0, msg);
      }
      switch (sub_type)
      {
         case 3: // archwagon
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 4: sprintf(lmsg[1],"Bouncer Location"); break;
         case 6: sprintf(lmsg[1],"Cannon Location"); break;
         case 7: // podzilla
         {
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Extended Postion");
            sprintf(lmsg[3],"Trigger Box");
            num_legend_lines = 4;

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;

            legend_color[3] = 14;
            if (legend_highlight == 3) legend_color[3] = flash_color;
         }
         break;
         case 8: // trakbot
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 9: // cloner
         {
            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            sprintf(lmsg[4],"Trigger Box");
            num_legend_lines = 5;

            legend_color[2] = 11;
            if (legend_highlight == 2) legend_color[2] = flash_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = flash_color;

            legend_color[4] = 14;
            if (legend_highlight == 4) legend_color[4] = flash_color;
         }
         break;
         case 10: // field
         {
            sprintf(lmsg[1],"Field Location");
            sprintf(lmsg[2],"Field Area");
            sprintf(lmsg[3],"Trigger Box");
            num_legend_lines = 4;

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;

            legend_color[3] = 14;
            if (legend_highlight == 3) legend_color[3] = flash_color;
         }
         break;
         case 11: sprintf(lmsg[1],"Block Walker Location"); break;
         case 12: // flapper
         {
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Trigger Box");
            sprintf(lmsg[3],"Height Above Player");
            num_legend_lines = 4;

            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = flash_color;
         }
         break;
      }
   }
   if (obt == 2)  // items
   {
      if (!legend_highlight)
      {
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_item(num, 1, ov_xc-94, yt+1);
         sprintf(msg,"%s %d of %d", item_name[sub_type], 1+num - item_first_num[sub_type],item_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], ov_xc-69, yt+8, 0, msg);
      }
      switch (sub_type)
      {
         case 1: // door
         {
            num_legend_lines = 3;
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;

            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (item[num][8] == 1)  // draw destination
            {
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2: sprintf(lmsg[1],"Bonus Location"); break;
         case 3: sprintf(lmsg[1],"Exit Location"); break;
         case 4: // key
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 5: sprintf(lmsg[1],"Start Location"); break;
         case 7: sprintf(lmsg[1],"Mine Location"); break;
         case 8:
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 9: // trigger
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 10:
         {
            sprintf(lmsg[1],"Message Location");
            sprintf(lmsg[2],"Display Position");
            num_legend_lines = 3;
         }
         break;
         case 11:
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 12: sprintf(lmsg[1],"Warp Location"); break;
         case 14: sprintf(lmsg[1],"Switch Location"); break;
         case 15: sprintf(lmsg[1],"Sproingy Location");
         {
            num_legend_lines = 3;
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 16: // block manip
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Block Manip Item Location");
            sprintf(lmsg[2],"Manip Field");
            legend_color[2] = 12;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
         case 17: // block damage
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = flash_color;
         }
         break;
      } // end of switch case
   }  // end of items

   if (!legend_highlight)
   {
      ov_window_h += num_legend_lines*8 + 8;
      ov_window_y2 = ov_window_y1 + ov_window_h;
      if (num_legend_lines > 0)
      {
         al_draw_text(font, palette_color[legend_color[0]], ov_xc, ov_window_y2-36+ (4-num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
         al_draw_rectangle(ov_xc-100, ov_window_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_window_y2-1, palette_color[13], 1); // big frame
         al_draw_rectangle(ov_xc-100, ov_window_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_window_y2-28+ (4-num_legend_lines)*8, palette_color[13], 1); // top frame
      }
      al_draw_rectangle(ov_window_x1, ov_window_y1, ov_window_x2, ov_window_y2, palette_color[13], 1);  // outline entire window

   }

   for (int x=1; x<num_legend_lines; x++)// draw text lines
      al_draw_text(font, palette_color[legend_color[x]], ov_xc, ov_window_y2-26+(3-num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);

}

void ovw_draw_overlays(int obt, int num, int legend_highlight)
{
   al_set_target_bitmap(level_buffer);

   if (obt == 4)  // lifts
   {
      int lift = num;
      int step = lifts[lift].current_step;
      int color = (lift_steps[lift][step].type >> 28) & 15;

      int x1 = lift_steps[lift][step].x-1;
      int y1 = lift_steps[lift][step].y-1;
      int x2 = x1 + lift_steps[lift][step].w+2;
      int y2 = y1 + lift_steps[lift][step].h+2;
      int xc = (x1 + x2) / 2;
      int yc = (y1 + y2) / 2;

      al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);

      al_draw_line(xc, 0, xc, y1, palette_color[color], 1);
      al_draw_line(xc, y2, xc, 2000, palette_color[color], 1);
      al_draw_line(0, yc, x1, yc, palette_color[color], 1);
      al_draw_line(x2, yc, 2000, yc, palette_color[color], 1);
   }
   if (obt == 3)  // enemies
   {
      int sub_type = Ei[num][0];
      int obj_x = al_fixtoi(Efi[num][0])+10;
      int obj_y = al_fixtoi(Efi[num][1])+10;

      int color = 13;
      if (legend_highlight == 1) color = flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);

      switch (sub_type)
      {
         case 3: // archwagon
         {
            // yellow bullet prox
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            int bs = Ei[num][17];
            al_draw_rectangle(obj_x-bs, obj_y-bs, obj_x+bs, obj_y+bs, palette_color[color], 1);
         }
         break;
         case 7: // podzilla
         {
            // extended position
            int color1 = 10;
            if (legend_highlight == 2) color1 = flash_color;


            int px=0, py=0;
            get_pod_extended_position(num, &px, &py);
            crosshairs_full(px+10, py+10, color1, 1);

            // draw tile at extended pos
            float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
            al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, px+10, py+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

            // draw connecting line
            al_draw_line(obj_x, obj_y, px+10, py+10, palette_color[10], 1);


            // trigger box
            int color = 14;
            if (legend_highlight == 3) color = flash_color;
            int tx1 = Ei[num][11];
            int ty1 = Ei[num][12];
            int tx2 = Ei[num][11]+Ei[num][13] + 20;
            int ty2 = Ei[num][12]+Ei[num][14] + 20;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color], 1);
         }
         break;
         case 8: // trakbot
         {
            // draw yellow bullet prox circle
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            al_draw_circle(obj_x, obj_y, Ei[num][17], palette_color[color], 1);
         }
         break;
         case 9: // cloner
         {
            int color2 = 11;
            if (legend_highlight == 2) color2 = flash_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = flash_color;

            int color4 = 14;
            if (legend_highlight == 4) color4 = flash_color;

            int cw = Ei[num][19];     // width
            int ch = Ei[num][20];     // height

            int cx1 = Ei[num][15];    // source
            int cy1 = Ei[num][16];
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            int cx3 = Ei[num][17];    // destination
            int cy3 = Ei[num][18];
            int cx4 = cx3 + cw;
            int cy4 = cy3 + ch;
            al_draw_rectangle(cx3, cy3, cx4, cy4, palette_color[color3], 1);

            // draw trigger box
            int tx1 = Ei[num][11];
            int ty1 = Ei[num][12];
            int tx2 = Ei[num][11]+Ei[num][13] + 20;
            int ty2 = Ei[num][12]+Ei[num][14] + 20;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color4], 1);
         }
         break;
         case 10: // field
         {

            int color2 = 10;
            if (legend_highlight == 2) color2 = flash_color;

            int color3 = 14;
            if (legend_highlight == 3) color3 = flash_color;

            int cw = Ei[num][17];     // width
            int ch = Ei[num][18];     // height
            int cx1 = Ei[num][15];    // source
            int cy1 = Ei[num][16];
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            // draw trigger box
            int tw = Ei[num][13];     // width
            int th = Ei[num][14];     // height
            int tx1 = Ei[num][11];
            int ty1 = Ei[num][12];
            int tx2 = tx1 + tw;
            int ty2 = ty1 + th;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color3], 1);
         }
         break;
         case 12: // flapper
         {
            int color2 = 14;
            if (legend_highlight == 2) color2 = flash_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = flash_color;

            // draw yellow bullet prox
            int bw  = Ei[num][17];
            int by1 = Ei[num][18];
            int by2 = Ei[num][19];
            al_draw_rectangle(obj_x-bw, obj_y-by1, obj_x+bw, obj_y+by2, palette_color[color2], 1);

            // draw red height above player line
            int hab = Ei[num][20];
            al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, palette_color[color3], 3);

            // draw flap height
            int fh = Ei[num][21];
            al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, palette_color[12], 1);
            al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, palette_color[12], 1);

         }
         break;
      }
      al_reset_clipping_rectangle();

   }
   if (obt == 2)  // items
   {
      int sub_type = item[num][0];
      int obj_x = item[num][4]+10;
      int obj_y = item[num][5]+10;
      int color = 13;
      if (legend_highlight == 1) color = flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);
      switch (sub_type)
      {
         case 1: // door
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                     int x2 = item[i][4]+10;
                     int y2 = item[i][5]+10;
                     crosshairs_full(x2, y2, color, 1);
                     al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
                  }
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]+10;
               int y2 = item[di][5]+10;
               crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
            }
         }
         break;
         case 4: // key
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);
         }
         break;
         case 8: // bomb
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 9: // trigger
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;

            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(1, num, 0);
         }
         break;
         case 11: // rocket
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 15: // sproingy
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            int y = al_fixtoi(get_sproingy_jump_height(num));
            crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
         {
            int color = 12;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);

         }
         break;
         case 17: // block damage
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);
         }
         break;

      } // end of switch case
   }
}

int ovw_draw_buttons(int obt, int num, int type)
{
   // erase background
   if (ovw_get_size(obt, type, &ov_window_w, &ov_window_h))
   {
      ov_window_x2 = ov_window_x1 + ov_window_w;
      ov_window_y2 = ov_window_y1 + ov_window_h;
      al_draw_filled_rectangle(ov_window_x1, ov_window_y1, ov_window_x2, ov_window_y2, palette_color[0]);
      //printf("x1:%d y1:%d x2:%d y2:%d w:%d h:%d\n", ov_window_x1, ov_window_y1, ov_window_x2, ov_window_y2, ov_window_w, ov_window_h);
   }
   else ov_window_w = 300;

   // button x position
   int xa = ov_window_x1+1;
   int xb = ov_window_x2-1;
   int ya = ov_window_y1+38;


   // split into half
//   int x12 = xa + 1 * (xb-xa) / 2; // 1/2

   // split into thirds
   int x13 = xa + 1 * (xb-xa) / 3; // 1/3
   int x23 = xa + 2 * (xb-xa) / 3; // 2/3

   // split into quarters
   int x14 = xa + 1 * (xb-xa) / 4; // 1/4
   //int x25 = xa + 2 * (xb-xa) / 5; // 2/5
   //int x35 = xa + 3 * (xb-xa) / 5; // 3/5
   //int x45 = xa + 4 * (xb-xa) / 5; // 4/5

   // split into fifths
//   int x15 = xa + 1 * (xb-xa) / 5; // 1/5
   //int x25 = xa + 2 * (xb-xa) / 5; // 2/5
   //int x35 = xa + 3 * (xb-xa) / 5; // 3/5
   //int x45 = xa + 4 * (xb-xa) / 5; // 4/5

   // split into sevenths
   //int x17 = xa + 1 * (xb-xa) / 7; // 1/7
   int x27 = xa + 2 * (xb-xa) / 7; // 2/7
   //int x37 = xa + 3 * (xb-xa) / 7; // 3/7
   //int x47 = xa + 4 * (xb-xa) / 7; // 4/7
   int x57 = xa + 5 * (xb-xa) / 7; // 5/7
   //int x67 = xa + 6 * (xb-xa) / 7; // 6/7

   //mdw_slider(xb-40,  ya-33,    xb,    ya-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height
   //mdw_slider(xa,  ya-35,    xb,    ya-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height

   int lc = 6; // lock_color;
   if (viewer_lock) lc = 7;

   int a=0;
   int mb = 0;   // button selection
   if (mdw_button(xa,  ya+a*bts, x27-1, ya+(a+1)*bts-2, 23, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 22; // prev
       mdw_button(x27, ya+a*bts, x57-1, ya+(a+1)*bts-2, 56, num, type, obt, 0, lc, 15, 0, 1,0,0,0);          // lock
   if (mdw_button(x57, ya+a*bts, xb,    ya+(a+1)*bts-2, 22, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 21; // next
   a++;
   if (mdw_button(xa,  ya+a*bts, x13-1, ya+(a+1)*bts-2, 19, num, type, obt, 0, 13, 15, 0, 1,0,0,0)) mb = 18; // move
   if (mdw_button(x13, ya+a*bts, x23-1, ya+(a+1)*bts-2, 20, num, type, obt, 0, 14, 15, 0, 1,0,0,0)) mb = 19; // create
   if (mdw_button(x23, ya+a*bts, xb,    ya+(a+1)*bts-2, 21, num, type, obt, 0, 10, 15, 0, 1,0,0,0)) mb = 20; // delete
   a++;
   if (mdw_button(xa,  ya+a*bts, x14-1, ya+(a+1)*bts-2, 25, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 24; // viewer help
   if (mdw_button(x14, ya+a*bts, xb,    ya+(a+1)*bts-2, 57, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 25; // specific object help
   a+=2;

   a = obj_buttons(obt, num, xa, xb, ya, a, bts);

   if ((obt ==2) || (obt == 3))
   {
      ov_window_h = 38+a*bts;
   }

   ov_window_y2 = ov_window_y1 + ov_window_h;

  // printf("y2:%d h:%d\n", ov_window_y2, ov_window_h);

   return mb;

}

int obj_buttons(int obt, int num, int xa, int xb, int ya, int a, int bts)
{
   int mb = 0;

   if (obt == 4) // lifts
   {
      int lift = num;
      int step = lifts[num].current_step;

      // mode, mode 1 timer and name
      mdw_button(                             xa, ya+a*bts, xb, ya+(a+1)*bts-2, 500, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift mode
      if (lifts[lift].mode == 1) { mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 555, lift, 0, 0, 0, 13, 15, 15, 1,0,0,0); a++; }   // lift mode 1 player ride timer
      if (mdw_button(                         xa, ya+a*bts, xb, ya+(a+1)*bts-2, 504, lift, 0, 0, 0,  4, 15,  0, 1,0,0,0)) mb = 26; // lift name
      a+=2;

      // list of step buttons
      // --------------------------------------------------------------------------------
      int ysb = ya + (a*bts); // y pos of step buttons

      // process mouse on list of step buttons
      int step_pointer = -99;
      if ((mouse_x > xa + 10) && (mouse_x < xb - 10)) // is mouse on step buttons?
      {
         int step0_y_offset = ysb + 14 + bts;
         int mouse_step = (mouse_y - step0_y_offset) / bts;               // calculate step that mouse is on
         if ((mouse_step >= 0) && (mouse_step < lifts[lift].num_steps))   // is this a valid step?
         {
            step_pointer = mouse_step;  // set step pointer to this step
            if (mouse_b1)
            {
               step = mouse_step; // set current step to this step
               lifts[lift].current_step = step; // set current step in lift
            }
            if (mouse_b2) step_popup_menu(lift, step_pointer); // step pop-up menu for this step
         }
      }

      // draw the list of steps  - this has to go after, because it can eat the mouse clicks needed for previous section
      int ycs = ysb + draw_steps(xa, xb, ysb, lift, step, step_pointer);
      ycs +=bts;

      // draw buttons for the current step button and get y postion for next item (lift)
      int yld = ycs + draw_current_step_buttons(xa, xb, ycs, lift, step);
      ov_window_lift_buttons_h = yld - ov_window_y1-9; // global variable for height of ovw when variable due to lift
      yld += bts;

      // draw current lift under step list buttons
      if (bts == 16) // only if max button size
      {
         int x1 = xa+10;
         int y1 = yld; // only see in 2 highest screen modes
         int s = step;

         // if step is not a move step, find prev that is
         if ((lift_steps[lift][s].type & 31) != 1) s = lift_find_previous_move_step(lift, s);

         // get w h from step
         int w  = lift_steps[lift][s].w;
         int h  = lift_steps[lift][s].h;

         // get x2 and y2 based on x1 y1 and w h
         int x2 = x1 + w;
         int y2 = y1 + h;
         draw_lift(lift, x1, y1, x2, y2);
      }

      if (mb == 26)
      {
         char fst[80];
         strcpy(fst, lifts[lift].lift_name);
         if (edit_lift_name(lift, yld, xa+10, fst)) strcpy(lifts[lift].lift_name, fst);
      }
   }


 //  printf("xa:%d xb:%d ya:%d, a:%d bts:%d obt:%d type:%d num:%d\n",xa, xb, ya, a, bts, obt, type, num);


   int d = 96; // dim
   if (obt == 3)    // enemies
   {
      int type = Ei[num][0];
      switch (type) // enemy subtypes
      {
         case 3:     // archwag
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 9, 0.7, 0.01, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:"); a+=2;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15); a++;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15); a+=2;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off"); a+=2;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 4: // bouncer
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][8], 100, 0, 1,   "Seek Count:"); a+=2;
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 13, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // main shape
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 14, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // seek shape
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,    "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,  "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,    "Health Bonus:"); a++;
         break;
         case 6: // cannon
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][8], 100, 0, 1,   "Seek Count:"); a+=2;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 8, 15, 15,  1,0,0,0, Ei[num][9], 40, 0, 1,     "Extra Hits To Kill:"); a+=2;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15, 1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 7: // podzilla
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 29, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 30, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // pause
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 15, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // move extended position
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a+=2;  // set trigger box
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 8: // trakbot
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 11, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][2], 10, .5, 0.1, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 10, .5, 0.1, "Y-Speed:"); a+=2;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][7], "Drop Mode:Off", "Drop Mode:On ", 15, 15, 11, 11); a+=2;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 9: // cloner
            mdw_toggle(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][30], "Not Invincible", "Invincible", 15, 15, 12, 12); a++;
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 27, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++; // trigger type
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][6], 1000, 20, 1, "Delay Timer:"); a++;
            a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15,  1,0,0,0, Ei[num][9], 4800, 0, 1, "Created Time To Live:", "-"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15,  1,0,0,0, Ei[num][10], 600, 0, 1, "Max Created Objects:", "-"); a++;
            a++;
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 17, num, type, obt, 0, 11, 11,  0, 1,0,0,0); a++; // get source
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 18, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++; // get destination
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a++; // get trigger box
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 81, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; // show boxes
            a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 11: // block walker
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 9, 0.7, 0.01, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:"); a++;
            a++;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15); a++;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15); a++;
            a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off"); a++;
            mdw_slider0_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off"); a++;
            a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 12: // flapper
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][5], 8, .5, 0.1,  "X-Speed:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 2, 0.1, 0.1, "X-Accel:"); a++;
            mdw_toggle(     xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 15, 15, 12, 12); a++;
            a++;

            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Efi[num][3], 5, 0, 0.01,      "Y-Speed:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Efi[num][10], 8, .5, 0.1,     "Flap Speed:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Ei[num][21], 400, 0, 10,      "Flap Height:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Ei[num][20], 1000, -1000, 10, "Height Above Player:"); a++;
            a++;

            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][17], 500, 20, 1, "Trigger Width:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][18], 600, 1, 10, "Trigger Height:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][19], 600, 1, 11, "Trigger Depth:"); a++;
            a++;

            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;



         case 10: // field
         {
            int FLAGS = Ei[num][3];
            int MODE = Ei[num][5];

            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 98,  num, type, obt, 0, 15, 15,  0, 1,0,0,0); a+=2; // Draw Mode
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 85,  num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // Get New Damage Field
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 102, num, type, obt, 0, 10, 15,  0, 1,0,0,0); a++;  // Damage Field draw type

            if (FLAGS & PM_ENEMY_FIELD_LIFT_DMG_ON)
            {
                mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 134, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // Damage Field follows lift:ON
                mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 84,  num, type, obt, 0,  13,   15, 15, 1,0,0,0); a++;  // Lift Number
                mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 136, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // X Alignment
                mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 137, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // Y Alignment

            }
            else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 134, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Damage Field follows lift:OFF

            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_PLAYER)
            {
                   mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 120, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Damage Field Affects Player ON
                   mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 83,  num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Player damage
            }
            else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 120, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Damage Field Affects Player OFF


            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_ENEMY) { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 121, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 121, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_ITEM)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 122, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 122, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_PBUL)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 123, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 123, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_EBUL)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 124, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 124, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }


            a++; // leave a blank line

            if (MODE == 0) { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 87, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++; } // MODE (0) // always on


            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2 and 3
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 87,  num, type, obt, 0,  8, 15,  0, 1,0,0,0); a++;  // MODE


               if (MODE == 1) // toggle
               {
                  mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 133, num, type, obt, 0, 11, 15,  0, 1,0,0,0); a++;  // Set Damage Field Initially ON or OFF
               }

               if (MODE == 2) // Damage Field Off Time
               {
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 90,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;
               }

               if (MODE == 3) // Damage Field On Time
               {
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 89,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;
               }

               a++; // leave a blank line


               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 86,  num, type, obt, 0, 14, 14,  0, 1,0,0,0); a++;   // Get New Trigger Field
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 105, num, type, obt, 0, 14, 15,  0, 1,0,0,0); a++;   // Trigger Field draw type

               if (FLAGS & PM_ENEMY_FIELD_LIFT_TRG_ON) // Trigger Field follows lift:ON
               {
                      mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 135, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Trigger Field follows lift:ON
                      mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 85,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
                      mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 138, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
                      mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 139, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment



               }
               else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 135, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // Trigger Field follows lift:OFF

               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_PLAYER) { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 125, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 125, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_ENEMY)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 126, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 126, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_ITEM)   { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 127, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 127, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_PBUL)   { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 128, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 128, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_EBUL)   { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 129, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 129, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }

            }

            if (MODE == 4) // Timed on and off
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 87, num, type, obt, 0,  8, 15,  0, 1,0,0,0); a+=2; // mode
               mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 86, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // total timer
               mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 88, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // damage time
               mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 87, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // initial time
            }
         }
         break;
      }
   }
   if (obt == 2) // items
   {
      int type = item[num][0];
      switch (type) // item subtypes
      {
         case 1: // door
          {
            int abc = 11; // regular button frame color
            int atc = 15; // regular button text color

            if (item[num][8] == 0) // if exit only change color of inactive buttons
            {
                abc = 15;
                atc = 127;
            }
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 26, num, type, obt, 0,  15,  13, 14, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 49, num, type, obt, 0, 12 ,  15,  0, 1,0,0,0); a++; // door type
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 4,  num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // set linked item
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 50, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // enter mode (up | down)
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 53, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // move type
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 51, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // exit link show
            mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 52, num, type, obt, 0,  13,  15,  0, 1,0,0,0); a++; // get new shape
            mdw_colsel(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 5,  num, type, obt, 0,   0,   0,  0, 0,0,0,0); a++; // change color
         }
         break;
         case 2: // bonus
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            a++;
            if (item[num][6] == 1)
            {
               mdw_buttonp(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 101, num, type, obt, 0, 11, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               mdw_slider2_int(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 0,0,0,0,  0, 11, 15, 15, 1,0,0,0, item[num][7], 100, 2, 1, "Health Bonus:"); a++;
               item[num][1] = 1035;
               item[num][2] = 1;
               item[num][3] = 1;
               if (item[num][7] == 10) item[num][1] = 1023;
               if (item[num][7] == 50) item[num][1] = 1027;
            }
            else if (item[num][6] == 2)
            {
               mdw_buttonp(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 101, num, type, obt, 0, 10, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               a++;
               item[num][1] = 1038;
               item[num][2] = 1;
               item[num][3] = 1;
            }
            else if (item[num][6] == 3)
            {
               mdw_buttonp(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 101, num, type, obt, 0, 8, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               a++;
               item[num][1] = 197;
               item[num][2] = 0;
               item[num][3] = 1;
            }
         break;
         case 3: // exit
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider2_int(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, item[num][8], 100, 0, 1,  "Enemy Count Lock:"); a++;
         break;
         case 4: // key
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 2,  num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 5,  num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++; // set new block range
            mdw_toggle(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, item[num][12], "Remove All Blocks", "Remove Only Matching", 15, 15, 4, 4); a++; // range type
         break;
         case 5: // start
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 78, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // start mode
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 79, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // start index
         break;
         case 7: // mine
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 11, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // mine damage
         break;
         case 8: // bomb
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 76, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // sticky
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 77, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // fuse timer / remote detonator
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  5, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // fuse length
         break;
         case 9: // trigger
         {
            int FLAGS = item[num][3];

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 200,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // Get New Trigger Field




            // draw trigger field on/off with optional color select if on
            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF           ","Draw Trigger Field:ON            ", 15+96, 15, 15+96, item[num][2]); a++; // Draw on/off
            if (FLAGS & PM_ITEM_TRIGGER_DRAW_ON)
            {
               mdw_colsel(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  6,   num, type, obt, 0,  0,   0,  0, 0,0,0,0); a++; // color select
            }
            a++;

            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF         ","Triggered by Players:ON          ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF         ","Triggered by Enemies:ON          ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF           ","Triggered by Items:ON            ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_PBUL,   "Triggered by Player's Bullets:OFF","Triggered by Player's Bullets:ON ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_EBUL,   "Triggered by Enemy's Bullets:OFF ","Triggered by Enemy's Bullets:ON  ", 15+d, 15, 14+d, 14); a++;
            a++;

            if (FLAGS & PM_ITEM_TRIGGER_LIFT_ON) // Trigger Field follows lift:ON
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 210, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Trigger Field follows lift:ON
               mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 91,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 211, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 212, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment
            }
            else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 210, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // Trigger Field follows lift:OFF

            a++;


            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 93,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output CURR ON
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 94,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output CURR OFF
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 95,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output TGON
            mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 96,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output TGOF


         }
         break;


         case 16: // block manip

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 300,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // Get New Block Range



            if (item[num][2])
            {
               int col = item[num][12];
               mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 304,  num, type, obt, 0, col, 15, 15, 1,0,0,0); a++; // Draw on/off
               mdw_colsel(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  7,   num, type, obt, 0,  0,   0,  0, 0,0,0,0); a++; // color select

            }
            else
            {
               mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 304,  num, type, obt, 0, 15+96, 15+96, 15, 1,0,0,0); a++; // Draw on/off
            }
            a++;

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 301,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // mode

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 310,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a++;  // block 1
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 311,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // block 2

            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 92,   num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // pm_event trigger

            a++;

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 320,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // set trigger

         break;


         case 17: // block damage
         {
            int FLAGS = item[num][3];
            int MODE = item[num][11];

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 400,  num, type, obt, 0, 10, 10, 10, 1,0,0,0); a+=2; // Get New Damage Field

            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 404,  num, type, obt, 0, 8, 15, 15, 1,0,0,0); a+=2; // damage draw mode


            if (MODE == 0) { mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 402,  num, type, obt, 0, 10, 15, 14, 1,0,0,0); a++; } //  MODE 0


            if (MODE == 1)
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 402, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++;  // MODE 1
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 433, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++;  // Set Damage Field Initially ON or OFF
            }



            if ((MODE == 2) || (MODE == 3) || (MODE == 4)) // Mode 2, 3 and 4
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 402,  num, type, obt, 0,  11, 15,  0, 1,0,0,0); a++;  // MODE

               if (MODE == 2) { mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 104,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;} // Damage Field Off Time
               if (MODE == 3) { mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 103,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;} // Damage Field On Time
               if (MODE == 4) // Timed on and off
               {
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 100, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // total timer
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 102, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // damage time
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 101, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // initial time
               }
               mdw_button(   xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 401, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // timer draw mode
            }

            a++;

            if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) // DAMAGE Field follows lift:ON
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 410, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // DAMAGE Field follows lift:ON
               mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 97,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 411, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 412, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment
            }
            else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 410, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // DAMAGE Field follows lift:OFF

            a++;

            if (FLAGS & PM_ITEM_DAMAGE_PLAYER)
            {
               mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 420, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Item Damage Affects Player ON

               if (item[num][3] & PM_ITEM_DAMAGE_INSTGIB)
               {
                  mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 403, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++; // Instant death for player:ON
               }
               else
               {
                  mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 403, num, type, obt, 0, 10+d,  15+d,0, 1,0,0,0); a++;   // Instant death for player:OFF
                  mdw_slider(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 98,  num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Player damage
               }
            }
            else { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 420, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Item Damage Affects Player OFF

            if (FLAGS & PM_ITEM_DAMAGE_ENEMY) { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 421, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 421, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_ITEM)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 422, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 422, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_PBUL)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 423, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 423, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_EBUL)  { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 424, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ya+(a*bts), xb, ya+(a+1)*bts-2, 424, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            a++;
            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 92,   num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++; // pm_event trigger
               a++;
               mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 320,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // set trigger
            }
         }
         break;
         case 10: // message
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 55, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a++; // set msg position
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 54, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // display time
            mdw_colsel(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  2, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // frame color select
            mdw_colsel(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  3, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // text color select
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  7, num, type, obt, 0, 15, 13, 14, 1,0,0,0); a++; // edit text
            a+=2; // leave space for OK and Cancel buttons

            // draw the current one last to ensure it is on top
            pop_msg_viewer_pos = ya+a*bts+bts/2+2;
            display_pop_message(num, pmsgtext[num], (xa+xb)/2, pop_msg_viewer_pos, 0, 0); // show the message
            a+=8;

         break;
         case 11: // rocket
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 82, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | ride through door
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  6, num, type, obt, 0, 15, 13,  0, 1,0,0,0); a++; // set initial direction
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  6, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // acceleration
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  7, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // max speed
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  8, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // steerability
         break;
         case 14: // switch
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
         break;
         case 15: // sproingy
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // jump height
         break;
         case 12: // warp
            mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 26, num, type, obt, 0, 15, 13, 0,  1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 28, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // warp level
         break;

      } // end of switch type
   } // end of if obt = item
   return a;
}

void ovw_map_move(int &obt, int &num)
{
   int gx=0, gy=0, hx=0, hy=0;
   ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

   int type=0, lift=0, step=0;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];
   if (obt == 4)
   {
      lift = num;
      step = lifts[lift].current_step;
   }

   int mouse_on_obj = 0;
   int mouse_on_podx = 0;
   int mouse_on_podx_num = 0;
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;
   int mouse_on_trk = 0;

   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_msg_ul = 0;
   int mouse_on_kbr_ul = 0;
   int mouse_on_kbr_lr = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;

   int mouse_on_lift = 0;

   int mouse_move = 0;
   int mouse_adj = 0;

   int mst = 6;      // mouse select threshold
   int msn = 10-mst; // mouse select negative threshold
   int msp = 10+mst; // mouse select positive threshold

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on enemy
   // -----------------------------------------------------------
   for (int b=0; b<100; b++)
      if ((Ei[b][0]) && (obj_filter[3][Ei[b][0]])) // iterate valid enemies that have the obj_filter on
      {
         int ex = al_fixtoi(Efi[b][0]);
         int ey = al_fixtoi(Efi[b][1]);
         int type = Ei[b][0];
         if ((hx>ex+msn) && (hx<ex+msp) && (hy>ey+msn) && (hy<ey+msp)) // set this enemy to current object
         {
            obt = 3;
            num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         if (type == 8) // trakbot bullet prox
         {
            float x0 = al_fixtof(Efi[b][0])+10; // get center of item location
            float y0 = al_fixtof(Efi[b][1])+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            float bdr = (float) Ei[b][17]; // prox radius
            float dif = dst-bdr; // difference
            if ((dif < 3 ) && (dif > -3))  // mouse is on prox circle
            {
               mouse_on_trk = 1;
               mouse_move = 1;
            }
         }
         if (type == 7) // podzilla extended position
         {
            int px=0, py=0;
            get_pod_extended_position(b, &px, &py);
            if ((hx>px+msn) && (hx<px+msp) && (hy>py+msn) && (hy<py+msp))
            {
               mouse_move = 1;
               mouse_on_podx = 1;
               mouse_on_podx_num = b;
            }
         }
         if ((type == 7) || (type == 9)) // podzilla and cloner trigger box
         {
            int x1 = Ei[b][11];
            int y1 = Ei[b][12];
            int x2 = Ei[b][11]+Ei[b][13]+20;
            int y2 = Ei[b][12]+Ei[b][14]+20;
            if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
            {
               mouse_on_tb_ul = 1;
               mouse_move = 1;
            }
            if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
            {
               mouse_on_tb_lr = 1;
               mouse_adj = 1;
            }
         }
         if (type == 9) // cloner source and destination boxes
         {
            int w = Ei[num][19];     // width
            int h = Ei[num][20];     // height
            int x1 = Ei[num][15];    // source box
            int y1 = Ei[num][16];
            int x2 = x1 + w - 1;
            int y2 = y1 + h - 1;
            int x3 = Ei[num][17];    // dest box
            int y3 = Ei[num][18];
            if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // source upper left corner (move)
            {
               mouse_on_csb_ul = 1;
               mouse_move = 1;
            }
            if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // source lower right corner (re-size)
            {
               mouse_on_csb_lr = 1;
               mouse_adj = 1;
            }
            if ((hx>x3-mst) && (hx<x3+mst) && (hy>y3-mst) && (hy<y3+mst)) // destination upper left corner (move)
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
      if ((item[b][0]) && (obj_filter[2][item[b][0]])) // iterate valid items that have the obj filter on
      {
         int ix = item[b][4];
         int iy = item[b][5];
         if ((hx>ix+msn) && (hx<ix+msp) && (hy>iy+msn) && (hy<iy+msp)) // set this item to current object
         {
            obt = 2;
            num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         if (item[b][0] == 10) // pop message
         {
            int x1 = item[num][10];
            int y1 = item[num][11];
            if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
            {
               mouse_on_msg_ul = 1;
               mouse_move = 1;
            }
         }
         if ((item[b][0] == 8) || (item[b][0] == 11)) // bomb or rocket
         {
            float x0 = (float) item[num][4]+10; // get center of item location
            float y0 = (float) item[num][5]+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            float bdr = (float) item[num][7]; // bomb damage radius
            float dif = dst-bdr; // difference
            if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
            {
               mouse_on_bmb = 1;
               mouse_move = 1;
            }
         }
         if (item[b][0] == 15) // sproingy
         {
            int x1 = item[num][4];
            int y1 = item[num][5];
            int y2 = y1 - al_fixtoi(get_sproingy_jump_height(num));

            if ((hx>x1+msn) && (hx<x1+msp) && (hy>y2+msn) && (hy<y2+msp))
            {
               mouse_on_sp = 1;
               mouse_move = 1;
            }
         }
         if ((item[b][0] == 4) || (item[b][0] == 9) || (item[b][0] == 16) || (item[b][0]== 17)) // key, trigger, manip, damage
         {
            int x1 = item[num][6];
            int y1 = item[num][7];
            int x2 = x1 + item[num][8];
            int y2 = y1 + item[num][9];

            if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
            {
               mouse_on_kbr_ul = 1;
               mouse_move = 1;
            }
            if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
            {
               mouse_on_kbr_lr = 1;
               mouse_adj = 1;
            }




         }
      }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on lift
   // -----------------------------------------------------------
   mouse_on_lift = 0;

   if (obj_filter[4][1])
   {

      if ((!key[MAP_LOCK_KEY]) && (!viewer_lock)) // no lock...check all lifts and steps
      {
         for (int x=0; x<num_lifts; x++)  // cycle lifts
            for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
               if ((lift_steps[x][y].type & 31) == 1) // look for move step
               {
                  int w =  lift_steps[x][y].w / 2;
                  int h =  lift_steps[x][y].h / 2;
                  int nx = lift_steps[x][y].x + w;
                  int ny = lift_steps[x][y].y + h;
                  if ((hx > nx - w)  && (hx < nx + w) && (hy > ny - h)  && (hy < ny + h)) // is mouse on this step ?
                  {
                     mouse_on_lift = 1;
                     obt = 4;
                     num = x;
                     lift = x;
                     step = y;
                     set_lift_to_step(lift, step);   // set current step in current lift
                  }
               }
      }

      if (((key[MAP_LOCK_KEY]) || (viewer_lock)) && (obt == 4)) // locked, but locked to current lift
      {
         int x = lift; // check only current lift
         for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
            if ((lift_steps[x][y].type & 31) == 1) // look for move step
            {
               int w =  lift_steps[x][y].w / 2;
               int h =  lift_steps[x][y].h / 2;
               int nx = lift_steps[x][y].x + w;
               int ny = lift_steps[x][y].y + h;
               if ((hx > nx - w)  && (hx < nx + w) && (hy > ny - h)  && (hy < ny + h)) // is mouse on this step ?
               {
                  mouse_on_lift = 1;
                  step = y;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
      }
   }
   if (mouse_on_lift)
   {
      mouse_on_obj = 1;
      if (mouse_b2) step_popup_menu(lift, step);

      // is mouse on lower right adjustable corner
      int x2 = lift_steps[lift][step].x + lift_steps[lift][step].w;
      int y2 = lift_steps[lift][step].y + lift_steps[lift][step].h;
      if ((hx > x2-8) && (hy > y2-8)) mouse_adj = 1;
      else mouse_move = 1;
   }

   // -----------------------------------------------------------
   // --  set mouse cursor
   // -----------------------------------------------------------
   if (mouse_move) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);


   if (mouse_b1)
   {
      while (mouse_b1)
      {
         if (mouse_on_obj)
         {
            //printf("mouse pressed on obj\n");
            if (obt == 2) // move item
            {
               // get offset of move
               int x_off = gx - item[num][4] / 20;
               int y_off = gy - item[num][5] / 20;

               item[num][4] = gx*20;
               item[num][5] = gy*20;
               itemf[num][0] = al_itofix(gx*20);
               itemf[num][1] = al_itofix(gy*20);

               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                   if ((item[num][0] == 4) || (item[num][0] == 9) || (item[num][0] == 16) || (item[num][0]== 17)) // key, trigger, manip, damage
                   {
                      item[num][6] += x_off*20;
                      item[num][7] += y_off*20;
                   }
                   if (item[num][0] == 10) // msg
                   {
                      item[num][10] += x_off*20;
                      item[num][11] += y_off*20;
                   }
               }
            }
            if (obt == 3) // move enemy
            {
               // get offset of move
               int x_off = gx - al_fixtoi(Efi[num][0]) / 20;
               int y_off = gy - al_fixtoi(Efi[num][1]) / 20;

               // set new position
               Efi[num][0] = al_itofix(gx*20);
               Efi[num][1] = al_itofix(gy*20);

               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  // move podzilla's trigger box too
                  if (Ei[num][0] == 7)
                  {
                     Ei[num][11] += x_off*20;
                     Ei[num][12] += y_off*20;
                  }
                  // move cloner's stuff too
                  if (Ei[num][0] == 9)
                  {
                     Ei[num][11] += x_off*20; // trigger box
                     Ei[num][12] += y_off*20;
                     Ei[num][15] += x_off*20; // source
                     Ei[num][16] += y_off*20;
                     Ei[num][17] += x_off*20; // dest
                     Ei[num][18] += y_off*20;
                  }
               }
            }
            if (obt == 4) // lift
            {
               if (mouse_move)
               {
                  lift_steps[lift][step].x = gx*20;
                  lift_steps[lift][step].y = gy*20;

                  set_lift_to_step(lift, step);   // set current step in current lift
               }
               if (mouse_adj)
               {
                  // don't allow lr to be less than ul
                  if (gx < lift_steps[lift][step].x/20+1) gx = lift_steps[lift][step].x/20+1;
                  if (gy < lift_steps[lift][step].y/20+1) gy = lift_steps[lift][step].y/20+1;

                  // set new position
                  lift_steps[lift][step].w = gx*20 - lift_steps[lift][step].x;
                  lift_steps[lift][step].h = gy*20 - lift_steps[lift][step].y;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
         }
         if (mouse_on_podx)
         {
            int num = mouse_on_podx_num;
            Efi[num][5] = al_itofix(gx * 20);
            Efi[num][6] = al_itofix(gy * 20);
            recalc_pod(num);
         }
         if (mouse_on_tb_ul) // move trigger box from ul
         {
            //printf("mouse pressed on tb_ul\n");
            Ei[num][11] = gx*20;
            Ei[num][12] = gy*20;
         }
         if (mouse_on_tb_lr)  // resize trigger box from lr
         {
            // prevent lr corner from being less than ul corner
            if (gx < Ei[num][11]/20) gx = Ei[num][11]/20;
            if (gy < Ei[num][12]/20) gy = Ei[num][12]/20;
            // set new postion
            Ei[num][13] = gx*20 - Ei[num][11];
            Ei[num][14] = gy*20 - Ei[num][12];
         }
         if (mouse_on_msg_ul) // move msg
         {
            item[num][10] = gx*20;
            item[num][11] = gy*20;
         }
         if (mouse_on_sp) // adjust sproingy jump height
         {
            item[num][7] -= mouse_dy/2;
            // bounds check
            if (item[num][7] < 40) item[num][7] = 40;
            if (item[num][7] > 200) item[num][7] = 200;
         }
         if (mouse_on_bmb) // adjust bomb blast radius
         {
            float x0 = (float) item[num][4]+10; // get center of item location
            float y0 = (float) item[num][5]+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            item[num][7] = (int) dist;
         }
         if (mouse_on_trk) // adjust trakbot bulet prox
         {
            float x0 = al_fixtof(Efi[num][0])+10; // get center of item location
            float y0 = al_fixtof(Efi[num][1])+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            Ei[num][17] = (int) dst;
         }
         // ranges for key, trigger, manip and damage
         if (mouse_on_kbr_ul) // move block range from ul
         {
            // set new position
            item[num][6] = gx*20;
            item[num][7] = gy*20;
         }
         if (mouse_on_kbr_lr) // adjust block range from lr
         {
            // don't allow lr to be less than ul
            if (gx < item[num][6]/20) gx = item[num][6]/20;
            if (gy < item[num][7]/20) gy = item[num][7]/20;

            // set new position
            item[num][8] = gx*20 - item[num][6];
            item[num][9] = gy*20 - item[num][7];
         }
         if (mouse_on_csb_ul) // move cloner source box from ul
         {
            Ei[num][15] = gx*20; // set new postion
            Ei[num][16] = gy*20;
         } // end of mouse csb_ul
         if (mouse_on_csb_lr) // resize box from lr
         {
            // get ul corner
            int x1 = Ei[num][15]/20;
            int y1 = Ei[num][16]/20;

            // prevent lr corner from being less than ul corner
            if (gx < x1+1) gx = x1+1;
            if (gy < y1+1) gy = y1+1;

            // set new sizes
            Ei[num][19] = (gx-x1)*20;
            Ei[num][20] = (gy-y1)*20;
         } // end of mouse csb_lr

         if (mouse_on_cdb_ul) // cloner destination ul
         {
            Ei[num][17] = gx*20; // set new postion
            Ei[num][18] = gy*20;
         }
         ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
         ovw_redraw_background(obt, num, type, 0, 0);
      } // end of while mouse pressed
   } // end of if mouse pressed
}

void object_viewerw(int obt, int num)
{
   init_level_background();
   bts = 16; // button y size
   int legend_line = -1;
   int quit = 0;
   int type=0, obj_x=0, obj_y=0, lift=0, step=0;

   while (!quit)
   {
      if (obt == 2)
      {
         type = item[num][0];
         obj_x = item[num][4]+10;
         obj_y = item[num][5]+10;
      }
      if (obt == 3)
      {
         type = Ei[num][0];
         obj_x = al_fixtoi(Efi[num][0])+10;
         obj_y = al_fixtoi(Efi[num][1])+10;
      }
      if (obt == 4)
      {
         lift = num;
         step = lifts[num].current_step;
         set_lift_to_step(lift, step);   // set current step in current lift
         lifts[num].current_step = step;
      }

      // ------------------------------------------------------------------------
      // ----  redraw the level background and the object viewer window ---------
      // ------------------------------------------------------------------------
      int mb = ovw_redraw_background(obt, num, type, legend_line, 1);

      // ------------------------------------------------------------------------
      // ----  if mouse on legend lines, show highlights
      // ------------------------------------------------------------------------
      process_flash_color();
      legend_line = 0;
      int y1_legend = ov_window_y2 - 34 + (5-num_legend_lines)*8; // legend pos
      int y2_legend = y1_legend + (num_legend_lines-1)*8;
      if ((mouse_x > ov_window_x1) && (mouse_x < ov_window_x2) && (mouse_y > y1_legend) && (mouse_y < y2_legend)) // is mouse on legend
      {
         legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?
         ovw_title(obt, num, legend_line);
      }


      draw_filter_toggles(20, 100, 300, 2);


      ovw_proc_move_window(obt, num, type);

      // if mouse is on button window
      if ((mouse_x > ov_window_x1) && (mouse_x < ov_window_x2) && (mouse_y > ov_window_y1) && (mouse_y < ov_window_y2))
      {
         // al_draw_rectangle(ov_window_x, ov_window_y, ov_window_x2, ov_window_y2, palette_color[14], 1);
      }
      else
      {
         ovw_process_scrolledge();
         ovw_map_move(obt, num);
      }






      if (key[ALLEGRO_KEY_B])
      {
         while (key[ALLEGRO_KEY_B]) proc_controllers();
         if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
         {
            printf("save bookmark\n");
            bookmark_level = last_level_loaded;
            bookmark_obj = obt;
            bookmark_num = num;
            save_config();
         }
         else
         {
            printf("load bookmark\n");
            if (bookmark_level == last_level_loaded)
            {
               if ((bookmark_obj == 2) && (item[bookmark_num]))
               {
                  obt = bookmark_obj;
                  num = bookmark_num;
               }
               if ((bookmark_obj == 3) && (Ei[bookmark_num]))
               {
                  obt = bookmark_obj;
                  num = bookmark_num;
               }
               if ((bookmark_obj == 4) && (bookmark_num < num_lifts))
               {
                  obt = bookmark_obj;
                  num = bookmark_num;
               }
            }
         }
      }
















      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;  // wait for release
      }
      while (key[ALLEGRO_KEY_DELETE])
      {
         mb = 20;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_RIGHT])
      {
         mb = 21;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_LEFT])
      {
         mb = 22;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_UP])
      {
         mb = 26;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_DOWN])
      {
         mb = 27;
         proc_controllers();
      }

      switch(mb)
      {
         case 18: // move
         {
            if (obt==2)
            {
               sprintf(msg,"%s", item_name[type]);
               getxy(msg,obt, type, num);
            }
            if (obt==3)
            {
               sprintf(msg,"%s", enemy_name[type]);
               getxy(msg,obt, type, num);
            }
            if (obt==4)
            {
               while (!key[ALLEGRO_KEY_ESCAPE])
               {
                  for (int t=0; t<2; t++) move_lifts(1);  // move lifts for 2 frames
                  al_flip_display();
                  proc_scale_factor_change();
                  proc_controllers();
                  proc_frame_delay();
                  get_new_background(0);
                  draw_lifts();
                  draw_items();
                  draw_enemies();
                  ovw_draw_overlays(obt, num, legend_line);
                  get_new_screen_buffer(3, obj_x, obj_y);
                  ovw_draw_buttons(obt, num, type);
                  ovw_title(obt, num, 0);
               }
               while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release
               lift_setup(); // reset all lifts to step 0
            }
         }
         break;
         case 19:
            num = create_obj(obt, type, num);
         break;
         case 20: // delete
            if (obt== 2)
            {
               erase_item(num);
               sort_item();
               if (num >= item_first_num[type]+item_num_of_type[type]) num--;
               if (item_num_of_type[type] < 1) quit = 1;
            }
            if (obt == 3)
            {
               Ei[num][0] = 0;
               sort_enemy();
               if (num >= e_first_num[type]+e_num_of_type[type]) num--;
               if (e_num_of_type[type] < 1) quit = 1;
            }
            if (obt == 4)
            {
               erase_lift(num);
               if (--num < 0) num = 0;      // set to prev lift or zero
               if (num_lifts < 1) quit = 1; // if erased last lift; exit lift viewer
            }
         break;
         case 21: // next
            if ((obt==2) && (++num >= item_first_num[type] + item_num_of_type[type])) num--;
            if ((obt==3) && (++num >= e_first_num[type] + e_num_of_type[type])) num--;
            if ((obt==4) && (++num > num_lifts-1)) num--;
         break;
         case 22: // previous
            if ((obt==3) && (--num < e_first_num[type])) num++;
            if ((obt==2) && (--num < item_first_num[type])) num++;
            if ((obt==4) && (--num < 0)) num++;
         break;
         case 24:  // viewer help
            help("Viewer Basics");
         break;
         case 25:  // object help
            if (obt==3)
            {
               if (type == 3)  help("Archwagon Viewer");
               if (type == 4)  help("Bouncer Viewer");
               if (type == 6)  help("Cannon Viewer");
               if (type == 7)  help("Podzilla Viewer");
               if (type == 8)  help("Trakbot Viewer");
               if (type == 9)  help("Cloner Viewer");
               if (type == 10) help("Field Viewer");
               if (type == 11) help("Block Walker Viewer");
               if (type == 12) help("Flapper Viewer");
            }
            if (obt==2)
            {
               if (type == 1)  help("Door Viewer");
               if (type == 2)  help("Health Bonus Viewer");
               if (type == 3)  help("Exit Viewer");
               if (type == 4)  help("Key Viewer");
               if (type == 5)  help("Start Viewer");
               if (type == 6)  help("Free Man Viewer");
               if (type == 7)  help("Mine Viewer");
               if (type == 8)  help("Bomb Viewer");
               if (type == 10) help("Message Viewer");
               if (type == 11) help("Rocket Viewer");
               if (type == 12) help("Warp Viewer");
               if (type == 14) help("Switch Viewer");
               if (type == 15) help("Sproingy Viewer");
            }
            if (obt==4)
            {
               help("Lift Viewer");
            }
         break;
         case 26: // up
         {
            if (obt == 2)  // prev item type
            {
               do
               {
                  type--;
                  if (type < 1) type = 20;
               } while (item_num_of_type[type] == 0);
               num = item_first_num[type];
            }
            if (obt == 3) // prev enemy type
            {
               while (e_num_of_type[--type] == 0)
                  if (type < 3) type = 20;
               num = e_first_num[type];
            }
            if (obt == 4) // prev lift step
            {
               if (--step < 0) step = 0;
            }
         }
         break;
         case 27: // down
         {
            if (obt == 2) // next obj type
            {
               do
               {
                  type++;
                  if (type > 20) type = 1;
               } while (item_num_of_type[type] == 0);
               num = item_first_num[type];
            }
            if (obt == 3) // next obj type
            {
               while (e_num_of_type[++type] == 0)
                  if (type > 20) type = 1;
               num = e_first_num[type];
            }
            if (obt == 4) // next lift step
            {
               if (++step > (lifts[lift].num_steps-1)) step = (lifts[lift].num_steps-1);
            }
         }
         break;
      } // end of switch (mb)
   } // end of while (!quit)
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
}


