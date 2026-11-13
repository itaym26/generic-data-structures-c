# Regression tests

From the repository root on Linux with a C compiler that supports AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
sh tests/run_tests.sh
```

The runner builds in a temporary directory and removes its binaries on exit. It checks BST empty-tree and deletion boundaries, hash-map lookup/removal/rehash/statistics/destructor callbacks, and the zero-capacity stack allocation path.

These are targeted regression tests, not complete verification of every API. In particular, the heap and BST headers currently both define `Comparator` with different function types, so including both headers in one translation unit remains unsupported pending an API naming decision.
