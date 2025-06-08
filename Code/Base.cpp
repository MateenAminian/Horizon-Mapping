#include "OpenGEX.h"


using namespace Framework;


File::File()
{
	storage = nullptr;
	data = nullptr;
	size = 0;
}

File::File(const char *name)
{
	storage = nullptr;
	Load(name);
}

File::~File()
{
	delete[] storage;
}

void File::Load(const char *name)
{
	LARGE_INTEGER	fileSize;

	HANDLE fileHandle = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	GetFileSizeEx(fileHandle, &fileSize);
	size = fileSize.QuadPart;

	// Allocate enough space for the file contents plus another 63 bytes of padding
	// that might be needed to align the data on a 64-byte boundary, plus one byte for a terminator.

	storage = new char[size + 64];
	data = storage + (-reinterpret_cast<int64>(storage) & 0x3F);

	DWORD	actual;

	ReadFile(fileHandle, data, DWORD(size), &actual, nullptr);
	CloseHandle(fileHandle);

	data[size] = 0;
}


namespace
{
	struct TargaHeader
	{
		uint8		idLength;
		uint8		colorMapType;
		uint8		imageType;
		uint8		colorMapStartL;
		uint8		colorMapStartH;
		uint8		colorMapLengthL;
		uint8		colorMapLengthH;
		uint8		colorMapDepth;
		int16		xOffset;
		int16		yOffset;
		int16		width;
		int16		height;
		uint8		pixelDepth;
		uint8		imageDescriptor;

		const uint8 *GetPixelData(void) const
		{
			return (reinterpret_cast<const uint8 *>(this + 1) + idLength);
		};
	};


	void CopyTarga_L8(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount; a++)
		{
			uint32 value = data[0];
			image[a].Set(value, value, value, 255);
			data++;
		}
	}

	void CopyTarga_RGB16(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount; a++)
		{
			uint32 p = *reinterpret_cast<const uint16 *>(data);
			uint32 red = p >> 10;
			uint32 green = (p >> 5) & 31;
			uint32 blue = p & 31;

			red = (red << 3) | (red >> 2);
			green = (green << 3) | (green >> 2);
			blue = (blue << 3) | (blue >> 2);

			image[a].Set(red, green, blue, 255);
			data += 2;
		}
	}

	void CopyTarga_RGB24(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount; a++)
		{
			image[a].Set(data[2], data[1], data[0], 255);
			data += 3;
		}
	}

	void CopyTarga_RGBA32(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount; a++)
		{
			image[a].Set(data[2], data[1], data[0], data[3]);
			data += 4;
		}
	}

	void DecompressTarga_L8(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount;)
		{
			uint8 d = *data++;
			int32 count = (d & 0x7F) + 1;

			if ((d & 0x80) != 0)
			{
				Color4U		c;

				uint32 value = data[0];
				c.Set(value, value, value, 255);
				do
				{
					image[a++] = c;
				} while (--count > 0);

				data++;
			}
			else
			{
				do
				{
					uint32 value = data[0];
					image[a++].Set(value, value, value, 255);
					data++;

				} while (--count > 0);
			}
		}
	}

	void DecompressTarga_RGB16(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount;)
		{
			uint8 d = *data++;
			int32 count = (d & 0x7F) + 1;

			if ((d & 0x80) != 0)
			{
				Color4U		c;

				uint32 p = data[0] | (data[1] << 8);
				data += 2;

				uint32 red = p >> 10;
				uint32 green = (p >> 5) & 31;
				uint32 blue = p & 31;

				red = (red << 3) | (red >> 2);
				green = (green << 3) | (green >> 2);
				blue = (blue << 3) | (blue >> 2);

				c.Set(red, green, blue, 255);
				do
				{
					image[a++] = c;
				} while (--count > 0);
			}
			else
			{
				do
				{
					uint32 p = data[0] | (data[1] << 8);
					data += 2;

					uint32 red = p >> 10;
					uint32 green = (p >> 5) & 31;
					uint32 blue = p & 31;

					red = (red << 3) | (red >> 2);
					green = (green << 3) | (green >> 2);
					blue = (blue << 3) | (blue >> 2);

					image[a++].Set(red, green, blue, 255);
				} while (--count > 0);
			}
		}
	}

	void DecompressTarga_RGB24(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount;)
		{
			uint8 d = *data++;
			int32 count = (d & 0x7F) + 1;

			if ((d & 0x80) != 0)
			{
				Color4U		c;

				c.Set(data[2], data[1], data[0], 255);
				do
				{
					image[a++] = c;
				} while (--count > 0);

				data += 3;
			}
			else
			{
				do
				{
					image[a++].Set(data[2], data[1], data[0], 255);
					data += 3;

				} while (--count > 0);
			}
		}
	}

	void DecompressTarga_RGBA32(const uint8 *data, Color4U *image, int32 pixelCount)
	{
		for (int32 a = 0; a < pixelCount;)
		{
			uint8 d = *data++;
			int32 count = (d & 0x7F) + 1;

			if ((d & 0x80) != 0)
			{
				Color4U		c;

				c.Set(data[2], data[1], data[0], data[3]);
				do
				{
					image[a++] = c;
				} while (--count > 0);

				data += 4;
			}
			else
			{
				do
				{
					image[a++].Set(data[2], data[1], data[0], data[3]);
					data += 4;

				} while (--count > 0);
			}
		}
	}
}

bool Framework::ImportTargaImageFile(const char *name, Color4U **image, Integer2D *size)
{
	File file(name);

	const TargaHeader *header = reinterpret_cast<const TargaHeader *>(file.GetData());
	int32 pixelDepth = header->pixelDepth;
	int32 imageType = header->imageType;

	if ((header->colorMapType != 0) || ((pixelDepth != 8) && (pixelDepth != 16) && (pixelDepth != 24) && (pixelDepth != 32)) || ((imageType != 2) && (imageType != 3) && (imageType != 10) && (imageType != 11)))
	{
		return (false);
	}

	int32 width = header->width;
	int32 height = header->height;
	size->Set(width, height);

	int32 pixelCount = width * height;
	Color4U *pixel = new Color4U[pixelCount];
	*image = pixel;

	const uint8 *data = header->GetPixelData();
	if ((imageType & 8) == 0)
	{
		if (pixelDepth == 8)
		{
			CopyTarga_L8(data, pixel, pixelCount);
		}
		else if (pixelDepth == 16)
		{
			CopyTarga_RGB16(data, pixel, pixelCount);
		}
		else if (pixelDepth == 24)
		{
			CopyTarga_RGB24(data, pixel, pixelCount);
		}
		else
		{
			CopyTarga_RGBA32(data, pixel, pixelCount);
		}
	}
	else
	{
		if (pixelDepth == 8)
		{
			DecompressTarga_L8(data, pixel, pixelCount);
		}
		else if (pixelDepth == 16)
		{
			DecompressTarga_RGB16(data, pixel, pixelCount);
		}
		else if (pixelDepth == 24)
		{
			DecompressTarga_RGB24(data, pixel, pixelCount);
		}
		else
		{
			DecompressTarga_RGBA32(data, pixel, pixelCount);
		}
	}

	if ((header->imageDescriptor & 0x20) != 0)
	{
		int32 h = height >> 1;
		for (int32 j = 0; j < h; j++)
		{
			Color4U *top = pixel + j * width;
			Color4U *bottom = pixel + (height - j - 1) * width;
			for (int32 i = 0; i < width; i++)
			{
				Color4U c = top[i];
				top[i] = bottom[i];
				bottom[i] = c;
			}
		}
	}

	return (true);
}

void Framework::ReleaseTargaImageData(Color4U *image)
{
	delete[] image;
}


namespace
{
	int32 CalculateMipmapChainPixelCount(const Integer3D& size, int32 *mipmapCount)
	{
		int32 levelCount = 0;
		int32 pixelCount = 0;

		int32 width = size.x;
		int32 height = size.y;
		for (;;)
		{
			levelCount++;
			pixelCount += width * height;

			if (Max(width, height) == 1)
			{
				break;
			}

			width = Max(width >> 1, 1);
			height = Max(height >> 1, 1);
		}

		*mipmapCount = levelCount;
		return (pixelCount * size.z);
	}
}

int32 Framework::GenerateMipmapImages(const Integer3D& size, const Color4U *source, Color4U **image)
{
	int32	mipmapCount;

	int32 pixelCount = CalculateMipmapChainPixelCount(size, &mipmapCount);
	ColorRgba *pixelData = new ColorRgba[pixelCount];
	ColorRgba *outputData = pixelData;

	const Color4U *inputData = source;
	int32 layerPixelCount = size.x * size.y;

	for (int32 layer = 0; layer < size.z; layer++)
	{
		for (int32 k = 0; k < layerPixelCount; k++)
		{
			const Color4U& pixel = inputData[k];
			outputData[k].Set(float(pixel.red) * (1.0F / 255.0F), float(pixel.green) * (1.0F / 255.0F), float(pixel.blue) * (1.0F / 255.0F), float(pixel.alpha) * (1.0F / 255.0F));
		}

		const ColorRgba *inputMipmap = outputData;
		ColorRgba *outputMipmap = outputData + size.x * size.y * (size.z - layer);

		Integer2D sourceSize(1, 1);
		float multiplier = 1.0F;

		Integer2D mipmapSize(size.x, size.y);
		while ((mipmapSize.x != 1) || (mipmapSize.y != 1))
		{
			if (mipmapSize.x != 1)
			{
				mipmapSize.x >>= 1;
				sourceSize.x <<= 1;
				multiplier *= 0.5F;
			}

			if (mipmapSize.y != 1)
			{
				mipmapSize.y >>= 1;
				sourceSize.y <<= 1;
				multiplier *= 0.5F;
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * layer;

			for (machine y = 0; y < mipmapSize.y; y++)
			{
				const ColorRgba *input = inputMipmap + y * sourceSize.y * size.x;
				ColorRgba *output = outputMipmap + y * mipmapSize.x;

				for (machine x = 0; x < mipmapSize.x; x++)
				{
					ColorRgba color(0.0F, 0.0F, 0.0F, 0.0F);
					for (machine j = 0; j < sourceSize.y; j++)
					{
						int32 k = j * size.x + x * sourceSize.x;
						for (machine i = 0; i < sourceSize.x; i++)
						{
							color += input[k + i];
						}
					}

					output[x] = color * multiplier;
				}
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * (size.z - layer);
		}

		inputData += size.x * size.y;
		outputData += size.x * size.y;
	}

	Color4U *colorData = new Color4U[pixelCount];
	for (int32 k = 0; k < pixelCount; k++)
	{
		colorData[k] = pixelData[k];
	}

	delete[] pixelData;

	*image = colorData;
	return (mipmapCount);
}

int32 Framework::GenerateMipmapImages(const Integer3D& size, const Color2S *source, Color2S **image)
{
	int32	mipmapCount;

	int32 pixelCount = CalculateMipmapChainPixelCount(size, &mipmapCount);
	Vector2D *pixelData = new Vector2D[pixelCount];
	Vector2D *outputData = pixelData;

	const Color2S *inputData = source;
	int32 layerPixelCount = size.x * size.y;

	for (int32 layer = 0; layer < size.z; layer++)
	{
		for (int32 k = 0; k < layerPixelCount; k++)
		{
			const Color2S& pixel = inputData[k];
			outputData[k].Set(float(pixel.red) * (1.0F / 127.0F), float(pixel.green) * (1.0F / 127.0F));
		}

		const Vector2D *inputMipmap = outputData;
		Vector2D *outputMipmap = outputData + size.x * size.y * (size.z - layer);

		Integer2D sourceSize(1, 1);
		float multiplier = 1.0F;

		Integer2D mipmapSize(size.x, size.y);
		while ((mipmapSize.x != 1) || (mipmapSize.y != 1))
		{
			if (mipmapSize.x != 1)
			{
				mipmapSize.x >>= 1;
				sourceSize.x <<= 1;
				multiplier *= 0.5F;
			}

			if (mipmapSize.y != 1)
			{
				mipmapSize.y >>= 1;
				sourceSize.y <<= 1;
				multiplier *= 0.5F;
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * layer;

			for (machine y = 0; y < mipmapSize.y; y++)
			{
				const Vector2D *input = inputMipmap + y * sourceSize.y * size.x;
				Vector2D *output = outputMipmap + y * mipmapSize.x;

				for (machine x = 0; x < mipmapSize.x; x++)
				{
					Vector2D color(0.0F, 0.0F);
					for (machine j = 0; j < sourceSize.y; j++)
					{
						int32 k = j * size.x + x * sourceSize.x;
						for (machine i = 0; i < sourceSize.x; i++)
						{
							color += input[k + i];
						}
					}

					output[x] = color * multiplier;
				}
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * (size.z - layer);
		}

		inputData += size.x * size.y;
		outputData += size.x * size.y;
	}

	Color2S *colorData = new Color2S[pixelCount];
	for (int32 k = 0; k < pixelCount; k++)
	{
		colorData[k].Set(int32(pixelData[k].x * 127.0F), int32(pixelData[k].y * 127.0F));
	}

	delete[] pixelData;

	*image = colorData;
	return (mipmapCount);
}

int32 Framework::GenerateMipmapImages(const Integer3D& size, const Color1S *source, Color1S **image)
{
	int32	mipmapCount;

	int32 pixelCount = CalculateMipmapChainPixelCount(size, &mipmapCount);
	float *pixelData = new float[pixelCount];
	float *outputData = pixelData;

	const Color1S *inputData = source;
	int32 layerPixelCount = size.x * size.y;

	for (int32 layer = 0; layer < size.z; layer++)
	{
		for (int32 k = 0; k < layerPixelCount; k++)
		{
			const Color1S& pixel = inputData[k];
			outputData[k] = float(pixel) * (1.0F / 127.0F);
		}

		const float *inputMipmap = outputData;
		float *outputMipmap = outputData + size.x * size.y * (size.z - layer);

		Integer2D sourceSize(1, 1);
		float multiplier = 1.0F;

		Integer2D mipmapSize(size.x, size.y);
		while ((mipmapSize.x != 1) || (mipmapSize.y != 1))
		{
			if (mipmapSize.x != 1)
			{
				mipmapSize.x >>= 1;
				sourceSize.x <<= 1;
				multiplier *= 0.5F;
			}

			if (mipmapSize.y != 1)
			{
				mipmapSize.y >>= 1;
				sourceSize.y <<= 1;
				multiplier *= 0.5F;
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * layer;

			for (machine y = 0; y < mipmapSize.y; y++)
			{
				const float *input = inputMipmap + y * sourceSize.y * size.x;
				float *output = outputMipmap + y * mipmapSize.x;

				for (machine x = 0; x < mipmapSize.x; x++)
				{
					float color = 0.0F;
					for (machine j = 0; j < sourceSize.y; j++)
					{
						int32 k = j * size.x + x * sourceSize.x;
						for (machine i = 0; i < sourceSize.x; i++)
						{
							color += input[k + i];
						}
					}

					output[x] = color * multiplier;
				}
			}

			outputMipmap += mipmapSize.x * mipmapSize.y * (size.z - layer);
		}

		inputData += size.x * size.y;
		outputData += size.x * size.y;
	}

	Color1S *colorData = new Color1S[pixelCount];
	for (int32 k = 0; k < pixelCount; k++)
	{
		colorData[k] = int8(pixelData[k] * 127.0F);
	}

	delete[] pixelData;

	*image = colorData;
	return (mipmapCount);
}

void Framework::ReleaseMipmapImages(void *image)
{
	delete[] image;
}


namespace
{
	struct RandomState
	{
		uint32		n[4];

		RandomState();
	};

	RandomState		randomState;
}


RandomState::RandomState()
{
	SYSTEMTIME			time;

	uint32 a = GetTickCount();
	GetSystemTime(&time);
	uint32 b = (time.wMinute << 16) | time.wSecond;

	n[0] = a;
	n[1] = ~a;
	n[2] = b;
	n[3] = ~b;
}


uint32 Framework::RandomInteger(uint32 n)
{
	uint32 a = randomState.n[0] * 0xBC658A9D + 1;
	uint32 b = randomState.n[1] * 0x102F38E5 + 1;
	uint32 c = randomState.n[2] * 0x8712D6BD + 1;
	uint32 d = randomState.n[3] * 0x9DEA7405 + 1;

	randomState.n[0] = a;
	randomState.n[1] = b;
	randomState.n[2] = c;
	randomState.n[3] = d;

	return ((((a + b + c + d) >> 16) * n) >> 16);
}

float Framework::RandomFloat(float n)
{
	uint32 a = randomState.n[0] * 0xBC658A9D + 1;
	uint32 b = randomState.n[1] * 0x102F38E5 + 1;
	uint32 c = randomState.n[2] * 0x8712D6BD + 1;
	uint32 d = randomState.n[3] * 0x9DEA7405 + 1;

	randomState.n[0] = a;
	randomState.n[1] = b;
	randomState.n[2] = c;
	randomState.n[3] = d;

	return (float((a + b + c + d) >> 16) * (n * (1.0F / 65536.0F)));
}

Vector2D Framework::RandomUnitVector2D(void)
{
	Vector2D cs = CosSin(RandomFloat(Math::tau));
	return (Vector2D(cs.x, cs.y));
}

Vector3D Framework::RandomUnitVector3D(void)
{
	float z = RandomFloat(2.0F) - 1.0F;
	float sp = Sqrt(1.0F - z * z);
	Vector2D cs = CosSin(RandomFloat(Math::tau));
	return (Vector3D(cs.x * sp, cs.y * sp, z));
}

Bivector3D Framework::RandomUnitBivector3D(void)
{
	float z = RandomFloat(2.0F) - 1.0F;
	float sp = Sqrt(1.0F - z * z);
	Vector2D cs = CosSin(RandomFloat(Math::tau));
	return (Bivector3D(cs.x * sp, cs.y * sp, z));
}

Quaternion Framework::RandomUnitQuaternion(void)
{
	float z = RandomFloat(2.0F) - 1.0F;
	float sp = Sqrt(1.0F - z * z);

	Vector2D t = CosSin(RandomFloat(Math::tau));
	Vector2D u = CosSin(RandomFloat(Math::tau_over_2));

	return (Quaternion(u.y * t.x * sp, u.y * t.y * sp, u.y * z, u.x));
}

Point2D Framework::RandomPointInsideDisk(float radius)
{
	float r = Sqrt(RandomFloat(1.0F)) * radius;
	Vector2D cs = CosSin(RandomFloat(Math::tau));
	return (Point2D(cs.x * r, cs.y * r));
}

Point3D Framework::RandomPointInsideSphere(float radius)
{
	float r = Pow(RandomFloat(1.0F), 0.33333333F) * radius;
	Vector2D cs = CosSin(RandomFloat(Math::tau));
	float z = RandomFloat(2.0F) - 1.0F;
	float sp = Sqrt(1.0F - z * z) * r;
	return (Point3D(cs.x * sp, cs.y * sp, z * r));
}
