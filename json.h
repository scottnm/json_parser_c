#ifndef __JSON_H__
#define __JSON_H__

enum class vtype { NUM, STR };//, obj, arr };

struct value
{
    enum vtype type;
    union
    {
        double num;
        char*  str;
        // key-value hastable
        // value-vec
    };
};
typedef struct value value;

#endif // __JSON_H__
/*
obj = hashtable of key-value pairs

typedef struct
{
    value* b;
    value* top;
    value* e;
} val_vec;



enum vtype {int, str, obj, arr}
struct value
{
    vtype t;
    union
    {
        double d;
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
