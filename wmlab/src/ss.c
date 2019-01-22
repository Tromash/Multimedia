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
  vec yv;
  mat tile = mat_new_zeros( sz, sz );

  int i, j;
  for ( i = 0 ; i < mat_height( y ) ; i+= sz ) {
    for ( j = 0 ; j < mat_width( y ) ; j+= sz ) {
      int ii, jj;
      for ( ii = 0 ; ii < sz && i+ii < mat_height(y) ; ii++ ) {
        for ( jj = 0 ; jj < sz && j+jj < mat_width(y) ; jj++ ) {
          tile[ii][jj] += y[i+ii][j+jj];
        }
      }
    }
  }

  yv = mat_to_vec( tile );

  for ( i = 0 ; i < mat_width( U ) ; i++ ) {
    vec ui = mat_get_col( U, i );
    corrs[ i ] = vec_inner_product( ui, yv );
    if ( corrs[ i ] < 0 ) {
      mest[ i ] = 1;
    }
    else {
        mest[ i ] = 0;
    }
    vec_delete( ui );
  }

  vec_delete( yv );
  mat_delete( tile );
  mat_delete( U );

  return corrs;
}

vec get_correlations_subimage( mat y, uint key, uint sz, uint Nc, bvec mest, uint ups )
{

  uint Nv = sz*sz;
  mat U = make_carriers( Nc,Nv,key, ups );
  vec corrs = vec_new_zeros( Nc );
  vec AbsCorrs = vec_new_zeros( Nc );
  int AbsSumCorrs;
  int AbsLoop;
  vec yv;
  mat tile = mat_new_zeros( sz, sz );

  vec maxcorrs = vec_new_zeros( Nc );
  int maxcorr;
  int maxline = 0;
  int maxcol = 0;

  // Itere sur les decalages
  int line, col;
  for (line = 0; line < sz; line++) {
    for (col = 0; col < sz; col++) {
      // Calcule les Correlations
      int i, j;
      for ( i = 0 ; i < mat_height( y ) ; i+= sz ) {
        for ( j = 0 ; j < mat_width( y ) ; j+= sz ) {
          int ii, jj;
          for ( ii = 0 ; ii < sz && i+ii < mat_height(y) ; ii++ ) {
            for ( jj = 0 ; jj < sz && j+jj < mat_width(y) ; jj++ ) {
              tile[ii][jj] += y[i+ii][j+jj];
            }
          }
        }
      }

      yv = mat_to_vec( tile );

      AbsSumCorrs = 0;
      for ( i = 0 ; i < mat_width( U ) ; i++ ) {
        vec ui = mat_get_col( U, i );
        corrs[ i ] = vec_inner_product( ui, yv );
        AbsCorrs[i] = abs(corrs[i]);
        AbsSumCorrs += AbsCorrs[i];
      }

      if (AbsSumCorrs > maxcorr) {
        maxcorrs = corrs;
        for ( i = 0 ; i < mat_width( U ) ; i++ ) {
          maxcorrs[i] = corrs[i];
        }
        maxcorr = AbsSumCorrs;
        maxline = line;
        maxcol = col;
      }
    }
  }



  int i;
  for ( i = 0 ; i < mat_width( U ) ; i++ ) {
    if ( maxcorrs[ i ] < 0 ) {
      mest[ i ] = 1;
    }
    else {
        mest[ i ] = 0;
    }
  }
  vec_delete( yv );
  mat_delete( tile );
  mat_delete( U );

  return maxcorrs;
}
