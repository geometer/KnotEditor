/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#include <math.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

void usage (const char *program)
{
  cerr << "usage:\n"
       << program
       << " <number_of_points (>0)> <meredian_number> <longitude_number>\n";
}

int main (int argc, char **argv)
{
  int num, L, M;

  if ( (argc != 4) ||
     !sscanf (argv [1], "%d", &num) ||
     (num < 1) ||
     !sscanf (argv [2], "%d", &M) ||
     !sscanf (argv [3], "%d", &L) )
    return usage (argv [0]), 1;

  {
    int m = abs (M), l = abs (L);
    while (l && m)
      if (l < m)
        m %= l;
      else
        l %= m;

    if (l + m != 1)
      return cerr << "Error: longitude and meredian numbers are not coprime.\n", 2;
  }

  cout << "#KNOT Torus (" << M << ", " << L << ") knot\n#LENGTH " << num << "\n";

  for (int i = 0; i < num; i++)
    cout <<
     sin (2 * L * M_PI / num * i) * (0.7 + 0.3 * cos (2 * M * M_PI / num * i) ) <<
     " " <<
     cos (2 * L * M_PI / num * i) * (0.7 + 0.3 * cos (2 * M * M_PI / num * i) ) <<
     " " <<
     0.3 * sin (2 * M * M_PI / num * i) <<
     "\n";
  
  return 0;
}
