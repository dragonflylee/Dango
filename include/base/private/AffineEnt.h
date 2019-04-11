/*
 * AffineEnt.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef _AFFINE_ENT_H
#define _AFFINE_ENT_H


#ifndef __SKIP_DOC__


#include "../../io/ISerializableV2.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{ 
	class AffineEnt : public ISerializableV2 
	{
	
	public :
		AffineEnt() ;
		virtual ~AffineEnt() ;
	
	public:
		void init( AffineEnt &ent ) ;
		virtual void readV2(BReader & br , MemoryParam* memParam ) ;
	
	#ifdef L2D_SAMPLE
		void DUMP() ;
	#endif 
		
	public:
		float originX;			// ���_X
		float originY;			// ���_Y
		float scaleX;			// �g�嗦X
		float scaleY;			// �g�嗦Y�B�g�嗦��X�����g�p���Ȃ�
		float rotateDeg;		// �x�B�͈͂Ȃ��ŁA����]��\���ł���B	

		bool reflectX ;		//SDK2.0
		bool reflectY ;		//SDK2.0
	};
} 
//------------------------- LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif // _AFFINE_ENT_H