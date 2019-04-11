/**
 *  LDRectF.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_LDRECTF_H__
#define __LIVE2D_LDRECTF_H__


#ifndef __SKIP_DOC__


#include "../Live2D.h"
#include "../memory/LDObject.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{
	
	class LDRectF   : public live2d::LDObject 
	{
	
	public:
		// Constructor
		LDRectF();
		LDRectF(l2d_pointf x , l2d_pointf y , l2d_pointf w , l2d_pointf h );
	
		// Destructor
		virtual ~LDRectF();

		void setRect(LDRectF *r);
	
	public:
		//  ����X�擾
		l2d_pointf getCenterX(){ return x + 0.5f*width ; } 
	
		//  ����Y�擾
		l2d_pointf getCenterY(){ return y + 0.5f*height; }
	
		//  �E�[�擾
		l2d_pointf getRight(){ return x + width ; }
	
		//  ���[�擾
		l2d_pointf geBottom(){ return y + height; }

		//  ���������ɏc�����g�k
		void expand(float w, float h);
		
	public:
		l2d_pointf x;				// ���[X
		l2d_pointf y;				// ��[Y
		l2d_pointf width;			// ��
		l2d_pointf height;			// ����
	};
}
//------------------------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_LDRECTF_H__