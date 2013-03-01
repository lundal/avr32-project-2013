#ifndef TONES_H
#define TONES_H

#include "sound.h"

#define TONES_TOTAL 120

#define C_2 tones[24]
#define Cx2 tones[25]
#define D_2 tones[26]
#define Dx2 tones[27]
#define E_2 tones[28]
#define F_2 tones[29]
#define Fx2 tones[30]
#define G_2 tones[31]
#define Gx2 tones[32]
#define A_2 tones[33]
#define Ax2 tones[34]
#define B_2 tones[35]

#define C_3 tones[36]
#define Cx3 tones[37]
#define D_3 tones[38]
#define Dx3 tones[39]
#define E_3 tones[40]
#define F_3 tones[41]
#define Fx3 tones[42]
#define G_3 tones[43]
#define Gx3 tones[44]
#define A_3 tones[45]
#define Ax3 tones[46]
#define B_3 tones[47]

#define C_4 tones[48]
#define Cx4 tones[49]
#define D_4 tones[50]
#define Dx4 tones[51]
#define E_4 tones[52]
#define F_4 tones[53]
#define Fx4 tones[54]
#define G_4 tones[55]
#define Gx4 tones[56]
#define A_4 tones[57]
#define Ax4 tones[58]
#define B_4 tones[59]

#define C_5 tones[60]
#define Cx5 tones[61]
#define D_5 tones[62]
#define Dx5 tones[63]
#define E_5 tones[64]
#define F_5 tones[65]
#define Fx5 tones[66]
#define G_5 tones[67]
#define Gx5 tones[68]
#define A_5 tones[69]
#define Ax5 tones[70]
#define B_5 tones[71]

#define C_6 tones[72]
#define Cx6 tones[73]
#define D_6 tones[74]
#define Dx6 tones[75]
#define E_6 tones[76]
#define F_6 tones[77]
#define Fx6 tones[78]
#define G_6 tones[79]
#define Gx6 tones[80]
#define A_6 tones[81]
#define Ax6 tones[82]
#define B_6 tones[83]

#define C_7 tones[84]
#define Cx7 tones[85]
#define D_7 tones[86]
#define Dx7 tones[87]
#define E_7 tones[88]
#define F_7 tones[89]
#define Fx7 tones[90]
#define G_7 tones[91]
#define Gx7 tones[92]
#define A_7 tones[93]
#define Ax7 tones[94]
#define B_7 tones[95]

// Tones
sample_t **tones;
sample_t *silence;

// Prototypes
void tones_init();

#endif
