#include "ngfx.enums.hpp"

namespace ngfx {
    struct DepthStencilOp {
        StencilOperation    stencilFailOp;
        StencilOperation    depthStencilFailOp;
        StencilOperation    stencilPassOp;
        ComparisonFunction  stencilFunc;
        uint32              compareMask;
        uint32              writeMask;
        uint32              reference;
    };
    struct DepthStencilState {
        bool                depthTest;
        DepthWriteMask      depthWriteMask;
        ComparisonFunction  depthFunction;
        bool                stencilTest;
        DepthStencilOp      front;
        DepthStencilOp      back;
    };
    struct RasterizerState {
        FillMode            fillMode;
        CullMode            cullMode;
        bool                frontCCW;
        bool                depthClipEnable;
        float               depthBiasClamp;
        float               depthBiasSlope;
        float               depthBias;
        MultisampleFlags    multisample;
    };
    struct RenderTargetBlendState {
        bool                blendEnable;
        BlendFactor         srcColor;
        BlendFactor         destColor;
        BlendOperation      colorOp;
        BlendFactor         srcAlpha;
        BlendFactor         destAlpha;
        BlendOperation      alphaOp;
        uint32              colorWriteMask;
    };
    struct BlendState {
       array<RenderTargetBlendState, 8> renderTargets;
    };
	struct Rect
	{
		uint32 x;
		uint32 y;
		uint32 width;
		uint32 height;
	};
	struct Viewport
	{
		float x;
		float y;
		float width;
		float height;
		float near;
		float far;
	};
	struct VertexLayout {
		VertexInputRate		inputRate;
		uint32				stride;
	};
	struct VertexAttribute {
		PixelFormat format;
		uint32 offset;
		uint32 slot;
	};
    struct VertexInputState {
		array<VertexAttribute>	attributes;
		array<VertexLayout>		layouts;
    };
	[[bitmask("true")]]
	enum BufferUsage {
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
		UnorderedAccess,
		AccelerationStructure
	};
	[[bitmask("true")]]
	enum TextureUsage {
        ShaderResource,
        RenderTarget,
        DepthStencil
	};
	struct BufferDesc {
		BufferUsage usages;
		uint32		size;
		uint64		deviceMask;
	};
	struct TextureDesc {
		TextureUsage usages;
		PixelFormat format;
		uint32		width;
		uint32		height;
		uint32		depth;
		uint32		layers;
		uint32		mipLevels;
		uint64		deviceMask;
	};

	struct SamplerDesc {
		FilterMode minFilter;
		FilterMode magFilter;
		FilterMode mipMapFilter;
		SamplerAddressMode u;
		SamplerAddressMode v;
		SamplerAddressMode w;
		ComparisonFunction comparisonFunction;
		uint32 maxAnistropy;
		float minLodBias;
		float minLod;
		float maxLod;
	};

    struct PresentLayerDesc {
        PixelFormat format;
        uint32      width;
        uint32      height;
        ColorSpace  colorSpace;
        bool        hdrDisplay;
        uint32      maxImages;
		void*		winHandle;
		void*		extraData;
    };
}