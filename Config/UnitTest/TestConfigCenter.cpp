#include "ConfigCenter.h"
#include <iostream>
#include <string>
using namespace std;

void intChange(const int theValue)
{
    cout << "change to value:" << theValue << endl;
}

int main()
{
    int test0 = Config::ConfigCenter::instance()->get("test.test0", -1);
    cout << "test0:" << test0 << endl;
    int test1 = Config::ConfigCenter::instance()->get("test.test1", -1);
    cout << "test1:" << test1 << endl;
    int notexist = Config::ConfigCenter::instance()->get("notexist", -1);
    cout << "notexist:" << notexist << endl;
    std::string str = Config::ConfigCenter::instance()->get("test.str", "notexist");
    cout << "str:" << str << endl;

    Config::ConfigCenter::instance()->
        registValueWatcher("test.test0", (void*)0, Config::IntParameter::Watcher(intChange));

    Config::ConfigCenter::instance()->set("test.test0", test0 + 1);
    Config::ConfigCenter::instance()->set("test.test1", test1 + 1);
    Config::ConfigCenter::instance()->set("test.str", "write back");
    Config::ConfigCenter::instance()->saveXml(".saved_config.xml");
    return 0;
}

