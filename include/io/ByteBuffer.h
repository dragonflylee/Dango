/**
 *  ByteBuffer.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_BYTE_BUFFER_H__
#define __LIVE2D_BYTE_BUFFER_H__


#ifndef __SKIP_DOC__


#include "../Live2D.h"
#include "../type/LDVector.h"
#include "../memory/LDObject.h"
#include "RefString.h"

#define __LIVE2D_BYTE_BUFFER_DEBUG___
#define CHANGE_ENDIAN_WITH_PTR 0		//  PSP�̃o�C�g�A���C���������|����C�z

//--------- LIVE2D NAMESPACE ------------
namespace live2d
{
	class ByteBuffer 
	{
	
	public:
		ByteBuffer(char *array, int length);
	
		virtual ~ByteBuffer();
	
		void rollback(int byteLen);
	
		float readFloat();	
		double readDouble();
		long long readLong();
		int readNum();
		int readInt();
		bool readBoolean();
		char readByte();
		short readShort();
		double *readArrayDouble(MemoryParam* owner, int *ret_length);
		float *readArrayFloat(MemoryParam* owner, int *ret_length);
		int *readArrayInt(MemoryParam* owner, int *ret_length);		
		unsigned short* readArrayIntAsUShort(MemoryParam* owner, int *ret_length);
		LDVector<double> *readArrayDoubleAsVector(MemoryParam* owner);
		LDVector<float> *readArrayFloatAsVector(MemoryParam* owner);
		LDVector<int> *readArrayIntAsVector(MemoryParam* owner);
		live2d::LDString* readString(MemoryParam* owner );
		RefString& readStringAsRef();
	
		const char* readCStr() ;
		char* getCurPtr(){ return cur_ptr ; }
		void setEndian( bool isBigEndian ) { this->isBigEdianData = isBigEndian ; }
		void skip( int bytes ) ;

		// �ǂݍ��݂Ȃ���Endian�����ւ��Ă����ꍇ��true
		// ��x�ǂݍ��񂾃o�b�t�@���ēx���[�h����ꍇ�ɓ��ڂ𓯂�Endian�ň�����悤�ɂ���
		void setChangeEndian( bool change ) { this->changeEndian = change ; }

		LDVector<live2d::LDString> & getErrors();
	

		static void TEST_BYTE_BUFFER();
		static char *CREATE_SAMPLE_FILE_DATA(int *size);
		static void TEST_FILE_LOAD();
		static void staticInit_notForClientCall();
	
	private:
		void setError(const live2d::LDString & str);
		void setRangeError();
		void setSampleFileData(char *cur_ptr);
		void CHECK_VALUE(char *& src_data, int bufSize);
	
	private:
		char *				start_ptr ;			// �|�C���^�̐擪
		char *				cur_ptr ;			// ���݂̃|�C���^
		char *				end_ptr ;			// �|�C���^�̏I�[
		int 				length ;			// 
		
		LDVector<LDString> 	errors;				// �G���[������
		char 				cache8[8];			//
	
		RefString			refString ;			// ���[�h���̕�������ꎞ�I�ɕێ����邽�߂̌^�i�g���܂킷�B�J�����Ȃ��j
	
		static bool isBigEndianSys ;			// SYSTE(Hard)��Endian
		bool isBigEdianData ;					// 

		bool changeEndian ;						// ���[�h����Endian�����ւ��Ă����ꍇ��true
	
	};
}
//--------- LIVE2D NAMESPACE ------------

#endif		// __SKIP_DOC__

#endif		// __LIVE2D_BYTE_BUFFER_H__