/**
 *  LDRect.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_LD_RECT_H__
#define __LIVE2D_LD_RECT_H__


#ifndef __SKIP_DOC__

#include "../memory/LDObject.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d
{ 
	
	
	
	class LDRect  : public live2d::LDObject 
	{
	public:
		// Constructor
		LDRect();
		LDRect(int x , int y , int w , int h );
	
		// Destructor
		virtual ~LDRect();
		
	public:
		//  ����X���W�擾
		int getCenterX(){ return x + width/2 ; } 
	
		//  ����Y���W�擾
		int getCenterY(){ return y + height/2 ; }
	
		//  �E�[�擾
		int getRight(){ return x + width ; }
	
		//  ��ԉ��擾
		int geBottom(){ return y + height ; }
		
	public:
		int x;			// ���[�ƂȂ�X���W
		int y;			// ��[�ƂȂ�Y���W
		int width;		// ��`�̕�
		int height;		// ��`�̍���
		
	};

}
//------------------------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_LD_RECT_H__