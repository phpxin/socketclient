/**
 * su_trim
 * 去除两边的指定字符
 * return 1/0 成功/失败
 */
int su_trim(char *str, char *cs);

/**
 * su_rtrim
 * 去除右边的指定字符
 * return 1/0 成功/失败
 */
int su_rtrim(char *str, char *cs);

/**
 * su_ltrim
 * 去除左边的指定字符
 * return 1/0 成功/失败
 */
int su_ltrim(char *str, char *cs);

/**
 * su_cfpos
 * 查找某字符首次出现的位置
 * return -1 未找到，0-n 为字符c在字符串中出现的位置
 */
int su_cfpos(char *arr, char c);
