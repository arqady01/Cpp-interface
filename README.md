* [设计模式](#1)
	* [单例模式](#1.1)
	* [简单工厂模式](#1.2)
	* [工厂模式](#1.3)
	* [抽象工厂模式](#1.4)
* [GDB](#2)

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
懒汉式存在线程安全问题<br>
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
