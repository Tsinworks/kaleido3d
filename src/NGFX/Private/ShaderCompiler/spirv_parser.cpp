#include "spirv_parser.h"

namespace ngfx
{
	SpirvParser::SpirvParser(const uint32_t* data, uint32_t size)
		: data_(data), size_(size) 
	{
		initAndParse();
	}

	SpirvParser::~SpirvParser()
	{

	}

	void SpirvParser::initAndParse()
	{

	}

	void SpirvParser::getShaderResource(Vec<ShaderResource>& shr)
	{

	}
}