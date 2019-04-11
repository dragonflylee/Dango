/*
 *  MemoryParam.h
 *
 * �������m�ۗp�p�����[�^
 * 
 * �e�탁�\�b�h�ɂ��̃p�����[�^��`�d�����ă������Ǘ��𐧌䂷��B
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */


#pragma once


#ifndef __SKIP_DOC__

#include "LDUnmanagedObject.h"
#include "fixed/MemoryHolderFixed.h"
#include "tmp/MemoryHolderTmp.h"
#include "LDAllocator.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{


	class MemoryPage ;

	//----------------------------------------
	class MemoryStackEnt : public LDUnmanagedObject// ����������LDObject���p�����Ȃ�
	{
		friend class MemoryParam ;
	public:
		MemoryStackEnt() : nextEnt(NULL), allocType(0) , align(0) , holder(NULL)
		{
		}

		virtual ~MemoryStackEnt(){}

	private:
		MemoryStackEnt*		nextEnt ;

		int					allocType ;
		int					align ;
		AMemoryHolder*		holder ;// �J�����Ȃ�
	} ;


	//----------------------------------------
	class MemoryParam : public LDObject// Tmp��Placement new�ɂ��邱�ƂŁATmp���������p���\�ɕύX
	{
	public:

	//	MemoryParam();
		MemoryParam(AMemoryHolder* main , AMemoryHolder* gpu);
		
		virtual ~MemoryParam() ;

		// Alloc�̐ݒ���w�肷��(�߂�l�͒��O�̐ݒ�j
		LDAllocator::Type  setAllocType( LDAllocator::Type  allocType )
		{
			LDAllocator::Type  ret = this->curAllocType	 ;
			this->curAllocType	= allocType ;
			this->curMemoryHolder	= getMemoryHolder( allocType ) ;	
			return ret ;
		}

		// Alloc�̐ݒ�����o��(�߂�l�͒��O�̐ݒ�j
		int  setAllocAlign( int align )
		{
			int ret = this->curAlign ;
			this->curAlign = align ;
			return ret ;
		}

		// Alloc�̐ݒ�����o��
		LDAllocator::Type getAllocType( )
		{
			return this->curAllocType ;
		}

		// Alloc�̐ݒ�����o��
		int  getAllocAlign( )
		{
			return curAlign ;
		}


		void* malloc_exe( l2d_size_t size  )
		{
			return curMemoryHolder->malloc_exe( size , curAlign ) ;
		}

		AMemoryHolder*  getCurMemoryHolder()
		{
			return curMemoryHolder ;
		}

		AMemoryHolder*  getMemoryHolder( LDAllocator::Type  allocType ) ;

		void clear() ;

		// �J�����Ȃ��̂ŊO���ŊǗ�����
		void setMemoryHolderMain(AMemoryHolder*	h)
		{
			this->memoryHolderMain = h ;
		}

		// �J�����Ȃ��̂ŊO���ŊǗ�����
		void setMemoryHolderGPU(AMemoryHolder*	h)
		{
			this->memoryHolderGPU = h ;
		}

		void checkMemory()
		{
			memoryHolderMain->healthCheck();
			memoryHolderGPU->healthCheck();
		}
	private:

		AMemoryHolder*			curMemoryHolder ;
		LDAllocator::Type		curAllocType ;
		int						curAlign ;

		AMemoryHolder*			memoryHolderMain ;	
		AMemoryHolder*			memoryHolderGPU ;	

	};


}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__