/**
 *  Live2DMotionBin.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#pragma once


#include "../Live2D.h"
#include "../ALive2DModel.h"
#include "../type/LDVector.h"
#include <vector>
#include "AMotion.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	class MotionBin ;
	class MotionQueueEnt ;



	
	class Live2DMotionBin : public AMotion{
	public:

		// loadMotion�Ńo�C�i����n�����ꍇ�ɁA��R�������ǂ̂悤�ɂ��邩���w�肷��
		typedef enum {
			// �f�X�g���N�g���Ɍ��f�[�^��j�����Ȃ�
			DO_NOTHING_ON_DESTRUCT ,		// �O���Ŕj��
			DUPLICATE_AND_DESTRUCT ,		// �n�����f�[�^�𕡐����Ďg���i�n�����f�[�^�ɂ͉������Ȃ��j

			// �f�X�g���N�g���Ɏw��̕����Ŕj������
			FREE_ON_DESTRUCT ,				// free()�Ŕj��
			DELETE_ARRAY_ON_DESTRUCT , 		// delete[]�Ŕj��
			DELETE_NORMAL_ON_DESTRUCT  , 	// �ʏ��delete �Ŕj��
			
			L2D_FREE_ON_DESTRUCT ,			// Live2D�Ǝ��̃������Ǘ������Ŕj��
			UTFILE_RELEASE_ON_DESTRUCT		// UtFile::releaseLoadBuffer()�Ŕj��
		} BufType ;



		Live2DMotionBin();
		virtual ~Live2DMotionBin();
		
		virtual void updateParamExe( live2d::ALive2DModel * model , long long timeMSec , float weight , MotionQueueEnt *motionQueueEnt) ;


		void setLoop( bool _loop ){ this->loop = _loop ; }
		bool isLoop( ){ return this->loop ; }
		
		/**
		 * ���[�V�����̒�����Ԃ��i���[�v�̂Ƃ��� -1�j
		 * -1�̂Ƃ��͏I���Ȃ������ƂȂ�
		 */
		virtual int getDurationMSec() ;
		
		
		/** 
		 * mtn�t�@�C���Œ�`������A�̃��[�V�����̒����i�����[�v�P�񕪂̒����j��Ԃ� (0.9.00b8)
		 */
		virtual int getLoopDurationMSec(){ 
			return loopDurationMSec ; // mtn�t�@�C���Œ�`������A�̃��[�V�����̒���
		}

		/*
		 * ���[�V�����f�[�^�����[�h����
		 * 
		 * �Q�o�C�g�����͖��Ή�
		 */
		static Live2DMotionBin * loadMotion( const l2d_string & filepath ) ;

		/*
		 * ���[�V�����f�[�^���o�C�g�񂩂烍�[�h����
		 *
		 * bufType  buf�̃|�C���^���ǂ̂悤�ɔj�����邩�|�C���^�m�ۂ̕����ɂ��킹�Ĉȉ�����w�肷��B
		 *
		 *   BufType::DO_NOTHING_ON_DESTRUCT ,		// �f�X�g���N�g���ɔj�����Ȃ��i�O���Ŕj���j
		 *	 BufType::DUPLICATE_AND_DESTRUCT ,		// �n�����f�[�^�𕡐����Ďg���i�n�����f�[�^�ɂ͉������Ȃ��j
		 *	 BufType::FREE_ON_DESTRUCT ,			// �f�X�g���N�g����free()�Ŕj��
		 *	 BufType::DELETE_ARRAY_ON_DESTRUCT , 	// �f�X�g���N�g����delete[]�Ŕj��
		 *	 BufType::DELETE_NORMAL_ON_DESTRUCT  , // �f�X�g���N�g���ɒʏ��delete �Ŕj��
		 *	 BufType::UTFILE_RELEASE_ON_DESTRUCT	// �f�X�g���N�g����UtFile::releaseLoadBuffer()�Ŕj��
		 *
		 * deleteBufOnDestructor buf��Live2DMotionBin��Destructor�Ŕj������ꍇ��true
		 */
		static Live2DMotionBin * loadMotion( 
						const void * buf ,					// ���[�V�����t�@�C�������[�h�����|�C���^
						int bufSize ,						// ���[�V�����̃f�[�^�T�C�Y(�o�C�g�j
						Live2DMotionBin::BufType  bufType	// buf�̎g�����A�j���̎d�����w�肷��
					) ;
		


		void dump() ;
		


	private:
		const void *				buf ;// ���[�h���ꂽ�o�C�i���BdeleteBufOnDestructor��true�̏ꍇ�ɔj��

		Live2DMotionBin::BufType	bufType ;// buf���ǂ̂悤�Ɉ������ienum BufType �Ŏw�肷��j
	//	bool						deleteBufOnDestructor ;// Destructor�Ń|�C���^��j������ꍇ��true

		//---- 
		MotionBin**					motionPtrArray ;
		int							paramCount ;//count of MotionBin

		// ---- ���[�h�f�[�^
		float 						srcFps ;// ���[�h�����t�@�C����FPS�B�L�q��������΃f�t�H���g�l15fps�ƂȂ�
		int 						maxLength ;

		int 						loopDurationMSec ;// mtn�t�@�C���Œ�`������A�̃��[�V�����̒���
		bool 						loop ;
		int 						objectNoForDebug ;
		
		float 						lastWeight ;
		live2d::MemoryParam*		memoryManagement ;
		live2d::AMemoryHolder*		memoryHolderFixedMain ;

	};




#ifndef __SKIP_DOC__

	//=================================================
	//  	��̃p�����[�^�ɂ��ẴA�N�V�������`����N���X
	//=================================================
	class MotionBin : public live2d::LDObject
	{
	public:
		static const int MOTION_TYPE_PARAM = 0 ;
		static const int MOTION_TYPE_PARTS_VISIBLE = 1 ;
		
		// �����ύX�s�i < �Ŕ�r���Ă��邽�� �j
		static const int MOTION_TYPE_LAYOUT_X = 100 ;
		static const int MOTION_TYPE_LAYOUT_Y = 101 ;
		static const int MOTION_TYPE_LAYOUT_ANCHOR_X = 102 ;
		static const int MOTION_TYPE_LAYOUT_ANCHOR_Y = 103 ;
		static const int MOTION_TYPE_LAYOUT_SCALE_X = 104 ;
		static const int MOTION_TYPE_LAYOUT_SCALE_Y = 105 ;
		
		MotionBin(void);
		virtual ~MotionBin(void);
		
		int getParamIndex( live2d::ALive2DModel* model ) ;

	public:
		//--- fields ---
		l2d_string * 			paramIDStr ;

		// �����̃��f���Ń��[�V�������g���܂킷��Index�̃L���b�V�����ł��Ȃ�
		live2d::ParamID* 		cached_paramID ;
		int  					cached_paramIndex ;//
		live2d::ALive2DModel* 	cached_model_ofParamIndex ;

	//	l2d_vector<float> 		values ;

		// �f�[�^�̎��������Afloat[] short[]�^�̓��ގg��������
		// �ʏ�̓f�[�^�T�C�Y�����������邽�� min,max�Ԃ�short(65536�i�K16bit)�ɕ�������
		// �l�̃����W���L���ꍇ�i min,max = -1000,1000�̂悤�ȏꍇ�j��
		// ��萸�x�̍���float[](������23bit)��p���Ē��ڒl��ݒ肷��

		// min,max�̊Ԃ�short�l��ݒ肷��ꍇ�� true
		bool					isShortArray ;

		void*					valuePtr ;// float[]�܂���short[]���w���B�t�@�C���̃o�C�i���𒼐ڎQ�Ƃ���̂Ŕj�����Ȃ�
		float					minValue ;
		float					maxValue ;
		
		int						valueCount ;
		
		int 					motionType ;
	};

#endif // __SKIP_DOC__

}
//--------- LIVE2D NAMESPACE ------------