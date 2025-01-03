// pm.h

#define PM_VERSION "7.24.0811.1"

#include <stdio.h>
#include <math.h>
#include <chrono>
#include <libnet.h>
#include <zlib.h>


#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#define NUM_SPRITES 1024
#define NUM_ANS 256
#define NUM_PLAYERS 8
#define BORDER_WIDTH 14
#define NUM_LIFTS 40

#define STATE_SIZE 112480

#define PACKET_BUFFER_SIZE 1500
#define PACKET_PAYLOAD_CHUNK_SIZE 1400

#define PM_COMPMOVE_LEFT   0b00000001
#define PM_COMPMOVE_RIGHT  0b00000010
#define PM_COMPMOVE_UP     0b00000100
#define PM_COMPMOVE_DOWN   0b00001000
#define PM_COMPMOVE_JUMP   0b00010000
#define PM_COMPMOVE_FIRE   0b00100000
#define PM_COMPMOVE_MENU   0b01000000


#define PM_GAMEMOVE_TYPE_LEVEL_START      0
#define PM_GAMEMOVE_TYPE_PLAYER_ACTIVE    1
#define PM_GAMEMOVE_TYPE_PLAYER_INACTIVE  2
#define PM_GAMEMOVE_TYPE_PLAYER_HIDDEN    3
#define PM_GAMEMOVE_TYPE_PLAYER_MOVE      5
#define PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK   8
#define PM_GAMEMOVE_TYPE_SHOT_CONFIG      9

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
#define PM_BTILE_MOST_FLAGS        0b00111111111111110000000000000000
#define PM_BTILE_ALL_FLAGS         0b01111111111111110000000000000000
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
#define PM_BTILE_SECRET            0b00100000000000000000000000000000
#define PM_BTILE_SHOW_SELECT_WIN   0b01000000000000000000000000000000

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

#define PM_ITEM_DAMAGE_PLAYER   0b000000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b000000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b000000000000000100
#define PM_ITEM_DAMAGE_PSHOT    0b000000000000001000
#define PM_ITEM_DAMAGE_ESHOT    0b000000000000010000
#define PM_ITEM_DAMAGE_CURR     0b000000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b000000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b000000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b000000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b000000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b000000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b000000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b000001000000000000
#define PM_ITEM_DAMAGE_INSTGIB  0b100000000000000000

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

#define PM_CFG_SAVE_DISPLAY                     1
#define PM_CFG_SAVE_SOUND                       2
#define PM_CFG_SAVE_CONTROLS                    3
#define PM_CFG_SAVE_BOTTOM_MESSAGE              4
#define PM_CFG_SAVE_LOCAL_PLAYER_COLOR          5
#define PM_CFG_SAVE_START_LEVEL                 6
#define PM_CFG_SAVE_SERVER_ADDRESS              7
#define PM_CFG_SAVE_NETGAME_SHOTS               8
#define PM_CFG_SAVE_NETGAME_CLIENT_CHASE_OFFSET 9
#define PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED 10



