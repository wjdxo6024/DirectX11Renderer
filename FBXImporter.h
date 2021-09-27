#pragma once
#ifndef _FBXIMPORTER_H_
#define _FBXIMPORTER_H_
#include "DXHeader.h"

#include <fbxsdk.h>
#include <vector>

class FBXImporter
{
private:
	
public:
	void ProcessMesh(FbxNode* inNode);
	void ProcessControlPoints(FbxNode* inNode);
};

#endif