/*-
* SPDX-License-Identifier: BSD-2-Clause
*
* Copyright (c) 2022 Alexandre Joannou <aj443@cam.ac.uk>
*
* This material is based upon work supported by the DoD Information Analysis
* Center Program Management Office (DoD IAC PMO), sponsored by the Defense
* Technical Information Center (DTIC) under Contract No. FA807518D0004.  Any
* opinions, findings and conclusions or recommendations expressed in this
* material are those of the author(s) and do not necessarily reflect the views
* of the Air Force Installation Contracting Agency (AFICA).
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
* $FreeBSD$
*/

#define _GNU_SOURCE // for the splice() call

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SPLICE_LEN 1073741824 // 1GB

void usage ()
{
  printf ("Splice two UNIX FIFOs:\n"
          "  forever-splice [PRODUCER] [CONSUMER]\n");
  exit (EXIT_FAILURE);
}

int main (int argc, char* argv[])
{

  if (argc != 3) usage ();

  int fd_in = open (argv[1], O_RDONLY);
  if (fd_in == -1) {
    int errsv = errno;
    printf ("Couldn't open %s\n", argv[1]);
    exit (EXIT_FAILURE);
  }
  int fd_out = open (argv[2], O_WRONLY);
  if (fd_out == -1) {
    int errsv = errno;
    printf ("Couldn't open %s\n", argv[2]);
    exit (EXIT_FAILURE);
  }

  for (;;) {
    printf ("new splice\n");
    ssize_t n = splice (fd_in, NULL, fd_out, NULL, SPLICE_LEN, SPLICE_F_MORE);
    if (n < 0) {
      int errsv = errno;
      printf ( "%s:l%d: %s: %s (errno: %d)\n"
             , __FILE__, __LINE__, __func__, strerror (errsv), errsv);
      exit (EXIT_FAILURE);
    }
  }

  printf ("splicing done?\n");
  exit (EXIT_SUCCESS);
}
