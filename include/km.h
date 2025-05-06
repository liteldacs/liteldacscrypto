
#ifndef KM_H
#define KM_H


/* #undef USE_SDF */
/* #undef USE_PIICO */
#define USE_GMSSL

#ifdef USE_SDF

#include <sdf/libsdf.h>
#include <sdfkmt/sdfe-func.h>
#include <sdfkmt/sdfe-type.h>
#define ALGO_WITH_KEK SGD_SM4_ECB
#endif

#ifdef USE_PIICO
#include <piico_pc/api.h>
#include <piico_pc/piico_define.h>
#include <piico_pc/piico_error.h>
#define ALGO_WITH_KEK SGD_SM4_ECB
#endif

#ifdef USE_GMSSL
#include <gmssl/sdf/sdf.h>
#define ALGO_WITH_KEK SGD_SM4_CBC

#endif

#endif
