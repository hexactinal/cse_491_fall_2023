/**
 * @file SecondWorld.hpp
 * @author Jayson Van Dam
 * @author Kurt LaBlanc
 * @author Satvik Ravipati
 */

#pragma once
#include <algorithm>
#include <vector>

#include "../core/Entity.hpp"
#include "../core/WorldBase.hpp"

namespace group4 {
/**
 * Creates a world with agents and a win flag
 */
class SecondWorld : public cse491::WorldBase {
 protected:
  enum ActionType {
    REMAIN_STILL = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
  };

  /// Easy access to floor CellType ID.
  size_t floor_id;

  /// Easy access to flag CellType ID.
  size_t flag_id;

  /// Easy access to wall CellType ID.
  size_t wall_id;

  /// Easy access to Entity char
  const size_t entity_id = '+';

  std::vector<std::unique_ptr<cse491::Entity>> inventory;

  /// Provide the agent with movement actions.
  void ConfigAgent(cse491::AgentBase& agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
  }

 public:
  /**
   * Constructor with no arguments
   */
  SecondWorld() {
    floor_id =
        AddCellType("floor", "Floor that you can easily walk over.", ' ');
    flag_id = cse491::WorldBase::AddCellType(
        "flag", "Goal flag for a game end state", 'g');
    wall_id = AddCellType(
        "wall", "Impenetrable wall that you must find a way around.", '#');
    main_grid.Read("../assets/grids/group4_maze.grid", type_options);
  }

  /**
   * Destructor
   */
  ~SecondWorld() = default;

  void UpdateWorld() override {}

  /**
   * This function gives us an output.json file
   */
  virtual void Run() override {
    while (!run_over) {
      RunAgents();
      UpdateWorld();
    }

    std::string filename = "output.json";

    std::ofstream ofs(filename);

    ofs << "[" << std::endl;
    size_t index = 0;

    for (const auto& other_agent : agent_set) {
      auto new_position = other_agent->GetPosition();

      std::string agent_name = other_agent->GetName();

      double x_pos = new_position.GetX();
      double y_pos = new_position.GetY();

      ofs << "  {" << std::endl;
      ofs << "    \"name\": "
          << "\"" << agent_name << "\""
          << ",\n";

      ofs << "    \"x\": " << x_pos << "," << std::endl;
      ofs << "    \"y\": " << y_pos << std::endl;

      if (index == agent_set.size() - 1) {
        ofs << "  }" << std::endl;
      } else {
        ofs << "  }," << std::endl;
      }

      index++;
    }
    ofs << "]" << std::endl;
  }

  /**
   * Allows agents to move around the maze.
   * Also checks if the next agent position will be the win flag.
   */
  int DoAction(cse491::AgentBase& agent, size_t action_id) override {
    cse491::GridPosition new_position;
    switch (action_id) {
      case REMAIN_STILL:
        new_position = agent.GetPosition();
        break;
      case MOVE_UP:
        new_position = agent.GetPosition().Above();
        break;
      case MOVE_DOWN:
        new_position = agent.GetPosition().Below();
        break;
      case MOVE_LEFT:
        new_position = agent.GetPosition().ToLeft();
        break;
      case MOVE_RIGHT:
        new_position = agent.GetPosition().ToRight();
        break;
    }
    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (main_grid.At(new_position) == wall_id) {
      return false;
    }

    if (main_grid.At(new_position) == flag_id) {
      // Set win flag to true
      std::cout << "flag found" << std::endl;
      run_over = true;
    }

    if (main_grid.At(new_position) == entity_id) {
      auto item_found = std::find_if(
          item_set.begin(), item_set.end(),
          [&new_position](const std::unique_ptr<cse491::ItemBase>& item) {
            return item && item->GetPosition() == new_position;
          });

      std::cout << "You found " << (*item_found)->GetName() << "!" << std::endl;

      // Move the ownership of the item to the agents inventory
      inventory.push_back(std::move(*(item_found)));

      CleanEntities();

      // Change the grid position to floor_id so it's not seen on the grid
      main_grid.At(new_position) = floor_id;
    }

    agent.SetPosition(new_position);
    return true;
  }

  /**
   * Adds an entity to the non-agent entity list
   * @param entity The entity we are adding
   */
  size_t AddEntity(std::unique_ptr<cse491::Entity> entity) {
    // Sets the '+' as the item in grid
    main_grid.At(entity->GetPosition()) = entity_id;

    // Moves the ownership of the entity into item_set
    size_t id = entity->GetID();

    // TODO: item_set now is a std::vector<std::unique_ptr<ItemBase>>,
    // so we can't call emplace_back on it with a std::unique_ptr<Entity>.
    // How should we go about this?
    item_set.emplace_back(std::move(entity));
    return id;
  }

  /**
   * Cleans itemset of all null entities
   */
  void CleanEntities() {
    std::erase_if(item_set,
                  [](const std::unique_ptr<cse491::Entity>& entityPtr) {
                    return entityPtr == nullptr;
                  });
  }

  /**
   * Removes an Entity from itemset (testing)
   * @param removeID The ID of the entity we're removing
   */
  void RemoveEntity(size_t removeID) {
    std::erase_if(item_set,
                  [removeID](const std::unique_ptr<cse491::Entity>& entityPtr) {
                    return entityPtr->GetID() == removeID;
                  });

    CleanEntities();
  }

  /**
   * Prints the entities in item_set (testing)
   */
  void PrintEntities() {
    for (const auto& elem : item_set) {
      if (!elem) continue;
      std::cout << elem->GetName() << "\n";
    }
    std::cout << std::endl;
  }
};
}  // namespace group4
