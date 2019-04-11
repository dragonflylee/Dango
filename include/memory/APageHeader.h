/*
 *  APageHeader.h
 *
 *  �������ێ��̂��߂̒��ۃN���X
 * 
 *  �ʏ�̃������Ǘ��ƁA�����Ǘ��^�̃������Ǘ��𒊏ۉ�
 * 
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#pragma once


#ifndef __SKIP_DOC__

#include "LDUnmanagedObject.h"
//#include "../memory/MemoryPage.h"
#include "LDAllocator.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{


	//==========================================================
	//==========================================================
	class APageHeader // placement new���g���Adelete���Ȃ�
	//class APageHeader  : public LDUnmanagedObject // �������Ǘ��ΏۊO
	{
	public:
		const static int ENTRY_OFFSET = 64 ;// �y�[�W�̍ŏ��̃f�[�^�ւ̃I�t�Z�b�g�ichunk��-4(32bit) -8(64bit))

		APageHeader(){}

		
		virtual void  free_exe( void* ptr ) = 0 ;


	protected:
		~APageHeader(){}// placement new���g��/delete�͂��Ȃ�

	};


}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__