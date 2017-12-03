#pragma once

#include <QListWidget>

#include <array>
#include <set>

static constexpr std::array<size_t, 14> kExpansionSizes = {8, 4, 4, 4, 1, 4, 4, 8, 5, 4, 4, 4, 1, 1};
static constexpr std::array<size_t, 14> kExpansionShifts = {0, 8, 12, 16, 20, 21, 25, 29, 37, 42, 46, 50, 54, 55};

void InitList(QListWidget* list, std::set<QString>& values);
