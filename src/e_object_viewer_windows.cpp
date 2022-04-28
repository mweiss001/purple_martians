// e_object_viewer_windows.cpp

#include "pm.h"

void draw_object_overlays(int obj_type, int num, int legend_highlight, int highlight_color)
{
   al_set_target_bitmap(level_buffer);
   int obj_x=0, obj_y=0, sub_type=0;
   if (obj_type == 2)
   {
      sub_type = item[num][0];
      obj_x = item[num][4]+10;
      obj_y = item[num][5]+10;
   }
   if (obj_type == 3)
   {
      sub_type = Ei[num][0];
      obj_x = al_fixtoi(Efi[num][0])+10;
      obj_y = al_fixtoi(Efi[num][1])+10;
   }

   // draw object location crosshairs
   int color = 13;
   if (legend_highlight == 1) color = highlight_color;
   crosshairs_full(obj_x, obj_y, color);

   if (obj_type == 3)  // enemies
   {
      switch (sub_type)
      {
         case 3: // archwagon
         {
            // yellow bullet prox
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            int bs = Ei[num][17];
            al_draw_rectangle(obj_x-bs, obj_y-bs, obj_x+bs, obj_y+bs, palette_color[color], 1);
         }
         break;
         case 7: // podzilla
         {
            // trigger box
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            int tx1 = Ei[num][11]*20;
            int ty1 = Ei[num][12]*20;
            int tx2 = Ei[num][13]*20 + 20;
            int ty2 = Ei[num][14]*20 + 20;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color], 1);

            // extended position
            int color1 = 10;
            if (legend_highlight == 2) color1 = highlight_color;
            al_fixed ex = Efi[num][0];
            al_fixed ey = Efi[num][1];
            for (int j=0; j<Ei[num][7]; j++)
            {
               ex += Efi[num][2];
               ey += Efi[num][3];
            }
            crosshairs_full(al_fixtoi(ex), al_fixtoi(ey), color1);
         }
         break;
         case 8: // trakbot
         {
            // draw yellow bullet prox circle
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, Ei[num][17], palette_color[color], 1);
         }
         break;
         case 9: // cloner
         {
            int color2 = 11;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = highlight_color;

            int color4 = 14;
            if (legend_highlight == 4) color4 = highlight_color;

            int cw = Ei[num][19]*20;     // width
            int ch = Ei[num][20]*20;     // height

            int cx1 = Ei[num][15]*20;    // source
            int cy1 = Ei[num][16]*20;
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;

            //rectangle_with_diagonal_lines(cx1, cy1, cx2, cy2, db/3, color2, color2+64);
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            int cx3 = Ei[num][17]*20;    // destination
            int cy3 = Ei[num][18]*20;
            int cx4 = cx3 + cw;
            int cy4 = cy3 + ch;
            //rectangle_with_diagonal_lines(cx3, cy3, cx4, cy4, db/3, color3, color3+64);
            al_draw_rectangle(cx3, cy3, cx4, cy4, palette_color[color3], 1);

            // draw trigger box
            int tx1 = Ei[num][11]*20;
            int ty1 = Ei[num][12]*20;
            int tx2 = Ei[num][13]*20 + 20;
            int ty2 = Ei[num][14]*20 + 20;
            //rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, db/3, color4, color4+64);
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color4], 1);

         }
         break;
         case 10: // field
         {

            int color2 = 10;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 14;
            if (legend_highlight == 3) color3 = highlight_color;

            int cw = Ei[num][17];     // width
            int ch = Ei[num][18];     // height
            int cx1 = Ei[num][15];    // source
            int cy1 = Ei[num][16];
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;
            //rectangle_with_diagonal_lines(cx1, cy1, cx2, cy2, db/3, color2, color2+64);
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            // draw trigger box
            int tw = Ei[num][13];     // width
            int th = Ei[num][14];     // height
            int tx1 = Ei[num][11];
            int ty1 = Ei[num][12];
            int tx2 = tx1 + tw;
            int ty2 = ty1 + th;
            //rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, db/3, color4, color4+64);
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color3], 1);
         }
         break;
         case 12: // flapper
         {
            int color2 = 14;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = highlight_color;

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
   if (obj_type == 2)  // items
   {
      switch (sub_type)
      {
         case 1: // door
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                     int x2 = item[i][4]*20+10;
                     int y2 = item[i][5]*20+10;
                     crosshairs_full(x2, y2, color);
                     al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
                  }
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]-wx*20+10;
               int y2 = item[di][5]-wy*20+10;
               crosshairs_full(x2, y2, color);
               al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
            }
         }
         break;
         case 4: // key
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
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
         }
         break;
         case 8:
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 9: // trigger
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;

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
         }
         break;
         case 11:
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 15: // sproingy
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            int y = al_fixtoi(get_sproingy_jump_height(num));
            crosshairs_full(obj_x, obj_y-y, color);
         }
         break;
         case 16: // block manip
         {
            int color = 12;
            if (legend_highlight == 2) color = highlight_color;
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

//            find_and_show_event_links(num);

         }
         break;
         case 17: // block damage
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
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

  //          find_and_show_event_links(num);
         }
         break;

      } // end of switch case
   }
}



void title_objw(int obj_type, int num, int legend_highlight, int highlight_color)
{
   int sub_type=0;
   if (obj_type == 2) sub_type = item[num][0];
   if (obj_type == 3) sub_type = Ei[num][0];

   int ov_x2 = ov_window_x + ov_window_w;
   int ov_xc = ov_window_x + ov_window_w/2;

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


   if (legend_highlight == 1) legend_color[1] = highlight_color;

   if (!legend_highlight)
   {
  //    al_draw_filled_rectangle(ov_x1, ov_y1, ov_x2, ov_y2, palette_color[0]);      // erase side panel

      // title bar
      msg[0] = 0;
      if (obj_type == 2) sprintf(msg, "Item Viewer [%d]", num);
      if (obj_type == 3) sprintf(msg, "Enemy Viewer [%d]", num);
      if (obj_type == 4) sprintf(msg, "Lift Viewer");
      for (int x=0; x<15; x++)
         al_draw_line(ov_window_x, ov_window_y+x, ov_x2, ov_window_y+x, palette_color[13+(x*16)], 1);
      al_draw_text(font, palette_color[15], ov_xc, ov_window_y+2, ALLEGRO_ALIGN_CENTER,  msg);
   }



   if (obj_type == 3)  // enemies
   {
      if (!legend_highlight)
      {
         int yt = ov_window_y+14;
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_enemy_shape(num, ov_xc-92, yt+1);
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
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 4: sprintf(lmsg[1],"Bouncer Location"); break;
         case 6: sprintf(lmsg[1],"Cannon Location"); break;
         case 7: // podzilla
         {
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Trigger Box");
            sprintf(lmsg[3],"Extended Postion");
            num_legend_lines = 4;

            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
         }
         break;
         case 8: // trakbot
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
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
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = highlight_color;

            legend_color[4] = 14;
            if (legend_highlight == 4) legend_color[4] = highlight_color;
         }
         break;
         case 10: // field
         {
            sprintf(lmsg[1],"Field Location");
            sprintf(lmsg[2],"Field Area");
            sprintf(lmsg[3],"Trigger Box");
            num_legend_lines = 4;

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 14;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
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
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
         }
         break;
      }
   }
   if (obj_type == 2)  // items
   {
      if (!legend_highlight)
      {
         int yt = ov_window_y+14;
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_item_shape(num, ov_xc-94, yt+1);
         sprintf(msg,"%s %d of %d", item_name[sub_type], 1+num - item_first_num[sub_type],item_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], ov_xc-69, yt+8, 0, msg);
      }
      switch (sub_type)
      {
         case 1: // door
         {
            num_legend_lines = 3;
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

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
            if (legend_highlight == 2) legend_color[2] = highlight_color;
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
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 9: // trigger
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
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
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 12: sprintf(lmsg[1],"Warp Location"); break;
         case 14: sprintf(lmsg[1],"Switch Location"); break;
         case 15: sprintf(lmsg[1],"Sproingy Location");
         {
            num_legend_lines = 3;
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 16: // block manip
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Block Manip Item Location");
            sprintf(lmsg[2],"Manip Field");
            legend_color[2] = 12;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 17: // block damage
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;

      } // end of switch case

   }  // end of items



   ov_window_h += num_legend_lines*8 + 8;

   int ov_y2 = ov_window_y + ov_window_h;

   for (int x=1; x<num_legend_lines; x++)// draw text lines
      al_draw_text(font, palette_color[legend_color[x]], ov_xc, ov_y2-26+(3-num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);

   if (!legend_highlight)
   {
      al_draw_text(font, palette_color[legend_color[0]], ov_xc, ov_y2-36+ (4-num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(ov_xc-100, ov_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_y2-1, palette_color[13], 1); // big frame
      al_draw_rectangle(ov_xc-100, ov_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_y2-28+ (4-num_legend_lines)*8, palette_color[13], 1); // top frame
      al_draw_rectangle(ov_window_x, ov_window_y, ov_x2, ov_y2, palette_color[13], 1);  // outline entire window

   }
}







int obv_draw_buttons(int num, int type, int obt)
{
   // button x position
   int xa = ov_window_x+1;
   int xb = ov_window_x+ov_window_w-1;
   int ty = ov_window_y+38;


   // split into half
   int x12 = xa + 1 * (xb-xa) / 2; // 1/2

   // split into thirds
   int x13 = xa + 1 * (xb-xa) / 3; // 1/3
   int x23 = xa + 2 * (xb-xa) / 3; // 2/3

   // split into fifths
   //int x15 = xa + 1 * (xb-xa) / 5; // 1/5
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

   //mdw_slider(xb-40,  ty-33,    xb,    ty-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height
   //mdw_slider(xa,  ty-35,    xb,    ty-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height

   int lc = 6; // lock_color;
   if (Viewer_lock) lc = 7;

   int a=0;
   int mb = 0;   // button selection
   if (mdw_button(xa,  ty+a*bts, x27-1, ty+(a+1)*bts-2, 23, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 22; // prev
       mdw_button(x27, ty+a*bts, x57-1, ty+(a+1)*bts-2, 56, num, type, obt, 0, lc, 15, 0, 1,0,0,0);          // lock
   if (mdw_button(x57, ty+a*bts, xb,    ty+(a+1)*bts-2, 22, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 21; // next
   a++;
   if (mdw_button(xa,  ty+a*bts, x13-1, ty+(a+1)*bts-2, 19, num, type, obt, 0, 13, 15, 0, 1,0,0,0)) mb = 18; // move
   if (mdw_button(x13, ty+a*bts, x23-1, ty+(a+1)*bts-2, 20, num, type, obt, 0, 14, 15, 0, 1,0,0,0)) mb = 19; // create
   if (mdw_button(x23, ty+a*bts, xb,    ty+(a+1)*bts-2, 21, num, type, obt, 0, 10, 15, 0, 1,0,0,0)) mb = 20; // delete
   a++;
   if (mdw_button(xa,  ty+a*bts, x12-1, ty+(a+1)*bts-2, 25, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 24; // viewer help
   if (mdw_button(x12, ty+a*bts, xb,    ty+(a+1)*bts-2, 57, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 25; // specific object help
   a+=2;

   a = obj_buttons(xa, xb, ty, a, bts, obt, num);

   ov_window_h = 38+a*bts;


   return mb;

}













void object_viewerw(int obt, int num)
{
   init_level_background();

   bts = 16; // button y size
   int legend_line = -1;

   int highlight_counter=0;



   int quit = 0;
   while (!quit)
   {
      int ov_window_x2 = ov_window_x + ov_window_w;
      int ov_window_y2 = ov_window_y + ov_window_h;
      //int ty = ov_window_y+38;

      int type=0, obj_x=0, obj_y=0;
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

      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();
      draw_object_overlays(obt, num, legend_line, highlight_counter);
      get_new_screen_buffer(1, obj_x, obj_y);

      draw_fps_display(8);


      int mb = obv_draw_buttons(num, type, obt);



      // draw button title, frame and legend lines
      title_objw(obt, num, 0, 15);

      // is mouse on title bar
      if ((mouse_x > ov_window_x) && (mouse_x < ov_window_x2) && (mouse_y > ov_window_y) && (mouse_y < ov_window_y+14))
      {
         // draw rectangle around title bar to show it can be dragged
         // al_draw_rectangle(ov_window_x, ov_window_y, ov_window_x2, ov_window_y+14, palette_color[14], 1);

         // color text in title bar to show it can be moved
//         msg[0] = 0;
//         if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
//         if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
//         int ov_xc = (ov_window_x + ov_window_x2) / 2;
//         al_draw_text(font, palette_color[10], ov_xc, ov_window_y+2, ALLEGRO_ALIGN_CENTER,  msg);



         if (mouse_b1)
         {
            int mxo = mouse_x - ov_window_x; // get offset from mouse position to window x, y
            int myo = mouse_y - ov_window_y;

            while (mouse_b1)
            {
               ov_window_x = mouse_x - mxo;
               ov_window_y = mouse_y - myo;
               ov_window_x2 = ov_window_x + ov_window_w;
               ov_window_y2 = ov_window_y + ov_window_h;

               al_flip_display();
               proc_scale_factor_change();

               proc_controllers();
               proc_frame_delay();
               get_new_background(0);
               draw_lifts();
               draw_items();
               draw_enemies();
               draw_object_overlays(obt, num, legend_line, highlight_counter);
               get_new_screen_buffer(1, obj_x, obj_y);

               title_objw(obt, num, 0, 15);
               obv_draw_buttons(num, type, obt);
            } // mouse b1 held
         } // mouse b1 pressed
      } // mouse on title bar


      // is mouse on legend ?
      legend_line = 0;
      int y1_legend = ov_window_y2 - 34 + (5-num_legend_lines)*8; // legend pos
      int y2_legend = y1_legend + (num_legend_lines-1)*8;
      if ((mouse_x > ov_window_x) && (mouse_x < ov_window_x2) && (mouse_y > y1_legend) && (mouse_y < y2_legend))
      {
         legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?
         if (++highlight_counter > 16) highlight_counter = 1;
         if ((highlight_counter > 0)  && (highlight_counter < 5))  title_objw(obt, num, legend_line, 10);
         if ((highlight_counter > 4)  && (highlight_counter < 9))  title_objw(obt, num, legend_line, 14);
         if ((highlight_counter > 8)  && (highlight_counter < 13)) title_objw(obt, num, legend_line, 15);
         if ((highlight_counter > 12) && (highlight_counter < 17)) title_objw(obt, num, legend_line, 11);
      }
      else highlight_counter = 0; // mouse not on legend


      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;  // wait for release
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
      while (key[ALLEGRO_KEY_DELETE])
      {
         mb = 20;
         proc_controllers();
      }

      switch(mb)
      {
         case 18: // move
            if (getxy("Set New Location",obt, type, num) == 1)
            {
               if (obt==3) // enemy
               {
                  Efi[num][0] = al_itofix(get100_x*20);
                  Efi[num][1] = al_itofix(get100_y*20);
               }
               if (obt==2) // item
               {
                  item[num][4] = get100_x*20;
                  item[num][5] = get100_y*20;
                  itemf[num][0] = al_itofix(item[num][4]);
                  itemf[num][1] = al_itofix(item[num][5]);
               }
            }
         break;
         case 19:
            num = create_obj(obt, type, num);
         break;
         case 20: // delete
            if (obt ==3)
            {
               Ei[num][0] = 0;
               sort_enemy();
               if (num >= e_first_num[type]+e_num_of_type[type]) num--;
               if (e_num_of_type[type] < 1) quit = 1;
            }
            if (obt==2)
            {
               erase_item(num);
               sort_item();
               if (num >= item_first_num[type]+item_num_of_type[type]) num--;
               if (item_num_of_type[type] < 1) quit = 1;
            }
         break;
         case 21: // next
            if ((obt==3) && (++num >= e_first_num[type] + e_num_of_type[type])) num--;
            if ((obt==2) && (++num >= item_first_num[type] + item_num_of_type[type])) num--;
         break;
         case 22: // previous
            if ((obt==3) && (--num < e_first_num[type])) num++;
            if ((obt==2) && (--num < item_first_num[type])) num++;
         break;
         case 23: // copy to draw item
            draw_item_num = num;
            draw_item_type = obt;
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
         break;
      } // end of switch (mb)
   } // end of while (!quit)
}
