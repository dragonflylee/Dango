/**
 *  MemoryHolderTmp.h
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
#include "../LDUnmanagedObject.h"
#include "../AMemoryHolder.h"
#include "../APageHeader.h"
#include "MHBin.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{

	class MemoryHolderTmp ;
	class MHPageHeaderTmp ;

	//==========================================================
	// �@�e���|�����^�̃������z���_
	// �@�ʏ�̃������Ǘ������i���R��malloc/free�\�j
	//==========================================================
	class MemoryHolderTmp : public LDUnmanagedObject
	//class MemoryHolderTmp : public AMemoryHolder
	{
	public:
		const static l2d_size_t		LARGE_0					= 0 ;	// ���R�ȃT�C�Y�̎��ɁAchunkSize �ApageSize�ɐݒ肳���
		const static l2d_size_t		PAGE_ALIGN				= 32 ;	//
		const static int		BIN_COUNT				= 6 ;	// BIN�̐�
		const static l2d_size_t		CHUNK_SIZE[ BIN_COUNT ] ;

		//------------
		MemoryHolderTmp(LDAllocator::Type allocType , const char* holderName );
		virtual ~MemoryHolderTmp() ;

		virtual void* malloc_exe( l2d_size_t size  , int align) ;
	
		virtual void free_exe( APageHeader* header , void* ptr ) ;
		virtual void clear() ;

	private:
		MHPageHeaderTmp*	createNewPage( MHBin* bin , l2d_size_t chunkSize ) ;

		// �^�[�Q�b�gpage�̑O��page���擾����B���������ꍇ�� *ret_found == true 
		// �߂�l�͑O�̃y�[�W�A*ret_found == true �Ŗ߂�l��NULL�̏ꍇ�� first �ibin->curPages���j�ƈ�v
		MHPageHeaderTmp*  getPrevPage( MHPageHeaderTmp*  first , MHPageHeaderTmp* targetPage , bool* ret_found ) ;

		void dumpPages_exe( MHPageHeaderTmp* p , MHBin* bin ) ;

		void deletePage( MHPageHeaderTmp* page , MHBin* bin ) ;
		MHBin*  getBinForSize( l2d_size_t  size ) ;
		MHBin*	getBin( int binNo ) ;

		void  dumpPages() ;

	private:
		LDAllocator::Type		allocType ;
		const char*				holderName ;	// MemoryHolder�̎��ʖ�
		int						holderNo ;		// MemoryHolder�̉��Ԗڂ̃C���X�^���X��

		MHBin					bins[BIN_COUNT] ;
	};


	//==========================================================
	//==========================================================
	class MemoryHolderSocket : public AMemoryHolder
	{
	public:
		MemoryHolderSocket(MemoryHolderTmp* impl) : impl( impl )
		{

		}

		virtual ~MemoryHolderSocket(){}

		virtual void* malloc_exe( l2d_size_t size  , int align){ return impl->malloc_exe( size , align ) ; }
		virtual void free_exe( APageHeader* header ,void* ptr ){ impl->free_exe( header , ptr ) ; }
		virtual void clear(){ impl->clear() ; }

	private:
		MemoryHolderTmp*	impl ;
	} ;

}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__