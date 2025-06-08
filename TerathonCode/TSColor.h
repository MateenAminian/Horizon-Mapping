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


#ifndef TSColor_h
#define TSColor_h


//# \component	Math Library
//# \prefix		Math/


#include "TSHalf.h"


#define TERATHON_COLORRGB 1
#define TERATHON_COLORRGBA 1
#define TERATHON_COLOR1U 1
#define TERATHON_COLOR2U 1
#define TERATHON_COLOR4U 1
#define TERATHON_COLOR1S 1
#define TERATHON_COLOR2S 1
#define TERATHON_COLOR4S 1
#define TERATHON_COLOR4H 1


namespace Terathon
{
	struct ConstColorRgba;


	//# \class	ColorRgb	Encapsulates a floating-point RGB color.
	//
	//# The $ColorRgb$ class encapsulates a floating-point RGB color.
	//
	//# \def	class ColorRgb
	//
	//# \data	ColorRgb
	//
	//# \ctor	ColorRgb();
	//# \ctor	ColorRgb(float r, float g, float b);
	//
	//# \desc
	//# The $ColorRgb$ class encapsulates a color having floating-point red, green, and blue
	//# components in the range [0.0,&#x202F;1.0]. An additional alpha component is provided by the
	//# $@ColorRgba@$ class. When a $ColorRgb$ object is converted to a $@ColorRgba@$ object,
	//# the alpha component is assumed to be 1.0.
	//#
	//# The default constructor leaves the components of the color undefined. If the values
	//# $r$, $g$, and $b$ are supplied, then they are assigned to the red, green, and blue
	//# components of the color, respectively.
	//
	//# \operator	float& operator [](machine k);
	//#				Returns a reference to the $k$-th component of a color.
	//#				The value of $k$ must be 0, 1, or 2.
	//
	//# \operator	const float& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th component of a color.
	//#				The value of $k$ must be 0, 1, or 2.
	//
	//# \operator	ColorRgb& operator =(float s);
	//#				Sets all three components to the value $s$.
	//
	//# \operator	ColorRgb& operator +=(const ColorRgb& c);
	//#				Adds the color $c$.
	//
	//# \operator	ColorRgb& operator -=(const ColorRgb& c);
	//#				Subtracts the color $c$.
	//
	//# \operator	ColorRgb& operator *=(const ColorRgb& c);
	//#				Multiplies by the color $c$.
	//
	//# \operator	ColorRgb& operator *=(float s);
	//#				Multiplies all three components by $s$.
	//
	//# \operator	ColorRgb& operator /=(float s);
	//#				Divides all three components by $s$.
	//
	//# \action		ColorRgb operator -(const ColorRgb& c);
	//#				Returns the negation of the color $c$.
	//
	//# \action		ColorRgb operator +(const ColorRgb& c1, const ColorRgb& c2);
	//#				Returns the sum of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgb operator -(const ColorRgb& c1, const ColorRgb& c2);
	//#				Returns the difference of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgb operator *(const ColorRgb& c1, const ColorRgb& c2);
	//#				Returns the product of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgb operator *(const ColorRgb& c, float s);
	//#				Returns the product of the color $c$ and the scalar $s$.
	//
	//# \action		ColorRgb operator *(float s, const ColorRgb& c);
	//#				Returns the product of the color $c$ and the scalar $s$.
	//
	//# \action		ColorRgb operator /(const ColorRgb& c, float s);
	//#				Returns the product of the color $c$ and the inverse of the scalar $s$.
	//
	//# \action		bool operator ==(const ColorRgb& c1, const ColorRgb& c2);
	//#				Returns a boolean value indicating whether the colors $c1$ and $c2$ are equal.
	//
	//# \action		bool operator !=(const ColorRgb& c1, const ColorRgb& c2);
	//#				Returns a boolean value indicating whether the colors $c1$ and $c2$ are not equal.
	//
	//# \action		float Luminance(const ColorRgb& c);
	//#				Returns the luminance value of the color $c$.
	//
	//# \also	$@ColorRgba@$


	//# \function	ColorRgb::Set		Sets all three components of a color.
	//
	//# \proto	ColorRgb& Set(float r, float g, float b);
	//
	//# \param	r	The new red component.
	//# \param	g	The new green component.
	//# \param	b	The new blue component.
	//
	//# \desc
	//# The $Set$ function sets the red, green, and blue components of a color to the values
	//# given by the $r$, $g$, and $b$ parameters, respectively.
	//#
	//# The return value is a reference to the color object.


	//# \member		ColorRgb

	class ColorRgb
	{
		public:

			float		red;		//## The red component.
			float		green;		//## The green component.
			float		blue;		//## The blue component.

			inline ColorRgb() = default;

			ColorRgb(float r, float g, float b)
			{
				red = r;
				green = g;
				blue = b;
			}

			ColorRgb& Set(float r, float g, float b)
			{
				red = r;
				green = g;
				blue = b;
				return (*this);
			}

			void Set(float r, float g, float b) volatile
			{
				red = r;
				green = g;
				blue = b;
			}

			float& operator [](machine k)
			{
				return ((&red)[k]);
			}

			const float& operator [](machine k) const
			{
				return ((&red)[k]);
			}

			ColorRgb& operator =(const ColorRgb& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				return (*this);
			}

			void operator =(const ColorRgb& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
			}

			ColorRgb& operator =(float s)
			{
				red = s;
				green = s;
				blue = s;
				return (*this);
			}

			void operator =(float s) volatile
			{
				red = s;
				green = s;
				blue = s;
			}

			ColorRgb& operator +=(const ColorRgb& c)
			{
				red += c.red;
				green += c.green;
				blue += c.blue;
				return (*this);
			}

			ColorRgb& operator -=(const ColorRgb& c)
			{
				red -= c.red;
				green -= c.green;
				blue -= c.blue;
				return (*this);
			}

			ColorRgb& operator *=(const ColorRgb& c)
			{
				red *= c.red;
				green *= c.green;
				blue *= c.blue;
				return (*this);
			}

			ColorRgb& operator *=(float s)
			{
				red *= s;
				green *= s;
				blue *= s;
				return (*this);
			}

			ColorRgb& operator /=(float s)
			{
				s = 1.0F / s;
				red *= s;
				green *= s;
				blue *= s;
				return (*this);
			}

			TERATHON_API void GetHexString(char *string) const;
			TERATHON_API ColorRgb& SetHexString(const char *string);
	};


	inline ColorRgb operator -(const ColorRgb& c)
	{
		return (ColorRgb(-c.red, -c.green, -c.blue));
	}

	inline ColorRgb operator +(const ColorRgb& c1, const ColorRgb& c2)
	{
		return (ColorRgb(c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue));
	}

	inline ColorRgb operator -(const ColorRgb& c1, const ColorRgb& c2)
	{
		return (ColorRgb(c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue));
	}

	inline ColorRgb operator *(const ColorRgb& c1, const ColorRgb& c2)
	{
		return (ColorRgb(c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue));
	}

	inline ColorRgb operator *(const ColorRgb& c, float s)
	{
		return (ColorRgb(c.red * s, c.green * s, c.blue * s));
	}

	inline ColorRgb operator *(float s, const ColorRgb& c)
	{
		return (ColorRgb(s * c.red, s * c.green, s * c.blue));
	}

	inline ColorRgb operator /(const ColorRgb& c, float s)
	{
		s = 1.0F / s;
		return (ColorRgb(c.red * s, c.green * s, c.blue * s));
	}

	inline bool operator ==(const ColorRgb& c1, const ColorRgb& c2)
	{
		return ((c1.red == c2.red) && (c1.green == c2.green) && (c1.blue == c2.blue));
	}

	inline bool operator !=(const ColorRgb& c1, const ColorRgb& c2)
	{
		return ((c1.red != c2.red) || (c1.green != c2.green) || (c1.blue != c2.blue));
	}

	inline float Luminance(const ColorRgb& c)
	{
		return (c.red * 0.212639F + c.green * 0.715169F + c.blue * 0.072192F);
	}


	//# \class	ColorRgba	Encapsulates a floating-point RGBA color.
	//
	//# The $ColorRgba$ class encapsulates a floating-point RGBA color.
	//
	//# \def	class ColorRgba
	//
	//# \data	ColorRgba
	//
	//# \ctor	ColorRgba();
	//# \ctor	ColorRgba(const ColorRgb& c, float a = 1.0F);
	//# \ctor	ColorRgba(float r, float g, float b, float a = 1.0F);
	//
	//# \desc
	//# The $ColorRgba$ class encapsulates a color having floating-point red, green, blue, and
	//# alpha components in the range [0.0,&#x202F;1.0]. When a $@ColorRgb@$ object is converted
	//# to a $ColorRgba$ object, the alpha component is assumed to be 1.0.
	//#
	//# The default constructor leaves the components of the color undefined. If the values
	//# $r$, $g$, $b$, and $a$ are supplied, then they are assigned to the red, green, blue,
	//# and alpha components of the color, respectively.
	//
	//# \operator	float& operator [](machine k);
	//#				Returns a reference to the $k$-th component of a color.
	//#				The value of $k$ must be 0, 1, 2, or 3.
	//
	//# \operator	const float& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th component of a color.
	//#				The value of $k$ must be 0, 1, 2, or 3.
	//
	//# \operator	ColorRgba& operator =(const ColorRgb& c);
	//#				Copies the red, green, and blue components of $c$, and assigns
	//#				the alpha component a value of 1.
	//
	//# \operator	ColorRgba& operator =(float s);
	//#				Assigns the value $s$ to the red, green, and blue components, and
	//#				assigns the alpha component a value of 1.
	//
	//# \operator	ColorRgba& operator +=(const ColorRgba& c);
	//#				Adds the color $c$.
	//
	//# \operator	ColorRgba& operator +=(const ColorRgb& c);
	//#				Adds the color $c$. The alpha component is not modified.
	//
	//# \operator	ColorRgba& operator -=(const ColorRgba& c);
	//#				Subtracts the color $c$.
	//
	//# \operator	ColorRgba& operator -=(const ColorRgb& c);
	//#				Subtracts the color $c$. The alpha component is not modified.
	//
	//# \operator	ColorRgba& operator *=(const ColorRgba& c);
	//#				Multiplies by the color $c$.
	//
	//# \operator	ColorRgba& operator *=(const ColorRgb& c);
	//#				Multiplies by the color $c$. The alpha component is not modified.
	//
	//# \operator	ColorRgba& operator *=(float s);
	//#				Multiplies all four components by $s$.
	//
	//# \operator	ColorRgba& operator /=(float s);
	//#				Divides all four components by $s$.
	//
	//# \action		ColorRgba operator -(const ColorRgba& c);
	//#				Returns the negation of the color $c$.
	//
	//# \action		ColorRgba operator +(const ColorRgba& c1, const ColorRgba& c2);
	//#				Returns the sum of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgba operator +(const ColorRgba& c1, const ColorRgb& c2);
	//#				Returns the sum of the colors $c1$ and $c2$. The alpha component of $c2$ is assumed to be 0.
	//
	//# \action		ColorRgba operator -(const ColorRgba& c1, const ColorRgba& c2);
	//#				Returns the difference of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgba operator -(const ColorRgba& c1, const ColorRgb& c2);
	//#				Returns the difference of the colors $c1$ and $c2$. The alpha component of $c2$ is assumed to be 0.
	//
	//# \action		ColorRgba operator *(const ColorRgba& c1, const ColorRgba& c2);
	//#				Returns the product of the colors $c1$ and $c2$.
	//
	//# \action		ColorRgba operator *(const ColorRgba& c1, const ColorRgb& c2);
	//#				Returns the product of the colors $c1$ and $c2$. The alpha component of $c2$ is assumed to be 1.
	//
	//# \action		ColorRgba operator *(const ColorRgb& c1, const ColorRgba& c2);
	//#				Returns the product of the colors $c1$ and $c2$. The alpha component of $c1$ is assumed to be 1.
	//
	//# \action		ColorRgba operator *(const ColorRgba& c, float s);
	//#				Returns the product of the color $c$ and the scalar $s$.
	//
	//# \action		ColorRgba operator *(float s, const ColorRgba& c);
	//#				Returns the product of the color $c$ and the scalar $s$.
	//
	//# \action		ColorRgba operator /(const ColorRgba& c, float s);
	//#				Returns the product of the color $c$ and the inverse of the scalar $s$.
	//
	//# \action		bool operator ==(const ColorRgba& c1, const ColorRgba& c2);
	//#				Returns a boolean value indicating whether the two colors $c1$ and $c2$ are equal.
	//
	//# \action		bool operator ==(const ColorRgba& c1, const ColorRgb& c2);
	//#				Returns a boolean value indicating whether the two colors $c1$ and $c2$ are equal. The alpha component of $c2$ is assumed to be 1.
	//
	//# \action		bool operator !=(const ColorRgba& c1, const ColorRgba& c2);
	//#				Returns a boolean value indicating whether the two colors $c1$ and $c2$ are not equal.
	//
	//# \action		bool operator !=(const ColorRgba& c1, const ColorRgb& c2);
	//#				Returns a boolean value indicating whether the two colors $c1$ and $c2$ are not equal. The alpha component of $c2$ is assumed to be 1.
	//
	//# \action		float Luminance(const ColorRgba& c);
	//#				Returns the luminance value of the color $c$.
	//
	//# \also	$@ColorRgb@$


	//# \function	ColorRgba::Set		Sets all four components of a color.
	//
	//# \proto	ColorRgba& Set(float r, float g, float b, float a = 1.0F);
	//
	//# \param	r	The new red component.
	//# \param	g	The new green component.
	//# \param	b	The new blue component.
	//# \param	a	The new alpha component.
	//
	//# \desc
	//# The $Set$ function sets the red, green, blue, and alpha components of a color to the values
	//# given by the $r$, $g$, $b$, and $a$ parameters, respectively.
	//#
	//# The return value is a reference to the color object.


	//# \function	ColorRgba::GetColorRgb		Returns a reference to a $@ColorRgb@$ object.
	//
	//# \proto	ColorRgb& GetColorRgb(void);
	//# \proto	const ColorRgb& GetColorRgb(void) const;
	//
	//# \desc
	//# The $GetColorRgb$ function returns a reference to a $@ColorRgb@$ object that refers to
	//# the same data contained within the $ColorRgba$ object.


	//# \member		ColorRgba

	class ColorRgba
	{
		public:

			float		red;		//## The red component.
			float		green;		//## The green component.
			float		blue;		//## The blue component.
			float		alpha;		//## The alpha component.

			inline ColorRgba() = default;

			ColorRgba(const ColorRgb& c, float a = 1.0F)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = a;
			}

			ColorRgba(float r, float g, float b, float a = 1.0F)
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
			}

			ColorRgba& Set(const ColorRgb& c, float a)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = a;
				return (*this);
			}

			void Set(const ColorRgb& c, float a) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = a;
			}

			ColorRgba& Set(float r, float g, float b, float a = 1.0F)
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
				return (*this);
			}

			void Set(float r, float g, float b, float a = 1.0F) volatile
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
			}

			float& operator [](machine k)
			{
				return ((&red)[k]);
			}

			const float& operator [](machine k) const
			{
				return ((&red)[k]);
			}

			operator const ColorRgb&(void) const
			{
				return (reinterpret_cast<const ColorRgb&>(*this));
			}

			ColorRgb& GetColorRgb(void)
			{
				return (reinterpret_cast<ColorRgb&>(*this));
			}

			const ColorRgb& GetColorRgb(void) const
			{
				return (reinterpret_cast<const ColorRgb&>(*this));
			}

			ColorRgba& operator =(const ColorRgba& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
				return (*this);
			}

			void operator =(const ColorRgba& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
			}

			ColorRgba& operator =(const ColorRgb& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = 1.0F;
				return (*this);
			}

			void operator =(const ColorRgb& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = 1.0F;
			}

			ColorRgba& operator =(float s)
			{
				red = s;
				green = s;
				blue = s;
				alpha = 1.0F;
				return (*this);
			}

			void operator =(float s) volatile
			{
				red = s;
				green = s;
				blue = s;
				alpha = 1.0F;
			}

			ColorRgba& operator +=(const ColorRgba& c)
			{
				red += c.red;
				green += c.green;
				blue += c.blue;
				alpha += c.alpha;
				return (*this);
			}

			ColorRgba& operator +=(const ColorRgb& c)
			{
				red += c.red;
				green += c.green;
				blue += c.blue;
				return (*this);
			}

			ColorRgba& operator -=(const ColorRgba& c)
			{
				red -= c.red;
				green -= c.green;
				blue -= c.blue;
				alpha -= c.alpha;
				return (*this);
			}

			ColorRgba& operator -=(const ColorRgb& c)
			{
				red -= c.red;
				green -= c.green;
				blue -= c.blue;
				return (*this);
			}

			ColorRgba& operator *=(const ColorRgba& c)
			{
				red *= c.red;
				green *= c.green;
				blue *= c.blue;
				alpha *= c.alpha;
				return (*this);
			}

			ColorRgba& operator *=(const ColorRgb& c)
			{
				red *= c.red;
				green *= c.green;
				blue *= c.blue;
				return (*this);
			}

			ColorRgba& operator *=(float s)
			{
				red *= s;
				green *= s;
				blue *= s;
				alpha *= s;
				return (*this);
			}

			ColorRgba& operator /=(float s)
			{
				s = 1.0F / s;
				red *= s;
				green *= s;
				blue *= s;
				alpha *= s;
				return (*this);
			}

			TERATHON_API void GetHexString(char *string) const;
			TERATHON_API ColorRgba& SetHexString(const char *string);
	};


	inline ColorRgba operator -(const ColorRgba& c)
	{
		return (ColorRgba(-c.red, -c.green, -c.blue, -c.alpha));
	}

	inline ColorRgba operator +(const ColorRgba& c1, const ColorRgba& c2)
	{
		return (ColorRgba(c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue, c1.alpha + c2.alpha));
	}

	inline ColorRgba operator +(const ColorRgba& c1, const ColorRgb& c2)
	{
		return (ColorRgba(c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue, c1.alpha));
	}

	inline ColorRgba operator -(const ColorRgba& c1, const ColorRgba& c2)
	{
		return (ColorRgba(c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue, c1.alpha - c2.alpha));
	}

	inline ColorRgba operator -(const ColorRgba& c1, const ColorRgb& c2)
	{
		return (ColorRgba(c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue, c1.alpha));
	}

	inline ColorRgba operator *(const ColorRgba& c1, const ColorRgba& c2)
	{
		return (ColorRgba(c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue, c1.alpha * c2.alpha));
	}

	inline ColorRgba operator *(const ColorRgba& c1, const ColorRgb& c2)
	{
		return (ColorRgba(c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue, c1.alpha));
	}

	inline ColorRgba operator *(const ColorRgb& c1, const ColorRgba& c2)
	{
		return (ColorRgba(c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue, c2.alpha));
	}

	inline ColorRgba operator *(const ColorRgba& c, float s)
	{
		return (ColorRgba(c.red * s, c.green * s, c.blue * s, c.alpha * s));
	}

	inline ColorRgba operator *(float s, const ColorRgba& c)
	{
		return (ColorRgba(s * c.red, s * c.green, s * c.blue, s * c.alpha));
	}

	inline ColorRgba operator /(const ColorRgba& c, float s)
	{
		s = 1.0F / s;
		return (ColorRgba(c.red * s, c.green * s, c.blue * s, c.alpha * s));
	}

	inline bool operator ==(const ColorRgba& c1, const ColorRgba& c2)
	{
		return ((c1.red == c2.red) && (c1.green == c2.green) && (c1.blue == c2.blue) && (c1.alpha == c2.alpha));
	}

	inline bool operator ==(const ColorRgba& c1, const ColorRgb& c2)
	{
		return ((c1.red == c2.red) && (c1.green == c2.green) && (c1.blue == c2.blue) && (c1.alpha == 1.0F));
	}

	inline bool operator !=(const ColorRgba& c1, const ColorRgba& c2)
	{
		return ((c1.red != c2.red) || (c1.green != c2.green) || (c1.blue != c2.blue) || (c1.alpha != c2.alpha));
	}

	inline bool operator !=(const ColorRgba& c1, const ColorRgb& c2)
	{
		return ((c1.red != c2.red) || (c1.green != c2.green) || (c1.blue != c2.blue) || (c1.alpha != 1.0F));
	}

	inline float Luminance(const ColorRgba& c)
	{
		return (c.red * 0.212639F + c.green * 0.715169F + c.blue * 0.072192F);
	}


	struct ConstColorRgb
	{
		float		red;
		float		green;
		float		blue;

		operator const ColorRgb&(void) const
		{
			return (reinterpret_cast<const ColorRgb&>(*this));
		}

		const ColorRgb *operator &(void) const
		{
			return (reinterpret_cast<const ColorRgb *>(this));
		}

		const ColorRgb *operator ->(void) const
		{
			return (reinterpret_cast<const ColorRgb *>(this));
		}
	};


	struct ConstColorRgba
	{
		float		red;
		float		green;
		float		blue;
		float		alpha;

		operator const ColorRgba&(void) const
		{
			return (reinterpret_cast<const ColorRgba&>(*this));
		}

		const ColorRgba *operator &(void) const
		{
			return (reinterpret_cast<const ColorRgba *>(this));
		}

		const ColorRgba *operator ->(void) const
		{
			return (reinterpret_cast<const ColorRgba *>(this));
		}
	};


	typedef uint8	Color1U;
	typedef int8	Color1S;


	//# \class	Color2U		Encapsulates a two-component unsigned integer color.
	//
	//# The $Color2U$ class encapsulates a two-component unsigned integer color.
	//
	//# \def	class Color2U
	//
	//# \ctor	Color2U();
	//# \ctor	Color2U(uint32 r, uint32 g);
	//
	//# \desc
	//# The $Color2U$ class encapsulates a color having two unsigned integer
	//# components in the range [0,&#x202F;255].
	//#
	//# The default constructor leaves the components of the color undefined.
	//
	//# \also	$@Color2S@$
	//# \also	$@Color4U@$
	//# \also	$@Color4S@$


	class Color2U
	{
		public:

			uint8		red;
			uint8		green;

			inline Color2U() = default;

			Color2U(uint32 r, uint32 g)
			{
				red = uint8(r);
				green = uint8(g);
			}

			Color2U& Set(uint32 r, uint32 g)
			{
				red = uint8(r);
				green = uint8(g);
				return (*this);
			}

			Color2U& Clear(void)
			{
				reinterpret_cast<uint16&>(*this) = 0;
				return (*this);
			}

			uint16 GetPackedColor(void) const
			{
				return (reinterpret_cast<const uint16&>(*this));
			}

			Color2U& SetPackedColor(uint16 c)
			{
				reinterpret_cast<uint16&>(*this) = c;
				return (*this);
			}

			void SetPackedColor(uint16 c) volatile
			{
				reinterpret_cast<volatile uint16&>(*this) = c;
			}

			Color2U& operator =(const Color2U& c)
			{
				reinterpret_cast<uint16&>(*this) = c.GetPackedColor();
				return (*this);
			}

			void operator =(const Color2U& c) volatile
			{
				reinterpret_cast<volatile uint16&>(*this) = c.GetPackedColor();
			}

			bool operator ==(const Color2U& c) const
			{
				return (GetPackedColor() == c.GetPackedColor());
			}

			bool operator !=(const Color2U& c) const
			{
				return (GetPackedColor() != c.GetPackedColor());
			}
	};


	//# \class	Color2S		Encapsulates a two-component signed integer color.
	//
	//# The $Color2S$ class encapsulates a two-component signed integer color.
	//
	//# \def	class Color2S
	//
	//# \ctor	Color2S();
	//# \ctor	Color2S(int32 r, int32 g);
	//
	//# \desc
	//# The $Color2S$ class encapsulates a color having two signed integer
	//# components in the range [&minus;127,&#x202F;+127].
	//#
	//# The default constructor leaves the components of the color undefined.
	//
	//# \also	$@Color2U@$
	//# \also	$@Color4S@$
	//# \also	$@Color4U@$


	class Color2S
	{
		public:

			int8		red;
			int8		green;

			inline Color2S() = default;

			Color2S(int32 r, int32 g)
			{
				red = int8(r);
				green = int8(g);
			}

			Color2S& Set(int32 r, int32 g)
			{
				red = int8(r);
				green = int8(g);
				return (*this);
			}

			Color2S& Clear(void)
			{
				reinterpret_cast<int16&>(*this) = 0;
				return (*this);
			}

			uint16 GetPackedColor(void) const
			{
				return (reinterpret_cast<const uint16&>(*this));
			}

			Color2S& SetPackedColor(uint16 c)
			{
				reinterpret_cast<uint16&>(*this) = c;
				return (*this);
			}

			void SetPackedColor(uint16 c) volatile
			{
				reinterpret_cast<volatile uint16&>(*this) = c;
			}

			Color2S& operator =(const Color2S& c)
			{
				reinterpret_cast<uint16&>(*this) = c.GetPackedColor();
				return (*this);
			}

			void operator =(const Color2S& c) volatile
			{
				reinterpret_cast<volatile uint16&>(*this) = c.GetPackedColor();
			}

			bool operator ==(const Color2S& c) const
			{
				return (GetPackedColor() == c.GetPackedColor());
			}

			bool operator !=(const Color2S& c) const
			{
				return (GetPackedColor() != c.GetPackedColor());
			}
	};


	//# \class	Color4U		Encapsulates a four-component unsigned integer color.
	//
	//# The $Color4U$ class encapsulates a four-component unsigned integer color.
	//
	//# \def	class Color4U
	//
	//# \ctor	Color4U();
	//# \ctor	Color4U(uint32 r, uint32 g, uint32 b, uint32 a = 255);
	//# \ctor	explicit Color4U(const ColorRgba& c);
	//
	//# \desc
	//# The $Color4U$ class encapsulates a color having unsigned integer red, green, blue, and alpha
	//# components in the range [0,&#x202F;255].
	//#
	//# The default constructor leaves the components of the color undefined.
	//
	//# \also	$@Color4S@$
	//# \also	$@Color2U@$
	//# \also	$@Color2S@$


	class Color4U
	{
		public:

			uint8		red;
			uint8		green;
			uint8		blue;
			uint8		alpha;

			inline Color4U() = default;

			Color4U(uint32 r, uint32 g, uint32 b, uint32 a = 255)
			{
				red = uint8(r);
				green = uint8(g);
				blue = uint8(b);
				alpha = uint8(a);
			}

			explicit Color4U(const ColorRgba& c)
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = uint8(c.alpha * 255.0F);
			}

			Color4U& Set(uint32 r, uint32 g, uint32 b, uint32 a = 255)
			{
				red = uint8(r);
				green = uint8(g);
				blue = uint8(b);
				alpha = uint8(a);
				return (*this);
			}

			void Set(uint32 r, uint32 g, uint32 b, uint32 a = 255) volatile
			{
				red = uint8(r);
				green = uint8(g);
				blue = uint8(b);
				alpha = uint8(a);
			}

			Color4U& Set(const ColorRgba& c)
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = uint8(c.alpha * 255.0F);
				return (*this);
			}

			void Set(const ColorRgba& c) volatile
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = uint8(c.alpha * 255.0F);
			}

			Color4U& Clear(void)
			{
				reinterpret_cast<uint32&>(*this) = 0;
				return (*this);
			}

			Color4U& ClearMaxAlpha(void)
			{
				reinterpret_cast<uint32&>(*this) = 0xFF000000;
				return (*this);
			}

			uint32 GetPackedColor(void) const
			{
				return (reinterpret_cast<const uint32&>(*this));
			}

			uint32 GetPackedRGBColor(void) const
			{
				return (reinterpret_cast<const uint32&>(*this) & 0x00FFFFFF);
			}

			Color4U& SetPackedColor(uint32 c)
			{
				reinterpret_cast<uint32&>(*this) = c;
				return (*this);
			}

			void SetPackedColor(uint32 c) volatile
			{
				reinterpret_cast<volatile uint32&>(*this) = c;
			}

			Color4U& operator =(const Color4U& c)
			{
				reinterpret_cast<uint32&>(*this) = c.GetPackedColor();
				return (*this);
			}

			void operator =(const Color4U& c) volatile
			{
				reinterpret_cast<volatile uint32&>(*this) = c.GetPackedColor();
			}

			Color4U& operator =(const ColorRgb& c)
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = 0xFF;
				return (*this);
			}

			void operator =(const ColorRgb& c) volatile
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = 0xFF;
			}

			Color4U& operator =(const ColorRgba& c)
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = uint8(c.alpha * 255.0F);
				return (*this);
			}

			void operator =(const ColorRgba& c) volatile
			{
				red = uint8(c.red * 255.0F);
				green = uint8(c.green * 255.0F);
				blue = uint8(c.blue * 255.0F);
				alpha = uint8(c.alpha * 255.0F);
			}

			bool operator ==(const Color4U& c) const
			{
				return (GetPackedColor() == c.GetPackedColor());
			}

			bool operator !=(const Color4U& c) const
			{
				return (GetPackedColor() != c.GetPackedColor());
			}
	};


	//# \class	Color4S		Encapsulates a four-component signed integer color.
	//
	//# The $Color4S$ class encapsulates a four-component signed integer color.
	//
	//# \def	class Color4S
	//
	//# \ctor	Color4S();
	//# \ctor	Color4S(int32 r, int32 g, int32 b, int32 a = 0);
	//
	//# \desc
	//# The $Color4S$ class encapsulates a color having signed integer red, green, blue, and alpha
	//# components in the range [&minus;127,&#x202F;+127].
	//#
	//# The default constructor leaves the components of the color undefined.
	//
	//# \also	$@Color4U@$
	//# \also	$@Color2S@$
	//# \also	$@Color2U@$


	class Color4S
	{
		public:

			int8		red;
			int8		green;
			int8		blue;
			int8		alpha;

			inline Color4S() = default;

			Color4S(int32 r, int32 g, int32 b, int32 a = 0)
			{
				red = int8(r);
				green = int8(g);
				blue = int8(b);
				alpha = int8(a);
			}

			Color4S& Set(int32 r, int32 g, int32 b, int32 a = 0)
			{
				red = int8(r);
				green = int8(g);
				blue = int8(b);
				alpha = int8(a);
				return (*this);
			}

			void Set(int32 r, int32 g, int32 b, int32 a = 0) volatile
			{
				red = int8(r);
				green = int8(g);
				blue = int8(b);
				alpha = int8(a);
			}

			Color4S& Clear(void)
			{
				reinterpret_cast<uint32&>(*this) = 0;
				return (*this);
			}

			uint32 GetPackedColor(void) const
			{
				return (reinterpret_cast<const uint32&>(*this));
			}

			Color4S& SetPackedColor(uint32 c)
			{
				reinterpret_cast<uint32&>(*this) = c;
				return (*this);
			}

			void SetPackedColor(uint32 c) volatile
			{
				reinterpret_cast<volatile uint32&>(*this) = c;
			}

			Color4S& operator =(const Color4S& c)
			{
				reinterpret_cast<uint32&>(*this) = c.GetPackedColor();
				return (*this);
			}

			void operator =(const Color4S& c) volatile
			{
				reinterpret_cast<volatile uint32&>(*this) = c.GetPackedColor();
			}

			bool operator ==(const Color4S& c) const
			{
				return (GetPackedColor() == c.GetPackedColor());
			}

			bool operator !=(const Color4S& c) const
			{
				return (GetPackedColor() != c.GetPackedColor());
			}
	};


	struct ConstColor4U
	{
		uint8		red;
		uint8		green;
		uint8		blue;
		uint8		alpha;

		operator const Color4U&(void) const
		{
			return (reinterpret_cast<const Color4U&>(*this));
		}

		const Color4U *operator &(void) const
		{
			return (reinterpret_cast<const Color4U *>(this));
		}

		const Color4U *operator ->(void) const
		{
			return (reinterpret_cast<const Color4U *>(this));
		}
	};


	class Color4H
	{
		public:

			Half		red;
			Half		green;
			Half		blue;
			Half		alpha;

			inline Color4H() = default;

			Color4H(float r, float g, float b, float a = 0.0F)
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
			}

			explicit Color4H(const ColorRgba& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
			}

			Color4H& Set(float r, float g, float b, float a = 0.0F)
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
				return (*this);
			}

			void Set(float r, float g, float b, float a = 0.0F) volatile
			{
				red = r;
				green = g;
				blue = b;
				alpha = a;
			}

			Color4H& Set(const ColorRgba& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
				return (*this);
			}

			void Set(const ColorRgba& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
			}

			Color4H& operator =(const Color4H& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
				return (*this);
			}

			void operator =(const Color4H& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
			}

			Color4H& operator =(const ColorRgba& c)
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
				return (*this);
			}

			void operator =(const ColorRgba& c) volatile
			{
				red = c.red;
				green = c.green;
				blue = c.blue;
				alpha = c.alpha;
			}

			bool operator ==(const Color4H& c) const
			{
				return ((red == c.red) && (green == c.green) && (blue == c.blue) && (alpha == c.alpha));
			}

			bool operator !=(const Color4H& c) const
			{
				return ((red != c.red) || (green != c.green) || (blue != c.blue) || (alpha != c.alpha));
			}
	};


	namespace Color
	{
		TERATHON_API extern const ConstColorRgba black;
		TERATHON_API extern const ConstColorRgba white;
		TERATHON_API extern const ConstColorRgba transparent;
		TERATHON_API extern const ConstColorRgba red;
		TERATHON_API extern const ConstColorRgba green;
		TERATHON_API extern const ConstColorRgba blue;
		TERATHON_API extern const ConstColorRgba yellow;
		TERATHON_API extern const ConstColorRgba cyan;
		TERATHON_API extern const ConstColorRgba magenta;

		TERATHON_API extern const uint8 srgbLinearizationTable[256];
		TERATHON_API extern const uint8 srgbDelinearizationTable[256];
		TERATHON_API extern const float srgbFloatLinearizationTable[256];

		TERATHON_API float Linearize(float color);
		TERATHON_API float Delinearize(float color);
		TERATHON_API ColorRgb Linearize(const ColorRgb& color);
		TERATHON_API ColorRgb Delinearize(const ColorRgb& color);
		TERATHON_API ColorRgba Linearize(const ColorRgba& color);
		TERATHON_API ColorRgba Delinearize(const ColorRgba& color);
	}
}


#endif
