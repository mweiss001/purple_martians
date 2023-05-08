// mwHelp.cpp

#include "pm.h"
#include "mwHelp.h"
#include "mwMiscFnx.h"
#include "mwLogo.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLoop.h"
#include "mwScreen.h"


mwHelp mHelp;

int mwHelp::load_help(void)
{
   char msg[1024];
   char filename[100] = "help/pmhelp.txt";
   FILE *filepntr;
   char buff[200];
   int line=0;
   int ch=0;

   if ((al_filename_exists(filename)) == 0)
   {
      sprintf(msg, "Failed to load help file from %s\n", filename);
      mInput.m_err(msg);
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

void mwHelp::help(const char *topic)
{
   //if (mDisplay.SCREEN_H < 480) return;       // wont work with SCREEN_H < 480.. it will work, just won't look good..
   mLoop.help_screens_running = 1;
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
         hlift = al_load_bitmap("help/Lift.bmp");
         al_destroy_bitmap(status_window);
         status_window = al_load_bitmap("help/status_window.bmp");
         al_destroy_bitmap(selection_window);
         selection_window = al_load_bitmap("help/selection_window.bmp");
      }

      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));


      int line_height = 8;

      int dx = mDisplay.SCREEN_W/2 - 320;
      int lpp = (mDisplay.SCREEN_H - 40) / line_height;   // lines per page

      last_pos = num_of_lines - lpp - 2;

      for (int x=0; x<16; x++)
         al_draw_rectangle(dx+x, x, dx+639-x, mDisplay.SCREEN_H-1-x, mColor.pc[fc+(x*16)], 1);

      al_draw_text(mFont.pr8, mColor.pc[ftc], dx+320,                                       2, ALLEGRO_ALIGN_CENTRE, "Purple Martians Help");
      al_draw_text(mFont.pr8, mColor.pc[ftc], dx+16,                                        2, 0,                    "<UP><DOWN>");
      al_draw_text(mFont.pr8, mColor.pc[ftc], dx+640-(11*line_height)-16,                   2, 0,                    "<ESC>-quits");
      al_draw_text(mFont.pr8, mColor.pc[ftc], dx+16,                      mDisplay.SCREEN_H-9, 0,                    "<PAGEUP><PAGEDOWN>");
      al_draw_text(mFont.pr8, mColor.pc[ftc], dx+640-(11*line_height)-16, mDisplay.SCREEN_H-9, 0,                    "<HOME><END>");


      int sy = 12-line_height; // running ypos initial val

      for (int c=0; c<lpp; c++) // cycle lines
      {
         int xindent = 0;                         // start at xindent = 0
         int just = 0;                            // default just = left
         int color = 15;                          // default regular color unless changed
         sprintf(msg, "%s", help_string[line+c]); // put line to process in msg

         int processed_tag_this_time_through;
         int nexttag = 0;
         while (nexttag != -1)
         {
            processed_tag_this_time_through = 0;

            int tay = 0; // text align for lining up with bitmaps

            sy += line_height; // running ypos

            int sy = 12 + (c*line_height);    // shape draw y position
            int sx = 20 + xindent;  // shape draw x pos left just
            int sxc = (640/2) - 10; // shape draw x pos centered

            if (strncmp(msg, "<title>", 7) == 0) // show title
            {
                mScreen.draw_title(dx+sxc+10, sy+8, 360, 48, 8);
                msg[0]= 0;
            }
            if (strncmp(msg, "<section>", 9) == 0) // <section> "txt" - make a new section with "txt"
            {
               mMiscFnx.chop_first_x_char(msg, 9);
               for (int i=0; i<16; i++)
               {
                  al_draw_line(dx+i, sy+16+i, dx+639-i, sy+16+i, mColor.pc[sc+(i*16)], 1 );
                  al_draw_line(dx+i, sy+16-i, dx+639-i, sy+16-i, mColor.pc[sc+(i*16)], 1 );
               }
               al_draw_text(mFont.pr8, mColor.pc[stc], dx+320, 12+sy,  ALLEGRO_ALIGN_CENTER, msg);
               msg[0]= 0;
            }
            if (strncmp(msg, "<end of file>", 13) == 0)
            {
//               for (int i=0; i<16; i++)
//               {
//                  al_draw_line(dx+i, sy+16+i, dx+639-i, sy+16+i, mColor.pc[sc+(i*16)], 1 );
//                  al_draw_line(dx+i, sy+16-i, dx+639-i, sy+16-i, mColor.pc[sc+(i*16)], 1 );
//               }
//               al_draw_text(mFont.pr8, mColor.pc[stc], dx+320, 12+sy,  ALLEGRO_ALIGN_CENTER, msg);
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
               float sc = .25;
               int xo = (int)(200 * sc)+16;
               mLogo.mdw_an(dx+320 + sxc-xo, sy+50, sc);
               mLogo.mdw_an(dx+320 - sxc+xo, sy+50, sc);
               msg[0]= 0;
            }
            if (strncmp(msg, "<ac", 3) == 0) // <acxxx> show animation sequence (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = msg[5];
                buff2[3] = 0;
                int ans = mBitmap.zz[0][atoi(buff2)];
                al_draw_bitmap(mBitmap.tile[ans], dx+sxc, sy, 0 );
                msg[0]= 0;
            }

            if (strncmp(msg, "<ab", 3) == 0) // <axx> show animation sequence (left just) // for block tiles instead of tiles
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = 0;
                //printf("s:'%s' i:%d\n", buff2, atoi(buff2));
                int ans = mBitmap.zz[0][atoi(buff2)];
                al_draw_bitmap(mBitmap.btile[ans], dx+sx, sy, 0 );
                mMiscFnx.chop_first_x_char(msg, 6);
                xindent +=24;
            }
            else if (strncmp(msg, "<a", 2) == 0) // <axx> show animation sequence (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int ans = mBitmap.zz[0][atoi(buff2)];
                al_draw_bitmap(mBitmap.tile[ans], dx+sx, sy, 0 );

                mMiscFnx.chop_first_x_char(msg, 5);
                xindent +=24;
            }
            if (strncmp(msg, "<hb", 3) == 0) // <hb> show centered health bar
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = msg[5];
                buff2[3] = 0;
                int health = atoi(buff2);
                mScreen.draw_percent_bar(dx+sxc+10, sy, 88, 10, health);
                al_draw_textf(mFont.pr8, mColor.pc[14], dx+sxc+10, sy+2, ALLEGRO_ALIGN_CENTER, "Health:%-2d", health);
                msg[0]= 0;
            }
            if (strncmp(msg, "<pc", 3) == 0) // <pcxx> show player and color text (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = mBitmap.zz[1][9];
                al_draw_bitmap(mBitmap.player_tile[pco][ans], dx+sxc-40, sy, 0 );
                al_draw_text(mFont.pr8, mColor.pc[pco], dx+sxc+24-40, sy+7, 0, mColor.color_name[pco]);
                msg[0]= 0;
            }
            if (strncmp(msg, "<p", 2) == 0) // <pxx> show player (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = mBitmap.zz[1][9];
                al_draw_bitmap(mBitmap.player_tile[pco][ans], dx+sx, sy, 0 );
                mMiscFnx.chop_first_x_char(msg, 5);
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
               al_draw_bitmap(mBitmap.btile[ans], dx+sx, sy, 0 );
               mMiscFnx.chop_first_x_char(msg, 7);
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
               al_draw_bitmap(mBitmap.tile[ans], dx+sx, sy, 0 );
               mMiscFnx.chop_first_x_char(msg, 6);
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
                  al_draw_bitmap(mBitmap.btile[ans], dx+sx+(z*20), sy, 0 );
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
               mMiscFnx.chop_first_x_char(msg, 5);
            }
            if (strncmp(msg, "<h", 2) == 0) // <lxx> line color xx (left just) plus 4 for y align
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               mMiscFnx.chop_first_x_char(msg, 5);
               tay=3;
            }
            if (strncmp(msg, "<c", 2) == 0) // <cxx> line color xx (centered)
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               mMiscFnx.chop_first_x_char(msg, 5);
               just = 1;
            }
            if (strncmp(msg, "<b", 2) == 0) // <bxx> draw a line in color xx
            {
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = 0;
               color = atoi(buff2);
               mMiscFnx.chop_first_x_char(msg, 5);
               al_draw_line(dx+20, sy+16, dx+620, sy+16, mColor.pc[color], 1);
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
               mMiscFnx.chop_first_x_char(msg, nexttag);   // remove this from the beginning of 'msg'
            }
            if (just) al_draw_text(mFont.pr8, mColor.pc[color], dx+320,          12+sy,     ALLEGRO_ALIGN_CENTER, txt );
            else      al_draw_text(mFont.pr8, mColor.pc[color], dx+20 + xindent, 12+sy+tay,                    0, txt );

            xindent += strlen(txt) * 8;
         } // end of while nexttag != -1
      } // end of cycle lines


      mEventQueue.proc();

      line -= mInput.mouse_dz * 4;
      mInput.mouse_dz = 0;

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])  quit = 1;
      if (mInput.key[ALLEGRO_KEY_UP][0])   line --;
      if (mInput.key[ALLEGRO_KEY_DOWN][0]) line ++;
      if (mInput.key[ALLEGRO_KEY_HOME][0]) line = 0;
      if (mInput.key[ALLEGRO_KEY_END][0])  line = last_pos;
      if (mInput.key[ALLEGRO_KEY_PGUP][3])
      {
         while ((strncmp(help_string[--line], "<section>", 9) != 0) && (line > 0));
      }
      if (mInput.key[ALLEGRO_KEY_PGDN][3])
      {
         while ((strncmp(help_string[++line], "<section>", 9) != 0) && (line < num_of_lines - lpp));
      }

      // limits
      if (line < 0)  line = 0;
      if (line > last_pos)  line = last_pos;
      if (got_num) al_draw_textf(mFont.pr8, mColor.pc[ftc], dx+320, mDisplay.SCREEN_H-9, ALLEGRO_ALIGN_CENTER, "jump to section %1d_", got_num-48);
      if (mInput.key_pressed_ASCII)
      {
          int k = mInput.key_pressed_ASCII;
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
      mLoop.frame_num++;
      mBitmap.update_animation();
      al_rest(0.02);
      while ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.mouse_b[2][0]))
      {
         quit = 1;
         mEventQueue.proc();
      }
   }  // end of while not quit
   al_destroy_bitmap(status_window);
   al_destroy_bitmap(selection_window);
   al_destroy_bitmap(hlift);
   mLoop.help_screens_running = 0;
}
