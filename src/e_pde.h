// e_pde.h
int load_PDE();
void save_PDE();
void PDE_swap(int s1, int s2);
void PDE_sort(void);
void PDE_edit_text(int EN);
void predefined_enemies(void);

extern int PDEi[100][32];
extern al_fixed PDEfx[100][16];
extern char PDEt[100][20][40];
