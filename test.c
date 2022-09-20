int add(int x, int y);
int sub(int x, int y);

int main()
{
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

