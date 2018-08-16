
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ocl_helper.hpp"

cl::Program CreateProgram(const std::string &file) {

    std::cout << "file: " << file << "\n";

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.at(1);
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	std::cout << "ocl devices: " << devices.size() << "\n";
    assert(devices.size() > 0);

    auto device = devices.at(0);

    std::ifstream cl_file(file);
    std::string src(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

    cl::Context context(device);
    cl::Program program(context, sources);

    auto err = program.build("-cl-std=CL1.2");

    cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
    if (status == CL_BUILD_ERROR) {
        // Get the build log
        std::string name = device.getInfo<CL_DEVICE_NAME>();
        std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        std::cerr << "Build log for " << name << ":" << std::endl
                  << buildlog << std::endl;
    }
std::string name = device.getInfo<CL_DEVICE_NAME>();
        std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        std::cerr << "Build log for " << name << ":" << std::endl
                  << buildlog << std::endl;
    std::cout << "platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\nvendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\ntype: " << device.getInfo<CL_DEVICE_TYPE>() << "\n\n";

	std::cout << "ocl context devices: " <<   program.getInfo<CL_PROGRAM_CONTEXT>().getInfo<CL_CONTEXT_DEVICES>().size() << "\n";

    std::cout << "err: " << err << "\n";
    assert(err == 0);
    return program;
}