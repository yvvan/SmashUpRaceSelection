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
kFactions = {"Aliens", "Dinosaurs", "Ninjas", "Pirates", "Robots", "Tricksters", "Wizards",
             "Zombies", "Bear Cavalry", "Ghosts", "Killer Plants", "Steampunks", "Elder Things",
             "Innsmouth", "Minions of Cthulhu", "Miskatonic University", "Cyborg Apes",
             "Shapeshifters", "Super Spies", "Time Travelers", "Geeks", "Giant Ants",
             "Mad Scientists", "Vampires", "Werewolves", "Fairies", "Kitty Cats", "Mythic Horses",
             "Princesses", "Clerics", "Dwarves", "Elves", "Halflings", "Mages", "Orcs", "Thieves",
             "Warriors", "Dragons", "Mythic Greeks", "Sharks", "Superheroes", "Tornadoes",
             "Astroknightes", "Changerbots", "Ignobles", "Star Roamers", "Explorers", "Grannies",
             "Rock Stars", "Teddy Bears", "Itty Critters", "Kaiju", "Magical Girls",
             "Mega Troopers", "Smash Up All Stars", "Sheep"};

static constexpr size_t kBasesNumber = kFactionsNumber * 2;

static constexpr const std::array<const char*, kBasesNumber>
kBases = {"The Homeworld","The Mothership","Jungle Oasis","Tar Pits","Ninja Dojo","Temple of Goju",
          "The Grey Opal","Tortuga","Factory 436-1337","The Central Brain","Cave of Shinies",
          "Mushroom Kingdom","School of Wizardry","The Great Library","Evans City Cemetery",
          "Rhodes Plaza Mall","Field of Honor","Tsar's Palace","Haunted House","The Dread Gazebo",
          "Secret Grove","The Greenhouse","Inventor's Salon","Workshop","Antarctic Base",
          "Plateau of Leng","Innsmouth","Ritual Site","Mountains of Madness","R'lyeh","Asylum",
          "Miskatonic University","Monkey Lab","Primate Park","Faceless City","The Vats",
          "ISI's Swingin' Pad","Secret Volcano Headquarters","Portal Room","The Nexus","TableTop",
          "The Con","Egg Chamber","The Hill","Golem Schloß","Laboratorium","Castle Blood","Crypt",
          "Moot Site","Standing Stones","Enchanted Glen","Fairy Circle","Cool Cats' Alley",
          "The House of Nine Lives","Equaria","Pony Land","Beautiful Castle","Ice Castle",
          "Hotel of Holiness","Whack-A-Ghoul","The Mines","Treasure Bath","Helper's Hollow",
          "Treehouse","Birthday Party","Subterranean Lair","Dimension Doors","Mage's Tower",
          "Garrison","The Pits","The Coffers","Thieves' Guild","Bastion","The Gauntlet",
          "Dragon's Lair","Wyrm's Desolation","Oracle at Delphi","Wooden Horse","Shark Reef",
          "The Deep","Converted Cave","Crystal Fortress","Tornado Alley","Trailer Park",
          "Hive of Scum and Villainy","No-Moon","Changing Room","Unicrave","Spikey Chair Room",
          "Wintersquashed","Neutral Space","USS Undertaking","Ancient Temple","City of Gold",
          "Grandma's House","Retirement Community","Lake Minnetonka","Palooza","Out in the Woods",
          "Under the Bed","Critter Combat Club","Itty City","Kaiju Island","Tokyo","Akihabara High",
          "Q Point","Juice Bar","Moon Dumpster","Locker Room","Stadium","Sheep Shrine",
          "The Pasture"};

class QBoxLayout;
void ClearLayout(QBoxLayout* layout);

void AddGroup(QBoxLayout* cur_layout, QListWidget* bases_list, size_t player, size_t faction_index1,
              size_t faction_index2);

template<size_t N>
std::vector<size_t> GetSelectedIndeces(const std::array<bool, N>& selected_factions) {
  std::vector<size_t> faction_indeces;
  faction_indeces.reserve(kFactionsNumber);
  size_t current {0};
  for (bool faction: selected_factions) {
    if (faction) {
      faction_indeces.push_back(current);
    }
    ++current;
  }
  return faction_indeces;
}

template<size_t N>
void InitList(QListWidget* list, std::array<const char*, N> values,
              std::array<bool, N>* selected = nullptr) {
  int current {0};
  for (const auto& value: values) {
    list->addItem(value);
    auto* item = list->item(current);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    if (selected && !(*selected)[current]) {
      item->setCheckState(Qt::Unchecked);
    } else {
      item->setCheckState(Qt::Checked);
    }
    ++current;
  }
}
