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


#include "TSBox.h"


using namespace Terathon;


void Box2D::Union(const Box2D& box)
{
	float xmin = Fmin(min.x, box.min.x);
	float ymin = Fmin(min.y, box.min.y);

	float xmax = Fmax(max.x, box.max.x);
	float ymax = Fmax(max.y, box.max.y);

	min.Set(xmin, ymin);
	max.Set(xmax, ymax);
}

void Box2D::Union(const Point2D& p)
{
	min.x = Fmin(min.x, p.x);
	min.y = Fmin(min.y, p.y);
	max.x = Fmax(max.x, p.x);
	max.y = Fmax(max.y, p.y);
}

void Box2D::Intersect(const Box2D& box)
{
	float xmin = Fmax(min.x, box.min.x);
	float ymin = Fmax(min.y, box.min.y);

	float xmax = Fmin(max.x, box.max.x);
	float ymax = Fmin(max.y, box.max.y);

	min.Set(xmin, ymin);
	max.Set(xmax, ymax);
}

void Box2D::Transform(const Transform4D& transform)
{
	Point2D		p[3];

	p[0] = transform * Point2D(min.x, max.y);
	p[1] = transform * Point2D(max.x, max.y);
	p[2] = transform * Point2D(max.x, min.y);

	min = transform * Point2D(min.x, min.y);
	max = min;

	for (machine a = 0; a < 3; a++)
	{
		float x = p[a].x;
		min.x = Fmin(min.x, x);
		max.x = Fmax(max.x, x);

		float y = p[a].y;
		min.y = Fmin(min.y, y);
		max.y = Fmax(max.y, y);
	}
}

void Box2D::Calculate(int32 vertexCount, const Point2D *vertex)
{
	float xmin = vertex[0].x;
	float ymin = vertex[0].y;
	float xmax = xmin;
	float ymax = ymin;

	for (machine a = 1; a < vertexCount; a++)
	{
		const Point2D& v = vertex[a];

		float x = v.x;
		xmin = Fmin(xmin, x);
		xmax = Fmax(xmax, x);

		float y = v.y;
		ymin = Fmin(ymin, y);
		ymax = Fmax(ymax, y);
	}

	min.Set(xmin, ymin);
	max.Set(xmax, ymax);
}

void Box2D::Calculate(const Point2D& p, int32 vertexCount, const Point2D *vertex)
{
	float xmin = p.x;
	float ymin = p.y;
	float xmax = xmin;
	float ymax = ymin;

	for (machine a = 0; a < vertexCount; a++)
	{
		const Point2D& v = vertex[a];

		float x = v.x;
		xmin = Fmin(xmin, x);
		xmax = Fmax(xmax, x);

		float y = v.y;
		ymin = Fmin(ymin, y);
		ymax = Fmax(ymax, y);
	}

	min.Set(xmin, ymin);
	max.Set(xmax, ymax);
}

bool Box2D::Contains(const Point2D& p) const
{
	return ((p.x >= min.x) && (p.x <= max.x) && (p.y >= min.y) && (p.y <= max.y));
}

bool Box2D::Contains(const Box2D& box) const
{
	return ((box.min.x >= min.x) && (box.max.x <= max.x) && (box.min.y >= min.y) && (box.max.y <= max.y));
}

bool Box2D::Intersection(const Box2D& box) const
{
	if ((max.x < box.min.x) || (min.x > box.max.x))
	{
		return (false);
	}

	if ((max.y < box.min.y) || (min.y > box.max.y))
	{
		return (false);
	}

	return (true);
}

Box2D Terathon::Union(const Box2D& box1, const Box2D& box2)
{
	float xmin = Fmin(box1.min.x, box2.min.x);
	float ymin = Fmin(box1.min.y, box2.min.y);

	float xmax = Fmax(box1.max.x, box2.max.x);
	float ymax = Fmax(box1.max.y, box2.max.y);

	return (Box2D(Point2D(xmin, ymin), Point2D(xmax, ymax)));
}

Box2D Terathon::Transform(const Box2D& box, const Transform4D& transform)
{
	Point2D		p[3];

	p[0] = transform * Point2D(box.min.x, box.max.y);
	p[1] = transform * Point2D(box.max.x, box.max.y);
	p[2] = transform * Point2D(box.max.x, box.min.y);

	Point2D pmin = transform * Point2D(box.min.x, box.min.y);
	Point2D pmax = pmin;

	for (machine a = 0; a < 3; a++)
	{
		float x = p[a].x;
		pmin.x = Fmin(pmin.x, x);
		pmax.x = Fmax(pmax.x, x);

		float y = p[a].y;
		pmin.y = Fmin(pmin.y, y);
		pmax.y = Fmax(pmax.y, y);
	}

	return (Box2D(pmin, pmax));
}


#ifdef TERATHON_NO_SIMD

	void Box3D::Union(const Box3D& box)
	{
		float xmin = Fmin(min.x, box.min.x);
		float ymin = Fmin(min.y, box.min.y);
		float zmin = Fmin(min.z, box.min.z);

		float xmax = Fmax(max.x, box.max.x);
		float ymax = Fmax(max.y, box.max.y);
		float zmax = Fmax(max.z, box.max.z);

		min.Set(xmin, ymin, zmin);
		max.Set(xmax, ymax, zmax);
	}

	void Box3D::Union(const Point3D& p)
	{
		min.x = Fmin(min.x, p.x);
		min.y = Fmin(min.y, p.y);
		min.z = Fmin(min.z, p.z);
		max.x = Fmax(max.x, p.x);
		max.y = Fmax(max.y, p.y);
		max.z = Fmax(max.z, p.z);
	}

	void Box3D::Intersect(const Box3D& box)
	{
		float xmin = Fmax(min.x, box.min.x);
		float ymin = Fmax(min.y, box.min.y);
		float zmin = Fmax(min.z, box.min.z);

		float xmax = Fmin(max.x, box.max.x);
		float ymax = Fmin(max.y, box.max.y);
		float zmax = Fmin(max.z, box.max.z);

		min.Set(xmin, ymin, zmin);
		max.Set(xmax, ymax, zmax);
	}

#endif

void Box3D::Transform(const Transform4D& transform)
{
	Point3D		p[7];

	p[0] = transform * Point3D(max.x, min.y, min.z);
	p[1] = transform * Point3D(min.x, max.y, min.z);
	p[2] = transform * Point3D(max.x, max.y, min.z);
	p[3] = transform * Point3D(min.x, min.y, max.z);
	p[4] = transform * Point3D(max.x, min.y, max.z);
	p[5] = transform * Point3D(min.x, max.y, max.z);
	p[6] = transform * max;

	#ifndef TERATHON_NO_SIMD

		vec_float pmin = TransformPoint3D(transform, VecLoadUnaligned(&min.x));
		vec_float pmax = pmin;

		for (machine a = 0; a < 7; a++)
		{
			vec_float v = VecLoadUnaligned(&p[a].x);
			pmin = VecMin(pmin, v);
			pmax = VecMax(pmax, v);
		}

		VecStore3D(pmin, &min.x);
		VecStore3D(pmax, &max.x);

	#else

		min = transform * min;
		max = min;

		for (machine a = 0; a < 7; a++)
		{
			float x = p[a].x;
			min.x = Fmin(min.x, x);
			max.x = Fmax(max.x, x);

			float y = p[a].y;
			min.y = Fmin(min.y, y);
			max.y = Fmax(max.y, y);

			float z = p[a].z;
			min.z = Fmin(min.z, z);
			max.z = Fmax(max.z, z);
		}

	#endif
}

void Box3D::Calculate(int32 vertexCount, const Point3D *vertex)
{
	#ifndef TERATHON_NO_SIMD

		vec_float pmin = VecLoadUnaligned(&vertex[0].x);
		vec_float pmax = pmin;

		for (machine a = 1; a < vertexCount; a++)
		{
			vec_float v = VecLoadUnaligned(&vertex[a].x);
			pmin = VecMin(pmin, v);
			pmax = VecMax(pmax, v);
		}

		VecStore3D(pmin, &min.x);
		VecStore3D(pmax, &max.x);

	#else

		float xmin = vertex[0].x;
		float ymin = vertex[0].y;
		float zmin = vertex[0].z;
		float xmax = xmin;
		float ymax = ymin;
		float zmax = zmin;

		for (machine a = 1; a < vertexCount; a++)
		{
			const Point3D& v = vertex[a];

			float x = v.x;
			xmin = Fmin(xmin, x);
			xmax = Fmax(xmax, x);

			float y = v.y;
			ymin = Fmin(ymin, y);
			ymax = Fmax(ymax, y);

			float z = v.z;
			zmin = Fmin(zmin, z);
			zmax = Fmax(zmax, z);
		}

		min.Set(xmin, ymin, zmin);
		max.Set(xmax, ymax, zmax);

	#endif
}

void Box3D::Calculate(const Point3D& p, int32 vertexCount, const Point3D *vertex)
{
	#ifndef TERATHON_NO_SIMD

		vec_float pmin = VecLoadUnaligned(&p.x);
		vec_float pmax = pmin;

		for (machine a = 0; a < vertexCount; a++)
		{
			vec_float v = VecLoadUnaligned(&vertex[a].x);
			pmin = VecMin(pmin, v);
			pmax = VecMax(pmax, v);
		}

		VecStore3D(pmin, &min.x);
		VecStore3D(pmax, &max.x);

	#else

		float xmin = p.x;
		float ymin = p.y;
		float zmin = p.z;
		float xmax = xmin;
		float ymax = ymin;
		float zmax = zmin;

		for (machine a = 0; a < vertexCount; a++)
		{
			const Point3D& v = vertex[a];

			float x = v.x;
			xmin = Fmin(xmin, x);
			xmax = Fmax(xmax, x);

			float y = v.y;
			ymin = Fmin(ymin, y);
			ymax = Fmax(ymax, y);

			float z = v.z;
			zmin = Fmin(zmin, z);
			zmax = Fmax(zmax, z);
		}

		min.Set(xmin, ymin, zmin);
		max.Set(xmax, ymax, zmax);

	#endif
}

Point3D Box3D::CalculateSupportPoint(const Vector3D& direction, float radius) const
{
	#ifndef TERATHON_NO_SIMD

		Point3D		result;

		vec_float r = VecLoadSmearScalar(&radius);
		vec_float v1 = VecLoadUnaligned(&min.x) - r;
		vec_float v2 = VecLoadUnaligned(&max.x) + r;

		vec_float mask = VecMaskCmplt(VecLoadUnaligned(&direction.x), VecFloatGetZero());
		VecStore3D(VecSelect(v2, v1, mask), &result.x);
		return (result);

	#else

		float x = (direction.x < 0.0F) ? min.x - radius : max.x + radius;
		float y = (direction.y < 0.0F) ? min.y - radius : max.y + radius;
		float z = (direction.z < 0.0F) ? min.z - radius : max.z + radius;
		return (Point3D(x, y, z));

	#endif
}

void Box3D::CalculateOpposingSupportPoints(const Vector3D& direction, float radius, Point3D *support) const
{
	#ifndef TERATHON_NO_SIMD

		vec_float r = VecLoadSmearScalar(&radius);
		vec_float v1 = VecLoadUnaligned(&min.x) - r;
		vec_float v2 = VecLoadUnaligned(&max.x) + r;

		vec_float mask = VecMaskCmplt(VecLoadUnaligned(&direction.x), VecFloatGetZero());
		VecStore3D(VecSelect(v2, v1, mask), &support[0].x);
		VecStore3D(VecSelect(v1, v2, mask), &support[1].x);

	#else

		float xmin = min.x - radius;
		float xmax = max.x + radius;
		float ymin = min.y - radius;
		float ymax = max.y + radius;
		float zmin = min.z - radius;
		float zmax = max.z + radius;

		if (direction.x < 0.0F)
		{
			support[0].x = xmin;
			support[1].x = xmax;
		}
		else
		{
			support[0].x = xmax;
			support[1].x = xmin;
		}

		if (direction.y < 0.0F)
		{
			support[0].y = ymin;
			support[1].y = ymax;
		}
		else
		{
			support[0].y = ymax;
			support[1].y = ymin;
		}

		if (direction.z < 0.0F)
		{
			support[0].z = zmin;
			support[1].z = zmax;
		}
		else
		{
			support[0].z = zmax;
			support[1].z = zmin;
		}

	#endif
}

bool Box3D::ExteriorSphere(const Point3D& center, float radius) const
{
	float x = center.x;
	if ((x < min.x - radius) || (x > max.x + radius))
	{
		return (true);
	}

	float y = center.y;
	if ((y < min.y - radius) || (y > max.y + radius))
	{
		return (true);
	}

	float z = center.z;
	if ((z < min.z - radius) || (z > max.z + radius))
	{
		return (true);
	}

	return (false);
}

bool Box3D::ExteriorSweptSphere(const Point3D& p1, const Point3D& p2, float radius) const
{
	if ((!ExteriorSphere(p1, radius)) || (!ExteriorSphere(p2, radius)))
	{
		return (false);
	}

	Point3D q1 = p1;
	Point3D q2 = p2;

	float f = max.z + radius;
	float d1 = f - q1.z;
	float d2 = f - q2.z;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dz = q2.z - q1.z;
		if (Fabs(dz) > Math::min_float)
		{
			// Move the first endpoint up to the box boundary by interpolating.
			// The z coordinate must be set exactly so that round-off errors in
			// the interpolation calculation do not push the endpoint all the way
			// through an ultra-thin box (maybe with zero thickness), causing the
			// swept sphere to be incorrectly culled in the next step.

			float t = d1 / dz;
			q1.xy = q1.xy + (q2.xy - q1.xy) * t;
			q1.z = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dz = q2.z - q1.z;
		if (Fabs(dz) > Math::min_float)
		{
			float t = d1 / dz;
			q2.xy = q1.xy + (q2.xy - q1.xy) * t;
			q2.z = f;
		}
	}

	f = min.z - radius;
	d1 = q1.z - f;
	d2 = q2.z - f;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dz = q1.z - q2.z;
		if (Fabs(dz) > Math::min_float)
		{
			float t = d1 / dz;
			q1.xy = q1.xy + (q2.xy - q1.xy) * t;
			q1.z = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dz = q1.z - q2.z;
		if (Fabs(dz) > Math::min_float)
		{
			float t = d1 / dz;
			q2.xy = q1.xy + (q2.xy - q1.xy) * t;
			q2.z = f;
		}
	}

	f = max.y + radius;
	d1 = f - q1.y;
	d2 = f - q2.y;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dy = q2.y - q1.y;
		if (Fabs(dy) > Math::min_float)
		{
			float t = d1 / dy;
			q1.xz = q1.xz + (q2.xz - q1.xz) * t;
			q1.y = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dy = q2.y - q1.y;
		if (Fabs(dy) > Math::min_float)
		{
			float t = d1 / dy;
			q2.xz = q1.xz + (q2.xz - q1.xz) * t;
			q2.y = f;
		}
	}

	f = min.y - radius;
	d1 = q1.y - f;
	d2 = q2.y - f;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dy = q1.y - q2.y;
		if (Fabs(dy) > Math::min_float)
		{
			float t = d1 / dy;
			q1.xz = q1.xz + (q2.xz - q1.xz) * t;
			q1.y = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dy = q1.y - q2.y;
		if (Fabs(dy) > Math::min_float)
		{
			float t = d1 / dy;
			q2.xz = q1.xz + (q2.xz - q1.xz) * t;
			q2.y = f;
		}
	}

	f = max.x + radius;
	d1 = f - q1.x;
	d2 = f - q2.x;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dx = q2.x - q1.x;
		if (Fabs(dx) > Math::min_float)
		{
			float t = d1 / dx;
			q1.yz = q1.yz + (q2.yz - q1.yz) * t;
			q1.x = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dx = q2.x - q1.x;
		if (Fabs(dx) > Math::min_float)
		{
			float t = d1 / dx;
			q2.yz = q1.yz + (q2.yz - q1.yz) * t;
			q2.x = f;
		}
	}

	f = min.x - radius;
	d1 = q1.x - f;
	d2 = q2.x - f;

	if (d1 < 0.0F)
	{
		if (d2 < 0.0F)
		{
			return (true);
		}

		float dx = q1.x - q2.x;
		if (Fabs(dx) > Math::min_float)
		{
			float t = d1 / dx;
			q1.yz = q1.yz + (q2.yz - q1.yz) * t;
			q1.x = f;
		}
	}
	else if (d2 < 0.0F)
	{
		float dx = q1.x - q2.x;
		if (Fabs(dx) > Math::min_float)
		{
			float t = d1 / dx;
			q2.yz = q1.yz + (q2.yz - q1.yz) * t;
			q2.x = f;
		}
	}

	return (false);
}

bool Box3D::Contains(const Point3D& p) const
{
	return ((p.x >= min.x) && (p.x <= max.x) && (p.y >= min.y) && (p.y <= max.y) && (p.z >= min.z) && (p.z <= max.z));
}

bool Box3D::Contains(const Box3D& box) const
{
	return ((box.min.x >= min.x) && (box.max.x <= max.x) && (box.min.y >= min.y) && (box.max.y <= max.y) && (box.min.z >= min.z) && (box.max.z <= max.z));
}

bool Box3D::Intersection(const Box3D& box) const
{
	#ifndef TERATHON_NO_SIMD

		if (VecCmpltAny3D(VecLoadUnaligned(&max.x), VecLoadUnaligned(&box.min.x)))
		{
			return (false);
		}

		if (VecCmpgtAny3D(VecLoadUnaligned(&min.x), VecLoadUnaligned(&box.max.x)))
		{
			return (false);
		}

	#else

		if ((max.x < box.min.x) || (min.x > box.max.x))
		{
			return (false);
		}

		if ((max.y < box.min.y) || (min.y > box.max.y))
		{
			return (false);
		}

		if ((max.z < box.min.z) || (min.z > box.max.z))
		{
			return (false);
		}

	#endif

	return (true);
}

Box3D Terathon::Union(const Box3D& box1, const Box3D& box2)
{
	#ifndef TERATHON_NO_SIMD

		vec_float pmin = VecMin(VecLoadUnaligned(&box1.min.x), VecLoadUnaligned(&box2.min.x));
		vec_float pmax = VecMax(VecLoadUnaligned(&box1.max.x), VecLoadUnaligned(&box2.max.x));
		return (Box3D(pmin, pmax));

	#else

		float xmin = Fmin(box1.min.x, box2.min.x);
		float ymin = Fmin(box1.min.y, box2.min.y);
		float zmin = Fmin(box1.min.z, box2.min.z);

		float xmax = Fmax(box1.max.x, box2.max.x);
		float ymax = Fmax(box1.max.y, box2.max.y);
		float zmax = Fmax(box1.max.z, box2.max.z);

		return (Box3D(Point3D(xmin, ymin, zmin), Point3D(xmax, ymax, zmax)));

	#endif
}

Box3D Terathon::Transform(const Box3D& box, const Transform4D& transform)
{
	Point3D		p[7];

	p[0] = transform * Point3D(box.max.x, box.min.y, box.min.z);
	p[1] = transform * Point3D(box.min.x, box.max.y, box.min.z);
	p[2] = transform * Point3D(box.max.x, box.max.y, box.min.z);
	p[3] = transform * Point3D(box.min.x, box.min.y, box.max.z);
	p[4] = transform * Point3D(box.max.x, box.min.y, box.max.z);
	p[5] = transform * Point3D(box.min.x, box.max.y, box.max.z);
	p[6] = transform * box.max;

	#ifndef TERATHON_NO_SIMD

		vec_float pmin = TransformPoint3D(transform, VecLoadUnaligned(&box.min.x));
		vec_float pmax = pmin;

		for (machine a = 0; a < 7; a++)
		{
			vec_float v = VecLoadUnaligned(&p[a].x);
			pmin = VecMin(pmin, v);
			pmax = VecMax(pmax, v);
		}

	#else

		Point3D pmin = transform * box.min;
		Point3D pmax = pmin;

		for (machine a = 0; a < 7; a++)
		{
			float x = p[a].x;
			pmin.x = Fmin(pmin.x, x);
			pmax.x = Fmax(pmax.x, x);

			float y = p[a].y;
			pmin.y = Fmin(pmin.y, y);
			pmax.y = Fmax(pmax.y, y);

			float z = p[a].z;
			pmin.z = Fmin(pmin.z, z);
			pmax.z = Fmax(pmax.z, z);
		}

	#endif

	return (Box3D(pmin, pmax));
}
