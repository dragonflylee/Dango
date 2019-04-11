/*
 *  IContextData.h
 *  
 *  Immutable��DrawData�ABaseData�̕`��p�̃f�[�^��ێ�����B
 *  
 *  Created by joe on 11/04/29.
 *  Copyright 2011 Live2D Inc. All rights reserved.
 */

#ifndef __LIVE2D_I_DRAW_DATA_H__
#define __LIVE2D_I_DRAW_DATA_H__


#ifndef __SKIP_DOC__

#include "../memory/LDObject.h"

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	class IDrawData;
	class ClipContext;
	
	class IDrawContext : public live2d::LDObject 
	{
	public:
		IDrawContext(IDrawData* src) ;
		virtual ~IDrawContext(){};
	
		void* getSrcPtr(){ return this->srcPtr ; } 
	
		//  �p�[�c�ԍ��Z�b�g
		void setPartsIndex( int p ){ this->partsIndex = p ; }
	
		//  �p�[�c�ԍ��擾
		int getPartsIndex() { return this->partsIndex ; }
	
		//  ���p�ł��邩�擾�@TODO �X�V
		bool isAvailable(){ return available && ! paramOutside ; }
	
	public:
		int 			interpolatedDrawOrder ;		// �`�揇�̕�Ԍ���
		float 			interpolatedOpacity ;		// �s�����x�̕�Ԍ���
		
		bool			paramOutside ;				// ��Ԍ��ʂƂ��āA�p�����[�^���͈͊O�̂Ƃ��� true 
		
		float			partsOpacity ;				// �p�[�c�̕s�����x
		bool			available ;					// TODO �X�V
	
		float			baseOpacity;				// SDK2.0 ��������f�t�H�[�}�̗ݐϓ����x

		ClipContext		*clipBufPre_clipContext;	// SDK2.1 �}�X�N�̐ݒ�

	private:
		IDrawData* 		srcPtr ;					
		int 			partsIndex ;				
	};
}
//------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_I_DRAW_DATA_H__
