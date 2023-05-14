# CEventSignalSlot

A project like Qt Core (just a toy:))

# Warning ！！！！！ JUST A TOY

# ## Build(with windows vs 2022 ：） )

```bash
git clone https://github.com/Clittlelearn/CEventSignalSlot.git

cd CEventSignalSlot
mkdir build
cmake ..
# Everyone knows !!!!!!!! 
```

## ## USE

just like Qt 5 (Not exactly the same :( , There is no MOC here ha ha ha .....),

But there is no graphical interface

You need to inherit Object like this:

```cpp
class Widget:public Object{
public:
    
};
```

and signal :

```cpp
class Widget:public Object{
public:
    _SIGNAL_VOID(Widget,hello) // this is signal

    /*
       I certainly know that these parameters are not handled elegantly enough

    */
    _SIGNAL(Widget,person,
    _ARGS(const std::string & name,int age,bool sex),
    _ARGS(const std::string &,int,bool),
    _ARGS(name,age,sex)
    )
};
```

and connect:

```cpp
class Widget:public Object{
public:
    Widget(){
        connect(this,&Widget::hello,this,&Widget::world);
        connect(this,&Widget::person,this,&Widget::person_slot);
    }

    _SIGNAL_VOID(Widget,hello)

    _SIGNAL(Widget,person,
    _ARGS(const std::string & name,int age,bool sex),
    _ARGS(const std::string &,int,bool),
    _ARGS(name,age,sex)
    )

    void world() {
        std::cout <<  "world" << std::endl;
    }

    void person_slot(const std::string& name, int age, bool sex) {
        infoL("name:" << name << "age:" << age
            << "sex:" << (sex ? "Woman" : "man")
        )
    }
}
```

In the  main function:

```cpp
int main(){
    PCore core;
    Widget widget;
    widget.hello();

    widget.person("AAAA",23,false);

    core.exec();

}
```

C++ is a magic language hhhhhhhhhh......

(This README is terrible :( ,Forgive me )
