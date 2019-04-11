/**
 *  Live2DMotion.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_MOTION_H__
#define __LIVE2D_MOTION_H__


#include "../Live2D.h"
#include "../memory/LDObject.h"
#include "../ALive2DModel.h"
#include "../type/LDVector.h"
#include "AMotion.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d
{

	class Motion ;
	class MotionQueueEnt ;
	class MemoryParam ;

	
	class Live2DMotion : public live2d::AMotion
	{
	public:
		// Constructor
		Live2DMotion();

		// Destructor
		virtual ~Live2DMotion();
		
	public:
		//  �p�����[�^�X�V
		virtual void updateParamExe( live2d::ALive2DModel * model , long long timeMSec , float weight , MotionQueueEnt *motionQueueEnt) ;

		//  ���[�v�ݒ�
		void setLoop( bool _loop ){ this->loop = _loop ; }

		//  ���[�v���邩?
		bool isLoop( ){ return this->loop ; }
		
		//  ���[�v���Ƀt�F�[�h�C�����L�����ǂ�����ݒ肷��B�f�t�H���g�ł͗L���B
		void setLoopFadeIn( bool _loopFadeIn ){ this->loopFadeIn = _loopFadeIn ; }
		
		//  ���[�v���̃t�F�[�h�C�����L�����ǂ������擾
		bool isLoopFadeIn( ){ return this->loopFadeIn ; }
		
		//  ���[�V�����̒�����Ԃ��B���[�v�̎���-1
		virtual int getDurationMSec() ;
		
		
		//  mtn�t�@�C���Œ�`����Ă����A�̃��[�V�����̒�����Ԃ�
		virtual int getLoopDurationMSec()
		{
			return loopDurationMSec ; 
		}

		//  ���[�V�����f�[�^�����[�h(2byte������Ή�)
		static Live2DMotion * loadMotion( const live2d::LDString & filepath ) ;
		//  ���[�V�����f�[�^�����[�h(2byte������Ή�)
		static Live2DMotion * loadMotion( const void * buf , int bufSize ) ;
		
		void dump() ;
		

		//SDK2.0
		// �p�����[�^���Ƃ̃t�F�[�h�̐ݒ�
		//void setParamFadeIn(const char* paramID,int value);
		//void setParamFadeOut(const char* paramID,int value);
		void setParamFadeIn(LDString paramID,int value);
		void setParamFadeOut(LDString paramID,int value);
		
		int getParamFadeIn(const char* paramID);
		int getParamFadeOut(const char* paramID);


	private:
		live2d::LDVector<Motion*>* 	motionsPtr ;			//  ���[�V�������X�g

		float 						srcFps ;				//  ���[�h�����t�@�C����FPS�B�L�q��������΃f�t�H���g�l15fps�ƂȂ�
		int 						maxLength ;				//  

		int 						loopDurationMSec ;		//  mtn�t�@�C���Œ�`������A�̃��[�V�����̒���
		bool 						loop ;					//  ���[�v���邩?
		bool 						loopFadeIn;				//  ���[�v���Ƀt�F�[�h�C�����L�����ǂ����̃t���O�B�����l�ł͗L���B
		int 						objectNoForDebug ;		//  �f�o�b�O�p
		
		float 						lastWeight ;
		live2d::MemoryParam*		memoryManagement ;
		live2d::AMemoryHolder*		memoryHolderFixedMain ;
	};




#ifndef __SKIP_DOC__

	/***************************************************************************
	��̃p�����[�^�ɂ��ẴA�N�V������`
	***************************************************************************/
	class Motion : public live2d::LDObject
	{
	public:
		static const int MOTION_TYPE_PARAM = 0 ;
		static const int MOTION_TYPE_PARTS_VISIBLE = 1 ;
		static const int MOTION_TYPE_PARAM_FADEIN = 2 ;//SDK2.0
		static const int MOTION_TYPE_PARAM_FADEOUT = 3 ;//SDK2.0
		
		// �����ύX�s�i < �Ŕ�r���Ă��邽�� �j
		static const int MOTION_TYPE_LAYOUT_X = 100 ;
		static const int MOTION_TYPE_LAYOUT_Y = 101 ;
		static const int MOTION_TYPE_LAYOUT_ANCHOR_X = 102 ;
		static const int MOTION_TYPE_LAYOUT_ANCHOR_Y = 103 ;
		static const int MOTION_TYPE_LAYOUT_SCALE_X = 104 ;
		static const int MOTION_TYPE_LAYOUT_SCALE_Y = 105 ;
		
	public:
		Motion(live2d::MemoryParam* memParam) ;
		virtual ~Motion() ;
		
	public:
		int getParamIndex( live2d::ALive2DModel* model ) ;

	public:
		live2d::LDString * 			paramIDStr ;				//  �p�����[�^ID������
		
		live2d::ParamID* 			cached_paramID ;			//  �����̃��f���Ń��[�V�������g���܂킷��Index�̃L���b�V�����ł��Ȃ�
		int  						cached_paramIndex ;			//
		live2d::ALive2DModel* 		cached_model_ofParamIndex ;

		live2d::LDVector<float> 	values ;
		
		int 						motionType ;				//  ���[�V�����^�C�v

		//SDK2.0
		int 	fadeInMsec ;		// ���̃p�����[�^�̃t�F�[�h�C�����鎞�ԁB�����l�̂Ƃ��̓��[�V�����S�̂̐ݒ��D�悷��
		int 	fadeOutMsec ;		
	};

#endif // __SKIP_DOC__

}
//--------- LIVE2D NAMESPACE ------------

#endif		// __LIVE2D_MOTION_H__