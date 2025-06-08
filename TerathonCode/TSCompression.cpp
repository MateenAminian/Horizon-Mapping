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


#include "TSCompression.h"
#include "TSBasic.h"


using namespace Terathon;


namespace Terathon
{
	namespace Compression
	{
		enum
		{
			kUncompressedTinyLength,
			kUncompressedStandardLength,
			kCompressedTinyLengthShortDistance,
			kCompressedStandardLengthShortDistance,
			kCompressedTinyLengthLongDistance,
			kCompressedStandardLengthLongDistance,
			kCompressionInvalid = 6,
			kCompressionTerminator = 7
		};


		enum
		{
			kWindowSize			= 32768,
			kWindowMask			= kWindowSize - 1,
			kHashBitCount		= 12,
			kHashTableSize		= 1 << kHashBitCount,
			kHashShift			= 16 - kHashBitCount,
			kMaxMatchDepth		= 256,
			kMinMatchCount		= 4,
			kMaxTinyLength		= 31 + kMinMatchCount,
			kMaxShortLength		= 256 + kMaxTinyLength,
			kMaxLongLength		= 65536 + kMaxShortLength
		};

		static_assert((kWindowSize & (kWindowSize - 1)) == 0, "kWindowSize must be a power of 2");


		struct DictEntry
		{
			DictEntry		*prev;
			DictEntry		*next;
			const uint8		*data;
		};


		inline umachine GetDataHash(const uint8 *data)
		{
			return ((data[0] << (8 - kHashShift)) | (data[1] >> kHashShift));
		}


		void UpdateDictEntry(DictEntry *dictTable, DictEntry **hashTable, const uint8 *data);
		umachine CountMatchingBytes(const uint8 *x, const uint8 *y, umachine max);
		umachine WriteUncompressedCode(const uint8 *data, umachine length, uint8 *restrict code, umachine max);
		umachine WriteCompressedCode(umachine length, umachine distance, uint8 *restrict code, umachine max);
	}
}


#ifdef _MSC_VER

	#pragma optimize("gt", on)

#endif


void Compression::UpdateDictEntry(DictEntry *dictTable, DictEntry **hashTable, const uint8 *data)
{
	DictEntry *entry = &dictTable[GetPointerAddress(data) & kWindowMask];
	if (entry->data)
	{
		DictEntry *prev = entry->prev;
		DictEntry *next = entry->next;

		if (prev)
		{
			prev->next = next;
		}
		else
		{
			hashTable[GetDataHash(entry->data)] = next;
		}

		if (next)
		{
			next->prev = prev;
		}
	}

	entry->data = data;
	entry->prev = nullptr;

	DictEntry **start = &hashTable[GetDataHash(data)];
	DictEntry *first = *start;
	if (first)
	{
		entry->next = first;
		first->prev = entry;
	}
	else
	{
		entry->next = nullptr;
	}

	*start = entry;
}

umachine Compression::CountMatchingBytes(const uint8 *x, const uint8 *y, umachine max)
{
	umachine count = 0;
	do
	{
		if (x[count] != y[count])
		{
			break;
		}
	} while (++count < max);

	if (count >= kMinMatchCount)
	{
		return (count);
	}

	return (0);
}

umachine Compression::WriteUncompressedCode(const uint8 *data, umachine length, uint8 *restrict code, umachine max)
{
	uint8 *start = code;
	while (length > 65824)
	{
		if (max < 65827)
		{
			return (0);
		}

		code[0] = uint8((kUncompressedStandardLength << 5) | 0x01);
		code[1] = 0xFF;
		code[2] = 0xFF;
		code += 3;

		CopyMemory(data, code, 65824);

		code += 65824;
		data += 65824;
		length -= 65824;
		max -= 65827;
	}

	if (length != 0)
	{
		if (length <= 32)
		{
			if (length + 1 > max)
			{
				return (0);
			}

			code[0] = uint8((kUncompressedTinyLength << 5) | (length - 1));
			code++;
		}
		else if (length <= 288)
		{
			if (length + 2 > max)
			{
				return (0);
			}

			code[0] = uint8(kUncompressedStandardLength << 5);
			code[1] = uint8(length - 33);
			code += 2;
		}
		else
		{
			if (length + 3 > max)
			{
				return (0);
			}

			umachine x = length - 289;
			code[0] = uint8((kUncompressedStandardLength << 5) | 0x01);
			code[1] = uint8(x >> 8);
			code[2] = uint8(x);
			code += 3;
		}

		for (umachine a = 0; a < length; a++)
		{
			code[a] = data[a];
		}
	}

	return (umachine(code - start + length));
}

umachine Compression::WriteCompressedCode(umachine length, umachine distance, uint8 *restrict code, umachine max)
{
	umachine size = 0;
	if (--distance < 256)
	{
		while (length > kMaxLongLength)
		{
			if (max < 4)
			{
				return (0);
			}

			code[0] = uint8((kCompressedStandardLengthShortDistance << 5) | 0x01);
			code[1] = 0xFF;
			code[2] = 0xFF;
			code[3] = uint8(distance);
			size += 4;

			length -= kMaxLongLength;
			code += 4;
			max -= 4;
		}

		if (length != 0)
		{
			if (length <= kMaxTinyLength)
			{
				if (max < 2)
				{
					return (0);
				}

				code[0] = uint8((kCompressedTinyLengthShortDistance << 5) | (length - kMinMatchCount));
				code[1] = uint8(distance);
				size += 2;
			}
			else if (length <= kMaxShortLength)
			{
				if (max < 3)
				{
					return (0);
				}

				code[0] = uint8(kCompressedStandardLengthShortDistance << 5);
				code[1] = uint8(length - (kMaxTinyLength + 1));
				code[2] = uint8(distance);
				size += 3;
			}
			else
			{
				if (max < 4)
				{
					return (0);
				}

				umachine x = length - (kMaxShortLength + 1);
				code[0] = uint8((kCompressedStandardLengthShortDistance << 5) | 0x01);
				code[1] = uint8(x >> 8);
				code[2] = uint8(x);
				code[3] = uint8(distance);
				size += 4;
			}
		}
	}
	else
	{
		while (length > kMaxLongLength)
		{
			if (max < 5)
			{
				return (0);
			}

			code[0] = uint8((kCompressedStandardLengthLongDistance << 5) | 0x01);
			code[1] = 0xFF;
			code[2] = 0xFF;
			code[3] = uint8(distance >> 8);
			code[4] = uint8(distance);
			size += 5;

			length -= kMaxLongLength;
			code += 5;
			max -= 5;
		}

		if (length != 0)
		{
			if (length <= kMaxTinyLength)
			{
				if (max < 3)
				{
					return (0);
				}

				code[0] = uint8((kCompressedTinyLengthLongDistance << 5) | (length - kMinMatchCount));
				code[1] = uint8(distance >> 8);
				code[2] = uint8(distance);
				size += 3;
			}
			else if (length <= kMaxShortLength)
			{
				if (max < 4)
				{
					return (0);
				}

				code[0] = uint8(kCompressedStandardLengthLongDistance << 5);
				code[1] = uint8(length - (kMaxTinyLength + 1));
				code[2] = uint8(distance >> 8);
				code[3] = uint8(distance);
				size += 4;
			}
			else
			{
				if (max < 5)
				{
					return (0);
				}

				umachine x = length - (kMaxShortLength + 1);
				code[0] = uint8((kCompressedStandardLengthLongDistance << 5) | 0x01);
				code[1] = uint8(x >> 8);
				code[2] = uint8(x);
				code[3] = uint8(distance >> 8);
				code[4] = uint8(distance);
				size += 5;
			}
		}
	}

	return (size);
}

uint32 Compression::CompressData(const void *input, uint32 dataSize, uint8 *restrict code)
{
	if (dataSize < kMinMatchCount)
	{
		return (0);
	}

	DictEntry *dictTable = new DictEntry[kWindowSize];
	for (machine a = 0; a < kWindowSize; a++)
	{
		dictTable[a].data = nullptr;
	}

	DictEntry **hashTable = new DictEntry *[kHashTableSize];
	for (machine a = 0; a < kHashTableSize; a++)
	{
		hashTable[a] = nullptr;
	}

	const uint8 *data = static_cast<const uint8 *>(input);
	UpdateDictEntry(dictTable, hashTable, data);

	umachine codeSize = 0;
	umachine startPosition = 0;
	umachine compressPosition = 1;

	for (;;)
	{
		umachine maxLength = dataSize - compressPosition;
		if (maxLength <= kMinMatchCount)
		{
			umachine remain = dataSize - startPosition;
			if (remain != 0)
			{
				umachine size = WriteUncompressedCode(&data[startPosition], remain, &code[codeSize], dataSize - codeSize);
				if (size != 0)
				{
					codeSize += size;
				}
				else
				{
					codeSize = 0;
				}
			}

			break;
		}

		const uint8 *compressData = &data[compressPosition];
		const uint8 *bestMatchData = compressData - 1;
		umachine bestMatchLength = CountMatchingBytes(compressData, bestMatchData, maxLength);

		if (bestMatchLength < maxLength)
		{
			umachine bestControlSize = (bestMatchLength > 35) + (bestMatchLength > 291);

			machine depth = 0;
			const DictEntry *entry = hashTable[GetDataHash(compressData)];
			while (entry)
			{
				umachine matchLength = CountMatchingBytes(compressData, entry->data, maxLength);
				umachine controlSize = (matchLength > 35) + (matchLength > 291);
				if (matchLength - controlSize > bestMatchLength - bestControlSize)
				{
					bestMatchData = entry->data;
					bestMatchLength = matchLength;
					bestControlSize = controlSize;

					if (bestMatchLength == maxLength)
					{
						break;
					}
				}

				if (++depth == kMaxMatchDepth)
				{
					break;
				}

				entry = entry->next;
			}
		}

		if (bestMatchLength != 0)
		{
			if (compressPosition != startPosition)
			{
				umachine size = WriteUncompressedCode(&data[startPosition], compressPosition - startPosition, &code[codeSize], dataSize - codeSize);
				if (size == 0)
				{
					codeSize = 0;
					break;
				}

				codeSize += size;
			}

			umachine size = WriteCompressedCode(bestMatchLength, umachine(compressData - bestMatchData), &code[codeSize], dataSize - codeSize);
			if (size == 0)
			{
				codeSize = 0;
				break;
			}

			codeSize += size;

			umachine updateLength = Min(bestMatchLength, maxLength - kMinMatchCount);
			for (umachine a = 0; a < updateLength; a++)
			{
				UpdateDictEntry(dictTable, hashTable, &compressData[a]);
			}

			compressPosition += bestMatchLength;
			startPosition = compressPosition;
		}
		else
		{
			UpdateDictEntry(dictTable, hashTable, compressData);
			compressPosition++;
		}
	}

	delete[] hashTable;
	delete[] dictTable;

	umachine totalSize = (codeSize + 3) & ~3;
	for (umachine a = codeSize; a < totalSize; a++)
	{
		code[a] = kCompressionTerminator << 5;
	}

	return (uint32(totalSize));
}

void Compression::DecompressData(const uint8 *restrict code, uint32 codeSize, void *output)
{
	uint8 *data = static_cast<uint8 *>(output);

	const uint8 *restrict end = code + codeSize;
	while (code < end)
	{
		uint32		length;

		uint32 control = code[0];
		uint32 type = control >> 5;

		if (type <= kUncompressedStandardLength)
		{
			if (type == kUncompressedTinyLength)
			{
				length = (control & 0x1F) + 1;
				code++;

				do
				{
					*data++ = *code++;
				} while (--length != 0);
			}
			else
			{
				if ((control & 0x1F) != 0)
				{
					length = ((code[1] << 8) | code[2]) + 289;
					code += 3;
				}
				else
				{
					length = code[1] + 33;
					code += 2;
				}

				CopyMemory(code, data, length);
				data += length;
				code += length;
			}
		}
		else if (type <= kCompressedStandardLengthLongDistance)
		{
			uint32		distance;

			if (type <= kCompressedStandardLengthShortDistance)
			{
				if (type == kCompressedTinyLengthShortDistance)
				{
					length = (control & 0x1F) + kMinMatchCount;
					distance = code[1] + 1;
					code += 2;
				}
				else
				{
					if ((control & 0x1F) != 0)
					{
						length = ((code[1] << 8) | code[2]) + (kMaxShortLength + 1);
						distance = code[3] + 1;
						code += 4;
					}
					else
					{
						length = code[1] + (kMaxTinyLength + 1);
						distance = code[2] + 1;
						code += 3;
					}
				}
			}
			else
			{
				if (type == kCompressedTinyLengthLongDistance)
				{
					length = (control & 0x1F) + kMinMatchCount;
					distance = ((code[1] << 8) | code[2]) + 1;
					code += 3;
				}
				else
				{
					if ((control & 0x1F) != 0)
					{
						length = ((code[1] << 8) | code[2]) + (kMaxShortLength + 1);
						distance = ((code[3] << 8) | code[4]) + 1;
						code += 5;
					}
					else
					{
						length = code[1] + (kMaxTinyLength + 1);
						distance = ((code[2] << 8) | code[3]) + 1;
						code += 4;
					}
				}
			}

			const uint8 *source = data - distance;
			do
			{
				*data++ = *source++;
			} while (--length != 0);
		}
		else
		{
			break;
		}
	}
}
