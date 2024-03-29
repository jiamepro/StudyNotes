# C++ 基础
## 容器
* 一种数据结构模板，为的就是复用性的提升
* 各种数据结构，如vector、list、deque、set、map等,用来存放数据，从实现角度来看，STL容器是一种class template。常

### set容器基本概念
* Set的特性是所有元素都会根据元素的键值自动被排序。Set的元素不像map那样可以同时拥有实值和键值，set的元素即是键值又是实值。Set不允许两个元素有相同的键值。

* 我们不可以通过set的迭代器改变set元素的值，因为set元素值就是其键值，关系到set元素的排序规则。如果任意改变set元素值，会严重破坏set组织。换句话说，set的iterator是一种const_iterator.

* multiset特性及用法和set完全相同，唯一的差别在于它允许键值重复。set和multiset的底层实现是红黑树.   

* 常用方法：
  * （1）insert(x) 可将 x 插入 set 容器中，并自动递增排序和去重，时间复杂度为 O（logN），其中 N 为 set 内的元素个数。
  * （2）find() find(value) 返回 set 中对应值为 value 的迭代器，时间复杂度为 O（logN），其中 N 为 set 内的元素个数。
  * （3）erase() 删除单个元素或删除一个区间内的所有元素。
    * （1）st.erase(it)  ，即删除迭代器为 it 处的元素，时间复杂度为 O（1）。
    * （2）st.erase(value)  ，value 为所需要删除元素的值。时间复杂度为 O（logN），N 为 set 内的元素个数
    * erase(first,last) 即删除 [first,last) 内的所有元素。其中 first 为所需要删除区间的起始迭代器，而 last 则为所需要删除区间的末尾迭代器的下一个地址。时间复杂度为 O（last-first）。
  * （4）size() 用来获得 set 中元素的个数，时间复杂度为 O（1）。size()返回的是 unsigned 类型，不过一般来说用 %d 不会出很大问题，这一点对所有 STL 容器都是一样的。
  * （5）clear() 用来清空 set 中的所有元素，时间复杂度为 O（N），其中 N 为 set 中元素的个数。

到时候直接查用的命令！不用系统学C++的STL，嵌入式很少用到，C with class的用法和思想要熟悉

## 知乎对C和C++ 的评价
作者：听心跳的声音
链接：https://www.zhihu.com/question/374663834/answer/1257751860
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

* 在嵌入式Linux领域占据应用层的半壁江山(QT/C++怎么说也是嵌入式平台的主流界面方案之一), 单片机的主流编译语言可预见的长期仍然是C和少量汇编的结合体，而嵌入式Linux领域的未来在我看来更倾向于多语言范式的混合应用编程，内核模块使用C，应用层逻辑使用C++, Python, nodejs的混合编程，而界面的话使用java和QT/C++
* 原因：
1. 主流的flash容量仍然在64KB~256KB左右，目前的容量限制注定了C++中的模板，泛型编程和STL等很难被运用到开发中，
2. 只使用支持class的C++，C语言是有结构体+函数指针可以替代的，从C换成C++并没有迫切的需求，此外在加上调试困难。
3. 因为历史的惯性，目前行业内无论大小公司，都大量的遗留和正在做的都是C语言项目(包含原厂的方案)，替换成rust就是商业成本问题，节约成本，节约内存空间，C是比C++，python， js有明显优势的。
4. 在嵌入式Linux领域, C++绝对是应用层主力之一，QT/C++虽然目前因为芯片性能的提升，逐渐被Android/Java所替代，但仍然在医疗，工控，车载导航等领域占据主流地位，而且这也是目前C++的重要应用领域之一，


作者：CodeAllen嵌入式​：

1.嵌入式里编译器都是被优化过的，基本都是阉割版，对c++也不例外，你说的操作内存部分多半还是会用C语言做，**c++大多用在应用层**，编辑环境是eclipse还是很好用的，c++在单片机纯逻辑编程里边其实和C语言没有太大差别，当然也是因为很多牛逼的特性没有上的原因

2.原则上就是学个class就可以搞了，不过这样写出来的代码基本就是C语言模式了，想深入c++的精华还是要按着类，对象，重载，继承，多态，模板和STL库的顺序深入了解下，不过这个过程应该比较痛苦，这时候c++已经和C语言没有关系了


另一个人：
看你的题目详细描述，确实不需要C++。因为C++和C完全是两个语言，编程思维上有很大的不同。

面向对象在代码重用和代码抽象上更适合大型项目，如果项目的复杂度非常的大，大到可能有上百号人来协作，此时用C语言开发就会有很大的弊端，最主要的体现就在于数据的封装性。这样埋藏的bug可能会非常的深，直到某天这个bug爆发了，可能需要大规模的重写非常多的代码，造成难以预料的后果。**但是在C++这样的语言中，使用一个private关键字就能解决上述问题**。


作者：阿布编程
链接：https://www.zhihu.com/question/374663834/answer/1503404657
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

先说结论：1. 嵌入式、单片机里面C++非常好使；2. C with class用来作大部分开发是完全可以胜任，如果用的好，能明显改善你的代码质量（嵌入式领域，个人不鼓励STL和模板，这个后面再说）。  只有5千行代码的GuiLite是嵌入式、单片机中常用的GUI框架；它就是C++编写，在GitHub有4.8K star，在Gitee有2K star。可能你觉得5千行能做什么？它不仅可以作常规的界面元素，还能在单片机平台上进行3D操作、可以与网页结合，把界面效果用网页的形势表现出来，当然也支持VR特效、最近还与FFmpeg集成，可以无依赖的支持视频播放。多说无益，有图为证
这里不是说GuiLite多强，而是想说明C++语言的魅力，如果没有使用C++语言，而用C的的话，至少需要几万行才能实现相同的效果；还记得著名的爱因斯坦bug方程吗？代码多一点点，bug数量就会显著增加。其实GuiLite就是典型的C with Class；相信很多同学觉得这很低级，**但这正是C++语言发明的初心。这种特性让你完全告别的了函数指针**；当然很多C的高手，就是用函数指针实现了C++的所有特性。首先为高手点赞，但作为普通韭菜的我们要明白它的代价就是一大堆函数指针；**只要函数指针的大量存在，代码的可读性就大大降低**，而C with Class就能用最优雅的方式消灭所有的函数指针，虽然你觉得它很low，但它就能让你的代码量大大缩小；而且它对编译器的支持极好，任何单片机编译器都能支持这种简单的C++特性。如果你还读过Linux的虚拟文件系统代码，请问是什么反复打断你领会代码含义？答案是函数指针，为了实现对多文件系统的支持，Linus可是在拼命的往代码里面使用函数指针。而如果选择用继承，虚函数来实现，其代码就可以大大简化。这就是用C实现派生，虚函数扩展的代价；你可能会说：Linus这种方式效率高呀！答案是：不存在；无论你如何在C语言层次做优化，都没发跟编译器层次的优化相提并论。作为开发者，编程思想远远比语法糖重要的多。C with Class是编程思想的进步，虽然在语法难度上面它不值一提。记住，我这里说的是编程思想，即使这么简单地语法，现在还是被滥用了，**完全不考虑实际需要，上来就是一个class，完全不顾及class发明者的初衷。class需要你在高level重整代码结构**，但你却用它污染每一个细节，每一行代码。还是那句话，用的好，5千行就能解决很多问题；用的不好，还不如不用，还是用你最擅长的语言去污染你的代码吧，这样污染的更有效率，对吧？  最后，STL，模板适合嵌入式吗？个人觉得不大适合，**首先这是对编译器的极大挑战，windows，linux平台不是问题，但在单片机环境可能存在兼容性的问题**；另外，**模板，STL对调试非常不友好，不太适合运行成本（步骤）相对复杂的嵌入式、单片机开发环境**。STL，模板的发明初衷也不是为嵌入式，单片机准备的；所以，强行使用，会给你带来很多麻烦。STL，模板的最佳使用环境是大型“游戏”。这套东西是典型用空间换时间的产物，很多牛逼的游戏所需的cpu，内存资源极少，就是他们的功劳，但代价是你的代码会比较庞大，没有1T的硬盘，就不要玩游戏了吧~~~STL，模板为什么能在游戏行业里面如鱼得水呢？首先，运行效率很高，这里不再赘述；其次，则是游戏的重复性太高，大家回忆一下，DOTA，英雄联盟，王者荣耀在玩法上面是不是很相近呢？正是因为相似性太高，代码重用就显得非常必要，否则游戏工业化的效率就很低，现在之所以半年就能出一款大型游戏，我说这是STL、模板的功劳，你信吗？我说是游戏引擎的功劳，你信吗？我说游戏引擎跟STL、模板是你中有我，我中有你，你信吗？  **总结一下，C++编程思想对嵌入式开发者很有帮助，直接效果就是能大幅度降低你的代码量和逻辑复杂度；STL，模板原则不适合大部分嵌入式使用环境**，因为嵌入式软件的特殊性往往超过通用性，代码复用的需求不强，但只要你知道它们是为什么而生的，就会为它们选择合适的使用环境。


## 《C++》
* 其理念是设计与问题本质特征相对应的数据格式
* 类是一种规范，描述新型数据格式，比如描述公司管理人员的特征，对象代表特定的管理人员
* 类规定需要哪些数据表示对象，和对对象的数据进行哪些操作
* OOP设计，首先设计类
* 好处：
  * 创建可重用代码，减少重复————继承
  * 保护数据————私有
  * 多态给运算符和函数多个定义，按语境选择所使用的定义
  * 继承：旧类派生出新类
* 优点之一：可重用和修改现有的、经过仔细检查的代码
* 泛型编程：可以只写一个功能函数应用于多种类型的数据

* C语言简洁、适合系统开发，使用广泛，且与UNIX系统紧密联系

* 库对常见的编程问题提供了可靠的解决方法！！！

* cin.get() 读取下一次敲击，让程序等待，独立窗口程序

* 名称空间，用来区别不同厂商的同名产品，using namespace std;引入后不用使用前缀std::

* C声明一般在函数或者过程开始之前，C++声明一般在首次使用处前，容易找

* C++可以连续使用赋值运算符 = 
* 还可以重新定义 << 可以识别更多类型

* C++ 有两种发消息的方法：1. 类方法 = 函数，2. 重载符号

* 编译找不到函数库里的声明，编译器加上 -l 选项 比如 -lm 添加 math库

* C++的初始化语法：int wrens(432);int wrens{432}; 设置wrens = 432，int wrens{}; 设为0

* 控制符 endl, 进制——dec，hex，oct

* C++新增了bool类型

* C++中可以省略关键字struct

* 在C++中int * 是一种复合类型，数字赋值给指针是想要强制转换

* int * pn = new int; 找一个适合int类型存储的位置，返回给pn

* 浪费时间，《C++》这本书里大部分跟C语言很相似，区别之处基本上被淹没了！！！

* 面向对象编程的思想很重要，C语言也可以实现面向对象编程