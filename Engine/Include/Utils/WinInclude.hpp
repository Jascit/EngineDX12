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
#include <Core/Diagnostics/ErrorHandler.hpp>


using Microsoft::WRL::ComPtr;
using Entity = UINT;
const Entity INVALID_ENTITY = UINT_MAX;

#ifdef _DEBUG
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif

