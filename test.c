int add(int x, int y);
int sub(int x, int y);

typedef struct test_struct_t{
   int x, y;
   float* b;
}t;

typedef int new_int_t;

typedef new_int_t super_new_int_t;

int main()
{
   super_new_int_t abc = 32;
   t a, b;
   a.x = 3;
   int x, y;
   x = 3;
   y = 3 + 2*x;
   int sum = add(x, y);
   int subs = sub(x, y);
}

int add(int x, int y)
{
   return x+y;
}

int sub(int x, int y)
{
   return x-y;
}

