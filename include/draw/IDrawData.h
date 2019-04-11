/**
 *  IDrawData.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#ifndef __LIVE2D_IDRAWDATA_H__
#define __LIVE2D_IDRAWDATA_H__


#ifndef __SKIP_DOC__

#include "../Live2D.h"
#include "../io/ISerializableV2.h"
#include "../id/BaseDataID.h"
#include "IDrawContext.h"
#include <stdio.h>


//------------ LIVE2D NAMESPACE ------------
namespace live2d
{
	class DrawDataID ;
	class ModelContext ;
	class PivotManager ;
	class DrawParam ;

	
	class IDrawData : public ISerializableV2
	{
	public:
		static const int BASE_INDEX_NOT_INIT 	= -2 ;
		static const int DEFAULT_ORDER 			= 500 ;
		static const int TYPE_DD_TEXTURE 		= 2 ;
	public:	
		IDrawData();
		virtual ~IDrawData();
	
		virtual void readV2(BReader & br , MemoryParam* memParam ) ;
		
		// �I�[�o�[���C�h�s�v
		inline BaseDataID * getTargetBaseDataID(){ return targetBaseDataID ; } 
		inline void setTargetBaseDataID(BaseDataID *id){ targetBaseDataID = id ; } 
	
		bool needTransform()
		{ 
			return ( targetBaseDataID 
					&& (targetBaseDataID != BaseDataID::DST_BASE_ID() ) ) ;
		}
	
		//  �`��ID�擾		
		inline DrawDataID * getDrawDataID(){ return drawDataID ; } 

		//  �`��ID���Z�b�g
		inline void setDrawDataID( DrawDataID *id ){ drawDataID = id ; } 
	
		//  �s�����x�擾	
		inline float getOpacity( ModelContext &mdc , IDrawContext* cdata ){ return cdata->interpolatedOpacity ; }
	
		//  �`��I�[�_�[�擾	
		inline int getDrawOrder(ModelContext &mdc, IDrawContext* cdata){ return cdata->interpolatedDrawOrder; }

		// �N���b�s���O�}�X�N�̎擾(SDK2.1)
		inline LDVector<DrawDataID*>* getClipIDList(){ return clipIDList; }
		
		inline void setDrawOrder(LDVector<int>* orders )
		{
			for( int i = (int)orders->size()-1 ; i >= 0  ; --i )
			{
				int order = (*orders)[i] ;

				if( order < totalMinOrder ) 		totalMinOrder = order ;
				else if( order > totalMaxOrder ) 	totalMaxOrder = order ;// �����l �������Ă���ꍇ�� else if�ł悢
			}
		} 
	
		
		inline static int getTotalMinOrder(){ return totalMinOrder ; } 
		inline static int getTotalMaxOrder(){ return totalMaxOrder ; } 
		
		// �e�����isetupInterpolation/draw�j�Ŗ�����K�v�̂Ȃ��������ŏ��ɍs��
		// �e��p�����[�^���ݒ肳�ꂽ��ɌĂяo��
		virtual IDrawContext* init(ModelContext &mdc) = 0;
		
		// draw�̑O�i�K�Ƃ��āA��Ԃ��s��
		virtual void setupInterpolate(ModelContext &mdc , IDrawContext* cdata ) ;
		
		// draw�̑O�i�K�Ƃ��āA�ό`���s��
		virtual void setupTransform(ModelContext &mdc , IDrawContext* cdata ) ;
	
		// �N���b�s���O�Ȃǂ̑O�����`��
		virtual void preDraw(DrawParam& dp, ModelContext& mdc, IDrawContext* cdata) = 0;

		// �`����s��
		virtual void draw( DrawParam & dp , ModelContext &mdc , IDrawContext* cdata ) = 0 ;
	
		// IDrawData�̌^��Ԃ��BTYPE_DD_TEXTURE�Ȃ�
		virtual int getType() ;
		
		// �f�o�C�X���X�g���Ƀf�[�^��j������
		virtual void deviceLost( IDrawContext* drawContext ) {}
	
		virtual void setZ_TestImpl( ModelContext &mdc , IDrawContext* _cdata , float z ){}
	
#ifdef L2D_DEBUG
		virtual void dump() = 0 ;
#endif
	
	private:
		//Prevention of copy Constructor
		IDrawData( const IDrawData & ) ;
		IDrawData& operator=( const IDrawData & ) ; 	
	
	private:
		static int			totalMinOrder ;		//  ���s���̃f�[�^�m�ۗp(transient)
		static int			totalMaxOrder ;		//  ���s���̃f�[�^�m�ۗp(transient)
	
	protected:
		PivotManager *		pivotManager ;
		int 				averageDrawOrder ;	//  �`�揇��ύX���Ȃ������ŕ`���ꍇ�i__L2D_VERSION_STR__ 801�ȍ~�͎g��Ȃ��j
		LDVector<DrawDataID*>*	clipIDList;				//  SDK2.1 �N���b�s���O����`��I�u�W�F�N�g��ID

	private:
		DrawDataID *		drawDataID ;		//  ������Ȃ�
		BaseDataID *		targetBaseDataID ;	//  ������Ȃ�
		LDVector<int>*		pivotDrawOrder ;	//  ��ԗp��pivot��S�Ď����_�z�� [numPivots]
		LDVector<float>*	pivotOpacity ;		//  ��ԗp��pivot��S�Ď����_�z�� [numPivots]
	
		bool				dirty ;				//  �X�V�����init()���Ăяo���K�v������ꍇ�i������Ԃ��j
	};
}
//------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_IDRAWDATA_H__