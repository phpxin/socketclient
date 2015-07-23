/**
 * 主机字节序大小端判断
 * @return 0/1/2 未知/大端/小端
 */
int bl_or_ll();

/**
 * int转换成网络字节序
 */
int int_to_net(int value);

/**
 * 网络字节序转换int
 */
int net_to_int(int value);

/**
 * int大小端相互转换
 */
int int_bl_endian(int x);
