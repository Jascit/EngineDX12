#pragma once
#include <Core/Resources/Components/IComponents.hpp>
#include <Math/Matrix/Matrix.hpp>
#include <vector>
#include <Utils/RawContainer.hpp>
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
