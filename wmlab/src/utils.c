#include <it/vec.h>
#include <it/mat.h>
#include <it/io.h>

mat mat_localmeanestimate( mat m, int s ) 
{

  uint i  = 0, 
       j  = 0; 
  int  k  = 0, 
       e  = 0, 
       p  = 0, 
       q  = 0, 
       l  = 0; 
  mat  lm = NULL; 

  e = (int)rint((s-.5)/2.);

  lm = mat_new_zeros( mat_height( m ), mat_width( m ) );

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ )
      {
	for ( k= -e; k< e+1; k++ ) 
	  for ( l= -e; l< e+1; l++ ) 
	    {
	      p = i+k; 
	      if ( p < 0 ) 
		p = -p; 
	      if ( p > mat_height( m ) - 1 ) 
		p = mat_height( m ) - 1 - ( p - mat_height( m ) + 1 ); 

	      q = l+j; 
	      if ( q < 0 ) 
		q = -q; 
	      if ( q > mat_width( m ) - 1 ) 
		q = mat_width( m ) - 1 - ( q - mat_width( m ) + 1 ); 

	      lm[i][j] += m[p][q];
	    }

	lm[i][j] /= (double)((2*e+1)*(2*e+1)); 
      }

  return( lm );

}

mat mat_localvarestimate( mat m, int s ) 
{

  uint i  = 0, 
       j  = 0; 
  int  k  = 0, 
       e  = 0, 
       p  = 0, 
       q  = 0, 
       l  = 0; 
  mat  lv = NULL; 

  mat lm = mat_localmeanestimate( m, s );
  
  e = (uint)rint((s-.5)/2.);

  lv = mat_new_zeros( mat_height( m ), mat_width( m ) );

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ )
      {

	for ( k= -e; k< e+1; k++ ) 
	  for ( l= -e; l< e+1; l++ ) 
	    {
	      p = i+k; 
	      if ( p < 0 ) 
		p = -p; 
	      if ( p > mat_height( m ) - 1 ) 
		p = mat_height( m ) - 1 - ( p - mat_height( m ) + 1 ); 

	      q = l+j; 
	      if ( q < 0 ) 
		q = -q; 
	      if ( q > mat_width( m ) - 1 ) 
		q = mat_width( m ) - 1 - ( q - mat_width( m ) + 1 ); 

	      lv[i][j] += (lm[i][j]-m[p][q])*(lm[i][j]-m[p][q]);
	    }

	lv[i][j] /= (2*e+1)*(2*e+1)-1; 

      }

  mat_delete( lm );
  
  mat_pgm_write( "lvar.pgm", lv );
  
  return( lv );

}

mat mat_scale( mat m )
{
  uint i, j; 
  double min, max; 
  mat output = mat_clone( m );

  min = mat_min( output );
  max = mat_max( output );

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      {
	output[i][j] = 255*(output[i][j]-min)/(max-min);
      }
  
  return output;

}

uint mat_thresh(  mat m, double thr) 
{

  uint count  = 0, 
       i      = 0, 
       j      = 0; 
  mat  output = NULL; 

  output = mat_new_zeros( mat_height( m ), mat_width( m ) ); 

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ ) 
      if ( m[i][j] > thr )
	{
	  count++; 
	  output[i][j] = m[i][j];
	}

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ ) 
      m[i][j] = output[i][j];  

  mat_delete( output );

  return( count ); 

}

double mat_absmean( mat m ) 
{

  uint   i  = 0, 
         j  = 0; 
  double am = 0.; 

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ )
      am+= fabs( m[i][j] );

  return( am/(double)( mat_width(m)*mat_height(m) ) );

}

double vec_absmean( vec v ) 
{

  uint   i  = 0; 
  double am = 0.; 

  double mean = vec_mean( v ); 

  for ( i= 0; i< vec_length( v ); i++ )
    am+= fabs( v[i]-mean );

  return( am/(double)( vec_length( v ) ) ); 

}

double mat_psnr( mat x, mat y )
{

  double eqm = 0.;

  uint i, j;

  for ( i= 0; i< mat_height(x); i++ )
    for ( j= 0; j< mat_width(x); j++ ) eqm+= (x[i][j]-y[i][j])*(x[i][j]-y[i][j]);
  
  eqm /= mat_height(x)*mat_width(x);

  return( 10*log10( 255*255/eqm ) );

}

mat mat_localmax( mat m, uint s, double stdv, uint debugging ) 
{

  mat  peaks     = NULL; 
  uint i         = 0, 
       j         = 0, 
       n         = 0; 
  int  k         = 0, 
       l         = 0, 
       s1        = 0, 
       s2        = 0; 
  vec  v         = NULL; 

  double val; 
  double lmean, lvar; //, lalpha = 2; 
  double gmean, gvar; //, galpha = 3; 

  if ( !(s & 1) ) s++; 
  
  s1 = s; 
  s2 = s; 
  

  if ( debugging ) 
    fprintf( stdout, "Local max filtering with %dx%d window\n", s1, s2 ); 

  peaks = mat_new_zeros( mat_height( m ), mat_width( m ) );

  v = vec_new_zeros( s1*s2 ); 

  gmean = mat_mean( m );
  gvar  = sqrt( mat_variance( m ) ); 

  for ( i= 0; i< mat_height( m ); i+=s1 )
    for ( j= 0; j< mat_width( m ); j+=s2 ) 
      {

	n = 0; 

	for ( k= -s1/2; k< s1/2+1; k++ )
	  for ( l= -s2/2; l< s2/2+1; l++ )
	    v[n++] = m[(i+k+mat_height(m))%mat_height(m)][(j+l+mat_width(m))%mat_width(m)];

	vec_sort( v );

	val = v[ s1*s2-1 ]; 

	lmean = vec_mean( v );

	v[ s1*s2-1 ] = lmean; 

	lvar = sqrt( vec_variance( v ) );

	for ( k= -s1/2; k< s1/2+1; k++ )
	  for ( l= -s2/2; l< s2/2+1; l++ )
	    {
	      if ( m[(i+k+mat_height(m))%mat_height(m)][(j+l+mat_width(m))%mat_width(m)] == val && val > stdv*lvar ) 
		peaks[(i+k+mat_height(m))%mat_height(m)][(j+l+mat_width(m))%mat_width(m)] = m[(i+k+mat_height(m))%mat_height(m)][(j+l+mat_width(m))%mat_width(m)];
	    }

      } 

  mat_pgm_write( "est-peaks.pgm", peaks );

  vec_delete( v );

  return( peaks );

}

