#ifndef TONES_H
#define TONES_H

#include <stdint.h>

#include "sample.h"

#define C_3 36
#define Cx3 37
#define D_3 38
#define Dx3 39
#define E_3 40
#define F_3 41
#define Fx3 42
#define G_3 43
#define Gx3 44
#define A_3 45
#define Ax3 46
#define B_3 47

#define C_4 48
#define Cx4 49
#define D_4 50
#define Dx4 51
#define E_4 52
#define F_4 53
#define Fx4 54
#define G_4 55
#define Gx4 56
#define A_4 57
#define Ax4 58
#define B_4 59

#define C_5 60
#define Cx5 61
#define D_5 62
#define Dx5 63
#define E_5 64
#define F_5 65
#define Fx5 66
#define G_5 67
#define Gx5 68
#define A_5 69
#define Ax5 70
#define B_5 71

#define C_6 72
#define Cx6 73
#define D_6 74
#define Dx6 75
#define E_6 76
#define F_6 77
#define Fx6 78
#define G_6 79
#define Gx6 80
#define A_6 81
#define Ax6 82
#define B_6 83

#define C_7 84
#define Cx7 85
#define D_7 86
#define Dx7 87
#define E_7 88
#define F_7 89
#define Fx7 90
#define G_7 91
#define Gx7 92
#define A_7 93
#define Ax7 94
#define B_7 95

// Tones
sample_t **tones;
sample_t *silence;

// Prototypes
void tones_init(int16_t wave_type);

#endif
