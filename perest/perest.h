typedef unsigned int uint;

typedef struct {
	char* cycle;
	int* val;
	uint len;
} perest;


perest* new_perest(uint);

char* to_cycle(perest*);
perest* get_perest(char*, uint);
void print_perest(perest*, char);
void print_naive(perest*, char);

perest* copy(perest*);
int compare(perest* , perest* );
perest* compose(perest*, perest*);