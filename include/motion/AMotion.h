/**
 *  AMotion.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_A_MOTION_H__
#define __LIVE2D_A_MOTION_H__


#include "../memory/LDObject.h"

#include "../ALive2DModel.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{

	class MotionQueueEnt ;

	
	class AMotion : public live2d::LDObject 
	{
	public:
		AMotion();
		virtual ~AMotion() ;

	public:
		//  �p�����[�^�X�V
		void updateParam( live2d::ALive2DModel * model , MotionQueueEnt *motionQueueEnt ) ;

	public:

		
		void setFadeIn( int fadeInMsec ){ this->fadeInMsec = fadeInMsec ; }

		
		void setFadeOut( int fadeOutMsec){ this->fadeOutMsec = fadeOutMsec ; }
		
		
		int getFadeOut(){ return this->fadeOutMsec ; }

		
		int getFadeIn(){ return this->fadeInMsec ; }

		
		void setWeight( float weight){ this->weight = weight ; }
		
		
		float getWeight(){ return this->weight ; }

		/*****************************************************************
		 * ���[�V�����̒�����Ԃ��i���[�v�̂Ƃ��� -1�j 
		 * ���[�v�ł͂Ȃ��ꍇ�́A�I�[�o�[���C�h����
		 * 
		 * ���̒l�̎��͎擾����鎞�ԂŏI������
		 * -1�̂Ƃ��͊O�������~���߂���������I���Ȃ������ƂȂ�
		******************************************************************/
		virtual int getDurationMSec(){ return -1 ; }
		
		/*****************************************************************
		 * ���[�v�P�񕪂̒�����Ԃ� (0.9.00b8)
		 * 
		 * ��Ƀ��[�V�����̏��������ŗ��p����
		 * 
		 * �T�u�N���XLive2DMotion�̏ꍇ�́Amtn�t�@�C���Œ�`������A�̃��[�V�����̒�����Ԃ�
		 * ���[�v���Ȃ��ꍇ�� getDurationMSec()�Ɠ����l��Ԃ�
		 * ���[�v��񕪂̒�������`�ł��Ȃ��ꍇ�i�v���O�����I�ɓ���������T�u�N���X�Ȃǁj�̏ꍇ��-1��Ԃ�
		******************************************************************/
		virtual int getLoopDurationMSec(){ return -1 ; }


		/*****************************************************************
		 * ���[�V�����Đ��̊J�n������ݒ肷��
		 * �r������Đ�����ꍇ�Ɏg�p����
		 *
		 * version 0.9.00��蓋��
		******************************************************************/
		void setOffsetMSec( int offsetMsec ){ this->offsetMsec = offsetMsec ; }

		
		void reinit(){}

	protected:
		//  �p�����[�^�A�b�v�f�[�g
		virtual void updateParamExe( live2d::ALive2DModel * model , long long timeMSec , float weight , MotionQueueEnt *motionQueueEnt ) = 0 ;

		//  �C�[�W���O���擾
		float getEasing( float time , float totalTime , float accelerateTime ) ;

	protected:
		int 	fadeInMsec ;		// �t�F�[�h�C������ꍇ�B���ꂪ�����ƁA�ˑR�������ς�����肷��
		int 	fadeOutMsec ;		// �t�F�[�h�A�E�g����
		float 	weight ;			// Model�ɐݒ肳��Ă���l�Ɗ|�����킹��x�����B�P�Ȃ�㏑������

		int 	offsetMsec ;		// �Đ��̊J�n����

	};

}
//--------- LIVE2D NAMESPACE ------------


#endif		// __LIVE2D_A_MOTION_H__