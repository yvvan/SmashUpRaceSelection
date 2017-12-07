#include "mainmobile.h"

#include "ui_mainmobile.h"
#include "ui_mainwidget.h"

#include <QGroupBox>
#include <QMessageBox>

MainMobile::MainMobile(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainMobile>()) {
  ui_->setupUi(this);

  ui_main_ = std::make_unique<Ui::MainWidget>();
  selected_expansions_.fill(true);
  selected_factions_.fill(true);

  onBackClicked();
}

static QPushButton* AddButton(QVBoxLayout* layout, const QString& text) {
  QPushButton* back_button = new QPushButton(text);
  layout->addWidget(back_button);
  return back_button;
}

void MainMobile::ChangeCurrent(ActiveScreen new_current) {
  if (current_screen_ == ActiveScreen::Main && ui_main_->playersNumberCombo) {
    players_number_ = ui_main_->playersNumberCombo->currentText().toUInt();
  } else if (current_screen_ == ActiveScreen::Factions) {
    auto* factions_item = ui_->verticalLayout->itemAt(ui_->verticalLayout->count() - 1);
    auto* factions_list = qobject_cast<QListWidget*>(factions_item->widget());
    for (size_t i = 0; i < kFactionsNumber; ++i) {
      const auto* item = factions_list->item(static_cast<int>(i));
      selected_factions_[i] = (item->checkState() == Qt::Checked);
    }
  } else if (current_screen_ == ActiveScreen::Expansions) {
    auto* expansions_item = ui_->verticalLayout->itemAt(ui_->verticalLayout->count() - 1);
    auto* expansions_list = qobject_cast<QListWidget*>(expansions_item->widget());
    for (size_t i = 0; i < kExpansionsNumber; ++i) {
      const auto* item = expansions_list->item(static_cast<int>(i));
      const size_t shift = kExpansionShifts[i];
      const bool check = (item->checkState() == Qt::Checked);
      selected_expansions_[i] = check;
      for (size_t j = 0; j < kExpansionSizes[i]; ++j) {
        selected_factions_[shift + j] = check;
      }
    }
  }
  current_screen_ = new_current;
}

void MainMobile::onFactionsClicked() {
  ChangeCurrent(ActiveScreen::Factions);
  ClearLayout(ui_->verticalLayout);
  QListWidget* factions_list = new QListWidget;
  InitList(factions_list, kFactions, &selected_factions_);

  auto* back_button = AddButton(ui_->verticalLayout, "Back");
  ui_->verticalLayout->addWidget(factions_list);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onExpansionsClicked() {
  ChangeCurrent(ActiveScreen::Expansions);
  ClearLayout(ui_->verticalLayout);
  QListWidget* expansions_list = new QListWidget;
  InitList(expansions_list, kExpansions, &selected_expansions_);

  auto* back_button = AddButton(ui_->verticalLayout, "Back");
  ui_->verticalLayout->addWidget(expansions_list);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onBackClicked() {
  ChangeCurrent(ActiveScreen::Main);
  ClearLayout(ui_->verticalLayout);
  QWidget* widget = new QWidget;
  ui_main_->setupUi(widget);
  ui_main_->playersNumberCombo->setCurrentText(QString::number(players_number_));
  ui_->verticalLayout->addWidget(widget);
  QObject::connect(ui_main_->expansionsButton, &QPushButton::clicked, this,
                   &MainMobile::onExpansionsClicked);
  QObject::connect(ui_main_->factionsButton, &QPushButton::clicked, this,
                   &MainMobile::onFactionsClicked);
  QObject::connect(ui_main_->randomizeButton, &QPushButton::clicked, this,
                   &MainMobile::onRandomizeClicked);
}

void MainMobile::onRandomizeClicked() {
  ChangeCurrent(ActiveScreen::Results);
  ClearLayout(ui_->verticalLayout);

  auto* back_button = AddButton(ui_->verticalLayout, "Back");
  auto* repeat_button = AddButton(ui_->verticalLayout, "Repeat");
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
  QObject::connect(repeat_button, &QPushButton::clicked, this,
                   &MainMobile::onRandomizeClicked);

  std::vector<size_t> faction_indeces = GetSelectedIndeces(selected_factions_);
  const size_t factions_number = players_number_ * 2;
  if (faction_indeces.size() < factions_number) {
    onBackClicked();
    QMessageBox::critical(this, "Error",
                          "Select at least " + QString::number(factions_number, 10) + " factions");
    return;
  }
  std::random_shuffle(faction_indeces.begin(), faction_indeces.end());

  QListWidget* bases_list = new QListWidget();
  for (size_t i = 0; i < players_number_; ++i) {
    AddGroup(ui_->verticalLayout, bases_list, i,
             faction_indeces[i * 2], faction_indeces[i * 2 + 1]);
  }

  QGroupBox* bases_box = new QGroupBox();
  bases_box->setContentsMargins(0, 0, 0, 0);
  ui_->verticalLayout->addWidget(bases_box);
  bases_box->setTitle("Bases");
  QVBoxLayout* basesLayout = new QVBoxLayout(bases_box);
  basesLayout->setContentsMargins(0, 0, 0, 0);
  basesLayout->addWidget(bases_list);

  int row_height = bases_list->visualItemRect(bases_list->item(0)).height();
  int left, top, right, bottom;
  bases_list->getContentsMargins(&left, &top, &right, &bottom);
  bases_list->setFixedHeight(bases_list->count() * row_height + top + bottom);
}

MainMobile::~MainMobile() = default;
