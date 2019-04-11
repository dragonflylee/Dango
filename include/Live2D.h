/**
 *  Live2D.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */

#ifndef __LIVE2D_H__
#define __LIVE2D_H__

//========================================================
//  Compile target
// 	�i�v���W�F�N�g�ɒ�`���Ȃ��ꍇ#if�����R�����g���O��)
//========================================================
#if !( defined(L2D_TARGET_PSP) \
		|| defined(L2D_TARGET_IPHONE)	   \
		|| defined(L2D_TARGET_IPHONE_ES2) 	\
		|| defined(L2D_TARGET_MAC_OSX) 		\
		|| defined(L2D_TARGET_ANDROID) 	\
		|| defined(L2D_TARGET_ANDROID_ES2) 	\
		|| defined(L2D_TARGET_QT) 			\
		|| defined(L2D_TARGET_D3D) 			\
		|| defined(L2D_TARGET_D3D11) 		\
		|| defined(L2D_TARGET_WIN_GL) 		\
		|| defined(L2D_TARGET_3DS)		  \
		|| defined(L2D_TARGET_PS3 ) 		\
		|| defined(L2D_TARGET_PS4 ) 		\
		|| defined(L2D_TARGET_VITA ) 		\
	)
		//#define L2D_TARGET_PSP
		//#define L2D_TARGET_IPHONE
		//#define L2D_TARGET_IPHONE_ES2
		//#define L2D_TARGET_MAC_OSX
		//#define L2D_TARGET_ANDROID
		//#define L2D_TARGET_ANDROID_ES2
		//#define L2D_TARGET_QT
		//#define L2D_TARGET_D3D
		//#define L2D_TARGET_D3D11
		//#define L2D_TARGET_WIN_GL
		//#define L2D_TARGET_3DS
		//#define L2D_TARGET_PS3
		//#define L2D_TARGET_PS4
		//#define L2D_TARGET_VITA
#endif

#if defined( L2D_TARGET_PSP )
#define __L2D_PLATFORM_STR__ ("PSP")
#elif defined( L2D_TARGET_IPHONE )
#define __L2D_PLATFORM_STR__ ("iPhone")
#elif defined( L2D_TARGET_IPHONE_ES2 )
#define __L2D_PLATFORM_STR__ ("iPhone ES2")
#elif defined( L2D_TARGET_MAC_OSX )
#define __L2D_PLATFORM_STR__ ("MacOSX")
#elif defined( L2D_TARGET_ANDROID )
#define __L2D_PLATFORM_STR__ ("Android")
#elif defined( L2D_TARGET_ANDROID_ES2 )
#define __L2D_PLATFORM_STR__ ("Android ES2")
#elif defined( L2D_TARGET_QT )
#define __L2D_PLATFORM_STR__ ("Qt")
#elif defined( L2D_TARGET_D3D )
#define __L2D_PLATFORM_STR__ ("DirectX9")
#elif defined( L2D_TARGET_D3D11 )
#define __L2D_PLATFORM_STR__ ("DirectX11")
#elif defined( L2D_TARGET_D3D_OPEN )
#define __L2D_PLATFORM_STR__ ("DirectX_OPEN")
#elif defined( L2D_TARGET_WIN_GL )
#define __L2D_PLATFORM_STR__ ("Windows OpenGL")
#elif defined( L2D_TARGET_3DS )
#define __L2D_PLATFORM_STR__ ("3DS")
#elif defined( L2D_TARGET_VITA )
#define __L2D_PLATFORM_STR__ ("Vita")
#elif defined( L2D_TARGET_PS3 )
#define __L2D_PLATFORM_STR__ ("PS3")
#elif defined( L2D_TARGET_PS4 )
#define __L2D_PLATFORM_STR__ ("PS4")
#endif


#if defined( L2D_TARGET_ANDROID ) || defined( L2D_TARGET_IPHONE )
#define L2D_GRAPHICS_OPENGL_ES1
#endif

#if defined( L2D_TARGET_ANDROID_ES2 ) || defined( L2D_TARGET_IPHONE_ES2 ) || defined( L2D_TARGET_WIN_GL )
#define L2D_GRAPHICS_OPENGL_ES2
#endif

//========================================================
//	Options
//========================================================
// Unity�p�ɏ����o���Ƃ���1�ɂ��Ȃ��ƃe�N�X�`���Ȃǂ�����邩������Ȃ�
#define L2D_BUILD_FOR_UNITY			0

//------------ Debug options ---------------
#define L2D_SAMPLE			0	// �T���v���f�[�^�A�e�X�g���\�b�h�Ȃǂ𗘗p����ꍇ�� 1
#define L2D_VERBOSE			1	// �G���[���b�Z�[�W�Ȃǂ��͂������ꍇ�� 1

// �f�o�b�O�p�t���O�B�J�����̂�1
#if defined( L2D_TARGET_IPHONE ) || defined ( DEBUG )
#  define L2D_DEBUG			1
#elif defined( L2D_TARGET_3DS ) && defined ( NN_BUILD_DEBUG )
#  define L2D_DEBUG			1
#elif defined ( _DEBUG )
#  define L2D_DEBUG			1
#endif


#define L2D_TEMPORARY_DEBUG 0	// �ꎞ�I�ȃe�X�g�p

#define L2D_ALPHA_IMPL_LAYOUT 0 // ���f���Ɋg��k����ݒ肷��e�X�g����


//------------ Performance options ---------------
#define L2D_RANGE_CHECK		1	// �z��ւ̃A�N�Z�X���ɔ͈̓`�F�b�N���s���ꍇ��1�i���܂�p�x�͍����Ȃ��j
#define L2D_RANGE_CHECK_POINT	1	// 0..1��ԂɎ��܂�ׂ��_�̃`�F�b�N���s���ꍇ��1�@���x���ቺ����B
									// �`�F�b�N�����Ȃ��Ɣz��ɕs���A�N�Z�X����\�������� (70%->72%���x�ɃA�b�v����)
#define L2D_DEFORMER_EXTEND		1	// �f�t�H�[�}���g������(SDK2.0)

#define L2D_FORCE_UPDATE	0	// �p�����[�^�̍X�V�Ɋ֌W�Ȃ��A�S�ẴI�u�W�F�N�g���A�b�v�f�[�g����ꍇ��1�ɂ���

//------------ Texture options ------------
#if defined( L2D_TARGET_IPHONE ) || defined( L2D_TARGET_ANDROID )
#  define L2D_INVERT_TEXTURE 0
#elif defined( L2D_TARGET_IPHONE_ES2 ) || defined( L2D_TARGET_ANDROID_ES2 ) || defined( L2D_TARGET_WIN_GL)
	#if L2D_BUILD_FOR_UNITY
		#  define L2D_INVERT_TEXTURE 1
	#else
		#  define L2D_INVERT_TEXTURE 0
	#endif
#elif defined( L2D_TARGET_VITA ) || defined( L2D_TARGET_D3D11 ) || defined( L2D_TARGET_PS4 )
#  define L2D_INVERT_TEXTURE 0
#else
#  define L2D_INVERT_TEXTURE 1
#endif

//------------ Avatar options ------------
#define AVATAR_OPTION_A 1

//========================================================
//  Exception
//========================================================
#if defined( L2D_TARGET_PSP ) || defined ( L2D_TARGET_VITA ) || defined(L2D_TARGET_PS4 ) || defined(L2D_TARGET_3DS)	|| defined(L2D_TARGET_PS3 )
	#define L2D_THROW( c )	{}
#elif defined(L2D_TARGET_IPHONE_ES2)|| defined( L2D_TARGET_ANDROID_ES2 ) || defined( L2D_TARGET_ANDROID )
	#define L2D_THROW( c )	{}
#else
	#define L2D_THROW( c ) throw (c)
#endif

//========================================================
//  typedef
//========================================================
typedef float				l2d_uvmapf ;	// �ʏ� float �^��uv map�l
typedef float				l2d_pointf ;	// �ʏ� float �^��point �l
typedef float				l2d_paramf ;	// �ʏ� float�^�̃p�����[�^�l
typedef unsigned short		l2d_index ;		// �C���f�b�N�X�z��̌^
typedef signed short		l2d_order ;		// �`�揇���̌^(signed short)
typedef float				l2d_float ;		// �����x�ȂǊ�{�I�� float �^

#if defined(L2D_TARGET_IPHONE_ES2) || defined(L2D_TARGET_MAC_OSX)
typedef unsigned long	   l2d_size_t ;
#elif defined(L2D_TARGET_PS4)
#include <stddef.h>
typedef  size_t	   l2d_size_t ;
#elif defined(L2D_TARGET_WIN_GL) || defined(L2D_TARGET_D3D) || defined(L2D_TARGET_D3D11)
typedef size_t	  l2d_size_t ;
#elif defined(L2D_TARGET_IPHONE)
#include <stdlib.h>
typedef size_t	  l2d_size_t ;
#else
typedef unsigned int		l2d_size_t ;	// size_t �݊�, 64bit�ł�64bit�ɂ���i���Ή��j
#endif

// ��{�^�iLDxxx -> l2d_xxx�Ɉڍs�j
typedef bool				l2d_bool ;
typedef signed char			l2d_int8 ;
typedef unsigned char		l2d_uint8 ;

typedef signed short		l2d_int16 ;
typedef unsigned short		l2d_uint16 ;

typedef signed int			l2d_int32 ;
typedef unsigned int		l2d_uint32 ;

typedef signed long long	l2d_int64 ;
typedef unsigned long long	l2d_uint64 ;

// ����{�^�i�݊����ێ��̂��ߕێ��j
typedef signed char			LDint8 ;
typedef unsigned char		LDuint8 ;

typedef signed short		LDint16 ;
typedef unsigned short		LDuint16 ;

typedef signed int			LDint32 ;
typedef unsigned int		LDuint32 ;

typedef signed long long	LDint64 ;
typedef unsigned long long	LDuint64 ;


// ---------- �͈͊O�p�����[�^�̈���  ----------
#define L2D_OUTSIDE_PARAM_AVAILABLE 0	// �p�����[�^���͈͊O�̂Ƃ��`�悷��Ȃ� 1
//#define L2D_OUTSIDE_PARAM_BASE_AVAILABLE 1	// �p�����[�^���͈͊O�̂Ƃ����W�ϊ���L���ɂ��� 1



//========================================================
//	Compiler
//========================================================
#ifdef _MSC_VER
#pragma warning (disable : 4100)
#endif

#ifndef NULL
#  define NULL	0
#endif

//========================================================
//	Live2D
//========================================================
namespace live2d
{
	class LDAllocator;
}

#include "memory/UtMemory.h"


//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	
	class Live2D
	{
	public:
		//static const l2d_uint32 L2D_VERSION_NO 	= __L2D_VERSION_NO__ ;// ���[�h�G���[���������悤�C��

		//========================================================
		// 	�G���[�萔 Live2D::L2D_ERROR_XXX
		// 	Live2D.getError()�ŕԂ����
		//========================================================
		//   0- �G���[����
		static const int L2D_NO_ERROR								= 0 ;

		// 1000- Live2D�S�ʁE�������֘A
		static const int L2D_ERROR_LIVE2D_INIT_FAILED				= 1000 ;
		static const int L2D_ERROR_FILE_LOAD_FAILED					= 1001 ;
		static const int L2D_ERROR_MEMORY_ERROR						= 1100 ;

		// 2000- ���f���֘A�G���[
		static const int L2D_ERROR_MODEL_DATA_VERSION_MISMATCH		= 2000 ;	// ���̃��C�u�����Ŗ��Ή��o�[�W�����̃f�[�^
		static const int L2D_ERROR_MODEL_DATA_EOF_ERROR				= 2001 ;	// �I�[���s��

		// 3000- ���[�V�����֘A


		static const int L2D_COLOR_BLEND_MODE_MULT			= 0;
		static const int L2D_COLOR_BLEND_MODE_ADD			= 1;
		static const int L2D_COLOR_BLEND_MODE_INTERPOLATE = 2;

		static int getClippingMaskBufferSize();
		static void setClippingMaskBufferSize(int size);


		// Live2D�̃��C�u�����������������p�\�ȏ�Ԃɂ��܂�
		static void init( live2d::LDAllocator* allocator = NULL ) ;

		// Live2D�̃��C�u�������N���[�Y���A�S�Ẵ��\�[�X��������܂�
		// �������A�O������ݒ肳�ꂽ�摜�Ȃǂ̃��\�[�X�͉�����܂���
		// �O���œK�؂ɔj������K�v������܂��B
		static void dispose() ;

		// �o�[�W����������擾
		static const char * getVersionStr() ;

		// �o�[�W�����擾
		static l2d_uint32 getVersionNo() ;

		// ���C�u�����r���h���̐ݒ���m�F����
		static l2d_bool getBuildOption_RANGE_CHECK_POINT() ;
		static l2d_bool getBuildOption_AVATAR_OPTION_A() ;

		// ���_�̃_�u���o�b�t�@�̐ݒ�
		
		static void setVertexDoubleBufferEnabled(l2d_bool enabled)
		{
			flag_vertexDoubleBufferEnabled = enabled ;
		}


		
		static l2d_bool isVertexDoubleBufferEnabled()
		{
			return flag_vertexDoubleBufferEnabled ;
		}

		// �G���[�ݒ�(��������)
		static void setError(l2d_uint32 errorNo) ;

		// �G���[�̎擾
		static l2d_uint32 getError() ;

	#ifdef L2D_TARGET_PSP
		// PSP�ɂ����ă|���S���̕`��O�ɒ��_�z������C�g�o�b�N����ꍇ�� true��ݒ�(0.9.00b15)
		static void setPSPWritebackEnabled_alphaImpl(l2d_bool enabled)
		{
			flag_PSPWritebackEnabled_alphaImpl = enabled ;
		}

		// PSP�ɂ����ă|���S���̕`��O�ɒ��_�z������C�g�o�b�N����ꍇ�� true(0.9.00b15)
		// �f�t�H���g�� false
		static l2d_bool isPSPWritebackEnabled_alphaImpl()
		{
			return flag_PSPWritebackEnabled_alphaImpl ;
		}
	#endif


	private:
		Live2D(){}							// �C���X�^���X�������Ȃ�

		// Prevention of copy Constructor
		Live2D( const Live2D & ) ;
		Live2D& operator=( const Live2D & ) ;
		~Live2D(){}

	private:
		//--- field ---
		static l2d_bool flag_first_init ;
		static l2d_bool flag_vertexDoubleBufferEnabled ;// 0.9.00b14 DDTexture�̃|���S�����_�u���o�b�t�@����ꍇ��true

	#ifdef L2D_TARGET_PSP
		static l2d_bool flag_PSPWritebackEnabled_alphaImpl ;// 0.9.00b15 PSP�ɂ����ă|���S�����_�����C�g�o�b�N����ꍇtrue
	#endif

		static l2d_uint32	lastError ;
		static l2d_int32	initCount ;	// Live2D::init()�������񔭐�����̂�h���BLive2D::dispose()���s�킸��init()�𕡐���s�����ꍇ�͖�������B

		static int clippingMaskBufferSize;
	};

}
//--------- LIVE2D NAMESPACE ------------


#endif		// __LIVE2D_H__