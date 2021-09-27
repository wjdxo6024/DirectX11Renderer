#include "ModelMaterial.h"

XMFLOAT4 GetProperty(FbxSurfaceMaterial* material, const char* name, const char* factorName, std::wstring* textureName)
{
	return XMFLOAT4();
}

void ModelMaterial::GetProperty(FbxSurfaceMaterial* material, const char* name, std::wstring* textureName)
{
	FbxProperty prop = material->FindProperty(name);

	if (prop.IsValid())
	{
		UINT count = prop.GetSrcObjectCount<FbxFileTexture>();
		if (count > 0)
		{
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>();

		}
	}
}

// 각 텍스처 파일
void ModelMaterial::CreateView(std::wstring path, ID3D11ShaderResourceView** view)
{
	
}

XMFLOAT4 ModelMaterial::GetProperty(FbxSurfaceMaterial* material, const char* name, const char* factorName, std::wstring* textureName)
{
	return XMFLOAT4();
}

ModelMaterial::ModelMaterial(int number, FbxSurfaceMaterial* material)
{

}

ModelMaterial::~ModelMaterial()
{

}

ID3D11ShaderResourceView* ModelMaterial::GetAmbientView()
{
	return *ambientView;
}

ID3D11ShaderResourceView* ModelMaterial::GetEmissiveView()
{
	return *emissiveView;
}

ID3D11ShaderResourceView* ModelMaterial::GetDiffuseView()
{
	return *diffuseView;
}

ID3D11ShaderResourceView* ModelMaterial::GetSpecularView()
{
	return *specularView;
}

ID3D11ShaderResourceView* ModelMaterial::GetNormalMapView()
{
	return *normalMapView;
}