/**
 *  MotionQueueEnt.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_MOTION_QUEUE_ENT_H__
#define __LIVE2D_MOTION_QUEUE_ENT_H__


#ifndef __SKIP_DOC__

#include "../memory/LDObject.h"
#include "../ALive2DModel.h"

#include "../type/LDVector.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	class AMotion ;
	class Live2DMotion ;

	/********************************************************************************
	@brief	MotionQueueManager�ōĐ����Ă���e���[�V�����̊Ǘ��N���X
	
	MotionQueueManager* mqm = ... \n
	int motionQueueEntNo = mqm->startMotion(...)\n
	MotionQueueEnt ent = mqm->getMotionQueueEnt(motionQueueEntNo)
	********************************************************************************/
	class MotionQueueEnt : public live2d::LDObject 
	{
		// �t�����h
		friend class MotionQueueManager ;
		friend class AMotion ;
		friend class Live2DMotion ;
		friend class Live2DMotionBin ;
	public:
		MotionQueueEnt();
		virtual ~MotionQueueEnt(void);

	public:
		//  �t�F�[�h�A�E�g�J�n
		void startFadeout(long long fadeOutMsec)  ;

		//  �I������
		bool isFinished() { return finished ; }

		//  �J�n���Ԏ擾
		long long getStartTimeMSec(){ return startTimeMSec ; }

		//  �t�F�[�h�C���J�n���Ԏ擾
		long long getFadeInStartTimeMSec(){ return fadeInStartTimeMSec ; }

		//  �t�F�[�h�C���I�����Ԏ擾
		long long getEndTimeMSec(){ return endTimeMSec ; }

		//  �J�n���Ԑݒ�
		void setStartTimeMSec(long long t){ this->startTimeMSec = t ; }

		//  �t�F�[�h�C���J�n���Ԑݒ�
		void setFadeInStartTimeMSec(long long t){ this->fadeInStartTimeMSec = t ; }

		//  �t�F�[�h�C���I�����Ԑݒ�
		void setEndTimeMSec(long long t){ this->endTimeMSec = t ; }

		//  �I������ݒ�
		void setFinished( bool f ){ this->finished = f ; }

		//  ���[�V�������L�����Ԃ�
		bool isAvailable(){ return available ; }

		//  �L������ݒ�
		void setAvailable( bool v ){ this->available = v ; }

		//---- state for callback(Unity etc) ----
		//  ��Ԑݒ�
		void setState( long long time , float weight ){ this->state_time = time ; this->state_weight = weight ; }

		//  ���ԏ�Ԏ擾
		long long getState_time(){ return this->state_time ; }

		//  "�d��"�̏�Ԏ擾
		float getState_weight(){ return this->state_weight ; }
		
	private:
		// �p�u���b�N�Ȑݒ�l
		bool			autoDelete;					//  �����폜
		AMotion* 		motion;						//  ���[�V����

		bool 			available;					//  �L�����t���O
		bool 			finished ;					//  �I���t���O
		bool 			started ;					//  �J�n�t���O�i0.9.00�ȍ~�j
		long long 		startTimeMSec ;				//  �J�n����
		long long 		fadeInStartTimeMSec ;		//  �t�F�[�h�C���J�n�����i���[�v�̎��͏���̂݁j

		// �I���\�莞��
		// �@-1�̏ꍇ�͏I���\�莞���Ȃ��i���������A�܂��́@���[�v���Ӗ�����j
		// 	��loop�̏ꍇ�� endTimeMSec = ( startTimeMSec + 1�񕪂̎��� )
		// 	�ʃ��[�V�����̊����� MotionQueueManager���� startFadeout()���Ă΂��
		//		newEndTimeMSec = (��������+fadeOutMSec) ;
		//	if( endTimeMSec<0 /*���[�v��*/ || newEndTimeMSec < endTimeMSec ){
		//			endTimeMSec = newEndTimeMSec 
		// �I���\�莞�����߂����ꍇ�̓��[�V�������j�������
		long long endTimeMSec ;

		// ---- �C���X�^���X�̎��ʔԍ�
		static int static_motionQueueEntNo ;// �C���X�^���X���ƂɈ�ӂ̒l�������ʔԍ��̎��̒l
		int motionQueueEntNo ;// �C���X�^���X���ƂɈ�ӂ̒l�������ʔԍ��inew���ɐݒ�j(0.8.09�ǉ��j

	private:
		//---- state for callback(Unity etc) ----
		long long   	state_time ;				//  �����̏��
		float	   	state_weight ;				//  �d�݂̏��
	};

}
//--------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_MOTION_QUEUE_ENT_H__