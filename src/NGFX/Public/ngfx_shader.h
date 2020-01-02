/**
 * MIT License
 *
 * Copyright (c) 2019 Zhou Qin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ngfx_shader_20180511__
#define __ngfx_shader_20180511__
#pragma once

#if _WIN32
#if defined(NGFX_SHADER_BUILD)
#if defined(NGFX_SHADER_BUILD_SHARED_LIB)
#define NGFX_SHADER_API __declspec(dllexport)
#else
#define NGFX_SHADER_API __declspec(dllimport)
#endif
#else
#define NGFX_SHADER_API     
#endif
#else
#define NGFX_SHADER_API __attribute__((visibility("default"))) 
#endif
#include "ngfx.h"

namespace ngfx {
    enum class _ShaderProfile : int32_t
    {
        SM3_0 = 30, // Instancing
        SM3_1 = 31, // Compute
        SM5_0 = 50, // Compute, Tess
        SM6_0 = 60, // Add Raytracing
    };

#define MAKE_SHADER_FORMAT(a,b,c,d) uint32_t(a)<<24 | uint32_t(b) << 16 | uint32_t(c) << 8 | uint32_t(d)

    enum class _ShaderFormat : uint32_t
    {
        DXBC = MAKE_SHADER_FORMAT('D', 'X', 'B', 'C'), // SM5
        DXIL = MAKE_SHADER_FORMAT('D', 'X', 'I', 'L'), // SM6
        SPV1 = MAKE_SHADER_FORMAT('S', 'P', 'V', '1'), // VK
        HLSL = MAKE_SHADER_FORMAT('H', 'L', 'S', 'L'), // D3D
        GLSL = MAKE_SHADER_FORMAT('G', 'L', 'S', 'L'), // GL
        GLBC = MAKE_SHADER_FORMAT('G', 'L', 'B', 'C'), // GL Binary
        MTBC = MAKE_SHADER_FORMAT('M', 'T', 'B', 'C'), // Metal
        MTSL = MAKE_SHADER_FORMAT('M', 'T', 'S', 'L'), // Metal
    };

	struct ShaderResource
	{
		enum Type
		{
			Sampler,
			Constant,
			UniformBuffer,	// Or constant buffer
			Buffer,			// Buffer<float4>, RWBuffer, RWStructuredBuffer
			Texture1D,
			Texture2D,
			Texture3D,
			Texture1DArray,
			Texture2DArray,
			TextureCube,
			AccelerationStructure,
		};
		enum Access
		{
			Default = 0,
			ReadOnly = 1,
			WriteOnly = 2,
			ReadWrite = 3,
		};
		uint32_t type			: 4; // consume 16 types, buffer, texture, samplerstate, 
		uint32_t access			: 2; // 4 access
		uint32_t slot			: 5; // max to 32 (type) slots
		uint32_t sparse			: 1; // sparse binding resource
		uint32_t is_array		: 1;
		uint32_t is_vertexbuffer: 1;
		uint32_t has_structure  : 1; // if is structured buffer, need to know structure size
		uint32_t reserved		: 1;
		uint32_t stride			: 16; // size of structured element, max to 32768
		uint32_t array_length;
		char	 name[32];
	};

	static_assert(sizeof(ShaderResource) == 40, "size of ShaderResource is not 40!!");

	class NGFX_SHADER_API ShaderBinary
	{
	public:
        ShaderBinary();
        ~ShaderBinary();

        void Serialize(void* InOutData, size_t Length, bool In = false);

    private:
        // Length of shader
        int32_t				length_;
        int32_t				version_;
        _ShaderProfile		profile_;
		
		Vec<ShaderResource> shader_resource_table_;

        // DXBC/SPV1/DXIL/MTBC/GLSL/GLBC/ESSL/MTSL
        _ShaderFormat		format_;
        // Shader HashCode
        uint64_t			hash_; // use 128 instead ?
        // Shader Type
        //ShaderStageBit  m_Stage : 8;
		char				entry_point_[32];
	};

    class NGFX_SHADER_API ShaderLibrary
    {
    public:
        ShaderLibrary();
        ~ShaderLibrary();

        void Serialize(void* InOutData, size_t Length, bool In = false);
    private:
        int32_t				length_;
        int32_t				version_;
        uint64_t			hash_;
        ShaderBinary*		shaders_;
    };
}

#endif