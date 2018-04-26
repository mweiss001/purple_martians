// e_item.cpp (20100220 cleanup)
#include "pm.h"


al_fixed get_sproingy_jump_height(int item);


void draw_edit_item_shape(int x, int y, int obt, int sub_type, int num)
{
   if (obt == 2) draw_item_shape(num, x, y);
   if (obt == 3) draw_enemy_shape(num, x, y);
}

void crosshairs(int mx, int my, int x, int y, int color) // function to draw rectangle and crosshairs
{
   //al_draw_filled_rectangle(mx+(x*db), my+(y*db), mx+(x*db)+db-1, my+(y*db)+db-1, palette_color[color]);
   al_draw_filled_rectangle(mx+(x*db)-1, my+(y*db)-1, mx+(x*db)+db, my+(y*db)+db, palette_color[color]);
   al_draw_line(mx+1, my+(y*db)+db/2, mx+(100*db)-2, my+(y*db)+db/2, palette_color[color], 1);
   al_draw_line(mx+(x*db)+db/2, my+1, mx+(x*db)+db/2, my+(100*db)-2, palette_color[color], 1);
}

void crosshairs_nodb(int mx, int my, int x, int y, int db, int color) // funtion to draw rectangle and crosshairs
{
   al_draw_filled_rectangle(mx+(x), my+(y), mx+(x)+db-1, my+(y)+db-1, palette_color[color]);
   al_draw_line(mx, my+(y)+db/2, mx+(100*db), my+(y)+db/2, palette_color[color], 1);
   al_draw_line(mx+(x)+db/2, my, mx+(x)+db/2, my+(100*db), palette_color[color], 1);
}

void title(char *txt, int y, int tc, int fc)
{
   for (int x=0; x<15; x++)
      al_draw_line(db*100, y+x, SCREEN_W-2, y+x, palette_color[fc+(x*16)], 1);
   al_draw_text(font, palette_color[tc], txc, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}


void test_items(void)
{
   for (int c=0; c<500; c++)
      if (item[c][0])
      {
         // first check for valid type
         int good = 0;
         if (item[c][0] == 1) good = 1;
         if (item[c][0] == 2) good = 1;
         if (item[c][0] == 3) good = 1;
         if (item[c][0] == 4) good = 1;
         if (item[c][0] == 5) good = 1;
         if (item[c][0] == 6) good = 1;
         if (item[c][0] == 7) good = 1;
         if (item[c][0] == 8) good = 1;
         if (item[c][0] == 10) good = 1;
         if (item[c][0] == 11) good = 1;
         if (item[c][0] == 12) good = 1;
         if (item[c][0] == 14) good = 1;
         if (item[c][0] == 15) good = 1;
         if (!good)  printf("Item:%d - bad type:%d\n", c, item[c][0]);
         // if (!good)  item[c][0] = 0; //erase bad type
         if (good)
         {
            if ((item[c][1] < 169) || (item[c][1] > 1084))
               printf("Item:%d - bad shape:%d\n", c, item[c][1]);
            int xpos = item[c][4];
            if ((xpos < 20) || (xpos > 1960))
                printf("Item:%d - bad xpos%d\n", c, xpos);
            int ypos = item[c][5];
            if ((ypos < 20) || (ypos > 1960))
                printf("Item:%d - bad ypos%d\n", c, ypos);
         }
      }
}


void show_all_items(void)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   test_items();
   int text_pos = 0;

   int num_items = item_sort();
   text_pos = item_data(10, text_pos);

   for (int i=0; i<num_items; i++)
   {
      get_item_draw_shape(i);
      al_set_target_backbuffer(display);
      al_draw_bitmap(dtemp, 0, text_pos, 0);
      sprintf(msg,"item:[%2d] ",i );

      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"%d[%d] ", j, item[i][j]);
         if (j == 0) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 1) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 3) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 4) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 5) sprintf(msg2,"[%-4d] ", item[i][j]);
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[13], 20, text_pos+6, 0, msg);
      text_pos +=20;
      if (text_pos > SCREEN_H - 10)
      {
         al_flip_display();
         tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}


int item_sort(void)
{
   // to not break linked items
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1) //door
         item[c][15] = c; // tag this door with its original item number


   extern int item_first_num[20];
   int inum, c, d, quit, temp, swap;
   quit=0;
   while (!quit) // sort item list
   {
      quit=1; // quit if no swap
      for (c=0; c < 499; c++)
      {
         itemf[c][0] = al_itofix(item[c][4]);
         itemf[c][1] = al_itofix(item[c][5]);

         if (item[c][0] < item[c+1][0]) // sort by first value 'type'
            swap = 1;
//         else if (item[c][0] == item[c+1][0]) // if type is the same
//            if (item[c][1] < item[c+1][1]) // sort by 2nd value 'ans'
//               swap =1;

         if (swap)
         {
            quit=0;      // as long as a swap has been made
            swap = 0;
            if ((item[c][0] == 10) && (item[c+1][0] == 10)) // both messages
            {
               strcpy(msg, pmsg[c]);
               free(pmsg[c]);
               pmsg[c] = (char*) malloc (strlen(pmsg[c+1])+1);
               strcpy(pmsg[c], pmsg[c+1]);
               free(pmsg[c+1]);
               pmsg[c+1] = (char*) malloc (strlen(msg)+1);
               strcpy(pmsg[c+1], msg);
            }
            if ((item[c][0] == 10) && (item[c+1][0] != 10)) // first only
            {
               pmsg[c+1] = (char*) malloc (strlen(pmsg[c])+1);
               strcpy(pmsg[c+1], pmsg[c]);
               free(pmsg[c]);
            }
            if ((item[c][0] != 10) && (item[c+1][0] == 10)) // second only
            {
               pmsg[c] = (char*) malloc (strlen(pmsg[c+1])+1);
               strcpy(pmsg[c], pmsg[c+1]);
               free(pmsg[c+1]);
            }
            for (d=0; d<16; d++)
            {
               temp = item[c][d];
               item[c][d] = item[c+1][d];
               item[c+1][d] = temp;
            }
            for (d=0; d<4; d++)
            {
               temp = itemf[c][d];
               itemf[c][d] = itemf[c+1][d];
               itemf[c+1][d] = temp;
            }

         }
      }
   }

   // get data about first 20 item types
   // and make sub lists of item types using these variables
   inum = 0;    // zero the counters
   for (c=0; c<20; c++)
   {
      item_num_of_type[c] = 0;
      item_first_num[c] = 0;
   }

   for (c=0; c<500; c++) // get the type counts
   {
      item_num_of_type[item[c][0]]++; // inc number of this type
      if (item[c][0]) inum++;
   }
   for (c=0; c<20; c++)             // get first nums
      if (item_num_of_type[c] > 0)  // are there any of this type?
         for (d=0; d<500; d++)
            if (item[d][0] == c)
            {
               item_first_num[c] = d;
               d=500;   // exit loop
            }


   // to not break linked items
   for (int c=0; c < 500; c++)
      if ((item[c][0] == 1) && (item[c][8] == 1))           // if door && exit mode == link
      {
         int link = item[c][9];                             // look for doors with that link in 15
         for (int d=0; d < 500; d++)
            if ((item[d][0] == 1) && (item[d][15] == link)) // found a door with link in 15
               item[c][9] = d;                              // update to new link number
      }

   // erase what we put in 15
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1)     // door
         item[c][15] = 0;

   return inum;
}


void title_obj(int obj_type, int sub_type, int num, int legend_highlight, int highlight_color)
{
   extern int e_num_of_type[50];      // sort results
   extern int e_first_num[50];
   extern char eitype_desc[50][32][40];

   extern int item_first_num[20];
   extern char item_desc[20][40];

   extern int tw;   // button width

   char lmsg[5][80];
   int legend_color[5];

   int x, x1=0, y1=0;

   if (obj_type == 2)  // item x, y
   {
      x1 = item[num][4]/20;
      y1 = item[num][5]/20;
   }
   if (obj_type == 3) // enemy x, y
   {
      x1 = al_fixtoi(Efi[num][0]/20);
      y1 = al_fixtoi(Efi[num][1]/20);
   }

   // default legend line colors
   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused

   // default number of legend lines
   Num_legend_lines = 2;

   // clear legend text
   for (x=0; x<5; x++) sprintf(lmsg[x],"%s","");

   if (!legend_highlight)
   {
      show_big();

      if (obj_type == 4)
      {
         al_draw_rectangle(0, 0,  SCREEN_W-1, SCREEN_H-1, palette_color[13], 1);         // outline screen
         al_draw_rectangle(0, 0, 100*db-1, 100*db-1, palette_color[13], 1);              // outline map
         al_draw_rectangle(100*db-1, 0,  SCREEN_W-1, SCREEN_H-1, palette_color[13], 1);  // outline side panel
      }
      else
      {
         al_draw_rectangle(0, 0, 100*db-1, 100*db-1, palette_color[13], 1);       // outline map
         al_draw_rectangle(0, 0, SCREEN_W-1, 100*db-1, palette_color[13], 1);    // outline screen (with blank section on bottom)
      }
      msg[0] = 0;
      if (obj_type == 2) sprintf(msg, "Item Viewer [%d]", num);
      if (obj_type == 3) sprintf(msg, "Enemy Viewer [%d]", num);
      if (obj_type == 4) sprintf(msg, "Lift Viewer");
      title(msg,  0, 15, 13);
   }

   // draw object location crosshairs
   if (obj_type != 4) // not lift
   {
      if (legend_highlight == 1)
      {
         crosshairs(0, 0, x1, y1, highlight_color);
         legend_color[1] = highlight_color;
      }
      else crosshairs(0, 0, x1, y1, 13);
   }

   // show the obj num and num of subtype
   if (obj_type == 4)  // lifts
   {
      // draw small lift
      draw_lift_mp(sub_type);
      al_set_target_backbuffer(display);
      al_draw_bitmap(mp, txc - 86, 24, 0);

      int lift = sub_type;
      al_draw_rectangle(txc-90, 20, txc+90, 43, palette_color[15], 1);
      al_draw_textf(font, palette_color[13], txc, 29, ALLEGRO_ALIGN_CENTER, "Lift %d of %d",lift+1, num_lifts);

   }
   if (obj_type == 3)  // enemies
   {
      al_draw_rectangle(txc-tw, 20, txc+tw, 43, palette_color[15], 1);
      draw_enemy_shape(num, txc-tw+2, 22);
      sprintf(msg,"%s %d of %d", (const char *)eitype_desc[sub_type],1+num - e_first_num[sub_type],e_num_of_type[sub_type]);
      al_draw_text(font, palette_color[13], txc-tw+28, 29, 0, msg);
      al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
      switch (sub_type)
      {
         case 3: // archwagon
         {
            Num_legend_lines = 3;
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            int color = 14;
            legend_color[2] = 14;
            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }
            // draw yellow bullet prox
            int bs = Ei[num][17]/20*db;
            al_draw_rectangle((x1*db)-bs, (y1*db)-bs, (x1*db)+db-1+bs, (y1*db)+db-1+bs, palette_color[color], 1);
         }
         break;
         case 4: sprintf(lmsg[1],"Bouncer Location"); break;
         case 6: sprintf(lmsg[1],"Cannon Location"); break;
         case 7: // podzilla
         {
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Trigger Box");
            sprintf(lmsg[3],"Extended Postion");
            Num_legend_lines = 4;

            int color2 = 14;
            legend_color[2] = 14;
            if (legend_highlight == 2)
            {
               color2 = highlight_color;
               legend_color[2] = highlight_color;
            }

            int color3 = 10;
            legend_color[3] = 10;
            if (legend_highlight == 3)
            {
               color3 = highlight_color;
               legend_color[3] = highlight_color;
            }

            al_fixed ex = Efi[num][0];
            al_fixed ey = Efi[num][1];

            // draw trigger box
            int tx1 = Ei[num][11] * db;
            int ty1 = Ei[num][12] * db;
            int tx2 = Ei[num][13] * db + db;
            int ty2 = Ei[num][14] * db + db;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color2], 1);

            // get extended position
            for (int j=0; j<Ei[num][7]; j++)
            {
               ex += Efi[num][2];
               ey += Efi[num][3];
            }

            // show with more precision than snap to 0-100 block
            al_fixed ax = al_fixmul(ex, db); // multiply by db
            al_fixed ay = al_fixmul(ey, db);
            al_fixed bx = al_fixdiv(ax, 20); // divide by 20
            al_fixed by = al_fixdiv(ay, 20);
            int cx = al_fixtoi(bx);       // convert to int
            int cy = al_fixtoi(by);
            crosshairs_nodb(0, 0, cx, cy, db, color3);

         }
         break;
         case 8: // trakbot
         {
            Num_legend_lines = 3;
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            int color = 14;
            legend_color[2] = 14;
            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }
            // draw yellow bullet prox circle
            int bs = Ei[num][17]*db/20;
            al_draw_circle(x1*db+db/2, y1*db+db/2, bs, palette_color[color], 1);
         }
         break;
         case 9: // cloner
         {

            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            sprintf(lmsg[4],"Trigger Box");
            Num_legend_lines = 5;

            int color2 = 10;
            legend_color[2] = 10;
            if (legend_highlight == 2)
            {
               color2 = highlight_color;
               legend_color[2] = highlight_color;
            }

            int color3 = 11;
            legend_color[3] = 11;
            if (legend_highlight == 3)
            {
               color3 = highlight_color;
               legend_color[3] = highlight_color;
            }

            int color4 = 14;
            legend_color[4] = 14;
            if (legend_highlight == 4)
            {
               color4 = highlight_color;
               legend_color[4] = highlight_color;
            }

            int cw = Ei[num][19];     // width
            int ch = Ei[num][20];     // height

            int cx1 = Ei[num][15];    // source
            int cy1 = Ei[num][16];
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;
            al_draw_rectangle(cx1*db, cy1*db, cx2*db, cy2*db, palette_color[color2], 1);

            int cx3 = Ei[num][17];    // destination
            int cy3 = Ei[num][18];
            int cx4 = cx3 + cw;
            int cy4 = cy3 + ch;
            al_draw_rectangle(cx3*db, cy3*db, cx4*db, cy4*db, palette_color[color3], 1);

            // draw trigger box
            int tx1 = Ei[num][11] * db;
            int ty1 = Ei[num][12] * db;
            int tx2 = Ei[num][13] * db + db;
            int ty2 = Ei[num][14] * db + db;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color4], 1);

         }
         break;
         case 11: sprintf(lmsg[1],"Block Walker Location"); break;
         case 12: // flapper
         {
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Trigger Box");
            sprintf(lmsg[3],"Height Above Player");
            Num_legend_lines = 4;
            legend_color[2] = 14;
            legend_color[3] = 10;

            int color2 = 14;
            legend_color[2] = 14;
            if (legend_highlight == 2)
            {
               color2 = highlight_color;
               legend_color[2] = highlight_color;
            }

            int color3 = 10;
            legend_color[3] = 10;
            if (legend_highlight == 3)
            {
               color3 = highlight_color;
               legend_color[3] = highlight_color;
            }

            int bw  = Ei[num][17]*db/20;
            int by1 = Ei[num][18]*db/20;
            int by2 = Ei[num][19]*db/20;

            // draw yellow bullet prox
            al_draw_rectangle((x1*db)-bw+db/2, (y1*db)-by1, (x1*db)+bw+db/2, (y1*db)+by2, palette_color[color2], 1);

            // draw red height above player line
            int hab = Ei[num][20]*db/20;
            al_draw_line((x1*db)-db*3, (y1*db)+hab, (x1*db)+db*3, (y1*db)+hab, palette_color[color3], 1);
         }
         break;
      }
      al_reset_clipping_rectangle();

   }
   if (obj_type == 2)  // items
   {
      if (!legend_highlight)
      {
         al_draw_rectangle(txc-tw, 20, txc+tw, 43, palette_color[15], 1);
         draw_item_shape( num, txc-tw+2, 22);
         sprintf(msg,"%s %d of %d", item_desc[sub_type], 1+num - item_first_num[sub_type],item_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], txc-tw+28, 29, 0, msg);
      }
      al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
      switch (sub_type)
      {
         case 1: // door
         {
            int color = 10;
            legend_color[2] = 10;
            Num_legend_lines = 3;

            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }

            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                      num_src++;
                     int x2 = item[i][4]/20;
                     int y2 = item[i][5]/20;
                     crosshairs(0, 0, x2, y2, color);
                     int o = db/2;
                     al_draw_line(x1*db+o, y1*db+o, x2*db+o, y2*db+o, palette_color[color], 1);
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]/20;
               int y2 = item[di][5]/20;
               crosshairs(0, 0, x2, y2, color);
               int o = db/2;
               al_draw_line(x1*db+o, y1*db+o, x2*db+o, y2*db+o, palette_color[color], 1);
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2: sprintf(lmsg[1],"Bonus Location"); break;
         case 3: sprintf(lmsg[1],"Exit Location"); break;
         case 4: // key
         {
            int color = 10;
            legend_color[2] = 10;
            Num_legend_lines = 3;
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");

            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }

            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = item[num][8];
            int y3 = item[num][9];
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4*db+db/2, 100*db-1, y4*db+db/2, palette_color[color], 1);
            al_draw_line(x4*db+db/2, 0, x4*db+db/2, 100*db-1, palette_color[color], 1);
            al_draw_rectangle(x2*db, y2*db, x3*db+db-1, y3*db+db-1, palette_color[color], 1);
         }
         break;
         case 5: sprintf(lmsg[1],"Start Location"); break;
         case 6: sprintf(lmsg[1],"Free Man Location"); break;
         case 7: sprintf(lmsg[1],"Mine Location"); break;
         case 8:
         {
            int color = 14;
            legend_color[2] = 14;
            Num_legend_lines = 3;
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");

            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }
            // draw yellow bomb damage
            int bs = (item[num][7] / 20) * db;
            al_draw_rectangle(x1*db-bs, y1*db-bs, x1*db+db-1+bs, y1*db+db-1+bs, palette_color[color], 1);
         }
         break;
         case 10:
         {
            sprintf(lmsg[1],"Message Location");
            sprintf(lmsg[2],"Display Position");
            Num_legend_lines = 3;
         }
         break;
         case 11:
         {
            int color = 14;
            legend_color[2] = 14;
            Num_legend_lines = 3;
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");

            if (legend_highlight == 2)
            {
               color = highlight_color;
               legend_color[2] = highlight_color;
            }
            // draw yellow bomb damage
            int bs = (item[num][7] / 20) * db;
            al_draw_rectangle(x1*db-bs, y1*db-bs, x1*db+db-1+bs, y1*db+db-1+bs, palette_color[color], 1);
         }
         break;
         case 12: sprintf(lmsg[1],"Warp Location"); break;
         case 14: sprintf(lmsg[1],"Switch Location"); break;
         case 15: sprintf(lmsg[1],"Sproingy Location");
         {
            int color = 14;
            legend_color[2] = 14;
            Num_legend_lines = 3;

            if (legend_highlight == 2)
            {

               color = highlight_color;
               legend_color[2] = highlight_color;
            }

            al_fixed jump_height = get_sproingy_jump_height(num);
            sprintf(lmsg[2],"Jump Height: %4.1f", al_fixtof(jump_height)/20);

            // do all this instead of just calling crosshairs so that we don't snap to blocks
            al_fixed fy = al_itofix(y1) - al_fixdiv(jump_height, al_itofix(20));
            fy = al_fixmul(fy, al_itofix(db));
            int y = al_fixtoi(fy);
            int mx = 0;
            int my = 0;
            int qx = x1;
            al_draw_rectangle(mx+(qx*db), my+(y), mx+(qx*db)+db-1, my+(y)+db-1, palette_color[color], 1);
            al_draw_line(mx, my+(y)+db/2, mx+(100*db), my+(y)+db/2, palette_color[color], 1);
            al_draw_line(mx+(qx*db)+db/2, my, mx+(qx*db)+db/2, my+(100*db), palette_color[color], 1);
         }
         break;
      } // end of switch case
      al_reset_clipping_rectangle();

   }  // end of items
   if (obj_type != 4)  // no legend for lifts
   {
      for (x=1; x<Num_legend_lines; x++)// draw text lines
         al_draw_text(font, palette_color[legend_color[x]], txc, db*100-26+(3-Num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);

      if (!legend_highlight)
      {
         al_draw_text(font, palette_color[legend_color[0]], txc, db*100-36+ (4-Num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
         al_draw_rectangle(txc-100, db*100-38+ (4-Num_legend_lines)*8, txc+100, db*100-1, palette_color[13], 1); // big frame
         al_draw_rectangle(txc-100, db*100-38+ (4-Num_legend_lines)*8, txc+100, db*100-28+ (4-Num_legend_lines)*8, palette_color[13], 1); // top frame
      }
   }
}

int move_obt_with_map(int obt, int type, int num)
{
   int old_mx, old_my;
   int mouse_on_extra = 0;
   int mouse_move = 0;
   int mouse_adj = 0;

   // enemy
   int mouse_on_enemy = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;

   // item
   int mouse_on_item = 0;
   int mouse_on_kbr_ul = 0;
   int mouse_on_kbr_lr = 0;
   int mouse_on_sp = 0;
   int mouse_on_msg_ul = 0;

   // this is the awesome section that lets you move stuff on the map just by clicking and dragging
   if ((mouse_x < db*100)  && (mouse_x < db*100) ) // is mouse on map
   {
      int mx = mouse_x / db;
      int my = mouse_y / db;

      al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);

      // search for extra's for currently selected obt
      if ((obt == 3) && ((type == 7) || (type == 9))) // podzilla or cloner trigger box
      {
         // trigger box coordinates
         int x1 = Ei[num][11];
         int y1 = Ei[num][12];
         int x2 = Ei[num][13];
         int y2 = Ei[num][14];
         if ((!mouse_on_extra) && (mx == x1) && (my == y1)) // upper left corner
         {
            mouse_on_tb_ul = 1;
            mouse_on_extra = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[14+a*64], 1); // mark entire trigger box
            al_draw_rectangle(x1*db, y1*db, x1*db+db, y1*db+db, palette_color[14], 1);                 // mark ul corner
            mouse_move = 1;
         }
         if ((!mouse_on_extra) && (mx == x2+1) && (my == y2+1))  // lower right corner
         {
            mouse_on_tb_lr = 1;
            mouse_on_extra = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[14+a*64], 1); // mark entire trigger box
            al_draw_rectangle(x2*db, y2*db, x2*db+db, y2*db+db, palette_color[14], 1);                 // mark lr corner
            mouse_adj = 1;
         }
      }


      if ((obt == 3) && (type == 9)) // cloner
      {
         int w = Ei[num][19];     // width
         int h = Ei[num][20];     // height
         int x1 = Ei[num][15];    // source box
         int y1 = Ei[num][16];
         int x2 = x1 + w - 1;
         int y2 = y1 + h - 1;
         int x3 = Ei[num][17];    // dest box
         int y3 = Ei[num][18];
         int x4 = x3 + w - 1;
         int y4 = y3 + h - 1;

         if ((!mouse_on_extra) && (mx == x1) && (my == y1)) // source box upper left corner
         {
            mouse_on_csb_ul = 1;
            mouse_on_extra = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[10+a*64], 1); // mark entire box
            al_draw_rectangle(x1*db, y1*db, x1*db+db, y1*db+db, palette_color[10], 1);                 // mark ul corner
            mouse_move = 1;
         }
         if ((!mouse_on_extra) && (mx == x2+1) && (my == y2+1))  // source box lower right corner
         {
            mouse_on_csb_lr = 1;
            mouse_on_extra = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[10+a*64], 1); // mark entire box
            al_draw_rectangle(x2*db, y2*db, x2*db+db, y2*db+db, palette_color[10], 1);                 // mark lr corner
            mouse_adj = 1;
         }

         if ((!mouse_on_extra) && (mx == x3) && (my == y3)) // destination box upper left corner
         {
            mouse_on_cdb_ul = 1;
            mouse_on_extra = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x3*db+a, y3*db+a, x4*db+db-a, y4*db+db-a, palette_color[11+a*64], 1); // mark entire box
            al_draw_rectangle(x3*db, y3*db, x4*db+db, y4*db+db, palette_color[11], 1);                 // mark ul corner
            mouse_move = 1;
         }
      }  // end of cloner

      if ((obt == 2) && (type == 10)) // pop message
      {
         int x1 = item[num][10];
         int y1 = item[num][11];
         if ((!mouse_on_extra) && (mx == x1) && (my == y1)) // message upper left corner
         {
            mouse_on_msg_ul = 1;
            mouse_on_extra = 1;
            mouse_move = 1;
         }
      }


      if ((obt == 2) && (type == 4)) // key
      {
         int x1 = item[num][6];
         int y1 = item[num][7];
         int x2 = item[num][8];
         int y2 = item[num][9];
         if ((!mouse_on_extra) && (mx == x1) && (my == y1)) // key block range upper left corner
         {
            mouse_on_extra = 1;
            mouse_on_kbr_ul = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[10+a*64], 1); // mark entire box
            al_draw_rectangle(x1*db, y1*db, x1*db+db, y1*db+db, palette_color[10],1);                 // mark ul corner
            mouse_move = 1;
         }
         if ((!mouse_on_extra) && (mx == x2+1) && (my == y2+1))  // key block range lower right corner
         {
            mouse_on_extra = 1;
            mouse_on_kbr_lr = 1;
            for (int a=0; a<4; a++)
               al_draw_rectangle(x1*db+a, y1*db+a, x2*db+db-a, y2*db+db-a, palette_color[10+a*64],1); // mark entire box
            al_draw_rectangle(x2*db, y2*db, x2*db+db-1, y2*db+db-1, palette_color[10], 1);             // mark lr corner
            mouse_adj = 1;
         }
      }
      if ((obt == 2) && (type == 15)) // sproingy
      {
         int x0 = item[num][4];
         int y0 = item[num][5];
         int x1 = x0 /20;
         // find where extended position is
         al_fixed jump_height = get_sproingy_jump_height(num);
         // don't snap to blocks for y1
         al_fixed fy = al_itofix(y0/20) - al_fixdiv(jump_height, al_itofix(20));
         fy = al_fixmul(fy, al_itofix(db));
         int y1 = al_fixtoi(fy);
         //printf("mx:%d x1:%d   mouse_y:%d y1:%d jh:%d\n", mx, x1, mouse_y, y1, al_fixtoi(jump_height));

         if ((!mouse_on_extra) && (mx == x1) && (mouse_y > y1) && (mouse_y < y1+db))  // is mouse is on the jump location
         {
            mouse_on_sp = 1;
            mouse_on_extra = 1;
            al_draw_rectangle(x1*db, y1, x1*db+db-1, y1+db, palette_color[10], 1); // mark
         }
      }
      al_reset_clipping_rectangle();

      // if mouse is not on any extras for current object, check to see if mouse is on an object
      if (!mouse_on_extra)
      {
         // is mouse on an enemy?
         for (int e=0; e<100; e++)
         {
            int x0 = al_fixtoi(Efi[e][0]);
            int y0 = al_fixtoi(Efi[e][1]);
            if ((Ei[e][0]) && (x0 == mx*20) && (y0 == my*20))
            {
               if ((!key[MAP_LOCK_KEY]) && (!Viewer_lock))
               {
                  mouse_on_enemy = 1;
                  if (e != num) Redraw = 1;
                  num = e; // set current item
                  obt = 3;
               }
               else if ((obt == 3) && (e == num)) mouse_on_enemy = 1; // if locked, only allow select of current object
            }
         }

         if (!mouse_on_enemy)
         {
            // is mouse on an item?
            for (int x=0; x<500; x++)  // cycle items
            {
               int x0 = item[x][4];
               int y0 = item[x][5];
               if ((item[x][0]) && (x0 == mx*20) && (y0 == my*20))
               {
                  if ((!key[MAP_LOCK_KEY]) && (!Viewer_lock))
                  {
                     mouse_on_item = 1;
                     if (x != num) Redraw = 1;
                     num = x; // set current item
                     obt = 2;
                  }
                  else if ((obt == 2) && (x == num)) mouse_on_item = 1; // if locked, only allow select of current object
               }
            }
         }
         if ((!mouse_on_enemy) && (!mouse_on_item))
         {
            // is mouse on any lift?
            for (int x=0; x<num_lifts; x++)  // cycle lifts
               for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
                  if (lift_steps[x][y].type == 1) // look for move step
                  {
                     int nx = ((lift_steps[x][y].x + lifts[x].width  * 10) *db)/20;
                     int ny = ((lift_steps[x][y].y + lifts[x].height * 10) *db)/20;
                     int w = lifts[x].width  * 10 * db / 20 + 1;
                     int h = lifts[x].height * 10 * db / 20 + 1;
                     // is mouse on this step ?
                     if ((mouse_x > nx - w)  && (mouse_x < nx + w) && (mouse_y > ny - h)  && (mouse_y < ny + h))
                     {
                        if ((!key[MAP_LOCK_KEY]) && (!Viewer_lock))
                        {
                           obt = 4;  // lift
                           type = x; // lift #
                           num = y;  //step #
                        }
                     }
                  }
         }
      }


      // mark item or enemy if mouse is currently detected on it
      if ((mouse_on_item) || (mouse_on_enemy))
         //al_draw_rectangle(mx*db-1, my*db-1, mx*db+db+0, my*db+db+0, palette_color[13], 1);
         al_draw_rectangle(mx*db-1, my*db-1, mx*db+db+0, my*db+db+0, palette_color[14], 1);



      if (mouse_b1) // only when mouse_b is first pressed, move mouse position
      {
         if (mouse_on_tb_lr)  al_set_mouse_xy(display, (mouse_x-db) * les, (mouse_y-db) * les);  // move back one block
         if (mouse_on_kbr_lr) al_set_mouse_xy(display, (mouse_x-db) * les, (mouse_y-db) * les); // move back one block
         if (mouse_on_sp) old_my = mouse_y;
      }

      while (mouse_b1)
      {
         proc_controllers();
         draw_bs(14);

         if ((old_mx != mouse_x) || (old_my != mouse_y))
         {
            old_mx = mouse_x;
            old_my = mouse_y;
            mx = mouse_x / db;
            my = mouse_y / db;
            if (mx > 99) mx = 99;
            if (my > 99) my = 99;
            if (mx < 0) mx = 0;
            if (my < 0) my = 0;

            if (mouse_on_enemy)
            {
               // get offset of move
               int x_off = al_fixtoi( al_itofix(mx * 20) - Efi[num][0] ) / 20;
               int y_off = al_fixtoi( al_itofix(my * 20) - Efi[num][1] ) / 20;

               // set new position
               Efi[num][0] = al_itofix(mx * 20);
               Efi[num][1] = al_itofix(my * 20);

               // move podzilla's trigger box too
               if (Ei[num][0] == 7)
               {
                  Ei[num][11] += x_off;
                  Ei[num][12] += y_off;
                  Ei[num][13] += x_off;
                  Ei[num][14] += y_off;
               }

               // move cloner's stuff too
               if (Ei[num][0] == 9)
               {
                  Ei[num][11] += x_off; // trigger box
                  Ei[num][12] += y_off;
                  Ei[num][13] += x_off;
                  Ei[num][14] += y_off;
                  Ei[num][15] += x_off; // source
                  Ei[num][16] += y_off;
                  Ei[num][17] += x_off; // dest
                  Ei[num][18] += y_off;
               }
            } // end of mouse on enemy

            if (mouse_on_tb_ul) // move trigger box from ul
            {
               // get the width and height
               int w = Ei[num][13] - Ei[num][11];
               int h = Ei[num][14] - Ei[num][12];

               // set new position
               Ei[num][11] = mx;
               Ei[num][12] = my;
               Ei[num][13] = mx + w;
               Ei[num][14] = my + h;

          } // end of mouse tb_ul

            if (mouse_on_tb_lr) // resize trigger box from lr
            {
               // prevent lr corner from being less than ul corner
               if (mx < Ei[num][11]) mx = Ei[num][11];
               if (my < Ei[num][12]) my = Ei[num][12];

               // set new postion
               Ei[num][13] = mx;
               Ei[num][14] = my;
            } // end of mouse tb_lr


            if (mouse_on_csb_ul) // move cloner source box from ul
            {
               Ei[num][15] = mx; // set new postion
               Ei[num][16] = my;
            } // end of mouse csb_ul

            if (mouse_on_csb_lr) // resize box from lr
            {
               // get ul corner
               int x1 = Ei[num][15];
               int y1 = Ei[num][16];

               // prevent lr corner from being less than ul corner
               if (mx < x1+1) mx = x1+1;
               if (my < y1+1) my = y1+1;

               // set new sizes
               Ei[num][19] = mx - x1;
               Ei[num][20] = my - y1;
            } // end of mouse csb_lr

            if (mouse_on_cdb_ul) // cloner destination ul
            {
               Ei[num][17] = mx; // set new postion
               Ei[num][18] = my;
            } // end of mouse_on_cdb_ul

            if (mouse_on_sp) // adjust sproingy jump height
            {
                al_hide_mouse_cursor(display);
                while (mouse_b1) // trap here to hide mouse cursor
                {
                   mouse_dy = 0;  // reset y mouse movement
                   proc_controllers();
                   // set mouse to middle of screen so we have room to adjust
                   al_set_mouse_xy(display, SCREEN_W/2, SCREEN_H/2);

                   // adjust sproinginess with y mouse movement
                   item[num][7] -= mouse_dy/2;
                   // bounds check
                   if (item[num][7] < 40) item[num][7] = 40;
                   if (item[num][7] > 200) item[num][7] = 200;
                   al_flip_display();
                   al_clear_to_color(al_map_rgb(0,0,0));
                   draw_big(1);
                   title_obj(obt, type, num, 0, 15);
                }
                al_show_mouse_cursor(display);
            }

            if (mouse_on_msg_ul) // move msg
            {
               // set new position
               item[num][10] = mx;
               item[num][11] = my;
            }
            if (mouse_on_kbr_ul) // move key block range from ul
            {
               // get the width and height
               int w = item[num][8] - item[num][6];
               int h = item[num][9] - item[num][7];

               // set new position
               item[num][6] = mx;
               item[num][7] = my;
               item[num][8] = mx + w;
               item[num][9] = my + h;
            }
            if (mouse_on_kbr_lr) // adjust key block range from lr
            {
               // don't allow lr to be less than ul
               if (mx < item[num][6]) mx = item[num][6];
               if (my < item[num][7]) my = item[num][7];

               // set new position
               item[num][8] = mx;
               item[num][9] = my;
            }

            if (mouse_on_item)
            {
               // set new position
               item[num][4] = mx * 20;
               item[num][5] = my * 20;
               itemf[num][0] = al_itofix(mx * 20);
               itemf[num][1] = al_itofix(my * 20);
               al_draw_rectangle(mx*db-1, my*db-1, mx*db+db, my*db+db, palette_color[14], 1); // mark item we are moving
            } // end of mouse on item

            old_mx = mouse_x;
            old_my = mouse_y;


            // redraw
            draw_big(1);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            title_obj(obt, type, num, 0, 15);

            // show the message after the screen is redrawn
            if (mouse_on_msg_ul) display_pop_message(num, pmsg[num], txc, pop_msg_viewer_pos, 0, 0);


         } // end of if mouse changed
      } // end of while mouse b1 pressed
   } // end of mouse on map

   if (mouse_move) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);


   if (obt == 2) num+=1000; // mark as item
   if (obt == 4) num+=4000; // mark as lift
   return num;
}




void object_viewer(int obt, int num)
{
   int ret = 0;
   int quit = 0;

   al_set_target_backbuffer(display);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));


   extern int e_num_of_type[50];      // sort results
   extern int e_first_num[50];
   extern int item_first_num[20];
   extern int ty;   // button start
   extern int bts;  // button spacing

   int mb;   // button selection
   int type=0; // enemy or item type

   int highlight_counter=0;
   int a;


   while (!quit)
   {
      // button x position
      int xa = SCREEN_W-(SCREEN_W-(db*100))+1;
      int xb = SCREEN_W-3;

      // button y size
      bts = 18;
      if (SCREEN_H < 600) bts = 14;

      if (obt == 4) // lift
      {
         ret = lift_editor(num); // run lift viewer and stay there until quit or object changes
         if ((ret >= 2000) && (ret < 2500)) // obj changed to item
         {
            num = ret-2000;
            obt = 2;
         }
         if ((ret >= 1000) && (ret < 1100)) // obj changed to enemy
         {
            num = ret-1000;
            obt = 3;
         }
         if (ret == 0) quit = 1;
      }

      if ((obt == 2) || (obt == 3)) // item or enemy
      {
         if (obt == 2) type = item[num][0];
         if (obt == 3) type = Ei[num][0];

         draw_big(1);
         title_obj(obt, type, num, 0, 15);

         mb = 0;
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

         // is mouse on legend ?
         int y1_legend = db*100- 34 + (5-Num_legend_lines)*8; // legend pos
         int y2_legend = y1_legend + (Num_legend_lines-1)*8;
         if ((mouse_x > xa) && (mouse_x < xb) && (mouse_y > y1_legend) && (mouse_y < y2_legend))
         {
            int legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?
            if (++highlight_counter > 16) highlight_counter = 1;
            if ((highlight_counter > 0)  && (highlight_counter < 5)) title_obj(obt, type, num, legend_line, 10);
            if ((highlight_counter > 4)  && (highlight_counter < 9)) title_obj(obt, type, num, legend_line, 14);
            if ((highlight_counter > 8)  && (highlight_counter < 13)) title_obj(obt, type, num, legend_line, 15);
            if ((highlight_counter > 12) && (highlight_counter < 17)) title_obj(obt, type, num, legend_line, 11);
         }
         else highlight_counter = 0; // mouse not on legend

         // map move and adjust !!
         int ret_num = move_obt_with_map(obt, type, num);
         if ((ret_num > -1) && (ret_num < 100))    // enemy
         {
            obt = 3;
            num = ret_num;
         }
         if ((ret_num > 999) &&  (ret_num < 1500)) // item
         {
            obt = 2;
            num = ret_num - 1000;
         }
         if ((ret_num > 3999) && (ret_num < 4040)) // lift
         {
            obt = 4;
            num = ret_num - 4000;
         }

         a=0;

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

         int lc; // lock_color;
         if (Viewer_lock) lc = 7;
         else lc = 6;

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
         a++;

         if (obt == 3)    // enemies
         {
            a+=1; // leave a space between common button and specific
            switch (type) // enemy subtypes
            {
               case 3:     // archwag
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 12, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // x - speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 13, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // y - speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 8,  num, type, obt, 0, 15, 13,  0, 1,0,0,0); a++;  // initial direction
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 9,  num, type, obt, 0, 15, 13,  0, 1,0,0,0); a+=2; // bounce mode
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 15, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump wait count
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump under width
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 17, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump before wall
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 18, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a+=2;  // jump before hole
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 19, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 20, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet proximity
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 21, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a+=2;  // bullet retrigger time
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 4: // bouncer
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 23, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2; // seek count
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 13, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // main shape
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 14, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // seek shape
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 6: // cannon
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 23, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // seek count
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 35, num, type, obt, 0,  8, 15, 15, 1,0,0,0); a+=2;  // extra hits to kill
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 19, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 21, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a+=2;  // bullet retrigger time
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 7: // podzilla
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 29, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 30, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // pause
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 19, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 15, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // move extended position
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a+=2;  // set trigger box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 8: // trakbot
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 45, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // x - speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 46, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // y - speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 11, num, type, obt, 0, 11, 15,  0, 1,0,0,0); a++;  // initial direction
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 12, num, type, obt, 0, 11, 15,  0, 1,0,0,0); a+=2;  // drop mode
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 19, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 20, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet proximity
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 21, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a+=2;  // bullet retrigger time
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 9: // cloner
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 27, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // trigger type
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 34, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // delay timer

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 74, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++;   // created obj time to live
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 75, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a+=2;  // max created obj

                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 17, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // get source
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 18, num, type, obt, 0, 11, 11,  0, 1,0,0,0); a++;  // get destination
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a+=2;  // get trigger box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus
               break;
               case 11: // block walker
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 12, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // x - speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 13, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // y - speed
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 8,  num, type, obt, 0, 15, 13,  0, 1,0,0,0); a++;  // initial direction
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 9,  num, type, obt, 0, 15, 13,  0, 1,0,0,0); a+=2;  // bounce mode
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 15, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump wait count
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump under width
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 17, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a++;  // jump before wall
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 18, num, type, obt, 0,  7, 15, 15, 1,0,0,0); a+=2;  // jump before hole
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
               break;
               case 12: // flapper

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 60, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // x speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 61, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // x accel
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 8,  num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
                  a++;

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 36, num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // y speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 62, num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // flap yinc scale
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 63, num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // height above player
                  a++;

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 38, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a++;  // trigger width
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 39, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a++;  // trigger height
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 40, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a+=2;  // trigger depth

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 19, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a++;  // bullet speed
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 21, num, type, obt, 0,  9, 15, 15, 1,0,0,0); a+=2;  // bullet retrigger

                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 24, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // collision box
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 25, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health decrement
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 42, num, type, obt, 0,  4, 15, 15, 1,0,0,0); a++;  // health bonus


               break;
            }
         }
         if (obt == 2) // items
         {
            a+=1; // leave a space between common button and specific
            switch (type) // item subtypes
            {
               case 1: // door
                {
                  int abc = 11; // regular button frame color
                  int atc = 15; // regular button text color

                  int cbc = item[num][14];  // door color

                  if (item[num][8] == 0) // if exit only change color of inactive buttons
                  {
                      abc = 15;
                      atc = 127;
                  }
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 26, num, type, obt, 0,  15,  13, 14, 1,0,0,0); a++; // stat | fall | carry
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 49, num, type, obt, 0, 12 ,  15,  0, 1,0,0,0); a++; // door type
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 4,  num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // set linked item
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 50, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // enter mode (up | down)
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 53, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // move type
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 51, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // exit link show
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 52, num, type, obt, 0, cbc,  15,  0, 1,0,0,0); a++; // get new shape
                  mdw_colsel(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 5,  num, type, obt, 0,   0,   0,  0, 0,0,0,0);      // change color
               }
               break;
               case 2: // bonus
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1,  num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // health bonus
               break;
               case 3: // exit
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 47, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // exit with x enemies left
               break;
               case 4: // key
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 2,  num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 5,  num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++; // set new block range
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 48, num, type, obt, 0,  4, 15,  0, 1,0,0,0); a++; // range type
               break;
               case 5: // start
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
               break;
               case 6: // free man
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 2,  num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall
               break;
               case 7: // mine
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 11, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // mine damage
               break;
               case 8: // bomb
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  5, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // fuse length
               break;
               case 10: // message
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 55, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a++; // set msg position
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 54, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // display time
                  mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  2, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // frame color select
                  mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  3, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // text color select
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  7, num, type, obt, 0, 15, 13, 14, 1,0,0,0); a++; // edit text

                  a+=2; // leave space for OK and Cancel buttons

                  pop_msg_viewer_pos = ty+a*bts+bts/2+2;
                  display_pop_message(num, pmsg[num], txc, pop_msg_viewer_pos, 0, 0); // show the message

               break;
               case 11: // rocket
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  2, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  6, num, type, obt, 0, 15, 13,  0, 1,0,0,0); a++; // set initial direction
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  6, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // acceleration
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  7, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // max speed
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  8, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // steerability
               break;
               case 14: // switch
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
               break;
               case 15: // sproingy
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // jump height
               break;
               case 12: // warp
                  mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 13, 0,  1,0,0,0); a++; // stat | fall | carry
                  mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 28, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // warp level
               break;
            } // end of switch type
         } // end of if obt = item


         // change to control this with exit codes
         switch(mb)
         {
            case 18: // move
               if (getxy((char *)"Set New Location",obt, type, num) == 1)
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
                  item_sort();
                  if (num >= item_first_num[type]+item_num_of_type[type]) num--;
                  if (item_num_of_type[type] < 1) quit = 1;
               }
               draw_big(1);
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
               extern int draw_item_num, draw_item_type;
               draw_item_num = num;
               draw_item_type = obt;
            break;
            case 24:  // viewer help
               help((char *)"Viewer Basics");
            break;
            case 25:  // object help
            if (obt==3)
            {
               if (type == 3)  help((char *)"Archwagon Viewer");
               if (type == 4)  help((char *)"Bouncer Viewer");
               if (type == 6)  help((char *)"Cannon Viewer");
               if (type == 7)  help((char *)"Podzilla Viewer");
               if (type == 8)  help((char *)"Trakbot Viewer");
               if (type == 9)  help((char *)"Cloner Viewer");
               if (type == 11) help((char *)"Block Walker Viewer");
               if (type == 12) help((char *)"Flapper Viewer");
            }
            if (obt==2)
            {
               if (type == 1)  help((char *)"Door Viewer");
               if (type == 2)  help((char *)"Health Bonus Viewer");
               if (type == 3)  help((char *)"Exit Viewer");
               if (type == 4)  help((char *)"Key Viewer");
               if (type == 5)  help((char *)"Start Viewer");
               if (type == 6)  help((char *)"Free Man Viewer");
               if (type == 7)  help((char *)"Mine Viewer");
               if (type == 8)  help((char *)"Bomb Viewer");
               if (type == 10) help((char *)"Message Viewer");
               if (type == 11) help((char *)"Rocket Viewer");
               if (type == 12) help((char *)"Warp Viewer");
               if (type == 14) help((char *)"Switch Viewer");
               if (type == 15) help((char *)"Sproingy Viewer");
            }
            break;
         } // end of switch (mb)
      }

      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;  // wait for release
      }
   } // end of while (!quit)
}




























