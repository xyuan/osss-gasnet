/*
 *
 * Copyright (c) 2011 - 2013
 *   University of Houston System and Oak Ridge National Laboratory.
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * 
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * o Neither the name of the University of Houston System, Oak Ridge
 *   National Laboratory nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */



/**
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#include "trace.h"

/**
 * define accepted size units in ascending order, which are
 * S.I. compliant
 *
 * http://en.wikipedia.org/wiki/SI_Unit_Prefixes
 *
 */

static char *units_string = "kmgtpezy";

static const size_t multiplier = (size_t) 1000;

/**
 * segment size can be expressed with scaling units.  Parse those.
 */

size_t
__shmem_parse_size (char *size_str)
{
  char unit = '\0';
  size_t bytes = (size_t) 1;
  char *p;

  p = size_str;
  while (*p != '\0')
    {
      if (!isdigit (*p))
	{
	  unit = *p;
	  *p = '\0';		/* get unit, terminate numeral */
	  break;
	}
      p += 1;
    }

  /* if there's a unit, work out how much to scale */
  if (unit != '\0')
    {
      int foundit = 0;
      char *usp = units_string;

      unit = tolower (unit);
      while (*usp != '\0')
	{
	  bytes *= multiplier;
	  if (*usp == unit)
	    {
	      foundit = 1;
	      break;
	    }
	  usp += 1;
	}

      if (!foundit)
	{
	  /* don't know that unit! */
	  return (size_t) -1;
	}
    }

  return bytes * (size_t) strtol (size_str, (char **) NULL, 10);
}
