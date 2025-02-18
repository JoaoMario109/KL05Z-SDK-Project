/**
 * @file	string.cpp
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2023
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * String function implementations optimized for embedded.
 *
 */

#include <math.h>
#include <system/assert.h>
#include "string.h"
#include "swap.h"

#define ASSERT(x) SYSTEM_ASSERT(x)

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef STR_COPY_FUNC
// Credits: Erich Styger component from Processor Expert https://mcuoneclipse.com/author/mcuoneclipse/
void Util_StrCopy(uint8_t *dst, size_t dstSize, const char *src)
{
	dstSize--; /* for zero byte */
	while (dstSize > 0 && *src != '\0') {
		*dst++ = *src++;
		dstSize--;
	}
	*dst = '\0';
}
#endif /* STR_COPY_FUNC */

#ifdef STR_CAT_FUNC
// Credits: Erich Styger component from Processor Expert https://mcuoneclipse.com/author/mcuoneclipse/
void Util_StrCat(uint8_t *dst, size_t dstSize, const unsigned char *src) {
	dstSize--; /* for zero byte */
	/* point to the end of the source */
	while (dstSize > 0 && *dst != '\0') {
		dst++;
		dstSize--;
	}
	/* copy the src in the destination */
	while (dstSize > 0 && *src != '\0') {
		*dst++ = *src++;
		dstSize--;
	}
	/* terminate the string */
	*dst = '\0';
}
#endif /* EMB_STR_CAT_FUNC */

#ifdef CHAR_CAT_FUNC
// Credits: Erich Styger component from Processor Expert https://mcuoneclipse.com/author/mcuoneclipse/
void Util_CharCat(unsigned char *dst, size_t dstSize, uint8_t ch) {
	dstSize--; /* for zero byte */
	/* point to the end of the source */
	while (dstSize > 0 && *dst != '\0') {
		dst++;
		dstSize--;
	}
	/* copy the ch in the destination */
	if (dstSize > 0) {
		*dst++ = ch;
	}
	/* terminate the string */
	*dst = '\0';
}
#endif /* CHAR_CAT_FUNC */

#ifdef REVERSE_FUNC
void Util_ReverseStr(uint8_t *str, size_t length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		Swap(*(str + start), *(str + end));
		start++;
		end--;
	}
}
#endif

#ifdef STR_FIND_FUNC
// Credits: Erich Styger component from Processor Expert https://mcuoneclipse.com/author/mcuoneclipse/
int16_t Util_StrFind(uint8_t *str, uint8_t *subStr) {
	int16_t i, len;

	len = (int16_t) StrLen((char*) subStr);
	for (i = 0; *str != '\0'; i++, str++) {
		if (StrNCmp((char*)str, (char*)subStr, len) == 0) {
			return i; /* found */
		}
	}
	return -1; /* not found */
}
#endif /* STR_FIND_FUNC */

#ifdef ITOA_FUNC
int Util_ItoA(int32_t num, uint8_t* str, uint8_t base) {
	int i = 0;

	ASSERT(str);

	bool isNegative = false;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return i;
	}

	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}

	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	Util_ReverseStr(str, i);

	return i;
}
#endif

#ifdef ATOI_FUNC
/*------------------------------------------------------------------------/
 /  Universal string handler for user console interface
 /-------------------------------------------------------------------------/
 /
 /  Copyright (C) 2010, ChaN, all right reserved.
 /
 / * This software is a free software and there is NO WARRANTY.
 / * No restriction on use. You can use, modify and redistribute it for
 /   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
 / * Redistributions of source code must retain the above copyright notice.
 /
 /-------------------------------------------------------------------------*/
bool Util_AtoI(const unsigned char **str, int32_t *res) {
	/* 123 -5   0x3ff 0b1111 0377 3.25  w "
	 ^                               1st call returns 123 and next ptr
	 ^                            2nd call returns -5 and next ptr
	 ^                    3rd call returns 1023 and next ptr
	 ^             4th call returns 15 and next ptr
	 ^        5th call returns 255 and next ptr
	 ^      6th call returns 3 and next ptr, caller needs to read '.'
	 ^    7th call returns 25 and next ptr
	 ^ 8th call fails and returns ERR_FAILED
	 */
	uint32_t val;
	uint8_t c, r, s = 0;

	*res = 0;
	while (**str == ' ') {
		(*str)++; /* Skip leading spaces */
	}
	c = **str;
	if (c == '-') { /* negative? */
		s = 1;
		c = *(++(*str));
	}
	if (c == '0') {
		c = *(++(*str));
		switch (c) {
		case 'x': /* hexadecimal */
			r = 16;
			c = *(++(*str));
			break;
		case 'b': /* binary */
			r = 2;
			c = *(++(*str));
			break;
		default:
			if (c <= ' ' || c == '.') {
				return true; /* single zero */
			}
			if (c < '0' || c > '9') {
				return false; /* invalid char */
			}
			r = 8; /* octal */
			break;
		} /* switch */
	} else {
		if (c < '0' || c > '9') {
			return false; /* EOL or invalid char */
		}
		r = 10; /* decimal */
	}
	val = 0;
	while (c > ' ' && c != '.') {
		if (c >= 'a')
			c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9)
				return false; /* invalid char */
		}
		if (c >= r)
			return false; /* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));
	} /* while */
	if (s)
		val = 0 - val; /* apply sign if needed */
	*res = (long) val;
	return true;
}
#endif /* ATOI_FUNC */

#if defined(ITOA_FUNC) && defined(FTOA_FUNC)

// Credits: Geeks for geeks https://www.geeksforgeeks.org/convert-floating-point-number-string/
int Util_FtoA(float n, uint8_t *res, int afterpoint) {
	// Extract integer part
	int ipart = (int) n;

	// Extract floating part
	float fpart = n - (float) ipart;

	// convert integer part to string
	int i = Util_ItoA(ipart, res, 10);

	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.';  // add dot

		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		if (fpart < 0) {
			fpart *= -1;
		}

		i += Util_ItoA((int) fpart, res + i + 1, 10);
	}

	return i;
}
#endif /* defined(ITOA) && defined(FTOA) */

#ifdef ATOF_FUNC
bool Util_AtoF(const unsigned char** str, float* res)
{
	float rez = 0, fact = 1;
	uint8_t c;

	*res = 0;
	while (**str == ' ') {
		(*str)++; /* Skip leading spaces */
	}
	c = **str;

	if (c == '-') {
		c = *(++(*str));
		fact = -1;
	};
	for (int point_seen = 0; c;) {
		if (c == '.') {
			point_seen = 1;
		} else {
			int d = c - '0';
			if (d >= 0 && d <= 9) {
				if (point_seen) {
					fact /= 10.0f;
				}
				rez = rez * 10.0f + (float) d;
			} else {
				return false; // invalid char
			}
		}
		c = *(++(*str));
	}
	*res = rez * fact;
	return true;
}
#endif /* ATOF_FUNC */
