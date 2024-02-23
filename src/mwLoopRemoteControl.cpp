// mwLoopRemoteControl.cpp

#include "pm.h"
#include "mwLoop.h"

#include "mwNetgame.h"
#include "mwPacketBuffer.h"

#include "mwVisualLevel.h"
#include "mwEventQueue.h"
#include "mwInput.h"

#include "mwPlayer.h"
#include "mwMiscFnx.h"
#include "mwLevel.h"

#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwWidget.h"
#include "mwQuickGraph2.h"

void mwLoop::remote_control_loop(void)
{

   double t0 = al_get_time();

   mPacketBuffer.rx_and_proc();

   int fn = mPlayer.loc[0].srv_frame_num; // set frame number from last snfo packet update

   // check how long since last rtcl packet has been sent, and send keep alive packet if too long
   if (++remote_frames_since_last_rctl_sent > 400) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_keep_alive, 0);


   char msg[200];
   al_set_target_backbuffer(mDisplay.display);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));

   int cx = 10, cy = 10;

   al_draw_textf(mFont.pr8, mColor.pc[13], cx, cy, 0, "Server Remote Control ");
   al_draw_textf(mFont.pr8, mColor.pc[15], cx+180, cy, 0, "Level:[%d] - Time:[%s] - Frame:[%d] - Moves:[%d]", mPlayer.loc[0].srv_level, mMiscFnx.chrms(fn, msg), fn, mPlayer.loc[0].srv_total_game_moves); cy+=20;

   mScreen.sdg_show(cx, cy); // server debug grid

   int i = 0;
   cx = 38;
   if (mWidget.buttontcb(cx, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "cpu " ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 1;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "sync" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 2;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "ping" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 3;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "difs" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 4;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "tkbs" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 5;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "rwnd" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 6;
   if (mWidget.buttontcb(cx+=48, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "lcor" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   i = 7;
   if (mWidget.buttontcb(cx+=48*3, cy, 0, 11, 0,0,0,0, 0,-1,mQuickGraph2[i].col1 + (!mQuickGraph2[i].active)*128,15, 0,0,0,0, "rcor" ))
   {
      mQuickGraph2[i].active = !mQuickGraph2[i].active;
      initialize_and_resize_remote_graphs();
   }

   al_set_target_backbuffer(mDisplay.display);

   cy+=78;


   // -------------------------------------------
   // first column
   // -------------------------------------------
   cx = 10;
   int rcy = cy; // remember cy





   int cs = 16; // control spacing
   int btw = 12, bth = 12; // button size
   int btc = 15+96; // button color

   int b1x = cx+2;
   int tx  = cx+32;
   int b2x = cx+52;


   int gfc = 10; // group frame color
   if (mWidget.togglec(cx, cy, cx+200, 16, 0,0,0,0, 0,gfc,15,0, 1,0,1,0, mPlayer.syn[0].server_force_client_offset, "Force Client Offset", 15, 15) ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_force_client_offset, 0);
   if (mWidget.buttont_nb(b1x, cy, b1x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "-") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_client_offset_adj, -0.005);
   al_draw_textf(mFont.pr8, mColor.pc[15], tx, cy+1, ALLEGRO_ALIGN_CENTER, "%2.0f", mPlayer.syn[0].client_chase_offset*1000);
   if (mWidget.buttont_nb(b2x, cy, b2x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "+") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_client_offset_adj, 0.005);
   al_draw_text(mFont.pr8, mColor.pc[15], cx+76, cy+1, 0, "Offset (ms)");
   al_draw_rectangle(b1x-2, cy-2, b1x+198, cy+cs-4, mColor.pc[gfc], 1);

   cy+=20;

   gfc = 9; // group frame color
   if (mWidget.togglec(cx, cy, cx+200, 16, 0,0,0,0, 0,9,15,0, 1,0,1,0, mPlayer.syn[0].player_vs_player_shots, "Player vs Player Shots", 15, 15) ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_pvp_shots_toggle, 0);
   cy-=2;
   if (mWidget.togglec(cx, cy, cx+200, 16, 0,0,0,0, 0,9,15,0, 1,0,1,0, mPlayer.syn[0].player_vs_self_shots,   "Player vs Self Shots",   15, 15) ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_pvs_shots_toggle, 0);


   int inc = 1;
   if (mInput.CTRL()) inc = 10;

   if (mWidget.buttont_nb(b1x, cy, b1x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "-") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj, -inc);
   al_draw_textf(mFont.pr8, mColor.pc[15], tx, cy+1, ALLEGRO_ALIGN_CENTER, "%d", mPlayer.syn[0].player_vs_player_shot_damage);
   if (mWidget.buttont_nb(b2x, cy, b2x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "+") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj, +inc);
   al_draw_text(mFont.pr8, mColor.pc[15], cx+76, cy+1, 0, "Shot Damage");
   al_draw_rectangle(b1x-2, cy-2, b1x+198, cy+cs-4, mColor.pc[gfc], 1);



   int mxcy = cy+16; // max cy, for marking where to start next section


   // -------------------------------------------
   // second column
   // -------------------------------------------
   cx = 240;
   cy = rcy+2; // restore cy

   b1x = cx + 2;
   tx  = cx +32;
   b2x = cx +52;

   gfc = 8; // group frame color
   if (mWidget.buttont_nb(b1x, cy, b1x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "-") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_zlib_compression_adj, -1);
   al_draw_textf(mFont.pr8, mColor.pc[15], tx, cy+1, ALLEGRO_ALIGN_CENTER, "%d", mPlayer.loc[0].srv_zlib_cmp);
   if (mWidget.buttont_nb(b2x, cy, b2x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "+") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_zlib_compression_adj, 1);
   al_draw_text(mFont.pr8, mColor.pc[15], cx+76, cy+1, 0, "zlib compression level");
   al_draw_rectangle(b1x-2, cy-2, b1x+270, cy+cs-4, mColor.pc[gfc], 1);

   cy+=20;

   gfc = 14; // group frame color
   if (mWidget.buttont_nb(b1x, cy, b1x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "-") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_exra_packet_num_adj, -1);
   al_draw_textf(mFont.pr8, mColor.pc[15], tx, cy+1, ALLEGRO_ALIGN_CENTER, "%d", mPlayer.loc[0].srv_extra_packets_num);
   if (mWidget.buttont_nb(b2x, cy, b2x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "+") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_exra_packet_num_adj, 1);
   al_draw_text(mFont.pr8, mColor.pc[15], cx+76, cy+1, 0, "number of extra packets");
   al_draw_rectangle(b1x-2, cy-2, b1x+270, cy+cs-4, mColor.pc[gfc], 1);

   cy+=cs-2;
   if (mWidget.buttont_nb(b1x, cy, b1x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "-") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_exra_packet_siz_adj, -100);
   al_draw_textf(mFont.pr8, mColor.pc[15], tx, cy+1, ALLEGRO_ALIGN_CENTER, "%d", mPlayer.loc[0].srv_extra_packets_size);
   if (mWidget.buttont_nb(b2x, cy, b2x+btw, bth, 0,0,0,0, 0,btc,15,0, 1,0,0,0, "+") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_exra_packet_siz_adj, 100);
   al_draw_text(mFont.pr8, mColor.pc[15], cx+76, cy+1, 0, "extra packet size");
   al_draw_rectangle(b1x-2, cy-2, b1x+270, cy+cs-4, mColor.pc[gfc], 1);


   cy+=20;

   gfc = 7; // group frame color
   if (mWidget.togglec(cx, cy, cx+200, 16, 0,0,0,0, 0,gfc,15,0, 1,0,1,0, mPlayer.syn[0].server_force_fakekey, "Server Force Fakekey", 15, 15) ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_fakekey_toggle, 0);



   cy+=20;


   // -------------------------------------------
   // third column
   // -------------------------------------------
   cx = 540;
   cy = rcy; // restore cy
   int cxc2 = cx + 100;
   if (mWidget.buttontca(cxc2, cy, 0, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, " Select and Reload Level ") )
   {
      if (mVisualLevel.visual_level_select() == 1) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_server_reload, mLevel.start_level);
   }
   cy+=6;
   if (mWidget.buttontca(cxc2, cy, 0, 16, 0,0,0,0, 0,9,15,0, 1,0,1,0, "  Reload Current Level   ") ) mNetgame.client_send_rctl_packet(PM_RCTL_PACKET_TYPE_server_reload, -1);




   cy+=10;
   static int show_bandwidth = 0;
   if (mWidget.buttontcb(cx, cy, 0, 13, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Show/Hide Bandwidth") ) show_bandwidth = !show_bandwidth;

   cy = mxcy; // position of next section inder buttons
   cx = 10;
   if (show_bandwidth) mScreen.draw_bandwidth_stats(cx, cy); // bandwidth stats


   // this is when I know how much space I have left for graphs

   int height = mDisplay.SCREEN_H - cy - 20;
   int width = mDisplay.SCREEN_W - 20;

   if ((remote_graphs_height != height) || (remote_graphs_width != width))
   {
      remote_graphs_height = height;
      remote_graphs_width = width;
      initialize_and_resize_remote_graphs();
   }



   for (int i=0; i<8; i++)
      if (mQuickGraph2[i].active) mQuickGraph2[i].draw_graph();


   mQuickGraph2[9].set_pos(mDisplay.SCREEN_W-mQuickGraph2[9].width-28, 4);
   mQuickGraph2[9].draw_graph();

   if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
   {
      while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
      state[0] = PM_PROGRAM_STATE_QUIT;
   }

   // --------------------------------------------
   // measure time it took to process loop
   // --------------------------------------------
   double pt = al_get_time() - t0;

   // convert to 'cpu', a percent of the total frame time (25ms)
   float cpu = (pt / 0.025) * 100;

   // store in local cpu variables
   add_local_cpu_data(cpu);

}
