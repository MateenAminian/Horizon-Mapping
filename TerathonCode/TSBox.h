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


#ifndef TSBox_h
#define TSBox_h


//# \component	Math Library
//# \prefix		Math/


#include "TSTools.h"
#include "TSMatrix4D.h"


#define TERATHON_BOX2D 1
#define TERATHON_BOX3D 1


namespace Terathon
{
	//# \class	Box2D		Encapsulates a 2D aligned bounding box.
	//
	//# The $Box2D$ class encapsulates a 2D aligned bounding box.
	//
	//# \def	class Box2D : public Range<Point2D>
	//
	//# \ctor	Box2D(const Point2D& pmin, const Point2D& pmax);
	//
	//# \param	pmin	The point at the minimum corner of the bounding box.
	//# \param	pmax	The point at the maximum corner of the bounding box.
	//
	//# \desc
	//# The $Box2D$ class encapsulates the 2D aligned bounding box whose minimum and maximum extents are specified
	//# by the $pmin$ and $pmax$ parameters.
	//
	//# \base	Utilities/Range<Point2D>	An aligned box is implemented as a range of points.
	//
	//# \also	$@Box3D@$


	//# \function	Box2D::Calculate		Calculates a bounding box for a set of 2D vertices.
	//
	//# \proto	void Calculate(int32 vertexCount, const Point2D *vertex);
	//
	//# \param	vertexCount		The number of vertices. This must be at least 1.
	//# \param	vertex			A pointer to the array of vertex positions.
	//
	//# \desc
	//# The $Calculate$ function calculates the aligned bounding box for an array of vertices.


	//# \function	Box2D::Contains		Returns a boolean value indicating whether an aligned box contains a point.
	//
	//# \proto	bool Contains(const Point2D& p) const;
	//
	//# \param	p		The point that is tested for containment.
	//
	//# \desc
	//# The $Contains$ function determines whether the point specified by the $p$ parameter is contained in an
	//# aligned bounding box and returns $true$ if so and $false$ otherwise.
	//
	//# \also	$@Box2D::Intersection@$


	//# \function	Box2D::Intersection		Returns a boolean value indicating whether two aligned boxes intersect.
	//
	//# \proto	bool Intersection(const Box2D& box) const;
	//
	//# \param	box		The aligned bounding box with which intersection is tested.
	//
	//# \desc
	//# The $Intersection$ function determines whether an aligned bounding box intersects the aligned bounding box
	//# specified by the $box$ parameter. If the two boxes intersect, then the return value is $true$. Otherwise,
	//# the return value is $false$.
	//
	//# \also	$@Box2D::Contains@$


	class Box2D : public Range<Point2D>
	{
		public:

			inline Box2D() = default;

			Box2D(const Point2D& pmin, const Point2D& pmax) : Range<Point2D>(pmin, pmax)
			{
			}

			Vector2D GetSize(void) const
			{
				return (max - min);
			}

			Point2D GetCenter(void) const
			{
				return ((min + max) * 0.5F);
			}

			float GetComponent(int32 index) const
			{
				return (reinterpret_cast<const float *>(this)[index]);
			}

			Box2D& operator +=(const Box2D& box)
			{
				min += box.min;
				max += box.max;
				return (*this);
			}

			void Offset(const Vector2D& offset)
			{
				min += offset;
				max += offset;
			}

			void Scale(float scale)
			{
				min *= scale;
				max *= scale;
			}

			void Scale(const Vector2D& scale)
			{
				min *= scale;
				max *= scale;
			}

			void Expand(float expand)
			{
				min.x -= expand;
				min.y -= expand;
				max.x += expand;
				max.y += expand;
			}

			Vector2D IncludePoint(const Point2D& p)
			{
				min.x = Fmin(min.x, p.x);
				min.y = Fmin(min.y, p.y);
				max.x = Fmax(max.x, p.x);
				max.y = Fmax(max.y, p.y);

				return (max - min);
			}

			TERATHON_API void Union(const Box2D& box);
			TERATHON_API void Union(const Point2D& p);
			TERATHON_API void Intersect(const Box2D& box);
			TERATHON_API void Transform(const Transform4D& transform);
			TERATHON_API void Calculate(int32 vertexCount, const Point2D *vertex);
			TERATHON_API void Calculate(const Point2D& p, int32 vertexCount, const Point2D *vertex);

			TERATHON_API bool Contains(const Point2D& p) const;
			TERATHON_API bool Contains(const Box2D& box) const;
			TERATHON_API bool Intersection(const Box2D& box) const;
	};


	inline Box2D operator +(const Box2D& box, const Vector2D& offset)
	{
		return (Box2D(box.min + offset, box.max + offset));
	}

	inline Box2D operator +(const Box2D& box1, const Box2D& box2)
	{
		return (Box2D(box1.min + box2.min, box1.max + box2.max));
	}


	TERATHON_API Box2D Union(const Box2D& box1, const Box2D& box2);
	TERATHON_API Box2D Transform(const Box2D& box, const Transform4D& transform);


	//# \class	Box3D		Encapsulates a 3D aligned bounding box.
	//
	//# The $Box3D$ class encapsulates a 3D aligned bounding box.
	//
	//# \def	class Box3D : public Range<Point3D>
	//
	//# \ctor	Box3D(const Point3D& pmin, const Point3D& pmax);
	//
	//# \param	pmin	The point at the minimum corner of the bounding box.
	//# \param	pmax	The point at the maximum corner of the bounding box.
	//
	//# \desc
	//# The $Box3D$ class encapsulates the 3D aligned bounding box whose minimum and maximum extents are specified
	//# by the $pmin$ and $pmax$ parameters.
	//
	//# \base	Utilities/Range<Point3D>	An aligned box is implemented as a range of points.
	//
	//# \also	$@Box2D@$


	//# \function	Box3D::Calculate		Calculates a bounding box for a set of vertices.
	//
	//# \proto	void Calculate(int32 vertexCount, const Point3D *vertex);
	//
	//# \param	vertexCount		The number of vertices. This must be at least 1.
	//# \param	vertex			A pointer to the array of vertex positions.
	//
	//# \desc
	//# The $Calculate$ function calculates the aligned bounding box for an array of vertices.


	//# \function	Box3D::Contains		Returns a boolean value indicating whether an aligned box contains a point.
	//
	//# \proto	bool Contains(const Point3D& p) const;
	//
	//# \param	p		The point that is tested for containment.
	//
	//# \desc
	//# The $Contains$ function determines whether the point specified by the $p$ parameter is contained in an
	//# aligned bounding box and returns $true$ if so and $false$ otherwise.
	//
	//# \also	$@Box3D::Intersection@$


	//# \function	Box3D::Intersection		Returns a boolean value indicating whether two aligned boxes intersect.
	//
	//# \proto	bool Intersection(const Box3D& box) const;
	//
	//# \param	box		The aligned bounding box with which intersection is tested.
	//
	//# \desc
	//# The $Intersection$ function determines whether an aligned bounding box intersects the aligned bounding box
	//# specified by the $box$ parameter. If the two boxes intersect, then the return value is $true$. Otherwise,
	//# the return value is $false$.
	//
	//# \also	$@Box3D::Contains@$


	class Box3D : public Range<Point3D>
	{
		public:

			inline Box3D() = default;

			Box3D(const Point3D& pmin, const Point3D& pmax) : Range<Point3D>(pmin, pmax)
			{
			}

			#ifndef TERATHON_NO_SIMD

				Box3D(vec_float pmin, vec_float pmax)
				{
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
				}

				Box3D& Set(vec_float pmin, vec_float pmax)
				{
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
					return (*this);
				}

				using Range<Point3D>::Set;

			#endif

			Vector3D GetSize(void) const
			{
				return (max - min);
			}

			Point3D GetCenter(void) const
			{
				return ((min + max) * 0.5F);
			}

			float GetComponent(int32 index) const
			{
				return (reinterpret_cast<const float *>(this)[index]);
			}

			Box3D& operator +=(const Box3D& box)
			{
				min += box.min;
				max += box.max;
				return (*this);
			}

			void Offset(const Vector3D& offset)
			{
				min += offset;
				max += offset;
			}

			void Scale(float scale)
			{
				min *= scale;
				max *= scale;
			}

			void Scale(const Vector3D& scale)
			{
				min *= scale;
				max *= scale;
			}

			void Expand(float expand)
			{
				min.x -= expand;
				min.y -= expand;
				min.z -= expand;
				max.x += expand;
				max.y += expand;
				max.z += expand;
			}

			Vector3D IncludePoint(const Point3D& p)
			{
				min.x = Fmin(min.x, p.x);
				min.y = Fmin(min.y, p.y);
				min.z = Fmin(min.z, p.z);
				max.x = Fmax(max.x, p.x);
				max.y = Fmax(max.y, p.y);
				max.z = Fmax(max.z, p.z);

				return (max - min);
			}

			#ifndef TERATHON_NO_SIMD

				vec_float IncludePoint(vec_float p)
				{
					vec_float pmin = VecMin(VecLoadUnaligned(&min.x), p);
					vec_float pmax = VecMax(VecLoadUnaligned(&max.x), p);
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
					return (pmax - pmin);
				}

				void Union(const Box3D& box)
				{
					vec_float pmin = VecMin(VecLoadUnaligned(&min.x), VecLoadUnaligned(&box.min.x));
					vec_float pmax = VecMax(VecLoadUnaligned(&max.x), VecLoadUnaligned(&box.max.x));
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
				}

				void Union(const Point3D& p)
				{
					vec_float point = VecLoadUnaligned(&p.x);
					vec_float pmin = VecMin(VecLoadUnaligned(&min.x), point);
					vec_float pmax = VecMax(VecLoadUnaligned(&max.x), point);
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
				}

				void Intersect(const Box3D& box)
				{
					vec_float pmin = VecMax(VecLoadUnaligned(&min.x), VecLoadUnaligned(&box.min.x));
					vec_float pmax = VecMin(VecLoadUnaligned(&max.x), VecLoadUnaligned(&box.max.x));
					VecStore3D(pmin, &min.x);
					VecStore3D(pmax, &max.x);
				}

			#else

				TERATHON_API void Union(const Box3D& box);
				TERATHON_API void Union(const Point3D& p);
				TERATHON_API void Intersect(const Box3D& box);

			#endif

			TERATHON_API void Transform(const Transform4D& transform);
			TERATHON_API void Calculate(int32 vertexCount, const Point3D *vertex);
			TERATHON_API void Calculate(const Point3D& p, int32 vertexCount, const Point3D *vertex);

			TERATHON_API Point3D CalculateSupportPoint(const Vector3D& direction, float radius) const;
			TERATHON_API void CalculateOpposingSupportPoints(const Vector3D& direction, float radius, Point3D *support) const;

			TERATHON_API bool ExteriorSphere(const Point3D& center, float radius) const;
			TERATHON_API bool ExteriorSweptSphere(const Point3D& p1, const Point3D& p2, float radius) const;

			TERATHON_API bool Contains(const Point3D& p) const;
			TERATHON_API bool Contains(const Box3D& box) const;
			TERATHON_API bool Intersection(const Box3D& box) const;
	};


	inline Box3D operator +(const Box3D& box, const Vector3D& offset)
	{
		return (Box3D(box.min + offset, box.max + offset));
	}

	inline Box3D operator +(const Box3D& box1, const Box3D& box2)
	{
		return (Box3D(box1.min + box2.min, box1.max + box2.max));
	}


	TERATHON_API Box3D Union(const Box3D& box1, const Box3D& box2);
	TERATHON_API Box3D Transform(const Box3D& box, const Transform4D& transform);
}


#endif
