/**
 *  MHBin.h
 *
 * �ꎞ�C���X�^���X�p�̃������ێ��p�N���X
 * 
 * �ʏ�̊m�ہE�j�����s������������
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#pragma once


#ifndef __SKIP_DOC__

#include "../../Live2D.h"
#include "../LDObject.h"
#include "../AMemoryHolder.h"
#include "../APageHeader.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{

	class MemoryHolderTmp ;
	class MHPageHeaderTmp ;


	//==========================================================
	// �@Bin�i�r���j�N���X
	// �@32 , 64 , 128���o�C�g�T�C�Y���ƂɃy�[�W�f�[�^���i�[����
	//==========================================================
	class MHBin
	{
	public:
		MHBin() ;

		void init( l2d_uint16 binNo , l2d_size_t _chunkSize , l2d_size_t _pageSize ) ;
		
		l2d_size_t	getChunkSize( l2d_size_t  malloc_size ) ;

	public:// ��
		l2d_size_t					chunkSize ;			// �P�̃`�����N�T�C�Y�BAllocHeader���܂ށB0�̎��͎��R�ȃT�C�Y
		l2d_size_t					pageSize ;			// �y�[�W�S�̂̃T�C�Y�BAllocHeader���܂ށB0�̎��͎��R�ȃT�C�Y
		l2d_uint16				pageChunkCount ;	// �P�̃y�[�W�ɓ���`�����N��
		l2d_uint16				binNo ;				// ���g��Bin�ԍ��iindex�j
		l2d_uint32				bitmask[3] ;		// �g�p�\�ȃr�b�g��1�Ƃ���(�g�p���Ƃ����Ӗ��ł͂Ȃ�)�B0�͎g�p�s��

		MHPageHeaderTmp*		filledPages ;		// �S���g�p��
		MHPageHeaderTmp*		curPages ;			// �g�p�\�y�[�W
	} ;

}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__