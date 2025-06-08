//
// This file is part of the Terathon Common Library, by Eric Lengyel.
// Copyright 1999-2020, Terathon Software LLC
//
// Use of this library is free for all noncommercial purposes.
// For a commercial use license, contact Terathon Software.
// Attribution is required under all circumstances.
//


#ifndef TSComplex_h
#define TSComplex_h


#include "TSPlatform.h"


namespace Terathon
{
	//# \class	Complex		Encapsulates a complex number.
	//
	//# The $Complex$ class encapsulates a complex number.
	//
	//# \def	class Complex
	//
	//# \ctor	Complex();
	//# \ctor	Complex(float x, float y = 0.0F);
	//
	//# \param	x	The real part of the complex number.
	//# \param	y	The imaginary part of the complex number.
	//
	//# \desc
	//# The $Complex$ class encapsulates a complex number having the form <i>a</i>&#x202F;+&#x202F;<i>bi</i>.
	//# The real and imaginary parts of the complex number can be accessed through the members $a$ and $b$.
	//#
	//# The default constructor leaves the components of a complex number undefined. If the
	//# values $x$ and $y$ are supplied, then they are assigned to the real and imaginary components
	//# of the complex number, respectively.
	//
	//# \operator	float& operator [](machine k);
	//#				Returns a reference to the $k$-th component of a complex number.
	//#				The value of $k$ must be 0 or 1. Index 0 refers to the real part, and
	//#				index 1 refers to the imaginary part.
	//
	//# \operator	const float& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th component of a complex number.
	//#				The value of $k$ must be 0 or 1. Index 0 refers to the real part, and
	//#				index 1 refers to the imaginary part.
	//
	//# \operator	Complex& operator =(float r);
	//#				Assigns the value $r$ to the real component and sets the imaginary component to 0.
	//
	//# \operator	Complex& operator +=(const Complex& c);
	//#				Adds the complex number $c$.
	//
	//# \operator	Complex& operator +=(float r);
	//#				Adds the real number $r$.
	//
	//# \operator	Complex& operator -=(const Complex& c);
	//#				Subtracts the complex number $c$.
	//
	//# \operator	Complex& operator -=(float r);
	//#				Subtracts the real number $r$.
	//
	//# \operator	Complex& operator *=(const Complex& c);
	//#				Multiplies by the complex number $c$.
	//
	//# \operator	Complex& operator *=(float r);
	//#				Multiplies by the real number $r$.
	//
	//# \operator	Complex& operator /=(const Complex& c);
	//#				Divides by the complex number $c$.
	//
	//# \operator	Complex& operator /=(float r);
	//#				Divides by the real number $r$.
	//
	//# \action		Complex operator -(const Complex& c);
	//#				Returns the negation of the complex number $c$.
	//
	//# \action		Complex operator +(const Complex& c1, const Complex& c2);
	//#				Returns the sum of the complex numbers $c1$ and $c2$.
	//
	//# \action		Complex operator +(const Complex& c, float r);
	//#				Returns the sum of the complex number $c$ and the real number $r$.
	//
	//# \action		Complex operator +(float r, const Complex& c);
	//#				Returns the sum of the real number $r$ and the complex number $c$.
	//
	//# \action		Complex operator -(const Complex& c1, const Complex& c2);
	//#				Returns the difference of the complex numbers $c1$ and $c2$.
	//
	//# \action		Complex operator -(const Complex& c, float r);
	//#				Returns the difference of the complex number $c$ and the real number $r$.
	//
	//# \action		Complex operator -(float r, const Complex& c);
	//#				Returns the difference of the real number $r$ and the complex number $c$.
	//
	//# \action		Complex operator *(const Complex& c1, const Complex& c2);
	//#				Returns the product of the complex numbers $c1$ and $c2$.
	//
	//# \action		Complex operator *(const Complex& c, float r);
	//#				Returns the product of the complex number $c$ and the real number $r$.
	//
	//# \action		Complex operator *(float r, const Complex& c);
	//#				Returns the product of the real number $r$ and the complex number $c$.
	//
	//# \action		Complex operator /(const Complex& c1, const Complex& c2);
	//#				Returns the quotient of the complex numbers $c1$ and $c2$.
	//
	//# \action		Complex operator /(const Complex& c, float r);
	//#				Returns the quotient of the complex number $c$ and the real number $r$.
	//
	//# \action		Complex operator /(float r, const Complex& c);
	//#				Returns the quotient of the real number $r$ and the complex number $c$.
	//
	//# \action		bool operator ==(const Complex& c1, const Complex& c2);
	//#				Returns a boolean value indicating the equality of the two complex numbers $c1$ and $c2$.
	//
	//# \action		bool operator ==(const Complex& c, float r);
	//#				Returns a boolean value indicating the equality of the complex number $c$ and the real number $r$.
	//
	//# \action		bool operator !=(const Complex& c1, const Complex& c2);
	//#				Returns a boolean value indicating the inequality of the two complex numbers $c1$ and $c2$.
	//
	//# \action		bool operator !=(const Complex& c, float r);
	//#				Returns a boolean value indicating the inequality of the complex number $c$ and the real number $r$.
	//
	//# \action		float Modulus(const Complex& c);
	//#				Returns the modulus of the complex number $c$.
	//
	//# \action		float SquaredMod(const Complex& c);
	//#				Returns the squared modulus of the complex number $c$.
	//
	//# \action		Complex Conjugate(const Complex& c);
	//#				Returns the conjugate of the complex number $c$.
	//
	//# \action		Complex Inverse(const Complex& c);
	//#				Returns the inverse of the complex number $c$.


	//# \function	Complex::Set		Sets both components of a complex number.
	//
	//# \proto	Complex& Set(float x, float y = 0.0F);
	//
	//# \param	x	The new real component.
	//# \param	y	The new imaginary component.
	//
	//# \desc
	//# The $Set$ function sets the real and imaginary components of a complex number to the
	//# values given by the $x$ and $y$ parameters, respectively.
	//#
	//# The return value is a reference to the complex number object.


	class Complex
	{
		public:

			float	a;
			float	b;

			Complex() = default;

			Complex(float x, float y = 0.0F)
			{
				a = x;
				b = y;
			}

			Complex& Set(float x, float y = 0.0F)
			{
				a = x;
				b = y;
				return (*this);
			}

			void Set(float x, float y = 0.0F) volatile
			{
				a = x;
				b = y;
			}

			float& operator [](machine k)
			{
				return ((&a)[k]);
			}

			const float& operator [](machine k) const
			{
				return ((&a)[k]);
			}

			Complex& operator =(const Complex& c)
			{
				a = c.a;
				b = c.b;
				return (*this);
			}

			void operator =(const Complex& c) volatile
			{
				a = c.a;
				b = c.b;
			}

			Complex& operator =(float r)
			{
				a = r;
				b = 0.0F;
				return (*this);
			}

			void operator =(float r) volatile
			{
				a = r;
				b = 0.0F;
			}

			Complex& operator +=(const Complex& c)
			{
				a += c.a;
				b += c.b;
				return (*this);
			}

			Complex& operator +=(float r)
			{
				a += r;
				return (*this);
			}

			Complex& operator -=(const Complex& c)
			{
				a -= c.a;
				b -= c.b;
				return (*this);
			}

			Complex& operator -=(float r)
			{
				a -= r;
				return (*this);
			}

			Complex& operator *=(const Complex& c)
			{
				float r = a * c.a - b * c.b;
				b = a * c.b + b * c.a;
				a = r;
				return (*this);
			}

			Complex& operator *=(float r)
			{
				a *= r;
				b *= r;
				return (*this);
			}

			Complex& operator /=(float r)
			{
				r = 1.0F / r;
				a *= r;
				b *= r;
				return (*this);
			}

			Complex& operator /=(const Complex& c);
	};


	inline Complex operator -(const Complex& c)
	{
		return (Complex(-c.a, -c.b));
	}

	inline Complex operator +(const Complex& c1, const Complex& c2)
	{
		return (Complex(c1.a + c2.a, c1.b + c2.b));
	}

	inline Complex operator +(const Complex& c, float r)
	{
		return (Complex(c.a + r, c.b));
	}

	inline Complex operator +(float r, const Complex& c)
	{
		return (Complex(r + c.a, c.b));
	}

	inline Complex operator -(const Complex& c1, const Complex& c2)
	{
		return (Complex(c1.a - c2.a, c1.b - c2.b));
	}

	inline Complex operator -(const Complex& c, float r)
	{
		return (Complex(c.a - r, c.b));
	}

	inline Complex operator -(float r, const Complex& c)
	{
		return (Complex(r - c.a, -c.b));
	}

	inline Complex operator *(const Complex& c1, const Complex& c2)
	{
		return (Complex(c1.a * c2.a - c1.b * c2.b, c1.a * c2.b + c1.b * c2.a));
	}

	inline Complex operator *(const Complex& c, float r)
	{
		return (Complex(c.a * r, c.b * r));
	}

	inline Complex operator *(float r, const Complex& c)
	{
		return (Complex(r * c.a, r * c.b));
	}

	inline Complex operator /(const Complex& c, float r)
	{
		r = 1.0F / r;
		return (Complex(c.a * r, c.b * r));
	}

	inline bool operator ==(const Complex& c1, const Complex& c2)
	{
		return ((c1.a == c2.a) && (c1.b == c2.b));
	}

	inline bool operator ==(const Complex& c, float r)
	{
		return ((c.a == r) && (c.b == 0.0F));
	}

	inline bool operator !=(const Complex& c1, const Complex& c2)
	{
		return ((c1.a != c2.a) || (c1.b != c2.b));
	}

	inline bool operator !=(const Complex& c, float r)
	{
		return ((c.a != r) || (c.b != 0.0F));
	}


	inline float Modulus(const Complex& c)
	{
		return (Sqrt(c.a * c.a + c.b * c.b));
	}

	inline float SquaredMod(const Complex& c)
	{
		return (c.a * c.a + c.b * c.b);
	}

	inline Complex Conjugate(const Complex& c)
	{
		return (Complex(c.a, -c.b));
	}

	inline Complex Inverse(const Complex& c)
	{
		return (Conjugate(c) / SquaredMod(c));
	}

	inline Complex& Complex::operator /=(const Complex& c)
	{
		return (*this *= Inverse(c));
	}

	inline Complex operator /(const Complex& c1, const Complex& c2)
	{
		return (c1 * Inverse(c2));
	}

	inline Complex operator /(float r, const Complex& c)
	{
		return (r * Inverse(c));
	}
}


#endif
