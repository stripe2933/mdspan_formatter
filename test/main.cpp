#include "tests.hpp"

int main(int argc, char **argv) {
    mdspan_formatter_test::std_formatter::layout_right_test();
    mdspan_formatter_test::std_formatter::layout_left_test();
    mdspan_formatter_test::fmt_formatter::layout_right_test();
    mdspan_formatter_test::fmt_formatter::layout_left_test();
}