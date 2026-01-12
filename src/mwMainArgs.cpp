// mwMainArgs.cpp
#include "pm.h"
#include "mwMain.h"

#include "mwLog.h"
#include "mwNetgame.h"
#include "mwLogo.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwDemoMode.h"

#include "mwConfig.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

// #include "sqlite3.h"


void mwMain::pm_copy_src(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "copy src\\* %s\\src ", filepath);                          printf("%s\n",sys_cmd);   system(sys_cmd);
}


void mwMain::pm_copy_exe(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "copy pm.exe %s\\pm.exe /Y", filepath);                       printf("%s\n",sys_cmd); system(sys_cmd);
}

void mwMain::pm_copy_cfg(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "copy pm.cfg %s\\pm.cfg /Y", filepath);                       printf("%s\n",sys_cmd); system(sys_cmd);
   sprintf(sys_cmd, "copy data\\*.pm %s\\data ", filepath);                       printf("%s\n",sys_cmd); system(sys_cmd);
}


void mwMain::pm_copy_levels(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "mkdir %s\\levels ", filepath);                               printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy levels\\*.pml %s\\levels ", filepath);                  printf("%s\n",sys_cmd);   system(sys_cmd);
}


void mwMain::pm_delete_all(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "del %s\\*.* /S /Q", filepath);                               printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "rmdir %s\\ /S /Q", filepath);                                printf("%s\n",sys_cmd);   system(sys_cmd);
}

void mwMain::pm_copy_misc(const char* filepath)
{
   char sys_cmd[500];
   sprintf(sys_cmd, "copy license.txt %s\\pm.exe /Y", filepath);                  printf("%s\n",sys_cmd);   system(sys_cmd);

   sprintf(sys_cmd, "mkdir %s\\savegame\\demo ", filepath);                       printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy savegame\\demo\\*.gm %s\\savegame\\demo ", filepath);   printf("%s\n",sys_cmd);   system(sys_cmd);

   sprintf(sys_cmd, "mkdir %s\\bitmaps ", filepath);                              printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy bitmaps\\* %s\\bitmaps ", filepath);                    printf("%s\n",sys_cmd);   system(sys_cmd);

   sprintf(sys_cmd, "mkdir %s\\data ", filepath);                                 printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy data\\level_icons_100.bmp %s\\data ", filepath);        printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy data\\level_icons_200.bmp %s\\data ", filepath);        printf("%s\n",sys_cmd);   system(sys_cmd);

   sprintf(sys_cmd, "mkdir %s\\help ", filepath);                                 printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy help\\* %s\\help ", filepath);                          printf("%s\n",sys_cmd);   system(sys_cmd);

   sprintf(sys_cmd, "mkdir %s\\snd ", filepath);                                  printf("%s\n",sys_cmd);   system(sys_cmd);
   sprintf(sys_cmd, "copy snd\\* %s\\snd ", filepath);                            printf("%s\n",sys_cmd);   system(sys_cmd);

}

void mwMain::copy_files_to_clients(int type)
{
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
//   sprintf(client[num_clients++], "\\\\e6400\\pm_client27");  // win 7 (wifi and slow)

//   sprintf(client[num_clients++], "\\\\4230x\\pm_client16");  // ubuntu 20.04 laptop no point in copying windows stuff to linux


//   sprintf(client[num_clients++], "\\\\i990\\pm_client32");  // win 7

//   sprintf(client[num_clients++], "\\\\zulu\\pm_client52");  // win 10


  // sprintf(client[num_clients++], "\\\\o7040\\pm_client51");  // win 10


//   sprintf(client[num_clients++], "\\\\4230jj\\pm_client28"); // win 7
//   sprintf(client[num_clients++], "\\\\4230i\\pm_client25");  // win 7


//   sprintf(client[num_clients++], "\\\\4230y\\pm_client18");  // win 7
//   sprintf(client[num_clients++], "\\\\4230l\\pm_client29");  // win 7


   sprintf(client[num_clients++], "\\\\asp3\\pm_client53");  // win 10
   sprintf(client[num_clients++], "\\\\4230j\\pm_client30");  // win 7
   sprintf(client[num_clients++], "\\\\e6430\\pm_client50");  // win 10
   sprintf(client[num_clients++], "\\\\4230h\\pm_client26");  // win 7








   for (int c=0; c<num_clients; c++)
   {
      if (type == 1) // exe only
      {
         pm_copy_exe(client[c]);
      }
      if (type == 2) // exe and levels
      {
         pm_copy_exe(client[c]);
         pm_copy_levels(client[c]);
      }
      if (type == 3) // del all and copy like release
      {
         pm_delete_all(client[c]);
         pm_copy_levels(client[c]);
         pm_copy_misc(client[c]);
         pm_copy_exe(client[c]);
      }
      if (type == 4) // del all and copy absolutely everything
      {
         pm_delete_all(client[c]);
         sprintf(sys_cmd, "xcopy *.* %s /E /Y", client[c]);
         printf("%s\n",sys_cmd);
         system(sys_cmd);
      }
      if (type == 5) // cfg only
      {
         pm_copy_cfg(client[c]);
      }

   }
}

void mwMain::proc_command_line_args1(int argument_count, char **argument_array)
{






   if ((argument_count > 1) && (strcmp(argument_array[1],"-sh") == 0 )) headless_server = 1;

//   if ((argument_count > 1) && (strcmp(argument_array[1],"-rc") == 0 )) server_remote_control = 1;


   if (argument_count == 2) // example 'pmwin x'
   {


      if (strcmp(argument_array[1],"-test") == 0 )  // temp test
      {
         temp_test();
         exit(0);
      }




      if (strcmp(argument_array[1],"-t")  == 0 ) { copy_files_to_clients(1); exit(0); } // exe only
      if (strcmp(argument_array[1],"-tl") == 0 ) { copy_files_to_clients(2); exit(0); } // exe and levels
      if (strcmp(argument_array[1],"-tr") == 0 ) { copy_files_to_clients(3); exit(0); } // del all and copy like release
      if (strcmp(argument_array[1],"-ta") == 0 ) { copy_files_to_clients(4); exit(0); } // del all and copy absolutely everything

      if (strcmp(argument_array[1],"-tc") == 0 ) { copy_files_to_clients(5); exit(0); } // cfg only

//      if (strcmp(argument_array[1],"-tu") == 0 ) { pm_copy_src("\\\\scat\\pm23"); exit(0); } // copy src dir only to specific linux machine

      if (strcmp(argument_array[1],"-tu") == 0 ) { pm_copy_levels("\\\\scat\\pm23"); exit(0); } // copy src dir only to specific linux machine



      if (strcmp(argument_array[1],"-la") == 0 ) // turn all logging to console on
      {
         for (int i=0; i<100; i++)
            if ((mLog.log_types[i].group) && (mLog.log_types[i].group < 4)) mLog.log_types[i].action = 1;
      }





      if (strcmp(argument_array[1],"-?") == 0 )
      {
         printf("\nUsage:\n");

         printf("\n");
         printf(" -h        (runs the graphical help screens, eg: 'pm.exe -h')\n");

         printf("\n");
         printf(" no args   (starts the game normally, eg: 'pm.exe')\n");
         printf(" [lev]     (starts the game immediately playing level [lev], eg: 'pm.exe 5'\n");

         printf("\n");
         printf(" -c        (immediately tries to join a netgame using the saved server name, eg: 'pm.exe -c')\n");
         printf(" -c [srv]  (immediately tries to join a netgame using the supplied server name, eg: 'pm.exe -c '96.45.0.15')\n");

         printf("\n");
         printf(" -s        (immediately hosts a netgame on the current level eg: 'pm.exe -s')\n");
         printf(" -s [lev]  (immediately hosts a netgame on level [lev], eg: 'pm.exe -s 23')\n");

         printf(" -sh       (headless server mode\n");
         printf(" -rc       (server remote control mode\n");

         printf("\n");
         printf(" -e        (starts in level editor mode on the current level, eg: 'pm.exe -e')\n");
         printf(" -e [lev]  (starts in level editor mode on level [lev], eg: 'pm.exe -e 7')\n");

         printf("\n");
         printf(" -f        (prompts to choose a demo file to run, eg: 'pm.exe -f')\n");
         printf(" [file.pm] (immediately runs supplied demo file, eg: 'pm.exe C:/pm/savegame/demo/lev2.gm')\n");

         printf("\n");
         printf(" -l        (starts log file viewer and prompts for log file, eg: 'pm.exe -l')\n");
         printf(" -lr       (starts log file viewer and opens most recent log file, eg: 'pm.exe -lr')\n");

         printf("\n");
         printf("Advanced (windows only)\n");
         printf(" -t        (copies exe to all clients, eg: 'pm.exe -t')\n");
         printf(" -tl       (copies exe and levels to all clients, eg: 'pm.exe -tl')\n");
         printf(" -tr       (deletes all on clients, then copies all needed for release, eg: 'pm.exe -tr')\n");
         printf(" -ta       (deletes all on clients, then copies everything, eg: 'pm.exe -ta')\n");
         printf(" -ts       (copies exe to all clients then runs server mode locally, eg: 'pm.exe -ts')\n");
         printf(" -tc       (copies config only eg: 'pm.exe -tc')\n");


         fast_exit(0);
      }
   }
}



void mwMain::proc_command_line_args2(int argument_count, char **argument_array)
{
   if (argument_count == 2) // example 'pmwin arg1'
   {
      // run level editor -- eg: 'pm.exe -e'
      if (strcmp(argument_array[1],"-e") == 0 )
      {
         printf("running level editor for level:%d\n", mLevel.start_level);
         mLevel.set_start_level(mWM.loop(mLevel.start_level));
         fast_exit(0);
      }



      if (strcmp(argument_array[1],"-test2") == 0 )  // temp test
      {
         temp_test();
         exit(0);
      }




      if (strcmp(argument_array[1],"-h") == 0 )  // help
      {
         mHelp.help("Command Line");
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
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
         return;
      }

      if (strcmp(argument_array[1],"-l") == 0 )  // log file viewer
      {
         mLog.log_file_viewer(1);
         fast_exit(0);
      }

      if (strcmp(argument_array[1],"-lr") == 0 )  // log file viewer most recent
      {
         mLog.log_file_viewer(2);
         fast_exit(0);
      }

      // join netgame - no server specified, use server from config file
      if (strcmp(argument_array[1],"-c") == 0 )
      {
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_NEW_GAME;
         mLoop.done_action = 1;
         mLoop.quit_action = 0;
         return;
      }
      // host netgame - use start level from config file
      if (strcmp(argument_array[1],"-s") == 0 )
      {
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
         mLoop.quit_action = 0;
         mLoop.done_action = 1;
         return;
      }

      if (strcmp(argument_array[1],"-sh") == 0 ) // headless server
      {
         mNetgame.headless_server_setup();
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
         mLoop.quit_action = 0;
         mLoop.done_action = 1;
         return;
      }

      if (strcmp(argument_array[1],"-rc") == 0 ) // server remote control
      {
         server_remote_control = 1;
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP;
         return;
      }

      if (strcmp(argument_array[1],"-f") == 0 )
      {
         mLogo.show_splash_screen = 0;
         mDemoMode.run_single_from_cmd_prompt();
         fast_exit(0);
      }

      // keep this last so if no other single flag matches try to run like it an int level...
      // start game on specified level -- eg: 'pm.exe 211'
      int pl = atoi(argument_array[1]);
      if ((pl > 0) && (pl < 400))
      {
         mLogo.show_splash_screen = 0;
         mLevel.set_start_level(pl);
         printf("started game on level:%d\n", pl);
         mLoop.state[0] = PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME;
         mLoop.quit_action = 0;
         mLoop.done_action = 0;
      }
      else
      {
         printf("%s could not be parsed to an integer level number\n", argument_array[1]);
         printf("trying to run as a demo file\n");
         mLogo.show_splash_screen = 0;
         mDemoMode.run_single_from_cmd_fn(argument_array[1]);
         fast_exit(0);
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
            mLevel.set_start_level(pl);
            printf("running level editor for level:%d\n", pl);
            mLevel.set_start_level(mWM.loop(mLevel.start_level));
            fast_exit(0);
         }
         else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
      }
      if (strcmp(argument_array[1],"-c") == 0 )
      {
         char srv[256];
         sprintf(srv, "%s", argument_array[2]);
         if ((strlen(srv) == 2) && (strcmp(srv, "pm") == 0)) sprintf(srv, "%s", "purplemartians.org");

         sprintf(mNetgame.server_address, "%s", srv);


         mConfig.save_config(PM_CFG_SAVE_SERVER_ADDRESS);
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_NEW_GAME;
         mLoop.quit_action = 0;
         mLoop.done_action = 1;
         return;
      }





      if (strcmp(argument_array[1],"-s") == 0 )
      {
         int pl = atoi(argument_array[2]);
         if ((pl > 0) && (pl < 400))
         {
            mLevel.set_start_level(pl);
            mLogo.show_splash_screen = 0;
            mLoop.state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
            mLoop.quit_action = 0;
            mLoop.done_action = 1;
            return;
         }
         else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
      }

      if (strcmp(argument_array[1],"-sh") == 0 )
      {
         int pl = atoi(argument_array[2]);
         if ((pl > 0) && (pl < 400))
         {
            mNetgame.headless_server_setup();
            mLevel.set_start_level(pl);
            mLogo.show_splash_screen = 0;
            mLoop.state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
            mLoop.quit_action = 0;
            mLoop.done_action = 1;
            return;
         }
         else printf("%s could not be parsed to an integer level number\n", argument_array[2]);
      }

      if (strcmp(argument_array[1],"-rc") == 0 ) // server remote control
      {
         sprintf(mNetgame.server_address, "%s", argument_array[2]);
         mConfig.save_config(PM_CFG_SAVE_SERVER_ADDRESS);
         server_remote_control = 1;
         mLogo.show_splash_screen = 0;
         mLoop.state[0] = PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP;
         return;
      }


   } // end of argument_count == 3
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





//void draw_text_in_box(const char* txt, ALLEGRO_FONT *f, int x, int y, int color, int type)
//{
//   int bx, by, bw, bh;
//
//   if (type == 0) // the regular method
//   {
//      al_get_text_dimensions(f, txt, &bx, &by, &bw, &bh);
//      al_draw_rectangle(x, y, x+bw+4, y+bh+4, mColor.pc[color], 1);
//      al_draw_textf(f, mColor.pc[color], x+2, y+2, 0, txt);
//   }
//   if (type == 1) // use bx, by to offset text
//   {
//      al_get_text_dimensions(f, txt, &bx, &by, &bw, &bh);
//      al_draw_rectangle(x, y, x+bw+4, y+bh+4, mColor.pc[color], 1);
//      al_draw_textf(f, mColor.pc[color], x+2-bx, y+2-by, 0, txt);
//   }
//
//   if (type == 2) // use my method to force by offset and bh size
//   {
//      mw_get_text_dimensions(f, txt, bx, by, bw, bh);
//      al_draw_rectangle(x, y, x+bw+4, y+bh+4, mColor.pc[color], 1);
//      al_draw_textf(f, mColor.pc[color], x+2-bx, y+2-by, 0, txt);
//   }
//
//}
//

/*
static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

*/

#include "mwMiscFnx.h"
#include "mwBitmapTools.h"

#include "mwSql.h"
#include "mwDemoRecord.h"


void mwMain::temp_test(void)
{


   mDemoRecord.gdt();










//   void val_to_gma(int &type, int &d1, int &d2, int p, int c, char * name)
//   void gma_to_val(int type, int d1, int d2, int &p, int &c, char * name)

//   char name[9];
//   sprintf(name, "purple23");
//
//   int p = 5;
//   int c = 12;
//   int type, d1, d2;
//
//   mMiscFnx.val_to_gma(type, d1, d2, p, c, name);
//
////   mMiscFnx.printBits2(4, &type);
//
//
//
//   mMiscFnx.gma_to_val(type, d1, d2, p, c, name);
//


























//   char ts[256];
//   for (int i=0; i<1000; i++)
//      printf("test time %s\n", mMiscFnx.chr_dt(ts));




/*
#include <chrono>

   char ts[256];
   time_t now = time(NULL);
   struct tm *timenow = localtime(&now);
   strftime(ts, sizeof(ts), "%Y%m%d-%H%M%S", timenow);

//   printf("test time %s\n", ts);

   auto currentDateTime = std::chrono::system_clock::now();
   int ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;

   printf("test time %s.%d\n", ts, ms);

std::string get_current_time_and_date()
{
    auto const time = std::chrono::current_zone()
        ->to_local(std::chrono::system_clock::now());


    return std::format("{:%Y-%m-%d %X}", time);
}

auto currentDateTime = std::chrono::system_clock::now();

auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;

printf("ms: %d\n", ms);

currentDateTime = std::chrono::system_clock::now();
ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;
printf("ms: %d\n", ms);

//
//std::clog << std::put_time(&currentDateTimeLocalTime, "%Y%m%d_%H%M%S")
//    << "." << std::setfill('0') << std::setw(3) << ms << std::endl;

*/



/*

   sqlite3 *db;
   sqlite3_stmt* stmt;


   char q[1000];
	char* err;
   int exit;


   int row = 0;
   int dat;

   int rc = sqlite3_open("test.db", &db);

   if (rc)
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return;
   }

   fprintf(stderr, "Opened database successfully\n");

//
//   sprintf(q,  "DROP TABLE pm_log");
//
//   printf("Query: %s\n", q);
//	exit = sqlite3_exec(db, q, NULL, 0, &err);
//	if (exit != SQLITE_OK) printf("Error! (%s)\n", err);
//	else                   printf("Success!\n");
//
//
//
//   sprintf(q,"CREATE TABLE pm_log("
//  "id           INTEGER PRIMARY KEY,"
//  "received_at  datetime,"
//  "agt          double, "
//  "player_num   INTEGER,"
//  "message_type INTEGER,"
//  "frame_num    INTEGER,"
//  "message      TEXT); ");


//   printf("Query: %s\n", q);
//	exit = sqlite3_exec(db, q, NULL, 0, &err);
//	if (exit != SQLITE_OK) printf("Error! (%s)\n", err);
//	else                   printf("Success!\n");


   sprintf(q, "INSERT INTO pm_log (frame_num, player_num) VALUES(23, 7);"
              "INSERT INTO pm_log (frame_num, player_num) VALUES(24, 9);" );

   printf("Query: %s\n", q);
	exit = sqlite3_exec(db, q, NULL, 0, &err);
	if (exit != SQLITE_OK) printf("Error! (%s)\n", err);
	else                   printf("Success!\n");




   sprintf(q, "SELECT * FROM pm_log;");

   printf("STATE OF TABLE\n");

   sqlite3_exec(db, q, callback, NULL, NULL);


      sqlite3_close(db);

*/



   /*


  sprintf(q,  "CREATE TABLE PERSON("
					"ID INT PRIMARY KEY	 NOT NULL, "
					"NAME		 TEXT NOT NULL, "
					"SURNAME		 TEXT	 NOT NULL, "
					"AGE		 INT	 NOT NULL, "
					"ADDRESS	 CHAR(50), "
					"SALARY		 REAL );");

   printf("Create query: %s\n", q);

	char* mError;
	int exit = sqlite3_exec(db, q, NULL, 0, &mError);

	if (exit != SQLITE_OK)
   {
      printf("Error Creating Table (%s)\n", mError);
		sqlite3_free(mError);
	}
	else
   {
      printf("Table created Successfully\n");
   }
*/



   /*


   sprintf(q, "SELECT player_num FROM pm_log");

   printf("Query: %s\n", q);

   sqlite3_prepare(db, q, sizeof q, &stmt, NULL);

    bool done = false;
    while (!done)
    {

         printf("In select while\n");
         switch (sqlite3_step (stmt))
         {
            case SQLITE_ROW:
            dat = sqlite3_column_bytes(stmt, 0);
            printf ("row %d: %d\n", row, dat);
            row++;
            break;

            case SQLITE_DONE:
               done = true;
            break;

            default:
               fprintf(stderr, "Failed.\n");
            return;
        }
    }

    sqlite3_finalize(stmt);

*/











//   if (mNetgame.NetworkInit())
//   {
//      printf("Error: failed to initialize network\n");
//   }
//   mNetgame.Channel = net_openchannel(mNetgame.NetworkDriver, NULL); // dynamic port
//   if (mNetgame.Channel == NULL)
//   {
//      printf("Error: Client failed to create NetChannel\n");
//   }
//
//   char target[300];
//   sprintf(target, "%s:%d", "purplemartians.org", 514);
//   if (net_assigntarget(mNetgame.Channel, target))
//   {
//      printf("Error: Client failed to set NetChannel target:[%s]\n", target);
//   }
//   printf("Client network initialized -- target:[%s]\n", target);
//   printf("Local address:[%s]\n", net_getlocaladdress(mNetgame.Channel));


//   char BOM[3];
//   BOM[0] = 239;
//   BOM[1] = 187;
//   BOM[2] = 191;


//    unsigned char bom[] = { 0xEF,0xBB,0xBF };
//    f.write((char*)bom, sizeof(bom));


//   char data[50] = {0};
//   sprintf(data, "<1>1 grr %c%c%c arrgs [nice]", 239, 187, 191);
//
//   for (int i=0; i<(int)strlen(data); i++)
//   {
//      printf("%d %d %c\n", i, (int)data[i], data[i]);
//   }







//   char data[50] = {0};
//   sprintf(data, "<1>1 grr - - - arrgs [nice]");
//   net_send(mNetgame.Channel, data, strlen(data));


//   char data[1000] = {0};
//   sprintf(data, "<1>1 2003-10-11T22:17:15.003Z mymachine myapplication - - - message");
//   net_send(mNetgame.Channel, data, strlen(data));



//   char data[1000] = {0};
//   sprintf(data, "<1>1 - mymachie myapplication - - - message3");
//   sprintf(data, "<1>1 - mymachine myapplication - - - message7");
//   sprintf(data, "<1>1 message8");
//   net_send(mNetgame.Channel, data, strlen(data));





//   char msg[256];
//   time_t now = time(NULL);
//   struct tm *timenow = localtime(&now);
//   strftime(msg, sizeof(msg), "%Y%m%d-%H%M%S", timenow);
//   printf("%s\n", msg);






//   for (int i =0; i<256; i++)
//   {
//      int8_t i8 = i;
//      uint8_t ui8 = i;
//      printf("i8:%d ui8:%d\n", i8, ui8);
//   }





//   short int si;
//   int i;
//   long int li;
//   long long int lli;
//   printf("si:%d i:%d li%d lli:%d\n", (int) sizeof(si), (int) sizeof(i), (int) sizeof(li), (int) sizeof(lli));
//
//   int8_t i8;
//   int16_t i16;
//   int32_t i32;
//   int64_t i64;
//   printf("i8:%d i16:%d i32:%d i64:%d\n", (int) sizeof(i8), (int) sizeof(i16), (int) sizeof(i32), (int) sizeof(i64));
//
//
//   int64_t bmn = -9223372036854775807;
//   int64_t bmx =  9223372036854775807;
//   printf("min:%" PRId64 " max:%" PRId64 "\n", bmn, bmx);





//
//
//   al_set_target_backbuffer(mDisplay.display);
//   al_clear_to_color(mColor.pc[0]);
//   int quit = 0;
//   float x = mDisplay.SCREEN_W/2;
//   float y = mDisplay.SCREEN_H/2;
//   while (!quit)
//   {
//      float pi = ALLEGRO_PI;
//
//      float mx = mInput.mouse_x;
//      float my = mInput.mouse_y;
//      float xlen = mx-x;
//      float ylen = my-y;
//      float hy = sqrt(pow(xlen, 2) + pow(ylen, 2));
//      float ra = atan2(ylen, xlen) + pi/2;;
//
//      float a = ra;
//      if (ra < 0) a = ra + pi*2;
//      float a2 = 360 * (a/(pi*2));
//
//      int aa = a*1000;
//
//
//      al_draw_textf(mFont.pr8, mColor.pc[15], 100, 100, 0, "xlen:%f ylen:%f hy:%f", xlen, ylen, hy);
//      al_draw_textf(mFont.pr8, mColor.pc[15], 100, 110, 0, "ra:%f", ra);
//      al_draw_textf(mFont.pr8, mColor.pc[15], 100, 120, 0, "a+:%f", a);
//      al_draw_textf(mFont.pr8, mColor.pc[15], 100, 130, 0, "a2:%f", a2);
//
//
//
//      al_draw_line(x, y, mx, my, mColor.pc[15], 1);
//      al_draw_rotated_bitmap(mBitmap.tile[248], 10, 10, x, y, (float)aa/1000, 0);
//      al_flip_display();
//      al_clear_to_color(mColor.pc[0]);
//      mEventQueue.proc(1);
//      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
//   }
//
//
//






//
//   void convert_ttf_to_bitmap_font(const char* ttf_filename, const char* bmp_filename, int char_size);
//   convert_ttf_to_bitmap_font("Pristine.ttf", "Pristine_8.bmp", 8);
//   convert_ttf_to_bitmap_font("Pristine.ttf", "Pristine_16.bmp", 16);



// show char set of fonts
//   al_set_target_backbuffer(mDisplay.display);
//   al_clear_to_color(al_map_rgb(0, 0, 0));
//
//   for (int i=32; i<80; i++)
//      al_draw_textf(mFont.pr8, mColor.White, 10, (i-32)*8, 0, "%d '%c'     %d '%c'", i, i, i+48, i+48);
//
//   al_flip_display();
//   mInput.tsw();


//   ALLEGRO_FONT *f = mFont.pr8;
//   for (int j=0; j<3; j++)
//      for (int i=0; i<3; i++)
//      {
//         if (i == 0) f = mFont.pr8;
//         if (i == 1) f = font0;
//         if (i == 2) f = mFont.pixl;
//
//         draw_text_in_box("Hellg", f, 100, 100 + i*20 + j*100, 10, j);
//         draw_text_in_box("Hello", f, 150, 100 + i*20 + j*100, 10, j);
//         draw_text_in_box("emmem", f, 200, 100 + i*20 + j*100, 10, j);
//         draw_text_in_box("e{3}",  f, 250, 100 + i*20 + j*100, 10, j);
//      }
//
//   al_flip_display();
//   mInput.tsw();
//
//

//   // testing 3 segment line draw
//   int quit = 0;
//   while (!quit)
//   {
//      al_set_target_backbuffer(mDisplay.display);
//      al_clear_to_color(al_map_rgb(0, 0, 0));
//
//      int a = 10 - pct_y;
//      int b = 12;
//      int c = 13;
//
//      int lco = 0;
//
//      al_draw_textf(mFont.pr8, mColor.pc[15], 10, 10, 0, "a:%d b:%d c:%d  t:%d", a, b, c, a+b+c);
//
//
//      lco = 0;
//      mw_draw_line3(200, 200, 220, 200, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(220, 200, 220, 220, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(220, 220, 200, 220, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(200, 220, 200, 200, 1.0, a, 15, b, 10, c, 12, lco);
//
//
//      lco = 0;
//      mw_draw_line3(300, 300, 380, 300, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(380, 300, 380, 380, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(380, 380, 300, 380, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(300, 380, 300, 300, 1.0, a, 15, b, 10, c, 12, lco);
//
//      lco = 0;
//      mw_draw_line3(400, 400, 505, 400, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(505, 400, 505, 630, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(505, 630, 450, 630, 1.0, a, 15, b, 10, c, 12, lco);
//      mw_draw_line3(450, 630, 400, 400, 1.0, a, 15, b, 10, c, 12, lco);
//
//
//      al_flip_display();
//      mEventQueue.proc(1);
//      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
//   }
//

//   // testing 2 segment line draw
//   int quit = 0;
//   while (!quit)
//   {
//      al_set_target_backbuffer(mDisplay.display);
//      al_clear_to_color(al_map_rgb(0, 0, 0));
//
//      int a = 10 - pct_y;
//      int b = 12;
//
//      int lco = 0;
//
//      al_draw_textf(mFont.pr8, mColor.pc[15], 10, 10, 0, "a:%d b:%d t:%d", a, b, a+b);
//
//
//      lco = 0;
//      mw_draw_line2(200, 200, 220, 200, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(220, 200, 220, 220, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(220, 220, 200, 220, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(200, 220, 200, 200, 1.0, a, 15, b, 10, lco);
//
//
//      lco = 0;
//      mw_draw_line2(300, 300, 380, 300, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(380, 300, 380, 380, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(380, 380, 300, 380, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(300, 380, 300, 300, 1.0, a, 15, b, 10, lco);
//
//      lco = 0;
//      mw_draw_line2(400, 400, 505, 400, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(505, 400, 505, 630, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(505, 630, 450, 630, 1.0, a, 15, b, 10, lco);
//      mw_draw_line2(450, 630, 400, 400, 1.0, a, 15, b, 10, lco);
//
//
//      al_flip_display();
//      mEventQueue.proc(1);
//      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
//   }







//   void graph_test2(void);
//   graph_test2();



//   void load_ping_graph_from_static_file(const char* f);
//   load_ping_graph_from_static_file("logs/e.txt");



//   log_bandwidth_graph(load_log_lines_array_from_static_file("logs/a.txt"), 1);
//   log_client_server_sync_graph(load_log_lines_array_from_static_file("logs/a.txt"));
//   log_ping_graph(load_log_lines_array_from_static_file("logs/e.txt"));


//   void log_timestamp_graph(int num_lines);
//   log_timestamp_graph(load_log_lines_array_from_static_file("logs/g.txt"));

//
//void load_profile_graph(int num_lines);
//load_profile_graph(load_log_lines_array_from_static_file("logs/h.txt"));


//void load_profile_graph(void);
//load_profile_graph();


// load_profile_graph();




//   settings_pages(6);


//
//float v[4][2] = {0};
//
//v[0][0] = 100;
//v[0][1] = 100;
//
//v[1][0] = 100;
//v[1][1] = 200;
//
//v[2][0] = 200;
//v[2][1] = 200;
//
//v[3][0] = 200;
//v[3][1] = 100;
//
//
//al_draw_filled_rectangle(20,20,40,40, mColor.pc[10]);
//al_draw_filled_polygon(*v, 4, mColor.pc[14]);
//
//al_flip_display();
//
//mInput.tsw();
//
//


}
