/* -*- Mode: C++; c-basic-offset: 2; tab-width: 2; indent-tabs-mode: nil -*-
 * Copyright (c) 1998-2000 Ludus Design enr.
 * All Rights Reserved.
 * Tous droits r�serv�s.
 */

#ifndef _HEADER_CRYPT
#define _HEADER_CRYPT

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*/

#include "types.h"

typedef unsigned char *POINTER;
typedef unsigned short int UINT2;
typedef unsigned long int UINT4;

typedef struct {
	UINT4 state[4];                                   // state (ABCD)
	UINT4 count[2];        // number of bits, modulo 2^64 (lsb first)
	unsigned char buffer[64];                         // input buffer
} MD5_CTX;

class Crypt {
	MD5_CTX context;
	Byte digest[16];
	char result[33];
	void MD5Init(MD5_CTX *);
	void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
	void MD5Update2(MD5_CTX *, unsigned char *, unsigned int);
	void MD5Final(unsigned char [16], MD5_CTX *);
	void shuffle();
	bool finalized;
public:
	Crypt();
	Crypt(const char *s, bool do_shuffle);
	virtual ~Crypt();
	void step(Byte *buf, Dword size);
	void step2(Byte *buf, Dword size);
	void finalize(bool do_shuffle);
	const char *get_digest_string() const;
	const Byte *get_digest() const;
};

#endif
