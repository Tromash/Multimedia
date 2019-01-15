#include <it/mat.h>
#include <it/vec.h>
#include <it/parser.h>

#ifndef _UTILS_H_
#define _UTILS_H_

mat    mat_localmeanestimate( mat m, int s ); 
mat    mat_localvarestimate( mat m, int s ); 

vec get_correlations( mat y, uint key, uint sz, uint Nc, bvec mest, uint ups );
mat make_carriers( int Nc, int Nv, unsigned long int key, uint ups );
double mat_psnr( mat x, mat y );

mat mat_scale( mat m );
mat mat_resync( mat m );

#endif
