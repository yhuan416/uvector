#include "uvector.h"

#define GROW_STEP   (4)

void *uvector_malloc(uint32_t size)
{
    return (void *)malloc(size);
}

void uvector_free(void *ptr)
{
    free(ptr);
}

uvector uvector_make(uvector_key key, uvector_compare compare)
{
    uvector v = {
        .max = 0,
        .top = 0,
        .free = 0,
        .data = NULL,
        .key = key,
        .compare = compare,
    };

    return v;
}

void uvector_clear(uvector *v)
{
    if (v == NULL) {
        return;
    }
    if (v->data == NULL) {
        return;
    }
    uvector_free(v->data);
    v->max = 0;
    v->top = 0;
    v->free = 0;
    v->data = NULL;
}

static int16_t uvector_alloc_element(uvector *v)
{
    int16_t i;

    // 当前有空闲的空间
    if (v->top < v->max) {
        return v->top++;
    }

    // 使用被释放的位置(从后往前找)
    for (i = v->top - (int16_t)1; i >= 0; --i) {
        if (v->data[i] == NULL) {
            
            // 被释放的个数-1
            v->free--;

            // 返回查找到的位置
            return i;
        }
    }

    // 需要扩容(每次扩容一定的步长(GROW_STEP))
    void **data = (void **)uvector_malloc(sizeof(void *) * (v->max + GROW_STEP));
    if (data == NULL) {
        return INVALID_INDEX;
    }

    // 原来有数据
    if (v->data != NULL) {
        // 拷贝原来的数据到新的空间
        memcpy(data, v->data, sizeof(void *) * v->max);
        // 释放原来的空间
        uvector_free(v->data);
    }

    // 使用新空间
    v->data = data;

    // 新空间的最大值
    v->max += GROW_STEP;

    // 返回新空间的第一个位置
    return v->top++;
}

int16_t uvector_add(uvector *v, void *element)
{
    int16_t index = 0;

    if (v == NULL || element == NULL) {
        return INVALID_INDEX;
    }

    // 分配一个空间给新的元素
    index = uvector_alloc_element(v);
    if (index == INVALID_INDEX) {
        return INVALID_INDEX;
    }

    v->data[index] = element;
    return index;
}

// 已被分配的数量(包括删除的)
int16_t uvector_size(uvector *v)
{
    if (v == NULL) {
        return INVALID_INDEX;
    }
    return v->top;
}

// 当前有效的数量
int16_t uvector_num(uvector *v)
{
    if (v == NULL) {
        return INVALID_INDEX;
    }
    return v->top - v->free;
}

// 获取指定位置的元素
void *uvector_at(uvector *v, int16_t index)
{
    if (v == NULL || index < 0 || index >= v->top) {
        return NULL;
    }
    return v->data[index];
}

// // 交换一个已经存在的元素的值
// void *uvector_swap(uvector *v, int16_t index, void *element)
// {
//     void *old = NULL;

//     if (v == NULL || index < 0 || index >= v->top) {
//         return NULL;
//     }

//     // 不允许交换一个空的元素
//     if (element == NULL) {
//         return NULL;
//     }

//     old = v->data[index];
//     v->data[index] = element;
//     return old;
// }

// int16_t uvector_find(uvector *v, void *element)
// {
//     int16_t i;

//     if (v == NULL || element == NULL) {
//         return INVALID_INDEX;
//     }

//     for (i = 0; i < v->top; ++i) {
//         if (v->data[i] == element) {
//             return i;
//         }
//     }

//     return INVALID_INDEX;
// }

// 查找元素(使用key查找)
int16_t uvector_find_by_key(uvector *v, void *key)
{
    int16_t i;

    if (v == NULL || key == NULL) {
        return INVALID_INDEX;
    }

    for (i = 0; i < v->top; ++i) {

        // 空的元素不参与查找
        if (v->data[i] == NULL) {
            continue;
        }

        // 对比key
        if (v->key(v->data[i]) == key) {
            return i;
        }
    }

    return INVALID_INDEX;
}
