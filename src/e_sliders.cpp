// e_sliders.cpp

#include "pm.h"

char smsg[80];
int bw = 3; // slider adjustment bar width


// ------------------------------------------------------------------------------------
// ---------------------------sliders--------------------------------------------------
// ------------------------------------------------------------------------------------

void fill_smsg_slider(int bn, int type, int num)
{

   if (bn == 22) sprintf(smsg, "Speed:%-1.2f", al_fixtof(Efi[num][5])); // bouncer and cannon speed
   if (bn == 29) sprintf(smsg, "Speed:%-2.1f", al_fixtof(Efi[num][9])); // pod speed

   if (bn == 6)  sprintf(smsg, "Acceleration:%d", item[num][9]);
   if (bn == 7)  sprintf(smsg, "Maximum Speed:%d",item[num][8]);
   if (bn == 8)  sprintf(smsg, "Steerability:%d", item[num][6]);
   if (bn == 10) sprintf(smsg, "Sproinginess:%d", item[num][7]);

   if (bn == 27) sprintf(smsg, "Initial Time:%d", item[num][8]);
   if (bn == 28) sprintf(smsg, "Warp Level:%d", item[num][8]);
   if (bn == 33) sprintf(smsg, "X-Speed:%-1.2f", al_fixtof(Efi[num][2]));


   if (bn == 43) sprintf(smsg, "Lift Step Width:%d",lifts[num].width);
   if (bn == 44) sprintf(smsg, "Lift Step Height:%d",lifts[num].height);
   if (bn == 54) sprintf(smsg, "Message display time:%d",item[num][7]);

   if (bn == 78) sprintf(smsg, "Start Index:%d",item[num][7]);
   if (bn == 82) sprintf(smsg, "Damage Field Timer:%d", Ei[num][6]);
   if (bn == 83) sprintf(smsg, "Player Damage:%3.2f          ", al_fixtof(Efi[num][4]));
   if (bn == 84) sprintf(smsg, "Damage Field Follows Lift:%d", Ei[num][21]);
   if (bn == 85) sprintf(smsg, "Trigger Field Follows Lift:%d", Ei[num][20]);
   if (bn == 86) sprintf(smsg, "Total Time:%d",   Ei[num][6]);
   if (bn == 87) sprintf(smsg, "Initial Time:%d", Ei[num][7]);
   if (bn == 88) sprintf(smsg, "Damage Time:%d",  Ei[num][8]);
   if (bn == 89) sprintf(smsg, "Damage Field ON Time:%d",  Ei[num][6]);
   if (bn == 90) sprintf(smsg, "Damage Field OFF Time:%d",  Ei[num][6]);
   if (bn == 91) sprintf(smsg, "Trigger Field Follows Lift:%d", item[num][10]);

   if (bn == 92)
   {
      if (item[num][1]) sprintf(smsg, "Event Trigger:%2d ", item[num][1]); // block manip trigger input
      else              sprintf(smsg, "Event Trigger:OFF");
   }

   if (bn == 93)
   {
      if (item[num][11]) sprintf(smsg, "Set Event Always While On   :%2d ", item[num][11]); // trigger output
      else               sprintf(smsg, "Set Event Always While On   :OFF");
   }
   if (bn == 94)
   {
      if (item[num][12]) sprintf(smsg, "Set Event Always While Off  :%2d ", item[num][12]); // trigger output
      else               sprintf(smsg, "Set Event Always While Off  :OFF");
   }

   if (bn == 95)
   {
      if (item[num][13]) sprintf(smsg, "Set Event When Switching On :%2d ", item[num][13]); // trigger output
      else               sprintf(smsg, "Set Event When Switching On :OFF");
   }

   if (bn == 96)
   {
      if (item[num][14]) sprintf(smsg, "Set Event When Switching Off:%2d ", item[num][14]); // trigger output
      else               sprintf(smsg, "Set Event When Switching Off:OFF");
   }


   if (bn == 97) sprintf(smsg, "Damage Field Follows Lift:%d", item[num][10]);


   if (bn == 98) sprintf(smsg, "Player Damage:%-4d        ", item[num][15]); // item damage player damage amount


   if (bn == 100) sprintf(smsg, "Total Time:%d",   item[num][12]);
   if (bn == 101) sprintf(smsg, "Initial Time:%d", item[num][13]);
   if (bn == 102) sprintf(smsg, "Damage Time:%d",  item[num][14]);

   if (bn == 103) sprintf(smsg, "ON Time:%d",   item[num][12]);
   if (bn == 104) sprintf(smsg, "OFF Time:%d",  item[num][12]);


   // lifts ----------------------------
   if (bn == 550) sprintf(smsg, "Speed:%d",         lift_steps[num][type].val);  // lift step move and resize time
   if (bn == 551) sprintf(smsg, "Width:%d",         lift_steps[num][type].w);    // lift step width
   if (bn == 552) sprintf(smsg, "Height:%d",        lift_steps[num][type].h);    // lift step height
   if (bn == 553) sprintf(smsg, "Timer:%-3d",       lift_steps[num][type].val);  // lift step wait timer
   if (bn == 554) sprintf(smsg, "Distance:%-3d",    lift_steps[num][type].val);  // lift step wait player prox distance
   if (bn == 555) sprintf(smsg, "Reset Timer:%-3d", lifts[num].val2);            // lift mode 1 player ride timer
   if (bn == 556) sprintf(smsg, "Trigger:%-2d",     lift_steps[num][type].val);  // lift step wait trigger

}

void update_var(int bn, int type, int num, float f)
{
   if (bn == 22)                           // scale bouncer and cannon speed
   {
      al_fixed old_speed = Efi[num][5];
      Efi[num][5] = al_ftofix(f);
      al_fixed fratio = al_fixdiv(Efi[num][5], old_speed);
      Efi[num][2] = al_fixmul(Efi[num][2], fratio);
      Efi[num][3] = al_fixmul(Efi[num][3], fratio);

      if (f == 0)
      {
         Efi[num][2] = al_itofix(0);
         Efi[num][3] = al_itofix(0);
      }
   }
   if (bn == 29)                           // pod speed
   {
      Efi[num][9] = al_ftofix(f);
      recalc_pod(num);
   }



   if (bn == 6) item[num][9] = (int)f;      // accel
   if (bn == 7) item[num][8] = (int)f;      // max speed
   if (bn == 8) item[num][6] = (int)f;      // steerability
   if (bn == 9) item[num][6] = (int)f;      // jump length
   if (bn == 10) item[num][7] = (int)f;     // sproingieness



   if (bn == 28) item[num][8] = (int)f;    // warp level





   if (bn == 33) Efi[num][2] = al_ftofix(f);  // flapper x speed
   if (bn == 43) lifts[num].width = (int) f;  // lift width
   if (bn == 44) lifts[num].height = (int) f; // lift height
   if (bn == 54) item[num][7] = (int)f;       // pop message delay


   if (bn == 78) item[num][7] = (int)f;       // start index

   if (bn == 91)
   {
      item[num][10] = (int)f;                           // item trigger lift number
      set_item_trigger_location_from_lift(num, 1);
   }

   if (bn == 92) item[num][1] = (int)f; // block manip pm_event

   if (bn == 93) item[num][11] = (int)f; // // trigger output CURR ON
   if (bn == 94) item[num][12] = (int)f; // // trigger output CURR OFF
   if (bn == 95) item[num][13] = (int)f; // // trigger output TGON
   if (bn == 96) item[num][14] = (int)f; // // trigger output TGOF


   if (bn == 97)
   {
      item[num][10] = (int)f;                     // lift that damage field is slaved to
      //set_item_damage_location_from_lift(num, 1);
   }


   if (bn == 98) item[num][15] = (int)f; // item damage player amount


   if (bn == 100) item[num][12] = (int)f; // item damage total time
   if (bn == 101) item[num][13] = (int)f; // item damage initial time
   if (bn == 102) item[num][14] = (int)f; // item damage damage time

   if (bn == 103) item[num][12] = (int)f; // Damage Field ON Time
   if (bn == 104) item[num][12] = (int)f; // Damage Field OFF Time


   // lifts ---------------------------------
   if (bn == 550) lift_steps[num][type].val = (int)f; // lift step resize speed
   if (bn == 551) lift_steps[num][type].w   = (int)f; // lift step width
   if (bn == 552) lift_steps[num][type].h   = (int)f; // lift step height
   if (bn == 553) lift_steps[num][type].val = (int)f; // lift step wait timer
   if (bn == 554) lift_steps[num][type].val = (int)f; // lift step wait player prox distance
   if (bn == 555) lifts[num].val2           = (int)f; // lift mode 1 player ride timer
   if (bn == 556) lift_steps[num][type].val = (int)f; // lift step wait trigger

}



// q0 = background color; (not used)
// q1 = frame color
// q2 = text color    (use white 99% of time)
// q3 = slider color  (use white 99% of time)
// q4 = slider color  (draw frame mode) now i always use 1
// q5 = text justify  (0-center 1-left...buttons only)
// modified to be display only if q7 == 1
void mdw_slider(int x1, int y1, int x2, int y2,
                    int bn, int num, int type, int obt,
                    int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   float sdx, sul, sll, sinc, dsx;
   switch (bn)
   {
      case 22: sul=12;   sll=0;     sinc=.01; sdx=al_fixtof(Efi[num][5]);      break;  // cannon speed
      case 29: sul=30;   sll=.5;    sinc=.5;  sdx=al_fixtof(Efi[num][9]);      break;  // pod speed

      case 2:  sul=200;  sll=2;     sinc=1;   sdx=item[num][8];                break;  // bullet bonus
      case 3:  sul=400;  sll=5;     sinc=1;   sdx=item[num][9];                break;  // timer bonus


      case 6:  sul=200;  sll=1;     sinc=1;   sdx=item[num][9];                break;  // accel
      case 7:  sul=20;   sll=1;     sinc=1;   sdx=item[num][8];                break;  // max speed
      case 8:  sul=50;   sll=1;     sinc=1;   sdx=item[num][6];                break;  // steerability
      case 9:  sul=7;    sll=1;     sinc=1;   sdx=item[num][6];                break;  // jump length
      case 10: sul=200;  sll=40;    sinc=1;   sdx=item[num][7];                break;  // sproinginess

      case 27: sul=800;  sll=10;    sinc=1;   sdx=item[num][8];                break;  // initial time
      case 28: sul=100;  sll=1;     sinc=1;   sdx=item[num][8];                break;  // warp level
      case 33: sul=5;    sll=.7;    sinc=.1;  sdx=al_fixtof(Efi[num][2]);      break;  // flapper x speed


      case 43: sul=1999; sll=4;     sinc=1;   sdx=lifts[num].width;            break;  // lift width
      case 44: sul=1999; sll=4;     sinc=1;   sdx=lifts[num].height;           break;  // lift heigth
      case 54: sul=200;  sll=1;     sinc=1;   sdx=item[num][7];                break;  // Message delay time






      case 78: sul=20;   sll=0;     sinc=1;   sdx=item[num][7];                break;  // Start index



      case 82: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][6];                  break;  // field timer
      case 83: sul=100;  sll=.01;   sinc=.01; sdx=al_fixtof(Efi[num][4]);      break;  // field damage

      case 84: sul=39;   sll=0;     sinc=1;   sdx=Ei[num][21];                 break;  // damage lift number
      case 85: sul=39;   sll=0;     sinc=1;   sdx=Ei[num][20];                 break;  // trigger lift number
      case 86: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][6];                  break;  // field timer total time
      case 87: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][7];                  break;  // field timer initial time
      case 88: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][8];                  break;  // field timer damage time

      case 89: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][6];                  break;  // damage field on time
      case 90: sul=1000; sll=0;     sinc=1;   sdx=Ei[num][6];                  break;  // damage field off time

      case 91: sul=39;   sll=0;     sinc=1;   sdx=item[num][10];               break;  // item trigger lift number

      case 92: sul=99;   sll=0;     sinc=1;   sdx=item[num][1];                break;  // block manip pm_event trigger

      case 93: sul=99;   sll=0;     sinc=1;   sdx=item[num][11];                break;  // trigger output CURR ON
      case 94: sul=99;   sll=0;     sinc=1;   sdx=item[num][12];                break;  // trigger output CURR OFF
      case 95: sul=99;   sll=0;     sinc=1;   sdx=item[num][13];                break;  // trigger output TGON
      case 96: sul=99;   sll=0;     sinc=1;   sdx=item[num][14];                break;  // trigger output TGOF

      case 97: sul=39;   sll=0;     sinc=1;   sdx=item[num][10];               break;  // damage lift number

      case 98: sul=2000; sll=-2000; sinc=1;   sdx=item[num][15];               break;  // item damage player amount

      case 100: sul=1000; sll=0;    sinc=1;   sdx=item[num][12];               break;  // item damage total time
      case 101: sul=1000; sll=0;    sinc=1;   sdx=item[num][13];               break;  // item damage total time
      case 102: sul=1000; sll=0;    sinc=1;   sdx=item[num][14];               break;  // item damage total time

      case 103: sul=1000; sll=0;    sinc=1;   sdx=item[num][12];               break;  // item damage field on time
      case 104: sul=1000; sll=0;    sinc=1;   sdx=item[num][12];               break;  // item damage field off total time

      // lifts --------------------------------------
      case 550: sul=1000; sll=1;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step resize speed
      case 551: sul=1600; sll=20;   sinc=1;   sdx=lift_steps[num][type].w;     break;  // lift step width
      case 552: sul=1600; sll=20;   sinc=1;   sdx=lift_steps[num][type].h;     break;  // lift step height
      case 553: sul=2000; sll=1;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step wait time
      case 554: sul=200;  sll=20;   sinc=10;  sdx=lift_steps[num][type].val;   break;  // lift step wait player prox distance
      case 555: sul=2000; sll=1;    sinc=1;   sdx=lifts[num].val2;             break;  // lift mode 1 player ride timer
      case 556: sul=99;   sll=0;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step wait trigger


   }


   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);
   fill_smsg_slider(bn, type, num);
   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // is mouse on adjustment bar?
   if ((!q7) && (mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
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

void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   if (q1 != -1)
   {
      // erase with background color ( never shows unless if (aa > 224) break; is enabled below
      int d = y2-y1;
      // outline
      for (int c=0; c<d/2+1; c++)
      {
         int a;
         if (q4) // frame fades from black outer to solid inner
         {
            a = (c*32); // color increment
            // if (aa > 224) break; uncomment this line to let the background color q0 show through in the middle
            if (a>224) a = 224;
         }
         else // frame fades from solid outer to black inner
         {
            a = 224 - (c*32); // color increment
            if (a<0) a = 0;
         }
   //      al_draw_rectangle(x1+c, y1+c, x2-c, y2-c, palette_color[q1+a], 1);
         al_draw_rounded_rectangle(x1+c, y1+c, x2-c, y2-c, 1, 1, palette_color[q1+a], 1);
      }
   }
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



// modified to be display only if q7 == 1


void mdw_slider0_int(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2)
{
   int y2 = y1+bts-2;

   float sdx = (float) var;
   float dsx;

   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);


   if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
   else          sprintf(smsg, "%s%d", txt, var);



   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // is mouse on adjustment bar?
   if ((!q7) && (mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
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
               var = (int)f; // update var
            }
            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
            else          sprintf(smsg, "%s%d", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();

         } // end of mouse b4 held
      } // end of mouse b4 pressed
      if (mouse_b1) // only when initially clicked
      {
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
            var = (int)f; // update var


            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
            else          sprintf(smsg, "%s%d", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();
         }  // end of mouse b1 held
      }  // end of mouse b1 pressed
   }
   if (q6 == 1) y1+=bts;
}














// modified to be display only if q7 == 1
// y1 is passed as a reference and is modified by height (bts) if q6 == 1
void mdw_slider2_int(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt)
{
   int y2 = y1+bts-2;
   float sdx = (float) var;
   float dsx;

   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);

   sprintf(smsg, "%s%d", txt, var);
   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // is mouse on adjustment bar?
   if ((!q7) && (mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
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
               var = (int)f; // update var
            }
            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%d", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();

         } // end of mouse b4 held
      } // end of mouse b4 pressed
      if (mouse_b1) // only when initially clicked
      {
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
            var = (int)f; // update var


            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%d", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();
         }  // end of mouse b1 held
      }  // end of mouse b1 pressed
   }

   if (q6 == 1) y1+=bts;

}




// modified to be display only if q7 == 1
void mdw_slider2_fix(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 al_fixed &var, float sul, float sll, float sinc, const char *txt)
{
   int y2 = y1+bts-2;

   float sdx = al_fixtof(var);
   float dsx;

   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);

   sprintf(smsg, "%s%3.2f", txt, al_fixtof(var));

   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // is mouse on adjustment bar?
   if ((!q7) && (mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
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
               var = al_ftofix(f); // update var
            }
            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%3.2f", txt, al_fixtof(var));
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();

         } // end of mouse b4 held
      } // end of mouse b4 pressed
      if (mouse_b1) // only when initially clicked
      {
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
            var = al_ftofix(f); // update var

            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%3.2f", txt, al_fixtof(var));
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();
         }  // end of mouse b1 held
      }  // end of mouse b1 pressed
   }
   if (q6 == 1) y1+=bts;
}





// modified to accept no input if q7 == 1
void mdw_slider2_flt(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt)
{
   float sdx = var;
   float dsx;

   // draw the slider
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);

   sprintf(smsg, "%s%3.2f", txt, var);

   al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);



   // is mouse on adjustment bar?
   if ((!q7) && (mouse_x > dsx-bw) && (mouse_x < dsx+bw) && (mouse_y > y1) && (mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

      if (mouse_b3) // only when initially clicked
      {
         float f = sdx; //initial value
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
               var = f; // update var
            }
            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%3.2f", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();

         } // end of mouse b4 held
      } // end of mouse b4 pressed
      if (mouse_b1) // only when initially clicked
      {
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
            var = f; // update var

            draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
            sprintf(smsg, "%s%3.2f", txt, var);
            al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
            draw_slider_bar(f, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3);
            al_flip_display();
            proc_controllers();
         }  // end of mouse b1 held
      }  // end of mouse b1 pressed
   }
}























































































































































































































// ------------------------------------------------------------------------------------
// --------------------------buttons---------------------------------------------------
// ------------------------------------------------------------------------------------

// q0 = background color; (not used)
// q1 = frame color
// q2 = text color    (use white 99% of time)
// q5 = text justify  (0-center 1-left...buttons only)

// modified to be display only if q7 == 1
// q6 adjust y

int mdw_button(int x1, int &y1, int x2, int bts,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;

   if ((!q7) && (mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      press = 1;
   }




   if (bn == 4)
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      if (item[num][8] == 1) sprintf(smsg, "Set Desination Item (%d)", item[num][9]);
      if (press)
      {
         if (item[num][8] == 1) // Set Linked Item
         {
             int i = get_item(2, 1, num );
             if (i > -1) item[num][9] = i;
         }
      }
   }

   if (bn == 11)
   {
      sprintf(smsg, "Set Initial Direction");// trakbot direction
      if (press)
      {
         if (++Ei[num][5] > 7) Ei[num][5] = 0;
         set_trakbot_mode(num, Ei[num][5]);
      }
      if ((Ei[num][5] < 0) || (Ei[num][5] > 7)) Ei[num][5] = 0; // enforce limits
   }

   if (bn == 13)
   {
      sprintf(smsg, "Main Shape");
      if (press)
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
   }
   if (bn == 14)
   {
      sprintf(smsg, "Seek Shape");
      if (press)
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
   }




   if (bn == 49) // door type
   {
      if (item[num][8] == 0) sprintf(smsg, "Door Type:Exit Only");
      if (item[num][8] == 1) sprintf(smsg, "Door Type:Normal   ");
      if (press)
      {
         item[num][8] = !item[num][8];
         // check for bad link
         int link = item[num][9];
         if (item[link][0] != 1) // link is not door
         {
            item[num][9] = num;  // link to self
            item[num][11] = 1;   // trigger with up
         }
      }
   }
   if (bn == 50) // door entry type
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      else
      {
         if (press) item[num][11]++;
         if ((item[num][11] < 0) || (item[num][11] > 2)) item[num][11] = 0; // enforce limits
         if (item[num][11] == 0) sprintf(smsg, "Enter Immediate  ");
         if (item[num][11] == 1) sprintf(smsg, "Enter with <up>  ");
         if (item[num][11] == 2) sprintf(smsg, "Enter with <down>");
      }
   }
   if (bn == 51) // door show dest line type
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      else
      {
         if (press) item[num][12]++;
         if ((item[num][12] < 0) || (item[num][12] > 2)) item[num][12] = 0; // enforce limits
         if (item[num][12] == 0) sprintf(smsg, "Exit link:never show  ");
         if (item[num][12] == 1) sprintf(smsg, "Exit link:alway show  ");
         if (item[num][12] == 2) sprintf(smsg, "Exit link:when touched");
      }
   }
   if (bn == 52)
   {
      sprintf(smsg, "Change Door Shape");
      if (press)
      {
         int shape = item[num][13];
         if (shape == 448) shape = 1083;
         else if (shape == 1083) shape = 448;
         if ((shape != 448) && (shape != 1083)) shape = 1083;
         item[num][13] = shape;
         item[num][1] = shape;
         change_linked_door_color_and_shape(num);
      }
   }


   if (bn == 53) // door move type
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      else
      {
         if (press) item[num][7]++;
         if ((item[num][7] < 0) || (item[num][7] > 2)) item[num][7] = 0; // enforce limits
         if (item[num][7] == 0) sprintf(smsg, "Move Type:Automatic    ");
         if (item[num][7] == 1) sprintf(smsg, "Move Type:Force Instant");
         if (item[num][7] == 2) sprintf(smsg, "Move Type:Force Move   ");
      }
   }



   if (bn == 57)
   {
       if (obt == 2) sprintf(smsg,"%s Help", item_name[type]);
       if (obt == 3) sprintf(smsg,"%s Help", (const char *)enemy_name[type]);
       if (obt == 4) sprintf(smsg,"Lift Help");
       if (press) return 1;
   }

   if (bn == 77)
   {
      if (item[num][12] == 0) sprintf(smsg, "   Fuse Timer   ");
      if (item[num][12] == 1) sprintf(smsg, "Remote Detonator");
      if (press)
      {
         item[num][12] = !item[num][12];
         if (item[num][12]) item[num][1] = 537;
         else item[num][1] = 464;
      }
   }






   if (bn == 211) // Trigger Field X Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C) sprintf(smsg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
         int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
         int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XC; // set   C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 212) // Trigger Field Y Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
      int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
      int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;
      if (C) sprintf(smsg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
         int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
         int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YC; // set   C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 310) // block 1 select...
   {
      int tn = item[num][10]&1023; //block 1
      sprintf(smsg, "Block 1: %d", tn);
      if (press) item[num][10] = select_bitmap(item[num][10]);
   }
   if (bn == 311) // block 2 select...
   {
      int tn = item[num][11]&1023; //block 1
      sprintf(smsg, "Block 2: %d", tn);
      if (press) item[num][11] = select_bitmap(item[num][11]);
   }
   if (bn == 320)
   {
      sprintf(smsg, "Set Event Trigger (%d)", item[num][1]);
      if (press)
      {
         int i = get_trigger_item(2, item[num][0], num );
         if (i > -1)
         {
            int ev = get_unused_pm_event();
            item[num][1] = ev;

            if (item[num][0] == 16) // block manip
            {
               if (item[num][3] == 3) // mode 3 - toggle blocks
               {
                  item[i][11] = 0;
                  item[i][12] = 0;
                  item[i][13] = ev;  // needs a toggle trigger
                  item[i][14] = 0;
               }
               else
               {
                  item[i][11] = ev;  // regular trigger
                  item[i][12] = 0;
                  item[i][13] = 0;
                  item[i][14] = 0;
               }
            }
            if (item[num][0] == 17) // block damage
            {
               if (item[num][11] == 1) // mode 1 - toggle damage
               {
                  item[i][11] = 0;
                  item[i][12] = 0;
                  item[i][13] = ev;  // needs a toggle trigger
                  item[i][14] = 0;
               }
               else
               {
                  item[i][11] = ev;  // regular trigger
                  item[i][12] = 0;
                  item[i][13] = 0;
                  item[i][14] = 0;
               }
            }
         }
      }
   }





   if (bn == 401) // timer draw mode
   {
                                                 sprintf(smsg, "Timer Display: OFF          ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SN) sprintf(smsg, "Timer Display: Small Number ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BN) sprintf(smsg, "Timer Display: Large Number ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SP) sprintf(smsg, "Timer Display: Small Percent");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BP) sprintf(smsg, "Timer Display: Large Percent");
      if (press)
      {
         if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SN)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BN; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BN)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SP; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SP)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BP; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BP; // clear all
         }
         else if ( (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_SN))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_BN))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_SP))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)) ) // all clear
         {
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SN; // set
         }
      }
   }





   if (bn == 411) // DAMAGE Field X Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;

      if (C) sprintf(smsg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
         int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
         int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;


         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XC; // set   C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 412) // DAMAGE Field Y Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
      int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
      int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C) sprintf(smsg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
         int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
         int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YC; // set   C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 501)
   {
      if (num == -1) sprintf(smsg, "#");       // show row header
      else           sprintf(smsg, "%d", num); // show step num
   }
   if (bn == 502)
   {
      if (num == -1) sprintf(smsg, "Type");  // show row header
      if (num == 0)  sprintf(smsg, "----");
      if (num == 1)  sprintf(smsg, "Move");
      if (num == 2)  sprintf(smsg, "Wait");
      if (num == 3)  sprintf(smsg, "Wait");
      if (num == 4)  sprintf(smsg, "End ");
      if (num == 5)  sprintf(smsg, "Wait");
   }
   if (bn == 503)
   {
      int l = type;
      int s = obt;
      int v = lift_steps[l][s].val;

      if (num == -1) sprintf(smsg, "Details");  // show row header
      if (num == 0)  sprintf(smsg, "blank");
      if (num == 1)  sprintf(smsg, "and Resize [speed:%d]", v);
      if (num == 2)  sprintf(smsg, "for Timer:%d", v);
      if (num == 3)  sprintf(smsg, "for Player prox:%d", v);
      if (num == 5)  sprintf(smsg, "for Trigger Event:%d", v);
      if (num == 4)
      {
         sprintf(smsg, "Step - undefined val");
         if (v == 0) sprintf(smsg, "Step - Loop to Start");
         if (v == 1) sprintf(smsg, "Step - Warp to Start");
         if (v == 2) sprintf(smsg, "Step - Freeze Here");
      }
   }
   if (bn == 504)
   {
      sprintf(smsg, "Name:%s", lifts[num].lift_name); // edit lift name
      if (press) return 1;
   }
   if (bn == 505) // lift step end step mode
   {
      sprintf(smsg, "Undefined value");
      if (lift_steps[num][type].val == 0) sprintf(smsg, "Loop to Start");
      if (lift_steps[num][type].val == 1) sprintf(smsg, "Warp to Start");
      if (lift_steps[num][type].val == 2) sprintf(smsg, "Freeze Here  ");
      if (press) if (++lift_steps[num][type].val > 2) lift_steps[num][type].val = 0; // lift step end step mode
   }
   if (bn == 506)
   {
      if (num == -1) sprintf(smsg, "C");       // show row header
      else           sprintf(smsg, "%d", num); // show num
   }

   if (bn == 520)
   {
      sprintf(smsg, "Set Event Trigger (%d)", lift_steps[num][type].val);
      if (press)
      {
         //printf("520 type:%d num:%d \n", type, num);
         int i = get_trigger_item(4, type, num );
         if (i > -1)
         {
            int ev = get_unused_pm_event();
            lift_steps[num][type].val = ev;
            // toggle trigger
            item[i][11] = 0;
            item[i][12] = 0;
            item[i][13] = ev;
            item[i][14] = 0;
         }
      }
   }

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame

   if (q5) al_draw_text(font, palette_color[q2], x1+4, (y2+y1)/2-3, 0, smsg);
   else al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   // special cases that need bitmaps draw on them
   if (bn == 13)
   {
      float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
      al_draw_rotated_bitmap(tile[zz[0][Ei[num][5]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }
   if (bn == 14)
   {
      float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
      al_draw_rotated_bitmap(tile[zz[0][Ei[num][6]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }

   if (bn == 310)
   {
      int tn = item[num][10];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, palette_color[0]);
      al_draw_bitmap(btile[tn&1023], x, y, 0);
   }

   if (bn == 311)
   {
      int tn = item[num][11];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, palette_color[0]);
      al_draw_bitmap(btile[tn&1023], x, y, 0);
   }
   if (q6) y1+=bts;
   return retval;
}









































































// modified to be display only if q7 == 1
// and q6 for apply bts

void mdw_colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   int y2 = y1+bts-2;

   // erase
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw colors (1-15)
   float a = x2-x1; // range
   float b = a/15;  // color swatch width
   if (b<2) b = 2;  // min width
   for (int c=0; c<15; c++)
      al_draw_filled_rectangle((int)(x1+c*b), y1, (int)(b+x1+c*b), y2, palette_color[(int)c+1]);

   if (bn == 2) sprintf(smsg, "Select Text Color");
   if (bn == 3) sprintf(smsg, "Select Frame Color");
   if (bn == 4) sprintf(smsg, "Select Lift Color");
   if (bn == 5) sprintf(smsg, "Select Door Color");
   if (bn == 6) sprintf(smsg, "Select Trigger Field Color");
   if (bn == 7) sprintf(smsg, "Select Block Manip Field Color");
   if (bn == 8) sprintf(smsg, "Select Lift Step Color");
   al_draw_text(font, palette_color[0], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, smsg);

    // draw outline
   al_draw_rectangle(x1, y1, x2, y2, palette_color[15], 1);

   // is mouse pressed on button?
   if ((!q7) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2) && (mouse_b1))
   {
      while (mouse_b1) proc_controllers();
      int color = (int)(1+(mouse_x-x1)/b);
      if (bn == 2) item[num][8] = color;     // pop msg text color
      if (bn == 3) item[num][9] = color;     // pop msg frame color
      if (bn == 4) lifts[num].color = color; // lift color
      if (bn == 5)
      {
         item[num][6] = color;     // door color
         change_linked_door_color_and_shape(num);
      }
      if (bn == 6) item[num][2] = color;     // trigger color
      if (bn == 7) item[num][12] = color;    // block manip color


      if (bn == 8) // lift step color
      {
        // printf("n:%d t:%d c:%d\n",num, type, color);
         int cf = color << 28; // shift 4 bits of color into place
         lift_steps[num][type].type &= 0b00001111111111111111111111111111; // clear old color
         lift_steps[num][type].type |= cf; // merge color with type
      }
   }
   if (q6 == 1) y1+=bts;
}


/*

q0 = background color; (not used)
q1 = frame color
q2 = text color    (use white 99% of time)
q3 = slider color  (use white 99% of time)
q4 = slider color  (draw frame mode) now i always use 1
q5 = text justify  (0-center 1-left...buttons only)
q6 - (0-normal) (1-dont draw)
q7 - (0-normal) (1-dont process mouse b1 press)
*/


int mdw_toggle(int x1, int &y1, int x2, int bts,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   int y2 = y1+bts-2;
   int ret = 0;
   // is mouse pressed on this button?
   if ((mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2) && (!q7))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      var = ! var;
   }
   if (var)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(smsg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(smsg, "%s", t0);
      ret = 0;
   }
   if (!q6)
   {
      draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
      if (q5) al_draw_text(font, palette_color[q2], x1+4, (y2+y1)/2-3, 0, smsg);
      else al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);
   }
   if (bn == 1) y1+=bts;
   return ret;
}


// toggle the flag and displays the corresponding string
// returns the value of the flag
// y1 is passed as a reference and is modified by height (bts) if bn == 1
int mdw_togglf(int x1, int &y1, int x2, int bts,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   int ret = 0;
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   if ((!q7) && (mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      var ^= flag;
   }
   if (var & flag)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(smsg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(smsg, "%s", t0);
      ret = 0;
   }
   if (!q6)
   {
      draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
      if (q5) al_draw_text(font, palette_color[q2], x1+4,      (y1+y2)/2-3, 0,                    smsg);
      else    al_draw_text(font, palette_color[q2], (x2+x1)/2, (y1+y2)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   }
   if (bn == 1) y1+=bts;
   return ret;
}




/*
q0 = background color; (not used)
q1 = frame color
q2 = text color    (use white 99% of time)
q3 = slider color  (use white 99% of time)
q4 = slider color  (draw frame mode) now i always use 1
q5 = text justify  (0-center 1-left...buttons only)
q6 - (0-normal) (1-dont draw)
q7 - (0-normal) (1-dont process mouse b1 press)
*/

// displays a text string, and returns 1 if pressed
// y1 is passed as a reference and is modified by height (bts) if bn == 1
int mdw_buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;
   if (!q6)
   {
      sprintf(smsg, "%s", txt);
      draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
      if (q5) al_draw_text(font, palette_color[q2], x1+4,      y1+bts/2-3, 0,                    smsg);
      else    al_draw_text(font, palette_color[q2], (x2+x1)/2, y1+bts/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   }
   if ((!q7) && (mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      ret = 1;
   }
   if (bn) y1+=bts;
   return ret;
}



// modified to be display only if q7 == 1
// increment passed pointer (int &var) and display different text for each value

// q6 used for add bts or not

void mdw_buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var)
{
   int y2 = y1+bts-2;


   int press = 0;
   if ((!q7) && (mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      press = 1;
   }





   if (bn == 21)
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(smsg,  "Stationary");
      if (var == 1) sprintf(smsg,  "Fall");
   }

   if (bn == 22)
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var ==  1) sprintf(smsg, "Fall");
      if (var ==  0) sprintf(smsg, "Stationary");
      if (var == -1) sprintf(smsg, "Carry");
      if (var == -2) sprintf(smsg, "Carry Through Door");
   }
   if (bn == 23) // rocket only
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var == -1) var = 0;
      if (var ==  1) sprintf(smsg,  "Fall");
      if (var ==  0) sprintf(smsg,  "Stationary");
      if (var == -2) sprintf(smsg,  "Ride Through Door");
   }


   if (bn == 27) // cloner trigger type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Trigger Type:Timer Runs  ");
      if (var == 1) sprintf(smsg, "Trigger Type:Timer Resets");
      if (var == 2) sprintf(smsg, "Trigger Type:Immediate   ");
   }


   if (bn == 78)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Start Mode:Default");
      if (var == 1) sprintf(smsg, "Team Start");
      if (var == 2) sprintf(smsg, "Checkpoint Common");
      if (var == 3) sprintf(smsg, "Checkpoint Individual");
   }

   if (bn == 79)
   {
      if (press) var++;
      if ((var < 0) || (var > 7)) var = 0;
      sprintf(smsg, "Start Index:%d", var);
   }


   if (bn == 81)
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(smsg, "Draw Boxes:off");
      if (var == 1) sprintf(smsg, "Draw Boxes:trigger only");
      if (var == 2) sprintf(smsg, "Draw Boxes:src/dst only");
      if (var == 3) sprintf(smsg, "Draw Boxes:all");
   }
   if (bn == 100)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Action:Randomize");
      if (var == 1) sprintf(smsg, "Action:Step from Min to Max");
      if (var == 2) sprintf(smsg, "Action:Set all to Min");
   }
   if (bn == 101)
   {
      if (press) var++;
      if ((var < 1) || (var > 3)) var = 1;
      if (var == 1) sprintf(smsg, "Type: Health Bonus");
      if (var == 2) sprintf(smsg, "Type: Free Man");
      if (var == 3) sprintf(smsg, "Type: Purple Coin");
   }
   if (bn == 102)
   {
      if (press) var++;
      if ((var < 1039) || (var > 1042)) var = 1039;
      if (var == 1039) { sprintf(smsg, "Color:Red");    q1 = 10; }
      if (var == 1040) { sprintf(smsg, "Color:Green");  q1 = 11; }
      if (var == 1041) { sprintf(smsg, "Color:Blue");   q1 = 13; }
      if (var == 1042) { sprintf(smsg, "Color:Purple"); q1 = 8;  }
   }

   if (bn == 301) // block manip mode
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(smsg, "MODE:OFF");
      if (var == 1) sprintf(smsg, "MODE:Set All To Block 1");
      if (var == 2) sprintf(smsg, "MODE:Set All Block 2 To Block 1");
      if (var == 3) sprintf(smsg, "MODE:Toggle Block 2 To Block 1");
   }

   if (bn == 402) // damage mode
   {
      if (press) var++;
      if ((var < 0) || (var > 4)) var = 0;
      if (var == 0) sprintf(smsg, "MODE:Always ON");
      if (var == 1) sprintf(smsg, "MODE:Toggle");
      if (var == 2) sprintf(smsg, "MODE:ON Until Triggered");
      if (var == 3) sprintf(smsg, "MODE:OFF Until Triggered");
      if (var == 4) sprintf(smsg, "MODE:Timed ON And OFF");
   }

   if (bn == 404) // Block Damage draw mode
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Draw Type:none         ");
      if (var == 1) sprintf(smsg, "Draw Type:Red Rectangle");
      if (var == 2) sprintf(smsg, "Draw Type:Spikey Floor ");
   }


   if (bn == 500) // lift mode
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(smsg, "Mode 0 - Normal");
      if (var == 1) sprintf(smsg, "Mode 1 - Prox Run and Reset");
   }


















   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame

   if (q5) al_draw_text(font, palette_color[q2], x1+4, (y2+y1)/2-3, 0, smsg);
   else al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);



   if (q6) y1+=bts;






}

