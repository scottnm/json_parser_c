#ifndef __JSON_H__
#define __JSON_H__

#include <unordered_map>
#include <vector>

enum class vtype { NUM, STR, OBJ, ARR };

struct json_value;
typedef std::unordered_map<const char*, struct json_value> json_obj;
typedef std::vector<struct json_value> json_arr;

struct json_value
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
typedef struct json_value json_value;

#endif // __JSON_H__
