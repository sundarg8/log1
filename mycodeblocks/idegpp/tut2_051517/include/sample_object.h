#ifndef SAMPLE_OBJECT_H
#define SAMPLE_OBJECT_H
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

//enum class obj_action_types { GET, CREATE , MODIFY , DELETE };

enum  action_t  { GET, CREATE , MODIFY , DELETE };

class sample_object
{
    public:

        sample_object();
        virtual ~sample_object();


        sample_object(const sample_object &obj) {
            intf_name = obj.intf_name;
            speed     = obj.speed;
            stats     = obj.stats;
            //cout << setw(9) << "Copy Constructor called for Sample_object" << endl;
            //display_obj();
            ++no_of_copy_ctors;
        }


        void set_obj_params(string val_name, int val_speed, int val_stats) {
            intf_name = val_name;
            speed = val_speed;
            stats = val_stats;
        }
        void display_obj() {
            cout << setw(10) << "Attr_name: " << intf_name
                            << "   Attr_speed: " << speed
                            << "   Attr_stats: " << stats <<  endl;
        }
        static int no_of_copy_ctors;

    protected:
    private:
        string  intf_name;
        int     speed;
        int     stats;
};

#endif // SAMPLE_OBJECT_H
