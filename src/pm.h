// pm.h

#define PM_VERSION "7.24.0811.1b"

#include <stdio.h>
#include <math.h>
#include <chrono>
#include <libnet.h>
#include <zlib.h>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <format>

#include <random>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_color.h>


template <typename T> T map_range(T value, T fromLow, T fromHigh, T toLow, T toHigh)
{
   return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}






//#include <algorithm>
template <typename T>
class mRect
{
public:
   T x1;
   T y1;
   T x2;
   T y2;
   T w;
   T h;

   // default constructor:
   mRect() : x1(0), y1(0), x2(0), y2(0), w(0), h(0) {}


   mRect(T posX1, T posY1, T rectWidth, T rectHeight) { setXYWH(posX1, posY1, rectWidth, rectHeight); }

   // with 5 parameters to overload
   mRect(T posX1, T posY1, T posX2, T posY2, T junk)  { setX2Y2(posX1, posY1, posX2, posY2); }


   void setXYWH(T posX1, T posY1, T rectWidth, T rectHeight)
   {
      x1 = posX1;
      y1 = posY1;
      w = rectWidth;
      h = rectHeight;
      x2 = x1 + w;
      y2 = y1 + h;
   }
   void setX2Y2(T posX1, T posY1, T posX2, T posY2)
   {
      x1 = posX1;
      y1 = posY1;
      x2 = posX2;
      y2 = posY2;
      w = posX2 - posX1;
      h = posY2 - posY1;
   }

   void round()
   {
      float factor = std::pow(10.0f, 0);
      x1 = std::round(x1 * factor) / factor;
      x2 = std::round(x2 * factor) / factor;
      y1 = std::round(y1 * factor) / factor;
      y2 = std::round(y2 * factor) / factor;

//      w = std::round(w * factor) / factor;
//      h = std::round(h * factor) / factor;


      w = x2 - x1;
      h = y2 - y1;



   }
   void trunc()
   {
      x1 = (int) x1;
      y1 = (int) y1;
      x2 = (int) x2;
      y2 = (int) y2;
      w = (int) w;
      h = (int) h;
   }

   void move(T xMove, T yMove )
   {
      x1+= xMove;
      y1+= yMove;
      x2 = x1 + w;
      y2 = y1 + h;
   }

   void resize(T wAdj, T hAdj )
   {
      w+= wAdj;
      h+= hAdj;
      x2 = x1 + w;
      y2 = y1 + h;
   }

   void setWidth(T newWidth)
   {
      w = newWidth;
      x2 = x1 + w;
   }




   void set_clipping_rectangle()
   {
      al_set_clipping_rectangle(x1, y1, w, h);
   }



   void print()
   {
      //printf("x1:%f  y1:%f    x2:%f  y2:%f   w:%f h:%f\n", x1, y1, x2, y2, w, h);
      printf("x1:%f  x2:%f  w:%f  --  y1:%f  y2:%f  h:%f\n", x1, x2, w, y1, y2, h);
   }



   // Calculate core geometric properties
   T getArea()      const { return w * h; }
   T getPerimeter() const { return 2 * (w + h); }

   // Edge boundary getters
   T getLeft()   const { return x1; }
   T getTop()    const { return y1; }
   T getRight()  const { return x1 + w; }
   T getBottom() const { return y1 + h; }


   void draw_filled_rectangle(ALLEGRO_COLOR color)
   {
      al_draw_filled_rectangle(x1, y1, x2, y2, color);
   }

   void draw_rectangle(ALLEGRO_COLOR color, float thickness)
   {
      al_draw_rectangle(x1, y1, x2, y2, color, thickness);
   }


   // check if a point is inside the rectangle
   bool contains(T pointX, T pointY) const
   {
      return (pointX >= x1 && pointX <= x2 && pointY >= y1 && pointY <= y2);
   }



   // Check if this rectangle overlaps with another rectangle
   bool intersects(const mRect<T>& other) const
   {
      return (x1 < other.getRight() && getRight() > other.x1 && y1 < other.getBottom() && getBottom() > other.y1);
   }
};







struct client_status_buffer_row
{
   std::uint64_t timestamp;
   int frame;
   int p;
   int color;
   std::string name;
   std::string hostname;
   int cpu;
   int sync;
   int ping;
   int lcor;
   int rcor;
   int rwnd;
   int difs;
   int tkbs;
};



#define NUM_SPRITES 1024
#define NUM_ANS 256
#define NUM_PLAYERS 8
#define BORDER_WIDTH 14
#define NUM_LIFTS 40

#define STATE_SIZE 112704

#define PACKET_BUFFER_SIZE 1500
#define PACKET_PAYLOAD_CHUNK_SIZE 1400

#define PM_COMPMOVE_LEFT   0b00000001
#define PM_COMPMOVE_RIGHT  0b00000010
#define PM_COMPMOVE_UP     0b00000100
#define PM_COMPMOVE_DOWN   0b00001000
#define PM_COMPMOVE_JUMP   0b00010000
#define PM_COMPMOVE_FIRE   0b00100000
#define PM_COMPMOVE_MENU   0b01000000

#define PM_GAMEMOVE_TYPE_PLAYER_ACTIVE    1
#define PM_GAMEMOVE_TYPE_PLAYER_INACTIVE  2
#define PM_GAMEMOVE_TYPE_PLAYER_HIDDEN    3
#define PM_GAMEMOVE_TYPE_PLAYER_MOVE      5
#define PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK   8
#define PM_GAMEMOVE_TYPE_SHOT_CONFIG      9
//                                                    3       2       1       0
//                                             76543210765432107654321076543210
#define PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG  0b00000000100000000000000000000000




#define PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER  0
#define PM_PLAYER_CONTROL_METHOD_DEMO_MODE      1
#define PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE 2
#define PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL   3
#define PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL   4
#define PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN  8

#define PM_PLAYER_QUIT_REASON_MENU_KEY                          64
#define PM_PLAYER_QUIT_REASON_CLIENT_LOST_SERVER_CONNECTION     75
#define PM_PLAYER_QUIT_REASON_CLIENT_ENDED_GAME                 90
#define PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME                 92

#define PM_NETGAME_PACKET_TYPE_CDAT 1
#define PM_NETGAME_PACKET_TYPE_STAK 2
#define PM_NETGAME_PACKET_TYPE_CJON 3
#define PM_NETGAME_PACKET_TYPE_CJRC 4
#define PM_NETGAME_PACKET_TYPE_RCTL 5
#define PM_NETGAME_PACKET_TYPE_SFAK 6
#define PM_NETGAME_PACKET_TYPE_CRFL 7
#define PM_NETGAME_PACKET_TYPE_CLOG 8



#define PM_NETGAME_PACKET_TYPE_STDF 11
#define PM_NETGAME_PACKET_TYPE_SJON 12
#define PM_NETGAME_PACKET_TYPE_SJRC 13
#define PM_NETGAME_PACKET_TYPE_SNFO 14
#define PM_NETGAME_PACKET_TYPE_SFIL 15
#define PM_NETGAME_PACKET_TYPE_SRRF 16


#define PM_PROGRAM_STATE_QUIT                           0
#define PM_PROGRAM_STATE_MENU                           1
#define PM_PROGRAM_STATE_CONFIG                         3

#define PM_PROGRAM_STATE_DEMO_RECORD                    7

#define PM_PROGRAM_STATE_CLIENT_NEW_GAME               200
#define PM_PROGRAM_STATE_CLIENT_WAIT_FOR_JOIN          201
#define PM_PROGRAM_STATE_CLIENT_LEVEL_SETUP            202
#define PM_PROGRAM_STATE_CLIENT_WAIT_FOR_INITIAL_STATE 203

#define PM_PROGRAM_STATE_CLIENT_PREEXIT1               210
#define PM_PROGRAM_STATE_CLIENT_PREEXIT2               211
#define PM_PROGRAM_STATE_CLIENT_EXIT                   214

#define PM_PROGRAM_STATE_SERVER_NEW_GAME               300
#define PM_PROGRAM_STATE_SERVER_EXIT                   301

#define PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME        10
#define PM_PROGRAM_STATE_MAIN_GAME_LOOP                11
#define PM_PROGRAM_STATE_NEXT_LEVEL                    12
#define PM_PROGRAM_STATE_RESUME                        13
#define PM_PROGRAM_STATE_SINGLE_PLAYER_EXIT            14

#define PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP   40
#define PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_RUN     41


#define PM_ITEM_TYPE_DOOR     1
#define PM_ITEM_TYPE_BONUS    2
#define PM_ITEM_TYPE_EXIT     3
#define PM_ITEM_TYPE_KEY      4
#define PM_ITEM_TYPE_START    5
#define PM_ITEM_TYPE_ORB      6
#define PM_ITEM_TYPE_MINE     7
#define PM_ITEM_TYPE_BOMB     8
#define PM_ITEM_TYPE_TRIGGR   9
#define PM_ITEM_TYPE_MSG      10
#define PM_ITEM_TYPE_ROCKET   11
#define PM_ITEM_TYPE_TIMER    13
#define PM_ITEM_TYPE_SWITCH   14
#define PM_ITEM_TYPE_SPRING   15
#define PM_ITEM_TYPE_BLKMNP   16
#define PM_ITEM_TYPE_BLKDMG   17
#define PM_ITEM_TYPE_GATE     18
#define PM_ITEM_TYPE_HIDER    19

#define PM_ITEM_TYPE_LIT_RCKT 98
#define PM_ITEM_TYPE_LIT_BOMB 99


#define PM_ENEMY_VINEPOD_SHOW_PATH   0b00000000000000001
#define PM_ENEMY_VINEPOD_INV_INIT    0b00000000000000010
#define PM_ENEMY_VINEPOD_INV_EXTN    0b00000000000000100
#define PM_ENEMY_VINEPOD_INV_MOVE    0b00000000000001000
#define PM_ENEMY_VINEPOD_USE_SPLINE  0b00000000000010000
#define PM_ENEMY_VINEPOD_USE_TRIGGER 0b00000000000100000
#define PM_ENEMY_VINEPOD_FIRE_SHOT   0b00000000001000000


#define PM_LIFT_TYPE_BITS        0b00000000000000000000000000011111
#define PM_LIFT_FLAG_BITS        0b00001111111111111111111111100000
#define PM_LIFT_COLOR_BITS       0b11110000000000000000000000000000
#define PM_LIFT_NO_DRAW          0b00001000000000000000000000000000
#define PM_LIFT_SOLID_PLAYER     0b00000100000000000000000000000000
#define PM_LIFT_SOLID_ENEMY      0b00000010000000000000000000000000
#define PM_LIFT_SOLID_ITEM       0b00000001000000000000000000000000
#define PM_LIFT_HIDE_LINES       0b00000000100000000000000000000000

//                                   3       2       1       0
//                                   76543210765432107654321076543210
#define PM_BTILE_TILENUM_FLAGS     0b00000000000000000000001111111111
#define PM_BTILE_KEY_ACTIVE        0b00000000000000001000000000000000
#define PM_BTILE_KEY_COLOR1        0b00000000000000000100000000000000
#define PM_BTILE_KEY_COLOR0        0b00000000000000000010000000000000
#define PM_BTILE_ALL_FLAGS         0b01111111111111111110000000000000
#define PM_BTILE_ALL_SOLID         0b00000000000111110000000000000000
#define PM_BTILE_ALL_SEMI          0b00000000111001110000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000
#define PM_BTILE_SEMISOLID_PLAYER  0b00000000001000000000000000000000
#define PM_BTILE_SEMISOLID_ENEMY   0b00000000010000000000000000000000
#define PM_BTILE_SEMISOLID_ITEM    0b00000000100000000000000000000000
#define PM_BTILE_BOMBABLE          0b00000001000000000000000000000000
#define PM_BTILE_BREAKABLE_PSHOT   0b00000010000000000000000000000000
#define PM_BTILE_BREAKABLE_ESHOT   0b00000100000000000000000000000000
#define PM_BTILE_LADDER_MOVE       0b00001000000000000000000000000000
#define PM_BTILE_ROPE_MOVE         0b00010000000000000000000000000000
#define PM_BTILE_SHOW_OVERLAY      0b01000000000000000000000000000000

#define PM_BTILE_NUM_FLAGS 17


#define PM_ITEM_TRIGGER_PLAYER   0b00000000000000001
#define PM_ITEM_TRIGGER_ENEMY    0b00000000000000010
#define PM_ITEM_TRIGGER_ITEM     0b00000000000000100
#define PM_ITEM_TRIGGER_PSHOT    0b00000000000001000
#define PM_ITEM_TRIGGER_ESHOT    0b00000000000010000
#define PM_ITEM_TRIGGER_CURR     0b00000000000100000
#define PM_ITEM_TRIGGER_PREV     0b00000000001000000
#define PM_ITEM_TRIGGER_TGON     0b00000000010000000
#define PM_ITEM_TRIGGER_TGOF     0b00000000100000000
#define PM_ITEM_TRIGGER_LIFT_ON  0b00000001000000000
#define PM_ITEM_TRIGGER_LIFT_XC  0b00000010000000000
#define PM_ITEM_TRIGGER_LIFT_XF  0b00000100000000000
#define PM_ITEM_TRIGGER_LIFT_XL  0b00001000000000000
#define PM_ITEM_TRIGGER_LIFT_YC  0b00010000000000000
#define PM_ITEM_TRIGGER_LIFT_YF  0b00100000000000000
#define PM_ITEM_TRIGGER_LIFT_YL  0b01000000000000000
#define PM_ITEM_TRIGGER_DRAW_ON  0b10000000000000000


//                             3       2       1       0
//                      76543210765432107654321076543210
#define PM_ITEM_DAMAGE_PLAYER   0b0000000000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b0000000000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b0000000000000000000100
#define PM_ITEM_DAMAGE_PSHOT    0b0000000000000000001000
#define PM_ITEM_DAMAGE_ESHOT    0b0000000000000000010000
#define PM_ITEM_DAMAGE_CURR     0b0000000000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b0000000000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b0000000000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b0000000000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b0000000000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b0000000000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b0000000000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b0000000001000000000000
#define PM_ITEM_DAMAGE_LIFT_MS  0b0000000010000000000000
#define PM_ITEM_DAMAGE_ROTB     0b0000001100000000000000
#define PM_ITEM_DAMAGE_INSTGIB  0b0000100000000000000000









#define PM_ITEM_ORB_STATE       0b0000000000000001
#define PM_ITEM_ORB_PREV_STATE  0b0000000000000010
#define PM_ITEM_ORB_TGON        0b0000000000000100
#define PM_ITEM_ORB_TGOF        0b0000000000001000
#define PM_ITEM_ORB_TRIG_TOUCH  0b0000000000010000
#define PM_ITEM_ORB_TRIG_UP     0b0000000000100000
#define PM_ITEM_ORB_TRIG_DOWN   0b0000000001000000
#define PM_ITEM_ORB_TRIG_SHOT   0b0000000010000000
#define PM_ITEM_ORB_TRIG_CURR   0b0000000100000000
#define PM_ITEM_ORB_TRIG_PREV   0b0000001000000000
#define PM_ITEM_ORB_ROTB        0b1100000000000000





#define PM_ITEM_PMSG_SHOW_ALWAYS   0b00000000000000001
#define PM_ITEM_PMSG_SHOW_SCROLL   0b00000000000000010
#define PM_ITEM_PMSG_FRAME0        0b00000000000001000
#define PM_ITEM_PMSG_FRAME1        0b00000000000010000
#define PM_ITEM_PMSG_FRAME2        0b00000000000100000
#define PM_ITEM_PMSG_FRAME4        0b00000000001000000
#define PM_ITEM_PMSG_FRAME12       0b00000000010000000


#define PM_CFG_SAVE_DISPLAY                        1
#define PM_CFG_SAVE_SOUND                          2
#define PM_CFG_SAVE_CONTROLS                       3
#define PM_CFG_SAVE_BOTTOM_MESSAGE                 4
#define PM_CFG_SAVE_LOCAL_PLAYER_COLOR             5
#define PM_CFG_SAVE_START_LEVEL                    6
#define PM_CFG_SAVE_SERVER_ADDRESS                 7
#define PM_CFG_SAVE_NETGAME_SHOTS                  8
#define PM_CFG_SAVE_NETGAME_CLIENT_CHASE_OFFSET    9
#define PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED    10
#define PM_CFG_SAVE_PLAYER_NAME                    11
#define PM_CFG_LOAD_PLAYER_NAME_AND_COLOR          12




