#include <it/vec.h>
#include <it/mat.h>
#include <it/io.h>
#include <it/filter.h>

#include "utils.h"

/*
  Return Sobel gradient of input image
 */

mat mat_sobel( mat x )
{
  int i, j; 
  mat sx, sy, s; 

  mat xfilter = mat_new( 3, 3 );
  mat yfilter = mat_new( 3, 3 );

  xfilter[0][0] = 1; 
  xfilter[0][1] = 0; 
  xfilter[0][2] = -1; 
  
  xfilter[1][0] = 2; 
  xfilter[1][1] = 0; 
  xfilter[1][2] = -2; 
  
  xfilter[2][0] = 1; 
  xfilter[2][1] = 0; 
  xfilter[2][2] = -1; 
  
  yfilter[0][0] = 1; 
  yfilter[0][1] = 2; 
  yfilter[0][2] = 1; 
  
  yfilter[1][0] = 0; 
  yfilter[1][1] = 0; 
  yfilter[1][2] = 0; 
  
  yfilter[2][0] = -1; 
  yfilter[2][1] = -2; 
  yfilter[2][2] = -1; 
  
  sx = mat_filter_fir( x, xfilter, 2, 2 );
  sy = mat_filter_fir( x, yfilter, 2, 2 );

  s = mat_new( mat_height(x), mat_width(x) );

  for ( i= 0; i< mat_height(s); i++ )
    for ( j= 0; j< mat_width(s); j++ )
      {
	s[i][j] = sqrt(sx[i][j]*sx[i][j]+sy[i][j]*sy[i][j]);
      }

  mat_delete( sx );
  mat_delete( sy );
  mat_delete( xfilter );
  mat_delete( yfilter );
  
  mat_pgm_write( "sobel.pgm", s );
  
  return s; 
}


mat    mat_mask_custom( mat x )
{
  mat mask = NULL; 
  mat temp1 = NULL;

  temp1 = mat_sobel( x );

  mask = mat_localmeanestimate( temp1, 7 );
  
  mat_delete( temp1 );

  mat_pgm_write( "mask.pgm", mask );
  
  return mask; 
}
