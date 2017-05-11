#include "p5_utils.h"
#include "p4_classes.h"

int f2() {lsf  //data types

    ct("inside my1");
    int i1{7};
    double d1{2.2};
    ct(d1*2);
    ct(i1);

    auto b = false;
    ct(b)

    int v[] = {1,3,5,8,12,14};
    for (auto& x:v)
        ct(x)

    double* pd = nullptr;
    if (!pd)
        ct("we got a null ptr")
    return 0;

lef}

#if 1   //my1Vector Defns

void fn3_ex_vector() {

    my1Vector v(6);
    v[1] = 3.146;
    ct(v.size())
    ct(v[0])  ct(v[1])  ct(v[2])  ct(v[3])

    try {
        v[8] = 1.12;
        ct(v[8])
    } catch (out_of_range) {
        ctp("!!! v8 ..access ..My except hdlr for out_of_range")
    }
}

my1Vector::my1Vector(int s)
    : elem {new double[s]},sz {s}   {}

my1Vector::~my1Vector()  { delete []elem;}

int my1Vector::size()  { return sz ;}

double& my1Vector:: operator[](int i) {
    if (i < 0 || size() <=i) throw out_of_range{"myVector::operator[]"};
    return elem[i] ;
}


#endif

#if 1   //my2Vector Defns

my2Vector local_fn1_move_constructor () {
    my2Vector toBeDestroyed = { 10, 20, 30, 40 };
    return std::move(toBeDestroyed);
}


void fn4_ex_my2vector() {
    my2Vector v = { 1, 4, 7 , 9, 11,17};   //initializer constructor
    ct(v.size())  ct(v[0])  ct(v[1])  NL; NL;

    my2Vector cp1_v(v);
    ct(cp1_v.size())  ct(cp1_v[0])  ct(cp1_v[1]) NL; NL;

    my2Vector cp2_v = { 2 , 3, 4 };
    ct(cp2_v.size())  ct(cp2_v[0])  ct(cp2_v[1]) NL; NL;
    cp2_v=v;
    ct(cp2_v.size())  ct(cp2_v[0])  ct(cp2_v[1])  NL; NL;


    //The move constructor is much faster than a copy constructor
    //because it doesn’t allocate memory nor does it copy memory buffers.

    my2Vector mv_v = local_fn1_move_constructor();
    ct(mv_v.size())  ct(mv_v[0])  ct(mv_v[1])  NL; NL;

    my2Vector mv2_v (std::move (v));
    ct(mv2_v.size())  ct(mv2_v[0])  ct(mv2_v[1])  NL; NL;

}


my2Vector::my2Vector(std::initializer_list<double> lst)     // initialize with a list
    :elem{new double[lst.size()]},
     sz{static_cast<int>(lst.size())} {

    copy(lst.begin(),lst.end(),elem);           // copy from lst into elem
}

my2Vector::~my2Vector()  { delete []elem;}

int my2Vector::size()  { return sz ;}

double& my2Vector:: operator[](int i) {
    if (i < 0 || size() <=i) throw out_of_range{"myVector::operator[]"};
    return elem[i] ;
}

my2Vector::my2Vector(const my2Vector& a)      // copy constructor
    :elem{new double[a.sz]},                  // allocate space for elements
    sz{a.sz} {

    for (int i=0; i!=sz; ++i)                 // copy elements
          elem[i] = a.elem[i];
}

my2Vector& my2Vector::operator=(const my2Vector& a) {  // copy assignment

    double* p = new double[a.sz];
    for (int i=0; i!=a.sz; ++i)
          p[i] = a.elem[i];
    delete[] elem;                           // delete old elements
    elem = p;
    sz = a.sz;
    return *this;
}



my2Vector::my2Vector(my2Vector&& a)
    :elem{a.elem},                           //"grab the elements" from a
    sz{a.sz} {
    a.elem = nullptr;                        // now a has no elements
    a.sz = 0;
}




#endif // 1

#if 1   //my3_T_Vector Defns

void write(my3_T_Vector<string>& vs) {      // Vector of some strings
     for (int i = 0; i!=vs.size(); ++i)
        cout << vs[i] << '\n';
}

void write2(my3_T_Vector<string>& vs)     // Vector of some strings
{
     for (auto& s : vs)
           cout << s << '\n';
}

void fn5_ex_my3_T_vector() {
    my3_T_Vector<char> vc(200);       // vector of 200 characters
    my3_T_Vector<string> vs(17);      // vector of 17 strings

    vs[0] = "string one";
    vs[5] = "string five";
    vs[16] = "string sixteen";


    //write(vs);

    write2(vs);

    /// NOT WORKING
    //my3_T_Vector<list<int>> vli(45);  // vector of 45 lists of integers

}

template<typename T>
my3_T_Vector<T>::my3_T_Vector(int s) {
    elem = new T[s];
    sz = s;
}

template<typename T>
const T& my3_T_Vector<T>::operator[](int i) const
{
    return elem[i];
}

template<typename T>
T&  my3_T_Vector<T>:: operator[](int i) {
    if (i < 0 || size() <=i) throw out_of_range{"myVector::operator[]"};
    return elem[i] ;
}

template<typename T>
T* my3_T_Vector<T>::begin() {
    return this->sz ? &elem[0]: nullptr;           // pointer to first element
}

template<typename T>
T* my3_T_Vector<T>::end() {
    return begin()+sz;                   // pointer to one-past-last element
}

/// Looks like begin, end with zero params is used range-for- in write2
/*
template<typename T>
T* my3_T_Vector<T>::begin(my3_T_Vector<T>& x) {
    return x.size() ? &x[0]: nullptr;           // pointer to first element
}

template<typename T>
T* my3_T_Vector<T>::end(my3_T_Vector<T>& x) {
    return begin(x)+x.size();                   // pointer to one-past-last element
}
*/

#endif




