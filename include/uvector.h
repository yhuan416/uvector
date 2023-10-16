#ifndef _UVECTOR_H_
#define _UVECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INVALID_INDEX (-1)
typedef void *(*uvector_key)(const void *element);
typedef int (*uvector_compare)(const void *element1, const void *element2);

typedef struct uvector
{
    // 数据域总大小
    int16_t max;

    // 目前被分配到的位置
    int16_t top;

    // 已被删除的数量
    int16_t free;

    // 数据域
    void **data;

    // 计算key的函数
    uvector_key key;

    // 比较函数
    uvector_compare compare;
} uvector;

uvector uvector_make(uvector_key key, uvector_compare compare);

void uvector_clear(uvector *v);

int16_t uvector_add(uvector *v, void *element);

// 已被分配的数量(包括删除的)
int16_t uvector_size(uvector *v);

// 当前有效的数量
int16_t uvector_num(uvector *v);

void *uvector_at(uvector *v, int16_t index);

// 交换一个已经存在的元素的值
// void *uvector_swap(uvector *v, int16_t index, void *element);

// 查找元素(如果存在key, 则改为使用key查找, 否则直接匹配元素的值)
int16_t uvector_find(uvector *v, void *element);

// 查找元素(使用key查找)
int16_t uvector_find_by_key(uvector *v, void *key);

// 删除元素
void *uvector_remove(uvector *v, int16_t index);

#endif // !_UVECTOR_H_