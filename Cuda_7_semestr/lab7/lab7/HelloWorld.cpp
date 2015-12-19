// HelloWorld.cpp
// Hello World Implementation

#define ARRAY_SIZE 10000

#include <iostream>
#include <cstdlib>
#include <chrono> 
#include <fstream>
#include <sstream>
#include <time.h>

#include <CL/opencl.h>

//#include "functions.h"

using namespace std;

int main(int argc, char * argv[])
{

	// Start timing.

	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[3] = {0, 0, 0};
	cl_device_id *devices;
	size_t deviceBufferSize = -1;
	cl_int errNum;

	cl_uint numPlatforms;
	cl_platform_id firstPlatformID;

	// Select an OpenCL platform
	errNum = clGetPlatformIDs(1, &firstPlatformID, &numPlatforms);

	// Create an OpenCL context on the platform.
	// Default to GPU context first, CPU context next
	cl_context_properties contextProperties[]=
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties) firstPlatformID,
		0
	};
	context = clCreateContextFromType(contextProperties,
		CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);

	//Get the size of the devices buffer
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0,
		NULL, &deviceBufferSize);

	//Allocate memory for the devices buffer
	devices = new cl_device_id[deviceBufferSize/
		sizeof(cl_device_id)];
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES,
		deviceBufferSize, devices, NULL);
	
	//Choose the first available device
	commandQueue = clCreateCommandQueue(context, devices[0], 0,
		NULL);

	device = devices[0];
	delete [] devices;

	char *fileName = "HelloWorld.cl";

	ifstream kernelFile(fileName, ios::in);
	kernelFile.is_open();
	ostringstream oss;
	oss << kernelFile.rdbuf();
	string srcStdStr = oss.str();
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	// Create OpenCL kernel
	kernel = clCreateKernel(program, "hello_kernel", NULL);

	// Create memory objects that will be used as arguments to kernel.
	// First create host memory arrays that will store the arguments to kernel.

	float result[ARRAY_SIZE];
	float a[ARRAY_SIZE];
	float b[ARRAY_SIZE];

	for(int i = 0; i < ARRAY_SIZE; i++)
	{
		a[i] = i;
		b[i] = 2*i;
	}

	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeof(float)*ARRAY_SIZE, NULL, NULL);


	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
	
	size_t globalWorkSize[1] = {ARRAY_SIZE};
	size_t localWorkSize[1] = {1};

	clock_t begin;
	clock_t end;


	begin = clock();
	// Queue the kernel up for execution across the array
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	// Transfer the result from buffer back to host.
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, sizeof(float)*ARRAY_SIZE,
								 result, 0, NULL, NULL);

	for(int i = 1; i < ARRAY_SIZE; i++) {
		result[0] += result[i];
	}

	end = clock();

	std::cout << "OpenCl time = " << (end - begin) / (float) CLOCKS_PER_SEC << std::endl;

	float result_verify[ARRAY_SIZE];


	begin = clock();
	for (int i = 0; i < ARRAY_SIZE; i++) {
		for (int j = 0; j < 10000; j++) {
			result_verify[i] = a[i] * b[i] * cos((float)j);
		}
		result_verify[0] += result_verify[i];
	}

	end = clock();

	std::cout << "Cpu time = " << (end - begin) / (float) CLOCKS_PER_SEC << std::endl;

	std::cout << "result = " << result[0] << endl;
	std::cout << "result_verify = " << result_verify[0] << endl;

	float diff = result[0] - result_verify[0] ;
	std::cout << "diff = " << diff << endl;

	for (int i = 0; i < 3; i++)
	{
		if (memObjects[i]!=0)
			clReleaseMemObject(memObjects[i]);
	}
	if (commandQueue!=0)
		clReleaseCommandQueue(commandQueue);
	if (kernel!=0)
		clReleaseKernel(kernel);
	if (program!=0)
		clReleaseProgram(program);
	if (context!=0)
		clReleaseContext(context);

	system("pause");
	return 0;
}
