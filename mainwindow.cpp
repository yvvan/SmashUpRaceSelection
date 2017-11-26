#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QScroller>

#include <cassert>
#include <algorithm>
#include <array>
#include <ctime>

static void InitList(QListWidget* list, std::set<QString>& values) {
  for (auto i = 0; i < list->count(); ++i) {
    auto* item = list->item(i);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    values.insert(item->text());
  }
}

static constexpr std::array<size_t, 14> kExpansionSizes = {8, 4, 4, 4, 1, 4, 4, 8, 5, 4, 4, 4, 1, 1};

static void AddConnections(QListWidget* expansions, QListWidget* factions,
                    MainWindow::Connections& connections) {
  size_t shift {0};
  for (size_t i = 0; i < static_cast<size_t>(expansions->count()); ++i) {
    auto& factions_set = connections[expansions->item(static_cast<int>(i))];
    for (size_t j = shift; j < shift + kExpansionSizes.at(i); ++j) {
      factions_set.insert(factions->item(static_cast<int>(j)));
    }
    shift += kExpansionSizes.at(i);
  }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);

  assert(kExpansionSizes.size() == ui_->listWidget->count());
  InitList(ui_->listWidget, selected_expansions_);
  InitList(ui_->listWidget_2, selected_factions_);
  for (int i = 0; i < ui_->listWidget_3->count(); ++i) {
    auto item = ui_->listWidget_3->item(i);
    item->setHidden(true);
  }
  AddConnections(ui_->listWidget, ui_->listWidget_2, connections_);

  QObject::connect(ui_->listWidget, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(ExpansionChanged(QListWidgetItem*)));
  QObject::connect(ui_->listWidget_2, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(FactionChanged(QListWidgetItem*)));
  QObject::connect(ui_->pushButton, SIGNAL(clicked()),
                   this, SLOT(RandomizeClicked()));
  QScroller::grabGesture(ui_->listWidget, QScroller::LeftMouseButtonGesture);
  QScroller::grabGesture(ui_->listWidget_2, QScroller::LeftMouseButtonGesture);
  QScroller::grabGesture(ui_->listWidget_3, QScroller::LeftMouseButtonGesture);
  std::srand(std::time(0));
}

std::vector<QString> FactionsByExpansion(QString expansion) {
  std::vector<QString> factions;

  return factions;
}

MainWindow::~MainWindow() = default;

void MainWindow::ExpansionChanged(QListWidgetItem* item) {
  if (item->checkState() == Qt::Checked) {
    selected_expansions_.insert(item->text());
    for (auto* faction : connections_[item]) {
      faction->setCheckState(Qt::Checked);
      selected_factions_.insert(faction->text());
    }
  } else {
    selected_expansions_.erase(item->text());
    for (auto* faction : connections_[item]) {
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

int MainWindow::GetBaseIndexByFaction(QString faction_name) {
  auto items = ui_->listWidget_2->findItems(faction_name, Qt::MatchExactly);
  return ui_->listWidget_2->row(items[0]) * 2;
}

void MainWindow::ShowBases(QString faction_index) {
    int base_index = GetBaseIndexByFaction(faction_index);
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
