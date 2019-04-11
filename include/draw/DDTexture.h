/**
 *  DDTexture.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_DDTEXTURE_H__
#define __LIVE2D_DDTEXTURE_H__


#ifndef __SKIP_DOC__

#include "../Live2D.h"
#include "IDrawData.h"
#include "../param/PivotManager.h"
#include "IDrawContext.h"

#ifdef L2D_TARGET_D3D
# include <d3dx9.h>
# include "../graphics/DrawParam_D3D.h"
#endif

#ifdef L2D_TARGET_D3D11
#include <d3d11.h>
# include "../graphics/DrawParam_D3D11.h"
#endif

#ifdef L2D_TARGET_PS4
#include <gnmx.h>
#endif

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 
	class DDTextureContext ;
	
	
	
	class DDTexture : public IDrawData
	{
	public:
		static const int OPTION_FLAG_BARCODE_KANOJO_COLOR_CONVERT = 0x1 ;// �o�[�R�[�h�J�m�W���ŐF�ς��s���v�f�̏ꍇ 
		
		static const int MASK_COLOR_COMPOSITION = 0x1E ;//011110 
	
		static const int COLOR_COMPOSITION_NORMAL  = 0 ;// �ʏ�
		static const int COLOR_COMPOSITION_SCREEN  = 1 ;// �X�N���[��
		static const int COLOR_COMPOSITION_MULTIPLY= 2 ;// ��Z
	
		static int INSTANCE_COUNT ;
	
	public:
		DDTexture() ;
		virtual ~DDTexture(void) ;
		
	public:
		virtual void readV2(BReader & br , MemoryParam* memParam ) ;

		void initDirect(MemoryParam* memParam) ;
	
		//  �w�肵���e�N�X�`���ԍ��ɃZ�b�g
		void setTextureNo(int no){ this->textureNo = no ; }
	
		//  �e�N�X�`���ԍ��擾
		int getTextureNo(){ return this->textureNo ; }
		
		//  uv�z�� [ numPts*2 ]
		l2d_uvmapf * getUvMap(){ return this->uvmap ; }
		
		//  �_����Ԃ�
		int getNumPoints(){ return this->numPts ; }
	
		//  �|���S������Ԃ�
		int getNumPolygons(){ return this->numPolygons ; }
		
		virtual IDrawContext* init(ModelContext &mdc) ;
		
		virtual void setupInterpolate(ModelContext &mdc , IDrawContext* cdata ) ;
		
		virtual void setupTransform(ModelContext &mdc , IDrawContext* cdata ) ;
	
		virtual void preDraw(DrawParam& dp, ModelContext& mdc, IDrawContext* cdata) ;

		virtual void draw( DrawParam & dp , ModelContext &mdc , IDrawContext* cdata ) ;
	
	#ifdef L2D_TARGET_D3D
	
		void setupBufD3D( DrawParam_D3D& dpD3D , ModelContext &mdc , DDTextureContext* cdata ) ;
	
		virtual void deviceLost( IDrawContext* drawContext ) ;
	#endif
	#ifdef L2D_TARGET_D3D11

		void setupBufD3D(DrawParam_D3D11& dpD3D, ModelContext &mdc, DDTextureContext* cdata);

		virtual void deviceLost(IDrawContext* drawContext);
	#endif

		//  �e�N�X�`���^�C�v�擾
		virtual int getType(){ return TYPE_DD_TEXTURE ; }// 
	
		int getOptionFlag(){ return this->optionFlag ; }
	
		virtual void setZ_TestImpl( ModelContext &mdc , IDrawContext* _cdata , float z ) ;
	
		l2d_index*  getIndexArray(int* polygonCount);

	#if AVATAR_OPTION_A
		//  �F�擾	
		int getOption_KanojoColor(){ return this->optionKanojoColor ; }
	
	#endif
	
	#ifdef L2D_DEBUG
		virtual void dump() ;
	#endif
	
	private:
		// Prevention of copy Constructor
		DDTexture( const DDTexture & ) ;				
		DDTexture& operator=( const DDTexture & ) ;		
	
	private:
		int						textureNo ;				// �g�p����e�N�X�`���ԍ��i���f���̊Ǘ�����e�N�X�`���ԍ��j
		int						numPts ;				// ���_�̐��iPSP��sceGumDrawArray �ł� 65535�܂łɂȂ�j
		int						numPolygons ;			// �|���S���̐�
		int						optionFlag ;			// �I�v�V�����ݒ�
		
		// --- �K�v�ɉ����Đݒ肳���I�v�V�����l
	#if AVATAR_OPTION_A
		int						optionKanojoColor ;		// Avatar�̃I�v�V�����ioptionFlag & DDMorph.OPTION_FLAG_BARCODE_KANOJO_COLOR_CONVERT�������Ă���ꍇ�j
	#endif
		
		l2d_index *				indexArray ;			// ���_�C���f�b�N�X�z�� [numPolygons*3]
	
		//  points ( Pivot�z�� )�̃��[�� ( ��  xnum=3 , ynum=4 , znum=2 �̃p�����[�^�̏ꍇ )
		//  [x0,y0,z0],[x1,y0,z0],[x2,y0,z0]  , [x0,y1,z0],[x1,y1,z0],[x2,y1,z0].. �ƂȂ�
		
		LDVector<l2d_paramf*>*	pivotPoints ;			// ��ԗp��pivot��S�Ď����_�z�� [numPivots][numPts*2]
		l2d_uvmapf *			uvmap ;					// uv�z�� [ numPts*2 ]
		int						colorCompositionType ;	// �F�����̃^�C�v
		bool					culling ;				// 1.0beta3
	};
	
	
	class DDTextureContext : public IDrawContext 
	{
	public:
		DDTextureContext(IDrawData* src) ;
		virtual ~DDTextureContext() ;
		
	public:		
		// �ϊ��ς݂̓_���Ԃ� 
		// pointCount �͓_�̐����Ԃ�i�_�ix,y�j��pointCount���Ԃ̂ŁApointCount*2�̔z��ƂȂ�j
		l2d_pointf* getTransformedPoints( int* pointCount ) ;
	
	public:		
		int 				tmpBaseDataIndex ;		// IBaseData�̃C���f�b�N�X�B�s�ςȂ̂ň�x�擾����΂悢
		l2d_pointf * 		interpolatedPoints ;	// ��Ԃ��ꂽ�_�� 	PSP�̏ꍇ�́A[numPts*5]�@/ OpenGL�̏ꍇ�� [numPts*2]( texture x,y , ��ԁE�ό`���ꂽ��̓_ x,y , z=0 ) 
		l2d_pointf * 		transformedPoints ;		// �ό`���ꂽ�_��
		l2d_pointf * 		drawPoints ;			// �`��p�̓_��i�_�u���o�b�t�@�p�̏ꍇ�̓X���b�v���Ďg����j0.9.00b14
		
		unsigned char 		not_updated_count ;		// �X�V����Ȃ������񐔂��J�E���g����\n�P��ڂƂQ��ڂňӖ������B\ndouble buffer�̂Ƃ����X�V����Ȃ���΂��̂܂ܑO�̒l���g����
		
	#ifdef L2D_TARGET_D3D
		LPDIRECT3DVERTEXBUFFER9 		pUvBuf ; // Uv�z��
		LPDIRECT3DINDEXBUFFER9 			pIndexBuf ; // �C���f�b�N�X�z��
	#endif

	#ifdef L2D_TARGET_D3D11
		ID3D11Buffer* pUvBuf;		// �e�N�X�`��uv�o�b�t�@
		ID3D11Buffer* pIndexBuf;	// �C���f�b�N�X�o�b�t�@
	#endif

	#ifdef L2D_TARGET_PS4
		bool initializedBuffer;
		float* color;//�����x�܂߂��|�����킹��F�Bfloat[4]�B�V�F�[�_�p
		sce::Gnm::Buffer buffers[2];//position , textureUV 
	#endif
	} ;
} 
//------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif	// __LIVE2D_DDTEXTURE_H__
