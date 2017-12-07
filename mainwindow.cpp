#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QMessageBox>

#include <cassert>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);

  InitList(ui_->expansionsList, kExpansions);
  InitList(ui_->factionsList, kFactions);
  selected_expansions_.fill(true);
  selected_factions_.fill(true);

  QObject::connect(ui_->expansionsList, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(ExpansionChanged(QListWidgetItem*)));
  QObject::connect(ui_->factionsList, SIGNAL(itemChanged(QListWidgetItem*)),
                   this, SLOT(FactionChanged(QListWidgetItem*)));
  QObject::connect(ui_->pushButton, SIGNAL(clicked()),
                   this, SLOT(RandomizeClicked()));
}

MainWindow::~MainWindow() = default;

void MainWindow::ExpansionChanged(QListWidgetItem* item) {
  const size_t row = static_cast<size_t>(ui_->expansionsList->row(item));
  const size_t shift = kExpansionShifts[row];
  const bool check = (item->checkState() == Qt::Checked);
  selected_expansions_[row] = check;
  for (size_t i = 0; i < kExpansionSizes[row]; ++i) {
    auto* faction = ui_->factionsList->item(shift + i);
    faction->setCheckState(check ? Qt::Checked : Qt::Unchecked);
    selected_factions_[shift + i] = check;
  }
}

void MainWindow::FactionChanged(QListWidgetItem* item) {
  const size_t row = static_cast<size_t>(ui_->factionsList->row(item));
  selected_factions_[row] = (item->checkState() == Qt::Checked);
}

void MainWindow::RandomizeClicked() {
  const size_t players_number = ui_->comboBox->currentText().toUInt();
  const size_t factions_number = players_number * 2;

  std::vector<size_t> faction_indeces = GetSelectedIndeces(selected_factions_);
  if (faction_indeces.size() < factions_number) {
    QMessageBox::critical(this, "Error",
                          "Select at least " + QString::number(factions_number, 10) + " factions");
    return;
  }

  std::random_shuffle(faction_indeces.begin(), faction_indeces.end());

  ClearLayout(ui_->horizontalLayout_4);
  ClearLayout(ui_->horizontalLayout_5);
  ui_->basesList->clear();

  for (size_t i = 0; i < std::min(players_number, static_cast<size_t>(4)); ++i) {
    AddGroup(ui_->horizontalLayout_4, ui_->basesList, i,
             faction_indeces[i * 2], faction_indeces[i * 2 + 1]);
  }
  if (players_number > 4) {
    for (size_t i = 4; i < players_number; ++i) {
      AddGroup(ui_->horizontalLayout_5, ui_->basesList, i,
               faction_indeces[i * 2], faction_indeces[i * 2 + 1]);
    }
  }
}
