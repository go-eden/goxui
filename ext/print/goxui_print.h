//
// Created by sulin on 2019/4/7.
//

#ifndef GOXUI_PRINT_H
#define GOXUI_PRINT_H

#ifdef GOXUI
    #define API Q_DECL_EXPORT
#else
    #define API
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

/**
 * Initialize goxui's print external library.
 */
API void ui_init_print();

#ifdef __cplusplus
}
#endif

#endif //GOXUI_PRINT_H
