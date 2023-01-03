// z_args.cpp

#include "pm.h"


void proc_command_line_args1(int argument_count, char **argument_array)
{

   if (argument_count == 2) // example 'pmwin x'
   {
      int ret = 0;
      if (strcmp(argument_array[1],"-x") == 0 )
      {
          ret = copy_files_to_clients(2); // src only
          exit(0);
      }
      if (strcmp(argument_array[1],"-t") == 0 )
      {
          ret = copy_files_to_clients(1); // pm.exe and levels only
          exit(0);
      }
      if (strcmp(argument_array[1],"-u") == 0 )
      {
         ret = copy_files_to_clients(0); // all files
         exit(0);
      }
      sprintf(msg, "ret:%d\n",ret); // to suppress warning only!
   }

}



void proc_command_line_args2(int argument_count, char **argument_array)
{
   if (argument_count == 2) // example 'pmwin arg1'
   {

      // run level editor -- eg: 'pm.exe -e'
      if (strcmp(argument_array[1],"-e") == 0 )
      {
         play_level = start_level;
         set_start_level(play_level);
         printf("running level editor for level:%d\n", play_level);
         play_level = edit_menu(play_level);
         set_start_level(play_level);
         fast_exit(0);
      }


      if (strcmp(argument_array[1],"-h") == 0 )  // help
      {
         help("Command Line");
         fast_exit(0);
      }


      if (strcmp(argument_array[1],"-var") == 0 )  // variable sizes
      {
         show_var_sizes();
         fast_exit(0);
      }

      if (strcmp(argument_array[1],"-test") == 0 )  // temp test
      {
         temp_test();
         fast_exit(0);
      }

      if (strcmp(argument_array[1],"-ts") == 0 )  // copy to clients and run server
      {

         copy_files_to_clients(1); // pm.exe and levels only


         show_splash_screen = 0;
         play_level = start_level;
         new_program_state = 20;
         return;





      }




      if (strcmp(argument_array[1],"-l") == 0 )  // log file viewer
      {
         log_file_viewer(1);
         fast_exit(0);
      }

      if (strcmp(argument_array[1],"-lr") == 0 )  // log file viewer most recent
      {
         log_file_viewer(2);
         fast_exit(0);
      }

      // no server specified; use the one from the config file
      if (strcmp(argument_array[1],"-c") == 0 )
      {
         show_splash_screen = 0;
         new_program_state = 24;
         return;
      }
      // no start level specified; use play level from config file
      if (strcmp(argument_array[1],"-s") == 0 )
      {
         show_splash_screen = 0;
         play_level = start_level;
         new_program_state = 20;
         return;
      }

      if (strcmp(argument_array[1],"-f") == 0 )
      {
         show_splash_screen = 0;
         if (load_gm("-"))
         {
            new_program_state = 14;
            return;
         }
      }

      // keep this last so if no other single flag matches try to run like it an int level...
      // start game on specified level -- eg: 'pm.exe 211'
      int pl = atoi(argument_array[1]);
      if ((pl > 0) && (pl < 400))
      {
         show_splash_screen = 0;
         play_level = pl;
         set_start_level(pl);
         printf("started game on level:%d\n", play_level);
         new_program_state = 10;
      }
      else
      {
         printf("%s could not be parsed to an integer level number\n", argument_array[1]);

         printf("trying to run as a demo file\n");

         // let's see if it is a valid gm file

         if (load_gm(argument_array[1]))
         {
            show_splash_screen = 0;
            new_program_state = 14;
            return;
         }
      }



   } // end of argument_count == 2


   if (argument_count == 3) // example 'pmwin arg1 arg2'
   {
      // run level editor for specified level -- eg: 'pm.exe -e 211'
      if (strcmp(argument_array[1],"-e") == 0 )
      {
         int pl = atoi(argument_array[2]);
         if ((pl > 0) && (pl < 400))
         {
            play_level = pl;
            set_start_level(pl);
            printf("running level editor for level:%d\n", pl);
            pl = edit_menu(pl);
            set_start_level(pl);
            fast_exit(0);
         }
         else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
      }
      if (strcmp(argument_array[1],"-c") == 0 )
      {
         show_splash_screen = 0;
         sprintf(m_serveraddress, "%s", argument_array[2]);
         save_config();
         new_program_state = 24;
         return;
      }
      if (strcmp(argument_array[1],"-s") == 0 )
      {
         show_splash_screen = 0;
         play_level = atoi(argument_array[2]);
         new_program_state = 20;
         return;
      }
   } // end of argument_count == 3
}

int copy_files_to_clients(int exe_only)
{
   int ret;
   char sys_cmd[500];
   char client[20][255];
   int num_clients = 0;

//   sprintf(client[num_clients++], "\\\\E6400\\pm_client1");  //  E6400
//   sprintf(client[num_clients++], "\\\\E6410\\pm_client2");  // win7 portable dev system
//   sprintf(client[num_clients++], "\\\\4230j\\pm_client3");  // win7 acer laptop
//   sprintf(client[num_clients++], "\\\\E6430\\pm_client4");  // win7 studio pc
//   sprintf(client[num_clients++], "\\\\E6420\\pm_client5");  // win7 2560x1600 (my room)
//   sprintf(client[num_clients++], "\\\\pfv\\pm_client6");    // XP 1600x1200
//   sprintf(client[num_clients++], "\\\\m-4230-3\\pm_client7"); // ubuntu acer laptop
//   sprintf(client[num_clients++], "\\\\DESKTOP-DBNSJH8\\pm_client8"); // win 10 EID work laptop
//   sprintf(client[num_clients++], "\\\\e4230f\\pm_client9"); // acer laptop
//   sprintf(client[num_clients++], "\\\\4230a\\pm_client10"); // acer laptop
//   sprintf(client[num_clients++], "\\\\insp9400\\pm_client11"); // dell insp 9400 (backup machine)
//   sprintf(client[num_clients++], "\\\\m7667\\pm_client12"); // core2 2G 32bit GeForce 7500 LE
//   sprintf(client[num_clients++], "\\\\nv59\\pm_client13");  // gateway nv59 i3 4G 1600x1200
//   sprintf(client[num_clients++], "\\\\y510\\pm_client14");  // y510 XP SP3
//   sprintf(client[num_clients++], "\\\\zi3\\pm_client99");  // zaiden
//   sprintf(client[num_clients++], "\\\\sat-p100\\pm_client31");  // win 7 does not work...32 bit??
//   sprintf(client[num_clients++], "\\\\e6400\\pm_client27");  // win 7 -- has stupid network issues, sometimes take 4s to get a packet reply


//   sprintf(client[num_clients++], "\\\\4230j\\pm_client30");  // win 7



   sprintf(client[num_clients++], "\\\\e6430\\pm_client24");  // win 7

//   sprintf(client[num_clients++], "\\\\4230y\\pm_client18");  // win 7
//   sprintf(client[num_clients++], "\\\\4230i\\pm_client25");  // win 7
//   sprintf(client[num_clients++], "\\\\4230l\\pm_client29");  // win 7
//


//   sprintf(client[num_clients++], "\\\\4230jj\\pm_client28"); // win 7
//   sprintf(client[num_clients++], "\\\\4230h\\pm_client26");  // win 7
//
//   sprintf(client[num_clients++], "\\\\e6400\\pm_client27");  // win 7 (wifi and slow)
//



   if (exe_only == 1)
   {
      printf("copying exe to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "copy pm.exe %s\\pm.exe /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

//         sprintf(sys_cmd, "copy levels\\*.pml %s\\levels ", client[c]);
//         printf("%s\n",sys_cmd);
//         ret = system(sys_cmd);


/*
         sprintf(sys_cmd, "copy pm.cfg %s\\pm.cfg /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         */

      }
   }
   else if (exe_only == 2)
   {
      printf("copying src to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         sprintf(sys_cmd, "copy src\\*.cpp %s\\src ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         sprintf(sys_cmd, "copy src\\*.h %s\\src ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);

         sprintf(sys_cmd, "copy levels\\*.pml %s\\levels ", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
      }
   }
   else
   {
      printf("copying all files to clients\n");
      for (int c=0; c<num_clients; c++)
      {
         // erase all files (but not directories)
//         sprintf(sys_cmd, "del %s\\*.* /S /Q", client[c]);
//         printf("%s\n",sys_cmd);
//         ret = system(sys_cmd);

         sprintf(sys_cmd, "xcopy *.* %s /E /Y", client[c]);
         printf("%s\n",sys_cmd);
         ret = system(sys_cmd);
      }
   }
   return ret;
}









/*

double in=2.132;
char arr[12] = {};
memcpy(arr,&in,sizeof(in));

char arr2[12] = ...;
double out;
memcpy(&out,arr2,sizeof(out));

*/

//
//char packetbuffer1[1024] = {0};
//int packetsize1 = 0;
//int packetpos1 = 0;
//
//void PacketPutDouble1(double d)
//{
//   memcpy(packetbuffer1 + packetsize1, &d, 8);
//	packetsize1+=8;
//}
//
//double PacketGetDouble1(void)
//{
//   double d = 0;
//   memcpy(&d, packetbuffer1 + packetpos1, 8);
//	packetpos1+=8;
//	return d;
//}

void temp_test(void)
{




   void log_bandwidth_graph(int);


   log_bandwidth_graph(0);







//   double t0 = 0.000123;
//   printf("1 - ps:%d pp:%d sot:%d t0:%f\n", packetsize1, packetpos1, sizeof(t0), t0);
//
//   PacketPutDouble1(t0);
//   printf("2 - ps:%d pp:%d sot:%d t0:%f\n", packetsize1, packetpos1, sizeof(t0), t0);
//
//
//   double t1 = PacketGetDouble1();
//   printf("3 - ps:%d pp:%d sot:%d t1:%f\n", packetsize1, packetpos1, sizeof(t1), t1);
//
//
//













//   mdw_message_box_2("Hello", "poo", "pee");


 //  exit_level_editor_dialog();

 //


//   for (int b=0; b<1024; b+=256)
//   {
//      al_set_target_backbuffer(display);
//      al_clear_to_color(al_map_rgb(0,0,0));
//
//      int x = 100;
//      int y = 0;
//
//      for (int a=b; a<b+128; a++)
//         al_draw_textf(font, palette_color[15], x, y+=8, 0, "%d-[%c] %d-[%c]", a, a, a+128, a+128);
//      al_flip_display();
//      tsw();
//   }



/*

int it13=0;

printBits(4, &it13);
printf("b0:[%s]\n",msg);

set_int_3216(it13, 65535, 64000);

printBits(4, &it13);
printf("b1:[%s]\n",msg);




int tc = 0;
int fc = 0;
get_int_3216(it13, tc, fc);

printBits(4, &tc);
printf("b2:[%s]\n",msg);

printBits(4, &fc);
printf("b3:[%s]\n",msg);

printf("--- %d  %d\n", tc, fc);

*/


/*
   int test = 0;
   printBits(4, &test);

   printf("b4:[%s]\n",msg);

   test |= PM_LIFT_NO_DRAW;


   printBits(4, &test);

   printf("af:'%s'\n",msg);

*/



//   void colorize_tile(void);
//   colorize_tile();


//void combine_tile(void);
//   combine_tile();



/*
   load_level(start_level, 0);
   group_edit();

*/

/*

void show_level_done(void);

   al_set_target_backbuffer(display);

   int quit = 0;
   while (!quit)
   {
      proc_controllers();
      al_clear_to_color(al_map_rgb(0,0,0));
      show_level_done();
      al_flip_display();

      while ((key[ALLEGRO_KEY_ESCAPE][0]) || (mouse_b[2][0]))
      {
         proc_controllers();
         quit = 1;
      }
      if (key[ALLEGRO_KEY_1][3]) players[1].active = !players[1].active;
      if (key[ALLEGRO_KEY_2][3]) players[2].active = !players[2].active;
      if (key[ALLEGRO_KEY_3][3]) players[3].active = !players[3].active;
      if (key[ALLEGRO_KEY_4][3]) players[4].active = !players[4].active;
      if (key[ALLEGRO_KEY_5][3]) players[5].active = !players[5].active;
      if (key[ALLEGRO_KEY_6][3]) players[6].active = !players[6].active;
      if (key[ALLEGRO_KEY_7][3]) players[7].active = !players[7].active;
   }

*/

//   char tst[20];
//   printf("sizeof %d\n", sizeof(tst));

   //spline_test();
   //spline_adjust();


   //tile_editor();

//   edit_tile_attributes();

  // copy_tiles();

//            play_level = start_level;
//            set_start_level(play_level);
//            printf("running level editor for level:%d\n", play_level);
//            play_level = edit_menu(play_level);
//            set_start_level(play_level);
//            fast_exit(0);



//   al_show_mouse_cursor(display);
//   load_level(start_level, 0);
//   lift_viewer(4);

}
