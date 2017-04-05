#ifndef NpDebug_h__
#define NpDebug_h__

#include <fstream>
#include <time.h>

#define dwrite_log(...) dwrite_log_imp(__FUNCTION__, __LINE__,  __VA_ARGS__)

inline void dwrite_log_imp(const char* function, int line, const char* content, ...)
{
    FILE* file = fopen("Rocking.log", "a+");
    if (NULL == file)
        return;

    va_list args;
    va_start(args, content);
    char buf[65536];
    vsprintf(buf, content, args);
    va_end(args);

    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);
    fprintf(file, "[%04d/%02d/%02d %02d:%02d:%02d] %s(%d): %s\n",
        now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour,
        now->tm_min, now->tm_sec,
        function, line, buf);
    fflush(file);
    fclose(file);
}
#endif // NpDebug_h__
