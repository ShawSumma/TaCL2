#include <tach.h>

void tach_free_object(tach_object *obj) {
    obj->refc --;
    if (obj->refc != 0) {
        return;
    }
    switch (obj->type) {
        case tach_object_string: {
            free(obj->value.string.str);
            break;
        }
        case tach_object_vector: {
            tach_free_vector(obj->value.vector);
            break;
        }
        case tach_object_point: {
            for (uint32_t i = 0; i < obj->value.point.argc; i++) {
                tach_free_object(obj->value.point.args[i]);
            }
            if (obj->value.point.args != NULL) {
                free(obj->value.point.args);
            }
            break;
        }
        case tach_object_number: {
            tach_free_number(obj->value.number);
            break;
        }
        case tach_object_table: {
            tach_free_table(obj->value.table);
            break;
        }
        default: {
            break;
        }
    }
    free(obj);
}

void tach_free_number(tach_number *num) {
    tach_number_clear(num);
    free(num);
}

void tach_free_table(tach_table *table) {
    if (table->key != NULL) {
        tach_free_object(table->key);
        tach_free_object(table->value);
    }
    if (table->left != NULL) {
        tach_free_table(table->left);
    }
    if (table->right != NULL) {
        tach_free_table(table->right);
    }
    free(table);
}

