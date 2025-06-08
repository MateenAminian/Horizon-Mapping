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


#ifndef TSBuffer_h
#define TSBuffer_h


#include "TSPlatform.h"


namespace Terathon
{
	enum
	{
		kBufferListMinLogSize	= 8,
		kBufferListMaxLogSize	= 25,
		kBufferFreeListCount	= kBufferListMaxLogSize - kBufferListMinLogSize + 1
	};


	struct ManagedBlock
	{
		uint32				blockStart;
		uint32				blockSize;

		ManagedBlock		*prevBlock;
		ManagedBlock		*nextBlock;
		ManagedBlock		*prevFreeBlock;
		ManagedBlock		*nextFreeBlock;

		int32				remainCount;
		ManagedBlock		*nextReleasedBlock;
	};


	class ManagedBuffer
	{
		private:

			ManagedBlock		*firstFreeBlock[kBufferFreeListCount];
			ManagedBlock		*firstDeadBlock;

			ManagedBlock		*firstReleasedBlock;
			ManagedBlock		*lastReleasedBlock;

			uint32				bufferMemorySize;
			uint32				bufferBlockCount;
			uint32				bufferAlignment;
			int32				bufferRemainCount;

			uint32				usedBlockCount;
			uint32				usedBlockMemorySize;

			void				(*freeFunction)(void *);

			ManagedBlock		blockTable[1];

			ManagedBuffer(uint32 memorySize, uint32 blockCount, uint32 alignment, int32 remainCount, void (*freeFunc)(void *));
			~ManagedBuffer();

			void RegisterFreeBlock(ManagedBlock *block);
			void UnregisterFreeBlock(ManagedBlock *block, uint32 index);
			void UnregisterFreeBlock(ManagedBlock *block);

			ManagedBlock *SplitBlock(ManagedBlock *block, uint32 size, uint32 index);

		public:

			static ManagedBuffer *NewBuffer(uint32 memorySize, uint32 blockCount, uint32 alignment = 256, int32 remainCount = 2, void *(*allocFunc)(size_t) = nullptr, void (*freeFunc)(void *) = nullptr);
			static void ReleaseBuffer(ManagedBuffer *buffer);

			void ResetBuffer(void);
			void BufferTask(void);

			ManagedBlock *NewBlock(uint32 size);
			void ReleaseBlock(ManagedBlock *block);
			void DestroyBlock(ManagedBlock *block);
	};
}


#endif
