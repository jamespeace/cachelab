#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>

#define MAX_BUF 30

typedef struct {
	int valid;
	uint64_t tag;
} setblock;

typedef struct {
	setblock* pblock;
	int lru;
} entry;

int main(int argc, char *argv[])
{
	int opt;
	int f_help, f_verbose, f_set, f_asso, f_block, f_trace;
	int setNum, assoNum, blockSize;
	char *traceFile;
	entry *pcache;
	FILE *stream;
	char line[MAX_BUF];
	size_t len = 0;
	ssize_t read;

	f_help = f_set = f_asso = f_block = f_trace = 0;
	setNum = assoNum = blockSize = 0;
	traceFile = NULL;

	while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch (opt) {
		case 'h':
			f_help = 1;
			break;
		case 'v':
			f_verbose = 1;
			break;
		case 's':
			f_set = 1;
			setNum = atoi(optarg);
			break;
		case 'E':
			f_asso = 1;
			assoNum = atoi(optarg);
			break;
		case 'b':
			f_block = 1;
			blockSize = atoi(optarg);
			break;
		case 't':
			f_trace = 1;
			traceFile = optarg;
			break;
		default:
			fprintf(stderr, "Usage %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}
	
	if ((f_set == 0) || (f_asso == 0) || (f_block == 0) || (f_trace == 0)) {
		fprintf(stderr, "%s: Missing required argument\n", argv[0]);
		fprintf(stderr, "Usage %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Implement cacle table */
	pcache = calloc(setNum, sizeof(entry));
	if (!pcache) {
		fprintf(stderr, "ERROR: allocate pcache failed!\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < setNum; i++) {
		setblock* cacheline;

		cacheline = calloc(assoNum, sizeof(setblock));
		if (!cacheline) {
			fprintf(stderr, "ERROR: allocate pcache failed!\n");
			exit(EXIT_FAILURE);
		}
		pcache[i].pblock = cacheline;
	}

	/* get data */
	stream = fopen(traceFile, "r");
	if (stream == NULL) {
		fprintf(stderr, "open file failed!\n");
		goto END;
	}

	/* parse data */
	while ((read = getline(&line, &len, stream)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
	}
    //printSummary(0, 0, 0);
END:
	for (int i = 0; i < setNum; i++) {
		free(pcache[i].pblock);
	}
	free(pcache);
    return 0;
}
