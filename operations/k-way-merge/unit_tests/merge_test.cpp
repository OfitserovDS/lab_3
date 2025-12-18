#include <gtest/gtest.h>
#include <vector>
#include "../KwayMerge.h"
#include <algorithm>
#include <random>
#include <numeric>

class MergeTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty_vector = {};
        single_element = {42};
        sorted_vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        reverse_sorted = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        random_vector = {5, 2, 9, 1, 5, 6, 3, 8, 4, 7};
        with_duplicates = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        negative_numbers = {-5, 3, -1, 0, -3, 2, -2, 1, -4};
        large_vector.resize(1000);
        std::iota(large_vector.rbegin(), large_vector.rend(), 1);
        double_vector = {3.14, 2.71, 1.41, 1.61, 0.0, -1.0, 2.0};
    }

    void TearDown() override {
    }
    
    std::vector<int> empty_vector;
    std::vector<int> single_element;
    std::vector<int> sorted_vector;
    std::vector<int> reverse_sorted;
    std::vector<int> random_vector;
    std::vector<int> with_duplicates;
    std::vector<int> negative_numbers;
    std::vector<int> large_vector;
    std::vector<double> double_vector;
};
// 1: пустой массив
TEST_F(MergeTest, EmptyArray) {
    std::vector<int> result = KwayMergeSort(empty_vector);
    EXPECT_EQ(result.size(), 0);
}

// 2: массив из одного элемента
TEST_F(MergeTest, SingleElement) {
    std::vector<int> result = KwayMergeSort(single_element);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

// 3: уже отсортированный массив
TEST_F(MergeTest, AlreadySorted) {
    std::vector<int> result = KwayMergeSort(sorted_vector);
    EXPECT_EQ(result, sorted_vector);
}

// 4: обратно отсортированный массив
TEST_F(MergeTest, ReverseSorted) {
    std::vector<int> result = KwayMergeSort(reverse_sorted);
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(result, expected);
}

// 5: случайный массив
TEST_F(MergeTest, RandomArray) {
    std::vector<int> result = KwayMergeSort(random_vector);
    
    for (size_t i = 1; i < result.size(); i++) {
        EXPECT_LE(result[i-1], result[i]);
    }
    
    std::sort(random_vector.begin(), random_vector.end());
    EXPECT_EQ(result, random_vector);
}

// 6: массив с дубликатами
TEST_F(MergeTest, ArrayWithDuplicates) {
    std::vector<int> result = KwayMergeSort(with_duplicates);
    
    for (size_t i = 1; i < result.size(); i++) {
        EXPECT_LE(result[i-1], result[i]);
    }
    
    std::sort(with_duplicates.begin(), with_duplicates.end());
    EXPECT_EQ(result, with_duplicates);
}

// 7: массив с отрицательными числами
TEST_F(MergeTest, NegativeNumbers) {
    std::vector<int> result = KwayMergeSort(negative_numbers);
    
    std::sort(negative_numbers.begin(), negative_numbers.end());
    EXPECT_EQ(result, negative_numbers);
}

// 8: большой массив
TEST_F(MergeTest, LargeArray) {
    std::vector<int> result = KwayMergeSort(large_vector, 8, 64);
    
    for (size_t i = 1; i < result.size(); i++) {
        EXPECT_LE(result[i-1], result[i]);
    }
    
    EXPECT_EQ(result.size(), 1000);
}

// 9: различные значения параметра k
TEST_F(MergeTest, DifferentKValues) {
    std::vector<int> result_k2 = KwayMergeSort(random_vector, 2);
    std::vector<int> result_k4 = KwayMergeSort(random_vector, 6);
    std::vector<int> result_k10 = KwayMergeSort(random_vector, 10);
    
    std::vector<int> expected = random_vector;
    std::sort(expected.begin(), expected.end());
    
    EXPECT_EQ(result_k2, expected);
    EXPECT_EQ(result_k4, expected);
    EXPECT_EQ(result_k10, expected);
}

// 10: различные размеры блоков
TEST_F(MergeTest, DifferentBlockSizes) {
    std::vector<int> result_block1 = KwayMergeSort(random_vector, 4, 1);
    std::vector<int> result_block2 = KwayMergeSort(random_vector, 4, 2);
    std::vector<int> result_block4 = KwayMergeSort(random_vector, 4, 4);
    std::vector<int> result_block10 = KwayMergeSort(random_vector, 4, 10);
    
    std::vector<int> expected = random_vector;
    std::sort(expected.begin(), expected.end());
    
    EXPECT_EQ(result_block1, expected);
    EXPECT_EQ(result_block2, expected);
    EXPECT_EQ(result_block4, expected);
    EXPECT_EQ(result_block10, expected);
}

// 11: типы с плавающей точкой
TEST_F(MergeTest, FloatingPointTypes) {
    std::vector<double> result = KwayMergeSort(double_vector);
    
    for (size_t i = 1; i < result.size(); i++) {
        EXPECT_LE(result[i-1], result[i]);
    }
}

// 12: граничные значения параметров
TEST_F(MergeTest, EdgeCaseParameters) {
    std::vector<int> result_k1 = KwayMergeSort(random_vector, 1, 2);
    std::vector<int> result_block0 = KwayMergeSort(random_vector, 4, 0);
    
    std::vector<int> expected = random_vector;
    std::sort(expected.begin(), expected.end());
    
    EXPECT_EQ(result_k1, expected);
    EXPECT_EQ(result_block0, expected);
}

// 13: сравнение с разными параметрами
TEST_F(MergeTest, PerformanceComparison) {
    auto result1 = KwayMergeSort(large_vector, 2, 10);
    auto result2 = KwayMergeSort(large_vector, 6, 10);
    auto result3 = KwayMergeSort(large_vector, 8, 10);
    
    EXPECT_TRUE(std::is_sorted(result1.begin(), result1.end()));
    EXPECT_TRUE(std::is_sorted(result2.begin(), result2.end()));
    EXPECT_TRUE(std::is_sorted(result3.begin(), result3.end()));
}

// 14: параметры по умолчанию
TEST_F(MergeTest, DefaultParameters) {
    std::vector<int> result_default = KwayMergeSort(random_vector);
    std::vector<int> result_explicit = KwayMergeSort(random_vector, 4, 8);
    
    std::vector<int> expected = random_vector;
    std::sort(expected.begin(), expected.end());
    
    EXPECT_EQ(result_default, expected);
    EXPECT_EQ(result_explicit, expected);
}