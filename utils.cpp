#include "utils.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>

void ClearLayout(QBoxLayout* layout) {
  auto* item = layout->takeAt(0);
  while (item) {
    QBoxLayout* childLayout = static_cast<QBoxLayout*>(item->layout());
    if (childLayout) {
      ClearLayout(childLayout);
      delete childLayout;
      item = layout->takeAt(0);
      continue;
    }
    delete item->widget();
    delete item;
    item = layout->takeAt(0);
  }
}

static void ShowBases(QListWidget* bases_list, size_t faction_index) {
    const size_t base_index = faction_index * 2;
    bases_list->addItem(kBases[base_index]);
    bases_list->addItem(kBases[base_index + 1]);
    auto* item1 = bases_list->item(bases_list->count() - 2);
    auto* item2 = bases_list->item(bases_list->count() - 1);
    item1->setFlags(item1->flags() & ~Qt::ItemIsSelectable);
    item2->setFlags(item2->flags() & ~Qt::ItemIsSelectable);
}

void AddGroup(QBoxLayout* cur_layout, QListWidget* bases_list,
                     size_t player, size_t faction_index1, size_t faction_index2) {
  QGroupBox* groupBox = new QGroupBox();
  groupBox->setContentsMargins(0, 0, 0, 0);
  cur_layout->addWidget(groupBox);
  groupBox->setTitle("Player " + QString::number(player + 1, 10));
  QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);
  groupLayout->setContentsMargins(9, 0, 9, 4);
  QLabel* faction1 = new QLabel(groupBox);
  QLabel* faction2 = new QLabel(groupBox);
  groupLayout->addWidget(faction1);
  faction1->setText(kFactions[faction_index1]);
  groupLayout->addWidget(faction2);
  faction2->setText(kFactions[faction_index2]);
  ShowBases(bases_list, faction_index1);
  ShowBases(bases_list, faction_index2);
}
