/**
 *  MotionQueueManager.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_MOTION_QUEUE_MANAGER_H__
#define __LIVE2D_MOTION_QUEUE_MANAGER_H__


#include "../memory/LDObject.h"
#include "../ALive2DModel.h"

#include "../type/LDVector.h"
#include "AMotion.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	class Live2DMotion ;
	class MotionQueueEnt ;
	class MemoryParam ;


	
	class MotionQueueManager : public live2d::LDObject 
	{
	public:
		MotionQueueManager();
		virtual ~MotionQueueManager();
		
		// �w��̃��[�V�������J�n����
		// �����^�C�v�̃��[�V���������ɂ���ꍇ�́A�����̃��[�V�����ɏI���t���O�𗧂āA�t�F�[�h�A�E�g���J�n������B
		int startMotion( AMotion * motion , bool autoDelete ) ;
		
		// ���f���̃p�����[�^��ݒ�A�X�V����i�����𔽉f����j
		bool updateParam( live2d::ALive2DModel * model ) ;

		//  �S�Ẵ��[�V�������I�����Ă��邩
		bool isFinished() ;
		
		// �����Ŏw�肵�����[�V�������I�����Ă��邩��Ԃ��B(0.8.09�ǉ��j
		bool isFinished(int motionQueueEntNo ) ;


		//  �S�Ẵ��[�V�������~����
		void stopAllMotions() ;

		//  ���[�V�����̊J�n�E�I�����_���v����
		void setMotionDebugMode( bool f ){ this->motionDebugMode = f ; }

		//  MotionQueueEnt���擾
		MotionQueueEnt* getMotionQueueEnt( int entNo ) ;
		
	#if L2D_VERBOSE
		void DUMP() ;
	#endif

	private:
		live2d::LDVector<MotionQueueEnt *> *motions ;

		bool motionDebugMode ;// ���[�V�����̍Đ��E��~���_���v����t���O

		live2d::MemoryParam*  memoryManagement ;
		live2d::AMemoryHolder*	memoryHolderFixedMain ;
	};

}
//--------- LIVE2D NAMESPACE ------------

#endif		// __LIVE2D_MOTION_QUEUE_MANAGER_H__