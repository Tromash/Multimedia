#include "it/mat.h"
#include "it/vec.h"
#include "it/parser.h"
#include "it/io.h"
#include "it/random.h"
#include "it/distance.h"

#include "utils.h"
#include "estimation.h"


int main(int argc, char **argv)
{
  mat
    e  = NULL, /* Watermark estimate */
    y  = NULL; /* Host image */

  parser_t * parser = NULL;

  char * imgin; 
  uint key, sz, ups, verbose; 
  bvec m, mest; 
  vec corrs; 
  
  if ( argc == 1 )
    {
      fprintf( stderr, "Usage: %s config.cfg\n", argv[0] );
      exit( EXIT_FAILURE );
    }
  
  parser=parser_init( argc, argv, argv[1], NULL );

  key = parser_get_int( parser, "KEY" );

  imgin = parser_get_string( parser, "IN" );
  sz = parser_get_int( parser, "SZ" );
  ups = parser_get_int( parser, "UPSAMPLING" );
  verbose = parser_get_int( parser, "VERBOSE" );
  m = parser_get_bvec( parser, "MESSAGE" );

  mest = bvec_new_zeros( bvec_length(m) );

  y = mat_pgm_read( imgin );

  if ( verbose )
    fprintf( MSGOUT, "Read %dx%d cover image %s\n", mat_height(y), mat_width(y), imgin );

  /* Key initialisation */
  if(key == 0) it_randomize();
  else it_seed(key);

  e = mat_estimate( y );

  corrs = get_correlations( e, key, sz, bvec_length(m), mest, ups ); 

  it_printf( "BER=%f\nCorrelations=$v\n", bvec_distance_hamming( m, mest )/(double)bvec_length(m), corrs );

  vec_delete( corrs );

  mat_delete( y  );
  mat_delete( e  );
  
  free( imgin );

  return(EXIT_SUCCESS);

}

