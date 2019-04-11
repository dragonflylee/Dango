/**
 *  EyeBlinkMotion.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_EYE_BLINK_MOTION_H__
#define __LIVE2D_EYE_BLINK_MOTION_H__


#include "../memory/LDObject.h"

#include "../type/LDVector.h"

#include "../ALive2DModel.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{

	
	class EyeBlinkMotion : public live2d::LDObject 
	{

	public:
		// ��̏�Ԓ萔
		enum EYE_STATE{
			STATE_FIRST = 0 , 
			STATE_INTERVAL ,
			STATE_CLOSING ,// ���Ă����r��
			STATE_CLOSED , // ���Ă�����
			STATE_OPENING ,// �J���Ă����r��
		};

	public:
		// Constructor
		EyeBlinkMotion();

		// Destructor
		virtual ~EyeBlinkMotion();

	public:
		//  ����̂܂΂������[�V�����̎�����ݒ�
		long long calcNextBlink() ;

		//  �C���^�[�o�����Ԃ̐ݒ�
		void setInterval( int blinkIntervalMsec) ;

		//  �܂΂������[�V�����̐ݒ�
		void setEyeMotion( int closingMotionMsec , int closedMotionMsec , int openingMotionMsec ) ;

		//  �w�肵�����f���̃p�����[�^�ݒ�
		void setParam( live2d::ALive2DModel *model ) ;

	private:
		long long 			nextBlinkTime ;				// �����p�`���鎞���imsec�j
		
		int 				eyeState ;					// ���݂̏��
		long long 			stateStartTime ;			// ���݂�state���J�n��������

		bool 				closeIfZero;				// ID�Ŏw�肳�ꂽ��̃p�����[�^���A0�̂Ƃ��ɕ���Ȃ� true �A1�̎��ɕ���Ȃ� false
		
		live2d::LDString 	eyeID_L ;					// ���ڂ�ID
		live2d::LDString 	eyeID_R ;					// �E�ڂ�ID

		int					blinkIntervalMsec ;			// 
		int 				closingMotionMsec ;			// �Ⴊ����܂ł̎���
		int 				closedMotionMsec  ;			// �����܂܂ł��鎞��
		int 				openingMotionMsec ;			// �Ⴊ�J���܂ł̎���

	};

}
//--------- LIVE2D NAMESPACE ------------


#endif		// __LIVE2D_EYE_BLINK_MOTION_H__