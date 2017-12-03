#pragma once

#include <QMainWindow>

#include <memory>
#include <set>

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
  int GetBaseIndexByFaction(QString faction_name);
  void ShowBases(QString faction_name);
  std::unique_ptr<Ui::MainWindow> ui_;
  std::unique_ptr<Ui::FactionsWidget> ui_factions_;
  std::unique_ptr<Ui::ExpansionsWidget> ui_expansions_;
  std::set<QString> selected_expansions_;
  std::set<QString> selected_factions_;

  QWidget* factions_widget_;
};
