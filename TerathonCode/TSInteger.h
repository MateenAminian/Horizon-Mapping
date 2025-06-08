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


#ifndef TSInteger_h
#define TSInteger_h


//# \component	Math Library
//# \prefix		Math/


#include "TSAlgebra.h"
#include "TSBasic.h"


#define TERATHON_INTEGER2D 1
#define TERATHON_INTEGER3D 1
#define TERATHON_INTEGER4D 1


namespace Terathon
{
	class Integer2D;
	class Integer3D;
	class Integer4D;


	//# \class	Integer2D	Encapsulates a vector containing a pair of integer coordinates.
	//
	//# The $Integer2D$ class encapsulates a vector containing a pair of integer coordinates.
	//
	//# \def	class Integer2D : public Vec2D<TypeInteger2D>
	//
	//# \ctor	Integer2D();
	//# \ctor	Integer2D(int32 i, int32 j);
	//# \ctor	template <typename type> Integer2D(const Vec2D<type>& u);
	//
	//# \param	i	The value of the <i>x</i> coordinate.
	//# \param	j	The value of the <i>y</i> coordinate.
	//# \param	u	Another 2D vector, possibly with a different component type, that is converted to an $Integer2D$.
	//
	//# \desc
	//# The $Integer2D$ class is used to store a vector having two integer components
	//# <i>x</i> and <i>y</i>.
	//#
	//# The default constructor leaves the components of the vector undefined.
	//# If the values $i$ and $j$ are supplied, then they are assigned to the
	//# <i>x</i> and <i>y</i> coordinates of the vector, respectively.
	//
	//# \operator	int32& operator [](machine k);
	//#				Returns a reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0 or 1.
	//
	//# \operator	const int32& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0 or 1.
	//
	//# \operator	Integer2D& operator +=(const Integer2D& v);
	//#				Adds the vector $v$.
	//
	//# \operator	Integer2D& operator -=(const Integer2D& v);
	//#				Subtracts the vector $v$.
	//
	//# \action		bool operator ==(const Integer2D& v1, const Integer2D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are equal.
	//
	//# \action		bool operator !=(const Integer2D& v1, const Integer2D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are not equal.
	//
	//# \action		Integer2D operator +(const Integer2D& v1, const Integer2D& v2) const;
	//#				Returns the componentwise sum of the vectors $v1$ and $v2$.
	//
	//# \action		Integer2D operator -(const Integer2D& v1, const Integer2D& v2) const;
	//#				Returns the componentwise difference of the vectors $v1$ and $v2$.
	//
	//# \action		Integer2D operator *(const Integer2D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been multiplied by $t$.
	//
	//# \action		Integer2D operator /(const Integer2D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been divided by $t$.
	//
	//# \privbase	Vec2D	Vectors use a generic base class to store their components.
	//
	//# \also	$@Integer3D@$
	//# \also	$@Integer4D@$


	//# \function	Integer2D::Set		Sets both components of a vector.
	//
	//# \proto	Integer2D& Set(int32 i, int32 j);
	//
	//# \param	i	The new <i>x</i> coordinate.
	//# \param	j	The new <i>y</i> coordinate.
	//
	//# \desc
	//# The $Set$ function sets the <i>x</i> and <i>y</i> coordinates of a vector to
	//# the values given by the $i$ and $j$ parameters, respectively.
	//#
	//# The return value is a reference to the vector object.


	struct TypeInteger2D
	{
		typedef int32 component_type;
		typedef Integer2D vector2D_type;
	};


	class Integer2D : public Vec2D<TypeInteger2D>
	{
		public:

			inline Integer2D() = default;

			Integer2D(int32 i, int32 j) : Vec2D<TypeInteger2D>(i, j) {}

			template <typename type>
			Integer2D(const Vec2D<type>& u) : Vec2D<TypeInteger2D>(int32(u.x), int32(u.y)) {}

			Integer2D& Set(int32 i, int32 j)
			{
				xy.Set(i, j);
				return (*this);
			}

			Integer2D& operator =(const Integer2D& v)
			{
				xy = v.xy;
				return (*this);
			}

			void operator =(const Integer2D& v) volatile
			{
				xy = v.xy;
			}

			template <typename type_struct, int count, int index_x, int index_y>
			Integer2D& operator =(const Subvec2D<type_struct, count, index_x, index_y>& v)
			{
				xy = v;
				return (*this);
			}

			template <typename type_struct, int count, int index_x, int index_y>
			void operator =(const Subvec2D<type_struct, count, index_x, index_y>& v) volatile
			{
				xy = v;
			}

			Integer2D& operator +=(const Integer2D& v)
			{
				xy += v.xy;
				return (*this);
			}

			template <typename type_struct, int count, int index_x, int index_y>
			Integer2D& operator +=(const Subvec2D<type_struct, count, index_x, index_y>& v)
			{
				xy += v;
				return (*this);
			}

			Integer2D& operator -=(const Integer2D& v)
			{
				xy -= v.xy;
				return (*this);
			}

			template <typename type_struct, int count, int index_x, int index_y>
			Integer2D& operator -=(const Subvec2D<type_struct, count, index_x, index_y>& v)
			{
				xy -= v;
				return (*this);
			}

			Integer2D& operator *=(const Integer2D& v)
			{
				xy *= v.xy;
				return (*this);
			}

			template <typename type_struct, int count, int index_x, int index_y>
			Integer2D& operator *=(const Subvec2D<type_struct, count, index_x, index_y>& v)
			{
				xy *= v;
				return (*this);
			}
	};

	inline Integer2D operator +(const Integer2D& v1, const Integer2D& v2)
	{
		return (Integer2D(v1.x + v2.x, v1.y + v2.y));
	}

	inline Integer2D operator -(const Integer2D& v1, const Integer2D& v2)
	{
		return (Integer2D(v1.x - v2.x, v1.y - v2.y));
	}

	inline Integer2D operator *(const Integer2D& v, int32 t)
	{
		return (Integer2D(v.x * t, v.y * t));
	}

	inline Integer2D operator /(const Integer2D& v, int32 t)
	{
		return (Integer2D(v.x / t, v.y / t));
	}

	inline Integer2D operator <<(const Integer2D& v, uint32 shift)
	{
		return (Integer2D(v.x << shift, v.y << shift));
	}

	inline Integer2D operator >>(const Integer2D& v, uint32 shift)
	{
		return (Integer2D(v.x >> shift, v.y >> shift));
	}

	inline Integer2D Min(const Integer2D& v1, const Integer2D& v2)
	{
		return (Integer2D(Min(v1.x, v2.x), Min(v1.y, v2.y)));
	}

	inline Integer2D Max(const Integer2D& v1, const Integer2D& v2)
	{
		return (Integer2D(Max(v1.x, v2.x), Max(v1.y, v2.y)));
	}


	//# \class	Integer3D	Encapsulates a vector containing a triplet of integer coordinates.
	//
	//# The $Integer3D$ class encapsulates a vector containing a triplet of integer coordinates.
	//
	//# \def	class Integer3D : public Vec3D<TypeInteger3D>
	//
	//# \ctor	Integer3D();
	//# \ctor	Integer3D(int32 i, int32 j, int32 k);
	//# \ctor	template <typename type> Integer3D(const Vec3D<type>& u);
	//
	//# \param	i	The value of the <i>x</i> coordinate.
	//# \param	j	The value of the <i>y</i> coordinate.
	//# \param	k	The value of the <i>z</i> coordinate.
	//# \param	u	Another 3D vector, possibly with a different component type, that is converted to an $Integer3D$.
	//
	//# \desc
	//# The $Integer3D$ class is used to store a vector having three integer components
	//# <i>x</i>, <i>y</i>, and <i>z</i>.
	//#
	//# The default constructor leaves the components of the vector undefined.
	//# If the values $i$, $j$, and $k$ are supplied, then they are assigned to the
	//# <i>x</i>, <i>y</i>, and <i>z</i> coordinates of the vector, respectively.
	//
	//# \operator	int32& operator [](machine k);
	//#				Returns a reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0, 1, or 2.
	//
	//# \operator	const int32& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0, 1, or 2.
	//
	//# \operator	Integer3D& operator +=(const Integer3D& v);
	//#				Adds the vector $v$.
	//
	//# \operator	Integer3D& operator -=(const Integer3D& v);
	//#				Subtracts the vector $v$.
	//
	//# \action		Integer3D operator +(const Integer3D& v1, const Integer3D& v2) const;
	//#				Returns the componentwise sum of the vectors $v1$ and $v2$.
	//
	//# \action		Integer3D operator -(const Integer3D& v1, const Integer3D& v2) const;
	//#				Returns the componentwise difference of the vectors $v1$ and $v2$.
	//
	//# \action		Integer3D operator *(const Integer3D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been multiplied by $t$.
	//
	//# \action		Integer3D operator /(const Integer3D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been divided by $t$.
	//
	//# \action		bool operator ==(const Integer3D& v1, const Integer3D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are equal.
	//
	//# \action		bool operator !=(const Integer3D& v1, const Integer3D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are not equal.
	//
	//# \privbase	Vec3D	Vectors use a generic base class to store their components.
	//
	//# \also	$@Integer2D@$
	//# \also	$@Integer4D@$


	//# \function	Integer3D::Set		Sets all three components of a vector.
	//
	//# \proto	Integer3D& Set(int32 i, int32 j, int32 k);
	//
	//# \param	i	The new <i>x</i> coordinate.
	//# \param	j	The new <i>y</i> coordinate.
	//# \param	k	The new <i>z</i> coordinate.
	//
	//# \desc
	//# The $Set$ function sets the <i>x</i>, <i>y</i>, and <i>z</i> coordinates of a vector to
	//# the values given by the $i$, $j$, and $k$ parameters, respectively.
	//#
	//# The return value is a reference to the vector object.


	struct TypeInteger3D
	{
		typedef int32 component_type;
		typedef Integer2D vector2D_type;
		typedef Integer3D vector3D_type;
	};


	class Integer3D : public Vec3D<TypeInteger3D>
	{
		public:

			inline Integer3D() = default;

			Integer3D(int32 i, int32 j, int32 k) : Vec3D<TypeInteger3D>(i, j, k) {}
			Integer3D(const Integer2D& ij, int32 k) : Vec3D<TypeInteger3D>(ij.x, ij.y, k) {}

			template <typename type>
			Integer3D(const Vec3D<type>& u) : Vec3D<TypeInteger3D>(int32(u.x), int32(u.y), int32(u.z)) {}

			Integer3D& Set(int32 i, int32 j, int32 k)
			{
				xyz.Set(i, j, k);
				return (*this);
			}

			Integer3D& operator =(const Integer3D& v)
			{
				xyz = v.xyz;
				return (*this);
			}

			void operator =(const Integer3D& v) volatile
			{
				xyz = v.xyz;
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z>
			Integer3D& operator =(const Subvec3D<type_struct, anti, count, index_x, index_y, index_z>& v)
			{
				xyz = v;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z>
			void operator =(const Subvec3D<type_struct, anti, count, index_x, index_y, index_z>& v) volatile
			{
				xyz = v;
			}

			Integer3D& operator +=(const Integer3D& v)
			{
				xyz += v.xyz;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z>
			Integer3D& operator +=(const Subvec3D<type_struct, anti, count, index_x, index_y, index_z>& v)
			{
				xyz += v;
				return (*this);
			}

			Integer3D& operator -=(const Integer3D& v)
			{
				xyz -= v.xyz;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z>
			Integer3D& operator -=(const Subvec3D<type_struct, anti, count, index_x, index_y, index_z>& v)
			{
				xyz -= v;
				return (*this);
			}

			Integer3D& operator *=(const Integer3D& v)
			{
				xyz *= v.xyz;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z>
			Integer3D& operator *=(const Subvec3D<type_struct, anti, count, index_x, index_y, index_z>& v)
			{
				xyz *= v;
				return (*this);
			}
	};

	inline Integer3D operator +(const Integer3D& v1, const Integer3D& v2)
	{
		return (Integer3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
	}

	inline Integer3D operator -(const Integer3D& v1, const Integer3D& v2)
	{
		return (Integer3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
	}

	inline Integer3D operator *(const Integer3D& v, int32 t)
	{
		return (Integer3D(v.x * t, v.y * t, v.z * t));
	}

	inline Integer3D operator /(const Integer3D& v, int32 t)
	{
		return (Integer3D(v.x / t, v.y / t, v.z / t));
	}

	inline Integer3D operator <<(const Integer3D& v, uint32 shift)
	{
		return (Integer3D(v.x << shift, v.y << shift, v.z << shift));
	}

	inline Integer3D operator >>(const Integer3D& v, uint32 shift)
	{
		return (Integer3D(v.x >> shift, v.y >> shift, v.z >> shift));
	}

	inline Integer3D Min(const Integer3D& v1, const Integer3D& v2)
	{
		return (Integer3D(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z)));
	}

	inline Integer3D Max(const Integer3D& v1, const Integer3D& v2)
	{
		return (Integer3D(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z)));
	}

	inline Integer3D Cross(const Integer3D& v1, const Integer3D& v2)
	{
		return (Integer3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x));
	}


	//# \class	Integer4D	Encapsulates a vector containing a 4-tuple of integer coordinates.
	//
	//# The $Integer4D$ class encapsulates a vector containing a 4-tuple of integer coordinates.
	//
	//# \def	class Integer4D : public Vec4D<TypeInteger4D>
	//
	//# \ctor	Integer4D();
	//# \ctor	Integer4D(int32 i, int32 j, int32 k, int32 l);
	//# \ctor	template <typename type> Integer4D(const Vec4D<type>& u);
	//
	//# \param	i	The value of the <i>x</i> coordinate.
	//# \param	j	The value of the <i>y</i> coordinate.
	//# \param	k	The value of the <i>z</i> coordinate.
	//# \param	l	The value of the <i>w</i> coordinate.
	//# \param	u	Another 4D vector, possibly with a different component type, that is converted to an $Integer4D$.
	//
	//# \desc
	//# The $Integer4D$ class is used to store a vector having four integer components
	//# <i>x</i>, <i>y</i>, <i>z</i>, and <i>w</i>.
	//#
	//# The default constructor leaves the components of the vector undefined.
	//# If the values $i$, $j$, $k$, and $l$ are supplied, then they are assigned to the
	//# <i>x</i>, <i>y</i>, <i>z</i>, and <i>w</i> coordinates of the vector, respectively.
	//
	//# \operator	int32& operator [](machine k);
	//#				Returns a reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0, 1, 2, or 3.
	//
	//# \operator	const int32& operator [](machine k) const;
	//#				Returns a constant reference to the $k$-th scalar component of a vector.
	//#				The value of $k$ must be 0, 1, 2, or 3.
	//
	//# \operator	Integer4D& operator +=(const Integer4D& v);
	//#				Adds the vector $v$.
	//
	//# \operator	Integer4D& operator -=(const Integer4D& v);
	//#				Subtracts the vector $v$.
	//
	//# \action		Integer4D operator +(const Integer4D& v1, const Integer4D& v2) const;
	//#				Returns the componentwise sum of the vectors $v1$ and $v2$.
	//
	//# \action		Integer4D operator -(const Integer4D& v1, const Integer4D& v2) const;
	//#				Returns the componentwise difference of the vectors $v1$ and $v2$.
	//
	//# \action		Integer4D operator *(const Integer4D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been multiplied by $t$.
	//
	//# \action		Integer4D operator /(const Integer4D& v, int32 t) const;
	//#				Returns a vector for which each component of the vector $v$ has been divided by $t$.
	//
	//# \action		bool operator ==(const Integer4D& v1, const Integer4D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are equal.
	//
	//# \action		bool operator !=(const Integer4D& v1, const Integer4D& v2) const;
	//#				Returns a boolean value indicating whether the two vectors $v1$ and $v2$ are not equal.
	//
	//# \privbase	Vec4D	Vectors use a generic base class to store their components.
	//
	//# \also	$@Integer2D@$
	//# \also	$@Integer3D@$


	//# \function	Integer4D::Set		Sets all four components of a vector.
	//
	//# \proto	Integer3D& Set(int32 i, int32 j, int32 k, int32 l);
	//
	//# \param	i	The new <i>x</i> coordinate.
	//# \param	j	The new <i>y</i> coordinate.
	//# \param	k	The new <i>z</i> coordinate.
	//# \param	l	The new <i>w</i> coordinate.
	//
	//# \desc
	//# The $Set$ function sets the <i>x</i>, <i>y</i>, <i>z</i>, and <i>w</i> coordinates of a vector to
	//# the values given by the $i$, $j$, $k$, and $l$ parameters, respectively.
	//#
	//# The return value is a reference to the vector object.


	struct TypeInteger4D
	{
		typedef int32 component_type;
		typedef Integer2D vector2D_type;
		typedef Integer3D vector3D_type;
		typedef Integer4D vector4D_type;
	};


	class Integer4D : public Vec4D<TypeInteger4D>
	{
		public:

			inline Integer4D() = default;

			Integer4D(int32 i, int32 j, int32 k, int32 l) : Vec4D<TypeInteger4D>(i, j, k, l) {}
			Integer4D(const Integer3D& ijk, int32 l) : Vec4D<TypeInteger4D>(ijk.x, ijk.y, ijk.z, l) {}

			template <typename type>
			Integer4D(const Vec4D<type>& u) : Vec4D<TypeInteger4D>(int32(u.x), int32(u.y), int32(u.z), int32(u.w)) {}

			Integer4D& Set(int32 i, int32 j, int32 k, int32 l)
			{
				xyzw.Set(i, j, k, l);
				return (*this);
			}

			Integer4D& operator =(const Integer4D& v)
			{
				xyzw = v.xyzw;
				return (*this);
			}

			void operator =(const Integer4D& v) volatile
			{
				xyzw = v.xyzw;
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z, int index_w>
			Integer4D& operator =(const Subvec4D<type_struct, anti, count, index_x, index_y, index_z, index_w>& v)
			{
				xyzw = v;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z, int index_w>
			void operator =(const Subvec4D<type_struct, anti, count, index_x, index_y, index_z, index_w>& v) volatile
			{
				xyzw = v;
			}

			Integer4D& operator +=(const Integer4D& v)
			{
				xyzw += v.xyzw;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z, int index_w>
			Integer4D& operator +=(const Subvec4D<type_struct, anti, count, index_x, index_y, index_z, index_w>& v)
			{
				xyzw += v;
				return (*this);
			}

			Integer4D& operator -=(const Integer4D& v)
			{
				xyzw -= v.xyzw;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z, int index_w>
			Integer4D& operator -=(const Subvec4D<type_struct, anti, count, index_x, index_y, index_z, index_w>& v)
			{
				xyzw -= v;
				return (*this);
			}

			Integer4D& operator *=(const Integer4D& v)
			{
				xyzw *= v.xyzw;
				return (*this);
			}

			template <typename type_struct, bool anti, int count, int index_x, int index_y, int index_z, int index_w>
			Integer4D& operator *=(const Subvec4D<type_struct, anti, count, index_x, index_y, index_z, index_w>& v)
			{
				xyzw *= v;
				return (*this);
			}
	};

	inline Integer4D operator +(const Integer4D& v1, const Integer4D& v2)
	{
		return (Integer4D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w));
	}

	inline Integer4D operator -(const Integer4D& v1, const Integer4D& v2)
	{
		return (Integer4D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w));
	}

	inline Integer4D operator *(const Integer4D& v, int32 t)
	{
		return (Integer4D(v.x * t, v.y * t, v.z * t, v.w * t));
	}

	inline Integer4D operator /(const Integer4D& v, int32 t)
	{
		return (Integer4D(v.x / t, v.y / t, v.z / t, v.w / t));
	}

	inline Integer4D operator <<(const Integer4D& v, uint32 shift)
	{
		return (Integer4D(v.x << shift, v.y << shift, v.z << shift, v.w << shift));
	}

	inline Integer4D operator >>(const Integer4D& v, uint32 shift)
	{
		return (Integer4D(v.x >> shift, v.y >> shift, v.z >> shift, v.w >> shift));
	}

	inline Integer4D Min(const Integer4D& v1, const Integer4D& v2)
	{
		return (Integer4D(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w)));
	}

	inline Integer4D Max(const Integer4D& v1, const Integer4D& v2)
	{
		return (Integer4D(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w)));
	}


	typedef int32			fix32;
	typedef uint32			ufix32;
	typedef Integer2D		Fixed2D;
	typedef Integer3D		Fixed3D;
	typedef Integer4D		Fixed4D;
}


#endif
