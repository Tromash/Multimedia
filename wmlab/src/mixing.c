#include <it/io.h>
#include <it/mat.h>

#include "modulation.h"
#include "masking.h"
#include "utils.h"

mat mix_signals( mat x, mat w, double psnr, int psycho )
{
  uint s = 3;
  uint i, j;
  double eqm=0., gamma=0.; 
  
  mat y = mat_clone( x );

  mat lm = NULL, lv = NULL;

  switch( psycho )
    {
    case 0 :
      lv = mat_new( mat_height(x), mat_width(x) );
      
      mat_ones( lv );
      break;
    case 1 :
      lv = mat_localvarestimate( x, s );
      break; 
    case 2 :
      lv = mat_sobel( x );
      break;
    case 3 :
      lv = mat_mask_custom( x );
      break;
    }
  
  for ( i= 0; i< mat_height( w ); i++ )
    for ( j= 0; j< mat_width( w ); j++ )
      eqm+= (w[i][j]*lv[i][j])*(w[i][j]*lv[i][j]);

  eqm /= mat_height(x)*mat_width(x);

  lm = mat_new_zeros( mat_height(x), mat_width(x) );
  
  mat_copy( lm, w );
  mat_elem_mul( lm, lv );

  gamma = sqrt( 255*255/( eqm*pow(10.,psnr/10.) ));

  for ( i= 0; i< mat_height( w ); i++ )
    for ( j= 0; j< mat_width( w ); j++ )
      y[i][j] = x[i][j] + gamma*lm[i][j]; 
  

  mat_delete(lm); 
  mat_delete(lv); 

  return( y );

}

mat make_wmk_signal( mat x, bvec m, uint key, uint sz, double psnr, int psycho, uint ups )
{

  uint Nv = sz*sz; 
  uint Nc = bvec_length(m); 
  mat w = mat_new( mat_height(x), mat_width(x) );
  mat y; 
  mat tilex, tilew; 
  mat U = make_carriers( Nc, Nv, key, ups ); 
  vec w1d = vec_new_zeros( Nv );
  uint ti, tj; 
  vec vx; 
  mat output;
  
  for ( ti=0; ti< mat_height(x); ti+= sz )
    for ( tj=0; tj< mat_width(x); tj+= sz )
      {
	tilex = mat_get_submatrix( x, ti, tj, ti+sz-1, tj+sz-1 );
	vx = mat_to_vec( tilex ); 

	w1d = encode_ss( vx,m,U,0. );

	vec_sub( w1d, vx );
	vec_delete( vx );

	tilew = vec_to_mat( w1d, sz );
	vec_delete( w1d ); 

	mat_set_submatrix( w, tilew, ti, tj );

	mat_delete( tilex );
	mat_delete( tilew );
      }

  y = mix_signals( x, w, psnr, psycho ); 

  output = mat_scale( w );

  mat_pgm_write( "original.pgm", output );

  mat_delete( output );
  
  mat_delete( U );
  mat_delete( w );
  
  return y; 
}

