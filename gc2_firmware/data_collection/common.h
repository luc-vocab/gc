#ifndef _COMMON_H_
#define _COMMON_H_

#define MANAGE_WIFI true // whether to switch off wifi in batch mode

#define DEBUG_LOG(x) serial_log(__func__, __LINE__, x)

void serial_log(const char *func, int line, String message);


#endif // _COMMON_H_
