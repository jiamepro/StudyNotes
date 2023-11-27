# 设备驱动学习——1.hello_world模块安装
## 编写hello_world模块和Makefile
hello_world模块(hello_module.c)
```
# include <linux/init.h>
# include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "Hello world\n");
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);

```
Makefile
```
obj-m := hello_module.o

KDIR := /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
```
* `M=$(shell pwd)`等号两边不加空格，否则出错
    `fatal error: linux/init.h: No such file or directory`
* 在Makefile中 -C 加内核指定目录
* 运行make之前要完成的工作：
  * 构建设备树，不然装载不了模块（没做这步）：
    `insmod: ERROR: could not insert module ./hello_module.ko: Invalid module format`
  * 安装库flex，bison

## 运行make后，装载模块出错
**运行make**
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ make
make -C /usr/src/linux-headers-6.2.0-37-generic M=/home/xiangjiajia/code modules
make[1]: 进入目录“/usr/src/linux-headers-6.2.0-37-generic”
make[1]: 离开目录“/usr/src/linux-headers-6.2.0-37-generic”
```
安装模块
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ insmod hello_module.ko
insmod: ERROR: could not insert module hello_module.ko: Operation not permitted
```
装载驱动要以管理员权限
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ sudo insmod hello_module.ko
[sudo] xiangjiajia 的密码： 
insmod: ERROR: could not insert module hello_module.ko: Invalid module format
```
检查：使用dmesg
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ sudo dmesg|grep hello
[17051.726477] hello_module: loading out-of-tree module taints kernel.
[17051.726525] hello_module: module verification failed: signature and/or required key missing - tainting kernel
```
问题1：`loading out-of-tree module taints kernel.`加载了树外驱动，也就是自己写的驱动，被检查出来了，没有危害，可以忽略
解决方法：[modulename: loading out-of-tree module taints kernel](https://blog.csdn.net/gzxb1995/article/details/105407014)
问题2：`module verification failed: signature and/or required key missing - tainting kernel`自 3.7 内核之后有了内核签名机制,需要对加载的模块签名
解决方法：[内核签名](https://blog.csdn.net/faxiang1230/article/details/104256011)

方法1：Makefile文件中加入CONFIG_MODULE_SIG=n (没用)
方法2：内核签名
1. 在`/usr/src/linux-headers-6.2.0-37-generic/` 中打开`.config`文件更改设置,先把文件权限改为可写
```
CONFIG_MODULE_SIG=y
CONFIG_MODULE_SIG_FORCE=y
CONFIG_MODULE_SIG_ALL=y
```
1. 查看是否修改成功
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:/usr/src/linux-headers-6.2.0-37-generic$ cat .config|grep CONFIG_MODULE_SIG
CONFIG_MODULE_SIG_FORMAT=y
CONFIG_MODULE_SIG=y
CONFIG_MODULE_SIG_FORCE=y
CONFIG_MODULE_SIG_ALL=y
......
```
1. 对hello_module.ko 进行签名出现错误
[准备签名文件](https://blog.csdn.net/u011050845/article/details/125699628)和[内核签名](https://blog.csdn.net/u011050845/article/details/125699924)
错误：`-bash: /usr/src/linux-headers-6.2.0-37-generic/sign-file: 没有那个文件或目录`
解决：sign-file前少打了一个script
    1. 建立`x509.genkey`文件
    ```
    [ req ]
    default_bits = 4096
    distinguished_name = req_distinguished_name
    prompt = no
    string_mask = utf8only
    x509_extensions = myexts

    [ req_distinguished_name ]
    CN = Modules

    [ myexts ]
    basicConstraints=critical,CA:FALSE
    keyUsage=digitalSignature
    subjectKeyIdentifier=hash
    authorityKeyIdentifier=keyid
    ```
    2. `openssl req -new -nodes -utf8 -sha512 -days 36500 -batch -x509 -config x509.genkey -outform DER -out signing_key.x509 -keyout signing_key.pem`
    3. `sudo cp signing_key.pem signing_key.x509 /usr/src/linux-headers-6.2.0-37-generic/certs/`
    4. `**sudo** /usr/src/linux-headers-6.2.0-37-generic/scripts/sign-file sha512 /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.pem /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.x509 hello_module.ko
`
5. 出现错误
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ /usr/src/linux-headers-6.2.0-37-generic/scripts/sign-file sha512 /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.pem /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.x509 hello_module.ko
At main.c:298:
- SSL error:FFFFFFFF80000002:system library::No such file or directory: ../crypto/bio/bss_file.c:67
- SSL error:10000080:BIO routines::no such file: ../crypto/bio/bss_file.c:75
sign-file: hello_module.ko
```
 hello_module.ko 还没生成
 6. make之后又出现错误
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ /usr/src/linux-headers-6.2.0-37-generic/scripts/sign-file sha512 /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.pem /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.x509 hello_module.ko
At main.c:167:
- SSL error:FFFFFFFF8000000D:system library::Permission denied: ../crypto/bio/bss_file.c:67
- SSL error:10080002:BIO routines::system lib: ../crypto/bio/bss_file.c:77
sign-file: /usr/src/linux-headers-6.2.0-37-generic/certs/signing_key.pem
```
没有用管理员权限
7. 启用管理员权限之后，没错
8. 还是不能安装模块！！！！
```
xiangjiajia@xiangjiajia-Lenovo-XiaoXin-Chao7000-14IKBR:~/code$ sudo insmod ./hello_module.ko
insmod: ERROR: could not insert module ./hello_module.ko: Invalid module format
```
> 2023年11月26日22:59:22：解决不了
##