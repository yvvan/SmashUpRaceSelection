#pragma once

#include <QListWidget>

#include <array>

static constexpr size_t kExpansionsNumber = 14;

static constexpr size_t
kExpansionSizes[kExpansionsNumber] = {8, 4, 4, 4, 1, 4, 4, 8, 5, 4, 4, 4, 1, 1};

static constexpr size_t
kExpansionShifts[kExpansionsNumber] = {0, 8, 12, 16, 20, 21, 25, 29, 37, 42, 46, 50, 54, 55};

static constexpr const std::array<const char*, kExpansionsNumber>
kExpansions = {"Core Set", "Awesome Level 9000", "The Obligatory Cthulhu Set",
               "Science Fiction Double Feature", "Geeks", "Monster Smash", "Pretty Pretty",
               "Munchkin", "It's Your Fault", "Cease & Desist", "What Were We Thinking?",
               "Big in Japan", "All Stars Event Kit", "Sheep Promotion"};

template <size_t N>
constexpr size_t ArrSum(size_t const (&arr)[N], std::size_t i = 0) {
  return i < N ? (arr[i] + ArrSum<N>(arr, i + 1)) : 0;
}

static constexpr size_t kFactionsNumber = ArrSum<kExpansionsNumber>(kExpansionSizes);

static constexpr const std::array<const char*, kFactionsNumber>
kFactions = {"Aliens", "Dinasaurs", "Ninjas", "Pirates", "Robots", "Tricksters", "Wizards",
             "Zombies", "Bear Cavalry", "Ghosts", "Killer Plants", "Steampunks", "Elder Things",
             "Innsmouth", "Minions of Cthulhu", "Miskatonic University", "Cyborg Apes",
             "Shapeshifters", "Super Spies", "Time Travelers", "Geeks", "Giant Ants",
             "Mad Scientists", "Vampires", "Werewolves", "Fairies", "Kitty Cats", "Mythic Horses",
             "Princesses", "Clerics", "Dwarves", "Elves", "Halflings", "Mages", "Orcs", "Thieves",
             "Warriors", "Dragons", "Mythic Greeks", "Sharks", "Superheroes", "Tornadoes",
             "Astroknightes", "Changerbots", "Ignobles", "Star Roamers", "Explorers", "Grannies",
             "Rock Stars", "Teddy Bears", "Itty Critters", "Kaiju", "Magical Girls",
             "Mega Troopers", "Smash Up All Stars", "Sheep"};

template<size_t N>
void InitList(QListWidget* list, std::array<const char*, N> values) {
  int current {0};
  for (const auto& value: values) {
    list->addItem(value);
    auto* item = list->item(current++);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
  }
}
