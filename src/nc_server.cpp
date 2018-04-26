// nc_server.cpp  20171203

// this is my test of using channels to replace connections

#include "pm.h"

#include "n_network.h"
#include "n_server.h"
#include "n_packet.h"


#ifdef CHANNEL
extern NET_CHANNEL *ListenChannel;     // listen channel
extern NET_CHANNEL *ClientChannel[32]; // array of channels
#endif // end of #ifdef CHANNEL



int ServerInit();
void ServerExit();
int ServerListen();
int ServerReceive(void *data, int *sender);
void ServerSendTo(void *data, int len, int who);
void ServerBroadcast(void *data, int len);



void nc_server(void)
{
   static int who;

   if (ServerInit())  printf ("Error initializing server\n");

   int quit = 0;
   while (!quit)
   {
      if (key[KEY_ESC]) quit = 1;
    
      ServerListen(); // listen for connections
  
      packetsize = ServerReceive(packetbuffer, &who); // get packets
      if (packetsize > 0)
      {
         if(PacketRead("poop"))  
         {
            int pc = Packet2ByteRead();
//            printf("poop:[%d]\n", pc);

            pc++; 
            
//            printf("Sending peep packet back to client\n");
            Packet("peep"); 
            PacketAdd2Bytes(pc);
            ServerSendTo(packetbuffer, packetsize, who); 
         }
      }



      if (key[KEY_0] && ClientNum)
      {
         while (key[KEY_0]);
         Packet("stoc"); 
         PacketAddByte(0);
         ServerSendTo(packetbuffer, packetsize, 0); 
      }           
      
   }
}     






















































/*

void check_for_new_connections()
{
   char buffer[1000];
	char address[32];
	int x = net_receive (ListenChannel, buffer, 999, address);
	if (x && (strcmp(buffer, "123") == 0))
   {

      if (!(ClientChannel[ClientChanNum] = net_openchannel (NetworkDriver, NULL)))
      {
			printf("!!! failed to open channel for %s\n", address);
			return;
      }	

		if (net_assigntarget (ClientChannel[ClientChanNum], address))
      {
			printf("!!! couldn't assign target `%s' to channel\n",address);
			net_closechannel (ClientChannel[ClientChanNum]);
			return;
		}

		printf("+++ join from %s\n", address);

      char bufr[32] = "456";
      net_send (ClientChannel[ClientChanNum], bufr, strlen (bufr) + 1);

      ClientChanNum++;

	}
}


void nc_server(void)
{
   if(NetworkInit()) printf ("Error initializing network\n");

   char buffer11[32] = "Data to send s1";
   char buffer12[32] = "Data to send s2";
   char buffer13[32] = "Data to send s3";
   char buffer2[32] = "";
   int x;

   // open the listening channel;
   ListenChannel = net_openchannel (NetworkDriver, "");
   net_assigntarget (ListenChannel, "");
     
   
   int quit = 0;
   while (!quit)
   {
      if (key[KEY_ESC]) quit = 1;
   

      check_for_new_connections();
   
      // poll existing clients for data
  
      if (ClientChanNum) 
      {
         for (int c=0; c<ClientChanNum; c++)
            if (net_query(ClientChannel[c])) 
            { 
               char address[32];
               packetsize = net_receive(ClientChannel[c], packetbuffer, 1024, address);
               if (x > 0)
               {
//                  printf("Received data:[%s] from %s  packetsize:[%d]\n", packetbuffer, address, packetsize);
                  if(PacketRead("sdat"))  
                  {
                     int pc = Packet3ByteRead();
                     printf("sdat packet rx'd with pc:%d\n", pc);
                  }

                  if(PacketRead("cdat"))  
                  {
                     int pc = Packet2ByteRead();
                     printf("cdat packet rx'd with pc:%d\n", pc);
                  }

                  if(PacketRead("poop"))  
                  {
                     int pc = Packet3ByteRead();
                     int pd = Packet2ByteRead();
                     printf("cdat packet rx'd with pc:%d pd:%d\n", pc, pd);

                     
                     printf("Sending peep packet back to client\n");
                     Packet("peep"); 
                     PacketAdd2Bytes(983);
                     net_send (ClientChannel[c], packetbuffer, strlen(packetbuffer) + 1);


                  }

  
               }
               if (x < 0) printf ("Error receiving data.\n");
            }
      }
  
      if (ClientChanNum) 
      {
         if (key[KEY_1]) net_send (ClientChannel[0], buffer11, strlen (buffer11) + 1);
         if (key[KEY_2]) net_send (ClientChannel[0], buffer12, strlen (buffer12) + 1);
         if (key[KEY_3]) net_send (ClientChannel[0], buffer13, strlen (buffer13) + 1);
         while ( (key[KEY_1]) || (key[KEY_2]) || (key[KEY_3]) );
      }
      
   }
}     

*/

