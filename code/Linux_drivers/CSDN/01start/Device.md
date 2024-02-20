# 驱动开发教程
@[toc]

第一次构建内核真的困难重重，终于成功了，将步骤公开，希望小伙伴们少走弯路, Uboot和Linux内核移植的特殊地方就在于需要根据你的板卡选择相应的默认配置信息/boot/configs
## 环境配置
工欲善其事必先利其器，搭建一个良好的开发环境是进行开发的基础，可以少出很多bug
### Ubuntu虚拟机
* 我们需要准备一个Linux系统的环境，可以用虚拟机搭建，网上有很多文章讲这个，就不赘述了
* 正点原子提供了开源的虚拟机和系统镜像的资料和搭建教程，完全开源的，请自取：[正点原子资料](http://www.openedv.com/docs/boards/arm-linux/zdyz-i.mx6ull.html)
> 博主在实践过程中发现用虚拟机会出现挺多问题，比如虚拟机网络问题导致不能安装一些库、虚拟机导致电脑蓝屏、虚拟机卡慢等等
* 我使用的是自己笔记本装的Linux双系统，读者如果要安装双系统，可以查找自己电脑型号对应的双系统安装教程，也比较繁杂，但是之后就是在真正的Linux里搞开发，不存在虚拟机里莫名其妙的问题。
* 博主的平台信息是：
```
Ubuntu版本：22.04
gcc版本： 11.4.0
主机linux内核版本： 6.5.0-15-generic
所构建的Linux内核版本：6.6.16
```

### Qemu
* 系统搭建好了之后安装qemu：
```
sudo apt-get install qemu
sudo apt-get install qemu-system
```

### 编译内核
首先编译内核，我们构建内核需要编译后产生的内核镜像和设备树文件
* 内核下载地址，官方网站可能下载比较慢，下载6.6.16版本：
[内核下载官方网站](https://mirrors.edge.kernel.org/pub/linux/kernel/)
[内核下载镜像网站](https://cdn.kernel.org/pub/linux/kernel/)
* 下载后解压，进入cd文件夹
* 编译内核
```
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
make vexpress_defconfig
make menuconfig
```
弹出的界面不设置，按两次esc退出,开始编译内核，时间有一点久

> export用于显示或设置环境变量，但是效果仅限于当前登录的终端，这样我们不用每次make都指定参数了
> vexpress_defconfig是ARM Versatile Express开发板的内核默认配置文件，文件夹里有很多厂家板子的默认配置文件，放在arch/arm/configs



我们编译内核镜像文件：make bzImage
> 编译结果注意一下zImage的地址

编译设备树文件：make dtbs
> 同样，编译结果会显示设备树的地址

我们可以看到，Linux内核镜像文件保存在arch/arm/boot/zImage，设备树文件：vexpress-v2p-ca9.dtb(在arch/arm/boot/dts/文件夹下)

运行qemu
```
qemu-system-arm -M vexpress-a9 -smp 4 -m 200M -kernel arch/arm/boot/zImage -append "rdinit=/linuxrc console=ttyAMA0 loglevel=8" -dtb arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic
```

编译好了之后在qemu运行,提示信息开头，说明成功进入内核
![Alt text](image-2.png)

因为我们还没指定根文件系统（用busybox制作），所以进入内核之后，尾行提示内核崩溃
![Alt text](image-3.png)

退出qemu：在另一个终端输入killall qemu-system-arm或者在qemu中输入ctrl+a再输入x

### 构建根文件系统（busybox）
* 博主下载的版本是1.36.1，下载网站[busybox下载](https://busybox.net/)
* 解压进入busybox，对busybox进行配置，使用默认配置
```
make defconfig
make menuconfig
```
跳出的界面里配置静态编译：Setting->Build static binary，点空格把它选上，然后Esc退出
![Alt text](image-4.png)

编译根文件系统：
```
mkdir /home/rootfs
make
make install CONFIG_PREFIX=/home/rootfs 
```
设置CONFIG_PREFIX参数，将编译结果指定存放在根文件系统目录下

* 创建etc,dev,mnt,sys,tmp,proc,root等目录
* 创建如下设备节点
```
cd dev/
sudo mknod console c 5 1
sudo mknod null c 1 3
```

然后把这个根文件系统打包成镜像:
> 参考[（2023）从零开始用qemu搭建虚拟arm环境](https://blog.csdn.net/amateurSU/article/details/134031150)
qemu-img create -f raw disk.img 256M

写一个/etc/init.d/rcS文件,注意给权限,内容如下(内容存疑)
```
#！/bin/sh
mount -a
mkdir /dev/pts
mount -t devpts devpts /dev/pts
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
```

还要创建一个/etc/fstab文件，这个文件在开机之后设置自动挂载哪些分区
```
proc            /proc           proc      defaults       0       0
tmpfs           /tmp            tmpfs     defaults       0       0
sysfs           /sys            sysfs     defaults       0       0

```

还要创建一个/etc/inittab文件
（这三个文件的解释见原子文档的p1028页）
```
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
console::askfirst:-/bin/sh
::restart:/sbin/init 
::ctrlaltdel:/sbin/reboot
::shutdown:/bin/umount -a -r
```

> 这三个文件参考[WSL2下Ubuntu22.04使用Qemu搭建虚拟Vexpress-A9开发板（三）——挂载根文件系统](https://blog.csdn.net/cotex_A9/article/details/132354963)，这个博主写批处理文件来完成构建感觉很专业，反复操作更有效率


创建好这三个文件之后，制作根文件系统镜像
```
qemu-img create -f raw linux.img 256M
mkfs -t ext4 ./linux.img

mkdir tmpfs
sudo mount -o loop ./linux.img tmpfs/
sudo cp -r rootfs/* tmpfs/
sudo umount tmpfs 
rm -r tmpfs
file linux.img
```

---[ end Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0) ]---
根文件系统构建有问题
> 2024年2月20日12:04:44：应该是qemu配置指定根文件系统那句没写对 也就是 -append 后边那句
重新构建文件系统，一个简单例子，参考[使用qemu搭建linux内核开发环境详细教程-五]（https://blog.csdn.net/weixin_38227420/article/details/88402738）
重新构建文件系统，先来个简单的
```
vim init.c

arm-linux-gnueabi-gcc -static -o init init.c
echo init|cpio -o --format=newc > initramfs

qemu-system-arm -M vexpress-a9 -smp 4 -m 256M -kernel linux-6.6.16/arch/arm/boot/zImage -dtb linux-6.6.16/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -append "init=/init console=ttyAMA0" -nographic -initrd ./initramfs

```
![Alt text](image-5.png)
![Alt text](image-6.png)

对比下来rcS和fstab写得不一样
写一个/etc/init.d/rcS文件,注意给权限
```
#！/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
sbin/mdev -s

```
不要/etc/fstab文件，手动挂载了
```
```
制作镜像
find ./rootfs/ | cpio -o --format=newc > ./rootfs.img
跟镜像没关系，一样的错
qemu-system-arm -M vexpress-a9 -smp 4 -m 256M -kernel linux-6.6.16/arch/arm/boot/zImage -dtb linux-6.6.16/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -append "root=/dev/ram rdinit=sbin/init console=ttyAMA0" -nographic -initrd ./rootfs.img

![Alt text](image-7.png)
出现一样的问题，不能挂载根文件系统在未知的块
参考（https://blog.csdn.net/qq153471503/article/details/126976481）

看原子哥的资料，可能是因为root = /dev/mmcblk
**SD/MMC/EMMC(卡) /dev/mmcblk0,/dev/mmcblk1 mmcblk=mmc卡**
挂载root的原因，之后试一下

可能是根文件系统没构建好，参考这篇文章再重新弄吧（https://blog.csdn.net/cotex_A9/article/details/132354963）

cd busybox-1.36.1
vi automake_arm32.sh
 
# 在automake_arm32.sh中添加下列代码
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- clean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- menuconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j12
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- CONFIG_PREFIX=../objects/rootfs-arm32 install

chmod +x automake_arm32.sh
./automake_arm32.sh

将“Settings->Build Options->Build static binary”选上

cd ../objects/rootfs-arm32

mkdir dev
sudo mknod -m 666 dev/tty1 c 4 1
sudo mknod -m 666 dev/tty2 c 4 2
sudo mknod -m 666 dev/tty3 c 4 3
sudo mknod -m 666 dev/tty4 c 4 4
sudo mknod -m 666 dev/console c 5 1
sudo mknod -m 666 dev/null c 1 3

mkdir lib
sudo cp -d /usr/arm-linux-gnueabi/lib/*.so* ./lib

mkdir -p etc/init.d
touch etc/init.d/rcS
chmod 777 etc/init.d/rcS
vim etc/init.d/rcS

标注这几个文件的出处

touch etc/fstab
vim etc/fstab
 
# 在fstab中填入以下内容
proc    /proc           proc    defaults        0       0
none    /dev/pts        devpts  mode=0622       0       0
mdev    /dev            ramfs   defaults        0       0
sysfs   /sys            sysfs   defaults        0       0
tmpfs   /dev/shm        tmpfs   defaults        0       0
tmpfs   /dev            tmpfs   defaults        0       0
tmpfs   /mnt            tmpfs   defaults        0       0
var     /dev            tmpfs   defaults        0       0
ramfs   /dev            ramfs   defaults        0       0

touch etc/profile
vim etc/profile
 

#!/bin/sh 
USER="root"
LOGNAME=$USER
# export HOSTNAME=vexpress-a9
export HOSTNAME=`cat /etc/sysconfig/HOSTNAME`
export USER=root
export HOME=root
export PS1="[$USER@$HOSTNAME:\w]\#"
PATH=/bin:/sbin:/usr/bin:/usr/sbin 
LD_LIBRARY_PATH=/lib:/usr/lib:$LD_LIBRARY_PATH
export PATH LD_LIBRARY_PATH


mkdir etc/sysconfig
vi etc/sysconfig/HOSTNAME
 
# 在HOSTNAME中添加以下内容
vexpress-a9

mkdir mnt proc root sys tmp var


cd ..
sudo mkdir /mnt/rootfs
sudo chmod 777 /mnt/rootfs
vi makefs-arm32.sh

chmod +x makefs-arm32.sh
./makefs-arm32.sh


vi qemu-start-vexpress-a9.sh

chmod +x qemu-start-vexpress-a9.sh
./qemu-start-vexpress-a9.sh

路径要改一下，完美运行了！！！

我们这个实验没有uboot也能行是因为qemu自带bootloader，uboot构建跟Linux内核的构建差不多，只是利用uboot构建内核的时候要涉及nfs网络的使用，利用网络将zImage和设备树传到uboot，然后网络挂载根文件系统，本系列以驱动开发学习为主，暂不介绍这些，要了解可以参考其他博主文章
> 1. [WSL2下Ubuntu22.04使用Qemu搭建虚拟Vexpress-A9开发板（四）——通过U-boot引导加载内核](https://blog.csdn.net/cotex_A9/article/details/132365036)
> 2. 正点原子的I.MX6U嵌入式Linux驱动开发指南V1.81（正点原子网站开源获取，前面有链接）

### 尝试安装第一个虚拟设备驱动


### 给热爱学习的小伙伴开的书单
* 深度探索Linux操作系统：系统构建和原理解析
    可以看出作者十分热爱技术，文字里充满激情，本文章借鉴了本书1——4章的内容。
* 奔跑吧Linux内核：入门篇
    这本书对初学者不太友好，我认为对于细节的处理是一本教材的灵魂，这本书恰恰在细节处的处理很潦草，有一些地方没展开讲，但本书以实验促进学习，使用qemu平台的思想很好，本系列也算是借鉴这本书的讲解思想。
* 正点原子的I.MX6U嵌入式Linux驱动开发指南V1.81（原子哥网站开源获取）
    原子哥的教程非常全，从Linux系统使用开始讲的，而且有配套实践视频，缺点就是太多了太全了，我认为初学者学习还是先有主线，先学重点，然后逐渐衍生学习才好
* qemu的help手册 boot特殊命令

### 欢迎评论
* 博主也是嵌入式初学，水平有限，文章难免有错误和不准确的地方，恳请广大读者提出宝贵意见！！！十分愿意与大家交流！！！


