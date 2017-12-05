#include "mainmobile.h"
#include "utils.h"

#include "ui_mainmobile.h"
#include "ui_mainwidget.h"

MainMobile::MainMobile(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainMobile>()) {
  ui_->setupUi(this);

  ui_main_ = std::make_unique<Ui::MainWidget>();
  std::copy(kFactions.begin(), kFactions.end(),
            std::inserter(selected_factions_, selected_factions_.begin()));
  std::copy(kExpansions.begin(), kExpansions.end(),
            std::inserter(selected_expansions_, selected_expansions_.begin()));
  onBackClicked();
}

static void ClearLayout(QBoxLayout* layout) {
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

static QPushButton* AddBackButton(QVBoxLayout* layout) {
  QHBoxLayout* horizontal_layout = new QHBoxLayout;
  layout->addLayout(horizontal_layout);
  QPushButton* back_button = new QPushButton("Back");
  horizontal_layout->addWidget(back_button);
  horizontal_layout->addStretch();
  return back_button;
}

void MainMobile::UncheckFactions(QListWidget* factions_list) {
  int current{0};
  for (const auto& faction: kFactions) {
    if (selected_factions_.find(faction) == selected_factions_.end()) {
      factions_list->item(current)->setCheckState(Qt::Unchecked);
    }
    ++current;
  }
}

void MainMobile::UncheckExpansions(QListWidget* expansions_list) {
  int current{0};
  for (const auto& expansion: kExpansions) {
    if (selected_expansions_.find(expansion) == selected_expansions_.end()) {
      expansions_list->item(current)->setCheckState(Qt::Unchecked);
    }
    ++current;
  }
}

void MainMobile::ChangeCurrent(ActiveScreen new_current) {
  if (current_screen_ == ActiveScreen::Factions) {
    auto* factions_item = ui_->verticalLayout->itemAt(ui_->verticalLayout->count() - 1);
    auto* factions_list = qobject_cast<QListWidget*>(factions_item->widget());
    for (size_t i = 0; i < kFactionsNumber; ++i) {
      const auto* item = factions_list->item(i);
      if (item->checkState() == Qt::Unchecked) {
        selected_factions_.erase(item->text());
      } else {
        selected_factions_.insert(item->text());
      }
    }
  } else if (current_screen_ == ActiveScreen::Expansions) {
    auto* expansions_item = ui_->verticalLayout->itemAt(ui_->verticalLayout->count() - 1);
    auto* expansions_list = qobject_cast<QListWidget*>(expansions_item->widget());
    for (size_t i = 0; i < kExpansionsNumber; ++i) {
      const auto* item = expansions_list->item(i);
      size_t shift = kExpansionShifts[i];
      if (item->checkState() == Qt::Checked) {
        selected_expansions_.insert(item->text());
        for (size_t j = 0; j < kExpansionSizes[i]; ++j) {
          selected_factions_.insert(kFactions[shift + j]);
        }
      } else {
        selected_expansions_.erase(item->text());
        for (size_t j = 0; j < kExpansionSizes[i]; ++j) {
          selected_factions_.erase(kFactions[shift + j]);
        }
      }
    }
  }
  current_screen_ = new_current;
}

void MainMobile::onFactionsClicked() {
  ChangeCurrent(ActiveScreen::Factions);
  ClearLayout(ui_->verticalLayout);
  QListWidget* factions_list = new QListWidget;
  InitList(factions_list, kFactions);
  UncheckFactions(factions_list);

  auto* back_button = AddBackButton(ui_->verticalLayout);
  ui_->verticalLayout->addWidget(factions_list);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onExpansionsClicked() {
  ChangeCurrent(ActiveScreen::Expansions);
  ClearLayout(ui_->verticalLayout);
  QListWidget* expansions_list = new QListWidget;
  InitList(expansions_list, kExpansions);
  UncheckExpansions(expansions_list);

  auto* back_button = AddBackButton(ui_->verticalLayout);
  ui_->verticalLayout->addWidget(expansions_list);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onBackClicked() {
  ChangeCurrent(ActiveScreen::Main);
  ClearLayout(ui_->verticalLayout);
  QWidget* widget = new QWidget;
  ui_main_->setupUi(widget);
  ui_->verticalLayout->addWidget(widget);
  QObject::connect(ui_main_->expansionsButton, &QPushButton::clicked, this,
                   &MainMobile::onExpansionsClicked);
  QObject::connect(ui_main_->factionsButton, &QPushButton::clicked, this,
                   &MainMobile::onFactionsClicked);
}

MainMobile::~MainMobile() = default;
