#include "p5_utils.h"
#include "u2_utils.h"
#include <sstream>
#include <list>
#include <deque>
#include <utility>
#include <map>
#include <set>
#include <string>



#if 1 ///f1
void u2_f1_test() {
    u2_test obj1(5);
    obj1.setName();
    obj1.display();
}
#endif

#if 1 ///f2 vector



void outputVector(const vector<int> &intArray) {   // const used here.
     for (auto& x: intArray)
        cout << x << " ";
    NL
}

void inputVector(vector<int> &myArray) {      // no const used here.
    for (size_t i=0; i < myArray.size(); i++)
        cin >> myArray[i];
}

void u2_f2_stdVector_test()  {
    vector<int> integers1(7);
    vector<int> integers2(10);

    integers1 = { 1, 2, 3, 4, 5, 6, 7};
    integers2 = { 11, 12 ,13, 14, 15 ,16, 17, 18, 19, 20, 21, 22};

    ctp("integers1")
    outputVector(integers1);
    ctp("integers2")
    outputVector(integers2);

    if (integers1 != integers2) ctp("both vectors are not equal")

    vector<int> integers3(integers1);
    ctp("integers3")
    outputVector(integers3);

    ctp("integers1")
    integers1[7]=8;             // no error returned , but value  not printed. for vector print
    ///integers1.at(7)=8;        // error for out-of-bounds.
    outputVector(integers1);

    ctp("integers1")
    integers1[0]=20.51;           ///assigning float value seems to work with truncated value stored..???
    integers1.at(0)=21.52;        /// this also worked.. ????
    outputVector(integers1);
}
#endif

#if 1 ///f3 pointers
void u2_f3_pointers_test() {
    int abc= 10;
    ctw(abc);
}

#endif

#if 1 //f4_generic progr

template <typename T>
void display_generic(T arr[], int size) {
    for (int i=0; i < size; i++)
        cout <<arr[i] << " ";
}

template <typename T>
T mymax(T &arg1, T &arg2 ) {
    if (arg1 > arg2) return arg1;
    else return arg2;
}


/// stringify both double on stream from same line
string sstream_stringify(double a , string s1) {
    stringstream stm;
    stm << s1 << ":" << a;
    //cout << s1 << ":" << a << endl; //this also seems to work.
    return stm.str();
}



template <typename T , typename U>
T mymax2(T arg1, U arg2) {
    return (arg1 > arg2) ? arg1: arg2;
}

template<class T, class U>
class CMap {
    vector<T> keys;
    vector<U> values;

    public:
        void insert(T key, U value) {
            keys.push_back(key);
            values.push_back(value);
        }
        void get(int n) {
            cout << keys[n] << ": " << values[n];
        }
};

void u2_f4_generic_prog_test(){   ///f4 generic prog
    int numbers[10]= { 1 ,2 ,3 ,4 ,5};
    display_generic(numbers, 5);
    string names[] = {"one", "two", "three"};
    display_generic(names, 3);

    double d1=2.2 , d2 = 2.4;
    cout << endl <<  mymax ( d1, d2) << endl;
    string s1 ="apple", s2 = "ball";
    cout <<  mymax(s1, s2) << endl;

    cout << sstream_stringify(d1, "Two");

    CMap<string, int> grades;
    grades.insert("Jones", 88);
    grades.insert("Smith", 90);
    NL; grades.get(0); cout << endl;  grades.get(1); cout << endl;


}
#endif // 1

#if 1           /// f5 seq. containers

void vector_container() {
    vector<int> numbers(4);
    ostream_iterator<int> output(cout, " "); ///ostream
    numbers.push_back(3); numbers.push_back(5);
    copy(numbers.begin(), numbers.end(), output);NL;
    ctw(numbers.size());
}

void list_container() {
    list<string> names;
    names.push_back("Mary"); names.push_back("Zach"); names.push_back("Elizabeth");
    list<string>::iterator iter;    print_iter(iter, names)
    ctw(names.front()); ctw(names.back());
    names.reverse();    print_iter(iter,names)
    names.sort(); print_iter(iter, names)

    ///push_front not available with vectors.
    names.push_front("Barry"); print_iter(iter,names)
    names.remove("Elizabeth");
    names.pop_front(); names.pop_back(); print_iter(iter,names)
    names.clear(); names.empty();
}

/// deque --double-ended queue..
/// queue -- push to end of line; pop from start of line
/// dequeue -- push and pop from both ends of the line
/// Vector --  optimized only for inserting at the end.

int find_in_deque(deque<string> d, string value) {
    for (int i=0 ; i< d.size(); i++)
        if (d.at(i) == value)
            return i;

    return -1;
}
void deque_container() {
    deque<string>  line;
    line.push_back("Cust 1");
    line.push_front("Cust 2");
    line.push_back("Cust 3");
    deque<string>::iterator iter; print_iter(iter,line);
    line.pop_back(); line.pop_front(); print_iter(iter,line);

    cout << find_in_deque(line, "Cust 1");
}

void u2_f5_seq_containers_test(){
    vector_container();
    list_container();
    deque_container();
}

#endif

#if 1           /// f6 assoc. container (key,value)
void myPairs() {
    pair<string, string> number1("Jones", "123");
    ctw(number1.first) ctw(number1.second)
}
void myMaps() {
    map<int, string> numbers;
    numbers[0x25] = "Jones"; numbers[0x21] = "Smith"; numbers[0x32] = "Green";
    ctw(numbers[0x25]);
    //numbers.erase(0x21);

    if(numbers.find(0x21) != numbers.end())     ctw(numbers[0x21])
    else                                        ctp("0x21 not found")

    numbers[0x22] = "Steve_SR";             /// Insert new key,value using subscript

    ///check if key already exists, before overwriting
    if(numbers.find(0x22) == numbers.end()) ///to avoid duplicates, existing key overwritten
        numbers[0x22] = "Steve_JR";         ///Subscripting  can change value ie overwrite

    numbers.insert(pair<int, string>(0x51, "Pam_SR"));
    numbers.insert(pair<int, string>(0x51, "Pam_JR")); ///dup ignored.

    /// sorted map
    map<int, string>::iterator iter; print_assoc_iter(iter, numbers)

}

void mySet_test() {
    set<string> mysets; size_t i = 0;

    /// Method1 ; fixed array size;; can't add new strings.
    array<string,6> mystrings_old = {"ccc" , "aaa", "zzzz" , "dd" ,"b", "dd"};
    ctw(mystrings_old[0]); ctw(mystrings_old[4]);
    /// mystrings_old[6]="nnnnn";   /// Runtime Error..

    /// Method 2 : Variable string array ..easy to expand vector array
    string name_arr[6] = {"ccc" , "aaa", "zzzz" , "dd" ,"b", "dd"};
    vector<string> mystrings(name_arr, name_arr+6);
    mystrings.push_back("nnnnnnnn");

    for (int i=0; i< mystrings.size(); i++) {
         mysets.insert(mystrings[i]);
    }
    ///Sorted set
    set<string>::iterator iter; print_iter(iter, mysets)

}
void myMultimap_test() {
    multimap<int, string> numbers;
    numbers.insert(pair<int, string>(0x51, "Pam"));
    numbers.insert(pair<int, string>(0x25, "Jones"));
    numbers.insert(pair<int, string>(0x22, "abc"));
    numbers.insert(pair<int, string>(0x25, "bob"));
    numbers.insert(pair<int, string>(0x31, "cat"));
    numbers.insert(pair<int, string>(0x41, "dog"));
    numbers.insert(pair<int, string>(0x25, "david"));
    ///numbers[0x55] = "new1";  ///ERROR

    ///SORTED multimap
    multimap<int,string>::iterator iter ; print_assoc_iter(iter, numbers)

    int search_no = 0x25;
    iter= numbers.find(search_no);
    multimap<int,string>::iterator last = numbers.upper_bound(search_no);

    for( ; ( (iter != numbers.end()) && (iter != last)) ; ++iter)
        cout << iter->first << " : " << iter->second << endl;

}

void u2_f6_associative_containers_test() {
    //myPairs();
    myMaps();
    //mySet_test();
    //myMultimap_test();
}

#endif

#if 1           /// f7 generic algos (can be used on any containers)
void u2_f7_generic_algos_test() {
    //back_inserter(vector1);
    //front_inserter(deque1);
    //vector<int> :: reverse_iterator riter= number.rbegin() to rend() ; riter++

}
#endif
