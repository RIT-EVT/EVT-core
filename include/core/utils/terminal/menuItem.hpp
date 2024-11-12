
namespace core:utils
{
    class MenuItem
    {
        public:

            MenuItem(char* option, char* text, void* cb, void* ctx);

        private:

            char* option;
            char* text;
            void* cb;
            void* ctx;
    }

    class subMenu : public menuItem
    {
        public:

            SubMenu(char* option, char* text, void* cb, void* ctx, menuItem[itemCount] items)

        private:

            int itemCount = 10;
            char* option;
            char* text;
            void* cb;
            void* ctx;
            void* items;
    }
}