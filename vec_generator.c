#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int debug; 
#define DEBUG(...) do {		 	\
	if(debug) {			\
		printf("[DEBUG] ");	\
		printf(__VA_ARGS__);	\
		fflush(stdout);		\
	}				\
} while (0)

float* random_vec_gen(uint32_t dim, uint32_t nvecs, float rmin, float rmax){
	uint32_t total_floats = dim*nvecs; // 0 to 4,294,967,295 
	float* vec = (float*) malloc(total_floats * sizeof(float));
	if(!vec){
		fprintf(stderr, "Error: Memory Allocation Failed for %u floats\n", total_floats);
		exit(EXIT_FAILURE);
	}
	srand((unsigned int)time(NULL));
	for(uint32_t i = 0; i < total_floats; ++i){
		float scale = (float)rand() / (float)RAND_MAX;
		vec[i] = rmin + scale * (rmax - rmin);
	}
	return vec;
}

void write_vec(uint32_t nvecs, uint32_t dim, float* vecs, const char* fname){
	FILE* bfile = fopen(fname, "wb");
	if(!bfile){
		fprintf(stderr, "Error: Failed to open file '%s' for writing\n", fname);
		exit(EXIT_FAILURE);
	}
	// write metadata headers 
	fwrite(&nvecs, sizeof(uint32_t), 1, bfile);
	fwrite(&dim, sizeof(uint32_t), 1, bfile);
	
	//binary payload 
	uint32_t total_floats = nvecs * dim;
	size_t fwritten = fwrite(vecs, sizeof(float), total_floats, bfile);
	if(fwritten != total_floats) {
		fprintf(stderr, "Warning: Only wrote %zu out of %u expected floats\n", 
	  			fwritten, total_floats);
	}
	
	fclose(bfile); return;

}

int main(int argc, char* argv[]){

	if(argc != 6) {
		fprintf(stderr, "Usage: ./%s [dimension] [num_vectors] [rmin] [rmax] [fname]\n", argv[0]);
		fprintf(stderr, "Example: ./%s 128 1000 -1.0 1.0 vector.bin\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char* val = getenv("DEBUG");
	debug = val != NULL && strcmp(val, "1") == 0; 

	uint32_t dim = (uint32_t) strtoul(argv[1], NULL, 10);
	uint32_t nvecs = (uint32_t) strtoul(argv[2], NULL, 10);
	float rmin = strtof(argv[3], NULL);
	float rmax = strtof(argv[4], NULL);
	const char* fname =  argv[5];


	if (dim * nvecs >  4294967294){
		fprintf(stderr, "vector size greater than 4,294,967,294"); 
		return EXIT_FAILURE;
	}

	DEBUG("dim: %u, nvecs: %d, rmin: %f, rmax: %f, fname: %s\n", dim, nvecs, rmin, rmax, fname);
	DEBUG("Generating %u vectors of dimension %u (Range: %.2f to %.2f) ...\n",nvecs, dim, rmin, rmax);

	float* vecs = random_vec_gen(dim, nvecs, rmin, rmax);
	write_vec(nvecs, dim, vecs, fname);
	printf("Successfully wrote data to %s \n", fname);

	free(vecs);
	return 0;
}

