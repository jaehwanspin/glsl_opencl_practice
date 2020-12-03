#include <iostream>
#include <string>

#define CL_TARGET_OPENCL_VERSION 120
#include <boost/compute/device.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/source.hpp>
#include <boost/compute/program.hpp>
#include <boost/compute/utility.hpp>
#include <boost/compute/system.hpp>
namespace bc = boost::compute;

std::string vector_addition_src = BOOST_COMPUTE_STRINGIZE_SOURCE(
__kernel void add(__global const int* a,
                  __global const int* b,
                  __global       int* c)
{
    const uint i = get_global_id(0);
    c[i] = a[i] + b[i];
}
);

int main(int argc, char** argv)
{
    bc::device cl_device = bc::system::default_device();
    bc::context cl_ctx(cl_device);
    bc::command_queue cmd_que(cl_ctx, cl_device); 

    int a[] = { 1 };
    int b[] = { 2 };
    int c[] = { 0 };
    bc::buffer buffer_a(cl_ctx, sizeof(int));
    bc::buffer buffer_b(cl_ctx, sizeof(int));
    bc::buffer buffer_c(cl_ctx, sizeof(int));

    bc::program add_program =
        bc::program::create_with_source(vector_addition_src, cl_ctx);
    add_program.build();
    bc::kernel add_kernel(add_program, "add");
    add_kernel.set_arg(0, buffer_a);
    add_kernel.set_arg(1, buffer_b);
    add_kernel.set_arg(2, buffer_c);
    
    cmd_que.enqueue_write_buffer(buffer_a, 0, sizeof(int), a);
    cmd_que.enqueue_write_buffer(buffer_b, 0, sizeof(int), b);
    cmd_que.enqueue_1d_range_kernel(add_kernel, 0, 1, 0);
    cmd_que.enqueue_read_buffer(buffer_c, 0, sizeof(int), c);

    std::cout << c[0] << std::endl;

    return 0;
}
