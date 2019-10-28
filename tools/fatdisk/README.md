# FAT文件系统镜像打包工具

# 文件说明

## fatdisk.xml

配置文件，默认不需要修改。

## fatdisk.exe

镜像打包工具


# 使用说明

1. 新建一个目录，如 flash0。

2. 把需要打包的文件放到 flash0 目录下。

3. 然后运行 fatdisk.exe 即可生成 `flash0.bin`。

留意最后生成的 flash0.bin 体积不要超过`679KB`。
因为BK7251默认download分区的大小是680KB，再加上RBL文件头96字节，所以需要减少1个删区。

生成镜像后，可以使用 rt_ota_packaging_tool， 把 `flash0.bin`制作成 `flash0.rbl`。
然后通过OTA下载到download分区即可。
