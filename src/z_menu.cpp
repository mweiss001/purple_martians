// z_menu.cpp

#include "pm.h"
#include "z_menu.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "z_player.h"
#include "n_netgame.h"
#include "mwLogo.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwProgramState.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_level.h"
#include "z_fnx.h"
#include "z_screen.h"


char help_string[5000][200];
char global_string[20][25][80];
char msg[1024];

int load_help(void)
{
   char filename[100] = "help/pmhelp.txt";
   FILE *filepntr;
   char buff[200];
   int line=0;
   int ch=0;

   if ((al_filename_exists(filename)) == 0)
   {
      sprintf(msg, "Failed to load help file from %s\n", filename);
      m_err(msg);
      return 0;
   }
   else
   {
      filepntr=fopen(filename,"r");
      while(ch != EOF)
      {
         int loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            if (ch != 13)
            {
               buff[loop] = ch;
               loop++;
            }
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         strcpy (help_string[line], buff);
         line++;
      }
      fclose(filepntr);
   }
   // printf("%d\n",line);
   return line;
}


void chop_first_x_char(char *str, int n)
{
   char tmp[200];
   for(int x=n; x < (signed int)strlen(str)+1; x++)
      tmp[x-n] = str[x]; // chop first n
   strcpy(str, tmp);
}








void help(const char *topic)
{
//   if (mwD.SCREEN_H < 480) return;       // wont work with mwD.SCREEN_H < 480
   mwPS.help_screens_running = 1;
   int num_of_lines = load_help();
   char section_names[60][80];
   int section_first_lines[60];
   int num_sections;
   char buff2[200];
   char msg[200];

   int redraw = 1;
   ALLEGRO_BITMAP * hlift = NULL;
   ALLEGRO_BITMAP * status_window = NULL;
   ALLEGRO_BITMAP * selection_window = NULL;

   int fc =    8;   // frame color
   int ftc =   15;  // frame text color
   //int ftdm = -1;   // frame text draw mode

   int sc =   fc;    // section divider color
   int stc =  ftc;   // section divider text color
   //int stdm = ftdm;  // section text draw mode

   // fill help_string[5000][200], section_name[60][80], section_first_lines[60] and set last_pos
   num_sections = 0;
   int line = 0;
   int last_pos;
   while (line < num_of_lines)
   {
      if (strncmp(help_string[line], "<section>", 9) != 0)
         line++;
      else
      {
         strcpy(msg, help_string[line]);
         for(int x=9; x < (signed int)strlen(msg)+1; x++)
            buff2[x-9] = msg[x]; // chop first 9
         strcpy(section_names[num_sections], buff2);
         section_first_lines[num_sections] = line;

         if (num_sections < 10)
            sprintf(help_string[line], "<section>0%-1d - %s",  num_sections, section_names[num_sections]);
         else
            sprintf(help_string[line], "<section>%-2d - %s",  num_sections, section_names[num_sections]);

         line++;
         num_sections++;
      }
      last_pos = section_first_lines[num_sections-1];
   }
   line=0;


   // add section headings to help_string
   int sl = 41;                      // line position to insert toc at
   int toc_size = num_sections * 2;  // size of toc to insert

   // slide all down
   for(int x=num_of_lines + toc_size; x > (sl-1+toc_size); x--)
      strcpy(help_string[x], help_string[x-toc_size]);

   for (int c=0; c<num_sections; c++)
   {
      if (c > 1) section_first_lines[c] += toc_size;  // slide section first line numbers
      if (c < 10) sprintf(msg, "<l15>              0%-1d ---- %s", c, section_names[c] );
      else        sprintf(msg, "<l15>              %-2d ---- %s",  c, section_names[c] );
      strcpy(help_string[sl+c*2], msg);
      strcpy(help_string[sl+c*2+1], ""); // blank line in between
   }

   // increase because of inserted section name list (toc)
   last_pos     += toc_size;
   num_of_lines += toc_size;

   // set section from parameter 'topic'
   if (strlen(topic) > 1)
      for (int c=0; c<num_sections; c++)
         if (strncmp(section_names[c], topic, strlen(topic)) == 0)
            line = section_first_lines[c];


   int got_num =0;
   int quit = 0;
   while (!quit)
   {

      if (redraw)
      {
         redraw = 0;
         al_destroy_bitmap(hlift);
         hlift = al_load_bitmap("help/lift.bmp");
         al_destroy_bitmap(status_window);
         status_window = al_load_bitmap("help/status_window.bmp");
         al_destroy_bitmap(selection_window);
         selection_window = al_load_bitmap("help/selection_window.bmp");
      }

      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0,0,0));

      int dx = mwD.SCREEN_W/2 - 320;
      //int xo  = (mwD.SCREEN_W - 640) / 2;   // x offset (distance from left screen edge to start of help screen)
      int lpp = (mwD.SCREEN_H - 40)  / 8;   // lines per page

      last_pos = num_of_lines - lpp - 2;

      for (int x=0; x<16; x++)
         al_draw_rectangle(dx+x, x, dx+639-x, mwD.SCREEN_H-1-x, mC.pc[fc+(x*16)], 1);

      al_draw_text(mF.pr8, mC.pc[ftc], dx+320,                    2, ALLEGRO_ALIGN_CENTRE, "Purple Martians Help");
      al_draw_text(mF.pr8, mC.pc[ftc], dx+16,                     2, 0, "<UP><DOWN>");
      al_draw_text(mF.pr8, mC.pc[ftc], dx+640-(11*8)-16,          2, 0, "<ESC>-quits");
      al_draw_text(mF.pr8, mC.pc[ftc], dx+16,            mwD.SCREEN_H-9, 0, "<PAGEUP><PAGEDOWN>");
      al_draw_text(mF.pr8, mC.pc[ftc], dx+640-(11*8)-16, mwD.SCREEN_H-9, 0, "<HOME><END>");

      for (int c=0; c<lpp; c++) // cycle lines
      {
         int xindent = 0;                   // start at xindent = 0
         int just = 0;                      // default just = left
         int color = 15;                    // default regular color unless changed
         sprintf(msg, "%s", help_string[line+c]); // put line to process in msg

         int processed_tag_this_time_through;
         int nexttag = 0;
         while (nexttag != -1)
         {
            processed_tag_this_time_through = 0;

            int tay = 0; // text align for lining up with bitmaps

            int sy = 12 + (c*8);    // shape draw y position
            int sx = 20 + xindent;  // shape draw x pos left just
            int sxc = (640/2) - 10; // shape draw x pos centered

            if (strncmp(msg, "<title>", 7) == 0) // show title
            {
                //al_set_clipping_rectangle((dx+12)*mwD.display_transform_double, (12*mwD.display_transform_double), (639-12*2)*mwD.display_transform_double, (mwD.SCREEN_H-12*2)*mwD.display_transform_double);
                mwD.mw_set_clipping_rect(dx+12, 12, 639-12*2, mwD.SCREEN_H-12*2);
                draw_title(dx+sxc+10, sy, 360, 64, 8);
                msg[0]= 0;
                al_reset_clipping_rectangle();
           }
            if (strncmp(msg, "<section>", 9) == 0) // <section> "txt" - make a new section with "txt"
            {
               chop_first_x_char(msg, 9);
               for (int x=0;x<16;x++)
               {
                  al_draw_line(dx+x, 28+(c*8)+x, dx+639-x, 28+(c*8)+x, mC.pc[sc+(x*16)], 1 );
                  al_draw_line(dx+x, 28+(c*8)-x, dx+639-x, 28+(c*8)-x, mC.pc[sc+(x*16)], 1 );
               }
               al_draw_text(mF.pr8, mC.pc[stc], dx+320, 24+(c*8),  ALLEGRO_ALIGN_CENTER, msg);
               msg[0]= 0;
            }
            if (strncmp(msg, "<end of file>", 13) == 0)
            {
               for (int x=0;x<16;x++)
               {
                  al_draw_line(dx+x, 28+(c*8)+x, dx+639-x, 28+(c*8)+x, mC.pc[sc+(x*16)], 1 );
                  al_draw_line(dx+x, 28+(c*8)-x, dx+639-x, 28+(c*8)-x, mC.pc[sc+(x*16)], 1 );
               }
               al_draw_text(mF.pr8, mC.pc[stc], dx+320, 24+(c*8),  ALLEGRO_ALIGN_CENTER, msg);
               c = lpp;   // end the cycle lines loop to prevent drawing unitialized lines
               msg[0]= 0;
            }
            if (strncmp(msg, "<lift>", 6) == 0) // show lift
            {
                al_draw_bitmap(hlift, dx+sxc-30, sy, 0 );
                msg[0]= 0;
            }
            if (strncmp(msg, "<sta_w>", 7) == 0) // show selection window
            {
                al_draw_bitmap(status_window, dx+sxc-160, sy, 0 );
                msg[0]= 0;
            }
            if (strncmp(msg, "<sel_w>", 7) == 0) // show selection window
            {
                al_draw_bitmap(selection_window, dx+sxc-161, sy, 0 );
                msg[0]= 0;
            }
            if (strncmp(msg, "<mdw1>", 6) == 0) // show mdw logo
            {
               mwD.mw_set_clipping_rect(dx+12, 12, 639-12*2, mwD.SCREEN_H-12*2);
               //al_set_clipping_rectangle((dx+12)*mwD.display_transform_double, (12*mwD.display_transform_double), (639-12*2)*mwD.display_transform_double, (mwD.SCREEN_H-12*2)*mwD.display_transform_double);
               float sc = .25;
               int xo = (int)(200 * sc)+16;
               mwL.mdw_an(dx+320 + sxc-xo, sy+50, sc);
               mwL.mdw_an(dx+320 - sxc+xo, sy+50, sc);
               msg[0]= 0;
               al_reset_clipping_rectangle();
            }
            if (strncmp(msg, "<ac", 3) == 0) // <acxxx> show animation sequence (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = msg[5];
                buff2[3] = 0;
                int ans = mwB.zz[0][atoi(buff2)];
                al_draw_bitmap(mwB.tile[ans], dx+sxc, sy, 0 );
                msg[0]= 0;
            }


            if (strncmp(msg, "<ab", 3) == 0) // <axx> show animation sequence (left just) // for block tiles instead of tiles
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = 0;

                //printf("s:'%s' i:%d\n", buff2, atoi(buff2));

                int ans = mwB.zz[0][atoi(buff2)];
                al_draw_bitmap(mwB.btile[ans], dx+sx, sy, 0 );

                chop_first_x_char(msg, 6);
                xindent +=24;
            }
            else if (strncmp(msg, "<a", 2) == 0) // <axx> show animation sequence (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int ans = mwB.zz[0][atoi(buff2)];
                al_draw_bitmap(mwB.tile[ans], dx+sx, sy, 0 );

                chop_first_x_char(msg, 5);
                xindent +=24;
            }


            if (strncmp(msg, "<hb", 3) == 0) // <hb> show centered health bar
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = msg[5];
                buff2[3] = 0;
                int health = atoi(buff2);
                draw_percent_bar(dx+sxc+10, sy, 88, 10, health);
                al_draw_textf(mF.pr8, mC.pc[14], dx+sxc+10, sy+2, ALLEGRO_ALIGN_CENTER, "Health:%-2d", health);
                msg[0]= 0;
            }
            if (strncmp(msg, "<pc", 3) == 0) // <pcxx> show player and color text (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = mwB.zz[1][9];
                al_draw_bitmap(mwB.player_tile[pco][ans], dx+sxc-40, sy, 0 );
                al_draw_text(mF.pr8, mC.pc[pco], dx+sxc+24-40, sy+7, 0, mC.color_name[pco]);
                msg[0]= 0;
            }
            if (strncmp(msg, "<p", 2) == 0) // <pxx> show player (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = mwB.zz[1][9];
                al_draw_bitmap(mwB.player_tile[pco][ans], dx+sx, sy, 0 );
                chop_first_x_char(msg, 5);
                xindent +=24;
            }
            if (strncmp(msg, "<sb", 3) == 0) // <sxxx> show shape left just) // for block tiles instead of tiles
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[3];
               buff2[1] = msg[4];
               buff2[2] = msg[5];
               buff2[3] = 0;
               int ans = atoi(buff2);
               al_draw_bitmap(mwB.btile[ans], dx+sx, sy, 0 );
               chop_first_x_char(msg, 7);
               xindent +=24;
            }
            else if (strncmp(msg, "<s", 2) == 0)   // <sxxx> show shape left just)
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = msg[4];
               buff2[3] = 0;
               int ans = atoi(buff2);
               al_draw_bitmap(mwB.tile[ans], dx+sx, sy, 0 );
               chop_first_x_char(msg, 6);
               xindent +=24;
            }
            if (strncmp(msg, "<ms", 2) == 0) // <msxxxxxxx....> show 20 shapes (left just)
            {
               int z;
               for(z=0; z < 20; z++)
               {
                  buff2[0] = msg[3+z*3];
                  buff2[1] = msg[4+z*3];
                  buff2[2] = msg[5+z*3];
                  buff2[3] = 0;
                  int ans = atoi(buff2);
                  al_draw_bitmap(mwB.btile[ans], dx+sx+(z*20), sy, 0 );
               }
               msg[0]= 0;
            }
            if (strncmp(msg, "<l", 2) == 0) // <lxx> line color xx (left just)
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               chop_first_x_char(msg, 5);
            }

            if (strncmp(msg, "<h", 2) == 0) // <lxx> line color xx (left just) plus 4 for y align
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               chop_first_x_char(msg, 5);
               tay=3;
            }



            if (strncmp(msg, "<c", 2) == 0) // <cxx> line color xx (centered)
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               chop_first_x_char(msg, 5);
               just = 1;
            }

            if (strncmp(msg, "<b", 2) == 0) // <bxx> draw a line in color xx
            {
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               chop_first_x_char(msg, 5);
               al_draw_line(dx+20, sy+16, dx+620, sy+16, mC.pc[color], 2);
               msg[0]= 0;
            }

            // by this point we should have processed and stripped any tag from the beginning of the line
            // if there was a tag we didn't process it will be printed as text
            // now we search for the beginning of another tag "<"
            // if we find one, we chop out the text up to point and print it
            // then we will run this loop again to process the next tag
            // if we don't find another, we will print the whole line


            char txt[200];                        // line to print
            strcpy(txt, msg);                     // by default txt will have all of msg
            nexttag = -1;                         // default if no next tag found


            if ((!processed_tag_this_time_through) && (msg[0] == '<')) // bad tag
               msg[0] = '*';

            int len = strlen(msg);
            if (len>1)
               for (int x=0; x<len; x++)
                  if (msg[x] == '<')
                  {
                     nexttag = x;
                     break;
                  }
            if (nexttag != -1)                    // another '<' was found
            {
               txt[nexttag] = 0;                  // terminate 'txt' with NULL to shorten string
               chop_first_x_char(msg, nexttag);   // remove this from the beginning of 'msg'
            }
            if (just) al_draw_text(mF.pr8, mC.pc[color], dx+320,          24+(c*8), ALLEGRO_ALIGN_CENTER, txt );
            else      al_draw_text(mF.pr8, mC.pc[color], dx+20 + xindent, 24+(c*8)+tay, 0, txt );
            xindent += strlen(txt) * 8;
         } // end of while nexttag != -1
      } // end of cycle lines


      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_ESCAPE][0])  quit = 1;
      if (mI.key[ALLEGRO_KEY_UP][0])   line --;
      if (mI.key[ALLEGRO_KEY_DOWN][0]) line ++;
      if (mI.key[ALLEGRO_KEY_HOME][0]) line = 0;
      if (mI.key[ALLEGRO_KEY_END][0])  line = last_pos;
      if (mI.key[ALLEGRO_KEY_PGUP][3])
      {
         while ((strncmp(help_string[--line], "<section>", 9) != 0) && (line > 0));
      }
      if (mI.key[ALLEGRO_KEY_PGDN][3])
      {
         while ((strncmp(help_string[++line], "<section>", 9) != 0) && (line < num_of_lines - lpp));
      }

      // limits
      if (line < 0)  line = 0;
      if (line > last_pos)  line = last_pos;
      if (got_num) al_draw_textf(mF.pr8, mC.pc[ftc], dx+320, mwD.SCREEN_H-9, ALLEGRO_ALIGN_CENTER, "jump to section %1d_", got_num-48);
      if (mI.key_pressed_ASCII)
      {
          int k = mI.key_pressed_ASCII;
          if ((k>47) && (k<58))   // if 0-9
          {
             if (got_num) // last keypress was num
             {
                int sn = (got_num-48)*10 + (k-48);
                if (sn < num_sections) line = section_first_lines[sn];
                got_num = 0;
             }
             else got_num = k;
          }
          else got_num = 0; // keypressed but not num
      }

      al_flip_display();
      mwPS.frame_num++;
      mwB.update_animation();
      al_rest(0.02);
      while ((mI.key[ALLEGRO_KEY_ESCAPE][0]) || (mI.mouse_b[2][0]))
      {
         quit = 1;
         mwEQ.proc_event_queue();
      }
   }  // end of while not quit
   al_destroy_bitmap(status_window);
   al_destroy_bitmap(selection_window);
   al_destroy_bitmap(hlift);
   mwPS.help_screens_running = 0;
}

void menu_setup(void)
{
   strcpy (lift_step_type_name[0], "BAD!");
   strcpy (lift_step_type_name[1], "Move");
   strcpy (lift_step_type_name[2], "Wait");
   strcpy (lift_step_type_name[3], "Prox");
   strcpy (lift_step_type_name[4], "End ");
   strcpy (lift_step_type_name[5], "Trig");


   strcpy(item_name[0],  "item_empty");
   strcpy(item_name[1],  "Door");
   strcpy(item_name[2],  "Bonus");
   strcpy(item_name[3],  "Exit");
   strcpy(item_name[4],  "Key");
   strcpy(item_name[5],  "Start");
   strcpy(item_name[6],  "Orb");
   strcpy(item_name[7],  "Mine");
   strcpy(item_name[8],  "Bomb");
   strcpy(item_name[9],  "Trigger");
   strcpy(item_name[10], "Message");
   strcpy(item_name[11], "Rocket");
   strcpy(item_name[12], "Warp");
   strcpy(item_name[13], "undef");
   strcpy(item_name[14], "Switch");
   strcpy(item_name[15], "Sproingy");
   strcpy(item_name[16], "Block Manip");
   strcpy(item_name[17], "Block Damage");


   item_tile[0]  = 0;
   item_tile[1]  = 448;
   item_tile[2]  = 304;
   item_tile[3]  = 398;
   item_tile[4]  = 272;
   item_tile[5]  = 383;
   item_tile[6]  = 0;
   item_tile[7]  = 456;
   item_tile[8]  = 538;
   item_tile[9]  = 991;
   item_tile[10] = 256;
   item_tile[11] = 249;
   item_tile[12] = 825;
   item_tile[13] = 0;
   item_tile[14] = 745;
   item_tile[15] = 237;
   item_tile[16] = 989;
   item_tile[17] = 988;
   item_tile[18] = 0;
   item_tile[19] = 0;



   strcpy (enemy_name[0][0],  "empty");
   strcpy (enemy_name[1][0],  "undef");
   strcpy (enemy_name[2][0],  "undef");
   strcpy (enemy_name[3][0],  "ArchWagon");
   strcpy (enemy_name[4][0],  "Bouncer");
   strcpy (enemy_name[5][0],  "JumpWorm");
   strcpy (enemy_name[6][0],  "Cannon");
   strcpy (enemy_name[7][0],  "PodZilla");
   strcpy (enemy_name[8][0],  "TrakBot");
   strcpy (enemy_name[9][0],  "Cloner");
   strcpy (enemy_name[10][0], "undef");
   strcpy (enemy_name[11][0], "Block Walker");
   strcpy (enemy_name[12][0], "Flapper");
   strcpy (enemy_name[13][0], "VinePod");


   strcpy (enemy_name[0][1],  "mpty");
   strcpy (enemy_name[1][1],  "undf");
   strcpy (enemy_name[2][1],  "undf");
   strcpy (enemy_name[3][1],  "arch");
   strcpy (enemy_name[4][1],  "boun");
   strcpy (enemy_name[5][1],  "jump");
   strcpy (enemy_name[6][1],  "cann");
   strcpy (enemy_name[7][1],  "podz");
   strcpy (enemy_name[8][1],  "trak");
   strcpy (enemy_name[9][1],  "clon");
   strcpy (enemy_name[10][1], "undf");
   strcpy (enemy_name[11][1], "blkw");
   strcpy (enemy_name[12][1], "flap");
   strcpy (enemy_name[13][1], "vine");
   strcpy (enemy_name[66][1], "dth2");
   strcpy (enemy_name[99][1], "dth1");



   enemy_tile[3]  = 496;
   enemy_tile[4]  = 508;
   enemy_tile[5]  = 706;
   enemy_tile[6]  = 415;
   enemy_tile[7]  = 374;
   enemy_tile[8]  = 384;
   enemy_tile[9]  = 550;
   enemy_tile[10] = 550;
   enemy_tile[11] = 866;
   enemy_tile[12] = 159;
   enemy_tile[13] = 374;



   strcpy (global_string[2][0], "Level Editor Pop-Up Menu");
   strcpy (global_string[2][1], "------------------------");
   strcpy (global_string[2][2], "Copy ---");
   strcpy (global_string[2][3], "View ---");
   strcpy (global_string[2][4], "Delete ---");
   strcpy (global_string[2][5], "------------------------");
   strcpy (global_string[2][6], "Edit Selection");
   strcpy (global_string[2][7], "Group Edit");
   strcpy (global_string[2][8], "Tile Helper");
   strcpy (global_string[2][9], "Show Status Window");
   strcpy (global_string[2][10], "Show Selection Window");
   strcpy (global_string[2][11], "------");
   strcpy (global_string[2][12],"New Level");
   strcpy (global_string[2][13],"Load Level");
   strcpy (global_string[2][14],"Save Level");
   strcpy (global_string[2][15],"Save and Quit");
   strcpy (global_string[2][16],"Help Screens");
   strcpy (global_string[2][17],"Quit Level Editor");
   strcpy (global_string[2][18],"end");

   sprintf(global_string[6][0],"Lift:0 Step:0");
   sprintf(global_string[6][1],"---------------");
   sprintf(global_string[6][2],"Cancel");
   sprintf(global_string[6][3],"Move Step 0");
   sprintf(global_string[6][4],"Delete Step 0");
   sprintf(global_string[6][5],"Insert Steps");
   sprintf(global_string[6][6],"end");

   strcpy (global_string[7][0], ""); // main menu
   strcpy (global_string[7][1], "");
   strcpy (global_string[7][2], "Start Level (1)");
   strcpy (global_string[7][3], "Start New Game");
   strcpy (global_string[7][4], "Resume Current Game");
   strcpy (global_string[7][5], "Host Network Game");
   strcpy (global_string[7][6], "Join Network Game");
   strcpy (global_string[7][7], "Settings");
   strcpy (global_string[7][8], "Level Editor");
   strcpy (global_string[7][9], "Demo Mode");
   strcpy (global_string[7][10], "Help Screens");
   strcpy (global_string[7][11], "end");
}


int tmenu(int menu_num, int menu_pos, int x1, int y1)  // this menu function does not pass through, it waits for a selection and then exits
{
   int pc = players[active_local_player].color;

   int highlight = menu_pos;
   int selection = 999;
   int c=0, last_list_item = 0;
   int max_strlen = 0;

   // find max string length
   while (strcmp(global_string[menu_num][c],"end") != 0)
   {
      int len = strlen(global_string[menu_num][c]);
      if (len > max_strlen) max_strlen = len;
      c++;
   }
   last_list_item = c;


   int yh = 12; // y height

   int x2 = x1+max_strlen*8+2;
   int y2 = y1+last_list_item*yh-1;

   while (selection == 999)
   {
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);

      // draw menu title
      int mt = strlen(global_string[menu_num][0])*8;
      al_draw_filled_rectangle(x1-2, y1-2, x1+mt+2, y1+yh, mC.pc[pc+128]);
      al_draw_rectangle(       x1-2, y1-2, x1+mt+2, y1+yh, mC.pc[15], 1);
      al_draw_text(mF.pr8, mC.pc[15], x1, y1+1, 0, global_string[menu_num][0]);

      // erase menu background
      al_draw_filled_rectangle(x1-2, y1+12, x2, y2, mC.pc[pc+128+48]);
      al_draw_rectangle(x1-2, y1+12, x2, y2, mC.pc[15], 1);

      // draw the menu items
      for (c=1; c<last_list_item; c++)
      {
         int b = 15+48; // default text color
         int h = 15;    // highlight text color
         al_draw_text(mF.pr8, mC.pc[b], x1, y1+(c*12)+2, 0, global_string[menu_num][c]);
         if (c == highlight)
         {
            al_draw_filled_rectangle(            x1-2, y1+(c*12), x2, y1+(c*12)+11, mC.pc[h+128]);
            al_draw_text(mF.pr8, mC.pc[h], x1,   y1+(c*12)+2, 0, global_string[menu_num][c]);
            al_draw_rectangle(                   x1-2, y1+(c*12), x2, y1+(c*12)+11, mC.pc[h], 1);
         }
      }

      if ( (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1 ) && (mI.mouse_y < y2) )
      {
         highlight = (mI.mouse_y - y1)/yh;
      }
      else highlight = 0;

      if (highlight < 1) highlight = 0;
      if (highlight > last_list_item-1) highlight = 0;


      if (mI.mouse_b[1][0])
      {
         while (mI.mouse_b[1][0]) mwEQ.proc_event_queue();
         selection = highlight;
      }

      if (mI.mouse_b[2][0])
      {
         while (mI.mouse_b[2][0]) mwEQ.proc_event_queue();
         selection = 0;
      }

      if (mI.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
         selection = 0; // default position for back
      }

   }
   return selection;
}


int zmenu(int menu_num, int menu_pos, int y)  // this menu function does not pass through, it waits for a selection and then exits
{
   y+=4;

   mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;

   int highlight = menu_pos;
   int selection = 999;
   int last_list_item;

   int up_held = 0;
   int down_held = 0;
   int left_held = 0;
   int right_held = 0;

   mI.initialize();

   while (selection == 999)
   {
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));


      while (!mwEQ.menu_update) mwEQ.proc_event_queue_menu();
      mwEQ.menu_update = 0;


      frame_and_title(1);
      mwL.mdw_an(mwL.mdw_map_logo_x, mwL.mdw_map_logo_y, mwL.mdw_map_logo_scale);



      int mx = mwD.SCREEN_W/2;
      if (menu_num == 7)
      {
         draw_level(); // only draw map on main menu

         if (resume_allowed) mwDM.demo_mode_enabled = 0;
         if (mwDM.demo_mode_enabled)
         {
            if (--mwDM.demo_mode_countdown_val < 0)
            {
               mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
               mwDM.demo_mode_enabled = 0;
               return 9;
            }
            sprintf(global_string[7][9], "Demo Mode (%d)", mwDM.demo_mode_countdown_val / 100);
         }
         else sprintf(global_string[7][9], "Demo Mode");
      }

      // draw the menu items
      int c = 0;
      while (strcmp(global_string[menu_num][c],"end") != 0)
      {
         int b = 15; // b = players[active_local_player].color;
         if ((!resume_allowed) && (c==4)) b+=80; // dimmer if can't resume

         if (c == highlight)
         {
            int sl = strlen(global_string[menu_num][c]) * 4;
            al_draw_rectangle(mx-sl-2+0.5f, y+(c*10)-1+0.5f, mx+sl+0.5f, y+(c*10)+9+0.5f, mC.pc[b+80], 1);
         }
         al_draw_text(mF.pr8, mC.pc[b], mx, y+(c*10)+1, ALLEGRO_ALIGN_CENTRE, global_string[menu_num][c]);
         c++;
      }
      last_list_item = c-1;


      if (((mI.key[ALLEGRO_KEY_RIGHT][0]) || (players[0].right)) && (right_held == 0))
      {
         right_held = 1;
         selection = highlight + 100;
      }
      if ( (!(mI.key[ALLEGRO_KEY_RIGHT][0])) &&  (!(players[0].right)) )  right_held = 0;
      if (((mI.key[ALLEGRO_KEY_LEFT][0]) || (players[0].left)) && (left_held == 0))
      {
         left_held = 1;
         selection = highlight + 200;
      }
      if ( (!(mI.key[ALLEGRO_KEY_LEFT][0])) &&  (!(players[0].left)) )  left_held = 0;
      if (((mI.key[ALLEGRO_KEY_DOWN][0]) || (players[0].down))  && (down_held == 0))
      {
         if (++highlight > last_list_item) highlight = last_list_item;
         down_held = 1;
         mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
         mwDM.demo_mode_enabled = 0;
      }
      if ( (!(mI.key[ALLEGRO_KEY_DOWN][0])) && (!(players[0].down))) down_held = 0;
      if (((mI.key[ALLEGRO_KEY_UP][0]) || (players[0].up)) && (up_held == 0))
      {
         if (--highlight < 2) highlight = 2;
         up_held = 1;
         mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
         mwDM.demo_mode_enabled = 0;
      }
      if ( (!(mI.key[ALLEGRO_KEY_UP][0])) && (!(players[0].up))) up_held = 0;


      // shortcut keys
      if (mI.key[ALLEGRO_KEY_L][0])                      return 8; // level editor
      if (mI.key[ALLEGRO_KEY_O][0] && mI.SHFT() && mI.CTRL() ) return 7; // settings

      if (mI.key[ALLEGRO_KEY_PGDN][0])   highlight = last_list_item;
      if (mI.key[ALLEGRO_KEY_PGUP][0])   highlight = 2;
      if (mI.key[ALLEGRO_KEY_ENTER][0])  selection = highlight;
      if (mI.key[ALLEGRO_KEY_ESCAPE][0]) selection = 1;
   }
   return selection;
}







// this menu function does not pass through like the next one
// it waits for a selection and then exits
// its is entered with mI.mouse_b[2][0] pressed and exits when released
int pmenu(int menu_num, int bg_color)
{
   int highlight = 2;
   int selection = 999;
   int last_list_item;
   int c, b;

   int kx = mI.mouse_x;
   if (kx < 100) kx = 100;
   if (kx > mwD.SCREEN_W-100) kx = mwD.SCREEN_W-100;

   int up = 0;
   int ky = mI.mouse_y-20;
   if (menu_num == 2) if (ky > mwD.SCREEN_H - 160) up=1; // main editor menu
   if (menu_num == 6) if (ky > mwD.SCREEN_H - 60) up=1;  // lift step menu
   if (menu_num == 5) if (ky > mwD.SCREEN_H - 80) up=1;  // generic menu

   if (!up) // normal version
   {
      do   // until selection is made
      {
         al_rest(0.02);
         int max_strlen = 0;
         int w = 0;
         if (bg_color != 0)
         {
            c = 0;
            // run through the menu to get height and width
            while (strcmp(global_string[menu_num][c],"end") != 0)
            {
               int sl = strlen(global_string[menu_num][c]);
               if (sl > max_strlen) max_strlen = sl;
               c++;
            }
            w = max_strlen*4 + 2;
            al_draw_filled_rectangle(kx-w, ky-2, kx+w, ky+c*8+1, mC.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky-2, kx+w, ky+c*8+1, mC.pc[bg_color], 1);     // frame entire menu
         }
         c = 0;
         while (strcmp(global_string[menu_num][c],"end") != 0)
         {
            b = 9 + 96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;

            int sl = strlen(global_string[menu_num][c]);
            if (sl > max_strlen) max_strlen = sl;
            w = sl*4;

            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky+(c*8), kx+w, ky+(c*8)+8, mC.pc[0]);
            al_draw_text(mF.pr8, mC.pc[b], kx, ky+(c*8),  ALLEGRO_ALIGN_CENTER, global_string[menu_num][c]);
            c++;
         }
         last_list_item = c-1;
         al_flip_display();
         mwEQ.proc_event_queue();
         highlight = 2;
         if ( (mI.mouse_x > (kx - 100)) && (mI.mouse_x < (kx+100)) )
            if ( (mI.mouse_y > ky ) && (mI.mouse_y < ky + ((last_list_item+1)*8)) )
               highlight = (mI.mouse_y-ky) / 8;
         if (!(mI.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   if (up)  // reverse version
   {
      if (menu_num == 5) ky += 18;  // generic menu
      else ky = mI.mouse_y+12; // to put mouse on default button
      if (ky > mwD.SCREEN_H) ky = mwD.SCREEN_H;
      do   // until selection is made
      {
         al_rest(0.02);
         int max_strlen = 0;
         int w = 0;
         if (bg_color != 0)
         {
            c = 0;
            // run through the menu to get height and width
            while (strcmp(global_string[menu_num][c],"end") != 0)
            {
               int sl = strlen(global_string[menu_num][c]);
               if (sl > max_strlen) max_strlen = sl;
               c++;
            }
            w = max_strlen*4 + 2;
            al_draw_filled_rectangle(kx-w, ky+9, kx+w, ky-c*8+6, mC.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky+9, kx+w, ky-c*8+6, mC.pc[bg_color], 1);     // frame entire menu
         }

         c = 0;
         while (strcmp(global_string[menu_num][c],"end") != 0)
         {
            b = 9+96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int w = strlen(global_string[menu_num][c])*4;
            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky-(c*8), kx+w, ky-(c*8)+8, mC.pc[0]);
            al_draw_text(mF.pr8, mC.pc[b], kx, ky-(c*8),  ALLEGRO_ALIGN_CENTER, global_string[menu_num][c]);
            c++;
         }
         last_list_item = c-1;

         al_flip_display();
         mwEQ.proc_event_queue();

         //show_mouse(screen);
         highlight = 2;
         if ( (mI.mouse_x > (kx - 100)) && (mI.mouse_x < (kx+100)) )
            if ( (mI.mouse_y < ky ) && (mI.mouse_y > ky - ((last_list_item+1)*8) ) )
               highlight = (ky-mI.mouse_y+8) / 8;
         if (!(mI.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   //show_mouse(NULL);
   return selection;
}





// edit text stuff




void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore)
{
   int len = strlen(f);
   char dt[40][120];
   int row=0, col=0, cursor_row=0, cursor_col=0;
   // get cursor row and column and fill dt
   for (int a=0; a<len+1; a++)
   {
      if (a == cursor_pos)
      {
         cursor_row = row;
         cursor_col = col;
      }
      if (f[a] == 126) // line break
      {
         row++;
         col=0;
         dt[row][col] = (char)NULL; // in case len = 0
      }
      else  // regular char
      {
         dt[row][col] = f[a];
         col++;
         dt[row][col] = (char)NULL;
      }
   }

   // make a string from the cursor text char
   msg[0] = f[cursor_pos];
   msg[1] = 0;

   int x, y;
   x = cursor_col*8+xpos_c - strlen(dt[cursor_row])*4;
   y = ypos+cursor_row*8;

   if (restore) // black background, text color text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[0]);
      al_draw_text(mF.pr8, mC.pc[cursor_color], x, y, 0, msg);
   }
   else // red background, black text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[10]);
      al_draw_text(mF.pr8, mC.pc[0], x, y, 0, msg);
   }
}


int edit_pmsg_text(int c, int new_msg)
{
   int bts = 16;
   int char_count;
   int cursor_pos=0;
   int blink_count = 8;
   int blink_counter = 0;
   char f[1800];
   int quit = 0;

   int xa = mwWM.mW[7].x1;
   int xb = mwWM.mW[7].x2;

   int smx = (xa+xb)/2;  // x center
   int smy = mwWM.mW[7].pop_msg_viewer_pos;

   int bad=0;

   if (new_msg)
   {
      smy = 200;
      f[0] = (char)NULL;
      f[1] = (char)NULL;
      char_count = 1;
   }
   else
   {
      strcpy(f, pmsgtext[c]);
      char_count = strlen(f);
   }

   while (!quit)
   {
      al_set_target_backbuffer(display);

      if (++blink_counter > blink_count) blink_counter = 0;
      if (blink_counter > 4) draw_pop_message(c, 1, smx, smy, cursor_pos, 1, f); // show the message with cursor_pos
      else                   draw_pop_message(c, 1, smx, smy, cursor_pos, 0, f); // show the message without

      int by = smy-bts/2-2;
      int ey = by+-3*bts; // erase y1
      int by1 = ey;

      mdw_buttont(    xa, by1, xb, bts, 0,0,0,0,  0,15,13,0, 1,0,1,1, "Edit Message"); // display text only
      if (mdw_buttont(xa, by1, xb, bts, 0,0,0,0,  0,11,15,0, 1,0,1,0, "OK"))     { quit = 1; bad = 0; }
      if (mdw_buttont(xa, by1, xb, bts, 0,0,0,0,  0,10,15,0, 1,0,1,0, "Cancel")) { quit = 1; bad = 1; }


//      al_draw_filled_rectangle(xa, by1-30, xb, by1-10, mC.pc[0]);
//      al_draw_rectangle(xa, by1-30, xb, by1-10, mC.pc[15], 1);
//      al_draw_textf(mF.pr8, mC.pc[15], xa, by1-20, 0, "%d/%d %d", cursor_pos, char_count, strlen(f));

      mwEQ.proc_event_queue();

      if (mI.key[ALLEGRO_KEY_HOME][0])  cursor_pos = 0;
      if (mI.key[ALLEGRO_KEY_END][0])   cursor_pos = char_count;
      if (mI.key[ALLEGRO_KEY_RIGHT][3])
      {
         if (mI.SHFT() && mI.CTRL())
         {
            while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         }
         else if (mI.SHFT())
         {
            while ((++cursor_pos < char_count) && (f[cursor_pos] != 32) && (f[cursor_pos] != 10));  // find next space or LF
         }
         else if (mI.CTRL()) cursor_pos+=16;

         else cursor_pos++;
         if (cursor_pos > char_count) cursor_pos = char_count;             // make sure we are not past the end
      }

      if (mI.key[ALLEGRO_KEY_LEFT][3])
      {
         //al_rest(0.02);
         while (mI.key[ALLEGRO_KEY_LEFT][3]) mwEQ.proc_event_queue();               // wait for release

         if ((mI.SHFT()) && (mI.CTRL()))
         {
            while ((--cursor_pos > 0) && (f[cursor_pos] != 10));  // find next LF
         }
         else if (mI.SHFT())
         {
            while ((--cursor_pos > 0) && (f[cursor_pos] != 32) && (f[cursor_pos] != 10)); // find next space or LF
         }
         else if (mI.CTRL()) cursor_pos-=16;

         else cursor_pos--;
         if (cursor_pos < 0) cursor_pos = 0;                             // make sure we are not before the start
      }


      if (mI.key[ALLEGRO_KEY_UP][3])                                           // move up one line
      {
         int ocp = cursor_pos;                                           // get original position
         while ((--cursor_pos > 0) && (f[cursor_pos] != 10));           // find previous LF
         int mv = cursor_pos - ocp;                                      // how far did we move?
         while ((--cursor_pos > 0) && (f[cursor_pos] != 10));           // find previous LF
         cursor_pos -= mv;                                               // subtract move
         if (cursor_pos < 0) cursor_pos = 0;                             // make sure we are not before the start
      }
      if (mI.key[ALLEGRO_KEY_DOWN][3])                                         // move down one line
      {
         int ocp = cursor_pos;                                           // get original position
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         int mv = cursor_pos - ocp;                                      // how far did we move?
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         cursor_pos -= mv;                                               // subtract move
         if (cursor_pos > char_count) cursor_pos = char_count;           // make sure we are not past the end
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         al_rest(0.02);
         for (int aa = cursor_pos; aa < char_count; aa++) f[aa]=f[aa+1];
         char_count--;
         f[char_count] = (char)NULL; // set last to NULL
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         al_rest(0.02);
         cursor_pos--;
         for (int aa = cursor_pos; aa < char_count; aa++) f[aa]=f[aa+1];
         char_count--;
         f[char_count] = (char)NULL; // set last to NULL
      }
      if (mI.key_pressed_ASCII)
      {
         al_rest(0.07);
         int k = mI.key_pressed_ASCII;
         if (k == 13) k = 10; // replace CR with LF

         if ( (k == 10) || ((k>31) && (k<127))) // if alphanumeric
         {
            for (int aa = char_count; aa>=cursor_pos; aa--) f[aa+1]=f[aa]; // move over to make room
            f[cursor_pos] = k; // set char
            cursor_pos++;      // inc both
            char_count++;
            f[char_count] = (char)NULL; // set last to NULL
         }
      }
      if (mI.key[ALLEGRO_KEY_ESCAPE][3])
      {
         quit = 1;
         bad = 1;
      }
      al_flip_display();
      al_draw_filled_rectangle(xa, ey, xb, ey+10*bts, mC.pc[0]);
      al_rest(0.07);

   } // end of while (!quit)

   if (bad) return 0;
   else
   {
      strcpy(pmsgtext[c], f);
      return 1;
   }
}



void edit_server_name(int type, int x, int y)
{
   char fst[80];
   strcpy(fst, m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (mI.key[ALLEGRO_KEY_ENTER][0]) mwEQ.proc_event_queue();
   int quit = 0;
   while (!quit)
   {
      int tx = mwD.SCREEN_W/2;
      int ty1 = mwD.SCREEN_H/2;
      int w = (char_count+1)*4;

      if (type == 1)
      {
         tx = x;
         ty1 = y+20;

      }

      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty1-4-2, tx+w+18, ty1+4+3, mC.pc[0]);

      al_draw_text(mF.pr8, mC.pc[15], tx, ty1-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty1-4-1, tx+w+6, ty1+6, mC.pc[15], 1);

      rtextout_centre(mF.pr8, NULL, fst, tx, ty1+1, 15, 1, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1-3, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1-3, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1-3, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (int a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         fst[char_count] = (char)NULL; // set last to NULL
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3])
      {
         strcpy(m_serveraddress, fst);
         quit = 1;
      }

      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   }
}


int edit_lift_name(int lift, int y1, int x1, char *fst)
{
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a=0;
   int char_count = strlen(fst);
   while (1)
   {
      al_flip_display();
      al_rest(0.05);


      int x2 = x1 + (lifts[lift].w) -1;
      int y2 = y1 + (lifts[lift].h) -1;
      int tx = ((x1+x2)/2);
      int ty1 = ((y1+y2)/2) - 3;

      //int color = lifts[lift].color;

      int color = (lift_steps[lift][0].type >> 28) & 15;


      // draw updated lift
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[color + ((9 - a)*16)], 2 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mC.pc[color] );
      al_draw_text(mF.pr8, mC.pc[color+160], tx, ty1, ALLEGRO_ALIGN_CENTRE, fst);


      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3]) return 1;
      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) return 0;
   }
}




















