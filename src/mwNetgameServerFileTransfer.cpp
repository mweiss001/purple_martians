// mwNetgameServerFileTransfer.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPacketBuffer.h"
#include "mwGameMoves.h"
#include "mwLog.h"
#include "mwPlayer.h"


void mwNetgame::server_send_file(int i)
{
   // does the file exist?
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(files_to_send[i].name);
   if (!al_fs_entry_exists(FS_fname))
   {
      printf("%s does not exist\n", al_get_fs_entry_name(FS_fname));
      files_to_send[i].active = 0;
      return;
   }

   // the file exists, get filename and size
   files_to_send[i].active = 2;
   char fname[1024];
   sprintf(fname, "%s", files_to_send[i].name);
   int fsize = al_get_fs_entry_size(FS_fname);
   //printf("%s exists -- size:%d \n", fname, fsize);

   // buffer for uncompressed file and filename
   char buf[fsize + 128];

   // read the file into the buffer
   FILE *fp = fopen(fname, "rb");
   if (!fp)
   {
      printf("Error opening %s", fname);
      return;
   }
   fread(buf + 128, sizeof(buf), 1, fp);
   fclose(fp);

   // add filename at the start of the buffer
   sprintf(buf, "%s", fname);

   // destination for the compressed data structure
   char dst[fsize + 128];

   // compress
   uLongf destLen = sizeof(dst);
   compress((Bytef*)dst, (uLongf*)&destLen, (Bytef*)buf, sizeof(buf));
   int dst_size = destLen;
   //printf("Compressed size:%d\n", dst_size);

   // break into pieces and send
   int num_packets = (dst_size / 1000) + 1;

   if (num_packets > 250) printf("File [%s] transfer aborted (more than 250 packets required)\n", fname);
   else
   {
      //float cr = (float)dst_size*100 / (float)(fsize+128); // compression ratio
      //printf("tx sfil fn:[%d] size:[%d] ratio:[%3.2f] [%d packets needed]\n", mLoop.frame_num, dst_size, cr, num_packets);

      int start_byte = 0;
      for (int packet_num=0; packet_num < num_packets; packet_num++)
      {
         int packet_data_size = 1000; // default size
         if (start_byte + packet_data_size > dst_size) packet_data_size = dst_size - start_byte; // last piece is smaller

         //printf("tx sfil piece fn:[%d] packet:[%d of %d]\n", mLoop.frame_num, packet_num+1, num_packets);

         char data[1024] = {0}; int pos;
         mPacketBuffer.PacketName(data, pos, "sfil");
         mPacketBuffer.PacketPutInt4(data, pos, files_to_send[i].id);
         mPacketBuffer.PacketPutByte(data, pos, packet_num);
         mPacketBuffer.PacketPutByte(data, pos, num_packets);
         mPacketBuffer.PacketPutInt4(data, pos, start_byte);
         mPacketBuffer.PacketPutInt4(data, pos, packet_data_size);
         mPacketBuffer.PacketPutInt4(data, pos, fsize); // uncompressed file size

         memcpy(data+pos, dst+start_byte, packet_data_size);
         pos += packet_data_size;

         ServerSendTo(data, pos, files_to_send[i].p);

         start_byte+=1000;
      }
   }
}

void mwNetgame::server_proc_sfak_packet(int i)
{
   int p = mPlayer.active_local_player;
   int id = mPacketBuffer.PacketGetInt4(i);  // client has acknowledged getting this file id
   mLog.addf(LOG_NET_file_transfer, p, "rx sfak - client acknowledged getting file id:%d\n", id);
   for (int i=0; i<20; i++)
      if (files_to_send[i].id == id)
      {
         files_to_send[i].active = 0;
       //  mLog.appf(LOG_NET_file_transfer, "%s\n", files_to_send[i].name);
        //printf("filename:%s]\n", files_to_send[i].name);
      }
}

void mwNetgame::server_proc_crfl_packet(int i)
{
   int p = mPacketBuffer.rx_buf[i].p;
   mLog.addf(LOG_NET_file_transfer, p, "rx clrf - client requested file\n");
   mGameMoves.save_gm_make_fn("server save on rx crfl packet", p);
}

void mwNetgame::server_add_file_to_send(const char * filename, int p)
{
   // does the file exist?
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(filename);
   if (!al_fs_entry_exists(FS_fname))
   {
      mLog.addf(LOG_NET_file_transfer, p, "file:%s does not exist\n", filename);
      printf("file:%s does not exist\n", filename);
      return;
   }

   // the file exists, get size and test
   int fsize = al_get_fs_entry_size(FS_fname);
   if (fsize > 200000)
   {
      mLog.addf(LOG_NET_file_transfer, p, "file:%s too large %d > 200,000\n", filename, fsize);
      printf("file:%s too large %d > 200,000\n", filename, fsize);
      return;
   }

   for (int i=0; i<20; i++)
      if (!files_to_send[i].active)
      {
         files_to_send[i].id = rand(); // random id
         files_to_send[i].active = 1;
         files_to_send[i].attempts = 0;
         files_to_send[i].p = p;
         sprintf(files_to_send[i].name, "%s", filename);
         return;
      }
}

void mwNetgame::server_proc_files_to_send(void)
{
   // check if file transfer in progress, waiting for acknowledgement
   int in_prog = 0;
   for (int i=0; i<20; i++)
      if (files_to_send[i].active > 1)
      {
         in_prog = 1;
         files_to_send[i].active++;
         //printf("file transfer [%s] -- wait:[%d] attempt[%d]\n", files_to_send[i].name, files_to_send[i].active, files_to_send[i].attempts);
         if (files_to_send[i].active > 9) // waited 10 frames for ack
         {
            if (++files_to_send[i].attempts < 4) server_send_file(i); // resend
            else files_to_send[i].active = 0;                         // abort
         }
      }
   // if no transfer in progress, see if we have one to start
   if (!in_prog)
   {
      for (int i=0; i<20; i++)
         if (files_to_send[i].active == 1)
         {
            mLog.addf(LOG_NET_file_transfer, mPlayer.active_local_player, "starting file transfer [%s]\n", files_to_send[i].name);
            //printf("starting file transfer [%s]\n", files_to_send[i].name);
            server_send_file(i);
         }
   }
}

