//
// This file is part of the Terathon Common Library, by Eric Lengyel.
// Copyright 1999-2022, Terathon Software LLC
//
// This software is licensed under the GNU General Public License version 3.
// Separate proprietary licenses are available from Terathon Software.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
// EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//


#ifndef TSRect_h
#define TSRect_h


//# \component	Math Library
//# \prefix		Math/


#include "TSInteger.h"


namespace Terathon
{
	class Rect : public Range<Integer2D>
	{
		public:

			inline Rect() = default;

			Rect(int32 left, int32 top, int32 right, int32 bottom)
			{
				min.x = left;
				min.y = top;
				max.x = right;
				max.y = bottom;
			}

			Rect(const Integer2D& pmin, const Integer2D& pmax) : Range<Integer2D>(pmin, pmax)
			{
			}

			Rect& Set(int32 left, int32 top, int32 right, int32 bottom)
			{
				min.x = left;
				min.y = top;
				max.x = right;
				max.y = bottom;
				return (*this);
			}

			Rect& operator +=(const Integer2D& dp)
			{
				min += dp;
				max += dp;
				return (*this);
			}

			Rect& operator -=(const Integer2D& dp)
			{
				min -= dp;
				max -= dp;
				return (*this);
			}

			Rect& operator |=(const Integer2D& p)
			{
				min.x = Min(min.x, p.x);
				min.y = Min(min.y, p.y);
				max.x = Max(max.x, p.x);
				max.y = Max(max.y, p.y);
				return (*this);
			}

			Rect& operator |=(const Rect& r)
			{
				min.x = Min(min.x, r.min.x);
				min.y = Min(min.y, r.min.y);
				max.x = Max(max.x, r.max.x);
				max.y = Max(max.y, r.max.y);
				return (*this);
			}

			Rect& operator &=(const Rect& r)
			{
				min.x = Max(min.x, r.min.x);
				min.y = Max(min.y, r.min.y);
				max.x = Min(max.x, r.max.x);
				max.y = Min(max.y, r.max.y);
				return (*this);
			}

			Rect operator -(void) const
			{
				return (Rect(-max.x, -max.y, -min.x, -min.y));
			}

			Rect operator +(const Integer2D& p) const
			{
				return (Rect(min + p, max + p));
			}

			Rect operator -(const Integer2D& p) const
			{
				return (Rect(min - p, max - p));
			}

			Rect operator |(const Integer2D& p) const
			{
				return (Rect(Min(min.x, p.x), Min(min.y, p.y), Max(max.x, p.x), Max(max.y, p.y)));
			}

			Rect operator |(const Rect& r) const
			{
				return (Rect(Min(min.x, r.min.x), Min(min.y, r.min.y), Max(max.x, r.max.x), Max(max.y, r.max.y)));
			}

			Rect operator &(const Rect& r) const
			{
				return (Rect(Max(min.x, r.min.x), Max(min.y, r.min.y), Min(max.x, r.max.x), Min(max.y, r.max.y)));
			}

			bool operator ==(const Rect& r) const
			{
				return ((min == r.min) && (max == r.max));
			}

			bool operator !=(const Rect& r) const
			{
				return ((min != r.min) || (max != r.max));
			}

			bool Empty(void) const
			{
				return ((max.x <= min.x) || (max.y <= min.y));
			}

			bool Contains(const Integer2D& p) const
			{
				return ((uint32(p.x - min.x) < uint32(max.x - min.x)) && (uint32(p.y - min.y) < uint32(max.y - min.y)));
			}

			int32 GetRectWidth(void) const
			{
				return (max.x - min.x);
			}

			int32 GetRectHeight(void) const
			{
				return (max.y - min.y);
			}

			void Inset(int32 x, int32 y)
			{
				min.x += x;
				min.y += y;
				max.x -= x;
				max.y -= y;
			}
	};
}


#endif
