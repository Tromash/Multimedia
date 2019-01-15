#include <it/vec.h>
#include <it/mat.h>
#include <it/source.h>
#include <it/random.h>
#include <it/linalg.h>

#include "utils.h"
#include "constants.h"
#include "upsampling.h"


mat make_carriers( int Nc, int Nv, unsigned long int key, uint ups )
{

  mat temp; 
  mat u; 
  vec v;

  int i= 0; 

  Nv /= ups*ups;
  
  temp = mat_new_zeros( (size_t)Nv, (size_t)Nc );

  it_seed( key );

  for ( i= 0; i< Nc; i++ ) 
    {
      v = source_gaussian( (size_t)Nv, 0.0, 1.0 ); 
      vec_decr( v, vec_mean( v ) ); 
      mat_set_col( temp, (size_t)i, v );
      vec_delete(v);
    }  

  mat_gs( temp ); 

  for ( i= 0; i< Nc; i++ ) 
    {
      v = mat_get_col( temp, i ); 
      vec_div_by( v, sqrt(vec_variance(v)) );

      mat_set_col( temp, i, v );
      vec_delete(v);
    }

  u = mat_upsample_carriers( temp, ups );

  mat_delete( temp );

  return( u );

}

vec get_correlations( mat y, uint key, uint sz, uint Nc, bvec mest, uint ups )
{

  uint Nv = sz*sz;
  mat U = make_carriers( Nc,Nv,key, ups );
  vec corrs = vec_new_zeros( Nc );

  mat_delete( U );
  
  return corrs; 
}

