
//#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <utility>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

int main(int argc, const char * argv[])
{
	std::vector<cl::Platform> platforms;

	cl::Platform::get(&platforms);

	assert(platforms.size() > 0);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);


	for(int i = 0; i < devices.size(); ++i) {
		std::cout << "dev" << i << "\nvendor: " << devices.at(i).getInfo<CL_DEVICE_VENDOR>()
				<< "\nType: " << devices.at(i).getInfo<CL_DEVICE_TYPE>();
	}
	assert(devices.size() > 0);

	auto device = devices.at(0);
	auto type = device.getInfo<CL_DEVICE_TYPE>();
	auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
	auto version = device.getInfo<CL_DEVICE_VERSION>();

	std::cout << "\nvendor: " << vendor << "\ntype: " << type << "\n\n";

	std::ifstream kecl(argv[1]);
	std::string src(std::istreambuf_iterator<char>(kecl), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	cl::Context context(device);

	cl::Program program(context, sources);

	auto err = program.build("-cl-std=CL2.0");

	std::cout << "err " << err << "\n";

	char buf[16];

	buf[0] = 'H';
	buf[1] = 'e';
	buf[2] = 'l';
	buf[3] = 'l';
	buf[4] = 'o';
	buf[5] = ',';
	buf[6] = ' ';
	buf[7] = 'W';
	buf[8] = 'o';
	buf[9] = 'r';
	buf[10] = 'l';
	buf[11] = 'd';
	buf[12] = '\n';

	std::cout << buf;
	std::cout << "\n";

	cl::Buffer mem_buf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));
	cl::Kernel kernel(program, "kernel_example", &err);

	kernel.setArg(0, mem_buf);

	cl::CommandQueue queue(context, device);
	queue.enqueueTask(kernel);
	queue.enqueueReadBuffer(mem_buf, CL_TRUE, 0, sizeof(buf), buf);

	std::cout << err;
	std::cout << "\n";

	std::cout << buf;
	std::cout << "\n";
}