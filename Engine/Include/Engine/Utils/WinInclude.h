#pragma once

#define NOMINMAX

#define ON_FAILURE_CATCH_HRESULT(hr, context)  \
  if (FAILED(hr)) { \
    ErrorHandler::Get().CatchHRESULT(hr, context); \
    return false; \
  }
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdlib>
#include <wincodec.h>
#include <iostream>
#include <wrl/client.h>
#include <Include/Engine/Core/Diagnostics/ErrorHandler.h>
#include <Include/Engine/Core/Interfaces/BaseUnknown.h>


using Microsoft::WRL::ComPtr;


#ifdef _DEBUG
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif

