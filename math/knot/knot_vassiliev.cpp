#include <iostream>

#include <math.h>

#include "knot.h"

static double det (const double *v1, const double *v2, const double *v3)
{
  return
      v1 [0] * (v2 [1] * v3 [2] - v2 [2] * v3 [1])
    + v1 [1] * (v2 [2] * v3 [0] - v2 [0] * v3 [2])
    + v1 [2] * (v2 [0] * v3 [1] - v2 [1] * v3 [0]);
}

static double vector_square (const double *v)
{
  return v [0] * v [0] + v [1] * v [1] + v [2] * v [2];
}

static double scalar_product (const double *v1, const double *v2)
{
  return v1 [0] * v2 [0] + v1 [1] * v2 [1] + v1 [2] * v2 [2];
}

static void vector_product (const double *v1, const double *v2, double *m)
{
  m [0] = v1 [1] * v2 [2] - v1 [2] * v2 [1];
  m [1] = v1 [2] * v2 [0] - v1 [0] * v2 [2];
  m [2] = v1 [0] * v2 [1] - v1 [1] * v2 [0];
}

#define			length			Parent -> length
#define			points			Parent -> points
#define			next			Parent -> next
#define			prev			Parent -> prev

void knot::prmAcn::compute (void)
{
  internalValue = 0.0;

  int i1, i2;

  // Вычисляем заранее касательные векторы.
  double **tangs = new double* [length];
  for (i1 = 0; i1 < length; i1++)
  {
    tangs [i1] = new double [3];
    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
  }

  double chord [3], chord_len;
  for (i1 = 0; i1 < length; i1++)
  {
    for (i2 = 0; i2 < i1; i2++)
    {
      chord [0] = ( points [i1] [0] + points [next (i1)] [0] -
                    points [i2] [0] - points [next (i2)] [0] ) / 2;
      chord [1] = ( points [i1] [1] + points [next (i1)] [1] -
                    points [i2] [1] - points [next (i2)] [1] ) / 2;
      chord [2] = ( points [i1] [2] + points [next (i1)] [2] -
                    points [i2] [2] - points [next (i2)] [2] ) / 2;
      chord_len = sqrt (vector_square (chord));
      internalValue += fabs (det (tangs [i1], tangs [i2], chord)) /
                       (chord_len * chord_len * chord_len);
    }
  }

  internalValue /= 2 * M_PI;

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
    delete[] tangs [i1];
  delete[] tangs;
}

void knot::prmSAcn::compute (void)
{
  internalValue = 0.0;

  int i1, i2;

  // Вычисляем заранее касательные векторы.
  double **tangs = new double* [length];
  for (i1 = 0; i1 < length; i1++)
  {
    tangs [i1] = new double [3];
    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
  }

  double chord [3], chord_len;
  for (i1 = 0; i1 < length; i1++)
  {
    for (i2 = 0; i2 < i1; i2++)
    {
      chord [0] = ( points [i1] [0] + points [next (i1)] [0] -
                    points [i2] [0] - points [next (i2)] [0] ) / 2;
      chord [1] = ( points [i1] [1] + points [next (i1)] [1] -
                    points [i2] [1] - points [next (i2)] [1] ) / 2;
      chord [2] = ( points [i1] [2] + points [next (i1)] [2] -
                    points [i2] [2] - points [next (i2)] [2] ) / 2;
      chord_len = sqrt (vector_square (chord));
      internalValue += det (tangs [i1], tangs [i2], chord) /
                       (chord_len * chord_len * chord_len);
    }
  }

  internalValue /= 2 * M_PI;

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
    delete[] tangs [i1];
  delete[] tangs;
}

void knot::prmKI::compute (void)
{
  internalValue = 0.0;

  int i1, i2, o;

  // Вычисляем заранее касательные векторы.
  double **tangs = new double* [length];
  for (i1 = 0; i1 < length; i1++)
  {
    tangs [i1] = new double [3];
    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
  }

  // Вычисляем ``гауссовы произведения''.
  double chord [3], chord_len;
  double **gauss = new double* [length];

  for (i1 = 0; i1 < length; i1++)
  {
    gauss [i1] = new double [length];
    gauss [i1] [i1] = 0.0;
    for (i2 = 0; i2 < i1; i2++)
    {
      chord [0] = ( points [i1] [0] + points [next (i1)] [0] -
                    points [i2] [0] - points [next (i2)] [0] ) / 2;
      chord [1] = ( points [i1] [1] + points [next (i1)] [1] -
                    points [i2] [1] - points [next (i2)] [1] ) / 2;
      chord [2] = ( points [i1] [2] + points [next (i1)] [2] -
                    points [i2] [2] - points [next (i2)] [2] ) / 2;
      chord_len = sqrt (vector_square (chord));
      gauss [i1] [i2] = det (tangs [i1], tangs [i2], chord) /
                        (chord_len * chord_len * chord_len);
      gauss [i2] [i1] = gauss [i1] [i2];
    }
  }

  // Вычисляем суммы ``гауссовых произведений''.

  // В gauss_sum [i1] [i2] находится сумма ``гауссовых произведений''
  // для всех хорд с началом в i1 и концом от next (i1) до i2.
  double **gauss_sum = new double* [length];

  for (i1 = 0; i1 < length; i1++)
  {
    gauss_sum [i1] = new double [length];
    gauss_sum [i1] [i1] = 0.0;
    for (i2 = next (i1); i2 != i1; i2 = next (i2))
      gauss_sum [i1] [i2] = gauss_sum [i1] [prev (i2)] + gauss [i1] [i2];
  }

  double tmp, tmp2;
  for (i1 = 0; i1 < length; i1++)
  {
    tmp = 0.0;
    for (i2 = next (next (i1)); i2 != i1; i2 = next (i2))
    {
      tmp += gauss_sum [prev (i2)] [prev (i1)] -
             gauss_sum [i2] [prev (prev (i2))] + gauss_sum [i2] [i1];
      tmp2 = gauss [i1] [i2];
      for (o = 1; o < order; o++)
        tmp2 *= tmp / 16;
      internalValue += tmp2;
    }
  }

  internalValue /= 4 * M_PI * M_PI;

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
  {
    delete[] tangs [i1];
    delete[] gauss [i1];
    delete[] gauss_sum [i1];
  }
  delete[] tangs;
  delete[] gauss;
  delete[] gauss_sum;
}

//void knot::prmExperimental::compute (void)
//{
//  internalValue = 0.0;
//
//  int i1, i2, i3, i4;
//
//  // Вычисляем заранее касательные векторы.
//  double **tangs = new double* [length];
//  for (i1 = 0; i1 < length; i1++)
//  {
//    tangs [i1] = new double [3];
//    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
//    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
//    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
//  }
//
//  // Вычисляем ``гауссовы произведения''.
//  double chord_len;
//  double ***chord = new double** [length];
//  double ***chord2 = new double** [length];
//  double tmp [3], tmp2 [3];
//
//  for (i1 = 0; i1 < length; i1++)
//  {
//    chord [i1] = new double* [length];
//    chord2 [i1] = new double* [length];
//    for (i2 = 0; i2 < length; i2++)
//    {
//      chord [i1] [i2] = new double [3];
//      chord2 [i1] [i2] = new double [3];
//    }
//
//    for (i2 = 0; i2 < i1; i2++)
//    {
//      chord [i1] [i2] [0] = ( points [i1] [0] + points [next (i1)] [0] -
//                    points [i2] [0] - points [next (i2)] [0] ) / 2;
//      chord [i1] [i2] [1] = ( points [i1] [1] + points [next (i1)] [1] -
//                    points [i2] [1] - points [next (i2)] [1] ) / 2;
//      chord [i1] [i2] [2] = ( points [i1] [2] + points [next (i1)] [2] -
//                    points [i2] [2] - points [next (i2)] [2] ) / 2;
//      chord [i2] [i1] [0] = - chord [i1] [i2] [0];
//      chord [i2] [i1] [1] = - chord [i1] [i2] [1];
//      chord [i2] [i1] [2] = - chord [i1] [i2] [2];
//      chord_len = sqrt (vector_square (chord [i1] [i2]));
//      chord_len = 1.0 / (chord_len * chord_len * chord_len);
//      vector_product (tangs [i1], tangs [i2], tmp);
//      vector_product (tmp, chord [i1] [i2], tmp2);
//      vector_product (tmp2, chord [i1] [i2], chord2 [i1] [i2]);
//      chord2 [i1] [i2] [0] *= chord_len;
//      chord2 [i1] [i2] [1] *= chord_len;
//      chord2 [i1] [i2] [2] *= chord_len;
//      chord2 [i2] [i1] [0] = - chord2 [i1] [i2] [0];
//      chord2 [i2] [i1] [1] = - chord2 [i1] [i2] [1];
//      chord2 [i2] [i1] [2] = - chord2 [i1] [i2] [2];
//    }
//  }
//
//  double normal [3];
//
//  for (i1 = 0; i1 < length; i1++)
//    for (i2 = i1 + 1; i2 < length; i2++)
//      for (i3 = i2 + 1; i3 < length; i3++)
//        for (i4 = i3 + 1; i4 < length; i4++)
//	{
//          vector_product (chord [i1] [i3], chord [i2] [i4], normal);
//
//          internalValue +=
//	    scalar_product (chord2 [i1] [i3], chord [i2] [i4]) *
//	    scalar_product (chord2 [i2] [i4], chord [i1] [i3]) /
//	    vector_square (normal);
//        }
//
//  internalValue /= 4 * M_PI * M_PI;
//
//  // Удаляем заранее вычисленные вспомогательные значения.
//  for (i1 = 0; i1 < length; i1++)
//  {
//    for (i2 = 0; i2 < length; i2++)
//    {
//      delete[] chord [i1] [i2];
//      delete[] chord2 [i1] [i2];
//    }
//
//    delete[] tangs [i1];
//    delete[] chord [i1];
//    delete[] chord2 [i1];
//  }
//  delete[] tangs;
//  delete[] chord;
//  delete[] chord2;
//}

void knot::prmExperimental::compute (void)
{
  internalValue = 0.0;

  int i1, i2;

  // Вычисляем заранее касательные векторы.
  double **tangs = new double* [length];
  for (i1 = 0; i1 < length; i1++)
  {
    tangs [i1] = new double [3];
    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
  }

  // Вычисляем ``гауссовы произведения''.
  double chord_len;
  double ***chord = new double** [length];
  double ***vector = new double** [length];
  double **sum = new double* [length];

  for (i1 = 0; i1 < length; i1++)
  {
    chord [i1] = new double* [length];
    vector [i1] = new double* [length];
    sum [i1] = new double [length];
    for (i2 = 0; i2 < length; i2++)
    {
      chord [i1] [i2] = new double [3];
      vector [i1] [i2] = new double [3];
    }

    for (i2 = 0; i2 < i1; i2++)
    {
      chord [i1] [i2] [0] = ( points [i1] [0] + points [next (i1)] [0] -
                    points [i2] [0] - points [next (i2)] [0] ) / 2;
      chord [i1] [i2] [1] = ( points [i1] [1] + points [next (i1)] [1] -
                    points [i2] [1] - points [next (i2)] [1] ) / 2;
      chord [i1] [i2] [2] = ( points [i1] [2] + points [next (i1)] [2] -
                    points [i2] [2] - points [next (i2)] [2] ) / 2;
      chord_len = sqrt (vector_square (chord [i1] [i2]));
      chord [i1] [i2] [0] /= chord_len;
      chord [i1] [i2] [1] /= chord_len;
      chord [i1] [i2] [2] /= chord_len;
      chord [i2] [i1] [0] = - chord [i1] [i2] [0];
      chord [i2] [i1] [1] = - chord [i1] [i2] [1];
      chord [i2] [i1] [2] = - chord [i1] [i2] [2];
      vector_product (tangs [i1], tangs [i2], vector [i1] [i2]);
      chord_len *= chord_len;
      vector [i1] [i2] [0] /= chord_len;
      vector [i1] [i2] [1] /= chord_len;
      vector [i1] [i2] [2] /= chord_len;
      vector [i2] [i1] [0] = - vector [i1] [i2] [0];
      vector [i2] [i1] [1] = - vector [i1] [i2] [1];
      vector [i2] [i1] [2] = - vector [i1] [i2] [2];
    }
  }

  int i, j;
  double tmp;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      for (i1 = 0; i1 < length; i1++)
      {
        sum [i1] [i1] = 0.0;
        for (i2 = next (i1); i2 != i1; i2 = next (i2))
        sum [i1] [i2] = sum [i1] [prev (i2)] +
	                chord [i1] [i2] [i] * vector [i1] [i2] [j];
      }

      for (i1 = 0; i1 < length; i1++)
      {
        tmp = 0.0;

        for (i2 = next (next (i1)); i2 != i1; i2 = next (i2))
        {
          tmp += sum [prev (i2)] [prev (i1)] -
	         sum [prev (i2)] [i2] -
                 sum [i2] [prev (prev (i2))] +
	         sum [i2] [i1];
          internalValue += tmp *
                           vector [i2] [i1] [j] *
        	           chord [i2] [i1] [i];
        }
      }
    }

  internalValue /= 64 * M_PI;

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
  {
    for (i2 = 0; i2 < length; i2++)
    {
      delete[] chord [i1] [i2];
      delete[] vector [i1] [i2];
    }

    delete[] tangs [i1];
    delete[] chord [i1];
    delete[] vector [i1];
    delete[] sum [i1];
  }
  delete[] tangs;
  delete[] chord;
  delete[] vector;
  delete[] sum;
}

static int inside (const double *a,
                   const double *b,
		   const double *c,
		   const double *d,
		   const double *x)
{
  int sign = (det (a, b, x) > 0);
  if ((det (b, c, x) > 0) != sign)
    return 0;
  if ((det (c, d, x) > 0) != sign)
    return 0;
  if ((det (d, a, x) > 0) != sign)
    return 0;
  return 1;
}

static int intersected (const double *a,
                        const double *b,
			const double *c,
			const double *d)
{
  if (det (a, b, c) * det (a, b, d) > 0)
    return 0;
  if (det (a, c, d) * det (b, c, d) > 0)
    return 0;
  return 1;
}

void knot::prmSingular::compute (void)
{
  internalValue = 0.0;

  int i1, i2, i3, i4;

  // Вычисляем заранее хорды.
  double chord_len;
  double ***chord = new double** [length];

  for (i1 = 0; i1 < length; i1++)
  {
    chord [i1] = new double* [length];
    for (i2 = 0; i2 < length; i2++)
      chord [i1] [i2] = new double [3];

    for (i2 = 0; i2 < i1; i2++)
    {
      chord [i1] [i2] [0] = points [i1] [0] - points [i2] [0];
      chord [i1] [i2] [1] = points [i1] [1] - points [i2] [1];
      chord [i1] [i2] [2] = points [i1] [2] - points [i2] [2];
      chord_len = sqrt (vector_square (chord [i1] [i2]));
      chord [i1] [i2] [0] /= chord_len;
      chord [i1] [i2] [1] /= chord_len;
      chord [i1] [i2] [2] /= chord_len;
      chord [i2] [i1] [0] = - chord [i1] [i2] [0];
      chord [i2] [i1] [1] = - chord [i1] [i2] [1];
      chord [i2] [i1] [2] = - chord [i1] [i2] [2];
    }
  }

  double curr_cos, min_cos = 1.0;
  for (i1 = 0; i1 < length; i1++)
    for (i2 = next (i1); i2 != prev (i1); i2 = next (i2))
    {
      curr_cos = fabs (scalar_product (chord [i1] [i2], chord [i1] [next (i2)]));
      if (min_cos > curr_cos)
        min_cos = curr_cos;
    }
  min_cos = 4 * min_cos - 3;
	std::cerr << min_cos << "\n";

  for (i1 = 0; i1 < length - 3; i1++)
    for (i2 = i1 + 1; i2 < length - 2; i2++)
      for (i3 = i2 + 1; i3 < length - 1; i3++)
        for (i4 = i3 + 1; i4 < length; i4++)
	{
	  if (fabs (scalar_product (chord [i1] [i3], chord [i2] [i4])) < min_cos)
	    continue;
//          internalValue += 1;
//	  if (intersected (chord [i1] [i3],
//	                   chord [i1] [next (i3)],
//	                   chord [i2] [i4],
//			   chord [i2] [next (i4)])
//            internalValue += 1;
	  if (inside (chord [i1] [i3],
	              chord [i1] [next (i3)],
	              chord [next (i1)] [next (i3)],
		      chord [next (i1)] [i3],
	              chord [i2] [i4]))
            internalValue += 1;
	  if (inside (chord [i1] [i3],
	              chord [i1] [next (i3)],
	              chord [next (i1)] [next (i3)],
		      chord [next (i1)] [i3],
	              chord [i2] [next (i4)]))
            internalValue += 1;
	  if (inside (chord [i1] [i3],
	              chord [i1] [next (i3)],
	              chord [next (i1)] [next (i3)],
		      chord [next (i1)] [i3],
	              chord [next (i2)] [i4]))
            internalValue += 1;
	  if (inside (chord [i1] [i3],
	              chord [i1] [next (i3)],
	              chord [next (i1)] [next (i3)],
		      chord [next (i1)] [i3],
	              chord [next (i2)] [next (i4)]))
            internalValue += 1;
	  if (inside (chord [i2] [i4],
	              chord [i2] [next (i4)],
	              chord [next (i2)] [next (i4)],
		      chord [next (i2)] [i4],
	              chord [i1] [i3]))
            internalValue += 1;
	  if (inside (chord [i2] [i4],
	              chord [i2] [next (i4)],
	              chord [next (i2)] [next (i4)],
		      chord [next (i2)] [i4],
	              chord [i1] [next (i3)]))
            internalValue += 1;
	  if (inside (chord [i2] [i4],
	              chord [i2] [next (i4)],
	              chord [next (i2)] [next (i4)],
		      chord [next (i2)] [i4],
	              chord [next (i1)] [i3]))
            internalValue += 1;
	  if (inside (chord [i2] [i4],
	              chord [i2] [next (i4)],
	              chord [next (i2)] [next (i4)],
		      chord [next (i2)] [i4],
	              chord [next (i1)] [next (i3)]))
            internalValue += 1;
        }

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
  {
    for (i2 = 0; i2 < length; i2++)
      delete[] chord [i1] [i2];
    delete[] chord [i1];
  }
  delete[] chord;
}

void knot::prmExperimental2::compute (void)
{
  internalValue = 0.0;

  int i1, i2, o;

  // Вычисляем заранее касательные векторы.
  double **tangs = new double* [length];
  for (i1 = 0; i1 < length; i1++)
  {
    tangs [i1] = new double [3];
    tangs [i1] [0] = points [next (i1)] [0] - points [i1] [0];
    tangs [i1] [1] = points [next (i1)] [1] - points [i1] [1];
    tangs [i1] [2] = points [next (i1)] [2] - points [i1] [2];
  }

  // Вычисляем ``гауссовы произведения''.
  double chord [3], chord_len;
  double **gauss = new double* [length];

  for (i1 = 0; i1 < length; i1++)
  {
    gauss [i1] = new double [length];
    gauss [i1] [i1] = 0.0;
    for (i2 = 0; i2 < i1; i2++)
    {
      chord [0] = ( points [i1] [0] + points [next (i1)] [0] -
                    points [i2] [0] - points [next (i2)] [0] ) / 2;
      chord [1] = ( points [i1] [1] + points [next (i1)] [1] -
                    points [i2] [1] - points [next (i2)] [1] ) / 2;
      chord [2] = ( points [i1] [2] + points [next (i1)] [2] -
                    points [i2] [2] - points [next (i2)] [2] ) / 2;
      chord_len = sqrt (vector_square (chord));
      gauss [i1] [i2] = det (tangs [i1], tangs [i2], chord) /
                        (chord_len * chord_len * chord_len);
      gauss [i2] [i1] = gauss [i1] [i2];
    }
  }

  // Вычисляем суммы ``гауссовых произведений''.

  // В gauss_sum [i1] [i2] находится сумма ``гауссовых произведений''
  // для всех хорд с началом в i1 и концом от next (i1) до i2.
  double **gauss_sum = new double* [length];

  for (i1 = 0; i1 < length; i1++)
  {
    gauss_sum [i1] = new double [length];
    gauss_sum [i1] [i1] = 0.0;
    for (i2 = next (i1); i2 != i1; i2 = next (i2))
      gauss_sum [i1] [i2] = gauss_sum [i1] [prev (i2)] + gauss [i1] [i2];
  }

  double tmp, tmp2;
  for (i1 = 0; i1 < length; i1++)
  {
    tmp = 0.0;
    for (i2 = next (next (i1)); i2 != i1; i2 = next (i2))
    {
      tmp += gauss_sum [prev (i2)] [prev (i1)] -
             gauss_sum [i2] [prev (prev (i2))] + gauss_sum [i2] [i1];
      tmp2 = gauss [i1] [i2];
      for (o = 1; o < order; o++)
        tmp2 *= tmp / 16;
      internalValue += tmp2;
    }
  }

  internalValue /= 4 * M_PI * M_PI;

  // Удаляем заранее вычисленные вспомогательные значения.
  for (i1 = 0; i1 < length; i1++)
  {
    delete[] tangs [i1];
    delete[] gauss [i1];
    delete[] gauss_sum [i1];
  }
  delete[] tangs;
  delete[] gauss;
  delete[] gauss_sum;
}
