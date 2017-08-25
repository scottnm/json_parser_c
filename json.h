#ifndef __JSON_H__
#define __JSON_H__

#include <unordered_map>
#include <vector>

enum class vtype { NUM, STR, OBJ, ARR };

struct value;
typedef std::unordered_map<const char*, struct value> json_obj;
typedef std::vector<struct value> json_arr;

struct value
{
    enum vtype type;
    union
    {
        double num;
        char*  str;
        json_obj* obj;
        json_arr* arr;
    };
};
typedef struct value value;

#endif // __JSON_H__
