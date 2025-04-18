#pragma once
#include <Include/Engine/Core/Resources/Components/IComponents.h>
#include <Include/Engine/Math/Matrix/Matrix.h>
#include <vector>
#include <Include/Engine/Utils/RawContainer.h>
#include <queue>
using namespace DirectX;
struct VertexData {
  DirectX::XMFLOAT4 pos;
  DirectX::XMFLOAT4 nor;
  DirectX::XMFLOAT2 texcoord;
  uint32_t cbvIndex;
};
struct IndicesData {

};
class MeshComponent : IComponent {
public:
  MeshComponent(std::vector<VertexData>& vertices,  std::vector<IndicesData>& indicies);
};