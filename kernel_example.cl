

__kernel void kernel_example(__global char* data) {
	data[0] = 'H';
	data[1] = 'e';
	data[2] = 'l';
	data[3] = 'l';
	data[4] = 'o';
	data[5] = ',';
	data[6] = ' ';
	data[7] = 'W';
	data[8] = 'o';
	data[9] = 'r';
	data[10] = 'l';
	data[11] = 'd';
	data[12] = '\n';
}


__kernel void ProcessArray(__global int* data, __global int* out_data) {
	out_data[get_global_id(0)] = data[get_global_id(0)] * 2;
}

__kernel void ProcessMultiArray(__global int* data) {

	size_t id = (get_global_id(1) * get_global_size(0)) + get_global_id(0);

	data[id] = data[id] * 2;
}

__kernel void NumericalReduction(__global int* data, __local int* local_data, __global int* out_data) {
	size_t global_id = get_global_id(0);
	size_t local_size = get_local_size(0);
	size_t local_id = get_local_id(0);

	local_data[local_id] = data[global_id];
	barrier(CLK_LOCAL_MEM_FENCE);

	for(int i = local_size >> 1; i > 0; i >>= 1) {
		if(local_id < i) {
			local_data[local_id] += local_data[local_id + i];
		}

		barrier(CLK_LOCAL_MEM_FENCE);
	} 

	if( local_id == 0) {
		out_data[get_group_id(0)] = local_data[0];
	}
}