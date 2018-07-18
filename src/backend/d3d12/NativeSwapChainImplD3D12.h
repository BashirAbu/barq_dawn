// Copyright 2018 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BACKEND_D3D12_NATIVESWAPCHAINIMPLD3D12_H_
#define BACKEND_D3D12_NATIVESWAPCHAINIMPLD3D12_H_

#include "backend/d3d12/d3d12_platform.h"

#include "nxt/nxt_wsi.h"
#include "nxt/nxtcpp.h"

#include <vector>

namespace backend { namespace d3d12 {

    class Device;

    class NativeSwapChainImpl {
      public:
        using WSIContext = nxtWSIContextD3D12;

        NativeSwapChainImpl(Device* device, HWND window);
        ~NativeSwapChainImpl();

        void Init(nxtWSIContextD3D12* context);
        nxtSwapChainError Configure(nxtTextureFormat format,
                                    nxtTextureUsageBit,
                                    uint32_t width,
                                    uint32_t height);
        nxtSwapChainError GetNextTexture(nxtSwapChainNextTexture* nextTexture);
        nxtSwapChainError Present();

        nxt::TextureFormat GetPreferredFormat() const;

      private:
        HWND mWindow = nullptr;
        Device* mDevice = nullptr;

        ComPtr<IDXGISwapChain3> mSwapChain = nullptr;
        std::vector<ComPtr<ID3D12Resource>> mBuffers;
        std::vector<uint64_t> mBufferSerials;
        uint32_t mCurrentBuffer;
    };

}}  // namespace backend::d3d12

#endif  // BACKEND_D3D12_NATIVESWAPCHAINIMPLD3D12_H_
