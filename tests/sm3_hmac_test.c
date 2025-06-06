/**
 * @author wencheng
 * @version 2024/08/28
 * @brief HMAC computation and database test
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "key_manage.h"
#include "kmdb.h"

int main() {
    // 数据库和表名定义
    const char *db_name = "keystore.db";
    const char *sgw_tablename = "sgw_keystore";
    // const char* as_tablename = "as_keystore";
    uint8_t *sac_sgw = "000010000"; // 测试本地标识
    // uint8_t *sac_gs_s = "GS1";
    // uint8_t *sac_gs_t = "GSt";
    uint8_t *sac_as = "000010010";

    // 要计算 HMAC 的数据
    uint8_t data[] = "Hello SM3 HMAC!";
    uint32_t data_len = sizeof(data) - 1; // 不包括结尾的 '\0'

    // HMAC 输出值
    uint8_t hmac_value[32]; // 假设 SM3 的 HMAC 长度为 32 字节
    uint32_t hmac_len = sizeof(hmac_value);

    // 查询根密钥id
    QueryResult_for_queryid *qr_mk = query_id(db_name, sgw_tablename, sac_as, sac_sgw, ROOT_KEY, ACTIVE);
    do {
        if (qr_mk->count != 1) {
            fprintf(stderr, "Query rkid failed.\n");
            break;
        }

        // 调用 HMAC 接口
        l_km_err hmac_err = km_sm3_hmac(db_name, sgw_tablename, qr_mk->ids[0], data, data_len, hmac_value, &hmac_len);
        if (hmac_err != LD_KM_OK) {
            fprintf(stderr, "HMAC computation failed with error code: %d\n", hmac_err);
            break;
        }

        // 打印 HMAC 值
        //log_buf(LOG_INFO, "hmac value", hmac_value, 32);

    } while (0);

    free_queryid_result(qr_mk);

    return 0;
}