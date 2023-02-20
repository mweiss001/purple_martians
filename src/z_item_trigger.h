// z_item_trigger.h

int proc_orb_shot_collision(int i);
void proc_orb(int i);
void draw_orb(int i, int x, int y);
void proc_orb_collision(int p, int i);

void proc_trigger(int i);
void set_item_trigger_location_from_lift(int i, int a20);
void detect_trigger_collisions(int i);
void draw_trigger(int i, int x, int y);

void proc_block_manip(int i);
void draw_block_manip(int i, int x, int y);

void set_item_damage_location_from_lift(int i, int a20);
void proc_item_damage_collisions(int i);
void draw_block_damage(int i, int x, int y, int custom);
void proc_block_damage(int i);

