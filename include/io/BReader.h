/**
 *  BReader.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_B_READER_H__
#define __LIVE2D_B_READER_H__


#ifndef __SKIP_DOC__


#include "../memory/LDObject.h"
#include "ByteBuffer.h"

#include "../type/LDVector.h"

//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	class AMemoryHolder ;
	
	
	class BReader : public live2d::LDObject 
	{
	public:
		static const int FLAG_READ_AS_USHORT_ARRAY	= 0x1 ;// int[]���A���[�h����ushort[]�Ƃ��Ĉ���
		static const int FLAG_MALLOC_ON_GPU			= 0x2 ;// GPU��Ƀ��������m�ۂ���
	
	
		// ���[�h���ɃI�u�W�F�N�g��ۊǂ���e�[�u���̃T�C�Y
		// ��������vector��resize����������B�傫���ƃ���������������
		// �g�p�������� sizeof(void*) * LOAD_OBJECT_INITIAL_CAPACITY�ƂȂ�
		// ����́A���f���̒��ɃI�u�W�F�N�g���𖄂ߍ��ޕ����ɕύX����
		static const int LOAD_OBJECT_INITIAL_CAPACITY = 10000 ; 
	
	
		// �J���̕��@���m�肹�����S�ł͂Ȃ����ߕK���O���ŊJ������悤�ɕύX
		//BReader( const char * buf , int length , bool deleteBufOnDestructor );
		BReader( const char * buf , int length );
		virtual ~BReader();
		
		//  �t�@�C���t�H�[�}�b�g�̃o�[�W���� 
		int getFormatVersion(){ return formatVersion ; }
	
		void setFormatVersion(int version){ formatVersion = version ; }
		
		void rollback( int byteLen ) ;// �w��̃o�C�g���߂�
	
		// Flags�ɂ͈ȉ���n��
		//	FLAG_READ_AS_USHORT_ARRAY	= 0x1 ;// int[]���A���[�h����ushort[]�Ƃ��Ĉ���
		//	FLAG_MALLOC_ON_GPU			= 0x2 ;// GPU��Ƀ��������m�ۂ���
		void * readObject( MemoryParam * memParam , int cno = -1 , int flags = 0 ) ;
		bool readBit() ;
		int readNum(){ return bin.readNum() ; } ;// �ϒ��̒������擾����(1..4�o�C�g�܂ł𒷂��ɉ����Ďg��������
		float readFloat(){	checkBits() ; return bin.readFloat() ;	}
		double readDouble(){ checkBits() ; return bin.readDouble() ;	}
		long long readLong(){ checkBits() ; return bin.readLong() ;	}
		int readInt() { checkBits() ; return bin.readInt() ;}
		bool readBoolean(){ checkBits() ; return bin.readBoolean() ;}
		char readByte(){ checkBits() ; return bin.readByte() ;}// 1byte���Ԃ��ijava�̃o�C�g�ƑΉ��Bchar��Java�ƌ^���قȂ�̂Ŗ��̂������j
		short readShort(){ checkBits() ; return bin.readShort() ;	}
	
		//  �i�|�C���^�j��Ԃ��B �O����L2D_FREE( )���K�v
		double* readArrayDouble(MemoryParam* memParam,int *ret_length){ checkBits() ; return bin.readArrayDouble(memParam, ret_length) ;	}// size, float* ��߂��K�v������
		float* readArrayFloat(MemoryParam* memParam,int *ret_length){ checkBits() ; return bin.readArrayFloat(memParam, ret_length) ;	}// size, float* ��߂��K�v������
	
		int * readArrayInt(MemoryParam* memParam,int *ret_length){ checkBits() ; return bin.readArrayInt(memParam, ret_length ) ;}
		unsigned short * readArrayIntAsUShort(MemoryParam* memParam,int *ret_length){ checkBits() ; return bin.readArrayIntAsUShort(memParam, ret_length ) ;}
	
		LDVector<double>* readArrayDoubleAsVector(MemoryParam* memParam){ checkBits() ; return bin.readArrayDoubleAsVector(memParam) ;	}// size, float* ��߂��K�v������
		LDVector<float>* readArrayFloatAsVector(MemoryParam* memParam){ checkBits() ; return bin.readArrayFloatAsVector(memParam) ;	}// size, float* ��߂��K�v������
		LDVector<int> * readArrayIntAsVector(MemoryParam* memParam){ checkBits() ; return bin.readArrayIntAsVector(memParam) ;}
		
		//  �}���`�o�C�g�����͖����؁ibytes�Œ������w�肵�Ă���̂ł����ł̃G���[�͂łȂ��j 
		live2d::LDString* readString(MemoryParam* memParam ){ checkBits() ; return bin.readString(memParam) ;	}
		RefString& readStringAsRef(){ checkBits() ; return bin.readStringAsRef() ;	}
	
		// c�̃|�C���^��Ԃ��A�I�[�L���܂Ői�߂�
		const char* readCStr(){ checkBits() ; return bin.readCStr() ; }
		char* getCurPtr(){ return bin.getCurPtr() ; }
		void setEndian( bool isBigEndian ) { bin.setEndian(isBigEndian) ; }
		void skip( int bytes ){ bin.skip( bytes ) ; }

		// �ǂݍ��݂Ȃ���Endian�����ւ��Ă����ꍇ��true
		// ��x�ǂݍ��񂾃o�b�t�@���ēx���[�h����ꍇ�ɓ��ڂ𓯂�Endian�ň�����悤�ɂ���
		void setChangeEndian( bool change ) { bin.setChangeEndian( change ) ; }
	
	#if L2D_SAMPLE
		static void TEST_1() ;
		static void TEST_MODEL_LOAD() ;
	#endif
	
		
	private:
		void* read_array_ent( MemoryParam* memParam ,int targetClassNo, int restDimension) ;// �߂�l�́A�z��^ ( �v���~�e�B�u������̂ŁA�K������Object[]�ł͂Ȃ� )
	
		void checkBits()
		{
			if( bitCount != 0 ) bitCount = 0 ;
		}
		
		int formatVersion ;
		
		ByteBuffer bin ;
		int bitCount ; //= 0 ;
		int bitBuff ;//= 0 ;// ���݂̃r�b�g�t���O���Ǘ�
	
		// TODO �I�u�W�F�N�g���X�g�̊Ǘ�������
		LDVector<void*> loadObjects ;// �������ꂽ�I�u�W�F�N�g�͑S�ēo�^�����B���[�h���ʂȂ̂Œ��g�͉�����Ȃ�
		
	};
}
//------------------------- LIVE2D NAMESPACE -------------

#endif // __SKIP_DOC__

#endif		// __LIVE2D_B_READER_H__