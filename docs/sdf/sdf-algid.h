
#ifndef _SDF_ALGID_H_
#define _SDF_ALGID_H_

// ##############################################
// GM/T 0006
typedef enum BlockAlgId_en {
    SGD_SM1_ECB     = 0x00000101,
    SGD_SM1_CBC     = 0x00000102,
    SGD_SM1_CFB     = 0x00000104,
    SGD_SM1_OFB     = 0x00000108,
    SGD_SM1_MAC     = 0x00000110,

    SGD_SM1_CTR		= 0x00000120,
    SGD_SM1_XTS		= 0x00000140,

    SGD_SSF33_ECB   = 0x00000201,
    SGD_SSF33_CBC   = 0x00000202,
    SGD_SSF33_CFB   = 0x00000204,
    SGD_SSF33_OFB   = 0x00000208,
    SGD_SSF33_MAC   = 0x00000210,

    SGD_SM4_ECB     = 0x00000401,
    SGD_SM4_CBC     = 0x00000402,
    SGD_SM4_CFB     = 0x00000404,
    SGD_SM4_OFB     = 0x00000408,
    SGD_SM4_MAC     = 0x00000410,
    SGD_SM4_XTS		= 0x00000440,

    SGD_ZUC_EEA3    = 0x00000801,
    SGD_ZUC_EIA3    = 0x00000802,

    SGD_DES_ECB     = 0x00001001,
    SGD_DES_CBC     = 0x00001002,
    SGD_DES_CFB     = 0x00001004,
    SGD_DES_OFB     = 0x00001008,
    SGD_DES_MAC     = 0x00001010,
    SGD_DES_CTR     = 0x00001020,

    SGD_3DES_ECB    = 0x00002001,
    SGD_3DES_CBC    = 0x00002002,
    SGD_3DES_CFB    = 0x00002004,
    SGD_3DES_OFB    = 0x00002008,
    SGD_3DES_MAC    = 0x00002010,
    SGD_3DES_CTR    = 0x00002020,

    SGD_AES128_ECB  = 0x00004001,
    SGD_AES128_CBC  = 0x00004002,
    SGD_AES128_CFB  = 0x00004004,
    SGD_AES128_OFB  = 0x00004008,
    SGD_AES128_MAC  = 0x00004010,
    SGD_AES128_XTS	= 0x00004040,

    SGD_AES256_ECB  = 0x00008001,
    SGD_AES256_CBC  = 0x00008002,
    SGD_AES256_CFB  = 0x00008004,
    SGD_AES256_OFB  = 0x00008008,
    SGD_AES256_MAC  = 0x00008010,

}BlockAlgId;

typedef enum AsymAlgId_en {
    SGD_RSA         =   0x00010000,

    SGD_RSA_SIGN    =   0x00010010,
    SGD_RSA_ENC	    =   0x00010020,

    SGD_SM2         =   0x00020100,
    SGD_SM2_1       =   0x00020200,
    SGD_SM2_2       =   0x00020400,
    SGD_SM2_3       =   0x00020800,

    SGD_ECC_SIGN    =   0x00020001,
    SGD_ECC_ENC	    =   0x00020002,
    SGD_ECC			=   0x00080100,

}AsymAlgId;

typedef enum HashAlgId_en {
    SGD_SM3         =   0x00000001,
    SGD_SHA1        =   0x00000002,
    SGD_SHA256      =   0x00000004,

    SGD_SHA384      =   0x00000008,
    SGD_SHA512      =   0x00000010,
    SGD_SHA224      =   0x00000020,
    SGD_MD5	        =   0x00000040,

    SGD_HMAC_SM3    =   0x00100001,
    SGD_HMAC_SHA1   =   0x00100002,
    SGD_HMAC_SHA256 =   0x00100004,
    SGD_HMAC_SHA384 =   0x00100008,
    SGD_HMAC_SHA512 =   0x00100010,
    SGD_HMAC_SHA224 =   0x00100020,
    SGD_HMAC_MD5    =   0x00100040,
}HashAlgId;

typedef enum SignAlgId_en {
    SGD_SM3_RSA     =   0x00010001,
    SGD_SHA1_RSA    =   0x00010002,
    SGD_SHA256_RSA  =   0x00010004,
    SGD_SM3_SM2     =   0x00020201,

}SignAlgId;

#define SGD_SYM_ALG_MASK		0xFFFFFF00
#define SGD_SYM_MODE_MASK		0x000000FF

#define SGD_SYM_ALG_SM1			0x00000100
#define SGD_SYM_ALG_SM4			0x00000400
#define SGD_SYM_ALG_DES		    0x00001000
#define SGD_SYM_ALG_TDES		0x00002000
#define SGD_SYM_ALG_AES128		0x00004000
#define SGD_SYM_ALG_AES256		0x00008000

#define SGD_SYM_MODE_ECB		0x00000001
#define SGD_SYM_MODE_CBC		0x00000002
#define SGD_SYM_MODE_CFB		0x00000004
#define SGD_SYM_MODE_OFB		0x00000008
#define SGD_SYM_MODE_MAC		0x00000010
#define SGD_SYM_MODE_XTS		0x00000040

#if 1
#define SGD_SYM_MODE_CTR		0x00000020
#define SGD_SYM_MODE_GCM		0x00000080
#endif

#define SYM_ALG_SM1			    0x00000100
#define SYM_ALG_SSF33		    0x00000200
#define SYM_ALG_SM4			    0x00000400
#define SYM_ALG_DES 		    0x00001000
#define SYM_ALG_3DES 		    0x00002000
#define SYM_ALG_AES128		    0x00004000
#define SYM_ALG_AES256		    0x00008000

#define SYM_ALG_MODE_ECB		0x00000001
#define SYM_ALG_MODE_CBC		0x00000002
#define SYM_ALG_MODE_CFB		0x00000004
#define SYM_ALG_MODE_OFB		0x00000008
#define SYM_ALG_MODE_MAC		0x00000010
#define SYM_ALG_MODE_XTS		0x00000040

#endif//_SDF_ALGID_H_

