/*
 * BaseDataID.h
 *
 *�@�I�[�o�[���C�h���Ȃ�����
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_BASE_DATA_ID_H__
#define __LIVE2D_BASE_DATA_ID_H__


#include "../Live2D.h"
#include "../type/LDVector.h"
#include "ID.h"

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{
	
	class BaseDataID : public ID
	{
	public:
		const static int L2D_BASEDATA_ID_INITIAL_CAPACITY = 128 ;
	
	public:
	
		// ID���擾����
		// �E����̒l�����S�Ă�ID���A�����|�C���^���w�����Ƃ�ۏ؂��� �i����̊m�F���@== ��r �ŗǂ��j \n
		// �ELive2D::dispose()���ɉ�������
		static BaseDataID * getID(const live2d::LDString &str ) ;
	
		static BaseDataID * getID(  const RefString& refStr ) ;
	
		static BaseDataID * DST_BASE_ID()
		{
			if( dstBaseId == NULL )
			{
				dstBaseId = getID("DST_BASE");
			}
			return dstBaseId ;
		}
	
		
		// �����[�X�p���\�b�h�iLive2D::dipose����Ă΂��)
		static void releaseStored_notForClientCall() ;
	
	public:
		
		const char * toChar(){ return id.c_str() ; }
		
	private:	
		BaseDataID() ;									
		BaseDataID(const live2d::LDString &str );		
		BaseDataID( const char* str ) ;					
		BaseDataID( const RefString& refStr ) ;			
	
		//Prevention of copy Constructor
		BaseDataID( const BaseDataID & ) ;				
		BaseDataID& operator=( const BaseDataID & ) ; 	
	
	
		virtual ~BaseDataID();
		
	private:
		static LDVector<BaseDataID*>* 		idlist_BaseDataID ;
		static BaseDataID * 				dstBaseId ;
		
	private:
		live2d::LDString 					id ;				// �������ɕ�������Ă��邽�߁ADestructor�ŊJ������
	};
	
}

#endif		// __LIVE2D_BASE_DATA_ID_H__
//------------ LIVE2D NAMESPACE ------------
