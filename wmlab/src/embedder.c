#include <it/mat.h>
#include <it/vec.h>
#include <it/parser.h>
#include <it/io.h>
#include <it/random.h>

#include "utils.h"
#include "mixing.h"
#include "constants.h"


int main(int argc, char **argv)
{
  mat
    x  = NULL, /* Host image */
    y  = NULL; /* Watermarked image */

  parser_t * parser = NULL;

  char * imgin, * imgout; 
  uint key, sz, ups;
  double psnr; 
  bvec m;
  int psycho = 0;
  uint verbose = 0; 

  if ( argc == 1 )
    {
      fprintf( stderr, "Usage: %s config.cfg\n", argv[0] );
      exit( EXIT_FAILURE );
    }
  
  parser=parser_init( argc, argv, argv[1], NULL );

  key = parser_get_int( parser, "KEY" );

  imgin = parser_get_string( parser, "IN" );
  imgout = parser_get_string( parser, "OUT" );
  m = parser_get_bvec( parser, "MESSAGE" );
  sz = parser_get_int( parser, "SZ" );
  psnr = parser_get_double( parser, "PSNR" );
  psycho = parser_get_int( parser, "PSYCHO" );
  ups = parser_get_int( parser, "UPSAMPLING" );
  verbose = parser_get_int( parser, "VERBOSE" );
  
  x = mat_pgm_read( imgin );

  if ( verbose )
    fprintf( MSGOUT, "Read %dx%d cover image %s\n", mat_height(x), mat_width(x), imgin );

  /* Key initialisation */
  if(key == 0) it_randomize();
  else it_seed(key);

  /* Watermark generation */
  y = make_wmk_signal( x, m, key, sz, psnr, psycho, ups ); 

  /* Write marked image */
  mat_pgm_write( imgout, y ); 

  /* Print PSNR */
  if ( verbose )
    it_printf( "PSNR=%2.2fdB\n", mat_psnr( x, y ) );

  mat_elem_sub( y, x );

  mat_delete( x );

  x = mat_scale( y );
  
  mat_pgm_write( "diff.pgm", x );
  
  bvec_delete( m );
  
  mat_delete( y  );

  mat_delete( x  );
  
  free( imgin );
  free( imgout );

  return(EXIT_SUCCESS);

}

