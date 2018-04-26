extern char packetbuffer[1024];
extern int packetsize;

void Packet(char *id);
int PacketRead(char *id);

void PacketAddByte(char b);
char PacketGetByte(void);

void PacketAdd2Bytes(int);
int Packet2ByteRead(void);

void PacketAdd3Bytes(int);
int Packet3ByteRead(void);

void PacketAdd4Bytes(int);
int Packet4ByteRead(void);

void PacketAddString(char*);
void PacketReadString(char*);
