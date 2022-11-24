//
// Created by danil on 21.10.2022.
//

#include <utils/encoding.h>
#include <utils/file.h>

//template<typename AttrType>
//class AttributeHolder {
//public:
//    AttrType attr;
//
//    AttrType& getAttr() {
//        return attr;
//    }
//};
//
//class ViewAttr {
//    int width;
//};
//
//class View : AttributeHolder<ViewAttr> {
//
//};
//
//class TextViewAttr : ViewAttr {
//    char* str;
//};
//
//class TextView : View, AttributeHolder<TextViewAttr> {
//
//};

class A {
public:
    virtual A& get() {
        return *this;
    }
};

class B : public A {
public:
    B& get() override {
        return *this;
    }
};

int main() {
    utils::configureUtf8();
    auto fos = utils::openFileOStream(std::wstring(L"тест.txt"), std::ios::out);
    fos << "hello" << std::endl;
    fos.flush();

//    B b;
//    std::cout << b.get() << std::endl;
//    std::cout << ((A)b).get() << std::endl;
//
//    A a;
//    a = b;
//    TextView textView;
//    auto x = (TextViewAttr)textView.getAttr();


    return 0;
}