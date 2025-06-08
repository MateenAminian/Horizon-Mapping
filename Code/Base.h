#ifndef Base_h
#define Base_h


#define USE_FULL_SCREEN		0
#define USE_RAW_INPUT		1


#if defined(_MSC_VER)

	#pragma warning(disable: 4101 4312 4530 4577)

#endif


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#undef near
#undef far

#include "TSString.h"
#include "TSInteger.h"
#include "TSColor.h"
#include "TSArray.h"
#include "TSList.h"
#include "TSTree.h"
#include "TSMatrix4D.h"
#include "TSQuaternion.h"
#include "TSTools.h"
#include "TSOpenDDL.h"
#include "SLSlug.h"


namespace Framework
{
	using namespace Terathon;


	typedef Bivector3D Normal;
	typedef Trivector4D Plane;


	inline float Dot(const Plane& plane, const Vector3D& v)
	{
		return (plane.x * v.x + plane.y * v.y + plane.z * v.z);
	}

	inline float Dot(const Plane& plane, const Point3D& p)
	{
		return (plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w);
	}


	// Define a really basic class for loading files.

	class File
	{
		private:

			char		*storage;
			char		*data;
			uint64		size;

		public:

			File();
			File(const char *name);
			~File();

			void Load(const char *name);

			const char *GetData(void) const
			{
				return (data);
			}

			uint64 GetSize(void) const
			{
				return (size);
			}
	};


	bool ImportTargaImageFile(const char *name, Color4U **image, Integer2D *size);
	void ReleaseTargaImageData(Color4U *image);

	int32 GenerateMipmapImages(const Integer3D& size, const Color4U *source, Color4U **image);
	int32 GenerateMipmapImages(const Integer3D& size, const Color2S *source, Color2S **image);
	int32 GenerateMipmapImages(const Integer3D& size, const Color1S *source, Color1S **image);
	void ReleaseMipmapImages(void *image);


	uint32 RandomInteger(uint32 n);
	float RandomFloat(float n);

	inline uint32 RandomInteger(int32 min, int32 max)
	{
		return (RandomInteger(max - min) + min);
	}

	inline uint32 RandomInteger(const Range<int32>& range)
	{
		return (RandomInteger(range.max - range.min) + range.min);
	}

	inline float RandomFloat(float min, float max)
	{
		return (RandomFloat(max - min) + min);
	}

	inline float RandomFloat(const Range<float>& range)
	{
		return (RandomFloat(range.max - range.min) + range.min);
	}

	Vector2D RandomUnitVector2D(void);
	Vector3D RandomUnitVector3D(void);
	Bivector3D RandomUnitBivector3D(void);
	Quaternion RandomUnitQuaternion(void);

	Point2D RandomPointInsideDisk(float radius);
	Point3D RandomPointInsideSphere(float radius);
}


#endif
