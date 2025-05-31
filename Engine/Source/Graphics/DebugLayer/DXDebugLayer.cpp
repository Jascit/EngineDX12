#include <Graphics/DebugLayer/DXDebugLayer.h>



bool DXDebugLayer::initialize(){
#ifdef _DEBUG
	HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(m_d3d12Interface.GetAddressOf()));
	if (SUCCEEDED(hr))
	{
		m_d3d12Interface->EnableDebugLayer();
		hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_dxgiInterface.GetAddressOf()));
		if (SUCCEEDED(hr)) {
			m_dxgiInterface->EnableLeakTrackingForThread();
			return true;
		}
		else
		{
			ErrorHandler::Get().CatchHRESULT(hr, "DXGI Debug Interface is not initialized");
			return false;
		}
	}
	else
	{
		ErrorHandler::Get().CatchHRESULT(hr, "D3D12 Debug Interface is not initialized");
		return false;
	}
#endif // _DEBUG
	return false;
}

void DXDebugLayer::shutdown(){
#ifdef _DEBUG
	if (m_dxgiInterface)
	{
		OutputDebugStringW(L"DXGI Reports living device objects:\n");
		m_dxgiInterface->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}

	m_dxgiInterface.Reset();
	m_d3d12Interface.Reset();
#endif // _DEBUG
}
