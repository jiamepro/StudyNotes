 #include "stdio.h"   
    void generate_initializer(char * string) {
        static char separator = ' ';
        printf("%c %s \n",separator,string);
        separator = ',';
        printf("%c %s \n",separator,string);
        separator = '.';
        printf("%c %s \n",separator,string);
    }

    int main(){
        generate_initializer("123");
        generate_initializer("456");
        return 0;
    }