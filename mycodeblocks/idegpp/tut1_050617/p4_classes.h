int fn_print_gpp_version();
int f2();
void fn3_ex_vector();
void fn4_ex_my2vector();
void fn5_ex_my3_T_vector();


class my1Vector {
    public:
        my1Vector(int s);
        ~my1Vector();
        double& operator[](int i);
        int size();
    private:
        double *elem;
        int sz;
};

class my2Vector {
    public:
        my2Vector(std::initializer_list<double> lst);
        ~my2Vector();
        double& operator[](int i);
        int size();
        my2Vector(const my2Vector&a);
        my2Vector& operator=(const my2Vector& a);
        my2Vector(my2Vector&& a);

    private:
        double *elem;
        int sz;
};

template<typename T>
class my3_T_Vector {
private:
     T* elem;  // elem points to an array of sz elements of type T
     int sz;
public:
     my3_T_Vector(int s);                   // constructor: establish invariant, acquire resources
     ~my3_T_Vector() { delete[] elem; }     // destructor: release resources
      //... copy and move operations ...
     T& operator[](int i);
     const T& operator[](int i) const;
     int size() const { return sz; };
     T* begin();
     T* end();

     T* begin(my3_T_Vector<T>& x);
     T* end(my3_T_Vector<T>& x);

};

