#define _CRT_SECURE_NO_WARNINGS
#include "MetDat.h"
#include <string.h>

void MD_SetInt(MD* md, const char* key, int value) {
    for (int i = 0; i < md->tag_amt; ++i) {
        if (strcmp(md->tags[i].key, key) == 0) {
            md->tags[i].type = TAG_INT;
            md->tags[i].int_val = value;
            return;
        }
    }
    if (md->tag_amt < MAX_MDAT_TAGS) {
        strncpy(md->tags[md->tag_amt].key, key, sizeof(md->tags[md->tag_amt].key));
        md->tags[md->tag_amt].type = TAG_INT;
        md->tags[md->tag_amt].int_val = value;
        md->tag_amt++;
    }
}

void MD_SetString(MD* nbt, const char* key, const char* value) {
    for (int i = 0; i < nbt->tag_amt; ++i) {
        if (strcmp(nbt->tags[i].key, key) == 0) {
            nbt->tags[i].type = TAG_STRING;
            strncpy(nbt->tags[i].string_val, value, MAX_STRING_LENGTH);
            return;
        }
    }
    if (nbt->tag_amt < MAX_MDAT_TAGS) {
        strncpy(nbt->tags[nbt->tag_amt].key, key, sizeof(nbt->tags[nbt->tag_amt].key));
        nbt->tags[nbt->tag_amt].type = TAG_STRING;
        strncpy(nbt->tags[nbt->tag_amt].string_val, value, MAX_STRING_LENGTH);
        nbt->tag_amt++;
    }
}

int MD_GetInt(MD* nbt, const char* key, int* out) {
    for (int i = 0; i < nbt->tag_amt; ++i) {
        if (strcmp(nbt->tags[i].key, key) == 0 && nbt->tags[i].type == TAG_INT) {
            *out = nbt->tags[i].int_val;
            return 1;
        }
    }
    return 0;
}

int MD_GetString(MD* nbt, const char* key, char* out, int max_len) {
    for (int i = 0; i < nbt->tag_amt; ++i) {
        if (strcmp(nbt->tags[i].key, key) == 0 && nbt->tags[i].type == TAG_STRING) {
            strncpy(out, nbt->tags[i].string_val, max_len);
            return 1;
        }
    }
    return 0;
}

int MD_SetDouble(MD* md, const char* key, double val) {
    for (int i = 0; i < md->tag_amt; ++i) {
        if (strcmp(md->tags[i].key, key) == 0) {
            md->tags[i].type = TAG_DOUBLE;
            md->tags[i].double_val = val;
            return 1;
        }
    }
    if (md->tag_amt >= MAX_MDAT_TAGS) return 0;
    strcpy(md->tags[md->tag_amt].key, key);
    md->tags[md->tag_amt].type = TAG_DOUBLE;
    md->tags[md->tag_amt].double_val = val;
    md->tag_amt++;
    return 1;
}

int MD_GetDouble(MD* md, const char* key, double* out_val) {
    for (int i = 0; i < md->tag_amt; ++i) {
        if (strcmp(md->tags[i].key, key) == 0 &&
            md->tags[i].type == TAG_DOUBLE) {
            *out_val = md->tags[i].double_val;
            return 1;
        }
    }
    return 0;
}