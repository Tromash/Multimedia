#include <it/mat.h>
#include <it/fourier.h>

cmat mat_fft2d( mat m ) 
{

  uint i = 0, 
       j = 0; 
  cmat ft = NULL; 
  cvec c  = NULL, 
       ct = NULL; 
  vec  v  = NULL; 

  // Perform forward Fourier transform
  ft = cmat_new_zeros( mat_height(m), mat_width(m) );   

  // First: FT on rows
  for ( i= 0; i< mat_height( m ); i++ ) 
    {
      v = mat_get_row( m, i ); 
      c = vec_to_cvec( v ); 
      vec_delete( v ); 

      ct = it_dft( c );

      cmat_set_row( ft, i, ct );

      cvec_delete( ct ); 
      cvec_delete( c ); 
    }

  // Next: on columns
  for ( j= 0; j< mat_width( m ); j++ ) 
    {
      c = cmat_get_col( ft, j ); 
      ct = it_dft( c ); 
      cvec_delete( c ); 

      cmat_set_col( ft, j, ct );

      cvec_delete( ct );

    }

  return( ft ); 

}

cmat cmat_ifft2d( cmat ft ) 
{

  uint i  = 0, 
       j  = 0; 
  cvec c  = NULL, 
       ct = NULL; 

  // Perform Inverse Fourier transform: columns first
  for ( j= 0; j< cmat_width(ft); j++ )
    {
      c = cmat_get_col( ft, j ); 
      ct = it_idft( c ); 
      cvec_delete( c ); 

      cmat_set_col( ft, j, ct ); 
      cvec_delete( ct );
    }

  // Rows next
  for ( i= 0; i< cmat_height(ft); i++ ) 
    {
      c = cmat_get_row( ft, i ); 
      ct = it_idft( c ); 
      cvec_delete( c ); 

      cmat_set_row( ft, i, ct ); 
      cvec_delete( ct );
    }

  return( ft ); 

}

mat mat_ms2d( mat m ) 
{

  uint i  = 0., 
       j  = 0.; 
  cmat ft = NULL; 
  mat  ms = NULL; 

  double scale = 0., 
         t     = 0.; 

  scale = 1./((double)mat_height(m)*mat_width(m)); 

  ft = mat_fft2d( m ); 

  ms = mat_new_zeros( mat_height(m), mat_width(m) );

  // Compute magnitude spectrum
  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m); j++ ) 
      ms[i][j] = scale*((creal(ft[i][j])*creal(ft[i][j]))+(cimag(ft[i][j])*cimag(ft[i][j]))); 

  for ( i= 0; i< mat_height(m)/2; i++ )
    for ( j= 0; j< mat_width(m); j++ )
      {
	t = ms[mat_height(m)/2+i][j]; 
	ms[mat_height(m)/2+i][j] = ms[i][j]; 
	ms[i][j] = t; 
      }

  for ( i= 0; i< mat_height(m); i++ )
    for ( j= 0; j< mat_width(m)/2; j++ )
      {
	t = ms[i][mat_width(m)/2+j]; 
	ms[i][mat_height(m)/2+j] = ms[i][j]; 
	ms[i][j] = t; 
      }

  cmat_delete( ft );

  return( ms );

}

mat mat_acf2d( mat m ) 
{

  mat  acf  = NULL; 
  cmat ifft = NULL; 

  uint i    = 0, 
       j    = 0; 

  acf = mat_ms2d( m );

  ifft = cmat_new_zeros( mat_height(m), mat_width(m) ); 

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ ) 
      {
	creal(ifft[i][j]) = acf[i][j]; 
	cimag(ifft[i][j]) = 0.0; 
      }

  cmat_ifft2d( ifft ); 

  for ( i= 0; i< mat_height( m ); i++ )
    for ( j= 0; j< mat_width( m ); j++ ) 
      acf[i][j] = creal( ifft[i][j] ); 

  cmat_delete( ifft );

  return( acf );

}

