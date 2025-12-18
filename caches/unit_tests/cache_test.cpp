#include <gtest/gtest.h>
#include "../CacheImp.h"

class CacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        small_cache = std::make_unique<LRUCache<int, std::string>>(3);
        int_cache = std::make_unique<LRUCache<int, int>>(5);
        string_cache = std::make_unique<LRUCache<std::string, int>>(4);
    }

    void TearDown() override {
    }

    std::unique_ptr<LRUCache<int, std::string>> small_cache;
    std::unique_ptr<LRUCache<int, int>> int_cache;
    std::unique_ptr<LRUCache<std::string, int>> string_cache;
};

// 1: создание пустого кэша
TEST_F(CacheTest, EmptyCache) {
    LRUCache<int, int> cache(10);
    EXPECT_EQ(cache.size(), 0);
    EXPECT_EQ(cache.capacity(), 10);
}

// 2: добавление одного элемента
TEST_F(CacheTest, PutAndGetSingleItem) {
    small_cache->set(1, "one");
    EXPECT_EQ(small_cache->size(), 1);
    EXPECT_EQ(small_cache->get(1), "one");
}

// 3: проверка наличия элемента
TEST_F(CacheTest, ContainsCheck) {
    small_cache->set(1, "one");
    EXPECT_TRUE(small_cache->contains(1));
    EXPECT_FALSE(small_cache->contains(2));
}

// 4: получение несуществующего элемента (должно быть исключение)
TEST_F(CacheTest, GetNonExistentItem) {
    EXPECT_THROW(small_cache->get(999), std::out_of_range);
}

// 5: обновление существующего элемента
TEST_F(CacheTest, UpdateExistingItem) {
    small_cache->set(1, "one");
    small_cache->set(1, "updated");
    EXPECT_EQ(small_cache->size(), 1);
    EXPECT_EQ(small_cache->get(1), "updated");
}

// 6: проверка LRU поведения
TEST_F(CacheTest, LRUEviction) {
    // Заполняем кэш емкостью 3
    small_cache->set(1, "one");
    small_cache->set(2, "two");
    small_cache->set(3, "three");
    
    EXPECT_EQ(small_cache->get(1), "one");
    small_cache->set(4, "four");
    
    EXPECT_EQ(small_cache->size(), 3);
    EXPECT_TRUE(small_cache->contains(1));
    EXPECT_FALSE(small_cache->contains(2)); 
    EXPECT_TRUE(small_cache->contains(3));
    EXPECT_TRUE(small_cache->contains(4));
}

// 7: полная очистка кэша
TEST_F(CacheTest, ClearCache) {
    small_cache->set(1, "one");
    small_cache->set(2, "two");
    EXPECT_EQ(small_cache->size(), 2);
    
    small_cache->clear();
    EXPECT_EQ(small_cache->size(), 0);
    EXPECT_FALSE(small_cache->contains(1));
    EXPECT_FALSE(small_cache->contains(2));
}

// 8: работа с разными типами ключей и значений
TEST_F(CacheTest, DifferentTypes) {
    string_cache->set("apple", 5);
    string_cache->set("banana", 3);
    
    EXPECT_EQ(string_cache->get("apple"), 5);
    EXPECT_EQ(string_cache->get("banana"), 3);
    EXPECT_EQ(string_cache->size(), 2);
}

// 9: заполнение до предела емкости
TEST_F(CacheTest, FillToCapacity) {
    for (int i = 1; i <= 5; ++i) {
        int_cache->set(i, i * 10);
    }
    
    EXPECT_EQ(int_cache->size(), 5);
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(int_cache->contains(i));
        EXPECT_EQ(int_cache->get(i), i * 10);
    }
}

// 10: вытеснение при переполнении
TEST_F(CacheTest, OverflowEviction) {
    for (int i = 1; i <= 5; ++i) {
        int_cache->set(i, i);
    }
    
    int_cache->get(2);
    int_cache->get(4);
    
    int_cache->set(6, 6);
    
    EXPECT_EQ(int_cache->size(), 5);
    EXPECT_FALSE(int_cache->contains(1));
    EXPECT_TRUE(int_cache->contains(2));
    EXPECT_TRUE(int_cache->contains(3));
    EXPECT_TRUE(int_cache->contains(4));
    EXPECT_TRUE(int_cache->contains(5));
    EXPECT_TRUE(int_cache->contains(6));
}

// 11: последовательное добавление одинаковых ключей
TEST_F(CacheTest, RepeatedSameKey) {
    small_cache->set(1, "first");
    small_cache->set(1, "second");
    small_cache->set(1, "third");
    
    EXPECT_EQ(small_cache->size(), 1);
    EXPECT_EQ(small_cache->get(1), "third");
}

// 12: проверка емкости 1
TEST_F(CacheTest, CapacityOne) {
    LRUCache<int, std::string> tiny_cache(1);
    tiny_cache.set(1, "one");
    EXPECT_EQ(tiny_cache.size(), 1);
    EXPECT_EQ(tiny_cache.get(1), "one");
    
    tiny_cache.set(2, "two");
    EXPECT_EQ(tiny_cache.size(), 1);
    EXPECT_FALSE(tiny_cache.contains(1));
    EXPECT_EQ(tiny_cache.get(2), "two");
}

// 13: комплексный сценарий использования
TEST_F(CacheTest, ComplexUsageScenario) {
    small_cache->set(1, "page1");
    small_cache->set(2, "page2");
    small_cache->get(1);        // page1 свежий
    
    small_cache->set(3, "page3"); // page2 старый
    small_cache->set(4, "page4"); // вытесняет page2
    
    EXPECT_EQ(small_cache->size(), 3);
    EXPECT_TRUE(small_cache->contains(1));
    EXPECT_FALSE(small_cache->contains(2));
    EXPECT_TRUE(small_cache->contains(3));
    EXPECT_TRUE(small_cache->contains(4));
    small_cache->get(1); // обновили page1
    small_cache->set(5, "page5");
    EXPECT_FALSE(small_cache->contains(3));
}
// 14: работа с большим кэшем
TEST_F(CacheTest, LargeCache) {
    LRUCache<int, int> large_cache(1000);
    
    for (int i = 0; i < 1000; ++i) {
        large_cache.set(i, i * 2);
    }
    
    EXPECT_EQ(large_cache.size(), 1000);
    
    large_cache.set(1000, 2000);
    EXPECT_EQ(large_cache.size(), 1000);
    EXPECT_FALSE(large_cache.contains(0));
    EXPECT_TRUE(large_cache.contains(1));
    EXPECT_TRUE(large_cache.contains(1000));
}

// 15: проверка неизмености размера при обновлении
TEST_F(CacheTest, SizeOnUpdate) {
    small_cache->set(1, "one");
    std::size_t size_before = small_cache->size();
    
    small_cache->set(1, "updated_one");
    EXPECT_EQ(small_cache->size(), size_before);
    EXPECT_EQ(small_cache->get(1), "updated_one");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

