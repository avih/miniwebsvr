#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
using namespace std;

/**
 * A Grobally accessible statically allocated string for when there is no data
 */
extern string blank;

/**
 * A string->string name->value pair
 */
class variable {
public:
        variable(const string &p_name,const string &p_value) {
                name=p_name;
                value=p_value;
        }

        string name;
        string value;
};

/**
 * A very unpotimized & simple string->string hash implementation
 */
class hash {
public:
        const string gethash(const string &p_name) const {
                unsigned int indx;
                for (indx = 0; indx < vars.size(); ++indx) {
                        if (vars[indx].name == p_name) return vars[indx].value;
                }
                return blank;
        }

        void push(const string &p_name, const string &p_value) {
                vars.push_back(variable(p_name,p_value));
        }

private:
        vector<variable> vars;
};


#endif // HASH_H
