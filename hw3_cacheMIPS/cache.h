
struct cacheline {
	unsigned int tag : 16; // among 32 bit, use only a few bits
	unsigned int sca : 1; // 자주 사용하는 값 지우는 거 방지 위해 체크!
	unsigned int valid : 1;
	unsigned int dirty : 1;
	int data[64];
};

void print_cache(struct cacheline cache, int a);