#include <Core/Window/Window.hpp>

Window::Window() : m_height(800), m_width(600), m_shouldResize(false), m_shouldClose(false), m_cpuHandles{ NULL },
                   m_isFullscreen(false), m_currentIndexBuffer(0), m_wndClass(NULL), m_wndHandle(nullptr) {}

bool Window::initialize()
{
  WNDCLASSEXW wcExW = {};
  wcExW.cbSize = sizeof(wcExW);
  wcExW.hInstance = GetModuleHandle(nullptr);
  wcExW.lpszClassName = LPCWSTR(L"EngienClassWindow");
  wcExW.style = CS_OWNDC;
  wcExW.lpfnWndProc = &Window::WndProc;
  wcExW.lpszMenuName = NULL;
  wcExW.cbWndExtra = 0;
  wcExW.cbClsExtra = 0;
  wcExW.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcExW.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcExW.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wcExW.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
  m_wndClass = RegisterClassExW(&wcExW);

  if (m_wndClass == 0)
  {
    std::string error = "Err: can not create window class";
    Logger::Get().logError(error);
    return 0;
  }

  POINT pt;
  GetCursorPos(&pt);
  HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
  MONITORINFO monitorInfo{};
  monitorInfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfoW(monitor, &monitorInfo);

  m_wndHandle = CreateWindowExW(
    WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
    (LPCWSTR)wcExW.lpszClassName,
    LPCWSTR(L"Engine"),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    monitorInfo.rcMonitor.left + 0,
    monitorInfo.rcMonitor.top + 0,
    m_width,
    m_height,
    nullptr,
    nullptr,
    wcExW.hInstance,
    nullptr
  );


  DXGI_SWAP_CHAIN_DESC1 scd;
  DXGI_SWAP_CHAIN_FULLSCREEN_DESC scf;

  scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  scd.BufferCount = GetFrameCount();
  scd.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
  scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  scd.Height = m_height;
  scd.Width = m_width;
  scd.SampleDesc.Count = 1;
  scd.SampleDesc.Quality = 0;
  scd.Scaling = DXGI_SCALING_STRETCH;
  scd.Stereo = false;
  scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

  scf.Windowed = true;

  auto* factory = DXContext::Get().GetFactory();
  ComPtr<IDXGISwapChain1> sc1;
  ErrorHandler::Get().CatchHRESULT(factory->CreateSwapChainForHwnd(DXContext::Get().GetGraphicsQueue()->getQueue(), m_wndHandle, &scd, &scf, nullptr, &sc1), "Creating swapchain");
  HRESULT hr = sc1->QueryInterface(IID_PPV_ARGS(m_swapChain.GetAddressOf()));
  ON_FAILURE_CATCH_HRESULT(hr, "can not create swap chain");

  D3D12_DESCRIPTOR_HEAP_DESC dhd;
  dhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  dhd.NodeMask = 0;
  dhd.NumDescriptors = frameCount;
  dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

  DXContext::Get().GetDevice()->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_descHeap));

  auto firstDesc = m_descHeap->GetCPUDescriptorHandleForHeapStart();
  auto increment = DXContext::Get().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  for (size_t i = 0; i < frameCount; i++)
  {
    m_cpuHandles[i] = firstDesc;
    m_cpuHandles[i].ptr += increment * i;
  }

  D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
  dsvHeapDesc.NumDescriptors = 1;
  dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
  dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  DXContext::Get().GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_dsvHeap.GetAddressOf()));

  if (!createBuffers())
  {
    return false;
  }

  return true;
}

void Window::SetFullscreen(bool enabled)
{
  DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
  DWORD exStyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW;
  if (enabled)
  {
    style = WS_POPUP | WS_VISIBLE;
    exStyle = WS_EX_APPWINDOW;
  }
  SetWindowLongW(m_wndHandle, GWL_STYLE, style);
  SetWindowLongW(m_wndHandle, GWL_EXSTYLE, exStyle);

  if (enabled)
  {
    HMONITOR monitor = MonitorFromWindow(m_wndHandle, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (GetMonitorInfoW(monitor, &monitorInfo))
    {
      SetWindowPos(m_wndHandle, nullptr,
        monitorInfo.rcMonitor.left,
        monitorInfo.rcMonitor.top,
        monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
        monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
        SWP_NOZORDER
      );
    }
  }
  else
  {
    ShowWindow(m_wndHandle, SW_MAXIMIZE);
  }

  m_isFullscreen = enabled;
}
void Window::createDepthBuffer() {
  D3D12_RESOURCE_DESC depthStencilDesc = {};
  depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  depthStencilDesc.Alignment = 0;
  depthStencilDesc.Width = m_width;
  depthStencilDesc.Height = m_height;
  depthStencilDesc.DepthOrArraySize = 1;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilDesc.SampleDesc.Count = 1;
  depthStencilDesc.SampleDesc.Quality = 0;
  depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

  D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
  depthOptimizedClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
  depthOptimizedClearValue.DepthStencil.Stencil = 0;

  D3D12_HEAP_PROPERTIES hp;
  hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  hp.CreationNodeMask = 0;
  hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  hp.Type = D3D12_HEAP_TYPE_DEFAULT;
  hp.VisibleNodeMask = 0;

  DXContext::Get().GetDevice()->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_READ, &depthOptimizedClearValue, IID_PPV_ARGS(m_depthBuffer.GetAddressOf()));

  D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
  dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
  dsvDesc.Texture2D.MipSlice = 0;
  DXContext::Get().GetDevice()->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

bool Window::createBuffers() {
  createDepthBuffer();
  for (int i = 0; i < frameCount; i++)
  {
    if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i])))) {
      std::string error = "ERR: can not get buffer";
      Logger::Get().logError(error);
      return false;
    }

    D3D12_RENDER_TARGET_VIEW_DESC rtvd;
    rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvd.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvd.Texture2D.MipSlice = 0;
    rtvd.Texture2D.PlaneSlice = 0;

    DXContext::Get().GetDevice()->CreateRenderTargetView(m_buffers[i].Get(), &rtvd, m_cpuHandles[i]);
  }
  return true;
}

void Window::startFrame(ID3D12GraphicsCommandList7* cmdList)
{
  m_currentIndexBuffer = m_swapChain->GetCurrentBackBufferIndex();

  D3D12_RESOURCE_BARRIER barr;
  barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
  barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barr.Transition.pResource = m_buffers[m_currentIndexBuffer].Get();
  barr.Transition.Subresource = 0;

  D3D12_RESOURCE_BARRIER barr2;
  barr2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barr2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barr2.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_READ;
  barr2.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
  barr2.Transition.pResource = m_depthBuffer.Get();
  barr2.Transition.Subresource = 0;

  D3D12_RESOURCE_BARRIER barrs[] = { barr, barr2 };

  cmdList->ResourceBarrier(2, barrs);
  D3D12_CPU_DESCRIPTOR_HANDLE temp = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
  float color[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
  cmdList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
  cmdList->ClearRenderTargetView(m_cpuHandles[m_currentIndexBuffer], color, 0, nullptr);
  cmdList->OMSetRenderTargets(1, &m_cpuHandles[m_currentIndexBuffer], false, &temp);
}

void Window::endFrame(ID3D12GraphicsCommandList7* cmdList)
{
  D3D12_RESOURCE_BARRIER  barr;
  barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
  barr.Transition.pResource = m_buffers[m_currentIndexBuffer].Get();
  barr.Transition.Subresource = 0;

  D3D12_RESOURCE_BARRIER barr2;
  barr2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barr2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barr2.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
  barr2.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_READ;
  barr2.Transition.pResource = m_depthBuffer.Get();
  barr2.Transition.Subresource = 0;

  D3D12_RESOURCE_BARRIER barrs[] = { barr, barr2 };
  cmdList->ResourceBarrier(2, barrs);
}

void Window::preset()
{
  m_swapChain->Present(1, 0);
}

void Window::update()
{
  MSG msg{};
  while (PeekMessage(&msg, m_wndHandle, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

void Window::destroyBuffers() {
  m_depthBuffer.Reset();
  for (size_t i = 0; i < frameCount; i++)
  {
    m_buffers[i].Reset();
  }
}

void Window::shutdown()
{
  destroyBuffers();
  m_swapChain.Reset();
  m_descHeap.Reset();
  if (m_wndHandle) {
    DestroyWindow(m_wndHandle);
  }
  if (m_wndClass)
  {
    UnregisterClassW((LPCWSTR)m_wndClass, GetModuleHandle(NULL));
  }
  if (m_dsvHeap) {
    m_dsvHeap.Reset();
  }
}

void Window::resize()
{
  destroyBuffers();
  RECT rc;
  if (GetClientRect(m_wndHandle, &rc))
  {
    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;

    m_swapChain->ResizeBuffers(frameCount, m_width, m_height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
    m_shouldResize = false;
  }
  createBuffers();
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
  switch (msg)
  {
  case WM_SIZE:
    if (lp && (HIWORD(lp) != Get().m_height || LOWORD(lp) != Get().m_width))
    {
      Get().m_shouldResize = true;
    }
    break;
  case WM_CLOSE:
    Get().m_shouldClose = true;
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_KEYDOWN:
    if (wp == VK_F11)
    {
      Get().SetFullscreen(!Get().isFullScreen());
    }
  }

  return DefWindowProc(hwnd, msg, wp, lp);
}


