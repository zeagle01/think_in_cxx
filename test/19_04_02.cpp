

#include "gtest/gtest.h"
#include "string.h"

char name[] = "fengjianhua123456780";

TEST(_19_04_02_Test, test_use)
{

    char output[8];
    strcpy(output, name);

    for (int32_t i = 0; i < 8 && output[i]; i++)
    {
        printf("\\0x%x", output[i]);
    }
}
