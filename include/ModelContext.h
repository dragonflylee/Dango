/**
 *  ModelContext.h
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_MODEL_CONTEXT_H__
#define __LIVE2D_MODEL_CONTEXT_H__


#include "Live2D.h"
#include "memory/LDObject.h"
#include "type/LDVector.h"
#include "DEF.h"

#if defined(L2D_TARGET_WIN_GL) || defined(L2D_TARGET_ANDROID_ES2) || defined(L2D_TARGET_IPHONE_ES2)
#include "graphics/ClippingManagerOpenGL.h"
#endif

#if L2D_VERBOSE
#include "util/UtDebug.h"
#endif

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	
	class ParamID ;
	class BaseDataID ;
	class PartsDataID ;
	class DrawDataID ;
	class IBaseData ;
	class IDrawData ;
	class PartsData ;
	class ALive2DModel ;
	class DrawParam ;
	class IBaseContext ;
	class IDrawContext ;
	class PartsDataContext ;
		
	#define _PARAM_FLOAT_MIN_ (-1000000) 
	#define _PARAM_FLOAT_MAX_ ( 1000000) 
	
	
	class ModelContext : public live2d::LDObject 
	{
	public:
		static const unsigned short NOT_USED_ORDER ; 
		static const unsigned short NO_NEXT ; 
	
	
	public:
		ModelContext(ALive2DModel *model) ;
		virtual ~ModelContext(void) ;
	
	public:
		// ���
		void release() ;
		
		// �������Bsetup()�̑O�Ɉ�x�����ĂԁB�\�����ς�����ꍇ���ĂԁB
		void init() ;
	
		// ���������ꊇ�Ǘ����邽�߂�MemoryParam��Ԃ�
		MemoryParam*  getMemoryParam(){ return memoryManagement ; }
	
		// �������̃o�[�W�������擾�i�ď����������ƃL���b�V���������ɂȂ邽�߁j
		inline int getInitVersion() const { return initVersion ; }
	
		inline bool requireSetup() const { return needSetup ; }
		
		// ���݂̃p�����[�^�l�ŕ`��p�̃f�[�^�𐶐�����
		bool update() ;
	
		// �N���b�s���O�p�̃}�X�N�𐶐�
		void preDraw(DrawParam &dp) ;
	
		// �`��
		void draw(DrawParam &dp) ;
	
	
				
		inline bool isParamUpdated( int paramIndex ) const { 
# if L2D_FORCE_UPDATE
			return true ;
# else
			return (*updatedFloatParamFlagsPtr)[ paramIndex ] == PARAM_UPDATED ; 
# endif
		}

		// paramID����A�N�Z�X�p��paramIndex���擾
		int getParamIndex( ParamID * paramID )   ;

		// BaseDataID����A�N�Z�X�p�� baseIndex ���擾����
		int getBaseDataIndex( BaseDataID * baseID ) ;

		// PartsDataID����A�N�Z�X�p�� partsIndex ���擾����
		int getPartsDataIndex( PartsDataID * partsID ) ;

		// drawDataID����A�N�Z�X�p�� drawDataIndex ���擾����
		int getDrawDataIndex( DrawDataID * drawDataID ) ;
	
		inline unsigned short * getTmpPivotTableIndicesRef(){ return tmpPivotTableIndices_short ; }
		inline float * getTmpT_ArrayRef(){ return tmpT_array ; }
		
	
		// float�p�����[�^�ǉ�
		int addFloatParam( ParamID * id , l2d_paramf value , l2d_paramf min , l2d_paramf max ) ;
	
	
		void setBaseData( unsigned int baseDataIndex , IBaseData * baseData ) ;
	
	
		void setParamFloat( unsigned int paramIndex , l2d_paramf value ) ;
	
		float getParamMax( unsigned int paramIndex ){ return (*floatParamMaxListPtr)[paramIndex] ; }
		float getParamMin( unsigned int paramIndex ){ return (*floatParamMinListPtr)[paramIndex] ; }
		
		// �O��saveParam�����p�����[�^�l�𕜌��isaveParam����Ă��Ȃ���Ή������Ȃ��j
		void loadParam() ;

		// ���݂̃p�����[�^�l��ۑ�
		void saveParam() ;
	
		// �p�[�c�̕s�����x���Z�b�g
		void setPartsOpacity( int partIndex , float opacity ) ;
	
		// �p�[�c�̕s�����x�擾
		float getPartsOpacity( int partIndex ) ;
		
	
		
		inline IBaseData* getBaseData( unsigned int baseDataIndex )
		{

	# if L2D_RANGE_CHECK		
			if( baseDataIndex >= baseDataListPtr->size() )
			{
	
				L2D_THROW( "out of range ModelDrawContext@getBaseData()" ) ;
			}
	# endif
			return (*baseDataListPtr)[ baseDataIndex ] ;// �͈͊O�͖���`�i�O���Ń`�F�b�N����j
		}

		
		inline IDrawData* getDrawData( unsigned int drawDataIndex )
		{
	# if L2D_RANGE_CHECK		
			if( drawDataIndex >= (*drawDataListPtr).size() )
			{
	
				L2D_THROW( "out of range ModelDrawContext@getBaseData()" ) ;
			}
	# endif
			return (*drawDataListPtr)[ drawDataIndex ] ;// �͈͊O�͖���`�i�O���Ń`�F�b�N����j
		}
	
		
		inline IBaseContext* getBaseContext( unsigned int baseDataIndex )
		{
			return (*baseContextListPtr)[ baseDataIndex ] ;// �͈͊O�͖���`�i�O���Ń`�F�b�N����j
		}

		
		inline IDrawContext* getDrawContext( unsigned int drawDataIndex )
		{
			return (*drawContextListPtr)[ drawDataIndex ] ;// �͈͊O�͖���`�i�O���Ń`�F�b�N����j
		}

		
		inline PartsDataContext* getPartsContext( unsigned int partsDataIndex )
		{
			return (*partsContextListPtr)[ partsDataIndex ] ;// �͈͊O�͖���`�i�O���Ń`�F�b�N����j
		}

		
		inline int getBaseDataCount(){	return (int)baseDataListPtr->size() ; }

		
		inline int getDrawDataCount(){	return (int)drawDataListPtr->size() ; }

		
		inline int getPartsDataCount(){	return (int)partsDataListPtr->size() ; }
		
		// parts�f�[�^���擾���܂�
		inline l2d_paramf getParamFloat( unsigned int paramIndex )
		{

	# if L2D_RANGE_CHECK
			if( paramIndex >= floatParamListPtr->size() )
			{
	
				L2D_THROW( "out of range ModelDrawContext@getParamFloat()" ) ;
			}
	# endif
			return (*floatParamListPtr)[ paramIndex ] ;
		}
		
		// DirectX�f�o�C�X���X�g���ɌĂяo��
		void deviceLost( ) ;
		
		/*
		 * ZBuffer��ݒ肷��
		 *
		 * �ʏ�`��ɂ͕K�v�Ȃ��̂�Z�l�͐ݒ肳��Ȃ����A
		 * �t�F�[�h�C���E�A�E�g�Ȃǂ̓����x��`�悷��ꍇ�Ɏg���܂��B
		 * update()��draw()�̊ԂŌĂяo���ĉ������B
		 * 
		 * startZ	��Ԕw�ʂɂ���|���S����Z�l
		 * stepZ	Z�l���X�V����X�e�b�v��
		 * 
		 */
		void updateZBuffer_TestImpl( float startZ , float stepZ ) ;
	
	#if L2D_SAMPLE
		void DUMP_PARAMS() ;
	#endif
	
	private:
		enum { PARAM_NOT_UPDATED = 0,	PARAM_UPDATED	};
		
		//Prevention of copy Constructor
		ModelContext( const ModelContext & ) ;				
		ModelContext& operator=( const ModelContext & ) ; 	

		void initValue();
		
	private:	
		bool 							needSetup ;						// �ŏ���setup������ł��Ȃ������ꍇ true
		ALive2DModel * 					model ;							// 
	
	
		int 							initVersion ;					// init���s�����o�[�W�����B���̃o�[�W�������قȂ�ꍇ�́A�L���b�V�����ꂽindex�l�Ȃǂ������Ƃ݂Ȃ�
		// ------------ �p�����[�^�l�ifloat�j ------------
		LDVector<ParamID *>*			floatParamIDListPtr ;			// ������Ȃ�
		LDVector<l2d_paramf>*			floatParamListPtr ;				// float �^�̃p�����[�^�l�e�[�u���iparamIndex�ŃA�N�Z�X����j
		LDVector<l2d_paramf>*			lastFloatParamListPtr ;			// �O��upate���̃p�����[�^�l�e�[�u���iparamIndex�ŃA�N�Z�X����j0.9.00b19 2011/10/27
		
		//-- 
		LDVector<l2d_paramf>*			floatParamMinListPtr ;			// float �^�̃p�����[�^�A�ŏ��l�e�[�u���iparamIndex�ŃA�N�Z�X����j
		LDVector<l2d_paramf>*			floatParamMaxListPtr ;			// float �^�̃p�����[�^�A�ő�l�e�[�u���iparamIndex�ŃA�N�Z�X����j
		
		LDVector<l2d_paramf>*			savedFloatParamListPtr ;		// saveParam() �ɂ��ۑ����ꂽ��Ԃ�float �^�̃p�����[�^�l�e�[�u���iparamIndex�ŃA�N�Z�X����j
	
		// �X�V���ꂽ�p�����[�^�̃t���O�𗧂Ă�  updatedFloatParamFlags[paramIndex] = 0 or 1(updated)
		LDVector<char>*					updatedFloatParamFlagsPtr ;		// bool�t���O�͗ǂ��Ȃ��̂�unsigned char
		
		// ------------ setup�������̂��߂� ------------
		// ���W�ϊ� 
		// 	�\�߈ˑ��֌W�̖��������ŕ��ׂĂ������ƂŁAsetup����OnDemandBuilder��s�v�ɂ���
		// 	index�ŃA�N�Z�X���邽��init()�ȊO�ŏ����ύX�͂��Ȃ�
		LDVector<IBaseData*>*			baseDataListPtr ;				// ������Ȃ�(Model�ȉ��ɏ����j
		
		// 	visible�� drawData�݂̂����X�g������Bsetup���̃I�[�_�[�Ȃǂ̐���ł͂��� index ��p����
		LDVector<IDrawData*>*			drawDataListPtr ;				// ������Ȃ�(Model�ȉ��ɏ����j
	
		LDVector<PartsData*>*			partsDataListPtr ;				// ������Ȃ�(Model�ȉ��ɏ����j
		
		// --- context data (�`��p�̉��Z���ʂ̃��X�g)---
		LDVector<IBaseContext*>*		baseContextListPtr ;			// �������
		LDVector<IDrawContext*>*		drawContextListPtr ;			// �������
		LDVector<PartsDataContext*>*	partsContextListPtr ;			// �������
	
		// �I�[�_�[�Ǘ��p�i�����Ƀ����j
		// �Y�����͓��� , 0�͔ԕ�
		LDVector<unsigned short>*		orderList_firstDrawIndexPtr ;	// order 1000 �����I�[�_�[�̍ŏ��� sub list��index
		LDVector<unsigned short>*		orderList_lastDrawIndexPtr  ;	// order 1000 �����I�[�_�[�̍Ō�� sub list��index
		
		// �Y������ drawDataList �Ɠ���
		LDVector<unsigned short>*		nextList_drawIndexPtr ;			// drawData��index�ƈ�v����B�����lNO_NEXT
		
		// ----------- ��Ԃ̕⏕�p�ꎟ�f�[�^�i�}���`�X���b�h���Ή��j -----------
		unsigned short 					tmpPivotTableIndices_short [DEF::PIVOT_TABLE_SIZE] ;	// ���ݕ�Ԃ��悤�Ƃ��Ă���v�f�̊e�s�{�b�g�ԍ����L�^����(�Ō��-1�F�ԕ�)
		float 							tmpT_array[DEF::MAX_INTERPOLATION*2] ;					// �����傫�ڂɊm��
	
		MemoryParam*					memoryManagement ;
		AMemoryHolder*					memoryHolderFixedMain ;
		AMemoryHolder*					memoryHolderFixedGPU;

#if defined(L2D_TARGET_WIN_GL) || defined(L2D_TARGET_ANDROID_ES2) || defined(L2D_TARGET_IPHONE_ES2)
		ClippingManagerOpenGL *clipManager;
#endif
		
	};
}
//------------ LIVE2D NAMESPACE ------------

#endif		// __LIVE2D_MODEL_CONTEXT_H__