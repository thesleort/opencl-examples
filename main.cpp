// #define CL_USE_DEPRECATED_OPENCL_2_0_APIS


#include <assert.h>
#include <iostream>
#include <fstream>

#include "ocl_helper.hpp"

int main(int argc, const char * argv[]) {

	// std::cout << argv[1] << "\n";
    auto program = CreateProgram(argv[1]);
    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    // auto devices = program.getInfo<CL_PROGRAM_DEVICES>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto device = devices.front();

	// for(int i = 0; i < devices.size(); ++i) {
	// 	std::cout << "device" << i << "\nvendor: " << devices.at(i).getInfo<CL_DEVICE_VENDOR>()
	// 			<< "\nType: " << devices.at(i).getInfo<CL_DEVICE_TYPE>() << "\n";
	// }

	// std::cout << "\nnum devices " << program.getInfo<CL_PROGRAM_DEVICES>().size() << "\n";
	assert(devices.size() > 0);

	char buf[16];
	
    cl::Buffer mem_buf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));
    cl::Kernel kernel(program, "kernel_example");
    kernel.setArg(0, mem_buf);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(mem_buf, CL_TRUE, 0, sizeof(buf), buf);

	std::cout << "out: " << buf;
	std::cout << "\n";

}