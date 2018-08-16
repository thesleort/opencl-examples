
#include <iostream>
#include <array>
#include <assert.h>

#include "ocl_helper.hpp"

int main(int argc, const char *argv[]) {

    cl::Program program = CreateProgram(argv[1]);

    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto &device = devices.front();

	assert(devices.size() > 0);

    const int rows = 3;
    const int cols = 2;
    const int count = rows * cols;
    std::array<std::array<int, cols>, rows> arr = {{{1, 1}, {2, 2}, {3, 3}}};

    cl::Buffer buf(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * count, arr.data());
    cl::Kernel kernel(program, "ProcessMultiArray");

	cl_int err = 0;
	auto workgroupsize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);

    kernel.setArg(0, buf);

    cl::CommandQueue queue(context, device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(2, 3));
    queue.enqueueReadBuffer(buf, CL_TRUE, 0, sizeof(int) * count, arr.data());

    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < arr.at(i).size(); ++j) {

            std::cout << arr.at(i).at(j);
        }
    }
	std::cout << "\n";
}