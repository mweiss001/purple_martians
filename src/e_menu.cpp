// e_menu.cpp  (20100220 cleanup)
#include "pm.h"


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
   if (menu_num == 9) if (ky > SCREEN_H - 160) up=1;
   if (menu_num == 6) if (ky > SCREEN_H - 60)  up=1;

   if (!up) // reverse version !
   {
      do   // until selection is made
      {
         c = 0;
         al_rest(0.02);
         //show_mouse(NULL);
         while (strcmp(global_string[menu_num][c],"end") != 0)
         {
            b = 9 + 96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int tw = strlen(global_string[menu_num][c])*4;
            al_draw_filled_rectangle(kx-tw, ky+(c*8), kx+tw, ky+(c*8)+8, palette_color[0]);
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
            int tw = strlen(global_string[menu_num][c])*4;
            al_draw_filled_rectangle(kx-tw, ky-(c*8), kx+tw, ky-(c*8)+8, palette_color[0]);
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

void text_setup()
{
   extern char item_desc[20][40];
   extern char eftype_desc[50][16][40];
   extern char eitype_desc[50][32][40];

   strcpy (item_desc[0],"item_empty");
   strcpy (item_desc[1],"Door");
   strcpy (item_desc[2],"Bonus");
   strcpy (item_desc[3],"Exit");
   strcpy (item_desc[4],"Key");
   strcpy (item_desc[5],"Start");
   strcpy (item_desc[6],"Free Man");
   strcpy (item_desc[7],"Mine");
   strcpy (item_desc[8],"Bomb");
   strcpy (item_desc[9], "undefined");
   strcpy (item_desc[10],"Message");
   strcpy (item_desc[11],"Rocket");
   strcpy (item_desc[12],"Warp");
   strcpy (item_desc[14],"Switch");
   strcpy (item_desc[15],"Sproingy");

  // --- enemy type 0 variable descriptions ---
   strcpy (eitype_desc[0][0],"<empty>");
   strcpy (eitype_desc[0][1],"!rsvd bitmap");
   strcpy (eitype_desc[0][2],"!rsvd draw_mode");
   strcpy (eitype_desc[0][3],"ans");
   strcpy (eitype_desc[0][4],"ans type");
   strcpy (eitype_desc[0][5],"-");
   strcpy (eitype_desc[0][6],"-");
   strcpy (eitype_desc[0][7],"-");
   strcpy (eitype_desc[0][8],"-");
   strcpy (eitype_desc[0][9],"collision window");

   strcpy (eftype_desc[0][0],"x pos");
   strcpy (eftype_desc[0][1],"y pos");
   strcpy (eftype_desc[0][2],"x inc");
   strcpy (eftype_desc[0][3],"y inc");
   strcpy (eftype_desc[0][4],"health dec");
   strcpy (eftype_desc[0][5],"-");
   strcpy (eftype_desc[0][6],"-");
   strcpy (eftype_desc[0][7],"-");
   strcpy (eftype_desc[0][8],"-");
   strcpy (eftype_desc[0][9],"-");
   // --- enemy type 1 variable descriptions ---
   strcpy (eitype_desc[1][0],"FOLLOWER no walls, grav");
   strcpy (eitype_desc[1][1],"!rsvd bitmap");
   strcpy (eitype_desc[1][2],"!rsvd draw_mode");
   strcpy (eitype_desc[1][3],"ans");
   strcpy (eitype_desc[1][4],"ans type");
   strcpy (eitype_desc[1][5],"-");
   strcpy (eitype_desc[1][6],"-");
   strcpy (eitype_desc[1][7],"-");
   strcpy (eitype_desc[1][8],"-");
   strcpy (eitype_desc[1][9],"collision window");
   strcpy (eftype_desc[1][0],"x pos");
   strcpy (eftype_desc[1][1],"y pos");
   strcpy (eftype_desc[1][2],"x inc");
   strcpy (eftype_desc[1][3],"y inc");
   strcpy (eftype_desc[1][4],"health dec");
   strcpy (eftype_desc[1][5],"-");
   strcpy (eftype_desc[1][6],"-");
   strcpy (eftype_desc[1][7],"-");
   strcpy (eftype_desc[1][8],"-");
   strcpy (eftype_desc[1][9],"-");

   strcpy (eitype_desc[2][0],"FOLLOWER with walls, grav");
   strcpy (eitype_desc[2][1],"!rsvd bitmap");
   strcpy (eitype_desc[2][2],"!rsvd draw_mode");
   strcpy (eitype_desc[2][3],"ans");
   strcpy (eitype_desc[2][4],"ans type");
   strcpy (eitype_desc[2][5]," - ");
   strcpy (eitype_desc[2][6]," - ");
   strcpy (eitype_desc[2][7]," - ");
   strcpy (eitype_desc[2][8]," - ");
   strcpy (eitype_desc[2][9],"collision window");
   strcpy (eftype_desc[2][0],"x pos");
   strcpy (eftype_desc[2][1],"y pos");
   strcpy (eftype_desc[2][2],"x inc");
   strcpy (eftype_desc[2][3],"y inc");
   strcpy (eftype_desc[2][4],"health dec");
   strcpy (eftype_desc[2][5]," - ");
   strcpy (eftype_desc[2][6]," - ");
   strcpy (eftype_desc[2][7]," - ");
   strcpy (eftype_desc[2][8]," - ");
   strcpy (eftype_desc[2][9]," - ");

   strcpy (eitype_desc[3][0],"ArchWagon");
   strcpy (eitype_desc[3][1],"!rsvd bitmap");
   strcpy (eitype_desc[3][2],"!rsvd draw_mode");
   strcpy (eitype_desc[3][3],"ans");
   strcpy (eitype_desc[3][4],"ans type");
   strcpy (eitype_desc[3][5],"bullet wait (0)none");
   strcpy (eitype_desc[3][6],"jump when under player");
   strcpy (eitype_desc[3][7],"jump wait count");
   strcpy (eitype_desc[3][8],"x move (0)follow (1)bounce");
   strcpy (eitype_desc[3][9],"collision window");
   strcpy (eftype_desc[3][0],"x pos");
   strcpy (eftype_desc[3][1],"y pos");
   strcpy (eftype_desc[3][2],"x inc");
   strcpy (eftype_desc[3][3],"y inc");
   strcpy (eftype_desc[3][4],"health dec");
   strcpy (eftype_desc[3][5],"bullet prox wait count");
   strcpy (eftype_desc[3][6],"bullet prox");
   strcpy (eftype_desc[3][7],"bullet prox wait");
   strcpy (eftype_desc[3][8],"!rsvd fall count");
   strcpy (eftype_desc[3][9],"!rsvd jumpcount");

   strcpy (eitype_desc[4][0],"Bouncer");
   strcpy (eitype_desc[4][1],"!rsvd bitmap");
   strcpy (eitype_desc[4][2],"!rsvd draw_mode");
   strcpy (eitype_desc[4][3],"ans");
   strcpy (eitype_desc[4][4],"ans type");
   strcpy (eitype_desc[4][5],"main ans");
   strcpy (eitype_desc[4][6],"seek ans");
   strcpy (eitype_desc[4][7],"bounce count");
   strcpy (eitype_desc[4][8],"seek bounce count wait");
   strcpy (eitype_desc[4][9],"collision window");
   strcpy (eftype_desc[4][0],"x pos");
   strcpy (eftype_desc[4][1],"y pos");
   strcpy (eftype_desc[4][2],"x inc");
   strcpy (eftype_desc[4][3],"y inc");
   strcpy (eftype_desc[4][4],"health dec");
   strcpy (eftype_desc[4][5],"");
   strcpy (eftype_desc[4][6],"");
   strcpy (eftype_desc[4][7],"");
   strcpy (eftype_desc[4][8],"seek speed");
   strcpy (eftype_desc[4][9],"!rsvd sekk");

   strcpy (eitype_desc[6][0],"Cannon");
   strcpy (eitype_desc[6][1],"!rsvd bitmap");
   strcpy (eitype_desc[6][2],"!rsvd draw_mode");
   strcpy (eitype_desc[6][3],"");
   strcpy (eitype_desc[6][4],"");
   strcpy (eitype_desc[6][5],"");
   strcpy (eitype_desc[6][6],"bullet wait");
   strcpy (eitype_desc[6][7],"bounce count");
   strcpy (eitype_desc[6][8],"seek bounce count wait");
   strcpy (eitype_desc[6][9],"collision window");
   strcpy (eftype_desc[6][0],"x pos");
   strcpy (eftype_desc[6][1],"y pos");
   strcpy (eftype_desc[6][2],"x inc");
   strcpy (eftype_desc[6][3],"y inc");
   strcpy (eftype_desc[6][4],"health dec");
   strcpy (eftype_desc[6][5],"");
   strcpy (eftype_desc[6][6],"");
   strcpy (eftype_desc[6][7],"bullet speed");
   strcpy (eftype_desc[6][8],"seek speed");
   strcpy (eftype_desc[6][9],"resvd sekk");

   strcpy (eitype_desc[7][0],"PodZilla ");
   strcpy (eitype_desc[7][1],"!rsvd bitmap");
   strcpy (eitype_desc[7][2],"!rsvd draw_mode");
   strcpy (eitype_desc[7][3],"!rsvd wait count");
   strcpy (eitype_desc[7][4],"wait limit");
   strcpy (eitype_desc[7][5],"seq count");
   strcpy (eitype_desc[7][6],"seq limit");
   strcpy (eitype_desc[7][7],"mode");
   strcpy (eitype_desc[7][8],"trigger distance");
   strcpy (eitype_desc[7][9],"collision window");
   strcpy (eftype_desc[7][0],"x pos");
   strcpy (eftype_desc[7][1],"y pos");
   strcpy (eftype_desc[7][2],"x inc");
   strcpy (eftype_desc[7][3],"y inc");
   strcpy (eftype_desc[7][4],"health dec");
   strcpy (eftype_desc[7][5],"x dest");
   strcpy (eftype_desc[7][6],"y dest");
   strcpy (eftype_desc[7][7],"bullet speed");
   strcpy (eftype_desc[7][8],"");
   strcpy (eftype_desc[7][9],"speed");

   strcpy (eitype_desc[8][0],  "TrakBot");
   strcpy (eitype_desc[9][0],  "Cloner");
   strcpy (eitype_desc[11][0], "Block Walker");
   strcpy (eitype_desc[12][0], "Flapper");


   strcpy (global_string[5][0],"PUT"); // PD sub menu
   strcpy (global_string[5][1],"PREV");
   strcpy (global_string[5][2],"NEXT");
   strcpy (global_string[5][3],"");
   strcpy (global_string[5][4],"COPY");
   strcpy (global_string[5][5],"SAVE");
   strcpy (global_string[5][6],"LOAD");
   strcpy (global_string[5][7],"BACK");

   strcpy (global_string[9][0], "Level Editor Pop-Up Menu");
   strcpy (global_string[9][1], "------------------------");
   strcpy (global_string[9][2], "Copy ---");
   strcpy (global_string[9][3], "View ---");
   strcpy (global_string[9][4], "Delete ---");
   strcpy (global_string[9][5], "------------------------");
   strcpy (global_string[9][6], "Zoom Full Screen");
   strcpy (global_string[9][7], "Show Status Window");
   strcpy (global_string[9][8], "Show Selection Window");
   strcpy (global_string[9][9], "------");
   strcpy (global_string[9][10],"New Level");
   strcpy (global_string[9][11],"Load Level");
   strcpy (global_string[9][12],"Save Level");
   strcpy (global_string[9][13],"Save and Quit");
   strcpy (global_string[9][14],"Help Screens");
   strcpy (global_string[9][15],"Quit Level Editor");
   strcpy (global_string[9][16],"end");

#ifndef RELEASE
   strcpy (global_string[9][16],"----");
   strcpy (global_string[9][17],"Predefined Enemy Editor");
   strcpy (global_string[9][18],"Global Level Thingy!!");
   strcpy (global_string[9][19],"Level Viewer!");
   strcpy (global_string[9][20],"Bitmap and Animation Editor");
   strcpy (global_string[9][21],"end");
#endif



}












































