#ifndef __JSON_H__
#define __JSON_H__

enum vtype { V_NUM, V_STR };//, obj, arr };

struct value
{
    enum vtype type;
    union
    {
        double num;
        char*  str;
        // kvpair* obj;
        // struct value* arr;
    };
};
typedef struct value value;

#endif // __JSON_H__
/*
obj = collection of key-value pairs

enum vtype {int, str, obj, arr}
struct value
{
    vtype t;
    union
    {
        int i;
        char* str;
        kvpair* obj; // arr
        struct value* arr; // arr
    };
};

struct kvpair
{
    char* k;
    struct value v;
};

arr = collection of value
struct value* vs;


execution path

main
    read loop
    when you encounter control element '{'
        launch into process_json_obj
    encounter anything else?
        error out: improper syntax/invalid character
*/
