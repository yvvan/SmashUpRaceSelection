#pragma once

#include "utils.h"

#include <QMainWindow>

#include <memory>

namespace Ui {
  class MainWindow;
  class FactionsWidget;
  class ExpansionsWidget;
}

class QListWidgetItem;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void ExpansionChanged(QListWidgetItem*);
  void FactionChanged(QListWidgetItem*);
  void RandomizeClicked();

private:
  std::unique_ptr<Ui::MainWindow> ui_;
  std::array<bool, kExpansionsNumber> selected_expansions_;
  std::array<bool, kFactionsNumber> selected_factions_;

  QWidget* factions_widget_;
};
