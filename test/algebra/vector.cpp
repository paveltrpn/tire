
#include <cmath>

#include "algebra/Vector.h"

#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE test_unit

#define BOOST_NO_CXX98_FUNCTION_BASE
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(algebra)

BOOST_AUTO_TEST_CASE(case_vector_construct_and_arithmetic, *utf::tolerance(0.00001)) {
    constexpr size_t __SZ = 5;
    tire::vector<float, __SZ> one{ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
    tire::vector<float, __SZ> two{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    tire::vector<float, __SZ> three{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    for (size_t i = 0; i < __SZ; ++i) {
        BOOST_TEST(two[i] == 0.0f);
        BOOST_TEST(three[i] == 1.0f);
    }

    tire::vector<float, __SZ> four = three;
    four = two + three;

    for (size_t i = 0; i < __SZ; ++i) {
        BOOST_TEST(four[i] == 1.0f);
    }

    // toy::algebra::vector<int, __SZ> intv{ 1, 1, 1, 1, 1 };
    // auto l = intv.lenght();
    // three = one + two;
}

BOOST_AUTO_TEST_CASE(case_vector2_construct_and_arithmetic, *utf::tolerance(0.00001)) {
    tire::vector2f one{ 0.0f, 1.0f };

    BOOST_TEST(one.x() == 0.0f);
    BOOST_TEST(one.y() == 1.0f);

    tire::vector2f two{ 1.0f, 1.0f };
    two.sum(one);

    BOOST_TEST(two.x() == 1.0f);
    BOOST_TEST(two.y() == 2.0f);

    tire::vector2f three{ 0.0f, 0.0f };
    three = one;

    BOOST_TEST(three.x() == one.x());
    BOOST_TEST(three.y() == one.y());

    auto lenght = two.sqLenght();

    BOOST_TEST(lenght == 5.0f);

    tire::vector2f four = one + two + three;

    BOOST_TEST(four.x() == 1.0f);
    BOOST_TEST(four.y() == 4.0f);

    // auto cr = two.cross(one);
}

BOOST_AUTO_TEST_CASE(case_vector3_construct_and_arithmetic, *utf::tolerance(0.00001)) {
    tire::vector3f one{ 0.0f, 1.0f, 2.0f };

    BOOST_TEST(one.x() == 0.0f);
    BOOST_TEST(one.y() == 1.0f);
    BOOST_TEST(one.z() == 2.0f);

    tire::vector3f two{ 1.0f, 1.0f, 1.0f };
    two.sum(one);

    BOOST_TEST(two.x() == 1.0f);
    BOOST_TEST(two.y() == 2.0f);
    BOOST_TEST(two.z() == 3.0f);

    tire::vector3f three{ 0.0f, 0.0f, 0.0f };
    three = one;

    BOOST_TEST(three.x() == one.x());
    BOOST_TEST(three.y() == one.y());
    BOOST_TEST(three.z() == one.z());

    auto lenght = three.sqLenght();

    BOOST_TEST(lenght == 5.0f);

    tire::vector3f four = one + two + three;

    BOOST_TEST(four.x() == 1.0f);
    BOOST_TEST(four.y() == 4.0f);
    BOOST_TEST(four.z() == 7.0f);

    // auto cr = two.cross(one);
}

BOOST_AUTO_TEST_SUITE_END()
