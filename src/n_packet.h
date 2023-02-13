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

extern char packetbuffer[1024];
extern int packetsize;
void set_packetpos(int pos);
int get_packetpos(void);
void Packet(const char *id);
int PacketRead(const char *id);
void PacketAddByte(char b);
char PacketGetByte(void);
void PacketAddString(char*);
void PacketReadString(char*);
void PacketPut1ByteInt(int b);
void PacketPut2ByteInt(int b);
void PacketPut3ByteInt(int b);
void PacketPut4ByteInt(int b);
int PacketGet1ByteInt(void);
int PacketGet2ByteInt(void);
int PacketGet3ByteInt(void);
int PacketGet4ByteInt(void);
void PacketPutDouble(double);
double PacketGetDouble(void);
