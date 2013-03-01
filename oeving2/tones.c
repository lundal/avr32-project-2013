#include "tones.h"

// Generates tones
void tones_init() {
    A_1 = generate_square_sample(220);
    Ax1 = generate_square_sample(233);
    B_1 = generate_square_sample(247);
    C_1 = generate_square_sample(262);
    Cx1 = generate_square_sample(277);
    D_1 = generate_square_sample(294);
    Dx1 = generate_square_sample(311);
    E_1 = generate_square_sample(330);
    F_1 = generate_square_sample(349);
    Fx1 = generate_square_sample(370);
    G_1 = generate_square_sample(392);
    Gx1 = generate_square_sample(415);
    A_2 = generate_square_sample(440);
    Ax2 = generate_square_sample(466);
    B_2 = generate_square_sample(494);
    C_2 = generate_square_sample(524);
    Cx2 = generate_square_sample(554);
    D_2 = generate_square_sample(588);
    Dx2 = generate_square_sample(622);
    E_2 = generate_square_sample(660);
    F_2 = generate_square_sample(698);
    Fx2 = generate_square_sample(740);
    G_2 = generate_square_sample(784);
    Gx2 = generate_square_sample(830);
}
