#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "collection.h"
#include "exceptions.h"

template <typename T>
std::shared_ptr<T> maxByMetric(const Collection<T>& collection) {
    if (collection.size() == 0) throw ObjectNotFoundException("Коллекция пуста, невозможно найти максимум");
    std::shared_ptr<T> best = nullptr;
    for (const auto& item : collection) {
        if (!item) continue;
        if (!best || item->calculateMetric() > best->calculateMetric()) {
            best = item;
        }
    }
    if (!best) throw ObjectNotFoundException("В коллекции нет валидных элементов");
    return best;
}

template <typename T>
double averageMetric(const Collection<T>& collection) {
    if (collection.size() == 0) throw ObjectNotFoundException("Коллекция пуста, невозможно вычислить среднее");

    double sum = 0.0;
    size_t count = 0;
    for (const auto& item : collection) {
        if (item) {
            sum += item->calculateMetric();
            ++count;
        }
    }
    if (count == 0) throw ObjectNotFoundException("В коллекции нет валидных элементов");
    return sum / count;
}

template <typename T, typename Comparator>
std::vector<std::shared_ptr<T>> sortBy(const Collection<T>& collection, Comparator comp) {
    std::vector<std::shared_ptr<T>> result;
    for (const auto& item : collection) {
        if(item) result.push_back(item);
    }

    std::sort(
        result.begin(), result.end(),
        [&comp](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return comp(*a, *b);
        }
    );
    return result;
}