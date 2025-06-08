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


#include "TSBezier.h"


using namespace Terathon;


Range<float> QuadraticBezier2D::CalculateBoundsX(void) const
{
	float xmin = Fmin(p[0].x, p[2].x);
	float xmax = Fmax(p[0].x, p[2].x);

	float t = (p[0].x - p[1].x) / (p[0].x - p[1].x * 2.0F + p[2].x);
	if ((t > 0.0F) && (t < 1.0F))
	{
		float u = 1.0F - t;
		float x = p[0].x * (u * u) + p[1].x * (t * u * 2.0F) + p[2].x * (t * t);
		xmin = Fmin(xmin, x);
		xmax = Fmax(xmax, x);
	}

	return (Range<float>(xmin, xmax));
}

Range<float> QuadraticBezier2D::CalculateBoundsY(void) const
{
	float ymin = Fmin(p[0].y, p[2].y);
	float ymax = Fmax(p[0].y, p[2].y);

	float t = (p[0].y - p[1].y) / (p[0].y - p[1].y * 2.0F + p[2].y);
	if ((t > 0.0F) && (t < 1.0F))
	{
		float u = 1.0F - t;
		float y = p[0].y * (u * u) + p[1].y * (t * u * 2.0F) + p[2].y * (t * t);
		ymin = Fmin(ymin, y);
		ymax = Fmax(ymax, y);
	}

	return (Range<float>(ymin, ymax));
}

Box2D QuadraticBezier2D::CalculateBoundingBox(void) const
{
	Range<float> bx = CalculateBoundsX();
	Range<float> by = CalculateBoundsY();
	return (Box2D(Point2D(bx.min, by.min), Point2D(bx.max, by.max)));
}

QuadraticBezier2D QuadraticBezier2D::CalculateSubcurve(float t1, float t2) const
{
	Point2D a = CalculatePosition(t1);
	Point2D c = CalculatePosition(t2);
	Point2D b = a + CalculateTangent(t1) * (t2 - t1);

	return (QuadraticBezier2D(a, b, c));
}

namespace
{
	float CalculateDifferentialLength(const QuadraticBezier2D& curve, float t)
	{
		float u = 1.0F - t;
		float x = (curve.p[2].x - curve.p[1].x) * t + (curve.p[1].x - curve.p[0].x) * u;
		float y = (curve.p[2].y - curve.p[1].y) * t + (curve.p[1].y - curve.p[0].y) * u;
		return (Sqrt(x * x + y * y) * 2.0F);
	}
}

float QuadraticBezier2D::CalculateCurveLength(float t1, float t2) const
{
	float dt = (t2 - t1) * 0.5F;
	float r = dt * Math::sqrt_3_over_3;
	float s = (t1 + t2) * 0.5F;

	float a = CalculateDifferentialLength(*this, s + r);
	float b = CalculateDifferentialLength(*this, s - r);
	return ((a + b) * dt);
}

Point2D QuadraticBezier2D::CalculateSupport(const Vector2D& v) const
{
	float d0 = Dot(p[0], v);
	float d1 = Dot(p[1], v);
	float d2 = Dot(p[2], v);

	float t = (d0 - d1) / (d0 - d1 * 2.0F + d2);
	return (CalculatePosition(t));
}


Box3D QuadraticBezier3D::CalculateBoundingBox(void) const
{
	Box3D box(p[0], p[0]);
	box.Union(p[2]);

	float tx = (p[0].x - p[1].x) / (p[0].x - p[1].x * 2.0F + p[2].x);
	if ((tx > 0.0F) && (tx < 1.0F))
	{
		float ux = 1.0F - tx;
		float x = p[0].x * (ux * ux) + p[1].x * (tx * ux * 2.0F) + p[2].x * (tx * tx);
		box.min.x = Fmin(box.min.x, x);
		box.max.x = Fmax(box.max.x, x);
	}

	float ty = (p[0].y - p[1].y) / (p[0].y - p[1].y * 2.0F + p[2].y);
	if ((ty > 0.0F) && (ty < 1.0F))
	{
		float uy = 1.0F - ty;
		float y = p[0].y * (uy * uy) + p[1].y * (ty * uy * 2.0F) + p[2].y * (ty * ty);
		box.min.y = Fmin(box.min.y, y);
		box.max.y = Fmax(box.max.y, y);
	}

	float tz = (p[0].z - p[1].z) / (p[0].z - p[1].z * 2.0F + p[2].z);
	if ((tz > 0.0F) && (tz < 1.0F))
	{
		float uz = 1.0F - tz;
		float z = p[0].z * (uz * uz) + p[1].z * (tz * uz * 2.0F) + p[2].z * (tz * tz);
		box.min.z = Fmin(box.min.z, z);
		box.max.z = Fmax(box.max.z, z);
	}

	return (box);
}

QuadraticBezier3D QuadraticBezier3D::CalculateSubcurve(float t1, float t2) const
{
	Point3D a = CalculatePosition(t1);
	Point3D c = CalculatePosition(t2);
	Point3D b = a + CalculateTangent(t1) * (t2 - t1);

	return (QuadraticBezier3D(a, b, c));
}


Box2D CubicBezier2D::CalculateBoundingBox(void) const
{
	Box2D box(p[0], p[0]);
	box.Union(p[3]);

	for (machine i = 0; i < 2; i++)
	{
		float a = p[3][i] - p[0][i] + (p[1][i] - p[2][i]) * 3.0F;
		float b = p[1][i] * 2.0F - p[0][i] - p[2][i];
		float c = p[1][i] - p[0][i];

		if (Fabs(a) > Math::min_float)
		{
			float d = Sqrt(b * b - a * c);
			a = 1.0F / a;

			float t = (b - d) * a;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}

			t = (b + d) * a;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}
		}
		else if (Fabs(b) > Math::min_float)
		{
			float t = c / b * 0.5F;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}
		}
	}

	return (box);
}

CubicBezier2D CubicBezier2D::CalculateSubcurve(float t1, float t2) const
{
	Point2D a = CalculatePosition(t1);
	Point2D d = CalculatePosition(t2);

	float dt = t2 - t1;
	Point2D b = a + CalculateTangent(t1) * dt;
	Point2D c = d - CalculateTangent(t2) * dt;

	return (CubicBezier2D(a, b, c, d));
}

namespace
{
	QuadraticBezier2D CalculateQuadraticBezier2D(const CubicBezier2D& cubicCurve)
	{
		constexpr float kDoubleEpsilon = 1.0F / 4194304.0F;

		// Calculate a quadratic Bézier curve that has the same position and
		// tangent at the first and last control points of the cubic Bézier curve.

		Vector2D v1 = cubicCurve.p[1] - cubicCurve.p[0];
		if (SquaredMag(v1) < kDoubleEpsilon)
		{
			v1 = cubicCurve.p[2] - cubicCurve.p[0];
		}

		Vector2D v2 = cubicCurve.p[2] - cubicCurve.p[3];
		if (SquaredMag(v2) < kDoubleEpsilon)
		{
			v2 = cubicCurve.p[1] - cubicCurve.p[3];
		}

		// Calculate homogeneous point where tangent lines intersect.

		Vector3D q = Bivector3D(cubicCurve.p[0], v1) ^ Bivector3D(cubicCurve.p[3], v2);
		return (QuadraticBezier2D(cubicCurve.p[0], Point2D::origin + q.xy / q.z, cubicCurve.p[3]));
	}

	float CalculateCurveDifference(const CubicBezier2D& cubicCurve, const QuadraticBezier2D& quadraticCurve)
	{
		// Sample both curves and calculate an approximation of the area between them.
		// Divide this by the average approximate length of the two curves to arrive at
		// a measure of the distance between them.

		float cubicLength = 0.0F;
		float quadraticLength = 0.0F;
		float cubicArea = 0.0F;
		float quadraticArea = 0.0F;

		Point2D c0 = cubicCurve.p[0];
		Point2D q0 = quadraticCurve.p[0];

		for (machine i = 1; i <= 8; i++)
		{
			float t = (float) i * 0.125F;
			Point2D c1 = cubicCurve.CalculatePosition(t);
			Point2D q1 = quadraticCurve.CalculatePosition(t);

			cubicLength += Magnitude(c1 - c0);
			quadraticLength += Magnitude(q1 - q0);
			cubicArea += c0.x * c1.y - c0.y * c1.x;
			quadraticArea += q0.x * q1.y - q0.y * q1.x;

			c0 = c1;
			q0 = q1;
		}

		// The area and sum of lengths both need to be divided by two, so they cancel out.

		return (Fabs(cubicArea - quadraticArea) / (cubicLength + quadraticLength));
	}

	int32 ApproximateCubicCurve(const CubicBezier2D& cubicCurve, float tolerance, QuadraticBezier2D *curveArray)
	{
		// Approximate a cubic Bézier curve having nonvanishing and nonextremal
		// curvature in (0,1) with 1, 2, or 3 quadratic Bézier curves.

		CubicBezier2D	subcurve[3];

		curveArray[0] = CalculateQuadraticBezier2D(cubicCurve);
		if (CalculateCurveDifference(cubicCurve, curveArray[0]) < tolerance)
		{
			return (1);
		}

		Point2D p0 = cubicCurve.CalculatePosition(0.5F);
		Vector2D t0 = cubicCurve.CalculateTangent(0.5F) * 0.5F;

		subcurve[0].p[0] = cubicCurve.p[0];
		subcurve[0].p[1] = cubicCurve.p[0] + cubicCurve.CalculateTangent(0.0F) * 0.5F;
		subcurve[0].p[2] = p0 - t0;
		subcurve[0].p[3] = p0;

		subcurve[1].p[0] = p0;
		subcurve[1].p[1] = p0 + t0;
		subcurve[1].p[2] = cubicCurve.p[3] - cubicCurve.CalculateTangent(1.0F) * 0.5F;
		subcurve[1].p[3] = cubicCurve.p[3];

		curveArray[0] = CalculateQuadraticBezier2D(subcurve[0]);
		curveArray[1] = CalculateQuadraticBezier2D(subcurve[1]);
		if (Fmax(CalculateCurveDifference(subcurve[0], curveArray[0]), CalculateCurveDifference(subcurve[1], curveArray[1])) < tolerance)
		{
			return (2);
		}

		p0 = cubicCurve.CalculatePosition(0.3333333F);
		t0 = cubicCurve.CalculateTangent(0.3333333F) * 0.3333333F;
		Point2D p1 = cubicCurve.CalculatePosition(0.6666667F);
		Vector2D t1 = cubicCurve.CalculateTangent(0.6666667F) * 0.3333333F;

		subcurve[0].p[0] = cubicCurve.p[0];
		subcurve[0].p[1] = cubicCurve.p[0] + cubicCurve.CalculateTangent(0.0F) * 0.3333333F;
		subcurve[0].p[2] = p0 - t0;
		subcurve[0].p[3] = p0;

		subcurve[1].p[0] = p0;
		subcurve[1].p[1] = p0 + t0;
		subcurve[1].p[2] = p1 - t1;
		subcurve[1].p[3] = p1;

		subcurve[2].p[0] = p1;
		subcurve[2].p[1] = p1 + t1;
		subcurve[2].p[2] = cubicCurve.p[3] - cubicCurve.CalculateTangent(1.0F) * 0.3333333F;
		subcurve[2].p[3] = cubicCurve.p[3];

		curveArray[0] = CalculateQuadraticBezier2D(subcurve[0]);
		curveArray[1] = CalculateQuadraticBezier2D(subcurve[1]);
		curveArray[2] = CalculateQuadraticBezier2D(subcurve[2]);
		return (3);
	};
}

int32 CubicBezier2D::CalculateQuadraticApproximation(float tolerance, QuadraticBezier2D *curveArray) const
{
	CubicBezier2D		cubicSubcurve[4];

	constexpr float kCurveEpsilon = 1.0F / 65536.0F;
	constexpr float kParamEpsilon = 1.0F / 64.0F;

	float xmin = Fmin(p[0].x, p[1].x, p[2].x, p[3].x);
	float xmax = Fmax(p[0].x, p[1].x, p[2].x, p[3].x);
	float ymin = Fmin(p[0].y, p[1].y, p[2].y, p[3].y);
	float ymax = Fmax(p[0].y, p[1].y, p[2].y, p[3].y);

	Vector2D size(xmax - xmin, ymax - ymin);
	if (Fmin(size.x, size.y) < kCurveEpsilon)
	{
		// The curve is a straight horizontal or vertical line.

		curveArray[0].p[0] = p[0];
		curveArray[0].p[1] = p[3];
		curveArray[0].p[2] = p[3];
		return (1);
	}

	float scale = Fmax(size.x, size.y);
	float inverseScale = 1.0F / scale;
	CubicBezier2D normCurve(Point2D::origin, Point2D::origin + (p[1] - p[0]) * inverseScale, Point2D::origin + (p[2] - p[0]) * inverseScale, Point2D::origin + (p[3] - p[0]) * inverseScale);

	// Check whether the curve is already quadratic because the cubic term is zero.

	Vector2D cubicPoint = normCurve.p[3] + (normCurve.p[1] - normCurve.p[2]) * 3.0F;
	if (Fmax(Fabs(cubicPoint.x), Fabs(cubicPoint.y)) < kCurveEpsilon)
	{
		curveArray[0].p[0] = p[0];
		curveArray[0].p[1] = p[1] * 1.5F - p[0] * 0.5F;
		curveArray[0].p[2] = p[3];
		return (1);
	}

	// Calculate the curvature function coefficients. These are identically zero for a straight line.
	// We can't just check whether the cubic and quadratic terms of the curve itself are zero because
	// the quadratic term will not be zero for a straight line that is followed at a nonlinear speed.

	float f12 = normCurve.p[1].x * normCurve.p[2].y - normCurve.p[2].x * normCurve.p[1].y;
	float f31 = normCurve.p[3].x * normCurve.p[1].y - normCurve.p[1].x * normCurve.p[3].y;
	float f23 = normCurve.p[2].x * normCurve.p[3].y - normCurve.p[3].x * normCurve.p[2].y;

	float ka = f23 + f31 * 2.0F + f12 * 3.0F;
	float kb = f12 * -3.0F - f31;
	float kc = f12;

	if (Fmax(Fabs(ka), Fabs(kb), Fabs(kc)) < kCurveEpsilon)
	{
		curveArray[0].p[0] = p[0];
		curveArray[0].p[1] = p[3];
		curveArray[0].p[2] = p[3];
		return (1);
	}

	// The curve really is cubic.

	int32 subcurveCount = 1;
	const CubicBezier2D *subcurveArray = &normCurve;

	if (Fabs(ka) > kCurveEpsilon)
	{
		float splitParam[3] = {0.0F, 0.0F, 0.0F};

		// If the extremum of the curvature function falls in (0,1),
		// then subdivide the curve at that point.

		float ke = 1.0F / ka;
		splitParam[1] = -0.5F * kb * ke;

		// Find the zeros of the curvature function. These are the inflection points.
		// Subdivide the curve at these points if either falls in (0,1).

		float kd = kb * kb - ka * kc * 4.0F;
		if (kd > Math::min_float)
		{
			kd = Sqrt(kd);
			ke = ke * 0.5F;
			float s1 = (-kb + kd) * ke;
			float s2 = (-kb - kd) * ke;
			float t1 = Fmin(s1, s2);
			float t2 = Fmax(s1, s2);
			if (t2 - t1 > kParamEpsilon)
			{
				splitParam[0] = t1;
				splitParam[2] = t2;
			}
		}

		// Perform subdivisions. This creates up to four cubic subcurves.

		int32 count = 0;
		float prevParam = 0.0F;
		for (machine i = 0; i < 3; i++)
		{
			float t = splitParam[i];
			if ((t > kParamEpsilon) && (t < 1.0F - kParamEpsilon))
			{
				cubicSubcurve[count] = normCurve.CalculateSubcurve(prevParam, t);
				prevParam = t;
				count++;
			}
		}

		if (count != 0)
		{
			cubicSubcurve[count] = normCurve.CalculateSubcurve(prevParam, 1.0F);
			subcurveCount = count + 1;
			subcurveArray = cubicSubcurve;
		}
	}
	else if (Fabs(kb) > kCurveEpsilon)
	{
		// The curvature function is linear.

		float t = -kc / kb;
		if ((t > kParamEpsilon) && (t < 1.0F - kParamEpsilon))
		{
			cubicSubcurve[0] = normCurve.CalculateSubcurve(0.0F, t);
			cubicSubcurve[1] = normCurve.CalculateSubcurve(t, 1.0F);

			subcurveCount = 2;
			subcurveArray = cubicSubcurve;
		}
	}

	int32 curveCount = 0;
	for (machine i = 0; i < subcurveCount; i++)
	{
		curveCount += ApproximateCubicCurve(subcurveArray[i], tolerance, &curveArray[curveCount]);
	}

	for (machine i = 0; i < curveCount; i++)
	{
		curveArray[i].p[0] = curveArray[i].p[0] * scale + p[0];
		curveArray[i].p[1] = curveArray[i].p[1] * scale + p[0];
		curveArray[i].p[2] = curveArray[i].p[2] * scale + p[0];
	}

	curveArray[curveCount - 1].p[2] = p[3];
	return (curveCount);
}


Box3D CubicBezier3D::CalculateBoundingBox(void) const
{
	Box3D box(p[0], p[0]);
	box.Union(p[3]);

	for (machine i = 0; i < 3; i++)
	{
		float a = p[3][i] - p[0][i] + (p[1][i] - p[2][i]) * 3.0F;
		float b = p[1][i] * 2.0F - p[0][i] - p[2][i];
		float c = p[1][i] - p[0][i];

		if (Fabs(a) > Math::min_float)
		{
			float d = Sqrt(b * b - a * c);
			a = 1.0F / a;

			float t = (b - d) * a;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}

			t = (b + d) * a;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}
		}
		else if (Fabs(b) > Math::min_float)
		{
			float t = c / b * 0.5F;
			if ((t > 0.0F) && (t < 1.0F))
			{
				float u = 1.0F - t;
				float t2 = t * t;
				float u2 = u * u;
				float f = p[0][i] * (u2 * u) + p[1][i] * (u2 * t * 3.0F) + p[2][i] * (u * t2 * 3.0F) + p[3][i] * (t2 * t);
				box.min[i] = Fmin(box.min[i], f);
				box.max[i] = Fmax(box.max[i], f);
			}
		}
	}

	return (box);
}

CubicBezier3D CubicBezier3D::CalculateSubcurve(float t1, float t2) const
{
	Point3D a = CalculatePosition(t1);
	Point3D d = CalculatePosition(t2);

	float dt = t2 - t1;
	Point3D b = a + CalculateTangent(t1) * dt;
	Point3D c = d - CalculateTangent(t2) * dt;

	return (CubicBezier3D(a, b, c, d));
}
