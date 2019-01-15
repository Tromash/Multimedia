#include <it/mat.h>

vec encode_ss( vec x, bvec m, mat u, double wcr )
{
  vec w = vec_clone(x);
  int i;
  for (i=0; i<mat_width(u); i++) {
    vec ui = mat_get_get(u,i);
    if ( m[i]==0){
      vec_add(w, ui);
    }
    else{
      vec_sub(w, ui);
    }
    vec_delete(ui);
  }

  return( w );

}
