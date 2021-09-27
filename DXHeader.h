#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

// DirectX 11

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DDSTextureLoader.h>
using namespace DirectX;

#include "AlignedAllocationPolicy.h"

// cpp
#include <cstdlib>
#include <memory>
#include <tchar.h>
#include <cwchar>
#include <cmath>
#include <DirectXMath.h>
// Vertex
struct ModelVertex {
	float _x, _y, _z;
	float _u, _v;
	float _nx, _ny, _nz;
};

struct ModelBumpVertex {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

struct ModelSkinningVertex {
	float _x, _y, _z; // Position
	float _u, _v; // UV
	float _nx, _ny, _nz; // Normal
	float _bwx, _bwy, _bwz, _bww; // Blend Weight
	float _bix, _biy, _biz, _biw; // Blend Index
	float _tx, _ty, _tz; // Tangent
	float _bx, _by, _bz; // Binormal

};

struct ModelVertexForTest {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct ModelVertex2 {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

// 애니메이션

//struct ModelFrame {
//	std::string FrameName;
//	XMMATRIX TransformationMatrix;
//	// 모델 데이터
//	ModelFrame* FrameSibling;
//	ModelFrame* FrameFirstChild;
//};

// 매크로
#define IF_IS_NOT_NULL_DELETE(x) if(x != NULL) {delete x;}
#define IF_IS_NOT_NULL_DELETE_ARRAY(x) if(x != NULL) {delete [] x;}
#define IF_IS_NOT_NULL_QUIT_AND_DELETE(x) if(x != NULL) { x->Quit(); delete x;}
#define IF_IS_NOT_NULL_QUIT_AND_DELETE_ARRAY(x) if(x != NULL) { x->Quit(); delete [] x;} // 오류 가능성 존재
