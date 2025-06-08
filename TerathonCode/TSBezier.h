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


#ifndef TSBezier_h
#define TSBezier_h


//# \component	Math Library
//# \prefix		Math/


#include "TSBox.h"


#define TERATHON_BEZIER 1


namespace Terathon
{
	//# \class	QuadraticBezier2D		Encapsulates a 2D quadratic B&eacute;zier curve.
	//
	//# The $QuadraticBezier2D$ class encapsulates a 2D quadratic B&eacute;zier curve, which consists of three 2D control points.
	//
	//# \def	class QuadraticBezier2D
	//
	//# \ctor	QuadraticBezier2D();
	//# \ctor	QuadraticBezier2D(const Point2D& a, const Point2D& b, const Point2D& c);
	//
	//# \param	a,b,c		The control points of the B&eacute;zier curve.
	//
	//# \desc
	//# The $QuadraticBezier2D$ class encapsulates the three control points of a 2D quadratic B&eacute;zier curve.
	//#
	//# The default constructor leaves the control points of the B&eacute;zier curve undefined.
	//
	//# \also	$@Point2D@$


	class QuadraticBezier2D
	{
		public:

			Point2D		p[3];

			inline QuadraticBezier2D() = default;

			QuadraticBezier2D(const Point2D& a, const Point2D& b, const Point2D& c)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
			}

			QuadraticBezier2D(const QuadraticBezier2D& bezier)
			{
				p[0] = bezier.p[0];
				p[1] = bezier.p[1];
				p[2] = bezier.p[2];
			}

			QuadraticBezier2D& Set(const Point2D& a, const Point2D& b, const Point2D& c)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				return (*this);
			}

			bool operator ==(const QuadraticBezier2D& bezier) const
			{
				return ((p[0] == bezier.p[0]) && (p[1] == bezier.p[1]) && (p[2] == bezier.p[2]));
			}

			bool operator !=(const QuadraticBezier2D& bezier) const
			{
				return ((p[0] != bezier.p[0]) || (p[1] != bezier.p[1]) || (p[2] != bezier.p[2]));
			}

			Point2D CalculatePosition(float t) const
			{
				float u = 1.0F - t;
				return (p[0] * (u * u) + p[1] * (t * u * 2.0F) + p[2] * (t * t));
			}

			Vector2D CalculateTangent(float t) const
			{
				// Derivative is scaled by 1/2.

				return (p[0] * (t - 1.0F) + p[1] * (1.0F - t * 2.0F) + p[2] * t);
			}

			TERATHON_API Range<float> CalculateBoundsX(void) const;
			TERATHON_API Range<float> CalculateBoundsY(void) const;
			TERATHON_API Box2D CalculateBoundingBox(void) const;
			TERATHON_API QuadraticBezier2D CalculateSubcurve(float t1, float t2) const;
			TERATHON_API float CalculateCurveLength(float t1, float t2) const;
			TERATHON_API Point2D CalculateSupport(const Vector2D& v) const;
	};


	class QuadraticBezier3D
	{
		public:

			Point3D		p[3];

			inline QuadraticBezier3D() = default;

			QuadraticBezier3D(const Point3D& a, const Point3D& b, const Point3D& c)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
			}

			QuadraticBezier3D(const QuadraticBezier3D& bezier)
			{
				p[0] = bezier.p[0];
				p[1] = bezier.p[1];
				p[2] = bezier.p[2];
			}

			QuadraticBezier3D& Set(const Point3D& a, const Point3D& b, const Point3D& c)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				return (*this);
			}

			bool operator ==(const QuadraticBezier3D& bezier) const
			{
				return ((p[0] == bezier.p[0]) && (p[1] == bezier.p[1]) && (p[2] == bezier.p[2]));
			}

			bool operator !=(const QuadraticBezier3D& bezier) const
			{
				return ((p[0] != bezier.p[0]) || (p[1] != bezier.p[1]) || (p[2] != bezier.p[2]));
			}

			Point3D CalculatePosition(float t) const
			{
				float u = 1.0F - t;
				return (p[0] * (u * u) + p[1] * (t * u * 2.0F) + p[2] * (t * t));
			}

			Vector3D CalculateTangent(float t) const
			{
				// Derivative is scaled by 1/2.

				return (p[0] * (t - 1.0F) + p[1] * (1.0F - t * 2.0F) + p[2] * t);
			}

			TERATHON_API Box3D CalculateBoundingBox(void) const;
			TERATHON_API QuadraticBezier3D CalculateSubcurve(float t1, float t2) const;
	};


	//# \class	CubicBezier2D		Encapsulates a 2D cubic B&eacute;zier curve.
	//
	//# The $CubicBezier2D$ class encapsulates a 2D cubic B&eacute;zier curve, which consists of four 2D control points.
	//
	//# \def	class CubicBezier2D
	//
	//# \ctor	CubicBezier2D();
	//# \ctor	CubicBezier2D(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d);
	//
	//# \param	a,b,c,d		The control points of the B&eacute;zier curve.
	//
	//# \desc
	//# The $CubicBezier2D$ class encapsulates the four control points of a 2D cubic B&eacute;zier curve.
	//#
	//# The default constructor leaves the control points of the B&eacute;zier curve undefined.
	//
	//# \also	$@Point2D@$


	//# \function	CubicBezier2D::CalculateQuadraticApproximation		Approximates a cubic B&eacute;zier curve with a set of quadratic B&eacute;zier curves.
	//
	//# \proto	int32 CalculateQuadraticApproximation(float tolerance, QuadraticBezier2D *curveArray) const;
	//
	//# \param	tolerance		The approximate maximum distance allowed between the input cubic curve and the output set of quadratic curves.
	//# \param	curveArray		A pointer to an array where the quadratic B&eacute;zier curves are returned. This array must be large enough to hold 12 curves.
	//
	//# \desc
	//# The $CalculateQuadraticApproximation$ function generates between one and twelve quadratic B&eacute;zier curves that approximate a single
	//# cubic B&eacute;zier curve within a given tolerance.
	//#
	//# The return value is the number of quadratic curves that were generated, which is never greater that $kMaxQuadraticCurveCount$, defined to be 12.
	//# The maximum number of quadratic curves is generated only in extreme cases, and the typical number is around three or four.


	class CubicBezier2D
	{
		public:

			enum {kMaxQuadraticCurveCount = 12};

			Point2D		p[4];

			inline CubicBezier2D() = default;

			CubicBezier2D(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				p[3] = d;
			}

			CubicBezier2D(const CubicBezier2D& bezier)
			{
				p[0] = bezier.p[0];
				p[1] = bezier.p[1];
				p[2] = bezier.p[2];
				p[3] = bezier.p[3];
			}

			CubicBezier2D& Set(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				p[3] = d;
				return (*this);
			}

			bool operator ==(const CubicBezier2D& bezier) const
			{
				return ((p[0] == bezier.p[0]) && (p[1] == bezier.p[1]) && (p[2] == bezier.p[2]) && (p[3] == bezier.p[3]));
			}

			bool operator !=(const CubicBezier2D& bezier) const
			{
				return ((p[0] != bezier.p[0]) || (p[1] != bezier.p[1]) || (p[2] != bezier.p[2]) || (p[3] != bezier.p[3]));
			}

			Point2D CalculatePosition(float t) const
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				return (p[0] * (u2 * u) + p[1] * (u2 * t * 3.0F) + p[2] * (u * t2 * 3.0F) + p[3] * (t2 * t));
			}

			Vector2D CalculateTangent(float t) const
			{
				// Derivative is scaled by 1/3.

				float t2 = t * t;
				return (p[0] * (-1.0F + t * 2.0F - t2) - p[1] * (-1.0F + t * 4.0F - t2 * 3.0F) + p[2] * (t * 2.0F - t2 * 3.0F) + p[3] * t2);
			}

			TERATHON_API Box2D CalculateBoundingBox(void) const;
			TERATHON_API CubicBezier2D CalculateSubcurve(float t1, float t2) const;
			TERATHON_API int32 CalculateQuadraticApproximation(float tolerance, QuadraticBezier2D *curveArray) const;
	};


	class CubicBezier3D
	{
		public:

			Point3D		p[4];

			inline CubicBezier3D() = default;

			CubicBezier3D(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				p[3] = d;
			}

			CubicBezier3D(const CubicBezier3D& bezier)
			{
				p[0] = bezier.p[0];
				p[1] = bezier.p[1];
				p[2] = bezier.p[2];
				p[3] = bezier.p[3];
			}

			CubicBezier3D& Set(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d)
			{
				p[0] = a;
				p[1] = b;
				p[2] = c;
				p[3] = d;
				return (*this);
			}

			bool operator ==(const CubicBezier3D& bezier) const
			{
				return ((p[0] == bezier.p[0]) && (p[1] == bezier.p[1]) && (p[2] == bezier.p[2]) && (p[3] == bezier.p[3]));
			}

			bool operator !=(const CubicBezier3D& bezier) const
			{
				return ((p[0] != bezier.p[0]) || (p[1] != bezier.p[1]) || (p[2] != bezier.p[2]) || (p[3] != bezier.p[3]));
			}

			Point3D CalculatePosition(float t) const
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				return (p[0] * (u2 * u) + p[1] * (u2 * t * 3.0F) + p[2] * (u * t2 * 3.0F) + p[3] * (t2 * t));
			}

			Vector3D CalculateTangent(float t) const
			{
				// Derivative is scaled by 1/3.

				float t2 = t * t;
				return (p[0] * (-1.0F + t * 2.0F - t2) - p[1] * (-1.0F + t * 4.0F - t2 * 3.0F) + p[2] * (t * 2.0F - t2 * 3.0F) + p[3] * t2);
			}

			TERATHON_API Box3D CalculateBoundingBox(void) const;
			TERATHON_API CubicBezier3D CalculateSubcurve(float t1, float t2) const;
	};


	inline QuadraticBezier2D operator *(const Transform4D& transform, const QuadraticBezier2D& curve)
	{
		return (QuadraticBezier2D(transform * curve.p[0], transform * curve.p[1], transform * curve.p[2]));
	}

	inline QuadraticBezier3D operator *(const Transform4D& transform, const QuadraticBezier3D& curve)
	{
		return (QuadraticBezier3D(transform * curve.p[0], transform * curve.p[1], transform * curve.p[2]));
	}

	inline CubicBezier2D operator *(const Transform4D& transform, const CubicBezier2D& curve)
	{
		return (CubicBezier2D(transform * curve.p[0], transform * curve.p[1], transform * curve.p[2], transform * curve.p[3]));
	}

	inline CubicBezier3D operator *(const Transform4D& transform, const CubicBezier3D& curve)
	{
		return (CubicBezier3D(transform * curve.p[0], transform * curve.p[1], transform * curve.p[2], transform * curve.p[3]));
	}
}


#endif
