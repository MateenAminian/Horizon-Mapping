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


#include "TSBuffer.h"
#include "TSBasic.h"
#include "TSTools.h"


using namespace Terathon;


namespace
{
	enum : uint32
	{
		kBlockFreeFlag		= 0x00000001,
		kBlockFlagsMask		= 0x0000000F
	};

	inline uint32 GetFreeListIndex(uint32 size)
	{
		return (Min(MaxZero(31 - kBufferListMinLogSize - Cntlz(size & ~kBlockFreeFlag)), kBufferFreeListCount - 1));
	}
}


ManagedBuffer::ManagedBuffer(uint32 memorySize, uint32 blockCount, uint32 alignment, int32 remainCount, void (*freeFunc)(void *))
{
	bufferBlockCount = blockCount;
	bufferMemorySize = memorySize;
	bufferAlignment = alignment - 1;
	bufferRemainCount = remainCount;
	freeFunction = freeFunc;

	ResetBuffer();
}

ManagedBuffer::~ManagedBuffer()
{
}

ManagedBuffer *ManagedBuffer::NewBuffer(uint32 memorySize, uint32 blockCount, uint32 alignment, int32 remainCount, void *(*allocFunc)(size_t), void (*freeFunc)(void *))
{
	Assert((alignment & (alignment - 1)) == 0, "ManagedBuffer::New(), alignment must be a power of 2");
	Assert(alignment >= 16, "ManagedBuffer::New(), alignment must be at least 16");

	size_t objectSize = sizeof(ManagedBuffer) + (blockCount - 1) * sizeof(ManagedBlock);
	void *storage = (allocFunc) ? (*allocFunc)(objectSize) : new char[objectSize];
	return (new(storage) ManagedBuffer(memorySize, blockCount, alignment, remainCount, freeFunc));
}

void ManagedBuffer::ReleaseBuffer(ManagedBuffer *buffer)
{
	void (*freeFunc)(void *) = buffer->freeFunction;
	buffer->~ManagedBuffer();

	if (freeFunc)
	{
		(*freeFunc)(buffer);
	}
	else
	{
		delete[] reinterpret_cast<char *>(buffer);
	}
}

void ManagedBuffer::ResetBuffer(void)
{
	usedBlockCount = 0;
	usedBlockMemorySize = 0;

	for (machine a = 0; a < kBufferFreeListCount; a++)
	{
		firstFreeBlock[a] = nullptr;
	}

	firstFreeBlock[GetFreeListIndex(bufferMemorySize)] = &blockTable[0];
	firstDeadBlock = &blockTable[1];

	firstReleasedBlock = nullptr;
	lastReleasedBlock = nullptr;

	blockTable[0].blockStart = 0;
	blockTable[0].blockSize = bufferMemorySize | kBlockFreeFlag;
	blockTable[0].prevBlock = nullptr;
	blockTable[0].nextBlock = nullptr;
	blockTable[0].prevFreeBlock = nullptr;
	blockTable[0].nextFreeBlock = nullptr;

	uint32 blockCount = bufferBlockCount;
	for (umachine a = 2; a < blockCount; a++)
	{
		blockTable[a - 1].nextBlock = &blockTable[a];
	}

	blockTable[blockCount - 1].nextBlock = nullptr;
}

void ManagedBuffer::BufferTask(void)
{
	ManagedBlock *last = nullptr;

	ManagedBlock *block = firstReleasedBlock;
	while (block)
	{
		ManagedBlock *next = block->nextReleasedBlock;

		int32 count = block->remainCount - 1;
		if (count > 0)
		{
			block->remainCount = count;
			last = block;
		}
		else
		{
			if (firstReleasedBlock == block)
			{
				firstReleasedBlock = next;
			}

			DestroyBlock(block);
		}

		block = next;
	}

	lastReleasedBlock = last;
}

void ManagedBuffer::RegisterFreeBlock(ManagedBlock *block)
{
	uint32 index = GetFreeListIndex(block->blockSize);
	ManagedBlock *firstBlock = firstFreeBlock[index];
	if (firstBlock)
	{
		firstBlock->prevFreeBlock = block;
	}

	block->prevFreeBlock = nullptr;
	block->nextFreeBlock = firstBlock;
	firstFreeBlock[index] = block;
}

void ManagedBuffer::UnregisterFreeBlock(ManagedBlock *block, uint32 index)
{
	ManagedBlock *prevBlock = block->prevFreeBlock;
	ManagedBlock *nextBlock = block->nextFreeBlock;

	if (firstFreeBlock[index] == block)
	{
		firstFreeBlock[index] = nextBlock;
	}

	if (prevBlock)
	{
		prevBlock->nextFreeBlock = nextBlock;
	}

	if (nextBlock)
	{
		nextBlock->prevFreeBlock = prevBlock;
	}
}

inline void ManagedBuffer::UnregisterFreeBlock(ManagedBlock *block)
{
	UnregisterFreeBlock(block, GetFreeListIndex(block->blockSize));
}

ManagedBlock *ManagedBuffer::SplitBlock(ManagedBlock *block, uint32 size, uint32 index)
{
	usedBlockCount++;
	usedBlockMemorySize += size;

	UnregisterFreeBlock(block, index);

	uint32 blockSize = block->blockSize & ~kBlockFreeFlag;
	uint32 freeSize = blockSize - size;

	if (freeSize >= (1 << kBufferListMinLogSize))
	{
		ManagedBlock *freeBlock = firstDeadBlock;
		if (freeBlock)
		{
			firstDeadBlock = freeBlock->nextBlock;

			ManagedBlock *nextBlock = block->nextBlock;
			if (nextBlock)
			{
				nextBlock->prevBlock = freeBlock;
			}

			block->nextBlock = freeBlock;

			freeBlock->blockStart = block->blockStart + size;
			freeBlock->blockSize = freeSize | kBlockFreeFlag;
			freeBlock->prevBlock = block;
			freeBlock->nextBlock = nextBlock;
			RegisterFreeBlock(freeBlock);

			block->blockSize = size;
			return (block);
		}
	}

	block->blockSize = blockSize;
	return (block);
}

ManagedBlock *ManagedBuffer::NewBlock(uint32 size)
{
	size = (size + bufferAlignment) & ~bufferAlignment;
	uint32 index = GetFreeListIndex(size);

	ManagedBlock *freeBlock = firstFreeBlock[index];
	while (freeBlock)
	{
		if ((freeBlock->blockSize & ~kBlockFreeFlag) >= size)
		{
			return (SplitBlock(freeBlock, size, index));
		}

		freeBlock = freeBlock->nextFreeBlock;
	}

	for (umachine a = index + 1; a < kBufferFreeListCount; a++)
	{
		freeBlock = firstFreeBlock[a];
		if (freeBlock)
		{
			return (SplitBlock(freeBlock, size, a));
		}
	}

	return (nullptr);
}

void ManagedBuffer::ReleaseBlock(ManagedBlock *block)
{
	block->remainCount = bufferRemainCount;
	block->nextReleasedBlock = nullptr;

	ManagedBlock *last = lastReleasedBlock;
	if (last)
	{
		last->nextReleasedBlock = block;
	}
	else
	{
		firstReleasedBlock = block;
	}

	lastReleasedBlock = block;
}

void ManagedBuffer::DestroyBlock(ManagedBlock *block)
{
	usedBlockCount--;
	usedBlockMemorySize -= block->blockSize;

	ManagedBlock *nextBlock = block->nextBlock;
	ManagedBlock *prevBlock = block->prevBlock;

	if ((nextBlock) && (nextBlock->blockSize & kBlockFreeFlag))
	{
		UnregisterFreeBlock(nextBlock);
		ManagedBlock *lastBlock = nextBlock->nextBlock;

		if ((prevBlock) && (prevBlock->blockSize & kBlockFreeFlag))
		{
			UnregisterFreeBlock(prevBlock);

			if (lastBlock)
			{
				lastBlock->prevBlock = prevBlock;
			}

			prevBlock->nextBlock = lastBlock;

			prevBlock->blockSize = ((prevBlock->blockSize + block->blockSize + nextBlock->blockSize) & ~kBlockFlagsMask) | kBlockFreeFlag;
			RegisterFreeBlock(prevBlock);

			nextBlock->nextBlock = firstDeadBlock;
			firstDeadBlock = block;
		}
		else
		{
			if (lastBlock)
			{
				lastBlock->prevBlock = block;
			}

			block->nextBlock = lastBlock;

			block->blockSize = ((block->blockSize + nextBlock->blockSize) & ~kBlockFlagsMask) | kBlockFreeFlag;
			RegisterFreeBlock(block);

			nextBlock->nextBlock = firstDeadBlock;
			firstDeadBlock = nextBlock;
		}
	}
	else if ((prevBlock) && (prevBlock->blockSize & kBlockFreeFlag))
	{
		UnregisterFreeBlock(prevBlock);

		if (nextBlock)
		{
			nextBlock->prevBlock = prevBlock;
		}

		prevBlock->nextBlock = nextBlock;

		prevBlock->blockSize = ((prevBlock->blockSize + block->blockSize) & ~kBlockFlagsMask) | kBlockFreeFlag;
		RegisterFreeBlock(prevBlock);

		block->nextBlock = firstDeadBlock;
		firstDeadBlock = block;
	}
	else
	{
		block->blockSize |= kBlockFreeFlag;
		RegisterFreeBlock(block);
	}
}
