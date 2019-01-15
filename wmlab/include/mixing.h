#include <it/mat.h>

#ifndef _MIXING_H_
#define _MIXING_H_

mat make_wmk_signal( mat x, bvec m, uint key, uint sz, double psnr, int psycho, uint ups );
mat mix_signals( mat x, mat w, double psnr, int psycho );


#endif
