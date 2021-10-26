//====================================================================================================================//
// k_util.h                                                                                                           //
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

#ifndef K_UTIL_H
#define K_UTIL_H

#define K_UTIL_MAX_LOG_LENGTH 256

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

double get_time();
int sec_to_min(int seconds);
int sec_to_h(int seconds);
char *read_file(char *fileName);
char *get_file_ext(char *filename);
void safe_free(void *ptr);
void msg(char *format, ...);
void dbg(char *format, ...);
void err(char *format, ...);
void panic(char *format, ...);

//--------------------------------------------------------------------------------------------------------------------//
// implementation                                                                                                     //
//--------------------------------------------------------------------------------------------------------------------//

#ifdef K_UTIL_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

double get_time() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec / 1000000;
}

int sec_to_min(int seconds) {
	return seconds / 60;
}

int sec_to_h(int seconds) {
	return sec_to_min(seconds) / 60;
}

char *read_file(char *fileName) {
	char *source = NULL;
	FILE *fp = fopen(fileName, "r");

	if (fp != NULL) {
		if (fseek(fp, 0L, SEEK_END) == 0) {
			long bufsize = ftell(fp);
			source = malloc(sizeof(char) * (bufsize + 1));
			fseek(fp, 0L, SEEK_SET);

			size_t newLen = fread(source, sizeof(char), bufsize, fp);

			if (ferror(fp) != 0)
				return NULL;
			else
				source[newLen++] = '\0';
		}

		fclose(fp);
	}

	return source;
}

char *get_file_ext(char *filename) {
	char *dot = strrchr(filename, '.');
	return (!dot || dot == filename) ? "" : dot + 1;
}

void safe_free(void *ptr) {
	if (ptr != NULL) {
		free(ptr);
		ptr = NULL;
	}
}

void msg(char *format, ...) {
	char buff[K_UTIL_MAX_LOG_LENGTH];

	va_list args;
	va_start(args, format);
	vsnprintf(buff, K_UTIL_MAX_LOG_LENGTH, format, args);
	va_end(args);

	printf("[\e[1m\e[32mMSG\e[0m] %s\n", buff);
	fflush(stdout);
}

void dbg(char *format, ...) {
#ifdef K_UTIL_DEBUG
	char buff[K_UTIL_MAX_LOG_LENGTH];

	va_list args;
	va_start(args, format);
	vsnprintf(buff, K_UTIL_MAX_LOG_LENGTH, format, args);
	va_end(args);

	printf("[\e[1m\e[33mDBG\e[0m] %s\n", buff);
	fflush(stdout);
#endif
}

void err(char *format, ...) {
	char buff[K_UTIL_MAX_LOG_LENGTH];

	va_list args;
	va_start(args, format);
	vsnprintf(buff, K_UTIL_MAX_LOG_LENGTH, format, args);
	va_end(args);

	printf("[\e[1m\e[31mERR\e[0m] %s\n", buff);
	fflush(stdout);
}

void panic(char *format, ...) {
	char buff[K_UTIL_MAX_LOG_LENGTH];

	va_list args;
	va_start(args, format);
	vsnprintf(buff, K_UTIL_MAX_LOG_LENGTH, format, args);
	va_end(args);

	printf("[\e[1m\e[31mPANIC!\e[0m] %s\n", buff);
	fflush(stdout);

	exit(-1);
}

#endif

#endif
