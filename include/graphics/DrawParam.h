/**
 * DrawParam.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_DRAWPARAM_H__
#define __LIVE2D_DRAWPARAM_H__


#include "../Live2D.h"
#include "../memory/LDObject.h"
#include "../type/LDVector.h"

#include "../id/DrawDataID.h"

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{
	class ClipContext;

	class TextureInfo :public LDObject{
	public:
		TextureInfo();
		float a;
		float r;
		float g;
		float b;

		float scale;
		float interpolate;

		int blendMode;
	};

	
	class DrawParam  : public live2d::LDObject 
	{
	public:
		static const int DEFAULT_FIXED_TEXTURE_COUNT = 32 ;

		static const int CLIPPING_PROCESS_NONE = 0;
		static const int CLIPPING_PROCESS_OVERWRITE_ALPHA = 1;	// �}�X�N�p�O�����B�A���t�@�̂ݏ㏑��
		static const int CLIPPING_PROCESS_MULTIPLY_ALPHA = 2;	// �}�X�N�p�O�����B�A���t�@�̂ݏ�Z
		static const int CLIPPING_PROCESS_DRAW = 3;				// �}�X�N�p�BDest�̃A���t�@��Src�̃A���t�@�ł��邩�̂悤�ɗ��p
		static const int CLIPPING_PROCESS_CLEAR_ALPHA = 4;		// �}�X�N���|����O�ɃA���t�@���N���A
	
	public:
		DrawParam();
		virtual ~DrawParam();
	
	public:
		//  �`��̃Z�b�g�A�b�v�B�K�v�ɉ����ăI�[�o�[���C�h����B
		virtual void setupDraw(){} ;
		// �`�抮����̏����B�K�v�ɉ����ăI�[�o�[���C�h����B
		virtual void cleanupDraw(){};
	
	
		
		virtual void drawTexture( int textureNo , int indexCount , int vertexCount , l2d_index * indexArray
				 , l2d_pointf * vertexArray , l2d_uvmapf * uvArray , float opacity, int colorCompositionType ) = 0 ;
	
	
		// �V�������p�ł���Model�̃e�N�X�`���ԍ����m��
		virtual int generateModelTextureNo() ;
		
		// Model�̃e�N�X�`���ԍ��𐶐�
		virtual void releaseModelTextureNo(int no) ;
	
		// ���f���`��̍ہA�S�̂Ɋ|�����킳���F�ݒ�B�e�F0..1 (1���W���̏�ԁj
		virtual void setBaseColor( float alpha , float red , float green , float blue ) ;
	
		// �J�����O�i�Жʕ`��j���s���ꍇ��true���Z�b�g����B
		void setCulling( bool culling ){ this->culling = culling ; }
	
		//  �z��͕��������̂Ō��̔z��͊O�Ŕj�����ėǂ�
		void setMatrix( float* _matrix4x4 )
		{
			for( int i = 0 ; i < 16 ; i++ )
			{
				matrix4x4[i] = _matrix4x4[i] ;
			}
		}
		float* getMatrix(){ return matrix4x4; }

		void setPremultipliedAlpha(bool enable);
		bool isPremultipliedAlpha();

		void setAnisotropy(int n);
		int getAnisotropy();

		void setTextureColor(int textureNo, float r, float g, float b, float a=1);
		void setTextureScale(int textureNo, float scale);
		void setTextureInterpolate(int textureNo, float interpolate);
		void setTextureBlendMode(int textureNo, int mode);

		bool enabledTextureInfo(int textureNo);

		void setpCurrentDrawDataID(DrawDataID* pDrawDataID);
		DrawDataID* getpCurrentDrawDataID();

		int getClippingProcess(){return clippingProcess;}
		void setClippingProcess(int v){ clippingProcess = v; }

		void setClipBufPre_clipContextForMask(ClipContext *clip);
		void setClipBufPre_clipContextForDraw(ClipContext *clip);

	protected:
		int 	fixedTexureCount ;
		
		float 	baseAlpha ;
		float 	baseRed ;
		float 	baseGreen ;
		float 	baseBlue ;
	
		bool	culling	;
		float 	matrix4x4[16] ;
		int		clippingProcess;
		
		bool 	premultipliedAlpha;
		int 	anisotropy;


		LDVector<TextureInfo*> 		texturesInfo;

		DrawDataID*	 pCurrentDrawDataID;//D3D�p�Ɍ��݂̕`��I�u�W�F�N�g�̃C���f�b�N�X��n���K�v������̂ŁA�ǉ��B���v���b�g�t�H�[���ł͎g�p�����B

		virtual bool enableDrawTexture(int textureNo, int vertexCount, l2d_index * indexArray,
			l2d_pointf * vertexArray, l2d_uvmapf * uvArray, float opacity, int colorCompositionType);



		ClipContext *clipBufPre_clipContextMask;// �N���b�vMask�p�ɕ`���Ƃ��ɐݒ肳���
		ClipContext *clipBufPre_clipContextDraw;// �N���b�vDraw�p�ɕ`���Ƃ��ɐݒ肳���

	private:
		//Prevention of copy Constructor
		DrawParam( const DrawParam & ) ;			
		DrawParam& operator=( const DrawParam & ) ;
	};
}
//------------ LIVE2D NAMESPACE ------------

#endif		// __LIVE2D_DRAWPARAM_H__
