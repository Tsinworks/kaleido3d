#pragma once

#include <stdint.h>
#include "spirv.h"
#include "ngfx_shader.h"

namespace ngfx
{
	namespace spirv
	{
		struct Instruction
		{
			uint16_t op;
			uint16_t count;
			uint32_t offset;
			uint32_t length;
		};
	}

	class SpirvParser
	{
	public:
		SpirvParser(const uint32_t* data, uint32_t size);
		~SpirvParser();

		void getShaderResource(Vec<ShaderResource>& shr);

	private:
		void initAndParse();

		const uint32_t* data_;
		uint32_t		size_;
	};
}