#ifndef EVT_TERM_STRUCTS
#define EVT_TERM_STRUCTS

struct TerminalInterface
{
    std::function<void()> func1;
    std::function<void()> func2;
    std::function<void()> func3;

    FunctionRunner(std::function<void()> f1, std::function<void()> f2, std::function<void()> f3)
        : func1(f1), func2(f2), func3(f3) {}

    void runAll() {
        std::cout << "Running func1...\n";
        func1();
        std::cout << "Running func2...\n";
        func2();
        std::cout << "Running func3...\n";
        func3();
    }
};

using callback_t = void (*)(core::io::UART&, char** inputList, void*);


#endif