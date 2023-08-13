export module math;

import:abc;

export int add(int a, int b){
    return a + b;
}

export int func_hide_imp(int a, int b);

export template<typename T>
auto a_tmp_func(T a)
{
    return a;
}

template<typename T>
T imp(T a)
{
    return a;
}

export template<typename T>
T a_tmp_func_hide_imp(T a)
{
    return imp(a);
}

export template<typename T>
T a_tmp_func_hide_imp_in_partition_unit(T a)
{
    return partition_imp(a);
}
