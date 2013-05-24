
	OpenJtag支持烧写MLC的Nand Flash啦！！！！

"Ok6410_mlc_demo"使用说明及注意事项：

	1."Ok6410_mlc_demo"是写给OK6410开发板的裸机程序，Nand Flash是4k每页的MLC，内存是256M；
	2.请使用"Ok6410_mlc_demo"下的"arm-linux-gcc-4.2.2-eabi.tar.bz2"编译器进行编译和测试，否则可能达不到期望的效果；
	3.如果裸机编译出来的bin文件大于2K，请用"Ok6410_mlc_demo"目录下的"mlc_tool"进行处理后，再进行下载；
		用法说明：
		(1).编译工具(在linux环境下)
		cd mlc_tool
		gcc -o mlc_boot_tool mlc_boot_tool.c	// 得到的 mlc_boot_tool 就是最终的工具
		
		(2).处理bin文件
		以"Ok6410_mlc_demo"目录下的"16th_nand_all"为例子
		a. cd Ok6410_mlc_demo
		b. make clean
		c. make						// 得到 uart.bin ，发现他大于2K，所以需要处理
		d. ./mlc_boot_tool uart.bin wy.bin		// 重新生成的 wy.bin 就这最用下载到开发板上面的bin文件
		
		注意：
		为什么需要"mlc_boot_tool"工具呢？请结合源码和6410的启动流程自行分析。
1. S3C6410 nand启动，会自动拷贝前8K数据到SRAM中，但是我们的MLC nand每页大小为4K的，而6410的CPU硬件决定了在它自动拷贝的时候每页只能拷贝2K。
   即NAND启动时，nand的前4页中，每页中的前2K数据才是有用的。
2.所以我们的工具就是把bin文件前面连续的8K数据拆出来放到MLC NAND的前4页里去。
  即：BIN文件中的第1个2K数据放在NAND的第1页，第2个2K数据放在第2页，第3个2K数据放在第3页，第4个2K数据放在第4页。
  因为MLC NAND页大小为4K，所以前面4页只有一半的空间被用到了。
  从第5页开始，每页的4K空间都被用到了。
3. mlc_boot_tool工具就是把bin文件做成：(2k有用数据+2k无用数据) x 4 + 剩余数据
		
	4.测试"Ok6410_mlc_demo"目录下的"16th_nand_all"程序的时候，需要通过串口发送二进制文件到开发板上，
		这里是不能给二进制文件加入任何协议和头部信息的，所以大家可以使用SecureCRT来发送，如下：
		
			SecureCRT->Transfer->Send Binary...    // SecureCRT是一终端软件，网上一大堆
	5.一路移植过来，关键地方也就是重定位过程。
		因此，重定位过程是个不断修改和完善的过程。
		请大家在阅读代码的时候，注意对比每个程序的重定位过程的细微变化，这也是成功的关键。
	6.在做"25th_lcd_pwm"的实验的时候，由于里面引用了除法运算，编译时会报错，如下：
		pwm.o: In function `pwm_set':
		pwm.c:(.text+0x64): undefined reference to `__aeabi_uidiv'
		pwm.o: In function `pwm_menu':
		pwm.c:(.text+0x184): undefined reference to `__aeabi_uidiv'
		pwm.c:(.text+0x1c0): undefined reference to `__aeabi_uidiv'
		make: *** [uart.bin] Error 1
		解决办法，请自行阅读如下资料：
		http://www.embedu.org/Column/Column526.htm
		
		解决问题关键点：
		a.Makefile中添加
		LIBPATH   	:= -lgcc -L/usr/local/arm/4.2.2-eabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.2.2

		b.在任意文件中，添加一个如下的空函数
		int raise(int signum)
		{
			return 0;
		}



广告一：
韦东山Linux视频第1期: S3C2440/6410裸板/u-boot/内核/文件系统/驱动入门
韦东山Linux视频第2期:深入驱动边写边画边讲
韦东山Linux视频第3期:实战项目,系统框架,应用驱动统筹考虑,边写边画边讲
网        站: www.100ask.net
淘        宝: 100ask.taobao.com
业务 QQ/邮箱: weidongshan@qq.com 
技术支持邮箱: 17653039@qq.com
视频QQ群    : 173968594,49256475,177535949,
              176512363,83101278,84174029,
              260353473,146766255,222798672
技术支持MSN : thisway.diy@163.com
业务范围    : Linux咨询/企培/故障解决





广告二：
uboot完全注释:

共享以前分析uboot时做的笔记，包括：
    1.u-boot-1.1.6的配置编译过程分析.doc
    2.u-boot-2012.04.01的配置编译过程分析.doc
    3.u-boot-1.1.6完全注释，基本实现了逐行的注释。
    ...... // 后继更新的将直接上传群共享
    
    请大家原谅哈，由于我们想建立一个uboot技术交流社区，
需要经费。故上述uboot笔记，需要9元钱购买，购买链接地址：

		http://item.taobao.com/item.htm?spm=0.0.0.31.lHTZaH&id=17051965131

	  当然，为了让大家觉得花钱值得。大家购买后，可以凭借
购买的帐号，加入“uboot移植交流群”。
		该群由我(韦东山LINUX视频的答疑助手@万勇)和吴伟东(韦东
山LINUX视频的答疑助手-小吴)负责，我们会在：
		每周3晚上7点到10点和每周日(下午3点到6点和晚上8点到10
点）负责答疑。和大家一起学习uboot，实现逐行分析注释uboot。

我们建立该uboot技术交流群的宗旨是：
      找些人在一起，为全天下的板子移植uboot！！！！
      
入群条件：
		1.你是高手，可以无条件的入群。当然有考核的；
    2.或者你是想学习的，购买“uboot完全注释”后，提供网名，入群