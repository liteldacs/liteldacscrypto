/**
 * 20240514 by wencheng
 * 根密钥预置完成后，密钥建立过程样例：AS GS SGW的密钥生成和分发
 */
#include <stdio.h>
#include <string.h>
#include "key_manage.h"
#include "database.h"
#include <uuid/uuid.h>

static field_desc km_fields[] = {
    // 密钥结构体字段描述
    {ft_uuid, 0, "id", NULL},
    {ft_enum, 0, "key_type", NULL},
    {ft_uint8t_pointer, 0, "owner1", NULL},
    {ft_uint8t_pointer, 0, "owner2", NULL},
    {ft_uint8t_pointer, 0, "key_cipher", NULL},
    {ft_uint32t, 0, "key_len", NULL},
    {ft_enum, 0, "key_state", NULL},
    {ft_timet, 128, "creatime", NULL},
    {ft_uint16t, 0, "updatecycle", NULL},
    {ft_uint32t, 0, "kek_len", NULL},
    {ft_uint8t_pointer, 0, "kek_cipher", NULL},
    {ft_uint8t_pointer, 0, "iv", NULL},
    {ft_uint16t, 0, "iv_len", NULL},
    {ft_enum, 0, "chck_algo", NULL},
    {ft_uint16t, 0, "check_len", NULL},
    {ft_uint8t_pointer, 0, "chck_value", NULL},
    {ft_uint16t, 0, "update_count", NULL},
    {ft_end, 0, NULL, NULL},
};

struct_desc static test_km_desc = {"km_pkg", km_fields};

int main()
{

    int ret;
    uint8_t *dbname = "keystore.db";
    uint8_t *as_tablename = "as_keystore";
    uint8_t *gs_tablename = "gs_s_keystore";
    uint8_t *sgw_tablename = "sgw_keystore";
    uint8_t *primary_key = "id";
    uint8_t *sac_sgw = "SGW"; // 测试本地标识
    uint8_t *sac_gs = "GS1";
    uint8_t *sac_as = "Berry";


    /**************************************************
     *                    SGW端                        *
     **************************************************/
    // 生成共享信息shareinfo
    uint32_t sharedinfo_len = 32;
    uint8_t sharedinfo[sharedinfo_len];
    ret = km_generate_random(sharedinfo, sharedinfo_len); // 用随机数填充
    if (ret != LD_KM_OK)
    {
        printf("[**sgw generate sharedinfo error**]\n");
        return 0;
    }

    // 获取根密钥
    CCARD_HANDLE handle_rootkey; // 根密钥句柄
    uint8_t *filepath = "/home/wencheng/crypto/key_management/keystore/rootkey.txt";
    ret = km_get_rootkey_handle(&handle_rootkey, filepath); // 获取根密钥
    if (ret != LD_KM_OK)
    {
        printf("[**sgw get_rootkey_handle error**]\n");
        return 0;
    }

    // 派生主密钥：密钥KAS-SGW=KDF(rootkey,sharedinfo)派生
    CCARD_HANDLE handle_kassgw; // AS和SGW之间的主密钥
    struct KeyPkg *pkg_kassgw;  // 用于保存主密钥信息
    uint16_t len_kassgw = 16;   // 主密钥长度
    pkg_kassgw = km_derive_key(handle_rootkey, MASTER_KEY_AS_SGW, len_kassgw, sac_sgw, sac_as, sharedinfo, sharedinfo_len, &handle_kassgw);
    if (pkg_kassgw == NULL)
    {
        printf("[**sgw derive_key kas-sgw error**]\n");
        return 0;
    }
    else
    {
        printf("[**sgw derive_key KAS-SGW OK**]\n");
        print_key_pkg(pkg_kassgw);
    }

    // 省略：构造AUC_RESP消息 用KAS-SGW计算消息校验值

   
    // 生成随机数N3,用于后续密钥派生
    uint16_t rand_len = 16;
    uint8_t rand[rand_len];
    ret = km_generate_random(rand, rand_len);
    if (ret != LD_KM_OK)
    {
        printf("[**AS generate_random N3 error**]\n");
        return 0;
    }

    // 省略：构造密钥确认消息 计算校验值

    // 主密钥KAS-GS派生
    uint16_t len_kasgs = 16;
    
    CCARD_HANDLE handle_kasgs;
    if (km_derive_masterkey_asgs(dbname, as_tablename, handle_kassgw, len_kasgs, sac_as, sac_gs, rand, rand_len, &handle_kasgs) != LD_KM_OK)
    {
        printf("[**AS derive master KAS-GS failed]\n");
        return 0;
    }
    printf("[**AS derive master KAS-GS OK]\n");
    
    // 会话密钥派生
    uint16_t len_session_key = 16;
    if (km_derive_all_session_key(dbname, as_tablename, handle_kasgs, len_session_key, sac_as, sac_gs, rand, rand_len) != LD_KM_OK)
    {
        printf("session key derive failed\n");
        return 0;
    }
    printf("[** AS derive session key OK]\n");

    

}