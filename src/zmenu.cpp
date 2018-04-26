#include "pm.h"


char help_string[5000][200];


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




void help(char *topic)
{
//   if (SCREEN_H < 480) return;       // wont work with SCREEN_H < 480


   int num_of_lines = load_help();
   char section_names[60][80];
   int section_first_lines[60];
   int num_sections;
   char buff2[200];
   char msg[200];

   Redraw = 1;
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

      if (Redraw)
      {
         Redraw = 0;
         al_destroy_bitmap(hlift);
         hlift = al_load_bitmap("help/lift.bmp");
         al_destroy_bitmap(status_window);
         status_window = al_load_bitmap("help/status_window.bmp");
         al_destroy_bitmap(selection_window);
         selection_window = al_load_bitmap("help/selection_window.bmp");
      }

      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0,0,0));

      int dx = SCREEN_W/2 - 320;
      //int xo  = (SCREEN_W - 640) / 2;   // x offset (distance from left screen edge to start of help screen)
      int lpp = (SCREEN_H - 40)  / 8;   // lines per page

      last_pos = num_of_lines - lpp - 2;

      for (int x=0; x<16; x++)
         al_draw_rectangle(dx+x, x, dx+639-x, SCREEN_H-1-x, palette_color[fc+(x*16)], 1);

      al_draw_text(font, palette_color[ftc], dx+320,                    2, ALLEGRO_ALIGN_CENTRE, "Purple Martians Help");
      al_draw_text(font, palette_color[ftc], dx+16,                     2, 0, "<UP><DOWN>");
      al_draw_text(font, palette_color[ftc], dx+640-(11*8)-16,          2, 0, "<ESC>-quits");
      al_draw_text(font, palette_color[ftc], dx+16,            SCREEN_H-9, 0, "<PAGEUP><PAGEDOWN>");
      al_draw_text(font, palette_color[ftc], dx+640-(11*8)-16, SCREEN_H-9, 0, "<HOME><END>");

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

            int sy = 12 + (c*8);    // shape draw y position
            int sx = 20 + xindent;  // shape draw x pos left just
            int sxc = (640/2) - 10; // shape draw x pos centered

            if (strncmp(msg, "<title>", 7) == 0) // show title
            {
                al_set_clipping_rectangle((dx+12)*les, (12*les), (639-12*2)*les, (SCREEN_H-12*2)*les);
                draw_title(dx+sxc+10, sy-108, 360, 64, 8);
                msg[0]= 0;
                al_reset_clipping_rectangle();
           }
            if (strncmp(msg, "<section>", 9) == 0) // <section> "txt" - make a new section with "txt"
            {
               chop_first_x_char(msg, 9);
               for (int x=0;x<16;x++)
               {
                  al_draw_line(dx+x, 28+(c*8)+x, dx+639-x, 28+(c*8)+x, palette_color[sc+(x*16)], 1 );
                  al_draw_line(dx+x, 28+(c*8)-x, dx+639-x, 28+(c*8)-x, palette_color[sc+(x*16)], 1 );
               }
               al_draw_text(font, palette_color[stc], dx+320, 24+(c*8),  ALLEGRO_ALIGN_CENTER, msg);
               msg[0]= 0;
            }
            if (strncmp(msg, "<end of file>", 13) == 0)
            {
               for (int x=0;x<16;x++)
               {
                  al_draw_line(dx+x, 28+(c*8)+x, dx+639-x, 28+(c*8)+x, palette_color[sc+(x*16)], 1 );
                  al_draw_line(dx+x, 28+(c*8)-x, dx+639-x, 28+(c*8)-x, palette_color[sc+(x*16)], 1 );
               }
               al_draw_text(font, palette_color[stc], dx+320, 24+(c*8),  ALLEGRO_ALIGN_CENTER, msg);
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

            if (strncmp(msg, "<mdw>", 5) == 0) // show mdw logo
            {

               float sc = .5;

               int xo = (int)(200 * sc)+16;

               void mdw_an3(int x, int y, float sc, int th);
               mdw_an3(dx+320 + sxc-xo, sy+xo+16, sc, 3);
               mdw_an3(dx+320 - sxc+xo, sy+xo+16, sc, 3);

               mdw_an3(dx+sxc, sy+50, .1, 1);
               mdw_an3(dx+sxc, sy+200, .2, 1);

               float bs = (float)640 / (float)560;


               mdw_an3(dx+sxc, sy+500, bs, 5);

               msg[0]= 0;
            }
            if (strncmp(msg, "<mdw1>", 6) == 0) // show mdw logo
            {
               al_set_clipping_rectangle((dx+12)*les, (12*les), (639-12*2)*les, (SCREEN_H-12*2)*les);
               float sc = .25;
               int xo = (int)(200 * sc)+16;
               void mdw_an3(int x, int y, float sc, int th);
               mdw_an3(dx+320 + sxc-xo, sy+xo+16-140, sc, 2);
               mdw_an3(dx+320 - sxc+xo, sy+xo+16-140, sc, 2);
               msg[0]= 0;
               al_reset_clipping_rectangle();
            }
            if (strncmp(msg, "<ac", 3) == 0) // <acxxx> show animation sequence (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = msg[5];
                buff2[3] = 0;
                int ans = zz[0][atoi(buff2)];
                al_draw_bitmap(memory_bitmap[ans], dx+sxc, sy, 0 );
                msg[0]= 0;
            }
            if (strncmp(msg, "<a", 2) == 0) // <axx> show animation sequence (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int ans = zz[0][atoi(buff2)];
                al_draw_bitmap(memory_bitmap[ans], dx+sx, sy, 0 );

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
                al_draw_textf(font, palette_color[14], dx+sxc+10, sy+2, ALLEGRO_ALIGN_CENTER, "Health:%-2d", health);
                msg[0]= 0;
            }
            if (strncmp(msg, "<pc", 3) == 0) // <pcxx> show player and color text (centered)
            {
                buff2[0] = msg[3];
                buff2[1] = msg[4];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = zz[1][9];
                al_draw_bitmap(player_bitmap[pco-1][ans], dx+sxc-40, sy, 0 );
                al_draw_text(font, palette_color[pco], dx+sxc+24-40, sy+6, 0, color_name[pco]);
                msg[0]= 0;
            }
            if (strncmp(msg, "<p", 2) == 0) // <pxx> show player (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int pco = atoi(buff2);
                int ans = zz[1][9];
                al_draw_bitmap(player_bitmap[pco-1][ans], dx+sx, sy, 0 );
                chop_first_x_char(msg, 5);
                xindent +=24;
            }
            if (strncmp(msg, "<s", 2) == 0) // <sxxx> show shape left just)
            {
               processed_tag_this_time_through = 1;
               buff2[0] = msg[2];
               buff2[1] = msg[3];
               buff2[2] = msg[4];
               buff2[3] = 0;
               int ans = atoi(buff2);
               al_draw_bitmap(memory_bitmap[ans], dx+sx, sy, 0 );
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
                  al_draw_bitmap(memory_bitmap[ans], dx+sx+(z*20), sy, 0 );
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
               al_draw_line(dx+20, sy+16, dx+620, sy+16, palette_color[color], 2);
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
            if (just) al_draw_text(font, palette_color[color], dx+320,          24+(c*8), ALLEGRO_ALIGN_CENTER, txt );
            else      al_draw_text(font, palette_color[color], dx+20 + xindent, 24+(c*8), 0, txt );
            xindent += strlen(txt) * 8;
         } // end of while nexttag != -1
      } // end of cycle lines


      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE])  quit = 1;
      if (key[ALLEGRO_KEY_UP])   line --;
      if (key[ALLEGRO_KEY_DOWN]) line ++;
      if (key[ALLEGRO_KEY_HOME]) line = 0;
      if (key[ALLEGRO_KEY_END])  line = last_pos;
      if (key[ALLEGRO_KEY_PGUP])
      {
         while (key[ALLEGRO_KEY_PGUP]) proc_controllers();
         while ((strncmp(help_string[--line], "<section>", 9) != 0) && (line > 0));
      }
      if (key[ALLEGRO_KEY_PGDN])
      {
         while (key[ALLEGRO_KEY_PGDN]) proc_controllers();
         while ((strncmp(help_string[++line], "<section>", 9) != 0) && (line < num_of_lines - lpp));
      }

      // limits
      if (line < 0)  line = 0;
      if (line > last_pos)  line = last_pos;
      if (got_num) al_draw_textf(font, palette_color[ftc], dx+320, SCREEN_H-9, ALLEGRO_ALIGN_CENTER, "jump to section %1d_", got_num-27);

      if (Key_pressed_ASCII)
      {
          int k = Key_pressed_ASCII;
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
      update_animation();
      al_rest(0.02);
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         quit = 1;
         proc_controllers();
      }
   }  // end of while not quit
   al_destroy_bitmap(status_window);
   al_destroy_bitmap(selection_window);
   al_destroy_bitmap(hlift);
}


int zmenu(int menu_num, int menu_pos, int y)  // this menu function does not pass through, it waits for a selection and then exits
{
   y+=4;

   demo_mode_countdown = 800;

   int highlight = menu_pos;
   int selection = 999;
   int last_list_item;

   int up_held = 0;
   int down_held = 0;
   int left_held = 0;
   int right_held = 0;

   // clear_key array (but not SHIFTS, CTRL etc above 200)
   for (int k = ALLEGRO_KEY_A; k < 200; k++) key[k] = 0;

   while (selection == 999)
   {
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));


      if (menu_num == 3)
      {
         frame_and_title(0);

      }
      else
      {
         frame_and_title(1);
         mdw_an();
      }

      int mx = SCREEN_W/2;
      if (menu_num == 7)
      {
         draw_level(); // only draw map on main menu
         if (--demo_mode_countdown < 0)
         {
            demo_mode_countdown = 800;
            return 9;
         }
         sprintf(global_string[7][9], "Demo Mode (%d)", demo_mode_countdown / 80);
      }

      // draw the menu items
      int c = 0;
      while (strcmp(global_string[menu_num][c],"end") != 0)
      {

         //ALLEGRO_COLOR reg = al_map_rgb(0,255,0);
         //ALLEGRO_COLOR dim = al_map_rgb(0,200,0);
         //int b = players[active_local_player].color;
         int b = 15;
         if ((!resume_allowed) && (menu_num == 7) && (c==4)) b+=80; // dimmer if can't resume

         al_draw_text(font, palette_color[b+48], mx, y+(c*10)+1, ALLEGRO_ALIGN_CENTRE, global_string[menu_num][c]);

         if (c == highlight)
            al_draw_text(font, palette_color[b], mx, y+(c*10)+1, ALLEGRO_ALIGN_CENTRE, global_string[menu_num][c]);

         if (c == highlight)
         {
            int sl = strlen(global_string[menu_num][c]) * 4;
            al_draw_rectangle(mx-sl-2+0.5f, y+(c*10)-1+0.5f, mx+sl+0.5f, y+(c*10)+9+0.5f, palette_color[b+80], 1);
         }
         c++;
      }
      last_list_item = c-1;


      if (proc_controllers()) demo_mode_countdown = 800;

      // shortcut key for level editor
      //if (menu_num == 7) if (key[ALLEGRO_KEY_L]) return 9;

      if (((key[ALLEGRO_KEY_RIGHT]) || (players[0].right)) && (right_held == 0))
      {
         right_held = 1;
         selection = highlight + 100;
      }
      if ( (!(key[ALLEGRO_KEY_RIGHT])) &&  (!(players[0].right)) )  right_held = 0;

      if (((key[ALLEGRO_KEY_LEFT]) || (players[0].left)) && (left_held == 0))
      {
         left_held = 1;
         selection = highlight + 200;
      }
      if ( (!(key[ALLEGRO_KEY_LEFT])) &&  (!(players[0].left)) )  left_held = 0;

      if (((key[ALLEGRO_KEY_DOWN]) || (players[0].down))  && (down_held == 0))
      {
         if (++highlight > last_list_item) highlight = last_list_item;
         down_held = 1;
         demo_mode_countdown = 800;
      }
      if ( (!(key[ALLEGRO_KEY_DOWN])) && (!(players[0].down))) down_held = 0;

      if (((key[ALLEGRO_KEY_UP]) || (players[0].up)) && (up_held == 0))
      {
         if (--highlight < 2) highlight = 2;
         up_held = 1;
         demo_mode_countdown = 800;
      }
      if ( (!(key[ALLEGRO_KEY_UP])) && (!(players[0].up))) up_held = 0;

      if (key[ALLEGRO_KEY_ENTER]) selection = highlight;

      if (key[ALLEGRO_KEY_ESCAPE])
      {
         selection = 2; // default position for back
         if (menu_num == 7)  selection = 1; // for top menu only
      }
   }
   return selection;
}

void menu_setup(void)
{
   strcpy (lift_step_type_name[0], "illegal!");
   strcpy (lift_step_type_name[1], "Move");
   strcpy (lift_step_type_name[2], "Wait");
   strcpy (lift_step_type_name[3], "Prox");
   strcpy (lift_step_type_name[4], "Loop");

   strcpy (enemy_name[3], "ArchWagon");
   strcpy (enemy_name[4], "Bouncer");
   strcpy (enemy_name[6], "Cannon");
   strcpy (enemy_name[7], "PodZilla ");
   strcpy (enemy_name[8], "TrakBot");
   strcpy (enemy_name[9], "Cloner");
   strcpy (enemy_name[11], "Block Walker");
   strcpy (enemy_name[12], "Flapper");

   strcpy (color_name[0], "Zombie");
   strcpy (color_name[1], "Violet");
   strcpy (color_name[2], "Mauve");
   strcpy (color_name[3], "Bluey");
   strcpy (color_name[4], "Reddy");
   strcpy (color_name[5], "Pink");
   strcpy (color_name[6], "Taan");
   strcpy (color_name[7], "Orange");
   strcpy (color_name[8], "Purple");
   strcpy (color_name[9], "Forest");
   strcpy (color_name[10], "Red");
   strcpy (color_name[11], "Green");
   strcpy (color_name[12], "Blue");
   strcpy (color_name[13], "Aqua");
   strcpy (color_name[14], "Yellow");
   strcpy (color_name[15], "White");


   strcpy (global_string[3][0], "Logging Options Menu");
   strcpy (global_string[3][1], "--------------------");
   strcpy (global_string[3][2], "Back to Game Menu");
   strcpy (global_string[3][3], "ALL ON");
   strcpy (global_string[3][4], "ALL OFF");
   strcpy (global_string[3][5], "LOGGING");
   strcpy (global_string[3][6], "LOGGING_NETPLAY");
   strcpy (global_string[3][7], "LOGGING_NETPLAY_JOIN");
   strcpy (global_string[3][8], "LOGGING_NETPLAY_bandwidth");
   strcpy (global_string[3][9], "LOGGING_NETPLAY_client_timer_adjust");
   strcpy (global_string[3][10], "LOGGING_NETPLAY_cdat");
   strcpy (global_string[3][11], "LOGGING_NETPLAY_game_move");
   strcpy (global_string[3][12], "LOGGING_NETPLAY_sdat");
   strcpy (global_string[3][13], "LOGGING_NETPLAY_sdak");
   strcpy (global_string[3][14], "LOGGING_NETPLAY_chdf");
   strcpy (global_string[3][15], "LOGGING_NETPLAY_chdf_all_packets");
   strcpy (global_string[3][16], "LOGGING_NETPLAY_chdf_when_to_apply");
   strcpy (global_string[3][17], "LOGGING_NETPLAY_show_dif1");
   strcpy (global_string[3][18], "LOGGING_NETPLAY_show_dif2");
   strcpy (global_string[3][19], "Auto Save Game On Level Done");
   strcpy (global_string[3][20], "Auto Save Game On Exit");
   strcpy (global_string[3][21], "end");




   strcpy (global_string[4][0], "Netgame Options Menu");
   strcpy (global_string[4][1], "--------------------");
   strcpy (global_string[4][2], "Back to Game Menu");
   strcpy (global_string[4][3], "Server IP or Hostname:<NOT SET>");
   strcpy (global_string[4][4], "Packet Type:PPP");
   strcpy (global_string[4][5], "Deathmatch Bullets:On");
   strcpy (global_string[4][6], "Suicide Bullets:On");
   strcpy (global_string[4][7], "Deathmatch Bullet Damage:5");
   strcpy (global_string[4][8], "end");

   // 5 used for PDE
   // 6 USED FOR lift pop up menu

   strcpy (global_string[7][0], ""); // main menu
   strcpy (global_string[7][1], "");
   strcpy (global_string[7][2], "Start Level (1)");
   strcpy (global_string[7][3], "Start New Game");
   strcpy (global_string[7][4], "Resume Current Game");
   strcpy (global_string[7][5], "   ---   ");
   strcpy (global_string[7][6], "   ---   ");

#ifdef NETPLAY
   strcpy (global_string[7][5], "Host Network Game");
   strcpy (global_string[7][6], "Join Network Game");
#endif

   strcpy (global_string[7][7], "Options Menu");
   strcpy (global_string[7][8], "Level Editor");
   strcpy (global_string[7][9], "Demo Mode");
   strcpy (global_string[7][10], "Help Screens");
   strcpy (global_string[7][11], "end");


   strcpy (global_string[8][0], "Options Menu");
   strcpy (global_string[8][1], "----------");
   strcpy (global_string[8][2], "Back to Game Menu");
   strcpy (global_string[8][3], "Netgame Options");
   strcpy (global_string[8][4], "<-Change Color->");
   strcpy (global_string[8][5], "Controller Setup");
   strcpy (global_string[8][6], "Speed:40fps");
   strcpy (global_string[8][7], "Sound:Off");
   strcpy (global_string[8][8], "Sound Effects Volume:9");
   strcpy (global_string[8][9], "Sound Track Volume:9");
   strcpy (global_string[8][10], "Logging Options");
   strcpy (global_string[8][11], "Splash Screen:ON");
   strcpy (global_string[8][12], "Save Game");
   strcpy (global_string[8][13], "Run Game");
   strcpy (global_string[8][14], "end");


   strcpy (global_string[9][0],  "Controller Setup Menu");
   strcpy (global_string[9][1],  "---------------------");
   strcpy (global_string[9][2],  "Back to Options Menu");
   strcpy (global_string[9][3],  "Test Controls" );
   strcpy (global_string[9][4],  "Set all controls" );
   strcpy (global_string[9][5],  "Set all to joy1" );
   strcpy (global_string[9][6],  "Set all to joy2" );
   strcpy (global_string[9][7],  "Set all to arrows" );
   strcpy (global_string[9][8],  "Set all to IJKL SPACE C" );
   strcpy (global_string[9][9],  " ----------------------" );
   strcpy (global_string[9][23], "end");
}

void set_key_menu(int menu, int p, int start_row)
{
   int a = start_row;
   int m = menu;
   sprintf(global_string[m][a+0], "Up ---- %s", key_names[ players1[p].up_key]);
   sprintf(global_string[m][a+1], "Down -- %s", key_names[ players1[p].down_key]);
   sprintf(global_string[m][a+2], "Right - %s", key_names[ players1[p].right_key]);
   sprintf(global_string[m][a+3], "Left -- %s", key_names[ players1[p].left_key]);
   sprintf(global_string[m][a+4], "Jump -- %s", key_names[ players1[p].jump_key]);
   sprintf(global_string[m][a+5], "Fire -- %s", key_names[ players1[p].fire_key]);
   sprintf(global_string[m][a+6], "Menu -- %s", key_names[ players1[p].menu_key]);
   sprintf(global_string[m][a+7], "end" );

   // find longest text key description
   unsigned int longest = 0;
   for (int b=a; b<a+7; b++)
      if (strlen(global_string[m][b]) > longest) longest = strlen(global_string[m][b]);

   // pad string length to make them all line up nice
   for (int b=a; b<a+7; b++)
      while (strlen(global_string[m][b]) < longest)
         strcat(global_string[m][b], " ");

}
































