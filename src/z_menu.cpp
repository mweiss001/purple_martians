// zmenu.cpp

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




void help(const char *topic)
{
//   if (SCREEN_H < 480) return;       // wont work with SCREEN_H < 480

   help_screens_running = 1;
   auto_set_display_transform_double();

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
                al_set_clipping_rectangle((dx+12)*display_transform_double, (12*display_transform_double), (639-12*2)*display_transform_double, (SCREEN_H-12*2)*display_transform_double);
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
               al_set_clipping_rectangle((dx+12)*display_transform_double, (12*display_transform_double), (639-12*2)*display_transform_double, (SCREEN_H-12*2)*display_transform_double);
               float sc = .25;
               int xo = (int)(200 * sc)+16;
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
                al_draw_bitmap(tile[ans], dx+sxc, sy, 0 );
                msg[0]= 0;
            }
            if (strncmp(msg, "<a", 2) == 0) // <axx> show animation sequence (left just)
            {
                processed_tag_this_time_through = 1;
                buff2[0] = msg[2];
                buff2[1] = msg[3];
                buff2[2] = 0;
                int ans = zz[0][atoi(buff2)];
                al_draw_bitmap(tile[ans], dx+sx, sy, 0 );

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
                al_draw_bitmap(player_tile[pco-1][ans], dx+sxc-40, sy, 0 );
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
                al_draw_bitmap(player_tile[pco-1][ans], dx+sx, sy, 0 );
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
               al_draw_bitmap(tile[ans], dx+sx, sy, 0 );
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
                  al_draw_bitmap(tile[ans], dx+sx+(z*20), sy, 0 );
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
      frame_num++;
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
   help_screens_running = 0;
   auto_set_display_transform_double();
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
         if (resume_allowed) demo_mode_countdown = 800;
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

      if (key[ALLEGRO_KEY_PGDN]) highlight = last_list_item;
      if (key[ALLEGRO_KEY_PGUP]) highlight = 2;





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


   strcpy (item_name[0],"item_empty");
   strcpy (item_name[1],"Door");
   strcpy (item_name[2],"Bonus");
   strcpy (item_name[3],"Exit");
   strcpy (item_name[4],"Key");
   strcpy (item_name[5],"Start");
   strcpy (item_name[6],"Free Man");
   strcpy (item_name[7],"Mine");
   strcpy (item_name[8],"Bomb");
   strcpy (item_name[9], "undefined");
   strcpy (item_name[10],"Message");
   strcpy (item_name[11],"Rocket");
   strcpy (item_name[12],"Warp");
   strcpy (item_name[14],"Switch");
   strcpy (item_name[15],"Sproingy");

   strcpy (enemy_name[0], "empty");
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



   strcpy (global_string[2][0], "Level Editor Pop-Up Menu");
   strcpy (global_string[2][1], "------------------------");
   strcpy (global_string[2][2], "Copy ---");
   strcpy (global_string[2][3], "View ---");
   strcpy (global_string[2][4], "Delete ---");
   strcpy (global_string[2][5], "------------------------");
   strcpy (global_string[2][6], "Zoom Full Screen");
   strcpy (global_string[2][7], "Show Status Window");
   strcpy (global_string[2][8], "Show Selection Window");
   strcpy (global_string[2][9], "------");
   strcpy (global_string[2][10],"New Level");
   strcpy (global_string[2][11],"Load Level");
   strcpy (global_string[2][12],"Save Level");
   strcpy (global_string[2][13],"Save and Quit");
   strcpy (global_string[2][14],"Help Screens");
   strcpy (global_string[2][15],"Quit Level Editor");
   strcpy (global_string[2][16],"end");

#ifndef RELEASE
   strcpy (global_string[2][16],"----");
   strcpy (global_string[2][17],"Predefined Enemy Editor");
   strcpy (global_string[2][18],"Global Level Thingy!!");
   strcpy (global_string[2][19],"Level Viewer!");
   strcpy (global_string[2][20],"Bitmap and Animation Editor");
   strcpy (global_string[2][21],"end");
#endif


   strcpy (global_string[3][0], "Logging Options Menu");
   strcpy (global_string[3][1], "--------------------");
   strcpy (global_string[3][2], "Back to Game Menu");
   strcpy (global_string[3][3], "ALL ON");
   strcpy (global_string[3][4], "ALL OFF");
   strcpy (global_string[3][5], "LOGGING_NETPLAY");
   strcpy (global_string[3][6], "LOGGING_NETPLAY_JOIN");
   strcpy (global_string[3][7], "LOGGING_NETPLAY_bandwidth");
   strcpy (global_string[3][8], "LOGGING_NETPLAY_cdat");
   strcpy (global_string[3][9], "LOGGING_NETPLAY_game_move");
   strcpy (global_string[3][10], "LOGGING_NETPLAY_sdat");
   strcpy (global_string[3][11], "LOGGING_NETPLAY_sdak");
   strcpy (global_string[3][12], "LOGGING_NETPLAY_stdf");
   strcpy (global_string[3][13], "LOGGING_NETPLAY_stdf_all_packets");
   strcpy (global_string[3][14], "LOGGING_NETPLAY_stdf_when_to_apply");
   strcpy (global_string[3][15], "LOGGING_NETPLAY_show_dif1");
   strcpy (global_string[3][16], "LOGGING_NETPLAY_show_dif2");
   strcpy (global_string[3][17], "Auto Save Game On Level Done");
   strcpy (global_string[3][18], "Auto Save Game On Exit");
   strcpy (global_string[3][19], "Log File Viewer");
   strcpy (global_string[3][20], "end");


   strcpy (global_string[4][0], "Netgame Options Menu");
   strcpy (global_string[4][1], "--------------------");
   strcpy (global_string[4][2], "Back to Game Menu");
   strcpy (global_string[4][3], "Server IP or Hostname:<NOT SET>");
   strcpy (global_string[4][4], "Packet Type:PPP");
   strcpy (global_string[4][5], "Deathmatch Bullets:On");
   strcpy (global_string[4][6], "Suicide Bullets:On");
   strcpy (global_string[4][7], "Deathmatch Bullet Damage:5");
   strcpy (global_string[4][8], "end");

   strcpy (global_string[5][0],"PUT"); // PD sub menu
   strcpy (global_string[5][1],"PREV");
   strcpy (global_string[5][2],"NEXT");
   strcpy (global_string[5][3],"");
   strcpy (global_string[5][4],"COPY");
   strcpy (global_string[5][5],"SAVE");
   strcpy (global_string[5][6],"LOAD");
   strcpy (global_string[5][7],"BACK");


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
   strcpy (global_string[8][12], "Save Demo");
   strcpy (global_string[8][13], "Run Demo");
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
   strcpy (global_string[9][9],  "-----------------------" );
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

int pmenu(int menu_num)  // this menu function does not pass through like the next one
{                        // it waits for a selection and then exits
                         // its is entered with mouse_b 2 pressed and exits when released
   int highlight = 2;
   int selection = 999;
   int last_list_item;
   int c, b;

   int kx = mouse_x;
   if (kx < 100) kx = 100;
   if (kx > SCREEN_W-100) kx = SCREEN_W-100;

   int up = 0;
   int ky = mouse_y-20;
   if (menu_num == 2) if (ky > SCREEN_H - 160) up=1; // main editor menu
   if (menu_num == 6) if (ky > SCREEN_H - 60) up=1;  // lift step menu

   if (!up) // reverse version !
   {
      do   // until selection is made
      {
         c = 0;
         al_rest(0.02);
         while (strcmp(global_string[menu_num][c],"end") != 0)
         {
            b = 9 + 96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int w = strlen(global_string[menu_num][c])*4;
            al_draw_filled_rectangle(kx-w, ky+(c*8), kx+w, ky+(c*8)+8, palette_color[0]);
            al_draw_text(font, palette_color[b], kx, ky+(c*8),  ALLEGRO_ALIGN_CENTER, global_string[menu_num][c]);
            c++;
         }
         last_list_item = c-1;
         al_flip_display();
         proc_controllers();

         highlight = 2;
         if ( (mouse_x > (kx - 100)) && (mouse_x < (kx+100)) )
            if ( (mouse_y > ky ) && (mouse_y < ky + ((last_list_item+1)*8)) )
               highlight = (mouse_y-ky) / 8;
         if (!(mouse_b2)) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   if (up)  // normal version
   {
      ky = mouse_y+12;
      if (ky > SCREEN_H) ky = SCREEN_H;
      do   // until selection is made
      {
         c = 0;
         al_rest(0.02);
         //show_mouse(NULL);
         while (strcmp(global_string[menu_num][c],"end") != 0)
         {
            b = 9+96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int w = strlen(global_string[menu_num][c])*4;
            al_draw_filled_rectangle(kx-w, ky-(c*8), kx+w, ky-(c*8)+8, palette_color[0]);
            al_draw_text(font, palette_color[b], kx, ky-(c*8),  ALLEGRO_ALIGN_CENTER, global_string[menu_num][c]);
            c++;
         }
         last_list_item = c-1;

         al_flip_display();
         proc_controllers();

         //show_mouse(screen);
         highlight = 2;
         if ( (mouse_x > (kx - 100)) && (mouse_x < (kx+100)) )
            if ( (mouse_y < ky ) && (mouse_y > ky - ((last_list_item+1)*8) ) )
               highlight = (ky-mouse_y+8) / 8;
         if (!(mouse_b2)) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   //show_mouse(NULL);
   return selection;
}





// edit text stuff




void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore, int rot)
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
   if (rot == 0)
   {
      x = cursor_col*8+xpos_c - strlen(dt[cursor_row])*4;
      y = ypos+cursor_row*8;

      if (restore) // black background, text color text
      {
         al_draw_filled_rectangle(x, y, x+8, y+8, palette_color[0]);
         al_draw_text(font, palette_color[cursor_color], x, y, 0, msg);
      }

      else // red background, black text
      {
         al_draw_filled_rectangle(x, y, x+8, y+8, palette_color[10]);
         al_draw_text(font, palette_color[0], x, y, 0, msg);
      }
   }

   if (rot == 64)
   {
      x = xpos_c+cursor_row*8;
      y = 8+ cursor_col*8+ypos - strlen(dt[cursor_row])*4;
      if (restore) // black background, text color text
      {
         al_draw_filled_rectangle(x-4, y-4, x+4, y+4, palette_color[0]);
         rtextout_centre(NULL, msg, x, y, cursor_color, 1, 64, 1);
      }
      else // red background, black text
      {
         al_draw_filled_rectangle(x-4, y-4, x+4, y+4, palette_color[10]);
         rtextout_centre(NULL, msg, x, y, 0, 1, 64, 1);
      }
   }
}


int edit_pmsg_text(int c, int new_msg)
{
   int tc = item[c][8];
   int char_count;
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a, k=0;
   char f[1800];
   int quit = 0;

   // button row x values
   int xa = 4+SCREEN_W-(SCREEN_W-(db*100));
   int xb = SCREEN_W-4;

   int smx = txc;  // x center
   int smy = pop_msg_viewer_pos;

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
      strcpy(f, pmsg[c]);
      char_count = strlen(f);
   }

   while (!quit)
   {

      title("Message Creator", 2, 15, 12);

      al_set_target_backbuffer(display);

      // draw the message
      display_pop_message(c, f, smx, smy, 1, 0);

      a = -3; //back up from the message to the buttons;;
      int by = smy-bts/2-2;

      mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 7, 999, 0, 0, 0, 14, 15,  0, 1,0,0,0);  // edit text placeholder

      a++;
      if (mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 1,   0, 0, 0, 0, 11, 15, 15, 1,0,0,0))  // OK
      {
         quit = 1;
         bad = 0;
      }

      a++;
      if (mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 3,   0, 0, 0, 0, 10, 15, 15, 1,0,0,0))  // Cancel
      {
         quit = 1;
         bad = 1;
      }

      if (blink_counter++ < blink_count)
         show_cursor(f, cursor_pos, smx, smy, tc, 0, 0);
      else show_cursor(f, cursor_pos, smx, smy, tc, 1, 0);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(f, old_cp, smx, smy, tc, 1, 0); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      k = proc_controllers();

      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           f[a]=f[a+1];
         char_count--;
         // set last to NULL
         f[char_count] = (char)NULL;
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           f[a]=f[a+1];
         char_count--;
         // set last to NULL
         f[char_count] = (char)NULL;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         // find next line break
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 126));
         cursor_pos++;
         // make sure we are not past the end
         if (cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_UP])
      {
         // find previous line break
         while ((--cursor_pos > 0) && (f[cursor_pos] != 126));
         cursor_pos--;
         // make sure we are not before the start
         if (cursor_pos < 0) cursor_pos = 0;
      }
      if (key[ALLEGRO_KEY_HOME])
      {
         cursor_pos = 0;
      }
      if (key[ALLEGRO_KEY_END])
      {
         cursor_pos = char_count-1;
      }

      if (k)
      {
         k = Key_pressed_ASCII;
         if (k==13) k = 126; // replace enter with 126 ~

         if ((k>31) && (k<127)) // if alphanumeric
         {
            // move over to make room
            for (a = char_count; a>=cursor_pos; a--)
               f[a+1]=f[a];

            // set char
            f[cursor_pos] = k;

            // inc both
            cursor_pos++;
            char_count++;

            // set last to NULL
            f[char_count] = (char)NULL;
         }
      }
      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
         bad = 1;
      }

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_rest(0.07);
   } // end of while (!quit)

   if (bad) return 0;
   else
   {
      free(pmsg[c]);
      pmsg[c] = (char*) malloc (strlen(f)+1);
      strcpy(pmsg[c], f);
      return 1;
   }
}


void edit_server_name(void)
{
   char fst[80];
   strcpy(fst, m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
   int quit = 0;
   while (!quit)
   {
      int tx = SCREEN_W/2;
      int ty = SCREEN_H/2;
      int w = (char_count+1)*4;

      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty-4-2, tx+w+18, ty+4+3, palette_color[0]);

      al_draw_text(font, palette_color[15], tx, ty-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty-4-1, tx+w+6, ty+6, palette_color[15], 1);

      rtextout_centre(NULL, fst, tx, ty+1, 15, 1, 0, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty-3, 15, 0, 0);
      else show_cursor(fst, cursor_pos, tx, ty-3, 15, 1, 0);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty-3, 15, 1, 0); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      int k = proc_controllers();
      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      k = Key_pressed_ASCII;
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
      if (key[ALLEGRO_KEY_ENTER])
      {
         while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
         strcpy(m_serveraddress, fst);
         quit = 1;
      }

      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
      }
   }
}


int edit_lift_name(int lift, int step_ty, int bts, char *fst)
{
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a, k=0;
   int char_count = strlen(fst);
   while (1)
   {
      al_flip_display();
      //al_clear_to_color(al_map_rgb(0,0,0));
      al_rest(.1);

      int x1 = (SCREEN_H/100)*100+22;
      int x2 = x1 + (lifts[lift].width * 20) -1;
      int y1 = step_ty + (lifts[lift].num_steps+3) * bts; // only see in 2 highest screen modes
      int y2 = y1 + (lifts[lift].height * 20) -1;

      int tx = ((x1+x2)/2);
      int ty = ((y1+y2)/2);
      int w = (char_count+1) *4;

      int rot = 0;
      if ((lifts[lift].width == 1) && (lifts[lift].height > 1)) rot = 64;
      int color = lifts[lift].color;

      // clear text background
      if (rot == 64) al_draw_filled_rectangle(tx-4, ty-w, tx+4, ty+w, palette_color[0]);
      else           al_draw_filled_rectangle(tx-w, ty-4, tx+w, ty+4, palette_color[0]);

      for (a=0; a<10; a++)
         al_draw_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color + ((9 - a)*16)], 1 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );
      rtextout_centre(NULL, fst, tx, ty+1, color+160, 1, rot, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty-3, 15, 0, rot);
      else show_cursor(fst, cursor_pos, tx, ty-3, 15, 1, rot);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty-3, 15, 1, rot); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      k = proc_controllers();
      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      k = Key_pressed_ASCII;
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
      if (key[ALLEGRO_KEY_ENTER]) return 1;
      if (key[ALLEGRO_KEY_ESCAPE]) return 0;
   }
}




















