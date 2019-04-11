/*
 *  IContextData.h
 *  
 *  Immutable��DrawData�ABaseData�̕`��p�̃f�[�^��ێ�����B
 *  
 *  Created by Live2D on 11/04/29.
 *  Copyright 2011 Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __IBASE_CONTEXT__
#define __IBASE_CONTEXT__


#ifndef __SKIP_DOC__

#include "../memory/LDObject.h"


//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	class IBaseData ;
		
	class IBaseContext : public live2d::LDObject 
	{
	public:
		IBaseContext(IBaseData* src) ;
		virtual ~IBaseContext(){}

		void* getSrcPtr(){ return this->srcPtr ; } 
		
		void setPartsIndex( int p ){ this->partsIndex = p ; }
		int getPartsIndex() { return this->partsIndex ; }

		bool isOutsideParam() {	return this->outsideParam;	}
		void setOutsideParam( bool outsideParam ) {	this->outsideParam = outsideParam ;	}

		bool isAvailable()
		{
			return available && ! outsideParam ;
		}

		void setAvailable(bool available)
		{
			this->available = available;
		}

		/*
		 * ��ʂ��玩�g�܂ł̔{���̑����Ԃ��B
		 * BDAffine�̏ꍇ�́A���g�̃X�P�[���Ɛe�܂ł̃X�P�[���̐ςɂȂ�
		 */
		float getTotalScale()
		{
			return totalScale ;
		}
	
		void setTotalScale_notForClient(float totalScale)
		{
			this->totalScale = totalScale ;
		}

		float getInterpolatedOpacity(){	return interpolatedOpacity ;}

		void setInterpolatedOpacity(float interpolatedOpacity){this->interpolatedOpacity = interpolatedOpacity ;}

		float getTotalOpacity(){return totalOpacity ;}
	
		void setTotalOpacity(float totalOpacity){this->totalOpacity = totalOpacity ;}


	private:
		IBaseData* srcPtr ;
		int partsIndex ;
		
		bool outsideParam ;
		bool available ;

		
	protected:
		// ��ʂ��玩�g�܂ł̔{���̑���
		// BDAffine�� scaleX,Y��XY�̈Ӗ���K�؂Ɏg���Ȃ��̂� scaleX �̂ݗp����B
		// scaleX�Ɛe��totalScale���|�������̂����g��totalScale�ƂȂ�
		// BDBoxGrid�̂悤�ɑS�ē_�ŕϊ����`����ꍇ�́A�{���͈Ӗ��������Ȃ��B
		// BDAffine�e�A BDAffine�q�̏ꍇ�ɔ{���𔽉f���邽�߂ɗp����
		float		totalScale;

		// �f�t�H�[�}�̓����x(SDK 2.0�ȍ~)
		float		interpolatedOpacity;		// �s�����x�̕�Ԍ���
		float		totalOpacity;				// ��ʂ��玩�g�܂ł̕s�����x�̗ݐρi���g���܂ށj

	};
}
//------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif // __IBASE_CONTEXT__