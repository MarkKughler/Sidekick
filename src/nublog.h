#pragma once
#include <iostream>

/* About: Color Codes
 * https://tforgione.fr/posts/ansi-escape-codes/
 *
 *   0 - black
 *   1 - red
 *   2 - green
 *   3 - yellow
 *   4 - blue
 *   5 - purple
 *   6 - light blue
 *   7 - white
 *
 * console escape code (color, pos)
 */
#define _ec(x) "\x1b["#x"m"

#ifdef NDEBUG

#define no_op void(0)
 // Macro: LOG(x) 
 // default log
#define LOG(x) 
// Macro: LOG_INFO(x, info)
// information log
#define LOG_INFO(x, info) 
// Macro: LOG_INFO2(info, sz, x, y)
// named dimension information log 
#define LOG_INFO2(info, sz, x, y) 
// Macro: LOG_INFO3(info, x, y, z)
// vector information log
#define LOG_INFO3(info, x, y, z)
// Macro: LOG_ERROR(x, info)
// error log
#define LOG_ERROR(x, info)
// Macro: LOG_WARN(x, info)
// warning log
#define LOG_WARN(x, info) 

// Macro: LOG_BLOCK_START
// set state block
#define LOG_BLOCK_START 
// Macro: LOG_BLOCK_END
// end state block
#define LOG_BLOCK_END 
#define LOG_WARN_START(x)
#define LOG_ERROR_START(x)

#else
#define LOG(x) std::cout << #x << std::endl

#define LOG_INFO(loc, info) std::cout << _ec(32) << "[Info] " << loc << " " << info << _ec(0) << std::endl
#define LOG_INFO2(info, sz, x, y) std::cout << _ec(32) << "[Info] " << info << " " << sz << " (" << x << ", " << y << ")" << _ec(0) << std::endl
#define LOG_INFO3(info, x, y, z) std::cout << _ec(32) << "[Info] " << info << " (" << x << ", " << y << ", " << z << ")" << _ec(0) << std::endl
#define LOG_ERROR(loc, info) std::cout << _ec(41)_ec(30) << "[Error] " << loc << "    \n" << info << _ec(0) << std::endl
#define LOG_WARN(loc, info) std::cout << _ec(43)_ec(30) << "[Warning] " << loc << "    \n" << info << _ec(0) << std::endl

#define LOG_BLOCK_START std::cout << _ec(36)
#define LOG_BLOCK_END std::cout << _ec(0) << std::endl
#define LOG_WARN_START(loc) std::cout << _ec(43)_ec(30) << "[Warning] " << loc << "  \n"
#define LOG_ERROR_START(loc) std::cout << _ec(41)_ec(30) << "[Error] " << loc << "\n"

#endif

