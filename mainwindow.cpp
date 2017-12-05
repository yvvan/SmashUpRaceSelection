#include "mainwindow.h"

#include "utils.h"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QScroller>

#include <cassert>
#include <algorithm>
#include <array>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);

  InitList(ui_->expansionsList, kExpansions);
  InitList(ui_->factionsList, kFactions);
  std::copy(kExpansions.begin(), kExpansions.end(),
            std::inserter(selected_expansions_, selected_expansions_.begin()));
  std::copy(kFactions.begin(), kFactions.end(),
            std::inserter(selected_factions_, selected_factions_.begin()));
  for (int i = 0; i < ui_->listWidget_3->count(); ++i) {
    auto item = ui_->listWidget_3->item(i);
    item->setHidden(true);
  }

  QObject::connect(ui_->expansionsList, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(ExpansionChanged(QListWidgetItem*)));
  QObject::connect(ui_->factionsList, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(FactionChanged(QListWidgetItem*)));
  QObject::connect(ui_->pushButton, SIGNAL(clicked()),
                   this, SLOT(RandomizeClicked()));
#ifdef Q_OS_ANDROID
  QScroller::grabGesture(ui_->expansionsList, QScroller::LeftMouseButtonGesture);
  QScroller::grabGesture(ui_->factionsList, QScroller::LeftMouseButtonGesture);
  QScroller::grabGesture(ui_->listWidget_3, QScroller::LeftMouseButtonGesture);
  ui_->expansionsList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui_->expansionsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui_->factionsList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui_->factionsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui_->listWidget_3->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  ui_->listWidget_3->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
#endif
  std::srand(std::time(0));
}

MainWindow::~MainWindow() = default;

void MainWindow::ExpansionChanged(QListWidgetItem* item) {
  size_t row = static_cast<size_t>(ui_->expansionsList->row(item));
  size_t shift = kExpansionShifts[row];
  if (item->checkState() == Qt::Checked) {
    selected_expansions_.insert(item->text());
    for (size_t i = 0; i < kExpansionSizes[row]; ++i) {
      auto* faction = ui_->factionsList->item(shift + i);
      faction->setCheckState(Qt::Checked);
      selected_factions_.insert(faction->text());
    }
  } else {
    selected_expansions_.erase(item->text());
    for (size_t i = 0; i < kExpansionSizes[row]; ++i) {
      auto* faction = ui_->factionsList->item(shift + i);
      faction->setCheckState(Qt::Unchecked);
      selected_factions_.erase(faction->text());
    }
  }
}

void MainWindow::FactionChanged(QListWidgetItem* item) {
  if (item->checkState() == Qt::Checked) {
    selected_factions_.insert(item->text());
  } else {
    selected_factions_.erase(item->text());
  }
}

static void ClearLayout(QHBoxLayout* layout) {
  auto* item = layout->takeAt(0);
  while (item) {
    delete item->widget();
    delete item;
    item = layout->takeAt(0);
  }
}

static void AddGroup(QGroupBox* parent, QHBoxLayout* cur_layout, size_t player,
                     QString item1, QString item2) {
  QGroupBox* groupBox = new QGroupBox(parent);
  groupBox->setContentsMargins(0, 0, 0, 0);
  cur_layout->addWidget(groupBox);
  groupBox->setTitle("Player " + QString::number(player + 1, 10));
  QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);
  groupLayout->setContentsMargins(9, 0, 9, 4);
  QLabel* faction1 = new QLabel(groupBox);
  QLabel* faction2 = new QLabel(groupBox);
  groupLayout->addWidget(faction1);
  faction1->setText(item1);
  groupLayout->addWidget(faction2);
  faction2->setText(item2);
}

int MainWindow::GetBaseIndexByFaction(const QString& faction_name) {
  auto items = ui_->factionsList->findItems(faction_name, Qt::MatchExactly);
  return ui_->factionsList->row(items[0]) * 2;
}

void MainWindow::ShowBases(const QString& faction_name) {
    int base_index = GetBaseIndexByFaction(faction_name);
    ui_->listWidget_3->item(base_index)->setHidden(false);
    ui_->listWidget_3->item(base_index + 1)->setHidden(false);
}

void MainWindow::RandomizeClicked() {
  bool ok;
  const unsigned player_number = ui_->comboBox->currentText().toUInt(&ok, 10U);
  const unsigned factions_number = player_number * 2;
  if (selected_factions_.size() < factions_number) {
    QMessageBox::critical(this, "Error",
                          "Select at least " + QString::number(factions_number, 10) + " factions");
    return;
  }
  assert(ok);
  std::vector<QString> all_factions(selected_factions_.size());
  std::copy(selected_factions_.begin(), selected_factions_.end(), all_factions.begin());
  std::vector<size_t> faction_indeces(selected_factions_.size());
  std::iota(faction_indeces.begin(), faction_indeces.end(), 0);
  std::random_shuffle(faction_indeces.begin(), faction_indeces.end());

  ClearLayout(ui_->horizontalLayout_4);
  ClearLayout(ui_->horizontalLayout_5);
  for (int i = 0; i < ui_->listWidget_3->count(); ++i) {
    auto item = ui_->listWidget_3->item(i);
    item->setHidden(true);
  }

  for (size_t i = 0; i < std::min(player_number, 4U); ++i) {
    AddGroup(ui_->verticalGroupBox, ui_->horizontalLayout_4, i,
             all_factions[faction_indeces[i * 2]],
             all_factions[faction_indeces[i * 2 + 1]]);
    ShowBases(all_factions[faction_indeces[i * 2]]);
    ShowBases(all_factions[faction_indeces[i * 2 + 1]]);
  }
  if (player_number > 4) {
    for (size_t i = 4; i < player_number; ++i) {
      AddGroup(ui_->verticalGroupBox, ui_->horizontalLayout_5, i,
               all_factions[faction_indeces[i * 2]],
               all_factions[faction_indeces[i * 2 + 1]]);
      ShowBases(all_factions[faction_indeces[i * 2]]);
      ShowBases(all_factions[faction_indeces[i * 2 + 1]]);
    }
  }
}
