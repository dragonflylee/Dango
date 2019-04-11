/**
 *  MHPageHeaderTmp.h
 *
 *  �ꎞ�C���X�^���X�p�̃������ێ��p�N���X
 * 
 *  �ʏ�̊m�ہE�j�����s������������
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
	class MHBin ;


	//==========================================================
	// 	�y�[�W�w�b�_ �N���X
	// 
	// 	Page�̐擪��PageHeader������A�ȍ~�Ƀf�[�^������
	// 	PageHeader�̃T�C�Y��64-sizeof(AllocHeader)�ȉ��Ƃ���
	// 	chunk�̐擪�́Athis + 64 - sizeof( AllocHeader )�ƂȂ�A�|�C���^�� this + 64�ƂȂ�
	//==========================================================
	class MHPageHeaderTmp : public APageHeader 
	{
	public:
		// placement new���s��
		MHPageHeaderTmp( MemoryHolderTmp* holder , MHBin* bin , l2d_size_t pageSize , l2d_size_t chunkSize , l2d_size_t pageOffset ) ;

		static l2d_size_t  calcChunkCount( l2d_size_t  pageSize , l2d_size_t  chunkSize ) ;

		// �g�p�\�ȍŏ��̗v�f�̃C���f�b�N�X�����o���A�g�p���t���O�𗧂Ă�
		int getFirstEmptyIndexAndSetFlag(MHBin* bin) ;

		void*  getPtr( int index ) ;

		AllocHeader*  getAllocHeaderPtr( int index ) ;

		int  getIndexOfPtr( void* ptr ) ;

		// �g�p�\�ȍŏ��̗v�f�̃C���f�b�N�X�����o���A�g�p���t���O�𗧂Ă�
		void setFlag( int index , bool flag ) ;

		bool getFlag( int index ) ;

		// �J������
		virtual void  free_exe( void* ptr ){ holder->free_exe( this , ptr ) ; }
		

	protected:
		~MHPageHeaderTmp(){}// placement new���g��/delete�͂��Ȃ�

	public:// ��
		// []���́A32�̃o�C�g���ƁA64bit�̃o�C�g��
		//void*					vtable			// [4-8]
		MemoryHolderTmp*		holder ;		// [4-8]
		l2d_uint32				bitmask[3] ;	// [12]	32*3=�ő�96�̃t���O�����A���A���Ȃ炸����Page��96�ێ�����Ƃ͌���Ȃ�
												// 		�g�p���̂��̂̓t���O�����B
		l2d_uint16				emptyCount ;	// [2]
		l2d_uint16				chunkCount ;	// [2]	�`�����N�̍��v��
		l2d_uint32				chunkSize ;		// [4]	��̃f�[�^�̃T�C�Y�iAllocHeader���܂ށj
		l2d_uint32				pageSize ;		// [4]
		// �����܂� 32-36
	
		MHPageHeaderTmp*		nextPage ;		// [4-8]
		l2d_uint8					pageOffset ;	// [1]	�y�[�W���A���C�����g���������ꍇ�̒����o�C�g���i�ő�32�j
		l2d_uint8					binNo ;			// [1]	MHBin�̔ԍ�

		// 8-16

		// 64bit�ł��l����ƍő�� 64-8 = 56byte
	} ;

}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__