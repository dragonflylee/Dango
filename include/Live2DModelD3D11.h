/*
 *  Live2DModelD3D11.h
 * 
 *  �O������Q�Ƃ���郂�f���f�[�^
 * 
 *  �����̍\���� ModelImpl
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */


#ifndef __LIVE2D_L2DMODEL_D3D11_H__
#define __LIVE2D_L2DMODEL_D3D11_H__


#include "Live2D.h"						// �v���b�g�t�H�[���ʐݒ�̑O�ŕK���Ăяo������
#ifdef L2D_TARGET_D3D11
#include "ALive2DModel.h"
#include "model/ModelImpl.h"
#include "graphics/DrawParam_D3D11.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d 
{
	class ModelContext;
	
	class Live2DModelD3D11 : public live2d::ALive2DModel
	{
	public:
		
		static void setGraphicsContext( ID3D11Device* device,ID3D11DeviceContext* context );

		// �f�o�C�X���X�g�������ɋ��ʂ��ČĂяo��
		static void deviceLostCommon();

	public:
		// Constructor
		Live2DModelD3D11( void );

		// Destructor
		virtual ~Live2DModelD3D11( void );

	public:
		// �e�N�X�`���Z�b�g
		void setTexture( int textureNo, ID3D11ShaderResourceView* texture );

		// �o�^���ꂽ�e�N�X�`����j������
		// �e�N�X�`�����O���ŋ��L���Ă���ꍇ�����邽��
		// �o�^���ꂽ�e�N�X�`���͎����I�ɂ͔j������Ȃ��B
		// �j������ɂ�Destructor�̑O�ɖ����I�ɌĂԕK�v������B
		void deleteTextures();

		// ���f�����[�h
		static Live2DModelD3D11* loadModel( const l2d_string& filepath );

		// ���f�����[�h
		static Live2DModelD3D11* loadModel( const void* buf, int bufSize );

		// Direct3D�̃f�o�C�X���X�g���̏���
		void deviceLostD3D();

		void setMatrix( float*  matrix ) ;

		// �`��
		virtual void draw();

		// �V�������p�ł���Model�̃e�N�X�`���ԍ����m��(Avatar�p)
		virtual int generateModelTextureNo();
	
		// Model�̃e�N�X�`���ԍ��𐶐�(Avatar�p)
		virtual void releaseModelTextureNo( int no );

		// �`��p�p�����[�^���擾
		virtual live2d::DrawParam* getDrawParam(){ return drawParamD3D; }

		void setTextureColor(int textureNo,float r,float g,float b);
		void setTextureBlendMode(int textureNo,int mode);
		void setTextureInterpolate(int textureNo, float interpolate);

		
		int getErrorD3D_tmp(){return drawParamD3D->getErrorD3D_tmp();}

	private:
		// Prevention of copy Constructor����
		Live2DModelD3D11( const Live2DModelD3D11& );
		Live2DModelD3D11& operator=( const Live2DModelD3D11& );

	private:
		DrawParam_D3D11* drawParamD3D;		// Direct3D�̕`��p�����[�^�i�[�p
	};
}
#endif		//L2D_TARGET_D3D11

#endif		//__LIVE2D_L2DMODEL_D3D11_H__