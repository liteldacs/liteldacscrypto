/**
 * created by wencheng on 2024-5-20
 * struct convert to bitstream , for store in database
 * 密钥存储、查询、修改
 */

#include "key_manage.h"
#include "km_field.h"

#define MAX_ID_LEN 128

typedef struct struct_desc_s {
    const char *name;
    struct km_field_desc *fields;
} struct_desc;

/*
************************************************************************
*                              新建                                *
************************************************************************
*/
/****************创建表*****************/
/**
 * @brief 外部接口：基于描述创建密钥表
 * @param[in] sd 理想的数据表描述
 * @param[in] db_name 数据库名
 * @param[in] table_name 表名
 * @param[in] primary_key 主键
 * @param[in] foreign_table 外键表 可以为空
 * @param[in] foreign_key 外键 可以为空
 * @param[in] is_auto_increase 是否自增
 * @return 处理结果:成功/失败
 */
l_km_err create_table_if_not_exist(
        struct_desc *sd,
        const char *db_name,
        const char *table_name,
        uint8_t *primary_key,
        const char *foreign_table,
        uint8_t *foreign_key,
        bool is_auto_increase);

/*************存储密钥*****************/

/**
 * @bref 存储密钥：按照描述编码和组装结构体 插入指定数据库表
 * @param[in] db_name 数据库名
 * @param[in] table_name 表名
 * @param[in] pkg 密钥结构体
 * @param[in] sd 结构体描述
 * @return 是否执行成功
 */
l_km_err store_key(
        const char *db_name,
        const char *table_name,
        struct KeyPkg *pkg,
        struct_desc *sd);

/**
 * @bref 存储随机数：为指定密钥存储用于密钥派生的随机数
 * @param[in] db_name 数据库名
 * @param[in] table_name 表名
 * @param[in] id 密钥id
 * @param[in] rand_len 随机数长度
 * @param[in] rand 随机数
 * @return 是否执行成功
 */
l_km_err store_rand(
        const char *db_name,
        const char *table_name,
        uint8_t *id,
        uint32_t rand_len,
        uint8_t *rand);

/*
************************************************************************
*                              查询                                     *
************************************************************************
*/
/***************查询id*****************/

typedef struct {
    uint8_t *ids[MAX_ID_LEN];
    uint32_t count;
} QueryResult_for_queryid;

/**
 * @brief 外部接口:基于所有者和密钥类型查询密钥
 *
 * 此函数用于根据提供的数据库名、表名、所有者以及密钥类型来查询密钥。
 *
 *@param[in] db_name指定要查询的数据库。
 *@param[in] table_name指定要查询的表。
 *@param[in] owner1 所有者
 *@param[in] owner2 所有者
 *@param[in] key_type 密钥类型
 *@param[in] state 密钥状态
 *
 * @return 返回一个指向QueryResult_for_queryid结构的指针，该结构包含查询结果。
 *         如果未查询到结果，则相关字段将被设置为NULL。
 */

QueryResult_for_queryid *query_id(
        const char *db_name,
        const char *table_name,
        const char *owner1,
        const char *owner2,
        enum KEY_TYPE key_type,
        enum STATE state);

void free_queryid_result(
        QueryResult_for_queryid *result);

/***************查询密钥明文****************/

// 结构体用于存储查询密钥
typedef struct {
    int key_len;  // 密钥长度
    uint8_t *key; // 密钥值（明文）
} QueryResult_for_keyvalue;

/**
 * @bref 外部接口：基于id查询密钥值
 * @bref 外部接口：通过密钥id查询密钥值
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥编号
 * @return key:密钥值，NULL :未查询到结果
 *
 */
QueryResult_for_keyvalue *query_keyvalue(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

/* 基于所有者查询密钥值 */
/**
 * @bref 外部接口：基于所有者查询密钥值
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] owner1 所有者
 * @param[in] owner2 所有者
 * @param[in] key_type 密钥类型
 * @param[in] state 密钥状态
 * @return key:密钥值，NULL :未查询到结果
 *
 */
QueryResult_for_keyvalue *query_keyvalue_by_owner(
        uint8_t *db_name,
        uint8_t *table_name,
        const char *owner1,
        const char *owner2,
        enum KEY_TYPE key_type,
        enum STATE state);

/**
 * @brief 释放 QueryResult_for_keyvalue 结构体所占用的内存
 * @param[in] result 指向 QueryResult_for_keyvalue 结构体的指针
 */
void free_keyvalue_result(
        QueryResult_for_keyvalue *result);

// 结构体用于查询随机数
typedef struct {
    int rand_len;  // 密钥长度
    uint8_t *rand; // 密钥值（明文）
} QueryResult_for_rand;

/**
 * @bref 外部接口：查询随机数
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] key_id 密钥编号
 * @return 随机数及长度
 *
 */
QueryResult_for_rand *query_rand(
        const char *db_name,
        const char *table_name,
        const char *key_id);

/**
 * @brief 释放 QueryResult_for_rand 结构体所占用的内存
 * @param[in] result 指向 QueryResult_for_rand 结构体的指针
 */
void free_rand_result(
        QueryResult_for_rand *result);

/******************** 查询kek**************************/

typedef struct {
    void *kek_handle;
    uint16_t iv_len;
    uint8_t *iv;
} QueryResult_for_kekhandle;

/**
 * @brief 获取kek密钥句柄
 * @param[in] db_name
 * @param[in] table_name
 * @param[in] id 密钥id
 * @return 查询结果
 */
QueryResult_for_kekhandle *query_kekhandle(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

/**
 * @brief 释放 QueryResult_for_kekhandle 结构体所占用的内存
 * @param[in] result 指向 QueryResult_for_kekhandle 结构体的指针
 */
void free_kekhandle_result(
        QueryResult_for_kekhandle *result);

/*************查询更新相关*****************/

// 查询结果结构体
typedef struct {
    uint16_t key_len;
    uint16_t update_cycle;
    uint16_t update_count;
} QueryResult_for_update;

QueryResult_for_update *query_for_update(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

void free_update_result(
        QueryResult_for_update *result);

// 优化：查询密钥长度
/**
 * @bref 查询密钥长度
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥编号
 * @return 密钥长度
 *
 */
uint32_t query_keylen(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

/**************查询所有者*****************/

// 结构体用于存储密钥所有者
typedef struct {
    uint8_t *owner1;
    uint8_t *owner2;
} QueryResult_for_owner;

/**
 * @bref 查询密钥拥有者
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥编号
 * @return 密钥所有者
 *
 */
QueryResult_for_owner *query_owner(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

void free_owner_result(
        QueryResult_for_owner *result);

/************查询子密钥*****************/

// 结果集结构体定义
typedef struct {
    int count;            // 结果数量
    uint8_t **subkey_ids; // 子密钥id数组
    uint8_t **key_types;  // key_type数组
} QueryResult_for_subkey;

/**
 * @bref 查询子密钥id
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥id
 * @return 子密钥id
 */
QueryResult_for_subkey *query_subkey(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

void free_query_result_for_subkey(
        QueryResult_for_subkey *result);

/**************查询密钥类型*****************/

/**
 * @brief 查询密钥类型
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥id
 * @return 密钥类型
 */
enum KEY_TYPE query_keytype(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

/**
 * @brief 查询密钥状态
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id
 * @return 密钥状态
 */
enum STATE query_state(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id);

/*
************************************************************************
*                              修改                                    *
************************************************************************
*/

/**
 * @brief 修改密钥状态
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥id
 * @param[in] state 想要改成的密钥状态
 */
l_km_err alter_keystate(
        const char *db_name,
        const char *table_name,
        uint8_t *id,
        enum STATE state);

/**
 * @brief 启用密钥
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥id
 * @return 成功：LD_KM_OK/失败：错误码
 */
l_km_err enable_key(
        const char *db_name,
        const char *table_name,
        const char *id);

/**
 * @brief 更新计数增加一次
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id
 * @param[in] dbname
 * @return 成功与否
 */
l_km_err increase_updatecount(
        uint8_t *dbname,
        uint8_t *tablename,
        uint8_t *id);

/**
 * @brief 修改密钥值
 * @param[in] dbname
 * @param[in] tablename
 * @param[in] id 密钥id
 * @param[in] key_len 密钥长度
 * @param[in] 密钥值
 * @return 修改成功与否
 */
l_km_err alter_keyvalue(
        uint8_t *db_name,
        uint8_t *table_name,
        uint8_t *id,
        uint16_t key_len,
        uint8_t *key);
