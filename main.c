#include <stdio.h>
#include <string.h>
#include "key_manage.h"
#include <time.h>
#include <sdf/libsdf.h>

int main(void)
{

    /*************************************************************
     *                     密码学基础功能测试                     *
     *************************************************************/

    void *DeviceHandle, *pSessionHandle, *phKeyHandle;
    uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t iv_enc[16] = {0x02, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x20};
    uint8_t iv_dec[16] = {0x02, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x20};
    int key_len = 16;
    uint8_t data[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
                        0x29, 0xbe, 0xe1, 0xd6, 0x52, 0x49, 0xf1, 0xe9, 0xb3, 0xdb, 0x87, 0x3e, 0x24, 0x0d, 0x06, 0x47};
    unsigned int data_len = 32;
    unsigned int alg_id = SGD_SM4_CFB;
    uint8_t cipher_data[32];
    unsigned int cipher_data_len;
    uint8_t plain_data[32];
    unsigned int plain_data_len;
    int ret;

    // 打开设备
    SDF_OpenDevice(&DeviceHandle);
    SDF_OpenSession(DeviceHandle, &pSessionHandle);

    /************************************************************************
     *                      基础密钥管理功能+密码运算                          *
     ************************************************************************/
    /*
        // 导入密钥
        printf(" import_key test====================================================\n");
        ret = import_key(key, key_len, &phKeyHandle);
        if (ret != SDR_OK)
        {
            printf("import key error!return 08x% 08x(%s)\n", ret, strerror(ret));
            SDF_CloseSession(pSessionHandle);
            SDF_CloseDevice(DeviceHandle);
            return ret;
        }
        else
        {
            printf("import key success!the Handle is %p\n", phKeyHandle);
        }

        // mac测试
                    HANDLE keyhandle_for_mac;
                    uint8_t mac_value[16];
                    int mac_len;
                    uint8_t key_for_mac[16] = {0xCD, 0x46, 0x14, 0x55, 0x8F, 0xC0, 0xA7, 0x8E,
                                                     0x06, 0xF1, 0xD9, 0x6F, 0xC9, 0x12, 0xC7, 0xAA};

                    uint8_t iv_for_mac[16] = {0xE8, 0xE1, 0xA7, 0x51, 0x85, 0x85, 0x44, 0x4C,
                                                    0xC5, 0xD4, 0xA8, 0xD5, 0xE9, 0x94, 0x5B, 0xB0};
                    uint8_t mac_raw_data[32] = {0xB9, 0x7F, 0x6C, 0x7A, 0x96, 0x30, 0x92, 0xE8,
                                                      0x87, 0xEA, 0x3E, 0x46, 0xEE, 0x3C, 0x23, 0x5E,
                                                      0x47, 0xEA, 0xC7, 0x32, 0xD8, 0xF6, 0xB4, 0x24,
                                                      0x1A, 0xA3, 0x5E, 0xF6, 0xE5, 0xE8, 0x53, 0x18};

                    uint8_t sm4mac[16] = {0xBB, 0xC0, 0xEC, 0x13, 0x1F, 0x10, 0xBE, 0xB0,
                                                0x9E, 0x71, 0x25, 0x76, 0x2A, 0x1D, 0xA2, 0x68};

                    ret = SDF_ImportKey(pSessionHandle, key_for_mac, 16, &keyhandle_for_mac);
                    mac(keyhandle_for_mac, SGD_SM4_MAC, iv_for_mac, mac_raw_data, 32, mac_value, &mac_len);

                    if (memcmp(mac_value, sm4mac, mac_len) != 0)
                    {
                        printf("SM4 Mac Compare with std data failed!\n");
                        printbuff("SM4std mac_value", sm4mac, 16);
                    }
                    else
                    {
                        printf("SM4 Mac Compare with std data succeed!\n");
                        printbuff("mac_value", mac_value, mac_len);
                    }

                    // SM3 HASH 标准待哈希数据样例 32byte = 256bit输入
                    uint8_t bhashed_data[64] = {0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
                                                      0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
                                                      0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
                                                      0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64};
                    // SM3 HASH 标准明文数据的哈希值
                    uint8_t bHashStdResult[32] = {0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d,
                                                        0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32};
                    uint8_t hash_value[32];
                    hash(SGD_SM3, bhashed_data, 64, hash_value);
                    printbuff("raw data", bhashed_data, 64);
                    printbuff("hash_value", hash_value, 32);

                    // 加密
                    printf("encrypt test====================================================");
                    int encrypt_result = encrypt_with_domestic_algo(phKeyHandle, alg_id, iv_enc, data, data_len, cipher_data, &cipher_data_len);
                    if (encrypt_result == 0)
                    {
                        // 加密成功，cipher_data 中存放着加密后的数据
                        // cipher_data_len 中存放着加密后数据的长度
                        printbuff("raw data", data, data_len);
                        printbuff("iv_enc", iv_enc, 16);
                        printbuff("key", key, key_len);
                        printf("key handle %p\n", phKeyHandle);
                        printbuff("encrypted data", cipher_data, cipher_data_len);
                        printf("Encryption successful!=========================\n");
                    }
                    else
                    {
                        // 加密失败，根据错误代码进行处理
                        printf("Encryption failed with error code: %d\n", encrypt_result);
                    }

                    // 解密
                    printf("decrypt test====================================================");
                    int decrypt_result = decrypt_with_domestic_algo(phKeyHandle, alg_id, iv_dec, cipher_data, cipher_data_len, plain_data, &plain_data_len);
                    printbuff("key", key, key_len);
                    printbuff("iv_dec", iv_dec, 16);
                    printf("key handle %p\n", phKeyHandle);
                    printf("plain data len:%d \n", plain_data_len);
                    printbuff("decrypted data", plain_data, plain_data_len);
                    if (decrypt_result == 0 && memcmp(data, plain_data, data_len) == 0) // 比对原始数据和解密结果
                    {
                        printf("Decryption successful!===========================\n");
                    }
                    else
                    {
                        // 解密失败，根据错误代码进行处理
                        printf("Decryption failed with error code: %d\n", decrypt_result);
                    }

                    // 随机数生成
                    uint8_t *random_data;
                    generate_random(16, random_data);
                    printbuff("random value", random_data, 16);
                    printf(" end test\n");

                    // hmac 测试
                    printf("hmac test====================================================");
                    uint8_t hmac_data[32]; // 分配足够的空间来存储 HMAC 数据
                    uint32_t hmac_data_len;
                    ret = sm3_hmac(key, key_len, data, data_len, hmac_data, &hmac_data_len);
                    if (ret == 0)
                    {
                        printbuff("key", key, key_len);
                        printbuff("data", data, data_len);
                        printbuff("hmac", hmac_data, hmac_data_len);
                    }
                    else
                    {
                        printf("Error in sm3_hmac: %d\n", ret);
                    }

                    // 密钥销毁 关闭会话
                    ret = SDF_DestroyKey(pSessionHandle, phKeyHandle); // 销毁密钥
                    if (ret != SDR_OK)
                    {
                        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x\n", ret);
                    }
                    SDF_CloseSession(pSessionHandle);
                    SDF_CloseDevice(DeviceHandle);
                */

    /*************************************************************
     *                          KEK 预置                          *
     *************************************************************/
    /*
    // 生成并导出KEK
    printf("generate_and_export_kek test=========================================\n");
    struct KEKPkg *kek_pkg = malloc(sizeof(struct KEKPkg));
    int kek_len = 256;
    int kek_index = 2;
    uint8_t *rcver = "AS1";
    ret = generate_and_export_kek(kek_len, kek_index, rcver, strlen(rcver), kek_pkg);
    if (ret != LD_OK)
    {
        printf("fail to generate_and_export_kek\n");
    }
    else
    {
        printf("succeed to generate_and_export_kek, stored in index %d\n", kek_index);
        print_kek_pkg(kek_pkg);
    }
    // 将结构体写入文件
    write_kekpkg_to_file("kekpkg.txt", kek_pkg);
    free(kek_pkg);

    // 导入并验证KEK
    printf("external_import_kek test====================================================\n");
    // 从文件中读取结构体
    struct KEKPkg *pkg_from_file = malloc(sizeof(struct KEKPkg));
    read_kdkpkg_from_file("kekpkg.txt", pkg_from_file);
    int kek_import_index = 1;
    uint8_t *local_id = "AS1";
    ret = external_import_kek(pkg_from_file, kek_import_index, local_id, strlen(local_id));
    if (ret != LD_OK)
    {
        printf("fail to import_kek\n");
    }
    else
    {
        printf("succeed to import kek\n");
    }
    // 释放分配的内存
    free(pkg_from_file);
*/
    /*************************************************************
     *                           业务密钥管理                      *
     ************************************************************/
    /*




        // pbkdf2 接口测试
        printf("pbkdf2 test====================================================\n");
        uint8_t *salt = "salt";
        uint32_t salt_len = strlen(salt);
        uint32_t derived_key_len = 32; // 需求中的派生密钥长度
        uint8_t derived_key[derived_key_len];
        ret = pbkdf2(key, key_len, salt, salt_len, 1024, derived_key_len, derived_key);
        if (ret == 0)
        {
            printbuff("pbkdf2 Derived Key", derived_key, derived_key_len);
        }

    // derive_key_assgw 接口测试
    printf("derive_key_assgw test====================================================\n");
    uint32_t key_asswg_len = 16;
    uint8_t *id_as = "AS1";
    uint32_t id_as_len = strlen(id_as);
    uint8_t *id_sgw = "SGW1";
    uint32_t id_sgw_len = strlen(id_sgw);
    uint8_t *shared_info = "shared info";
    uint8_t shared_info_len = strlen(shared_info);
    void *key_assgw_handle = (void *)malloc(sizeof(void *));
    struct KeyMetaData *key_info = malloc(sizeof(struct KeyMetaData));
    int result = derive_key_assgw(phKeyHandle, key_asswg_len, id_as, id_as_len, id_sgw, id_sgw_len,
                                  shared_info, shared_info_len, key_assgw_handle, key_info);
    if (result == 0)
    {
        printf("Key as-sgw syccessfully derived!the Handle is %p\n", key_assgw_handle);
        printf(" metadata is as followed:\n");
        print_key_metadata(key_info);
        // encrypt_with_domestic_algo(key_assgw_handle, alg_id, iv_enc, data, data_len, cipher_data, &cipher_data_len);
        // printbuff("cipher data:", cipher_data, cipher_data_len);
    }
    else
    {
        printf("Key derivation failed.\n");
    }

    // generate_key_with_kek接口测试
    printf(" generate_key_with_kek test================================================\n");
    time_t currentTime;
    int kek_index = 1;
    void *key_handle;
    uint8_t cipher_key[32];
    int cipher_len;
    struct KeyMetaData key_info = {
        .id = 1,
        .owner_1 = "AS1",
        .owner_2 = "GS1",
        .type = ROOT_KEY,
        .length = 16, // byte
        .state = PRE_ACTIVATION,
        .effectuation_time = time(&currentTime),
        .update_cycle = 365, // Replace with the actual update cycle
    };
    print_key_metadata(&key_info);

    generate_kek(128, kek_index); // 生成kek
    int result = generate_key_with_kek(kek_index, &key_info, &key_handle, cipher_key, &cipher_len);
    if (result == 0)
    {
        print_key_metadata(&key_info);
        printf("Generate Key with KEK succeed!the Handle is %p\n", key_handle);
        printf("cipher_len %d\n", cipher_len);
        printbuff("cipher key", cipher_key, cipher_len);
    }
    else
    {
        printf("Key generation failed.\n");
    }

    // 生成标准封装格式kek_pkg测试 生成后加上校验和密钥的密文 用于密钥存储和分发
    printf(" generate_key_pkg test====================================================\n");
    struct KeyPkg key_pkg;
    uint8_t iv_hmac[16] = {0x02, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x20};
    int result_generate_key_pkg = generate_key_pkg(&key_info, cipher_key, iv_hmac, key_handle, &key_pkg);
    if (result_generate_key_pkg != LD_OK)
    {
        printf("fail to generate_key_pkg.\n");
    }
    else
    {
        printf("succeed to generate_key_pkg.\n");
        print_key_pkg(&key_pkg);
    }

    write_keypkg_to_file("keystore.txt", &key_pkg);
 */
    /*************************************************************
     *                           根密钥预置                      *
     ************************************************************/
    // USB密码卡:生成+导出加密和签名密钥-调用密码卡工具
    /*
    // 导出SM2签名公钥 SM2加密公钥
    int ecc_key_index = 1;
    ECCrefPublicKey *pk_sign = malloc(sizeof(ECCrefPublicKey));
    ret = SDF_ExportSignPublicKey_ECC(pSessionHandle, 1, pk_sign);
    if (ret != LD_OK)
    {
        printf("SDF_ExportSignPublicKey_ECC error!return 0x%08x\n", ret);
    }
    ECCrefPublicKey *pk_enc = malloc(sizeof(ECCrefPublicKey));
    SDF_ExportEncPublicKey_ECC(pSessionHandle, 1, pk_enc);
    if (ret != LD_OK)
    {
        printf("SDF_ExportEncPublicKey_ECC error!return 0x%08x\n", ret);
    }

    // SGW:使用外部密钥生成根密钥 并导出根密钥 存储到到文件区
    int rootkey_len = 256; // bit
    ECCCipher *rootkey = malloc(sizeof(ECCCipher));
    void *rootkey_handle_in_sgw;
    ret = SDF_GenerateKeyWithEPK_ECC(pSessionHandle, rootkey_len, SGD_SM2, pk_enc, rootkey, &rootkey_handle_in_sgw);
    if (ret != LD_OK)
    {
        printf("SDF_GenerateKeyWithEPK_ECC error,return 0x%08x\n", ret);
    }
    else
    {
        printf("SDF_GenerateKeyWithEPK_ECC OK, the Handle is %p\n", rootkey_handle_in_sgw);
    }

    // USB密码卡:  分发密码卡给AS AS插上密码卡

    //  AS获取私钥使用权限
    uint8_t *pin = "11111111";
    unsigned int pin_len = strlen(pin);
    ret = SDF_GetPrivateKeyAccessRight(pSessionHandle, 2, pin, pin_len);
    if (ret != LD_OK)
    {
        printf("SDF_GetPrivateKeyAccessRight error,return 0x%08x\n", ret);
    }
    else
    {
        printf(" SDF_GetPrivateKeyAccessRight OK\n");
    }
    // USB密码卡-导入根密钥
    // 读取文件

    // 使用内部私钥解密根密钥
    ECCCipher *rootkey_1 = malloc(sizeof(ECCCipher));
    void *rootkey_handle_in_as = malloc(sizeof(void));
    printf("the rootkey_handle_in_as is %p\n", rootkey_handle_in_as);

    int prikey_index = 1;
    printf("The encrypt key is :\n");
    printbuff("data.x", rootkey->x, ECCref_MAX_LEN);
    printbuff("data.y", rootkey->y, ECCref_MAX_LEN);
    printbuff("data.M", rootkey->M, 32);
    printbuff("data.c", rootkey->C, rootkey->L);
    ret = SDF_ImportKeyWithISK_ECC(pSessionHandle, prikey_index, rootkey, &rootkey_handle_in_as);
    printf("the rootkey_handle_in_as is %p\n", rootkey_handle_in_as);
    if (ret != LD_OK)
    {
        printf("SDF_ImportKeyWithISK_ECC error,return 0x%08x\n", ret);
    }
    else
    {
        printf("the rootkey_handle_in_as is %p\n", rootkey_handle_in_as);
    }
*/
    return 0;
}