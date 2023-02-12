// n_packet.h

void init_packet_buffer(void);

extern struct packet_buffer packet_buffers[200];
struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int who;
   int packetsize;
   char data[1024];
};



