// z_game_menu.cpp

#include "pm.h"

void game_menu(void)
{
   old_program_state = 1;

   if (top_menu_sel < 3) top_menu_sel = 3;

   if (!splash_screen_done)
   {
      splash_screen();
      splash_screen_done = 1;
   }
   if (!resume_allowed) load_level(start_level, 0);
   do
   {
      top_menu_sel = zmenu(7, top_menu_sel, 10);

      if (top_menu_sel == 1)
      {
         program_state = 0;
         return;
      }

      if (key[ALLEGRO_KEY_L][2]) top_menu_sel = 8;


      if ((top_menu_sel == 4) && (resume_allowed)) // resume game
      {
         new_program_state = 13;
         return;
      }
      if (top_menu_sel == 3) // start new game
      {
         play_level = start_level;
         new_program_state = 10;
         top_menu_sel = 4; // so we come back on resume
         return;
      }
      if (top_menu_sel == 2) // start level
      {
         int pl = visual_level_select();
         if (pl)
         {
            play_level = pl;
            set_start_level(pl);
            printf("level selected:%d\n", play_level);
            top_menu_sel = 3;
         }
         else
         {
            top_menu_sel = 15; // dummy mode to redraw
         }
      }

      if (top_menu_sel == 5) // host network game
      {
         play_level = start_level;
         new_program_state = 20;
         return;
      }
      if (top_menu_sel == 6) // join network game
      {
         new_program_state = 24;
         return;
      }

      if (top_menu_sel == 9)
      {
         new_program_state = 2;
         return;
      }



      if (top_menu_sel == 8) // level editor
      {
         play_level = edit_menu(start_level);

         // restore menu items
         set_start_level(play_level);
         set_speed();

         top_menu_sel = 8;
         new_program_state = 10;
         return;
      }

      if (top_menu_sel == 10) help(""); // help

      if (top_menu_sel == 15) // dummy mode to redraw
      {
         load_level(start_level, 0);
         top_menu_sel = 2;
      }


      if (top_menu_sel == 7) // options menu
      {
         int options_menu_sel = 2;
         do
         {
            options_menu_sel = zmenu(8, options_menu_sel, 30);
            if (options_menu_sel == 3)
            {
               int netgame_menu_sel = 2;
               do
               {
                  netgame_menu_sel = zmenu(4, netgame_menu_sel, 30);
                  if (netgame_menu_sel == 3) // edit server name
                  {
                     edit_server_name();
                     save_config();
                  }
                  if (netgame_menu_sel == 4) // toggle TCP/UDP
                  {
                     TCP = !TCP;
                     save_config();
                  }
                  if (netgame_menu_sel == 5) // toggle deathmatch bullets
                  {
                     deathmatch_pbullets = !deathmatch_pbullets;
                     save_config();
                  }
                  if (netgame_menu_sel == 6) // toggle suicide bullets
                  {
                     suicide_pbullets = !suicide_pbullets;
                     save_config();
                  }
                  if ( (netgame_menu_sel >= 100) && (netgame_menu_sel < 200) ) // right
                  {
                     netgame_menu_sel -= 100;
                     if (netgame_menu_sel == 7) // damage inc
                     {
                        if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) deathmatch_pbullets_damage +=10;
                        else deathmatch_pbullets_damage++;
                        if (deathmatch_pbullets_damage > 100) deathmatch_pbullets_damage = 100;
                        save_config();
                     }
                  }
                  if ( (netgame_menu_sel >= 200) && (netgame_menu_sel < 300) )  // left
                  {
                     netgame_menu_sel -= 200;
                     if (netgame_menu_sel == 7) // damage dec
                     {
                        if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) deathmatch_pbullets_damage -=10;
                        else deathmatch_pbullets_damage--;
                        if (deathmatch_pbullets_damage < -10) deathmatch_pbullets_damage = -10;
                        save_config();
                     }
                  }

               }  while (netgame_menu_sel != 2); // end of netgame options menu
            }

            if (options_menu_sel == 10) // logging options menu
            {
               int logging_menu_sel = 2;
               do
               {
                  logging_menu_sel = zmenu(3, logging_menu_sel, 30);
                  if (logging_menu_sel == 3) // all on
                  {
                     L_LOGGING_NETPLAY=1;
                     L_LOGGING_NETPLAY_JOIN=1;
                     L_LOGGING_NETPLAY_PLAYER_ARRAY=1;
                     L_LOGGING_NETPLAY_bandwidth=1;
                     L_LOGGING_NETPLAY_cdat=1;
                     L_LOGGING_NETPLAY_game_move=1;
                     L_LOGGING_NETPLAY_sdat=1;
                     L_LOGGING_NETPLAY_sdak=1;
                     L_LOGGING_NETPLAY_stdf=1;
                     L_LOGGING_NETPLAY_stdf_all_packets=1;
                     L_LOGGING_NETPLAY_stdf_when_to_apply=1;
                     L_LOGGING_NETPLAY_show_dif1=1;
                     L_LOGGING_NETPLAY_show_dif2=1;
                     auto_save_game_on_level_done=1;
                     auto_save_game_on_exit=1;
                     save_config();
                  }
                  if (logging_menu_sel == 4 ) // all off
                  {
                     L_LOGGING_NETPLAY=0;
                     L_LOGGING_NETPLAY_JOIN=0;
                     L_LOGGING_NETPLAY_PLAYER_ARRAY=0;
                     L_LOGGING_NETPLAY_bandwidth=0;
                     L_LOGGING_NETPLAY_cdat=0;
                     L_LOGGING_NETPLAY_game_move=0;
                     L_LOGGING_NETPLAY_sdat=0;
                     L_LOGGING_NETPLAY_sdak=0;
                     L_LOGGING_NETPLAY_stdf=0;
                     L_LOGGING_NETPLAY_stdf_all_packets=0;
                     L_LOGGING_NETPLAY_stdf_when_to_apply=0;
                     L_LOGGING_NETPLAY_show_dif1=0;
                     L_LOGGING_NETPLAY_show_dif2=0;
                     auto_save_game_on_level_done=0;
                     auto_save_game_on_exit=0;
                     save_config();
                  }
                  int q = 5;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY= !L_LOGGING_NETPLAY;                                        save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_JOIN= !L_LOGGING_NETPLAY_JOIN;                              save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_PLAYER_ARRAY= !L_LOGGING_NETPLAY_PLAYER_ARRAY;              save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_bandwidth= !L_LOGGING_NETPLAY_bandwidth;                    save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_cdat= !L_LOGGING_NETPLAY_cdat;                              save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_game_move= !L_LOGGING_NETPLAY_game_move;                    save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_sdat = !L_LOGGING_NETPLAY_sdat;                             save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_sdak= !L_LOGGING_NETPLAY_sdak;                              save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_stdf= !L_LOGGING_NETPLAY_stdf;                              save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_stdf_all_packets= !L_LOGGING_NETPLAY_stdf_all_packets;      save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_stdf_when_to_apply= !L_LOGGING_NETPLAY_stdf_when_to_apply;  save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_show_dif1= !L_LOGGING_NETPLAY_show_dif1;                    save_config(); } q++;
                  if (logging_menu_sel == q) { L_LOGGING_NETPLAY_show_dif2 = !L_LOGGING_NETPLAY_show_dif2;                   save_config(); } q++;
                  if (logging_menu_sel == q) { auto_save_game_on_level_done= !auto_save_game_on_level_done;                  save_config(); } q++;
                  if (logging_menu_sel == q) { auto_save_game_on_exit= !auto_save_game_on_exit;                              save_config(); } q++;
                  if (logging_menu_sel == q) log_file_viewer(1);
               }  while (logging_menu_sel != 2); // end of netgame options menu
            }

            if (options_menu_sel ==  7) sound_toggle();
            if (options_menu_sel == 11) splash_toggle();
            if (options_menu_sel == 12) save_gm();
            if (options_menu_sel == 13) // run game
            {
               if (load_gm("-"))
               {
                  new_program_state = 14;
                  return;
                  // reset player data
                  for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
                  players[0].active = 1;
                  active_local_player = 0;
                  load_config(); // restore player color from config file
                  erase_log();
               }
            }

            if (options_menu_sel == 5) // controller setup menu
            {
               set_key_menu(9, 0, 10); //set controller menu keys
               int p1_menu_sel = 2;
               do
               {
                  p1_menu_sel = zmenu(9, p1_menu_sel, 30);
                  if (p1_menu_sel == 3) test_keys();

                  if (p1_menu_sel == 4) get_all_keys(0);

                  if (p1_menu_sel == 5) // set all to joy1
                  {
                     players1[0].up_key = 128;
                     players1[0].down_key = 129;
                     players1[0].right_key = 131;
                     players1[0].left_key = 130;
                     players1[0].jump_key = 133;
                     players1[0].fire_key = 132;
                     players1[0].menu_key = 135;
                  }

                  if (p1_menu_sel == 6) // set all to joy2
                  {
                     players1[0].up_key = 148;
                     players1[0].down_key = 149;
                     players1[0].right_key = 151;
                     players1[0].left_key = 150;
                     players1[0].jump_key = 153;
                     players1[0].fire_key = 152;
                     players1[0].menu_key = 155;
                  }

                  if (p1_menu_sel == 7) // set all to arrows
                  {
                     players1[0].up_key = 84;
                     players1[0].down_key = 85;
                     players1[0].right_key = 83;
                     players1[0].left_key = 82;
                     players1[0].jump_key = 75;
                     players1[0].fire_key = 3;
                     players1[0].menu_key = 59;
                  }

                  if (p1_menu_sel == 8) // set all to IJKL
                  {
                     players1[0].up_key = 9;
                     players1[0].down_key = 11;
                     players1[0].right_key = 12;
                     players1[0].left_key = 10;
                     players1[0].jump_key = 75;
                     players1[0].fire_key = 3;
                     players1[0].menu_key = 59;
                  }

                  if (p1_menu_sel == 10) players1[0].up_key =    my_readkey();
                  if (p1_menu_sel == 11) players1[0].down_key =  my_readkey();
                  if (p1_menu_sel == 12) players1[0].left_key =  my_readkey();
                  if (p1_menu_sel == 13) players1[0].right_key = my_readkey();
                  if (p1_menu_sel == 14) players1[0].jump_key =  my_readkey();
                  if (p1_menu_sel == 15) players1[0].fire_key =  my_readkey();
                  if (p1_menu_sel == 16) players1[0].menu_key =  my_readkey();

                  set_key_menu(9, 0, 10); //set controller menu keys
                  save_config();

                  if ( (p1_menu_sel >= 100) && (p1_menu_sel < 200) ) p1_menu_sel -= 100; // right
                  if ( (p1_menu_sel >= 200) && (p1_menu_sel < 300) ) p1_menu_sel -= 200; // left

               }  while (p1_menu_sel != 2);
            }


            if ( (options_menu_sel >= 100) && (options_menu_sel < 200) )  // right pressed on menu item
            {
               options_menu_sel -= 100;
               if (options_menu_sel == 4) // color inc
               {
                  int p = 0;
                  if (++players[p].color > 15) players[p].color = 1;
                  save_config();
               }
               if (options_menu_sel == 6) // frame_speed ++
               {
                  if      ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) frame_speed+=100;
                  else if ((key[ALLEGRO_KEY_LCTRL][0])  || (key[ALLEGRO_KEY_RCTRL][0]))  frame_speed+=10;
                  else                                                                   frame_speed++;
                  if (frame_speed > 500) frame_speed = 500;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol ++
               {
                  if (++se_scaler > 9) se_scaler = 9;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol ++
               {
                  if (++st_scaler > 9) st_scaler = 9;
                  set_st_scaler();
               }
            }
            if ( (options_menu_sel >= 200) && (options_menu_sel < 300) ) // left pressed on menu item
            {
               options_menu_sel -= 200;
               if (options_menu_sel == 4) // color dec
               {
                  int p = 0;
                  if (--players[p].color < 1) players[p].color = 15;
                  save_config();
               }
               if (options_menu_sel == 6) // frame_speed --
               {
                  if      ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) frame_speed-=100;
                  else if ((key[ALLEGRO_KEY_LCTRL][0])  || (key[ALLEGRO_KEY_RCTRL][0]))  frame_speed-=10;
                  else                                                                   frame_speed--;
                  if (frame_speed < 1) frame_speed = 1;
                  set_speed();
               }
               if (options_menu_sel == 8) // sound effects vol --
               {
                  if (--se_scaler < 0) se_scaler = 0;
                  set_se_scaler();
               }
               if (options_menu_sel == 9) // sound track vol --
               {
                  if (--st_scaler < 0) st_scaler = 0;
                  set_st_scaler();
               }
            }
         }  while (options_menu_sel != 2); // end of options menu
         al_rest(0.1);
      }
      if ((top_menu_sel >= 100) && (top_menu_sel < 200)) // right pressed on menu item
      {
         top_menu_sel -= 100;
         if (top_menu_sel == 2) // start level inc
         {
            if      ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) start_level+=100;
            else if ((key[ALLEGRO_KEY_LCTRL][0])  || (key[ALLEGRO_KEY_RCTRL][0]))  start_level+=10;
            else                                                                   start_level++;
            if (start_level > 399) start_level = 399;
            set_start_level(start_level);
            load_level(start_level, 0);
         }
      }
      if ((top_menu_sel >= 200) && (top_menu_sel < 300)) // left pressed on menu item
      {
         top_menu_sel -= 200;
         if (top_menu_sel == 2) // start level dec
         {
            if      ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) start_level-=100;
            else if ((key[ALLEGRO_KEY_LCTRL][0])  || (key[ALLEGRO_KEY_RCTRL][0]))  start_level-=10;
            else                                                                   start_level--;
            if (start_level < 1) start_level = 1;
            set_start_level(start_level);
            load_level(start_level, 0);
         }
      }
   } while (top_menu_sel != 1); // end of game menu
}

