namespace vulkan
{
	inline void GpuFactory::printLogStr(int level, const char * msg)
	{
		if (log_call_) {
			log_call_(level, msg);
		}
	}

	inline bool GpuFactory::debuggable() const
	{
		return debug_enable_;
	}
}