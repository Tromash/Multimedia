/*
   libit - Library for basic source and channel coding functions
   Copyright (C) 2005-2005 Vivien Chappelier, Herve Jegou

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
  Interleavers 
  Copyright (C) 2005 Herve Jegou, Vivien Chappelier
*/

#include <it/interleaver.h>


Vec Vec_new_interleave_block (Vec v, size_t width)
{
  size_t c, j, b = 0;
  size_t el_size = Vec_element_size (v);
  Vec  w = __Vec_new (el_size, Vec_length (v));

  for (c = 0; c < width; c++)
    for (j = c; j < Vec_length (v); j += width) {
      memcpy ((byte *) w + b * el_size, (byte *) v + j * el_size, el_size);
      b++;
    }

  return w;
}


Vec Vec_new_deinterleave_block (Vec v, size_t width)
{
  size_t c, j, b = 0;
  size_t el_size = Vec_element_size (v);
  Vec  w = __Vec_new (el_size, Vec_length (v));

  for (c = 0; c < width; c++)
    for (j = c; j < Vec_length (v); j += width) {
      memcpy ((byte *) w + j * el_size, (byte *) v + b * el_size, el_size);
      b++;
    }

  return w;
}


vec vec_new_interleave_block (vec v, size_t width)
{
  size_t c, j, b = 0;
  vec  w = vec_new (vec_length (v));

  for (c = 0; c < width; c++)
    for (j = c; j < vec_length (v); j += width)
      w[b++] = v[j];

  return w;
}


vec vec_new_deinterleave_block (vec w, size_t width)
{
  size_t c, j, b = 0;
  vec  v = vec_new (vec_length (w));

  for (c = 0; c < width; c++)
    for (j = c; j < vec_length (w); j += width)
      v[j] = w[b++];

  return v;
}


ivec ivec_new_interleave_block (ivec v, size_t width)
{
  size_t c, j, b = 0;
  ivec w = ivec_new (ivec_length (v));

  for (c = 0; c < width; c++)
    for (j = c; j < ivec_length (v); j += width)
      w[b++] = v[j];

  return w;
}


ivec ivec_new_deinterleave_block (ivec w, size_t width)
{
  size_t c, j, b = 0;
  ivec v = ivec_new (ivec_length (w));

  for (c = 0; c < width; c++)
    for (j = c; j < ivec_length (w); j += width)
      v[j] = w[b++];

  return v;
}


bvec bvec_new_interleave_block (bvec v, size_t width)
{
  size_t c, j, b = 0;
  bvec w = bvec_new (bvec_length (v));

  for (c = 0; c < width; c++)
    for (j = c; j < bvec_length (v); j += width)
      w[b++] = v[j];

  return w;
}


bvec bvec_new_deinterleave_block (bvec w, size_t width)
{
  size_t c, j, b = 0;
  bvec v = bvec_new (bvec_length (w));

  for (c = 0; c < width; c++)
    for (j = c; j < bvec_length (w); j += width)
      v[j] = w[b++];

  return v;
}


Vec __Vec_interleave_block (Vec v, size_t width)
{
  Vec  w = Vec_new_interleave_block (v, width);
  Vec_delete (v);
  return w;
}


vec __vec_interleave_block (vec v, size_t width)
{
  vec  w = vec_new_interleave_block (v, width);
  vec_delete (v);
  return w;
}


ivec __ivec_interleave_block (ivec v, size_t width)
{
  ivec w = ivec_new_interleave_block (v, width);
  ivec_delete (v);
  return w;
}


bvec __bvec_interleave_block (bvec v, size_t width)
{
  bvec w = bvec_new_interleave_block (v, width);
  bvec_delete (v);
  return w;
}


Vec __Vec_deinterleave_block (Vec v, size_t width)
{
  Vec  w = Vec_new_deinterleave_block (v, width);
  Vec_delete (v);
  return w;
}


vec __vec_deinterleave_block (vec v, size_t width)
{
  vec  w = vec_new_deinterleave_block (v, width);
  vec_delete (v);
  return w;
}


ivec __ivec_deinterleave_block (ivec v, size_t width)
{
  ivec w = ivec_new_deinterleave_block (v, width);
  ivec_delete (v);
  return w;
}


bvec __bvec_deinterleave_block (bvec v, size_t width)
{
  bvec w = bvec_new_deinterleave_block (v, width);
  bvec_delete (v);
  return w;
}


/*------------------------------------------------------------------*/
/* Matrix interleaver (compatible with the block vector interleaver)*/

Mat __Mat_new_interleave_block (Mat m, size_t width)
{
  size_t c, j, b = 0;
  size_t el_size = Mat_element_size (m);
  Mat  w = __Mat_new (el_size, Mat_height (m), Mat_width (m));

  for (c = 0; c < width; c++)
    for (j = c; j < Mat_height (m); j += width)
      Vec_copy (w[b++], m[j]);

  return w;
}


Mat __Mat_new_deinterleave_block (Mat m, size_t width)
{
  size_t c, j, b = 0;
  size_t el_size = Mat_element_size (m);
  Mat  w = __Mat_new (el_size, Mat_height (m), Mat_width (m));

  for (c = 0; c < width; c++)
    for (j = c; j < Mat_height (m); j += width)
      Vec_copy (w[j], m[b++]);

  return w;
}


mat mat_new_interleave_block (mat m, size_t width)
{
  return ((mat) Mat_new_interleave_block ((Mat) m, width));
}


mat mat_new_deinterleave_block (mat w, size_t width)
{
  return ((mat) Mat_new_deinterleave_block ((Mat) w, width));
}


imat imat_new_interleave_block (imat m, size_t width)
{
  return ((imat) Mat_new_interleave_block ((Mat) m, width));
}


imat imat_new_deinterleave_block (imat w, size_t width)
{
  return ((imat) Mat_new_deinterleave_block ((Mat) w, width));
}


bmat bmat_new_interleave_block (bmat m, size_t width)
{
  return ((bmat) Mat_new_interleave_block ((Mat) m, width));
}


bmat bmat_new_deinterleave_block (bmat w, size_t width)
{
  return ((bmat) Mat_new_deinterleave_block ((Mat) w, width));
}


Mat __Mat_interleave_block (Mat m, size_t width)
{
  Mat  w = Mat_new_interleave_block (m, width);
  Mat_copy (m, w);
  Mat_delete (w);
  return m;
}


mat mat_interleave_block (mat m, size_t width)
{
  mat  w = mat_new_interleave_block (m, width);
  mat_copy (m, w);
  mat_delete (w);
  return m;
}


imat imat_interleave_block (imat m, size_t width)
{
  imat w = imat_new_interleave_block (m, width);
  imat_copy (m, w);
  imat_delete (w);
  return m;
}


bmat bmat_interleave_block (bmat m, size_t width)
{
  bmat w = bmat_new_interleave_block (m, width);
  bmat_copy (m, w);
  bmat_delete (w);
  return m;
}


Mat __Mat_deinterleave_block (Mat m, size_t width)
{
  Mat  w = Mat_new_deinterleave_block (m, width);
  Mat_copy (m, w);
  Mat_delete (w);
  return m;
}


mat mat_deinterleave_block (mat m, size_t width)
{
  mat  w = mat_new_deinterleave_block (m, width);
  mat_copy (m, w);
  mat_delete (w);
  return m;
}


imat imat_deinterleave_block (imat m, size_t width)
{
  imat w = imat_new_deinterleave_block (m, width);
  imat_copy (m, w);
  imat_delete (w);
  return m;
}


bmat bmat_deinterleave_block (bmat m, size_t width)
{
  bmat w = bmat_new_deinterleave_block (m, width);
  bmat_copy (m, w);
  bmat_delete (w);
  return m;
}
