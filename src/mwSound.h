// mwSound.h

class mwSound
{
   public:

   mwSound(); // default constructor
   void initialize(void);

   ALLEGRO_VOICE *voice = NULL;
   ALLEGRO_MIXER *mn_mixer = NULL;
   ALLEGRO_MIXER *se_mixer = NULL;
   ALLEGRO_MIXER *st_mixer = NULL;
   ALLEGRO_SAMPLE *snd[20];
   ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
   ALLEGRO_AUDIO_STREAM *pm_theme_stream;
   int fuse_loop_playing;
   int sample_delay[8];
   int se_scaler=5;
   int st_scaler=5;
   int lit_item;
   int sound_on = 1;


   void start_music(int resume);
   void stop_sound(void);
   void proc_sound(void);
   void load_sound(void);
   void set_se_scaler(void);
   void set_st_scaler(void);
   void sound_toggle(void);






};
extern mwSound mSound;


