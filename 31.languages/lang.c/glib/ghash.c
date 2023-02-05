#include <glib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    GHashTable *table = g_hash_table_new (g_str_hash,  g_str_equal);
    if (!table) {
        fprintf(stderr, "Error: table alloc failed");
        exit(EXIT_FAILURE);
    }

    g_hash_table_insert(table, "John", 1000);
    g_hash_table_insert(table, "Jane", 1000);
    printf("contains John %d\n", g_hash_table_contains(table, "Mary"));

    gpointer found = g_hash_table_lookup(table, "John");
    if (found) {
        fprintf(stdout, "John is %d\n", found);
    }
    return 0;
}