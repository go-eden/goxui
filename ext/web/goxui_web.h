//
// Created by sulin on 2019/4/7.
//

#ifndef GOXUI_WEB_H
#define GOXUI_WEB_H

#ifdef GOXUI
    #define API Q_DECL_EXPORT
#else
    #define API
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

/**
 * Initialize goxui's web ext library.
 */
API void ui_init_web();

#ifdef __cplusplus
}
#endif

#endif //GOXUI_WEB_H
