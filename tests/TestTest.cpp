#include <cassert>
#include <criterion/criterion.h>

Test(simple, test1) {
  cr_assert(1 != 100);
}

Test(simple, test2) {
  cr_assert(2 != 100);
}

Test(simple, test3) {
  cr_assert(3 != 100);
}

Test(simple, test4) {
  cr_assert(4 != 100);
}

Test(simple, test5) {
  cr_assert(5 != 100);
}

Test(simple, test6) {
  cr_assert(6 != 100);
}

