// z_sound.h

extern ALLEGRO_SAMPLE *snd[20];
extern ALLEGRO_SAMPLE_INSTANCE *sid_hiss;
extern ALLEGRO_AUDIO_STREAM *pm_theme_stream;
extern int fuse_loop_playing;
extern int sample_delay[8];
extern int st_scaler;
extern int se_scaler;
extern int lit_item;
extern int sound_on;


// z_sound.h
void start_music(int resume);
void stop_sound(void);
void proc_sound(void);
void load_sound(void);
void set_se_scaler(void);
void set_st_scaler(void);
void sound_toggle(void);


