# DirectX11Renderer
2021-09-28
-현 렌더러는 Fbx파일인 DragonTest1.fbx 파일을 파싱하고 불러오는 작업을 하고 있습니다.

-GameScene - TestMultiTextureScene에서 모델을 초기화하고 렌더링합니다.

-ModelScene에서 애니메이션 부분은 완성되지 않았습니다. 기본적인 과정은 ModelScene - Model - ModelPart - ModelMesh 순입니다.

-Direct3D와 Direct2D는 D3DFrame에서 초기화되고 진행됩니다.

-입력은 현재 간단히 화살표 방향키 앞뒤만 받게 만들었습니다.
