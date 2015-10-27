#ifndef _COMMON_H_
#define _COMMON_H_

#define DEBUG_LOG(x) serial_log(__func__, __LINE__, x)

void serial_log(const char *func, int line, String message);


#endif // _COMMON_H_
