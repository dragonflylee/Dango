/**
 * ParamPivots.h
 *
 *
 * ��ԉ\�� IDrawData / IBaseData�ŗ��p�����
 * �p�����[�^���ƂɁA�s�{�b�g���Ƃ�l��ݒ肷��B
 *
 * ��j ANGLE_X ( 0 ) => pivots { -30 , 0 , 30 }
 *
 * �G�f�B�^�ȊO�ł͐ÓI�Ȓl�ƂȂ�B
 *
 * �A���A�`��p�̈ꎞ�f�[�^��ێ����邱�Ƃ�����B
 *
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_PARAMPIVOTS_H__
#define __LIVE2D_PARAMPIVOTS_H__


#ifndef __SKIP_DOC__


#include "../Live2D.h"
#include "../type/LDVector.h"
#include "../ModelContext.h"
#include "../io/ISerializableV2.h"


//------------ LIVE2D NAMESPACE ------------
namespace live2d
{
	class ParamID ;
	
	/**********************************************************************************************
	@brief	�p�����[�^�̃L�[�̊Ǘ��N���X
	**********************************************************************************************/
	class ParamPivots : public ISerializableV2 
	{
	public:
		static const int PARAM_INDEX_NOT_INIT = -2 ;	// �l�����݂��Ȃ��ꍇ�� -1�����̂� -2
	
	public:
		ParamPivots();
		virtual ~ParamPivots();

	public:
		virtual void readV2(BReader & br , MemoryParam* memParam ) ;
	
	public:
		
		inline int getParamIndex( int initVersion )
		{ 
			if( this->indexInitVersion != initVersion ) 
			{
				_paramIndex = PARAM_INDEX_NOT_INIT ;
			}
	
			return _paramIndex ;
		}

		
		inline void setParamIndex_(int index , int initVersion )
		{ 
			this->_paramIndex = index ;
			this->indexInitVersion = initVersion ;
		}
	
		
		inline ParamID * getParamID() const { return paramID ; }
	
		
		inline void setParamID(ParamID * v){ paramID = v ; }
	
		
		inline int getPivotCount(){ return pivotCount ; }
	
	
		
		inline l2d_paramf * getPivotValue(){ return pivotValue ; } 
	
		
		inline void setPivotValue(int _pivotCount , l2d_paramf * _values)
		{
			this->pivotCount = _pivotCount ;
			this->pivotValue = _values ; 
		} 
	
		
		inline int getTmpPivotIndex(){ return tmpPivotIndex ; } 
	
		
		inline void setTmpPivotIndex(int v){ tmpPivotIndex = v ; } 
	
		
		inline float getTmpT(){ return tmpT ; } 
	
		
		inline void setTmpT(float t){ tmpT = t ; } 
	
		
#if L2D_SAMPLE
		void DUMP() ;
#endif
	
	
	private:
		//Prevention of copy Constructor
		ParamPivots( const ParamPivots & ) ;			
		ParamPivots& operator=( const ParamPivots & ) ;
	
		
	private:
		//---- Field ----
		int				pivotCount ;
		ParamID*		paramID ;			// �p�����[�^ID�͉�����Ȃ��Ă悢
		l2d_paramf*		pivotValue ;		// ld_paramf�̔z��[] �����̒l���s�{�b�g�Ɏ��i�O���ŊǗ����邽��delete�s�v)
		
		// ---- �v�Z���̈ꎞ���
		int				_paramIndex ;		// �������O��PARAM_INDEX_NOT_INIT
		int				indexInitVersion ;	// index���L���b�V���������� modelDrawContext�̃o�[�W����
		
		int				tmpPivotIndex ;		// �ꎞ�s�{�b�g
		float			tmpT ;				// tmpPivotIndex , tmpPivotIndex+1 �̓�����@�i0�̏ꍇ�͓������Ȃ��j
	};
} 
//------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif		// __LIVE2D_PARAMPIVOTS_H__