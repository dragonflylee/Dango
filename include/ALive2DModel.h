/**
 *  ALive2DModel.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_ALIVE_2D_MODE_H__
#define __LIVE2D_ALIVE_2D_MODE_H__


#include "type/LDVector.h"
#include "Live2D.h"
#include "memory/LDObject.h"
#include "ModelContext.h"
#include "id/ParamID.h"
#include "id/DrawDataID.h"
#include "id/PartsDataID.h"

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	class ModelImpl ;
	class DrawParam ;
	
	
	class ALive2DModel : public LDObject
	{
	public:
		static const int FILE_LOAD_EOF_ERROR = 1 ;			// �f�[�^�̖�����ǂݍ��߂Ȃ��ꍇ�̃G���[
		static const int FILE_LOAD_VERSION_ERROR = 2 ;		// ���[�h�ł��Ȃ��o�[�W�����̃t�@�C��
		
		static int INSTANCE_COUNT ;	
	
	
	public:	
		ALive2DModel() ;
		virtual ~ALive2DModel() ;
	
		
	
		// �p�����[�^�̒l���擾����
		float getParamFloat( const char * paramID ) ;
		
		// �p�����[�^�̒l��ݒ肷��
		void setParamFloat( const char * paramID , float value , float weight = 1.0f ) ;
		
		// ���݂̒l�ɑ΂��āA�d�݂����Ēl��������B weight=1�̂Ƃ��͎w�肵���l�����̂܂܉�����B
		void addToParamFloat( const char * paramID , float value , float weight = 1.0f ) ;
	
		// ���݂̒l�ɑ΂��āA�d�݂����Ēl���|�����킹��Bweight=1�̂Ƃ��͎w�肵���l�����̂܂܊|�����킹��B
		void multParamFloat( const char * paramID , float mult , float weight = 1.0f  ) ;
		
		
		inline int getParamIndex( const char * paramID )
		{
			return modelContext->getParamIndex(ParamID::getID(paramID)) ;
		}
	
		
		inline int getParamIndex( ParamID* paramID )
		{
			return modelContext->getParamIndex( paramID ) ;
		}	

		inline float getParamFloat( int paramIndex )
		{
			return modelContext->getParamFloat( paramIndex ) ;
		}
	
		inline void setParamFloat( int paramIndex , float value , float weight = 1.0f)
		{
			if(weight==1)modelContext->setParamFloat( paramIndex , value ) ;
			else modelContext->setParamFloat( paramIndex ,  modelContext->getParamFloat( paramIndex )*(1-weight) + value*weight ) ;
		}
	
		inline void addToParamFloat( int paramIndex, float value , float weight = 1.0f)
		{
			modelContext->setParamFloat( paramIndex , modelContext->getParamFloat( paramIndex ) + value*weight ) ;
		}
	
		inline void multParamFloat( int paramIndex , float mult , float weight = 1.0f)
		{
			modelContext->setParamFloat( paramIndex , modelContext->getParamFloat( paramIndex )*(1.0f + (mult - 1.0f)*weight ) ) ;
		}
	
		// �O��saveParam�����Ƃ��̃p�����[�^�l�𕜌�����isaveParam����Ă��Ȃ���Ή������Ȃ��j
		void loadParam() ;

		// ���݂̃p�����[�^�l���ꎞ�I�ɋL������BloadParam()�ŕ����ł���B
		void saveParam() ;
	
		// �f�[�^�\���̕ύX�A�p�[�c�����ւ��Ȃǂ��s�����ꍇ�ɌĂ�
		virtual void init() ;
	
		// �p�����[�^�ύX�Ȃǂ��s�����ꍇ�ɌĂ�
		virtual void update() ;

		// �`�施�߁i�v���b�g�t�H�[�����ƂɃI�[�o�[���C�h�����j
		virtual void draw()  ;
		
		// �p�[�c�̕s�����x��ݒ�
		void setPartsOpacity( const char *partsID , float opacity ) ;
		void setPartsOpacity( int partsIndex , float opacity ) ;

		// �p�[�c�̕s�����x���擾
		float getPartsOpacity( const char *partsID ) ;
		float getPartsOpacity( int partsIndex ) ;
		
		// �p�[�c�̕\���O���[�v�𐧌�i�񐄏��j
		void setupPartsOpacityGroup_alphaImpl( const char* paramGroup[] , float deltaTimeSec ) ;
	
		// ���f���̃f�[�^�\����ݒ�
		void setModelImpl(ModelImpl* m) ;

		// ���f���̃f�[�^�\�����擾
		ModelImpl* getModelImpl() ;
	
		
		ModelContext * getModelContext(){ return modelContext ; }

		
		int getErrorFlags(){ return Live2D::getError() ; }
		
		// �V�������p�ł���Live2D�����ŊǗ������e�N�X�`���ԍ����m��(Avatar�p)
		virtual int generateModelTextureNo() ;
		
		// Live2D�����ŊǗ������e�N�X�`���ԍ������(Avatar�p)
		virtual void releaseModelTextureNo(int no) ;
	
		// �L�����o�X�̕����擾
		float getCanvasWidth() ;

		// �L�����o�X�̍������擾
		float getCanvasHeight() ;
	
		
		virtual DrawParam* getDrawParam(){ return NULL ; }
	
		
		virtual int getDrawDataIndex( const char * drawDataID )
		{
			return modelContext->getDrawDataIndex( DrawDataID::getID(drawDataID)) ;
		}
	
		
		virtual IDrawData* getDrawData( int drawIndex )
		{
			return modelContext->getDrawData( drawIndex ) ;
		}
		
		// �`��p�̓_���Ԃ��B
		virtual l2d_pointf* getTransformedPoints( int drawIndex , int*pointCount) ;
		virtual l2d_index* getIndexArray( int drawIndex , int*polygonCount) ;
	
		// �y�񐄏��zZBuffer��ݒ肷��
		// �ʏ�A�`��ɂ͕K�v�Ȃ��̂�Z�l�͐ݒ肳��Ȃ����A�t�F�[�h�C���A�A�E�g�Ȃǂ̓����x��`�悷��ꍇ�ɕK�v�B
		void updateZBuffer_TestImpl( float startZ , float stepZ )
		{
			modelContext->updateZBuffer_TestImpl( startZ , stepZ ) ;
		}
	
		
		inline int getPartsDataIndex( const char * partsID )
		{
			return modelContext->getPartsDataIndex(PartsDataID::getID(partsID)) ;
		}
		
		
		inline int getPartsDataIndex( PartsDataID* partsID )
		{
			return modelContext->getPartsDataIndex( partsID ) ;
		}

		
		void setPremultipliedAlpha(bool b);
		bool isPremultipliedAlpha();
		
		void setAnisotropy(int n);
		int getAnisotropy();


	#if L2D_ALPHA_IMPL_LAYOUT
		// �y�������z���f���̔z�u����Ԃ�
		// �|�C���^�𒼐ڕԂ�.���e���X�V����ƒ��ڔ��f�����
		//{ x , y , anchorX , anchorY , scaleX , scaleY }
		float* getLayout_alphaImpl(){ return layout_alphaImpl ; }
	#endif
		
	#ifdef L2D_DEBUG
		void dump() ;
	#endif
	
	protected:
		//------------ static method ------------
		// 2013/04/02 beta2 �G���[�t���O��Ԃ��悤�ɏC��
		static l2d_uint32 loadModel_exe( ALive2DModel *ret , const LDString & filepath ) ;
		static l2d_uint32 loadModel_exe( ALive2DModel *ret , const void * buf , int bufSize ) ;
	
	private:
		//Prevention of copy Constructor
		ALive2DModel( const ALive2DModel & ) ;				
		ALive2DModel& operator=( const ALive2DModel & ) ;
	
		
	protected:
		//------------ field ------------
		ModelImpl*			modelImpl ;						// ���f���̃f�[�^�\��
		ModelContext*		modelContext ;					// ���f���̏��			
//		int					errorFlags ;					// �G���[�t���O(1.00beta2���j��.Live2D::setError()�ɓ���)
	
	#if L2D_ALPHA_IMPL_LAYOUT
		float layout_alphaImpl[6] ;//{ x , y , anchorX , anchorY , scaleX , scaleY }
	#endif
		
	};

} 
//------------ LIVE2D NAMESPACE ------------

#endif		// __LIVE2D_ALIVE_2D_MODE_H__