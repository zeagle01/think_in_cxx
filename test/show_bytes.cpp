

#include "gmock/gmock.h"



namespace show_bytes
{
    template<typename T>
    void show_bytes(T* data)
    {
        unsigned char *bytes = (unsigned char *)data;
        for (size_t i = 0; i < sizeof(T); i++)
        {
            printf("%.2x ",bytes[i]);
        }
        printf("\n");
    }
}



TEST(Show_Bytes,aaa)
{
    int i=1;
    show_bytes::show_bytes(&i);

    int *pi = &i;
    show_bytes::show_bytes(&pi);


}