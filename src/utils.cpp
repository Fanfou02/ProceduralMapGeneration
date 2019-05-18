#include "utils.h"
#include "voxels.h"


void Error( const char *info ) {
	printf( "[Error] MV_VoxelModel :: %s\n", info );
}



size_t spin_the_bottle(const std::vector<double>& a, double between_zero_and_one)
{
	double sum = std::accumulate(a.begin(), a.end(), 0.0);

	if (sum == 0.0) {
		return std::floor(between_zero_and_one * a.size());
	}

	double between_zero_and_sum = between_zero_and_one * sum;

	double accumulated = 0;

	for (unsigned long i = 0; i < a.size(); ++i) {
		accumulated += a[i];
		if (between_zero_and_sum <= accumulated) {
			return i;
		}
	}

	return 0;
}

void checkBounds(size_t index, size_t bound){
	if(index >= bound) {
		std::cerr << "Index Out of Bounds: index(" << index << "); bound(" << bound << ")" << std::endl;
		print_stacktrace();
		exit(0);
	}
}
