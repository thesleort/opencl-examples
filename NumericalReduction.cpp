
#include <CL/cl.hpp>
#include <array>
#include <iostream>

#include "ocl_helper.hpp"

int main(int argc, char *argv[] ) {

	cl::Program program = CreateProgram(argv[1]);

	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	std::cout << "num devices: " << devices.size() << "\n";
    auto &device = devices.front();


	std::vector<int> vec(1024);

	for(int i = 0; i < vec.size(); ++i) {
		vec.at(i) = i;
	}	

	cl::Kernel kernel(program, "NumericalReduction");
	cl_int err = 0;
	auto workgroupsize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);
	std::cout << "err " << err << "\n";
	std::cout << "wg size: " << workgroupsize << "\n";

	auto num_work_groups = vec.size() / workgroupsize;

	cl::Buffer buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, vec.size(), vec.data());
	cl::Buffer out_buf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, num_work_groups);

	kernel.setArg(0, buf);
	kernel.setArg(1, sizeof(int) * workgroupsize, nullptr); // Don't create a buffer object on the host side when local
	kernel.setArg(2, out_buf);

	std::vector<int> out_vec(num_work_groups);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()), cl::NDRange(workgroupsize));
	queue.enqueueReadBuffer(out_buf, CL_TRUE, 0, sizeof(int) * out_vec.size(), out_vec.data());

	for(int i = 0; i < out_vec.size(); ++i) {
		std::cout << out_vec.at(i) << "\n";
	}
}