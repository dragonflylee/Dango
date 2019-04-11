/**
 * DrawParam_D3D11.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_DRAWPARAM_D3D11_H__
#define __LIVE2D_DRAWPARAM_D3D11_H__


#include "../Live2D.h"
#ifdef L2D_TARGET_D3D11
#include <d3d11.h>
#include "DrawParam.h"

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	
	class DrawParam_D3D11 : public DrawParam 
	{
	public:
		//--- error / Direct3D is 6000.. ---
		static const int ERROR_D3D_CREATE_VERTEX_BUFFER	= 6001;	//
		static const int ERROR_D3D_LOCK_VERTEX_BUFFER	= 6002;	//
		static const int ERROR_D3D_CREATE_INDEX_BUFFER	= 6003;	//
		static const int ERROR_D3D_LOCK_INDEX_BUFFER	= 6004;	//
		static const int ERROR_D3D_DEVICE_NOT_SET		= 6005;	//


		
		static void setGraphicsContext( ID3D11Device* device,ID3D11DeviceContext* context ){ s_pd3dDevice = device; s_pd3DContext = context; }

		// �f�o�C�X���X�g�������ɋ��ʂ��ČĂяo��
		static void deviceLostCommon();

		// ���L���\�[�X�̔j��
		static void dispose();

		static void updateGpuBuffer(ID3D11Buffer* dest, void* src, int size);

	public:
		// Constructor
		DrawParam_D3D11();

		// Destructor
		virtual ~DrawParam_D3D11();

	public:
		//  �e�N�X�`���폜(0.9.00b5���)
		virtual void deleteTextures();

		//  �`��Z�b�g�A�b�v(0.9.00b5���)
		virtual void setupDraw();

		// �`�抮����̏���
		virtual void cleanupDraw();

		//  �e�N�X�`���`��
		virtual void drawTexture( int textureNo, int indexCount, int vertexCount, l2d_index* indexArray,
			l2d_pointf* vertexArray, l2d_uvmapf* uvArray, float opacity, int colorCompositionType);

		//  �e�N�X�`���`��
		virtual void drawTexture(int textureNo, int indexCount, int vertexCount, l2d_index* indexArray,
			l2d_pointf* vertexArray, l2d_uvmapf* uvArray, float opacity, 
			ID3D11Buffer* pUvBuf,
			ID3D11Buffer* pIndexBuf,
			int colorCompositionType);

		//  �e�N�X�`���Z�b�g
		void setTexture( int modelTextureNo , ID3D11ShaderResourceView* texture );

		//  �V�������p�ł���Model�̃e�N�X�`���ԍ����m��(Avatar�p�j
		virtual int generateModelTextureNo();

		//  Model�̃e�N�X�`���ԍ��𐶐�(Avatar�p�j
		virtual void releaseModelTextureNo( int no );

		//  DirectX�f�o�C�X���X�g���ɌĂяo��
		void deviceLost();

		bool growVertexBuffer(int needBufSize);

		ID3D11Buffer* createUvBuffer(int needBufSize);

		ID3D11Buffer* createIndexBuffer(int needBufSize);

		
		int getErrorD3D_tmp()
		{
			int ret = error;
			error = 0;
			return ret;
		}


		
		void setErrorD3D_tmp( int error )
		{
			this->error = error ;
		}
		
	private:
		// �V�F�[�_�̃��[�h
		static void initShader();

		static void createVertexShader(ID3D11VertexShader** outShader, const void* data, int size);
		static void createInputLayout(ID3D11InputLayout** outLayout, const void* data, int size, 
			D3D11_INPUT_ELEMENT_DESC* inputLayout, int inputLayoutSize);
		static void createPixelShader(ID3D11PixelShader** outShader, const void* data, int size);

		static void createMaskTexture(int width, int height);
		static void createBlendState();
		static void createRasterizerState();
		static void createConstantBuffer();

	private:
		// Prevention of copy Constructor
		DrawParam_D3D11( const DrawParam_D3D11 & );
		DrawParam_D3D11& operator=( const DrawParam_D3D11 & );

		void releaseResource();

	private:
		enum PsShaderMode
		{
			PS_SHADER_MODE_MULT,
			PS_SHADER_MODE_MULT_PA,
			PS_SHADER_MODE_MULT_MASK,
			PS_SHADER_MODE_MULT_PA_MASK,

			PS_SHADER_MODE_ADD,
			PS_SHADER_MODE_ADD_PA,
			PS_SHADER_MODE_ADD_MASK,
			PS_SHADER_MODE_ADD_PA_MASK,

			PS_SHADER_MODE_INTERPOLATE,
			PS_SHADER_MODE_INTERPOLATE_PA,
			PS_SHADER_MODE_INTERPOLATE_MASK,
			PS_SHADER_MODE_INTERPOLATE_PA_MASK,

			PS_SHADER_MODE_LAST_ITEM  // �Ō��\���ԕ�
		};

		static ID3D11Device* s_pd3dDevice;
		static ID3D11DeviceContext* s_pd3DContext;
		static ID3D11VertexShader* s_vertexShader;
		static ID3D11PixelShader* s_pixelShaders[PS_SHADER_MODE_LAST_ITEM];
		static ID3D11InputLayout* s_inputLayout;
		static ID3D11RasterizerState* s_cullingOn;
		static ID3D11RasterizerState* s_cullingOff;
		static ID3D11RenderTargetView* s_mainRTV;

		static ID3D11BlendState* s_blendStateNormal;
		static ID3D11BlendState* s_blendStateScreen;
		static ID3D11BlendState* s_blendStateMultiply;

		static ID3D11Buffer* _vs1stBufferGpu; // �o�[�e�b�N�X�V�F�[�_�p�̃R���X�^���g�o�b�t�@(��p�x��������)
		static ID3D11Buffer* _ps1stBufferGpu; // �s�N�Z���V�F�[�_�p�̃R���X�^���g�o�b�t�@(��p�x��������)
		static ID3D11Buffer* _ps2ndBufferGpu; // �s�N�Z���V�F�[�_�p�̃R���X�^���g�o�b�t�@(���p�x��������)

		static ID3D11Texture2D* s_maskTexture;
		static ID3D11RenderTargetView* s_maskRTV;
		static ID3D11ShaderResourceView* s_maskSRV;
		static int s_maskTextureWidth;
		static int s_maskTextureHeight;

		LDVector<ID3D11ShaderResourceView*>	_textures;	// �e�N�X�`���f�[�^

		ID3D11Buffer* _vertexBuffer;	// ���_�̈ʒu���o�b�t�@
		int	_vertexBufSize;				// ���݂�Buffer�̃T�C�Y
		ID3D11SamplerState* _sampleLinear;	// �e�N�X�`���[�̃T���v���[

		int _prevAnisotropy;
		int error; // �G���[�p
	};
} 
//------------ LIVE2D NAMESPACE ------------
#endif// L2D_TARGET_D3D11

#endif		// __LIVE2D_DRAWPARAM_D3D11_H__