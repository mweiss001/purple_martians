// nc_client.cpp  20171203

// this is my test of using channels to replace connections

#include "pm.h"


#include "n_network.h"
#include "n_server.h"
#include "n_packet.h"


#ifdef CHANNEL
extern NET_CHANNEL *ServerChannel;
#endif // end of #ifdef CHANNEL


int ClientInit(char *serveraddress);
void ClientExit(void);
int ClientCheckResponse(void);
int ClientReceive(void *data);
void ClientSend(void *data, int len) ;


void nc_client(void)
{
   if (ClientInit("192.168.1.170"))  printf("Client init failed\n");

//   if (ClientCheckResponse())        printf("No server response\n");

   while (ClientCheckResponse()); // wait until its zero        printf("No server response\n");

  
   int quit = 0;
   while (!quit)
   {
      if (key[KEY_ESC]) quit = 1;

      packetsize = ClientReceive(packetbuffer);

      if (packetsize > 0)
      {
         if(PacketRead("peep"))  
         {
            int pc = Packet2ByteRead();
//            printf("peep:[%d]\n", pc);

            pc++; 

            if (pc < 1000)
            {
//               printf("Sending poop packet back to server\n");
               Packet("poop"); 
               PacketAdd2Bytes(pc);
               ClientSend(packetbuffer, packetsize);
            }
            else
            {
               // stop the timer                
               int finish_time = clock();
               int time = finish_time - log_timer;
               printf("time:[%d]ms\n", time);
                
            }   

         }
      }
      
      if (key[KEY_1])
      {
         while (key[KEY_1]);

         log_timer = clock(); // start the timer...

         printf("Sending poop packet to server\n");
         Packet("poop"); 
         PacketAddByte(0);
         ClientSend(packetbuffer, packetsize);
      }
   }
}   
















