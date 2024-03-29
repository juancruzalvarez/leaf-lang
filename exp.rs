module Math
{
   
import {
   Tests,
   Math,
   Drive
}


public:

   const F32_PI :: f32 = 3.141591;
   const PI := 3.141591;

   const ANGLE_TYPES {
      RIGHT_ANGLE,
      ACUTE_ANGLE,
      OBTUSE_ANGLE
   };

   type Vec3 :: struct {
      x, y :: f64;
   };

   fn (a :: vec3*) add(b :: vec3) {
      a.x += b.x;
      a.y += b.y;
   };

   fn dot(a, b :: vec3) -> f64 {
      return a.x*b.x + a.y*b.y;
   };

   fn cross(a, b :: vec3) -> vec3 {
      aux := 0.0;
      for i := 0; i<32; ++i {
         aux += a.x*b.x;
      }
      return aux;
   };


private:

   fn abs(x :: int) -> int => x > 0 ? x : -x;

   fn max(x, y :: int) -> int => x > y ? x : y;

   fn pow(x :: f32, n :: u32) -> f32 {
      res := 1.0;
      i := abs(n);
      while i >= 0 {
         res *= x;
         i--;
      }

      return n > 0 ? res : (1 / res) ;
   } 

}


//functions:
// fn identifier (? list< list<identifier> :: type> )? ::? type? { block } ;?

fn abs(x :: int) -> int{
   return x>0 ? x : -x;
}
fn sum(x, y :: int) -> int{
   return x + y;
}

fn sum(x, y) => x + y;

fn sum(x, y :: int) -> int => x+y;

fn abs(x :: int) -> int => x>0 ? x : -x;

//anonymous function

fn x => x*2;
fn x :: int -> int => x*2;
fn (x :: int) -> int => x*2;

abs_f := fn x => x>0 ? x : -x;
fn x, y => x + y;

fn (x) => x>0 ? x : -x;


//function type:

type num_condition_v1  :: fn int -> bool;
type bin_operator_v1   :: fn int, int -> int;
type pair_condition_v1 :: fn int, int -> bool;

type num_condition_v2  :: fn (int) -> bool;
type bin_operator_v2   :: fn (int, int) -> int;
type pair_condition_v2 :: fn (int, int) -> bool;

//struct type

type point_t :: struct{
   dist_to_center :: float;
   x, y :: int;
}


fn (point :: point_t*) move(x, y :: int){
   point.x+=x;
   point.y+=y;
}

//???????????????????????????
fn (a :: point_t) (==) (b :: point_t) -> bool{

}


//default values

type point_t :: struct{
   dist_to_center :: float = 0.0;
   x, y           :: int   = 0, 0;
};

// type alias

type new_int_t :: int;

type uint32_t :: uint32;

type typename :: type_value;


sorted_multiples_of_2_plus_five = nums
                                  .sort(fn x, y => x.a>y.a ? 1 : -1)
                                  .filter(fn x => x%2 = 0)
                                  .map(fn x => x+5);
//type union

type signed_number :: int32 | int64;

type unsigned_number :: uint32 | uint64;

type number :: signed_number | unsigned_number;


//interfaces?

class IEqable :: T has{
   fn (T) equals T -> bool; // ????????????
   fn T ( >= ) T -> bool;   //??????????????
   equals :: fn T, T -> bool;
   not_equals :: fn T, T -> bool;
}

class IComparable :: T has{
   equals(T) -> bool;
   greaterThan :: fn T, T -> bool;
   lessThan :: fn T, T -> bool;
}

class Supa :: T is IComparable & is IEqable & has{
   number :: int;
}

type List :: struct<T :: IComparable>{
   length :: int;
   data   :: *T;
}

type List :: struct<T :: IComparable>{
   length :: int;
   data   :: *T;
}


fn<T> push_back(l :: List<T>, x :: T){

}

list :: List<uint32>;
push_back(list, 32);

type Icomparable :: Iequals&Icompare;

type point_t :: struct{
   x, y :: int;
   equals     := fn (a, b :: point_t) :: bool => a.x == b.x && a.y == b.y;
   not_equals := fn (a, b :: point_t) :: bool => !equals();
}



//constants

const PI = 3.1415;
const PI :: f64 = 3.141596;

//not sure about this:
const DAYS_OF_THE_WEEK{
   MONDAY   ,
   TUESDAY  ,
   WEDNESDAY,
   THURSDAY ,
   FRIDAY   ,
   SATURDAY ,
   SUNDAY
};

//variable declaration

a, b :: int;
a, b :: i32;
a, b :: u8;
a, b :: float;
a, b :: double;
a, b :: f32;
a, b :: f64;
a, b :: bool;

c :: int = 32;
d := 32;
e, f, g :: float = 0.5, 0.6 ,0.7; 

h, i := 4.3, 4.5;

x, i := 43  //not allowed...
x, y, z := 43, 44, 45, 46 //not good.


//for loop

for i::int = 0; i<57; i++ {

}

for a:=0, b:=100; a>b; a++, b-- {

}

i::int = 1;
for ; i<5; i++ {

}

for ;; {
   
}

//while loop

while i != 0 {

}

while cond(f) {

}

//do while

do{

}while cond(f);

do{

}while i>5;

//if

if x>6 {

}

if x<5 {

}else {

}


if x>100 {

}else if x<144 {

}







































type bin_operator :: fn int, int -> int

type point_t :: struct{
   x, y :: int32; 
}

type point :: struct<T>{
   x, y :: T;
}

type number :: int32 | int64 | uint32 | uint64; 

fn<T1, T2> sum(x, y :: T1) -> x+y :: T2;

type bool :: uint8;

const true  = 1;
const false = 0;

const func = fn ( x, y :: int) :: int{ 
   return x+y; 
};
const func = fn (x, y) -> x+y;

fn abs(x :: int) :: int{
   return x >= 0 ? x : -x;
};

fn main() {
   arr := [0, 2, 4];
   i, j    :: int = 0, len(arr);
   point_1 :: math.Point;
   
   while i<j {
      arr[i] = i-j;
      arr[i] = math.cos(arr[j]);
      ++i;
   }

   if arr[0] && ptr {

   }  

   for x : arr {
      std->println(x);
   }

}