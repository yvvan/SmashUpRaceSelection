#include "utils.h"

void InitList(QListWidget* list, std::set<QString>& values) {
  for (auto i = 0; i < list->count(); ++i) {
    auto* item = list->item(i);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    values.insert(item->text());
  }
}
