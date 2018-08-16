
#include <iostream>

#include "ocl_helper.hpp"

int main(int argc, const char *argv[]) {
    cl::Program program = CreateProgram(argv[1]);

    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    // auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto devices = program.getInfo<CL_PROGRAM_DEVICES>();
	std::cout << "ocl context devices: " <<   program.getInfo<CL_PROGRAM_CONTEXT>().getInfo<CL_CONTEXT_DEVICES>().size() << "\n";
    auto &device = devices.front();

    std::vector<int> vec(1024);
    // std::fill(vec.begin(), vec.end(), 1);

    cl::Buffer in_buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * vec.size(), vec.data());
    cl::Buffer out_buf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * vec.size());

    cl::Kernel kernel(program, "ProcessArray");
    kernel.setArg(0, in_buf);
    kernel.setArg(1, out_buf);

    cl::CommandQueue queue(context, device);

    queue.enqueueFillBuffer(in_buf, 3, sizeof(int) * 10, sizeof(int) * (vec.size() - 10));

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
    queue.enqueueReadBuffer(out_buf, CL_FALSE, 0, sizeof(int) * vec.size(), vec.data());

    cl::finish();

    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec.at(i);
    }
	std::cout << "\n";
}