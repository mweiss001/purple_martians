// zsound.cpp

#include "pm.h"

void load_sound() // for normal loading of sound driver and samples
{
   int err = 0;
   if (sound_on)
   {
      if(!al_install_audio())
      {
         m_err("Failed to initialize audio.\n");
         err = 1;
      }
      //else printf("init audio\n");

      if ((!err) && (!al_init_acodec_addon()))
      {
         m_err("Failed to initialize audio codecs.\n");
         err = 1;
      }
      //else printf("init audio codecs\n");

      if (err)
      {
         sound_on = 0;
         save_config();
      }
      else
      {
         voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
         if (voice == NULL) m_err("Failed to create voice.\n");
         al_set_default_voice(voice);

         mn_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
         if (mn_mixer == NULL) m_err("Failed to create mn_mixer\n");

         se_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
         if (se_mixer == NULL) m_err("Failed to create se_mixer\n");

         st_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
         if (st_mixer == NULL) m_err("Failed to create st_mixer\n");

         if (!al_attach_mixer_to_voice(mn_mixer, voice)) m_err("Failed attaching mn_mixer\n");
         if (!al_attach_mixer_to_mixer(se_mixer, mn_mixer)) m_err("Failed attaching se_mixer\n");
         if (!al_attach_mixer_to_mixer(st_mixer, mn_mixer)) m_err("Failed attaching st_mixer\n");

         al_set_default_mixer(se_mixer);
         al_reserve_samples(20);
         char fn[20] = "snd/snd00.wav";
         for (int x=0; x<10; x++)
         {
            fn[8] = 48 + x;
            snd[x] = al_load_sample(fn);
         }
         sid_hiss = al_create_sample_instance(snd[3]);
         al_set_sample_instance_playmode(sid_hiss, ALLEGRO_PLAYMODE_LOOP);
         al_attach_sample_instance_to_mixer(sid_hiss, se_mixer);

//         pm_theme_stream = al_load_audio_stream("snd/pm.mp3", 8, 1024);
//         if (pm_theme_stream == NULL) m_err("Error loading snd/pm.xm\n");

         pm_theme_stream = al_load_audio_stream("snd/pm.wav", 8, 1024);
         if (pm_theme_stream == NULL) m_err("Error loading snd/pm.wav\n");

         al_set_audio_stream_playmode(pm_theme_stream, ALLEGRO_PLAYMODE_LOOP);
         al_set_audio_stream_playing(pm_theme_stream, 0);
         al_attach_audio_stream_to_mixer(pm_theme_stream, st_mixer);

         set_se_scaler();
         set_st_scaler();
      }
   }
}

void set_se_scaler(void)
{
   if (sound_on) al_set_mixer_gain(se_mixer, (float)se_scaler / 9);
   save_config();
}

void set_st_scaler(void)
{
   if (sound_on) al_set_mixer_gain(st_mixer, (float)st_scaler / 9);
   save_config();
}

void start_music(int resume)
{
   // reset sound counters
   for (int c=0; c<8; c++) sample_delay[c] = frame_num;
   if (sound_on)
   {
      if (!resume) al_rewind_audio_stream(pm_theme_stream);
      al_set_audio_stream_playing(pm_theme_stream, 1);
   }
}

void stop_sound(void)
{
   if (sound_on)
   {
      al_set_audio_stream_playing(pm_theme_stream, 0);
      al_set_sample_instance_playing(sid_hiss, 0);
   }
}

void proc_sound()  // called once per frame
{
   if (sound_on)
   {
      if (lit_item) al_set_sample_instance_playing(sid_hiss, 1);
      else          al_set_sample_instance_playing(sid_hiss, 0);
      lit_item = 0;
   }
}

void sound_toggle(void)
{
   if (sound_on) sound_on = 0;
   else
   {
      sound_on = 1;
      load_sound();
   }
   save_config();
}
