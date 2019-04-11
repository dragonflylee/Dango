/*
 *  MHPageHeaderFixed.h
 *
 *	�ꊇ�j���̂ݑΉ������������y�[�W
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#pragma once


#ifndef __SKIP_DOC__

#include "../../Live2D.h"
#include "../LDUnmanagedObject.h"
#include "../APageHeader.h"
#include "../AMemoryHolder.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{

	class MHPageHeaderFixed : public APageHeader // MemoryManagement�ΏۊO
	{
	public:
		MHPageHeaderFixed();

		int getPageNo(){ return pageNo ; }
		int getPageAmount(){ return pageAmount ; }

		void* getStartPtr(){ return (char*)this + ENTRY_OFFSET - sizeof(AllocHeader) ; }

		virtual void  free_exe( void* ptr ) ;

	protected:
		~MHPageHeaderFixed(){}// placement new���g��/delete�͂��Ȃ�

	public:// ---�@�ȉ��A�p�u���b�N�Q�Ƃ����� ---
		MHPageHeaderFixed*	nextPage ;		// [4-8]�ׂ̃y�[�W

	//	char*				startPtr ;		// [4-8]�����ɋL������i�m�ی�͌Œ�j
		char*				endPtr ;		// [4-8]�|�C���^�̏I�[�i�m�ی�͌Œ�A���̃A�h���X���͔͈̂͊O�j
		l2d_uint32			size ;			// [4-8]���̃y�[�W�̃������T�C�Y�i�m�ی�͌Œ�j

		char*				curPtr ;		// [4-8]���Ɋm�ۂł���|�C���^(AllocHeader�̐擪�ɂȂ�)�i�ρj
		l2d_uint32			rest ;			// [4-8]���̃y�[�W�̎c��T�C�Y�i�ρj
		l2d_uint32			pageNo ;		// [4]���̃y�[�W�̒ʂ��ԍ��iID�j


		//--- static ---
		static int			pageAmount ;	// �S�̂̃y�[�W���i�J�����Ă����炳�Ȃ��j
	};



}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__