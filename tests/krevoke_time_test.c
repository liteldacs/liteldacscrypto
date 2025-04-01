/**
 * @brief 密钥撤销时间测试
 * @date 2025/1/13
 */
#include <stdio.h>
#include <string.h>
#include "key_manage.h"
#include "kmdb.h"

int main()
{
    uint8_t *dbname = "keystore.db";
    uint8_t *as_tablename = "as_keystore";
    uint8_t *gs_s_tablename = "gs_s_keystore";
    uint8_t *gs_t_tablename = "gs_t_keystore";
    uint8_t *sgw_tablename = "sgw_keystore";

    uint8_t *sac_gs_s = "GS1";
    uint8_t *sac_gs_t = "GSt";
    uint8_t *sac_sgw = "000010000"; // 测试本地标识
    uint8_t *sac_as = "000010010";

    enum KEY_TYPE type = MASTER_KEY_AS_GS;
    QueryResult_for_queryid *qr = query_id(dbname, sgw_tablename, sac_as, sac_gs_s, type, ACTIVE);
    int count = 1024;
    clock_t start, end;
    double cpu_time_used;
    do
    {
        if (qr == NULL)
        {
            // fprintf(stderr, "Query failed.\n");
            break;
        }

        if (km_revoke_key(dbname, sgw_tablename, qr->ids[0]) != LD_KM_OK)
        {
            // fprintf(stderr, "key %s revoke failed\n", qr->ids[0]);
            break;
        }
    } while (0);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("revoke key Elapsed time: %f seconds\n", cpu_time_used);
    fprintf(stderr, "revoke %s %s and it's subkey OK\n", ktype_str(type), qr->ids[0]);

    free_queryid_result(qr);
}