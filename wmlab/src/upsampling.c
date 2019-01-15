#include <it/io.h>
#include <it/mat.h>

#include "utils.h"

mat mat_upsample( mat m, uint ups )
{
  mat u = mat_new( mat_height(m)*ups, mat_width(m)*ups );

  uint i, j; 

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      {
	uint k, l;

	for ( k= 0; k< ups; k++ )
	  for ( l= 0; l< ups; l++ )
	    u[i*ups+k][j*ups+l] = m[i][j];
      }
  
  return u; 
}


mat mat_upsample_carriers( mat m, uint ups )
{
  uint i;
  mat u = mat_new( mat_height(m)*ups*ups, mat_width(m) );
  mat output;
  
  for ( i= 0; i< mat_width(m); i++ )
    {
      vec v = mat_get_col( m, i );
      mat c = vec_to_mat( v, sqrt( mat_height(m) ) );

      mat up = mat_upsample( c, ups );

      output = mat_scale( up );
      
      mat_pgm_write( "carriers.pgm", output );

      mat_delete( output );
      
      vec_delete( v );

      v = mat_to_vec( up );

      mat_set_col( u, i, v );
      
      mat_delete( c );
      mat_delete( up );
      vec_delete( v );
    }

  return u; 
}

