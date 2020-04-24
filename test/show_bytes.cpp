

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

    float f=float(i);

    show_bytes::show_bytes(&f);

    int *pi = &i;
    show_bytes::show_bytes(&pi);

    float *pf = &f;
    show_bytes::show_bytes(&pf);


}

TEST(Show_Bytes,show_int_bytes)
{
    int i=1;
    show_bytes::show_bytes(&i);
}