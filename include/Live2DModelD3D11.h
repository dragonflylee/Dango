/*
 *  Live2DModelD3D11.h
 * 
 *  外部から参照されるモデルデータ
 * 
 *  内部の構造は ModelImpl
 * 
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */


#ifndef __LIVE2D_L2DMODEL_D3D11_H__
#define __LIVE2D_L2DMODEL_D3D11_H__


#include "Live2D.h"						// プラットフォーム別設定の前で必ず呼び出すこと
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

		// デバイスロストした時に共通して呼び出す
		static void deviceLostCommon();

	public:
		// Constructor
		Live2DModelD3D11( void );

		// Destructor
		virtual ~Live2DModelD3D11( void );

	public:
		// テクスチャセット
		void setTexture( int textureNo, ID3D11ShaderResourceView* texture );

		// 登録されたテクスチャを破棄する
		// テクスチャを外部で共有している場合があるため
		// 登録されたテクスチャは自動的には破棄されない。
		// 破棄するにはDestructorの前に明示的に呼ぶ必要がある。
		void deleteTextures();

		// モデルロード
		static Live2DModelD3D11* loadModel( const l2d_string& filepath );

		// モデルロード
		static Live2DModelD3D11* loadModel( const void* buf, int bufSize );

		// Direct3Dのデバイスロスト時の処理
		void deviceLostD3D();

		void setMatrix( float*  matrix ) ;

		// 描画
		virtual void draw();

		// 新しく利用できるModelのテクスチャ番号を確保(Avatar用)
		virtual int generateModelTextureNo();
	
		// Modelのテクスチャ番号を生成(Avatar用)
		virtual void releaseModelTextureNo( int no );

		// 描画用パラメータを取得
		virtual live2d::DrawParam* getDrawParam(){ return drawParamD3D; }

		void setTextureColor(int textureNo,float r,float g,float b);
		void setTextureBlendMode(int textureNo,int mode);
		void setTextureInterpolate(int textureNo, float interpolate);

		
		int getErrorD3D_tmp(){return drawParamD3D->getErrorD3D_tmp();}

	private:
		// Prevention of copy Constructor処理
		Live2DModelD3D11( const Live2DModelD3D11& );
		Live2DModelD3D11& operator=( const Live2DModelD3D11& );

	private:
		DrawParam_D3D11* drawParamD3D;		// Direct3Dの描画パラメータ格納用
	};
}
#endif		//L2D_TARGET_D3D11

#endif		//__LIVE2D_L2DMODEL_D3D11_H__