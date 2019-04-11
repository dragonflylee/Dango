/*
 *  LDAllocator.h
 *
 *  Live2D�Ŏg�p���郁�������m�ہE�j�����邽�߂̃N���X
 *
 *  �J�X�^�}�C�Y�������ꍇ�́A�T�u�N���X�����Amalloc, free���I�[�o�[���C�h���A
 *  Live2D�̏��������ɓo�^����
 *
 *  Live2D::init( live2d::LDAllocator*  allocator ) ;
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#ifndef __LIVE2D_LDALLOCATOR_H__
#define __LIVE2D_LDALLOCATOR_H__

//#include "../Live2D.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d { 


	
	class LDAllocator // LDObject���p�����Ȃ�
	{
	public:
		typedef enum {
			MAIN = 0 ,
			GPU 
#ifdef L2D_TARGET_PS4
			,SHADER,
#endif
		} Type ;

	public:
		LDAllocator();
		virtual ~LDAllocator();

		
		virtual void* pageAlloc( unsigned int size , LDAllocator::Type  allocType ) = 0 ;

		
		
		virtual void pageFree( void* ptr , LDAllocator::Type  allocType ) = 0 ;


		
		virtual void init(){}

		
		virtual void dispose(){}

	};

} 
//------------------------- LIVE2D NAMESPACE ------------

#endif