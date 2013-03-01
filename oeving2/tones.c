#include "tones.h"

// Generates tones
void tones_init() {
    silence = sample_gen_silence();
    A_1 = sample_gen_square(220);
    Ax1 = sample_gen_square(233);
    B_1 = sample_gen_square(247);
    C_1 = sample_gen_square(262);
    Cx1 = sample_gen_square(277);
    D_1 = sample_gen_square(294);
    Dx1 = sample_gen_square(311);
    E_1 = sample_gen_square(330);
    F_1 = sample_gen_square(349);
    Fx1 = sample_gen_square(370);
    G_1 = sample_gen_square(392);
    Gx1 = sample_gen_square(415);
    A_2 = sample_gen_square(440);
    Ax2 = sample_gen_square(466);
    B_2 = sample_gen_square(494);
    C_2 = sample_gen_square(524);
    Cx2 = sample_gen_square(554);
    D_2 = sample_gen_square(588);
    Dx2 = sample_gen_square(622);
    E_2 = sample_gen_square(660);
    F_2 = sample_gen_square(698);
    Fx2 = sample_gen_square(740);
    G_2 = sample_gen_square(784);
    Gx2 = sample_gen_square(830);
}
