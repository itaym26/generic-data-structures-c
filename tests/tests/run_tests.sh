#!/bin/sh
# Run from any directory. Requires a C compiler; enables ASan and UBSan.
set -eu
cd "$(dirname "$0")/.."
BUILD=$(mktemp -d)
trap 'rm -rf "$BUILD"' EXIT HUP INT TERM
CC=${CC:-cc}
FLAGS='-std=c11 -Wall -Wextra -Werror -g -fsanitize=address,undefined -fno-omit-frame-pointer'
$CC $FLAGS -I. tests/bst_test.c gen_BST/gen.bst.c -o "$BUILD/bst"
$CC $FLAGS -I. tests/hash_test.c gen_hash/gen_hash.c -o "$BUILD/hash"
$CC $FLAGS -I. tests/stack_zero_test.c gen_stack/stack.c gen_vector/gen_vector.c -o "$BUILD/stack"
"$BUILD/bst"
"$BUILD/hash"
"$BUILD/stack"
printf '%s\n' 'All regression tests passed.'
