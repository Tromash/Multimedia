#include <it/mat.h>

#ifndef _SS_H_
#define _SS_H_

mat make_carriers( int Nc, int Nv, unsigned long int key, uint ups );
vec get_correlations( mat y, uint key, uint sz, uint Nc, bvec mest, uint ups );


#endif 
