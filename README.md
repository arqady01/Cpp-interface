# 单例模式
## 饿汉式
饿汉式不存在线程安全问题
```c++
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

## 懒汉式
懒汉式存在线程安全问题<br>
```c++
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
```c++
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
```c++
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
```c++
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
```c++
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
https://github.com/arqady01/-/blob/main/%E6%97%A0%E6%A0%87%E9%A2%98%E7%BB%98%E5%9B%BE%20(2).png
