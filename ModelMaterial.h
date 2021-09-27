#pragma once
#include "DXHeader.h"
#include <fbxsdk.h>
#include <string>

class ModelMaterial
{
private:
	UINT number;
	XMFLOAT4 ambient;
	XMFLOAT4 emissive;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	float shininess;

	std::wstring ambientFile;
	std::wstring emissiveFile;
	std::wstring diffuseFile;
	std::wstring specularFile;
	std::wstring normalMapFile;

	ID3D11ShaderResourceView** ambientView;
	ID3D11ShaderResourceView** emissiveView;
	ID3D11ShaderResourceView** diffuseView;
	ID3D11ShaderResourceView** specularView;
	ID3D11ShaderResourceView** normalMapView;

	ID3D11Device* DXEngine;
	ID3D11DeviceContext* DXEngineContext;
private:
	XMFLOAT4 GetProperty(FbxSurfaceMaterial* material, const char* name, const char* factorName, std::wstring* textureName);
	void GetProperty(FbxSurfaceMaterial* material, const char* name, std::wstring* textureName);
	void CreateView(std::wstring path, ID3D11ShaderResourceView** view);
public:
	ModelMaterial(int number, FbxSurfaceMaterial* material);
	~ModelMaterial();

	ID3D11ShaderResourceView* GetAmbientView();
	ID3D11ShaderResourceView* GetEmissiveView();
	ID3D11ShaderResourceView* GetDiffuseView();
	ID3D11ShaderResourceView* GetSpecularView();
	ID3D11ShaderResourceView* GetNormalMapView();
};

