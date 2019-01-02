#include "Public/ngfx_shell.h"
#include "CoreMinimal.h"

static void log_print(int level, const char* msg)
{

}

typedef ngfx::Factory* (*fnCreate)(bool debug, decltype(log_print) call);

int main(int argc, char**argv) {
    k3d::os::LibraryLoader loader("ngfx_vk.dll");
    fnCreate create = (fnCreate)loader.ResolveSymbol("CreateFactory");
    ngfxu::Factory factory(create(true, log_print));
    ngfxu::Device device = factory.getDevice(0);
    ngfxu::CommandQueue queue = device.newQueue();
    ngfxu::Buffer buffer = device.newBuffer(ngfx::BufferDesc(), ngfx::StorageMode::Auto);
    ngfxu::RaytracingAccelerationStructure as = 
        device.newRaytracingAccelerationStructure(ngfx::RaytracingASDesc());
    //auto texture = device->newTexture(nullptr, ngfx::StorageMode::Auto);
    
    return 0;
}