* [➕ C/C++](#c/c++)
* [💾 数据库](#database)
* [📏 设计模式](#1)
	* [单例模式](#1.1)
	* [简单工厂模式](#1.2)
	* [工厂模式](#1.3)
	* [抽象工厂模式](#1.4)
* [GDB](#2)

<h1 id="c/c++">C/C++</h1>

## 左值&右值

### 什么是左值、右值

不考虑引用以减少干扰：左值可以取地址、位于等号左边；而右值没法取地址，位于等号右边<br>
左右值的概念很清晰，有地址的变量就是左值，没有地址的字面值、临时值就是右值

`int a = 5;`


- a可以通过&取地址，位于等号左边，所以a是左值
- 5位于等号右边，5没法通过&取地址，所以5是个右值

```cpp
struct A {
    A(int a = 0) {
        a_ = a;
    }
    int a_;
};
A a = A();
```

- a可以通过&取地址，位于等号左边，所以a是左值
- A()是个临时值，没法通过&取地址，位于等号右边，所以A()是个右值

### 左值引用

能指向左值，不能指向右值的就是左值引用，左值引用无法指向右值

```cpp
int a = 5;
int &ref_a = a; //左值引用指向左值，编译通过
int &ref_a = 5; //左值引用指向右值，编译失败
```

但是，const左值引用是可以指向右值的：

`const int &ref_a = 5; //编译通过`

const左值引用不会修改指向值，因此可以指向右值，这也是为什么要使用`const &`作为函数参数的原因之一，如`std::vector`的`push_back`：

`void push_back (const value_type& val);`

如果没有const，`vec.push_back(5)`这样的代码就无法编译通过

### 右值引用

右值引用的标志是`&&`，可以指向右值，不能指向左值

```cpp
int &&ref_a_right = 5; //ok
int a = 5;
int &&ref_a_left = a; //编译不过，右值引用不可以指向左值
ref_a_right = 6; //右值引用的用途：可以修改右值
```

### 右值引用指向左值

```cpp
int a = 5; // a是个左值
int &ref_a_left = a; //左值引用指向左值
int &&ref_a_right = std::move(a); //通过move将左值转为右值，可以被右值引用指向
cout << a; //打印结果：5
```

看上去是左值a通过std::move移动到了右值ref_a_right中，那是不是a里边就没有值了？并不是，打印出a的值仍然是5

move是一个非常有迷惑性的函数，往往以为它能把一个变量里的内容移动到另一个变量，但事实上std::move移动不了什么，唯一的功能是把左值强制转化为右值，让右值引用可以指向左值。其实现等同于一个类型转换，所以，单纯的move不会有性能提升

## 函数指针&指针函数

**函数指针**
定义一个指向函数的指针变量，
`int (*p)(int x, int  y);`

**指针函数**
指的是函数的返回值是一个指针，
`int *p(int a,int b);`

<h1 id="database">数据库</h1>

## 内存淘汰策略

当redis占用内存超过maxmemory时，会触发主动清理策略
- 针对设置了过期时间的key处理
	- volatile-ttl 筛选设置了过期时间的键值对，越早过期的越先被删除
	- volatile-random 筛选设置了过期时间的键值对，随机删除
	- volatile-lru 利用LRU算法筛选设置了过期时间的键值对进行删除
	- volatile-lfu 利用LFU算法筛选设置了过期时间的键值对进行删除
- 针对所有的key做处理
	- allkeys-random 在所有的键值对中进行随机删除
	- allkeys-lru 利用LRU算法对所有数据筛选后删除
	- allkeys-lfu 利用LFU算法对所有数据筛选后删除
- 不处理
	- noevent 不剔除任何数据，拒绝所有写入操作并返回错误信息，此时redis只响应读操作

## LRU ＆ LFU

- LRU，最近最少使用(least recently used)，以最近一次访问时间作为参考，淘汰很久没被访问过的数据
- LFU，最不经常使用(least frequently used)，以次数作为参考，淘汰最近一段事件被访问次数最少的数据
绝大多数情况使用LRU，但当存在大量热点缓存数据时，LFU更佳

<h1 id="1">设计模式</h1>
<h2 id="1.1">单例模式</h2>

<h3>饿汉式</h3>
饿汉式不存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "hungry\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = new Singoton; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

<h3>懒汉式</h3>
懒汉式存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```
试想，现在有两个线程调用了Singoton::getInstance()方法，线程A首先进行判空操作，于是new了一个Singoton对象，此时发生了上下文切换，调度权给到了线程B，线程B首先也进行判空操作，发现为空，于是也new了一个Singoton对象，就发生了问题。<br>
* 双重检查锁定解决线程安全问题

```cpp
#include <mutex>
class Singoton {
public:
	static Singoton* getInstance() {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
	static std::mutex mtx;
};
Singoton* Singoton::ptr = nullptr; //定义
std::mutex Singoton::mtx;
```
假想现在有三个线程A B C，线程A首先加锁，再判断、new对象，若此时发生了上下文切换，调度权给了线程B和线程C，但是他没有锁资源，进入阻塞态，最终调度权回归线程A，将锁头解锁，于是线程B和线程C自然就解除阻塞，进行判空操作，此时就会因为判空失败而直接返回，线程安全<br>
但是效率太低了，进行双重检查，在加锁之前再进行一次判空操作：

```cpp
static Singoton* getInstance() {
	if (ptr == nullptr) {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
	}
	return ptr;
}
```

* 原子操作弥补双重检查的bug
因为`new`操作符在汇编中是对应的机器指令可能会发生重排，依旧会产生线程安全问题，可以采用原子操作来弥补

```cpp
#include <iostream>
#include <mutex>
#include <atomic>
class Singoton {
public:
	static Singoton* getInstance() {
		Singoton* task = sptr.load(); //将sptr从原子中取出备用
		if (task == nullptr) {
			mtx.lock();
			task = sptr.load();
			if (task == nullptr) {
				task = new Singoton();
				sptr.store(task);
			}
			mtx.unlock();
		}
		task = sptr.load();
		return task;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static std::mutex mtx;
	static std::atomic<Singoton*> sptr;
};
std::mutex Singoton::mtx;
std::atomic<Singoton*> Singoton::sptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

* 静态对象（需要编译器兼容C++11标准）
```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		static Singoton sn; //必须静态，不然出了大括号其生命周期就结束
		return &sn; //取地址并返回
	}
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
};

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```
C++11规定，当逻辑进入一个未被初始化的声明变量，所有的并发操作都必须等待该变量完成初始化。<br>
静态变量在全局/静态区，只要在分区内，只要数据等于0，就是未被初始化的，比如`int a = 0;`，`int b;`；但是`int c = 1;`就是初始化过的变量<br>
所以`static Singoton sn;`也是未初始化的静态局部变量，于是所有的并发操作都会被阻塞

<h2 id="1.2">简单工厂模式</h2>
* 优点
简单工厂的特点就是“简单粗暴”，通过一个含参的工厂方法，我们可以实例化任何产品类，上至飞机火箭，下至土豆面条，无所不能。所以简单工厂有一个别名：上帝类。
* 缺点
违反“封闭-开放原则”，简单工厂对于增加新的产品，无能为力，因为增加新产品只能通过修改工厂方法来实现，违反“封闭”原则。

```cpp
#include <iostream>
class AbstractPerson {
public:
	virtual void IQ() = 0;
	virtual void EQ() = 0;
	//动态绑定就是父类指针绑定子类对象，在delete父类指针时，若没重写，只会释放父类资源而不会释放子类资源
	virtual ~AbstractPerson() {};
};
class School : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "学校期间涨20点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "学校期间涨3点EQ." << std::endl;
	}
};
class Social : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "社会期间降5点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "社会期间涨20点EQ." << std::endl;
	}
};

//工厂类
enum class Type:char{Shazi,Lengzi}; //C++强类型枚举，数据类型为char型
class Factory {
public:
	AbstractPerson* createPerson(Type type) {
		AbstractPerson* ptr = nullptr;
		if (type == Type::Shazi)
			ptr = new School; //傻子就送去学校
		else if (type == Type::Lengzi)
			ptr = new Social; //愣子就送进社会
		return ptr;
	}
};
int main() {
	Factory factory;
	AbstractPerson* p = factory.createPerson(Type::Shazi); //父类指针指向子类对象，实现动态绑定
	p->IQ();
	p->EQ();
}
```

<h2 id="1.3">工厂模式</h2>

```cpp
#include <iostream>
class AbstractPerson {
public:
	virtual void IQ() = 0;
	virtual void EQ() = 0;
	//动态绑定就是父类指针绑定子类对象，在delete父类指针时，若没重写，只会释放父类资源而不会释放子类资源
	virtual ~AbstractPerson() {};
};
class School : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "学校期间涨20点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "学校期间涨3点EQ." << std::endl;
	}
};
class Social : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "社会期间降5点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "社会期间涨20点EQ." << std::endl;
	}
};

enum class Type:char{Shazi,Lengzi}; //C++强类型枚举，数据类型为char型
//不同职责工厂的父类类
class AbstractFactory {
public:
	virtual AbstractPerson* createPerson() = 0;
	virtual ~AbstractFactory() {}
};
//加IQ的工厂类
class IQFactory : public AbstractFactory {
public:
	AbstractPerson* createPerson() {
		return new School;
	}
};
//加EQ的工厂类
class EQFactory : public AbstractFactory {
public:
	AbstractPerson* createPerson() {
		return new Social;
	}
};
int main() {
	AbstractFactory* af = new IQFactory;
	AbstractPerson* p = af->createPerson();
	p->IQ();
	p->EQ();
}
```

<h2 id="1.4">抽象工厂模式</h2>

```cpp
#include <iostream>
#include <string>
//船体
class shipBody {
public:
	virtual std::string getBody() = 0;
	virtual ~shipBody() {}; //虚析构函数
};
//钢制船
class steelBoot : public shipBody {
public:
	std::string getBody() override {
		return std::string("钢铁船身.");
	}
};
//合金船
class alloyBoot : public shipBody {
public:
	std::string getBody() override {
		return std::string("合金船身.");
	}
};

//引擎
class engine {
public:
	virtual std::string getEngine() = 0;
	virtual ~engine() {};
};
//柴油动力
class oilEngine : public engine {
public:
	std::string getEngine() override {
		return std::string("柴油动力.");
	}
};
//核动力
class atomicEngine : public engine {
public:
	std::string getEngine() override {
		return std::string("核动力.");
	}
};

//船
class ship {
public:
	ship(shipBody* s, engine* e) : m_shipBody(s), m_engine(e) {}
	~ship() {
		delete m_shipBody;
		delete m_engine;
		m_shipBody = nullptr;
		m_engine = nullptr;
	}
	std::string getInfo() {
		std::string info = m_shipBody->getBody() + m_engine->getEngine();
		return info;
	}
private:
	shipBody* m_shipBody;
	engine* m_engine;
};

//抽象工厂类
class AbstractFactory {
public:
	virtual ship* createShip() = 0;
	virtual ~AbstractFactory() {};
};
//造最基本的船
class basicShip : public AbstractFactory {
public:
	ship* createShip() override {
		ship* sp = new ship(new steelBoot, new oilEngine);
		return sp;
	}
};
//造最先进的船
class ultraShip : public AbstractFactory {
public:
	ship* createShip() override {
		ship* sp = new ship(new alloyBoot, new atomicEngine);
		return sp;
	}
};
int main() {
	//我要买最先进的船！
	AbstractFactory* asf = new basicShip;
	ship* sip = asf->createShip();
	std::cout << sip->getInfo() << std::endl;
	delete sip;
	delete asf;
}
```

<h1 id="2">GDB调试</h1>

### 运行
run(r)：运行程序，当遇到断点后，程序会在断点处停止运行，等待下一步命令<br>
continue(c)：继续执行，到下一个断点处（或运行结束）<br>
next(n)：单步跟踪程序，当遇到函数调用时，也不进入此函数体<br>
step(s)：单步调试如果有函数调用，则进入函数；与命令n不同，n是不进入调用的函数的<br>
until：当你厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体<br>
until+行号： 运行至某行，不仅仅用来跳出循环<br>
finish： 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数值等信息<br>
call 函数(参数)：调用程序中可见的函数，并传递“参数”，如：call gdb_test(55)<br>
quit(q)：退出gdb<br>

### 断点
break n (简写b n):在第n行处设置断点<br>
b fn1 if a＞b：条件断点设置<br>
break func (b func)：在函数func()的入口处设置断点<br>
delete n：删除第n个断点<br>
disable n：暂停第n个断点<br>
enable n：开启第n个断点<br>
info b：显示当前程序的断点设置情况<br>
delete breakpoints：清除所有断点<br>

### 查看运行信息
bt: 当前运行的堆栈列表<br>
bt backtrace: 显示当前调用堆栈<br>
up/down 改变堆栈显示的深度<br>
set args 参数:指定运行时的参数<br>
show args：查看设置好的参数<br>
info program：来查看程序的是否在运行，进程号，被暂停的原因<br>

### 调试正在运行中的程序
新建一个窗口，`ps -ef|grep a.out`显示正在运行的a.out程序的线程ID，`gdb a.out -p ID号`进入gdb调试<br>

### 结束进程
killall根据名称终止正在运行的进程，而kill根据进程ID号（PID）终止进程<br>
pidof a.out：找到进程的PID，或者`ps -ef|grep a.out`<br>
kill -9 2551 2514 ：中止PID为2511和2514的进程，或者`killall -9 a.out`<br>
