// e_sliders.cpp

#include "pm.h"

char smsg[80];
int bw = 3; // slider adjustment bar width


// ------------------------------------------------------------------------------------
// ---------------------------sliders--------------------------------------------------
// ------------------------------------------------------------------------------------

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


   if (bn == 43) sprintf(smsg, "Lift Step Width:%d",lifts[num].width);
   if (bn == 44) sprintf(smsg, "Lift Step Height:%d",lifts[num].height);


   if (bn == 45) sprintf(smsg, "X-speed:%-1.2f",al_fixtof(Efi[num][2]));
   if (bn == 46) sprintf(smsg, "Y-Speed:%-1.2f",al_fixtof(Efi[num][3]));
   if (bn == 47) sprintf(smsg, "Exit with %d enemies left",item[num][8]);
   if (bn == 54) sprintf(smsg, "Message display time:%d",item[num][7]);


   if (bn == 60) sprintf(smsg, "X Speed:%-3.2f",al_fixtof(Efi[num][5]));
   if (bn == 61) sprintf(smsg, "X Accel:%-3.2f",al_fixtof(Efi[num][6]));
   if (bn == 62) sprintf(smsg, "Y Flap :%-3.2f",al_fixtof(Efi[num][8]));

   if (bn == 63) sprintf(smsg, "Height Above Player:%d",Ei[num][20]);


   if (bn == 74) sprintf(smsg, "Created Objects Time To Live:%-2d", Ei[num][9]);
   if (bn == 75) sprintf(smsg, "Max Created Objects At One Time:%-2d", Ei[num][10]);


   if (bn == 78) sprintf(smsg, "Start Index:%d",item[num][7]);



   if (bn == 79) sprintf(smsg, "Flap Speed:%-1.2f", al_fixtof(Efi[num][10]));
   if (bn == 80) sprintf(smsg, "Flap Height:%d", Ei[num][21]);

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

   if (bn == 103) sprintf(smsg, "Damage Field ON Time:%d",   item[num][12]);
   if (bn == 104) sprintf(smsg, "Damage Field OFF Time:%d",  item[num][12]);


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



   if (bn == 74) Ei[num][9] = (int)f;        // cloner created object time to live
   if (bn == 75) Ei[num][10] = (int)f;       // cloner max created objects active at one time

   if (bn == 78) item[num][7] = (int)f;       // start index



   if (bn == 79) Efi[num][10] = al_ftofix(f);       // flap speed
   if (bn == 80) Ei[num][21] = (int)f;              // flap height

   if (bn == 82) Ei[num][6] = (int)f;                 // field timer
   if (bn == 83) Efi[num][4] = al_ftofix(f);          // field damage

   if (bn == 84)
   {
      Ei[num][21] = (int)f;                           // lift damage field is slaved to
      set_field_location_from_lift(num, 0, 1);
   }
   if (bn == 85)
   {
      Ei[num][20] = (int)f;                           // lift trigger field  is slaved to
      set_field_location_from_lift(num, 1, 1);
   }

   if (bn == 86) Ei[num][6] = (int)f;                 // field timer swap val
   if (bn == 87) Ei[num][7] = (int)f;                 // field timer swap val
   if (bn == 88) Ei[num][8] = (int)f;                 // field timer swap val

   if (bn == 89) Ei[num][6] = (int)f;                 // damage field on time
   if (bn == 90) Ei[num][6] = (int)f;                 // damage field off time


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

void mdw_slider(int x1, int y1, int x2, int y2,
                    int bn, int num, int type, int obt,
                    int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   float sdx, sul, sll, sinc, dsx;
   switch (bn)
   {
      case 1:  sul=100;  sll=2;     sinc=1;   sdx=item[num][7];                break;  // health bonus
      case 2:  sul=200;  sll=2;     sinc=1;   sdx=item[num][8];                break;  // bullet bonus
      case 3:  sul=400;  sll=5;     sinc=1;   sdx=item[num][9];                break;  // timer bonus
      case 4:  sul=1200; sll=20;    sinc=1;   sdx=item[num][7];                break;  // blast size
      case 5:  sul=2000; sll=1;     sinc=1;   sdx=item[num][9];                break;  // fuse length
      case 6:  sul=200;  sll=1;     sinc=1;   sdx=item[num][9];                break;  // accel
      case 7:  sul=20;   sll=1;     sinc=1;   sdx=item[num][8];                break;  // max speed
      case 8:  sul=50;   sll=1;     sinc=1;   sdx=item[num][6];                break;  // steerability
      case 9:  sul=7;    sll=1;     sinc=1;   sdx=item[num][6];                break;  // jump length
      case 10: sul=200;  sll=40;    sinc=1;   sdx=item[num][7];                break;  // sproinginess
      case 11: sul=20;   sll=1;     sinc=1;   sdx=item[num][8];                break;  // mine damage
      case 12: sul=9;    sll=.7;    sinc=.01; sdx=al_fixtof(Efi[num][6]);      break;  // archwagon x speed
      case 13: sul=9;    sll=.7;    sinc=.01; sdx=al_fixtof(Efi[num][3]);      break;  // y speed
      case 15: sul=500;  sll=0;     sinc=1;   sdx=Ei[num][6];                  break;  // jump wait count
      case 16: sul=600;  sll=0;     sinc=1;   sdx=Ei[num][7];                  break;  // jump under width
      case 17: sul=100;  sll=0;     sinc=1;   sdx=Ei[num][12];                 break;  // jump before wall
      case 18: sul=40;   sll=0;     sinc=1;   sdx=Ei[num][11];                 break;  // Jump before hole
      case 19: sul=20;   sll=.8;    sinc=.1;  sdx=al_fixtof(Efi[num][7]);      break;  // bullet speed
      case 20: sul=2000; sll=20;    sinc=1;   sdx=Ei[num][17];                 break;  // bullet prox
      case 21: sul=200;  sll=1;     sinc=1;   sdx=Ei[num][15];                 break;  // retrigger time
      case 22: sul=12;   sll=0;     sinc=.01; sdx=al_fixtof(Efi[num][5]);      break;  // cannon speed
      case 23: sul=100;  sll=0;     sinc=1;   sdx=Ei[num][8];                  break;  // seek count
      case 24: sul=20;   sll=0;     sinc=1;   sdx=Ei[num][29];                 break;  // collision box
      case 25: sul=10;   sll=0;     sinc=.1;  sdx=al_fixtof(Efi[num][4]);      break;  // health dec
      case 26: sul=40;   sll=4;     sinc=1;   sdx=bts;                         break;  // button height
      case 27: sul=800;  sll=10;    sinc=1;   sdx=item[num][8];                break;  // initial time
      case 28: sul=100;  sll=1;     sinc=1;   sdx=item[num][8];                break;  // warp level
      case 29: sul=30;   sll=.5;    sinc=.5;  sdx=al_fixtof(Efi[num][9]);      break;  // pod speed
      case 30: sul=40;   sll=0;     sinc=1;   sdx=Ei[num][9];                  break;  // pod wait time
      case 33: sul=5;    sll=.7;    sinc=.1;  sdx=al_fixtof(Efi[num][2]);      break;  // flapper x speed
      case 34: sul=1000; sll=20;    sinc=1;   sdx=Ei[num][6];                  break;  // create delay
      case 35: sul=40;   sll=0;     sinc=1;   sdx=Ei[num][9];                  break;  // cannon hits
      case 36: sul=5;    sll=0;     sinc=.01; sdx=al_fixtof(Efi[num][3]);      break;  // flapper y speed
      case 38: sul=500;  sll=20;    sinc=1;   sdx=Ei[num][17];                 break;  // width
      case 39: sul=600;  sll=1;     sinc=10;  sdx=Ei[num][18];                 break;  // y1
      case 40: sul=600;  sll=1;     sinc=10;  sdx=Ei[num][19];                 break;  // y2
      case 41: sul=50;   sll=0;     sinc=1;   sdx=Ei[num][24];                 break;  // dead enemy bullet bonus
      case 42: sul=50;   sll=0;     sinc=1;   sdx=Ei[num][25];                 break;  // dead enemy health bonus
      case 43: sul=1999; sll=4;     sinc=1;   sdx=lifts[num].width;            break;  // lift width
      case 44: sul=1999; sll=4;     sinc=1;   sdx=lifts[num].height;           break;  // lift heigth
      case 45: sul=10;   sll=.5;    sinc=.01; sdx=al_fixtof(Efi[num][2]);      break;  // trakbot x speed
      case 46: sul=10;   sll=.5;    sinc=.01; sdx=al_fixtof(Efi[num][3]);      break;  // trakbot y speed
      case 47: sul=100;  sll=0;     sinc=1;   sdx=item[num][8];                break;  // exit with x enemies left
      case 54: sul=200;  sll=1;     sinc=1;   sdx=item[num][7];                break;  // Message delay time

      case 60: sul=8;    sll=.5;    sinc=.1;  sdx=al_fixtof(Efi[num][5]);      break;  // flapper max x speed
      case 61: sul=2;    sll=.01;   sinc=.01; sdx=al_fixtof(Efi[num][6]);      break;  // flapper x accel
      case 62: sul=4;    sll=0;     sinc=.1;  sdx=al_fixtof(Efi[num][8]);      break;  // flapper yinc flap scale

      case 63: sul=1000; sll=-1000; sinc=10;  sdx=Ei[num][20];                 break;  // height above player


      case 74: sul=4800; sll=0;     sinc=1;   sdx=Ei[num][9];                  break;  // cloner created obj time to live
      case 75: sul=600;  sll=0;     sinc=1;   sdx=Ei[num][10];                 break;  // cloner max created obj at one time

      case 78: sul=20;   sll=0;     sinc=1;   sdx=item[num][7];                break;  // Start index


      case 79: sul=8;    sll=.5;    sinc=.1;  sdx=al_fixtof(Efi[num][10]);     break;  // flap speed
      case 80: sul=400;  sll=0;     sinc=10;  sdx=Ei[num][21];                 break;  // flap height

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

      case 98: sul=2000; sll=0;     sinc=1;   sdx=item[num][15];               break;  // item damage player amount

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

void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
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



































































// ------------------------------------------------------------------------------------
// --------------------------buttons---------------------------------------------------
// ------------------------------------------------------------------------------------

// q0 = background color; (not used)
// q1 = frame color
// q2 = text color    (use white 99% of time)
// q5 = text justify  (0-center 1-left...buttons only)


int mdw_button(int x1, int y1, int x2, int y2,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{

   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;




   if ((mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      press = 1;
   }


   if (bn == 1)
   {
      sprintf(smsg, "OK");
      if (press) return 1;
   }
   if (bn == 2)
   {
      if (item[num][3] ==  0) sprintf(smsg,  "    Stationary   ");
      if (item[num][3] ==  1) sprintf(smsg,  "       Fall      ");
      if (press) if (++item[num][3] > 1) item[num][3] = 0;
   }
   if (bn == 3)
   {
      sprintf(smsg, "Cancel");
      if (press) return 1;
   }
   if (bn == 4)
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      if (item[num][8] == 1) sprintf(smsg, "Set Desination Item (%d)", item[num][9]);
      if (press)
      {
         if (item[num][8] == 1) // Set Linked Item
         {
             int i = get_item("Select Another Door To Link To", 2, 1, num );
             if (i > -1) item[num][9] = i;
             Redraw = 1;
         }
      }
   }
   if (bn == 5)
   {
      sprintf(smsg, "Get New Block Range");
      if (press)
      {
         if (getbox( "Block Range", 2, 4, num) == 1)
         {
            if (--bx2 < bx1) bx2++;
            if (--by2 < by1) by2++;
            item[num][6] = bx1;
            item[num][7] = by1;
            item[num][8] = bx2;
            item[num][9] = by2;
            Redraw = 1;
         }
      }
   }
   if (bn == 6)
   {
      sprintf(smsg, "Set Initial Direction"); // rocket direction
      if (press)
      {
         if (getxy("Set New Direction", 97, 11, num) == 1)
         {
            set_rocket_rot(num, get100_x*20, get100_y*20);
            Redraw = 1;
         }
      }
   }
   if (bn == 7)
   {
      sprintf(smsg, "Edit Message");
      if (press)
      {
         if (num != 999) edit_pmsg_text(num, 0);
      }
   }
   if (bn == 8) // archwag direction
   {
      if (Ei[num][2]) sprintf(smsg, "Initial Direction:Right");
      else            sprintf(smsg, "Initial Direction:Left ");
      if (press)
      {
         Ei[num][2] = !Ei[num][2];
         if (Ei[num][2]) Efi[num][2] = Efi[num][6];
         else Efi[num][2] = -Efi[num][6];
         Redraw = 1;
      }
   }
   if (bn == 9) // archwag and walker
   {
      if (Ei[num][8]) sprintf(smsg, "Bounce Mode");
      else            sprintf(smsg, "Follow Mode");
      if (press) Ei[num][8] = !Ei[num][8];
   }
   if (bn == 10)
   {
      sprintf(smsg, "Set Initial Direction"); // bouncer direction
      if (press)
      {
         if (getxy("Set New Direction", 96, 4, num) == 1)
         {
            set_xyinc_rot(num, get100_x*20, get100_y*20);
            Redraw = 1;
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
         Redraw = 1;
      }
   }
   if (bn == 12) // trakbot drop mode
   {
      if (Ei[num][7] == 0) sprintf(smsg, "Drop Mode:Off");
      else                 sprintf(smsg, "Drop Mode:On ");
      if (press)
      {
         if (bn == 12) Ei[num][7] = !Ei[num][7]; // trakbot drop mode
      }
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
   if (bn == 15)
   {
      sprintf(smsg, "Move Extended Position");
      if (press) move_pod_extended(num);
   }
   if (bn == 16)
   {
      sprintf(smsg, "Set Trigger Box");
      if (press) move_trigger_box(num, 9);
   }
   if (bn == 17)
   {
      sprintf(smsg, "Set Source Area");
      if (press)
      {
         if (getbox("Cloner Source Area", 3, 9, num))
         {
            Ei[num][15] = bx1;
            Ei[num][16] = by1;
            Ei[num][19] = bx2-bx1;
            Ei[num][20] = by2-by1;
            Redraw = 1;
         }
      }
   }
   if (bn == 18)
   {
      sprintf(smsg, "Set Destination");
      if (press)
      {
         if (getxy("Set Cloner Destination", 98, 9, num) == 1)
         {
            Ei[num][17] = get100_x;
            Ei[num][18] = get100_y;
            Redraw = 1;
         }
      }
   }
   if (bn == 19)
   {
      sprintf(smsg, "Move");
      if (press) return 1;
   }

   if (bn == 20)
   {
      sprintf(smsg, "Create");
      if (press) return 1;
   }
   if (bn == 21)
   {
      sprintf(smsg, "Delete");
      if (press) return 1;
   }
   if (bn == 22)
   {
      sprintf(smsg, "Next");
      if (press) return 1;
   }
   if (bn == 23)
   {
      sprintf(smsg, "Prev");
      if (press) return 1;
   }
   if (bn == 24)
   {
      sprintf(smsg, "Copy to Draw Item");
      if (press) return 1;
   }
   if (bn == 25)
   {
      sprintf(smsg, "Viewer Help");
      if (press) return 1;
   }
   if (bn == 26)
   {
      if (item[num][3] ==  1) sprintf(smsg, "      Fall        ");
      if (item[num][3] ==  0) sprintf(smsg, "    Stationary    ");
      if (item[num][3] == -1) sprintf(smsg, "      Carry       ");
      if (item[num][3] == -2) sprintf(smsg, "Carry Through Door");
      if (press)
      {
         if (++item[num][3] > 1) item[num][3] = -2;
      }
   }
   if (bn == 27) // cloner trigger type
   {
      if (Ei[num][8] == 0) sprintf(smsg, "Trigger Type:Timer Runs  ");
      if (Ei[num][8] == 1) sprintf(smsg, "Trigger Type:Timer Resets");
      if (Ei[num][8] == 2) sprintf(smsg, "Trigger Type:Immediate   ");
      if (press)
      {
         if (++Ei[num][8] > 2) Ei[num][8] = 0;
      }
   }
   if (bn == 28)
   {
      sprintf(smsg, "Run Lifts");
      if (press) return 1;
   }
   if (bn == 48) // key block erase type
   {
      if (item[num][12]) sprintf(smsg, "Erase Only Matching Blocks");
      else               sprintf(smsg, "Erase All Blocks In Range ");
      if (press) item[num][12] = !item[num][12]; // key block remove type
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
         Redraw = 1;
      }
   }
   if (bn == 50) // door entry type
   {
      if (item[num][11] == 0) sprintf(smsg, "Enter Immediate  ");
      if (item[num][11] == 1) sprintf(smsg, "Enter with <up>  ");
      if (item[num][11] == 2) sprintf(smsg, "Enter with <down>");
      if (item[num][8]  == 0) sprintf(smsg, "disabled");
      if (press)
      {
         item[num][11]++;
         if (item[num][11] > 2) item[num][11] = 0;
         Redraw = 1;
      }
   }
   if (bn == 51) // door show dest line type
   {
      if (item[num][12] == 0) sprintf(smsg, "Exit link:never show  ");
      if (item[num][12] == 1) sprintf(smsg, "Exit link:alway show  ");
      if (item[num][12] == 2) sprintf(smsg, "Exit link:when touched");
      if (item[num][8]  == 0) sprintf(smsg, "disabled");
      if (press)
      {
         item[num][12]++;
         if (item[num][12] > 2) item[num][12] = 0;
         Redraw = 1;
      }
   }
   if (bn == 52)
   {
      sprintf(smsg, "Change Door Shape");
      if (press)
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
   }
   if (bn == 53) // door move type
   {
      if (item[num][7] == 0) sprintf(smsg, "Move Type:Automatic    ");
      if (item[num][7] == 1) sprintf(smsg, "Move Type:Force Instant");
      if (item[num][7] == 2) sprintf(smsg, "Move Type:Force Move   ");
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      if (press)
      {
         item[num][7]++;
         if (item[num][7] > 2) item[num][7] = 0;
      }
   }
   if (bn == 55)
   {
      sprintf(smsg, "Set Message Position");
      if (press)
      {
         if (getxy("Set Message Position", 2, 1010, num) == 1)
         {
            item[num][10] = get100_x;
            item[num][11] = get100_y;
            draw_big(1);
         }
         //set_mouse_sprite(NULL);
         Redraw = 1;
      }
   }
   if (bn == 56)
   {
      if (Viewer_lock) sprintf(smsg,  "Locked");
      else             sprintf(smsg, "Unlocked");
      if (press) Viewer_lock = !Viewer_lock;
   }
   if (bn == 57)
   {
       if (obt == 2) sprintf(smsg,"%s Help", item_name[type]);
       if (obt == 3) sprintf(smsg,"%s Help", (const char *)enemy_name[type]);
       if (obt == 4) sprintf(smsg,"Lift Help");
       if (press) return 1;
   }
   if (bn == 76)
   {
      if (item[num][11] == 0) sprintf(smsg, "Sticky:Off");
      if (item[num][11] == 1) sprintf(smsg, "Sticky:On ");
      if (press) item[num][11] = !item[num][11];
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
         Redraw = 1;
      }
   }
   if (bn == 78)
   {
      if (item[num][6] == 0) sprintf(smsg, "Start Mode:Default");
      if (item[num][6] == 1) sprintf(smsg, "Start Mode:Team Start");
      if (item[num][6] == 2) sprintf(smsg, "Start Mode:Checkpoint Common");
      if (item[num][6] == 3) sprintf(smsg, "Start Mode:Checkpoint Individual");
      if (press)
      {
         if (++item[num][6] > 3) item[num][6] = 0;
      }
   }
   if (bn == 79)
   {
      sprintf(smsg, "Start Index:%d", item[num][7]);
      if (press)
      {
         if (++item[num][7] > 7) item[num][7] = 0;
      }
   }

   if (bn == 81)
   {
      if (Ei[num][4] == 0) sprintf(smsg, "Draw Boxes:off");
      if (Ei[num][4] == 1) sprintf(smsg, "Draw Boxes:trigger only");
      if (Ei[num][4] == 2) sprintf(smsg, "Draw Boxes:src/dst only");
      if (Ei[num][4] == 3) sprintf(smsg, "Draw Boxes:all");
      if (press)
      {
         if (++Ei[num][4] > 3) Ei[num][4] = 0;
      }
   }
   if (bn == 82) // rocket only
   {
      if (item[num][3] ==  1) sprintf(smsg,  "       Fall      ");
      if (item[num][3] ==  0) sprintf(smsg,  "    Stationary   ");
      if (item[num][3] == -2) sprintf(smsg,  "Ride Through Door");
      if (press)
      {
         item[num][3]++;
         if (item[num][3] > 1) item[num][3] = -2;
         if (item[num][3] == -1) item[num][3] = 0;
      }
   }
   if (bn == 83) // cloner only
   {
      if (Ei[num][30] ==  0) sprintf(smsg,  "      Normal     ");
      if (Ei[num][30] ==  1) sprintf(smsg,  "    Invincible   ");
      if (press)
      {
         Ei[num][30] = !Ei[num][30]; // cloner invinciblility
      }
   }



   if (bn == 85)
   {
      printf(smsg, "Get New Damage Field");
      if (press)
      {
         if (getbox("Get New Field ", 3, 10, num))
         {
            Ei[num][15] = bx1*20;
            Ei[num][16] = by1*20;
            Ei[num][17] = (bx2-bx1)*20;
            Ei[num][18] = (by2-by1)*20;
            Redraw = 1;
         }
      }
   }
   if (bn == 86)
   {
      sprintf(smsg, "Get New Trigger Field");
      if (press)
      {
         if (getbox("Get New Field Trigger Box", 3, 10, num))
         {
            Ei[num][11] = bx1*20;
            Ei[num][12] = by1*20;
            Ei[num][13] = (bx2-bx1)*20;
            Ei[num][14] = (by2-by1)*20;
            Redraw = 1;
         }
      }
   }
   if (bn == 87) // field mode
   {
      if (Ei[num][5] == 0) sprintf(smsg, "MODE:Damage Field Always ON          ");
      if (Ei[num][5] == 1) sprintf(smsg, "MODE:Damage Field Toggle             ");
      if (Ei[num][5] == 2) sprintf(smsg, "MODE:Damage Field ON Until Triggered ");
      if (Ei[num][5] == 3) sprintf(smsg, "MODE:Damage Field OFF Until Triggered");
      if (Ei[num][5] == 4) sprintf(smsg, "MODE:Damage Field Timed ON And OFF   ");
      if (press)
      {
         Ei[num][5]++;
         if (Ei[num][5] > 4) Ei[num][5] = 0;

         if (Ei[num][5] == 0) // mode 0 - always on
         {
            Ei[num][3] |= PM_ENEMY_FIELD_DAMAGE_CURR; // set flag
         }
         if (Ei[num][5] == 1) // mode 1 - toggle
         {
         }
         if (Ei[num][5] == 2) // mode 2 - no damage until triggered
         {
            Ei[num][3] &= ~PM_ENEMY_FIELD_DAMAGE_CURR; // clear flag
         }
         if (Ei[num][5] == 3) // mode 3 - no damage when triggered
         {
            Ei[num][3] |= PM_ENEMY_FIELD_DAMAGE_CURR; // set flag
         }
         if (Ei[num][5] == 4) // mode 4 - Timed on and off
         {

         }
      }
   }
   if (bn == 98)
   {
      if (Ei[num][2] == 0) sprintf(smsg, "Draw Mode:Hidden           ");
      if (Ei[num][2] == 1) sprintf(smsg, "Draw Mode:Small Number     ");
      if (Ei[num][2] == 2) sprintf(smsg, "Draw Mode:Large Number     ");
      if (Ei[num][2] == 3) sprintf(smsg, "Draw Mode:Small Percent Bar");
      if (Ei[num][2] == 4) sprintf(smsg, "Draw Mode:Large Percent Bar");
      if (press) if (++Ei[num][2] > 4) Ei[num][2] = 0;
   }
   if (bn == 102) // Damage Field
   {
      if (Ei[num][19] == 0) sprintf(smsg, "Draw Type:Red Rectangle (default)");
      if (Ei[num][19] == 1) sprintf(smsg, "Draw Type:Spikey Floor");
      if (Ei[num][19] == 2) sprintf(smsg, "Draw Type:none");
      if (press) if (++Ei[num][19] > 2) Ei[num][19] = 0;
   }

   if (bn == 105) // Trigger Field
   {
      if (Ei[num][10] == 0) sprintf(smsg, "Draw Type:Yellow Rectangle (default)");
      if (Ei[num][10] == 1) sprintf(smsg, "Draw Type:none");
      if (press) if (++Ei[num][10] > 1) Ei[num][10] = 0;
   }









   if (bn == 120)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_PLAYER) sprintf(smsg, "Affects Players:ON          ");
      else                                           sprintf(smsg, "Affects Players:OFF         ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_PLAYER;
   }
   if (bn == 121)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_ENEMY)  sprintf(smsg, "Affects Enemies:ON          ");
      else                                           sprintf(smsg, "Affects Enemies:OFF         ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_ENEMY;
   }
   if (bn == 122)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_ITEM)   sprintf(smsg, "Affects Items:ON            ");
      else                                           sprintf(smsg, "Affects Items:OFF           ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_ITEM;
   }
   if (bn == 123)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_PBUL)   sprintf(smsg, "Affects Player's Bullets:ON ");
      else                                           sprintf(smsg, "Affects Player's Bullets:OFF");
   }
   if (bn == 124)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_EBUL)   sprintf(smsg, "Affects Enemy's Bullets:ON  ");
      else                                           sprintf(smsg, "Affects Enemy's Bullets:OFF ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_EBUL;
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_PBUL;
   }
   if (bn == 125)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_TRIGGER_PLAYER) sprintf(smsg, "Triggered by Players:ON          ");
      else                                            sprintf(smsg, "Triggered by Players:OFF         ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_TRIGGER_PLAYER;
   }
   if (bn == 126)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_TRIGGER_ENEMY)  sprintf(smsg, "Triggered by Enemies:ON          ");
      else                                            sprintf(smsg, "Triggered by Enemies:OFF         ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_TRIGGER_ENEMY;
   }
   if (bn == 127)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_TRIGGER_ITEM)   sprintf(smsg, "Triggered by Items:ON            ");
      else                                            sprintf(smsg, "Triggered by Items:OFF           ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_TRIGGER_ITEM;
   }
   if (bn == 128)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_TRIGGER_PBUL)   sprintf(smsg, "Triggered by Player's Bullets:ON ");
      else                                            sprintf(smsg, "Triggered by Player's Bullets:OFF");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_TRIGGER_PBUL;
   }
   if (bn == 129)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_TRIGGER_EBUL)   sprintf(smsg, "Triggered by Enemy's Bullets:ON  ");
      else                                            sprintf(smsg, "Triggered by Enemy's Bullets:OFF ");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_TRIGGER_EBUL;
   }
   if (bn == 133)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_DAMAGE_CURR) sprintf(smsg, "Damage Field Initially ON");
      else sprintf(smsg, "Damage Field Initially OFF");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_DAMAGE_CURR;
   }
   if (bn == 134)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_ON) sprintf(smsg, "Follows Lift:ON");
      else sprintf(smsg, "Follows Lift:OFF");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_LIFT_DMG_ON;
   }
   if (bn == 135)
   {
      if (Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_ON) sprintf(smsg, "Follows Lift:ON");
      else sprintf(smsg, "Follows Lift:OFF");
      if (press) Ei[num][3] ^= PM_ENEMY_FIELD_LIFT_TRG_ON;
   }










   if (bn == 136) // Damage Field X Lift Alignment
   {
      int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XC;
      int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XF;
      int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XL;
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
         int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XC;
         int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XF;
         int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_XL;


         if (C)    // 1 X X
         {  // set to 0 0 0
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XF; // clear F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XF; // clear F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_XF; // set   F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_XF; // set   F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_XC; // set   C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XL; // clear L flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_XF; // clear F flag
         }
      }
   }
   if (bn == 137) // Damage Field Y Lift Alignment
   {
      int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YC;
      int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YF;
      int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YL;

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
         int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YC;
         int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YF;
         int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_DMG_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YF; // clear F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YF; // clear F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_YF; // set   F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_YF; // set   F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_DMG_YC; // set   C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YL; // clear L flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_DMG_YF; // clear F flag
         }
      }
   }

   if (bn == 138) // Trigger Field X Lift Alignment
   {
      int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XC;
      int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XF;
      int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XL;

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
         int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XC;
         int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XF;
         int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_XL;
         if (C)    // 1 X X
         {  // set to 0 0 0
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XF; // clear F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XF; // clear F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_XF; // set   F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_XF; // set   F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_XC; // set   C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XL; // clear L flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_XF; // clear F flag
         }
      }
   }
   if (bn == 139) // Trigger Field Y Lift Alignment
   {
      int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YC;
      int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YF;
      int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YL;

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
         int C = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YC;
         int F = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YF;
         int L = Ei[num][3] & PM_ENEMY_FIELD_LIFT_TRG_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YF; // clear F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YC; // clear C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YF; // clear F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_YF; // set   F flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_YF; // set   F flag
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            Ei[num][3] |=  PM_ENEMY_FIELD_LIFT_TRG_YC; // set   C flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YL; // clear L flag
            Ei[num][3] &= ~PM_ENEMY_FIELD_LIFT_TRG_YF; // clear F flag
         }
      }
   }
   if (bn == 200)
   {
      sprintf(smsg, "Get New Trigger Field"); // item
      if (press)
          if (getbox("Draw New Trigger Field Rectangle", 2, 9, num))
         {

            if (bx2 < bx1) bx2 = bx1;
            if (by2 < by1) by2 = by1;
            item[num][6] = bx1*20;
            item[num][7] = by1*20;
            item[num][8] = (bx2-bx1)*20;
            item[num][9] = (by2-by1)*20;
            Redraw = 1;
         }
   }
   if (bn == 210)
   {
      if (item[num][3] & PM_ITEM_TRIGGER_LIFT_ON) sprintf(smsg, "Follows Lift:ON");
      else sprintf(smsg, "Follows Lift:OFF");
      if (press) item[num][3] ^= PM_ITEM_TRIGGER_LIFT_ON;
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
   if (bn == 300)
   {
      sprintf(smsg, "Get New Block Manip Field"); // item
      if (press)
         if (getbox("Draw New Block Manip Rectangle", 2, 16, num))
         {
            if (bx2 < bx1) bx2 = bx1;
            if (by2 < by1) by2 = by1;
            item[num][6] = bx1*20;
            item[num][7] = by1*20;
            item[num][8] = (bx2-bx1)*20;
            item[num][9] = (by2-by1)*20;
            Redraw = 1;
         }
   }
   if (bn == 301) // block manip mode
   {
      if (item[num][3] == 0) sprintf(smsg, "MODE:OFF");
      if (item[num][3] == 1) sprintf(smsg, "MODE:Set All To Block 1");
      if (item[num][3] == 2) sprintf(smsg, "MODE:Set All Block 2 To Block 1");
      if (item[num][3] == 3) sprintf(smsg, "MODE:Toggle Block 2 To Block 1");
      if (press) if (++item[num][3] > 3) item[num][3] = 0; // block manip mode
   }
   if (bn == 304) // Block Manip Draw
   {
      if (item[num][2])  sprintf(smsg, "Draw Block Manip Field:ON     ");
      else               sprintf(smsg, "Draw Block Manip Field:OFF    ");
      if (press) item[num][2] = !item[num][2]; // block manip draw mode
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
         int i = get_trigger_item("Select A Trigger To Link To", 2, item[num][0], num );
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
         Redraw = 1;
      }
   }
   if (bn == 400)
   {
      sprintf(smsg, "Get New Block Damage Field"); // item
      if (press)
         if (getbox("Draw New Block Damage Rectangle", 2, 17, num))
         {
            if (bx2 < bx1) bx2 = bx1;
            if (by2 < by1) by2 = by1;
            item[num][6] = bx1*20;
            item[num][7] = by1*20;
            item[num][8] = (bx2-bx1)*20;
            item[num][9] = (by2-by1)*20;
            Redraw = 1;
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
   if (bn == 402) // damage mode
   {
      if (item[num][11] == 0) sprintf(smsg, "MODE:Damage Field Always ON");
      if (item[num][11] == 1) sprintf(smsg, "MODE:Damage Field Toggle");
      if (item[num][11] == 2) sprintf(smsg, "MODE:Damage Field ON Until Triggered");
      if (item[num][11] == 3) sprintf(smsg, "MODE:Damage Field OFF Until Triggered");
      if (item[num][11] == 4) sprintf(smsg, "MODE:Damage Field Timed ON And OFF");
      if (press) if (++item[num][11] > 4) item[num][11] = 0;
   }
   if (bn == 403) // Instant death for player
   {
      if (item[num][3] & PM_ITEM_DAMAGE_INSTGIB) sprintf(smsg, "Player Instant Death:ON   ");
      else                                       sprintf(smsg, "Player Instant Death:OFF  ");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_INSTGIB;  // Instant death for player
   }
   if (bn == 404) // Block Damage draw mode
   {
      if (item[num][2] == 0) sprintf(smsg, "Block Damage Field Draw Type:none                   ");
      if (item[num][2] == 1) sprintf(smsg, "Block Damage Field Draw Type:Red Rectangle (default)");
      if (item[num][2] == 2) sprintf(smsg, "Block Damage Field Draw Type:Spikey Floor           ");
      if (press) if (++item[num][2] > 2) item[num][2] = 0; // draw mode
   }
   if (bn == 410)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_LIFT_ON) sprintf(smsg, "Follows Lift:ON");
      else sprintf(smsg, "Follows Lift:OFF");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_LIFT_ON;
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
   if (bn == 420)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_PLAYER) sprintf(smsg, "Affects Players:ON          ");
      else                                      sprintf(smsg, "Affects Players:OFF         ");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_PLAYER;
   }
   if (bn == 421)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_ENEMY)  sprintf(smsg, "Affects Enemies:ON          ");
      else                                      sprintf(smsg, "Affects Enemies:OFF         ");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_ENEMY;
   }
   if (bn == 422)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_ITEM)   sprintf(smsg, "Affects Items:ON            ");
      else                                      sprintf(smsg, "Affects Items:OFF           ");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_ITEM;
   }
   if (bn == 423)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_PBUL)   sprintf(smsg, "Affects Player's Bullets:ON ");
      else                                      sprintf(smsg, "Affects Player's Bullets:OFF");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_PBUL;
   }
   if (bn == 424)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_EBUL)   sprintf(smsg, "Affects Enemy's Bullets:ON  ");
      else                                      sprintf(smsg, "Affects Enemy's Bullets:OFF ");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_EBUL;
   }
   if (bn == 433)
   {
      if (item[num][3] & PM_ITEM_DAMAGE_CURR) sprintf(smsg, "Damage Field Initially ON");
      else sprintf(smsg, "Damage Field Initially OFF");
      if (press) item[num][3] ^= PM_ITEM_DAMAGE_CURR;
   }
   if (bn == 500) // lift mode
   {
      sprintf(smsg, "undefined mode (%d)", lifts[num].mode);
      if (lifts[num].mode == 0) sprintf(smsg, "Mode 0 - Normal");
      if (lifts[num].mode == 1) sprintf(smsg, "Mode 1 - Prox Run and Reset");
      if (press) if (++lifts[num].mode > 1) lifts[num].mode = 0; // lift mode
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
      int x = lift_steps[l][s].x;
      int y = lift_steps[l][s].y;
      int w = lift_steps[l][s].w;
      int h = lift_steps[l][s].h;
      int v = lift_steps[l][s].val;

      if (num == -1) sprintf(smsg, "Details");  // show row header
      if (num == 0)  sprintf(smsg, "blank");
      if (num == 1)  sprintf(smsg, "x:%4d y:%4d w:%4d h:%4d [speed:%d]", x, y, w, h, v);
      if (num == 2)  sprintf(smsg, "for Timer:%d", v);
      if (num == 3)  sprintf(smsg, "for Player prox:%d", v);
      if (num == 4)
      {
         if (v == 0) sprintf(smsg, "End Step - Loop to Start");
         if (v == 1) sprintf(smsg, "End Step - Warp to Start");
         if (v == 2) sprintf(smsg, "End Step - Freeze Here");
      }
      if (num == 5)  sprintf(smsg, "for Trigger Event:%d", v);
   }
   if (bn == 504)
   {
      sprintf(smsg, "Name:%s", lifts[num].lift_name); // edit lift name
      if (press) return 1;
   }
   if (bn == 505) // lift step end step mode
   {
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
   if (bn == 510)
   {
      if (lift_steps[num][type].type & PM_LIFT_NO_DRAW)   sprintf(smsg, "Hide Lift");
      else                                                sprintf(smsg, "Draw Lift");
      if (press) lift_steps[num][type].type ^= PM_LIFT_NO_DRAW;
   }
   if (bn == 511)
   {
      if (lift_steps[num][type].type & PM_LIFT_SOLID_PLAYER) sprintf(smsg, "Solid for Player:ON ");
      else                                                   sprintf(smsg, "Solid for Player:OFF");
      if (press) lift_steps[num][type].type ^= PM_LIFT_SOLID_PLAYER;
   }
   if (bn == 512)
   {
      if (lift_steps[num][type].type & PM_LIFT_SOLID_ENEMY)  sprintf(smsg, "Solid for Enemy:ON  ");
      else                                                   sprintf(smsg, "Solid for Enemy:OFF ");
      if (press) lift_steps[num][type].type ^= PM_LIFT_SOLID_ENEMY;
   }
   if (bn == 513)
   {
      if (lift_steps[num][type].type & PM_LIFT_SOLID_ITEM)   sprintf(smsg, "Solid for Item:ON   ");
      else                                                   sprintf(smsg, "Solid for Item:OFF  ");
      if (press) lift_steps[num][type].type ^= PM_LIFT_SOLID_ITEM;
   }


   if (bn == 514)
   {
      if (lift_steps[num][type].type & PM_LIFT_HIDE_LINES)   sprintf(smsg, "Hide Lift Lines");
      else                                                   sprintf(smsg, "Draw Lift Lines");
      if (press) lift_steps[num][type].type ^= PM_LIFT_HIDE_LINES;
   }




   if (bn == 520)
   {
      sprintf(smsg, "Set Event Trigger (%d)", lift_steps[num][type].val);
      if (press)
      {
         int i = get_trigger_item("Select A Trigger To Link To", 4, type, num );
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



   if (bn == 600)
   {
      if (num) sprintf(smsg, "Blocks:ON ");
      else     sprintf(smsg, "Blocks:OFF");
      if (press) return 1;
   }

   if (bn == 601)
   {
      if (num) sprintf(smsg, "Enemies:ON ");
      else     sprintf(smsg, "Enemies:OFF");
      if (press) return 1;
   }
   if (bn == 602)
   {
      if (num) sprintf(smsg, "Items:ON ");
      else     sprintf(smsg, "Items:OFF");
      if (press) return 1;
   }
   if (bn == 603)
   {
      if (num) sprintf(smsg, "Lifts:ON ");
      else     sprintf(smsg, "Lifts:OFF");
      if (press) return 1;
   }

   if (bn == 604)
   {
      if (num) sprintf(smsg, "Flags:ON ");
      else     sprintf(smsg, "Flags:OFF");
      if (press) return 1;
   }




   if (bn == 620)
   {
      sprintf(smsg, "Move Selection");
      if (press) return 1;
   }

   if (bn == 621)
   {
      sprintf(smsg, "Clear Selection");
      if (press) return 1;
   }
   if (bn == 622)
   {
      sprintf(smsg, "Paste Selection");
      if (press) return 1;
   }

   if (bn == 623)
   {
      sprintf(smsg, "Save To Disk");
      if (press) return 1;
   }
   if (bn == 624)
   {
      sprintf(smsg, "Load From Disk");
      if (press) return 1;
   }






   if (bn == 610)
   {
      sprintf(smsg, "Block Fill");
      if (press) return 1;
   }
   if (bn == 611)
   {
      sprintf(smsg, "Block Frame");
      if (press) return 1;
   }
   if (bn == 612)
   {
      sprintf(smsg, "Block Floodfill");
      if (press) return 1;
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

      al_draw_filled_rectangle(x, y, x+20, y+20, palette_color[0]);
      al_draw_bitmap(btile[tn&1023], x, y, 0);
   }

   if (bn == 311)
   {
      int tn = item[num][11];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x, y, x+20, y+20, palette_color[0]);
      al_draw_bitmap(btile[tn&1023], x, y, 0);
   }








   return retval;
}






void mdw_colsel(int x1, int y1, int x2, int y2, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
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
   if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2) && (mouse_b1))
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
      if (bn == 8)
      {
        // printf("n:%d t:%d c:%d\n",num, type, color);
         int cf = color << 28; // shift 4 bits of color into place
         lift_steps[num][type].type &= 0b00001111111111111111111111111111; // clear old color
         lift_steps[num][type].type |= cf; // merge color with type

      }
      Redraw = 1;
   }
}








int mdw_toggle(int x1, int y1, int x2, int y2,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)


{
   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;
   if ((mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      press = 1;
   }
   if (bn == 1000)
   {
      if (press) var = ! var;

      if (var)
      {
          q1 = frame_col1;
          q2 = text_col1;
          sprintf(smsg, "%s", t1);

      }
      else
      {
         q1 = frame_col0;
         q2 = text_col0;
         sprintf(smsg, "%s", t0);
      }
   }


   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame

   if (q5) al_draw_text(font, palette_color[q2], x1+4, (y2+y1)/2-3, 0, smsg);
   else al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   return retval;

}



int mdw_togglf(int x1, int y1, int x2, int y2,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)


{
   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;
   if ((mouse_b1) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      while (mouse_b1) proc_controllers(); // wait for release
      press = 1;
   }
   if (bn == 1000)
   {
      if (press) var ^= flag;

      if (var & flag)
      {
          q1 = frame_col1;
          q2 = text_col1;
          sprintf(smsg, "%s", t1);

      }
      else
      {
         q1 = frame_col0;
         q2 = text_col0;
         sprintf(smsg, "%s", t0);
      }
   }


   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame

   if (q5) al_draw_text(font, palette_color[q2], x1+4, (y2+y1)/2-3, 0, smsg);
   else al_draw_text(font, palette_color[q2], (x2+x1)/2, (y2+y1)/2-3, ALLEGRO_ALIGN_CENTER, smsg);

   return retval;

}





















