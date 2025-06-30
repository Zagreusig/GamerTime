#pragma once
#ifndef METDAT_H
#define METDAT_H

#include <stdint.h>
#include "cJSON.h"

#define MAX_MDAT_TAGS 16
#define MAX_STRING_LENGTH 128

typedef enum TagType{
    TAG_INT,
    TAG_STRING,
    TAG_DOUBLE
} TagType;

typedef struct Meta_Data_Tag_struct{
    char key[32];
    TagType type;
    union {
        int int_val;
        double double_val;
        char string_val[MAX_STRING_LENGTH];
    };
}MD_Tag;

typedef struct Meta_Data_struct{
    MD_Tag tags[MAX_MDAT_TAGS];
    int tag_amt;
} MD;

void MD_SetInt(MD* md, const char* key, int val);
void MD_SetString(MD* md, const char* key, const char* val);
int MD_SetDouble(MD* md, const char* key, double val);
int MD_GetDouble(MD* md, const char* key, double* out_val);
int MD_GetInt(MD* md, const char* key, int* out);
int MD_GetString(MD* md, const char* key, char* out, int max_len);

#endif
