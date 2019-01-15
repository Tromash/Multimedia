#include <it/mat.h>
#include <it/vec.h>
#include <it/io.h>
#include <it/wavelet2D.h>

#include "utils.h"
#include "masking.h"

mat mat_estimate( mat m )
{

  /* Config parameters */
  uint s          = 9; 

  /* Local variables */
  mat    lv     = NULL, 
         lm     = NULL, 
         e      = NULL; 

  mat output; 
  
  double sigma2w = 0.,
         sigma2x = 0.; 
  uint   i = 0, 
         j = 0; 

  e = mat_clone( m );

  lm = mat_localmeanestimate( m, s );
  lv = mat_localvarestimate( m, s ); 

  sigma2w = mat_mean( lv );

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      {
	sigma2x = lv[i][j]-sigma2w<0.?0.:lv[i][j]-sigma2w; 

	e[i][j] = sigma2w/(sigma2w+sigma2x)*( m[i][j]-lm[i][j] );
      }

  output = mat_scale( e );

  mat_pgm_write( "estimation.pgm", output );

  mat_delete( output );
  
  mat_delete( lv );
  mat_delete( lm );

  return( e );

}
