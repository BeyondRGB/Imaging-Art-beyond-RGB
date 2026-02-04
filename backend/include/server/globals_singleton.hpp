#ifndef GLOBALS_SINGLTON_H
#define GLOBALS_SINGLTON_H

#include <string>

class GlobalsSingleton {
  public:
    ~GlobalsSingleton();
    static GlobalsSingleton *get_instance();

    bool is_test();
    std::string app_root();
    int get_port();

    void set_is_test(bool is_test);
    void set_app_root(std::string app_root);
    void set_port(int p);

  protected:
  private:
    GlobalsSingleton() {}
    static GlobalsSingleton *instance;
    bool is_test_m = false;
    std::string app_root_m = "./";
    int port = 9002;
};

#endif // !GLOBALS_SINGLTON_H
