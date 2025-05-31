#pragma once
#include <Utils/WinInclude.hpp>
#include <Core/DX12/DXContext.hpp>

class Window
{
public:
  void shutdown();
  bool initialize();
  void update();
  void preset();
  void resize();
  void SetFullscreen(bool enabled);

  void startFrame(ID3D12GraphicsCommandList7* cmdList);
  void endFrame(ID3D12GraphicsCommandList7* cmdList);

  inline bool shouldClose() const
  {
    return m_shouldClose;
  }

  inline bool isFullScreen() const
  {
    return m_isFullscreen;
  }

  inline bool shouldResize() const
  {
    return m_shouldResize;
  }

  inline uint32_t GetWidth() const
  {
    return m_width;
  }

  inline uint32_t GetHeight() const
  {
    return m_height;
  }

  static constexpr uint32_t frameCount = 2;
  static constexpr uint32_t GetFrameCount() {
    return frameCount;
  }
private:
  bool createBuffers();
  void destroyBuffers();
  void createDepthBuffer();

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
private:
  ATOM m_wndClass;
  HWND m_wndHandle;
  uint32_t m_height;
  uint32_t m_width;

  bool m_shouldResize;
  bool m_shouldClose;
  bool m_isFullscreen;

  ComPtr<IDXGISwapChain3> m_swapChain;
  ComPtr<ID3D12Resource2> m_buffers[frameCount];
  ComPtr<ID3D12DescriptorHeap> m_descHeap;
  D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandles[frameCount];

  uint32_t m_currentIndexBuffer;

  ComPtr<ID3D12Resource2> m_depthBuffer;
  ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
public:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  static inline Window& Get() {
    static Window instance;
    return instance;
  }
private:
  Window();
};

