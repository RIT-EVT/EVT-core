
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
    };

    class SubMenu : MenuItem
    {
        public:

            SubMenu(char* option, char* text, void* cb, void* ctx, MenuItem[itemCount] items);

        private:
        
            int itemCount = 10;
            MenuItem[10] items;
    };
}