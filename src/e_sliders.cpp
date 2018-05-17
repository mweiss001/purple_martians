// e_sliders.cpp  (20100220 cleanup)
#include "pm.h"


extern int e_num_of_type[50];   // sort results
extern int e_first_num[50];
extern int item_first_num[20];
extern char item_desc[20][40];
extern int bts;



char smsg[80];
int bw = 3; // slider adjustment bar width

void update_var(int bn, int type, int num, float f)
{

   if (bn == 1) item[num][7] = (int)f;      // health bonus



   if (bn == 4) item[num][7] = (int)f;      // blast size
   if (bn == 5) item[num][9] = (int)f;      // fuse length
   if (bn == 6) item[num][9] = (int)f;      // accel
   if (bn == 7) item[num][8] = (int)f;      // max speed
   if (bn == 8) item[num][6] = (int)f;      // steerability
   if (bn == 9) item[num][6] = (int)f;      // jump length
   if (bn == 10) item[num][7] = (int)f;     // sproingieness
   if (bn == 11) item[num][8] = (int)f;     // mine damage
   if (bn == 12)                            // x - speed
   {
      Efi[num][6] = al_ftofix(f);
      if (Efi[num][2] > al_itofix(0)) Efi[num][2] = Efi[num][6];
      if (Efi[num][2] < al_itofix(0)) Efi[num][2] = -Efi[num][6];

/*
      Efi[num][2] = -Efi[num][6];

      Efi[num][6] = al_ftofix(f);
      if (Ei[num][2]) Efi[num][2] =  Efi[num][6];
      else            Efi[num][2] = -Efi[num][6];
*/


   }
   if (bn == 13) Efi[num][3] = al_ftofix(f);  // y - speed
   if (bn == 15) Ei[num][6] =  (int)f;     // jump wait count
   if (bn == 16) Ei[num][7] =  (int)f;     // jump under width
   if (bn == 17) Ei[num][12] = (int)f;     // jump before wall
   if (bn == 18) Ei[num][11] = (int)f;     // jump before hole
   if (bn == 19) Efi[num][7] = al_ftofix(f);  // bullet speed
   if (bn == 20) Ei[num][17] = (int)f;     // bullet prox
   if (bn == 21) Ei[num][15] = (int)f;     // retrigger time
   if (bn == 22)                           // scale bouncer speed
   {
      al_fixed old_speed = Efi[num][5];
      Efi[num][5] = al_ftofix(f);
      al_fixed ratio = al_fixdiv(Efi[num][5], old_speed);
      Efi[num][2] = al_fixmul(Efi[num][2], ratio);
      Efi[num][3] = al_fixmul(Efi[num][3], ratio);

      if (f == 0)
      {
         Efi[num][2] = al_itofix(0);
         Efi[num][3] = al_itofix(0);
      }
   }
   if (bn == 23) Ei[num][8] = (int)f;      // seek count
   if (bn == 24) Ei[num][29] = (int)f;     // collision box
   if (bn == 25) Efi[num][4] = al_ftofix(f);  // health decrement
   if (bn == 26) bts = (int)f;             // button height
   if (bn == 28) item[num][8] = (int)f;    // warp level
   if (bn == 29)                           // pod speed
   {
      Efi[num][9] = al_ftofix(f);
      recalc_pod(num);
   }
   if (bn == 30) Ei[num][9] = (int)f;      // pod wait time
   if (bn == 33) Efi[num][2] = al_ftofix(f);  // flapper x speed
   if (bn == 34)                           // cloner create delay
   {
      Ei[num][6] = (int)f;
      Ei[num][7] = (int)f;
   }
   if (bn == 35) Ei[num][9] = (int)f;         // cannon extra hits to kill
   if (bn == 36) Efi[num][3] = al_ftofix(f);     // flapper x speed
   if (bn == 38) Ei[num][17] = (int)f;        // flapper trigger box width
   if (bn == 39) Ei[num][18] = (int)f;        // flapper trigger box height
   if (bn == 40) Ei[num][19] = (int)f;        // flapper trigger box depth
   if ((bn == 38) ||(bn == 39) ||(bn == 40) ||(bn == 63))  // update flapper trigger box while adjusting
   {
       draw_big(1);
       show_big();
   }




   if (bn == 42) Ei[num][25] = (int)f;        // dead enemy health bonus

   if (bn == 43) lifts[num].width = (int) f;  // lift width
   if (bn == 44) lifts[num].height = (int) f; // lift height
   if (bn == 45) Efi[num][2] = al_ftofix(f);     // trakbot x speed
   if (bn == 46) Efi[num][3] = al_ftofix(f);     // trakbot y speed
   if (bn == 47) item[num][8] = (int)f;       // exit with x enemies left
   if (bn == 54) item[num][7] = (int)f;       // pop message delay

   if (bn == 60) Efi[num][5] = al_ftofix(f);       // flapper max x speed
   if (bn == 61) Efi[num][6] = al_ftofix(f);       // flapper x accel
   if (bn == 62) Efi[num][8] = al_ftofix(f);       // flapper yinc flap scaler

   if (bn == 63) Ei[num][20] =(int)f;       // flapper height above player

   if (bn == 71) lift_steps[type][num].val =(int)f;  // lift step move
   if (bn == 72) lift_steps[type][num].val =(int)f;  // lift step wait
   if (bn == 73) lift_steps[type][num].val =(int)f;  // lift step prox


   if (bn == 74) Ei[num][9] = (int)f;        // cloner created object time to live
   if (bn == 75) Ei[num][10] = (int)f;       // cloner max created objects active at one time


}


void fill_smsg_button(int bn, int obt, int type, int num)
{

   if (bn == 1) sprintf(smsg, "OK");
   if (bn == 3) sprintf(smsg, "Cancel");


   if (bn == 2)
   {
      if (item[num][3]) sprintf(smsg, "Fall:On ");
      else              sprintf(smsg, "Fall:Off");
   }
   if (bn == 4)
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      if (item[num][8] == 1) sprintf(smsg, "Set Desination Item (%d)", item[num][9]);
   }

   if (bn == 5) sprintf(smsg, "Get New Block Range");
   if (bn == 6) sprintf(smsg, "Set Initial Direction"); // rocket direction
   if (bn == 7) sprintf(smsg, "Edit Message");
   if (bn == 8) // archwag direction
   {
      if (Ei[num][2]) sprintf(smsg, "Initial Direction:Right");
      else            sprintf(smsg, "Initial Direction:Left ");
   }
   if (bn == 9) // archwag and walker
   {
      if (Ei[num][8]) sprintf(smsg, "Bounce Mode");
      else            sprintf(smsg, "Follow Mode");
   }
   if (bn == 10) sprintf(smsg, "Set Initial Direction"); // bouncer direction
   if (bn == 11) sprintf(smsg, "Set Initial Direction");// trakbot direction
   if (bn == 12) // trakbot drop mode
   {
      if (Ei[num][7] == 0) sprintf(smsg, "Drop Mode:Off");
      else                 sprintf(smsg, "Drop Mode:On ");
   }

   if (bn == 13) sprintf(smsg, "Main Shape");
   if (bn == 14) sprintf(smsg, "Seek Shape");
   if (bn == 15) sprintf(smsg, "Move Extended Position");
   if (bn == 16) sprintf(smsg, "Set Trigger Box");
   if (bn == 17) sprintf(smsg, "Set Source Area");
   if (bn == 18) sprintf(smsg, "Set Destination");

   if (bn == 19) sprintf(smsg, "Move");
   if (bn == 20) sprintf(smsg, "Create");
   if (bn == 21) sprintf(smsg, "Delete");
   if (bn == 22) sprintf(smsg, "Next");
   if (bn == 23) sprintf(smsg, "Prev");
   if (bn == 24) sprintf(smsg, "Copy to Draw Item");
   if (bn == 25) sprintf(smsg, "Viewer Help");
   if (bn == 26)
   {
      if (item[num][3] == 0) sprintf(smsg,  "    Stationary    ");
      if (item[num][3] == 1) sprintf(smsg,  "      Fall        ");
      if (item[num][3] == -1) sprintf(smsg, "      Carry       ");
      if (item[num][3] == -2) sprintf(smsg, "Carry Through Door");
   }
   if (bn == 27) // cloner trigger type
   {
      if (Ei[num][8] == 0) sprintf(smsg, "Trigger Type: Timer Runs  ");
      if (Ei[num][8] == 1) sprintf(smsg, "Trigger Type: Timer Resets");
      if (Ei[num][8] == 2) sprintf(smsg, "Trigger Type: Immediate   ");
   }
   if (bn == 28) sprintf(smsg, "Run Lifts");
   if (bn == 29) sprintf(smsg, "Name:%s", lifts[num] . lift_name);
   if (bn == 48) // key block erase type
   {
      if (item[num][12]) sprintf(smsg, "Erase Only Matching Blocks");
      else               sprintf(smsg, "Erase All Blocks In Range ");
   }
   if (bn == 49) // door type
   {
//      if (item[num][8] == 0) sprintf(smsg, "Exit Only");
//      if (item[num][8] == 1) sprintf(smsg, "Linked Destination (%d)", item[num][9]);
      if (item[num][8] == 0) sprintf(smsg, "Door Type:Exit Only");
      if (item[num][8] == 1) sprintf(smsg, "Door Type:Normal   ");
   }
   if (bn == 50) // door entry type
   {
      if (item[num][11] == 0) sprintf(smsg, "Enter Immediate  ");
      if (item[num][11] == 1) sprintf(smsg, "Enter with <up>  ");
      if (item[num][11] == 2) sprintf(smsg, "Enter with <down>");
      if (item[num][8] == 0 ) sprintf(smsg, "disabled");
   }
   if (bn == 51) // door show dest line type
   {
      if (item[num][12] == 0) sprintf(smsg, "Exit link: never show  ");
      if (item[num][12] == 1) sprintf(smsg, "Exit link: alway show  ");
      if (item[num][12] == 2) sprintf(smsg, "Exit link: when touched");
      if (item[num][8] == 0 ) sprintf(smsg, "disabled");
   }
   if (bn == 52) sprintf(smsg, "Door Shape");

   if (bn == 53) // door move type
   {
      if (item[num][7] == 0) sprintf(smsg, "Move Type: Automatic    ");
      if (item[num][7] == 1) sprintf(smsg, "Move Type: Force Instant");
      if (item[num][7] == 2) sprintf(smsg, "Move Type: Force Move   ");
      if (item[num][8] == 0) sprintf(smsg, "disabled");
   }
   if (bn == 55) sprintf(smsg, "Set Message Position");

   if (bn == 56)
   {
      if (Viewer_lock) sprintf(smsg,  "Locked");
      else             sprintf(smsg, "Unlocked");
   }
   if (bn == 57)
   {
       if (obt == 2) sprintf(smsg,"%s Help", item_desc[type]);
       if (obt == 3) sprintf(smsg,"%s Help", (const char *)eitype_desc[type]);
       if (obt == 4) sprintf(smsg,"Lift Help");
   }
   if (bn == 70) sprintf(smsg, "#  Step Type     ");
   if (bn == 74) sprintf(smsg, "%-2d Loop to Start ", num);




}

void fill_smsg_slider(int bn, int type, int num)
{
   if (bn == 1) sprintf(smsg, "Health Bonus:%d", item[num][7]);
   if (bn == 4) sprintf(smsg, "Damage Range:%d", item[num][7]);
   if (bn == 5) sprintf(smsg, "Fuse Length:%d",  item[num][9]);
   if (bn == 6) sprintf(smsg, "Acceleration:%d", item[num][9]);
   if (bn == 7) sprintf(smsg, "Maximum Speed:%d",item[num][8]);
   if (bn == 8) sprintf(smsg, "Steerability:%d", item[num][6]);
   if (bn == 10) sprintf(smsg, "Sproinginess:%d", item[num][7]);
   if (bn == 11) sprintf(smsg, "Mine Damage:%d", item[num][8]);
   if (bn == 12) sprintf(smsg, "X-Speed:%-1.2f", al_fixtof(Efi[num][6]));
   if (bn == 13) sprintf(smsg, "Y-Speed:%-1.2f", al_fixtof(Efi[num][3]));
   if (bn == 15)
   {
     if (Ei[num][6]) sprintf(smsg, "Jump Wait Count:%d",  Ei[num][6]);
     else            sprintf(smsg, "Jump Wait Count:Off");
   }
   if (bn == 16)
   {
      if (Ei[num][7]) sprintf(smsg, "Jump Under Width:%d",  Ei[num][7]);
      else            sprintf(smsg, "Jump Under Width:Off");
   }
   if (bn == 17)
   {
      if (Ei[num][12]) sprintf(smsg, "Jump Before Wall:%d",  Ei[num][12]);
      else             sprintf(smsg, "Jump Before Wall:Off");
   }
   if (bn == 18)
   {
      if (Ei[num][11]) sprintf(smsg, "Jump Before Hole:%d",  Ei[num][11]);
      else             sprintf(smsg, "Jump Before Hole:Off");
   }
   if (bn == 19) sprintf(smsg, "Bullet Speed:%-1.1f", al_fixtof(Efi[num][7]));
   if (bn == 20) sprintf(smsg, "Bullet Proximity:%d", Ei[num][17]);
   if (bn == 21) sprintf(smsg, "Bullet Retrigger Time:%d", Ei[num][15]);
   if (bn == 22) sprintf(smsg, "Speed:%-1.2f", al_fixtof(Efi[num][5]));
   if (bn == 23) sprintf(smsg, "Seek Count:%d", Ei[num][8]);
   if (bn == 24) sprintf(smsg, "Collision Box:%d", Ei[num][29]);
   if (bn == 25) sprintf(smsg, "Health Decrement:%-1.1f", al_fixtof(Efi[num][4]));


//   if (bn == 26) sprintf(smsg, "%s", "" ); // button height
   if (bn == 26) sprintf(smsg, "%d", bts ); // button height


   if (bn == 27) sprintf(smsg, "Initial Time:%d", item[num][8]);
   if (bn == 28) sprintf(smsg, "Warp Level:%d", item[num][8]);
   if (bn == 29) sprintf(smsg, "Speed:%-2.1f", al_fixtof(Efi[num][9]));
   if (bn == 30) sprintf(smsg, "Pause:%d", Ei[num][9]);
   if (bn == 33) sprintf(smsg, "X-Speed:%-1.2f", al_fixtof(Efi[num][2]));
   if (bn == 34) sprintf(smsg, "Delay Timer:%d", Ei[num][6]);
   if (bn == 35) sprintf(smsg, "Extra Hits to Kill:%d", Ei[num][9]);
   if (bn == 36) sprintf(smsg, "Y Speed:%-1.2f", al_fixtof(Efi[num][3]));
   if (bn == 38) sprintf(smsg, "Trigger Box Width:%d", Ei[num][17]);
   if (bn == 39) sprintf(smsg, "Trigger Box Height:%d", Ei[num][18]);
   if (bn == 40) sprintf(smsg, "Trigger Box Depth:%d", Ei[num][19]);
   if (bn == 41) sprintf(smsg, "Bullet Bonus:%d", Ei[num][24]);
   if (bn == 42) sprintf(smsg, "Health Bonus:%d", Ei[num][25]);
   if (bn == 43) sprintf(smsg, "Lift Width:%d",lifts[num].width);
   if (bn == 44) sprintf(smsg, "Lift Height:%d",lifts[num].height);
   if (bn == 45) sprintf(smsg, "X-speed:%-1.2f",al_fixtof(Efi[num][2]));
   if (bn == 46) sprintf(smsg, "Y-Speed:%-1.2f",al_fixtof(Efi[num][3]));
   if (bn == 47) sprintf(smsg, "Exit with %d enemies left",item[num][8]);
   if (bn == 54) sprintf(smsg, "Message display time:%d",item[num][7]);


   if (bn == 60) sprintf(smsg, "X Speed:%-3.2f",al_fixtof(Efi[num][5]));
   if (bn == 61) sprintf(smsg, "X Accel:%-3.2f",al_fixtof(Efi[num][6]));
   if (bn == 62) sprintf(smsg, "Y Flap :%-3.2f",al_fixtof(Efi[num][8]));

   if (bn == 63) sprintf(smsg, "Height Above Player:%d",Ei[num][20]);

   if (bn == 71) sprintf(smsg, "%-2d Move Speed:%-3d", num, lift_steps[type][num].val);
   if (bn == 72) sprintf(smsg, "%-2d Wait Time :%-3d", num, lift_steps[type][num].val);
   if (bn == 73) sprintf(smsg, "%-2d Prox Dist :%-3d", num, lift_steps[type][num].val);


   if (bn == 74) sprintf(smsg, "Created Objects Time To Live:%-2d", Ei[num][9]);
   if (bn == 75) sprintf(smsg, "Max Created Objects At One Time:%-2d", Ei[num][10]);
}



void mdw_colsel(int x1, int y1, int x2, int y2, int bn, int num,
              int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   // erase
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw colors (1-15)
   float a = x2-x1;      // range
   float b = a/15;       // color swatch width
   if (b<2) b = 2; // min width
   for (int c=0; c<15; c++)
      al_draw_filled_rectangle((int)(x1+c*b), y1, (int)(b+x1+c*b), y2, palette_color[(int)c+1]);

   // draw text
   if (bn == 2) sprintf(smsg, "Select Text Color");
   if (bn == 3) sprintf(smsg, "Select Frame Color");
   if (bn == 4) sprintf(smsg, "Select Lift Color");
   if (bn == 5) sprintf(smsg, "Select Door Color");
   al_draw_text(font, palette_color[0], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

    // draw outline
   al_draw_rectangle(x1, y1, x2, y2, palette_color[15], 1);

   // is mouse on button ?
   if ((mouse_x > x1) && (mouse_x < x2))
      if ((mouse_y > y1) && (mouse_y < y2))
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            int color = (int)(1+(mouse_x-x1)/b);
            if (bn == 2) item[num][8] = color;     // pmsg text color
            if (bn == 3) item[num][9] = color;     // pmsg frame color
            if (bn == 4) lifts[num].color = color; // lift color
            if (bn == 5)
            {
               item[num][6] = color;     // door color
               change_linked_door_color_and_shape(num);
            }
            Redraw = 1;
         }
}

void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   // erase with background color ( never shows unless if (aa > 224) break; is enabled below
   int d = y2-y1;
   // outline
   for (int c=0; c<d/2+1; c++)
   {
      int a;
      if (!q4) // frame fades from solid outer to black inner
      {
         a = 224 - (c*32); // color increment
         if (a<0) a = 0;
      }
      else // frame fades from black outer to solid inner
      {
         a = (c*32); // color increment
         // if (aa > 224) break; uncomment this line to let the background color q0 show through in the middle
         if (a>224) a = 224;
      }
      al_draw_rectangle(x1+c, y1+c, x2-c, y2-c, palette_color[q1+a], 1);
   }
}


int mdw_button(int x1, int y1, int x2, int y2, int bn, int num,
                int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);             // draw button frame
   fill_smsg_button(bn, obt, type, num);                                   // get button text
   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   if (bn == 13)
   {
      float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
      al_draw_rotated_bitmap(memory_bitmap[zz[0][Ei[num][5]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }
   if (bn == 14)
   {
      float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
      al_draw_rotated_bitmap(memory_bitmap[zz[0][Ei[num][6]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }

   if (bn == 52)
   {
      get_item_draw_shape(num);
      al_convert_mask_to_alpha(dtemp, al_map_rgb(0, 0, 0));
      al_set_target_backbuffer(display);
      al_draw_scaled_bitmap(dtemp, 0, 0, 20, 20, (x2+x1)/2+60, y1, bts-2, bts-2, 0) ;
   }
   // is mouse pressed on this button?
   if ((mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release

      if (bn == 1)   return 1;
      if (bn == 3)   return 1;

      if (bn == 2) item[num][3] = !item[num][3];
      if (bn == 4)
      {
         if (item[num][8] == 1) // Set Linked Item
         {
             int i = get_item((char *)"Select Another Door To Link To", 2, 1, num );
             if (i > -1) item[num][9] = i;
             Redraw = 1;
         }
      }
      if (bn == 5)
         if (getbox((char *) "Block Range", 2, 4, num) == 1)
         {
            if (--bx2 < bx1) bx2++;
            if (--by2 < by1) by2++;
            item[num][6] = bx1;
            item[num][7] = by1;
            item[num][8] = bx2;
            item[num][9] = by2;
            Redraw = 1;
         }
      if (bn == 6)
         if (getxy((char *)"Set New Direction", 97, 11, num) == 1)
         {
            set_rocket_rot(num, get100_x*20, get100_y*20);
            Redraw = 1;
         }
      if ((bn == 7) && (num != 999)) edit_pmsg_text(num, 0);

      if (bn == 8) // initial direction
      {
         Ei[num][2] = !Ei[num][2];
         if (Ei[num][2]) Efi[num][2] = Efi[num][6];
         else Efi[num][2] = -Efi[num][6];
         Redraw = 1;
      }
      if (bn == 9) Ei[num][8] = !Ei[num][8];
      if (bn == 10) // set new direction
         if (getxy((char *)"Set New Direction", 96, 4, num) == 1)
         {
            extern int get100_x, get100_y;
            set_xyinc_rot(num, get100_x*20, get100_y*20);
            Redraw = 1;
         }
      if (bn == 11) // trakbot direction
      {
         if (++Ei[num][5] > 7) Ei[num][5] = 0;
         set_trakbot_mode(num, Ei[num][5]);
         Redraw = 1;
      }
      if (bn == 12) Ei[num][7] = !Ei[num][7]; // trakbot drop mode
      if (bn == 13) // main ans
      {
         int main_ans = Ei[num][5];
         if (main_ans == 31) main_ans = 14;
         else
         {
            if (main_ans == 29) main_ans = 31;
            if (main_ans == 14) main_ans = 29;
         }
         Ei[num][5] = main_ans;
         Ei[num][3] = main_ans;
         Ei[num][1] = zz[5][main_ans];
      }
      if (bn == 14) // seek ans
      {
         int seek_ans = Ei[num][6];
         if (seek_ans == 31) seek_ans = 14;
         else
         {
            if (seek_ans == 29) seek_ans = 31;
            if (seek_ans == 14) seek_ans = 29;
         }
         Ei[num][6] = seek_ans;
      }
      if (bn == 15) move_pod_extended(num);
      if (bn == 16) move_trigger_box(num, 9);
      if (bn == 17)
         if (getbox((char *)"Cloner Source Area", 3, 9, num))
         {
            Ei[num][15] = bx1;
            Ei[num][16] = by1;
            Ei[num][19] = bx2-bx1;
            Ei[num][20] = by2-by1;
            Redraw = 1;
         }
      if (bn == 18)
         if (getxy((char *)"Set Cloner Destination", 98, 9, num) == 1)
         {
            Ei[num][17] = get100_x;
            Ei[num][18] = get100_y;
            Redraw = 1;
         }
      if (bn == 19)   return 1;
      if (bn == 20)   return 1;
      if (bn == 21)   return 1;
      if (bn == 22)   return 1;
      if (bn == 23)   return 1;
      if (bn == 24)   return 1;
      if (bn == 25)   return 1;
      if (bn == 26)
      {
         item[num][3]++;
         if (item[num][3] > 1) item[num][3] = -2;
      }
      if (bn == 27)
      {
         Ei[num][8]++;
         if (Ei[num][8] > 2) Ei[num][8] = 0;
      }
      if (bn == 28)   return 1;
      if (bn == 29)   return 1;

      if (bn == 48) // key block remove type
         item[num][12] = !item[num][12];

      if (bn == 49) // door type
      {
         item[num][8] = !item[num][8];
         // check for bad link
         int link = item[num][9];
         if (item[link][0] != 1) // link is not door
         {
            item[num][9] = num;  // link to self
            item[num][11] = 1;   // trigger with up
          }
         Redraw = 1;
      }
      if (bn == 50) // door entry type
      {
         item[num][11]++;
         if (item[num][11] > 2) item[num][11] = 0;
         Redraw = 1;
      }
      if (bn == 51) // door exit link draw type
      {
         item[num][12]++;
         if (item[num][12] > 2) item[num][12] = 0;
         Redraw = 1;
      }

      if (bn == 52) // door shape
      {
         int shape = item[num][13];
         if (shape == 448) shape = 1083;
         else
         {
            if (shape == 1083) shape = 448;
         }
         if ((shape != 448) && (shape != 1083)) shape = 1083;
         item[num][13] = shape;
         item[num][1] = shape;
         change_linked_door_color_and_shape(num);
         Redraw = 1;
      }
      if (bn == 53)
      {
         item[num][7]++;
         if (item[num][7] > 2) item[num][7] = 0;
      }
      if (bn == 55)
      {
         if (getxy((char *)"Set Message Position", 2, 1010, num) == 1)
         {
            item[num][10] = get100_x;
            item[num][11] = get100_y;
            draw_big(1);
         }
         //set_mouse_sprite(NULL);
         Redraw = 1;
       }
      if (bn == 56) Viewer_lock = !Viewer_lock;
      if (bn == 57) return 1;


   } // end of mouse pressed on button
   return 0;
}


float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col)
{
   float a, b, c, d, e, f;
   // get slider position
   a = sdx-sll; // relative postion
   b = sul-sll; // range
   c = a/b;     // ratio
   d = x2-x1;   // range
   e = d * c;   // range * old ratio
   f = e + x1;  // add offset
   int sx1 = (int)f - bw;
   int sx2 = (int)f + bw;
   // draw slider bar
   for (int i=0; i<bw+1; i++)
      al_draw_rectangle(sx1+i, y1+i, sx2-i, y2-i, palette_color[col+192-(i*64)], 1);

   // draw rectangle around slider bar to show highlight
   if (dm == 2) al_draw_rectangle(sx1-1, y1, sx2+1, y2, palette_color[15], 1);
   return f;
}


// q0 = background color; (not used)
// q1 = frame color
// q2 = text color    (use white 99% of time)
// q3 = slider color  (use white 99% of time)
// q4 = slider color  (draw frame mode) now i always use 1

void mdw_slider(int x1, int y1, int x2, int y2,
                    int bn, int num, int type, int obt,
                    int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   //ALLEGRO_BITMAP *tmp = NULL;
   float sdx, sul, sll, sinc, dsx;
   switch (bn)
   {
      case 1:  sul=100;  sll=2;  sinc=1;   sdx=item[num][7];         break;  // health bonus
      case 2:  sul=200;  sll=2;  sinc=1;   sdx=item[num][8];         break;  // bullet bonus
      case 3:  sul=400;  sll=5;  sinc=1;   sdx=item[num][9];         break;  // timer bonus
      case 4:  sul=800;  sll=20; sinc=1;   sdx=item[num][7];         break;  // blast size
      case 5:  sul=2000; sll=1;  sinc=1;   sdx=item[num][9];         break;  // fuse length
      case 6:  sul=200;  sll=1;  sinc=1;   sdx=item[num][9];         break;  // accel
      case 7:  sul=20;   sll=1;  sinc=1;   sdx=item[num][8];         break;  // max speed
      case 8:  sul=50;   sll=1;  sinc=1;   sdx=item[num][6];         break;  // steerability
      case 9:  sul=7;    sll=1;  sinc=1;   sdx=item[num][6];         break;  // jump length
      case 10: sul=200;  sll=40; sinc=1;   sdx=item[num][7];         break;  // sproinginess
      case 11: sul=20;   sll=1;  sinc=1;   sdx=item[num][8];         break;  // mine damage
      case 12: sul=9;    sll=.7; sinc=.01; sdx=al_fixtof(Efi[num][6]);  break;  // archwagon x speed
      case 13: sul=9;    sll=.7; sinc=.01; sdx=al_fixtof(Efi[num][3]);  break;  // y speed
      case 15: sul=500;  sll=0;  sinc=1;   sdx=Ei[num][6];           break;  // jump wait count
      case 16: sul=600;  sll=0;  sinc=1;   sdx=Ei[num][7];           break;  // jump under width
      case 17: sul=100;  sll=0;  sinc=1;   sdx=Ei[num][12];          break;  // jump before wall
      case 18: sul=40;   sll=0;  sinc=1;   sdx=Ei[num][11];          break;  // Jump before hole
      case 19: sul=20;   sll=.8; sinc=.1;  sdx=al_fixtof(Efi[num][7]);  break;  // bullet speed
      case 20: sul=600;  sll=20; sinc=1;   sdx=Ei[num][17];          break;  // bullet prox
      case 21: sul=200;  sll=1;  sinc=1;   sdx=Ei[num][15];          break;  // retrigger time
      case 22: sul=12;   sll=0;  sinc=.01; sdx=al_fixtof(Efi[num][5]);  break;  // cannon speed
      case 23: sul=100;  sll=0;  sinc=1;   sdx=Ei[num][8];           break;  // seek count
      case 24: sul=20;   sll=0;  sinc=1;   sdx=Ei[num][29];          break;  // collision box
      case 25: sul=10;   sll=0;  sinc=.1;  sdx=al_fixtof(Efi[num][4]);  break;  // health dec
      case 26: sul=40;   sll=4;  sinc=1;   sdx=bts;                  break;  // button height
      case 27: sul=800;  sll=10; sinc=1;   sdx=item[num][8];         break;  // initial time
      case 28: sul=100;  sll=1;  sinc=1;   sdx=item[num][8];         break;  // warp level
      case 29: sul=30;   sll=.5; sinc=.5;  sdx=al_fixtof(Efi[num][9]);  break;  // pod speed
      case 30: sul=40;   sll=0;  sinc=1;   sdx=Ei[num][9];           break;  // pod wait time
      case 33: sul=5;    sll=.7; sinc=.1;  sdx=al_fixtof(Efi[num][2]);  break;  // flapper x speed
      case 34: sul=1000; sll=20; sinc=1;   sdx=Ei[num][6];           break;  // create delay
      case 35: sul=20;   sll=0;  sinc=1;   sdx=Ei[num][9];          break;  // cannon hits
      case 36: sul=5;    sll=0;  sinc=.01; sdx=al_fixtof(Efi[num][3]);  break;  // flapper y speed
      case 38: sul=500;  sll=20; sinc=1;   sdx=Ei[num][17];          break;  // width
      case 39: sul=600;  sll=1;  sinc=10;  sdx=Ei[num][18];          break;  // y1
      case 40: sul=600;  sll=1;  sinc=10;  sdx=Ei[num][19];          break;  // y2
      case 41: sul=50;   sll=0;  sinc=1;   sdx=Ei[num][24];          break;  // dead enemy bullet bonus
      case 42: sul=50;   sll=0;  sinc=1;   sdx=Ei[num][25];          break;  // dead enemy health bonus
      case 43: sul=99;   sll=1;  sinc=1;   sdx=lifts[num].width;     break;  // lift width
      case 44: sul=99;   sll=1;  sinc=1;   sdx=lifts[num].height;    break;  // lift heigth
      case 45: sul=10;   sll=.5; sinc=.01; sdx=al_fixtof(Efi[num][2]);  break;  // trakbot x speed
      case 46: sul=10;   sll=.5; sinc=.01; sdx=al_fixtof(Efi[num][3]);  break;  // trakbot y speed
      case 47: sul=100;  sll=0;  sinc=1;   sdx=item[num][8];         break;  // exit with x enemies left
      case 54: sul=200;  sll=1;  sinc=1;   sdx=item[num][7];         break;  // Message delay time

      case 60: sul=8;   sll=.5;  sinc=.1;  sdx=al_fixtof(Efi[num][5]);  break;  // flapper max x speed
      case 61: sul=2;   sll=.01; sinc=.01; sdx=al_fixtof(Efi[num][6]);  break;  // flapper x accel
      case 62: sul=4;   sll=0;   sinc=.1;  sdx=al_fixtof(Efi[num][8]);  break;  // flapper yinc flap scale

      case 63: sul=1000; sll=-1000; sinc=10;  sdx=Ei[num][20];       break;  // height above player

      case 71: sul=29;    sll=4;  sinc=1;  sdx=lift_steps[type][num].val;   break;  // lift move speed
      case 72: sul=2000; sll=10;  sinc=10; sdx=lift_steps[type][num].val;   break;  // lift wait time
      case 73: sul=200;  sll=20;  sinc=10; sdx=lift_steps[type][num].val;   break;  // lift prox dist

      case 74: sul=4800;  sll=0;  sinc=1;  sdx=Ei[num][9];           break;  // cloner created obj time to live
      case 75: sul=600;   sll=0;  sinc=1;  sdx=Ei[num][10];          break;  // cloner max created obj at one time

   }

   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);
   fill_smsg_slider(bn, type, num);
   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // is mouse on adjustment bar?
   if ((mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      //Redraw = 2; // flag that we are in charge of drawing the mouse
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
         Redraw = 1;  // flag to not draw screen buffer until refreshed because it will have old position
         while (mouse_b3)
         {
            if (mouse_dz)
            {
               int dif = mouse_dz;
               mouse_dz = 0;

               f += dif * sinc;                  // only allow increments of sinc
               if (f < sll) f = sll;             // limit check
               if (f > sul) f = sul;
               f = round(f/sinc) * sinc;         // round to sinc
               update_var(bn, type, num, f);
            }
            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            fill_smsg_slider(bn, type, num);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();

         } // end of mouse b4 held
      } // end of mouse b4 pressed


      if (mouse_b1) // only when initially clicked
      {
         //Redraw = 1;  // flag to not draw screen buffer until refreshed because it will have old position
         while (mouse_b1)
         {
            float my = mouse_y;
            float mx = mouse_x;
            float a, b, c, d, e, f ;

            // enforce limits
            if (my<y1) my = y1;
            if (mx<x1) mx = x1;
            if (my>y2) my = y2;
            if (mx>x2) mx = x2;

            // get slider position
            a = mx-x1;                  // relative postion of slider bar in range
            b = x2-x1;                  // range
            c = a / b;                  // ratio = position / range
            d = sul-sll;                // range from buttons
            e = c * d;                  // ratio * range
            f = e + sll;                // add to ll
            f = round(f/sinc) * sinc;   // round to sinc
            update_var(bn, type, num, f);

            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            fill_smsg_slider(bn, type, num);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();
         }  // end of mouse b1 held
      }  // end of mouse b1 pressed
   }
}






























































