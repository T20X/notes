//variables
b:1+a:42 + k:90 //evaluates right to left
n:`int$2014.11.19 // dates integers - days since millenium!
n2:`long$12:00:00.0000000009 // long - timespan microseconds since midnight
type `2014.11.19
l:100 200 300;type l; 1h$0

l+42
dts:.z.D-til 300;


//lists
20#5 1 6 - makes a list containing 20 elements of 5, 1 6!
l:til 10
({x+y}/) l

l2:10 0N 10
a:f[1; 10]

l2:2 3
l[l2]

10 10 11  ? (1;10;1) //find



// functions x,y,z are implicit parameters
f:{x*y}
f[2;2]
f2:f:{[x;y;z] (x * y) + z }[2;2;]  //- partial function application
f2[10]
l: 2 2 2 2 
{x*x} l
({x+y}/) l

i:0
({[x;y] i::x*y;i}/) 10 10 2 1 3 // setting global variable inside funciton using ::

// ADVERBS
/ // over! loops over list and returns an aggregated value

(*/) 20#1
(*/) 2#1.4142135623730949
0 1 2 10 +/ 10 10 2
1 2 +/ 10 10 2
(+/) 10 10 2

\ // scan! loops over list and returns intermediate values!
(+\) 1 2 9 2

max 1 10 -1 // same as (|/)
(|/) 1 10 -9
(&/) 1 10 -9 // min

//adverbs
l: 1 1 1 1
each[count] l
ount each (10 20 30; 40 50)




// The adverb each-both ' modifies a dyadic function (operator, verb) to apply pairwise to corresponding list items
//("abc; "uv),'(`de; `xyz)
l4:(`abc; `uv),'(`de; `xyz)
each[,]  l4

`$"" sv string `abc`de

f:{`$"" sv string x}
f each l4


(1 1 2),'("222")


//strings
`$"**" sv (string `asdasd;string `asdsd) // scalar from vector strings
"|" sv (string `asdasd;string `asdsd)
` sv `asdasd`asdasdas`12222

c1:"asdasd"
-11h$c1 // to char string 
`$c1 // to char string 
string `asdasd //symbol to char

", " vs "spring, summer, autumn, winter" // vector from scalar!
` vs "abc\ndef\nghi"

`$"*" vs "abcd*ef*ghi"


colnname:`asdsa;
'(`) sv (colname;`invalid.colname)

"asd" /: ("asdas";"asdas";"asdas")
//`ssts (/:) `asd`asdasd`sasdas

-10h$89
null `
null " "

s1:`asdasdas;s2:`asdasdas;
l3:(s1;s2)
":" sv (s1;s2)

type "beeblebrox"[0 7 8]
type "b"
v2:"ab"
v2[0]
v3:`asdasdasd
c1:`asdasdasd
(string c1)[1]




test:([] price:`float$());
delete from test

test3:([price:`float$()] size:`float$());`test3 insert (1.4;1.3);
-3!test3

f:
{
:4
}

f{}

type a
a
test

a1:12
1+a1:23
a1

//x:12
a2:{[x] x*x}
a2[3]

l:1 2 3 4
a2 l

0 a2/ l

10 (|/) l / largest of
(&/) l / smallest of

F0:1 1
F0, sum -2#F0

10 {x, sum -2#x}/ 1 1


l3:1 {x*y}/ 2 3 2
l3

mins 20 10 40 30 5


({x+y}/) 1 2 3 4 1
(+/) 1 2 3 4  1
0 +/ 1 2 3 4  1
prd 1 2 3 2

l:2 2 3
(*/) 2#l

2#1.4142135623730949 / returns copies
{x,sum -2#x}[1 1]
{x,sum -2#x}[1 1 3]

l:1 2 3 1 1 1 
type 1#l
type 1
f:{x+y+z}
f l

l:0 0 0 1 1 1 1 1 1 1 1; 
c1:1;
{[t] m:4*c1;r::{sum x}[(4*c1)#t];c1+:1;r} l

l:0 0 0 1 1 1 1 1 1 1 1; 
type enlist 1
type 1
l?0
l=1
where l=1
2_l

//
// dict
//

d:`a`b`c!10 20 30
flip d
type d
d[`c]

dc:`c1`c2`c3!(10 20 30; 1 2.2 0n; 1 1 1)
dc[`c1;0]
dc[;2]
type dc[`c2;2]
flip dc

//
// tables
//

e:1 + til 6
e?5
5?e

t1:([] c1:10 20 30; c2:1.1 2.2 3.3)
type t1
type flip t1

//
// select
//

t:([] c1:1000+til 6; c2:`a`b`c`a`b`a; c3:10*1+til 6)
type t

r:select min c3, sum c3, N:count i by o:c2=`a from t
t[1b;`c3]

r:select count c2 by ovrund:c3<=40 from t
update c3:30*c3 from `t where c2=`a

1 2 3 wavg 50 60 70

.z.t+100
.z.p

5 xbar til 15

5.1 xbar 5.12 5.14 11.111

//
// datatypes
//
1?

-1?0Ng
a:"aaaaa"
type a

d:`int$01:03.100 / date to time
d:2000.01.01 / date to time

`date$.z.p
`timespan$.z.p
m:`month$.z.p
m.mm
m.dd

`int$m
s:`second$.z.p
`int$`second$.z.p
`second$.z.p

`hh$.z.p
`ss$.z.p
`mm$.z.p
`dd$.z.p

`date$2014.11.22D17:43:40.123456789



t:([] name:`char$())
`t insert (" ")
select count i by name from t where name = " "

t:([] name:`char$())
`t insert (" ")

select count i by name from t where null name


`asd~`asdsad
-3!t

"2"

L1:10 0N 30
L2:100 200 0N
L1 ^ L2
L1[0 2]



t:([] name:`symbol$(); iq:`int$())

dc:`c1`c2`c3!(10 20 30; 1 2.2 0n; 1 1 1)
d2:`c1`c2`c3!(10 20 30; 11 11 11; 11 11 11)
d2:flip d2
d2,:`c1`c2`c3!11 21 31
d2,:(10;11;12)
d2,:(10;11;13)
d2,:(10;12;11)
d2:`c1 xkey d2
d2:2#d2
insert (2 1 3; 5 1 3; 1 1 1)
`d2 upsert ([] c1:enlist 11; c2:enlist 50; c3:enlist 50)
`d2 upsert (11;1;1)

//
// select & exec
//
select [1 2] from d2 where c3=11
select c1, last c3 from d2
t:([] f:1.1 2.2 3.3; s:("abc";"d";"ef"))
select from t where s~\:"ef"
select name, color, max weight by city from p
select from p where weight=(max;weight) fby city

select from p where weight=(max;weight) fby ([] city;color)

p[`p1]

ungroup `city xgroup p

t:([] name:`a`b`c`d`e`u; state:`NY`FL`OH`NY`HI`HI)
d:exec name, distinct state from t
select name from t where state in `NY`HI

12#`a

(count p)?`a`b

update mine:(count p)#`a from `p

//
// joins
//

t:([] k:1 2 3 4; c:10 20 30 40)
kt:([k1:2 3 4 5]; v:2.2 3.3 4.4 5.5)
select c, kt[([] k1:k); `v] from t / column lookup

select c,v from  (t lj  `k xkey select k:k1,v from kt)


t1:([] k:1 2 3 4; c:10 20 30 40; d: 1 1 1 1)
t2:([] k:2 2 3 4 5; c:200 222 300 400 500; v:2.2 22.22 3.3 4.4 5.5)
t1 ij `k xkey t2
ej[`k;t1;t2]


t1:([] c1:`a`b`c; c2: 10 20 30)
t2:([] c1:`x`y`b`a; c3:8.8 9.9 1.1 1.2)
t1 uj t2


select by sym, 2 xbar ti from q

t:([] ti:10:01:01 10:01:03 10:01:04;sym:`msft`ibm`ge;qty:100 200 150)
q:([] ti:10:01:00 10:01:01 10:01:01 10:01:02 10:01:03 10:01:03;sym:`ibm`msft`msft`ibm`ge`ge;px:100 99 102 98 101 11)
aj[`sym`ti;t;q]
t asof `sym`ti!(`ibm;10:01:03)

promotions:([] name:`nuba`devi`nuba`devi`nuba;
    dt:2000.01.01 2005.02.14 2010.02.01 2012.08.12 2015.11.01;
    title:`associate`analyst`director`cfo`retired)

promotions asof `name`dt!(`nuba; 2009.07.04)


w:-2 1+\:t `time
c:`sym`time





