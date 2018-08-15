
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ocl_helper.hpp"

cl::Program CreateProgram(const std::string &file) {

    std::cout << "file: " << file << "\n";

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.at(2);
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

    assert(devices.size() > 0);

    auto device = devices.at(0);

    std::ifstream cl_file(file);
    std::string src(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

    cl::Context context(device);
    cl::Program program(context, sources);

    auto err = program.build("-cl-std=CL1.2");

    std::cout << "platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\nvendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\ntype: " << device.getInfo<CL_DEVICE_TYPE>() << "\n\n";

    assert(err == 0);
    std::cout << "err: " << err << "\n";
    return program;
}