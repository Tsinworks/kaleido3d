#pragma once
#include "ngfx.h"
namespace ngfxu
{
    template <typename T>
    class Handle {
    public:
        using Super = Handle<T>;

        Handle(T* obj) : ptr_(obj) {}

        Handle<T>& operator=(Handle<T> const& other) {
            ptr_.swap(other);
            return this;
        }

    protected:
        ngfx::Ptr<T> ptr_;
    };

    class CommandQueue : public Handle<ngfx::CommandQueue> {
    public:
        CommandQueue(ngfx::CommandQueue* queue) : Super(queue) {}
    };

    class RaytracingAccelerationStructure : public Handle<ngfx::RaytracingAS>
    {
    public:
        RaytracingAccelerationStructure(ngfx::RaytracingAS* as) : Super(as) {}
    };

    class Buffer : public Handle<ngfx::Buffer> {
    public:
        Buffer(ngfx::Buffer* buffer) : Super(buffer) {}
    };

    class Device : public Handle<ngfx::Device> {
    public:
        Device(ngfx::Device* device) : Super(device) {}
        CommandQueue newQueue() {
            ngfx::Result result;
            return CommandQueue(ptr_->newQueue(&result));
        }
        Buffer newBuffer(const ngfx::BufferDesc& desc, ngfx::StorageMode mode) {
            ngfx::Result result;
            return ptr_->newBuffer(&desc, mode, &result);
        }
        RaytracingAccelerationStructure newRaytracingAccelerationStructure(const ngfx::RaytracingASDesc& desc) {
            ngfx::Result result;
            return RaytracingAccelerationStructure(ptr_->newRaytracingAS(&desc, &result));
        }
    };

    class Factory : public Handle<ngfx::Factory> {
    public:
        Factory(ngfx::Factory* factory) : Super(factory) {}
        Device getDevice(uint32_t id) { return Device(ptr_->getDevice(id)); }
    };
}