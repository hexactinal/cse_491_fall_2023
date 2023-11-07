# Group 4: Scripted Language Description

## SecondWorld.hpp
- Everything is in the `group4` namespace

### Constants
- `FIRST_FLOOR_FILENAME`: Filename for the first floor grid file. This file should be in `/assets/grids/`.
- `SECOND_FLOOR_FILENAME`: Filename for the second floor grid file. This file should be in `/assets/grids/`.
- `FINAL_FLOOR_FILENAME`: Filename for the last floor grid file. Should be assigned to the last floor filename listed above.

### Functions
- `SecondWorld()`: Builds out the grid for `FIRST_FLOOR_FILENAME`.
It does not call `LoadFromFile()`, so do not expect a world with other agents if you use this constructor.
- `SecondWorld(std::string grid_filename, std::string agent_filename)`: Builds out the grid for `grid_filename` and loads the JSON file `agent_filename`.
It will populate the world with agents.
- It uses the default destructor because we don't expect to derive any classes from `SecondWorld`.
- `void LoadFromFile(const std::string& input_filename)`: Loads data from a JSON file and adds agents with specified properties into the world.
    - example properties: `chocolate_bar` will increase an agent's max health by 10
    - example JSON file:
    ```json
    [
        {
            "name": "Imp",
            "x": 3,
            "y": 1,
            "entities": [
                "chocolate_bar"
            ],
            "hasInventory": false
        },
        {
            "name": "Pinky",
            "x": 11,
            "y": 2,
            "entities": [
                
            ],
            "hasInventory": false
        }
    ]
    ```
- `void SaveToFile()`: This function gives us an output.json file using nlohmann::json library.
- `int DoAction(cse491::AgentBase& agent, size_t action_id)`: Allows agents to move around the maze.
Also checks if the next agent position will be the win flag.
If the player interface gets to the win flag, then it moves on to the next floor (if there are any floors left).
- `void PrintEntities()`: Prints the entities in item_set (testing).

## Language.hpp
- All language functions are in the `worldlang` namespace

### PEGTL structures
- `grammar`: Appears to be currently unused as of writing.
- `number`: Something that can be represented with regex `\-?[0-9]+(.[0-9]+)?`
- `identifier`:
- `identifier_list`:
- `op_prio_add`:
- todo: do we want to document every part of the language here?

### Functions
- `std::vector<Unit> parse_to_code(std::string program)`: Converts a program string into code units using PEGTL
    - Will print out "Parse error!" if a parse error occurred while processing at the end of the function.
    - `program` should be a newline-separated string of operations.
        - example: `std::string program = "test()\ntest(1)\nwow=3\na=2+test(123*5+4-6/7,abc)-wow\ntest(123-a,435,7650+8*8)\n";`

## ProgramExecutor.hpp
- All code is inside the `ProgramExecutor` class
- Only has a default constructor

### Functions
- `void registerFunction(std::string name, Callable callable)`: Registers the lambda function `callable` with name `name` to the executor.
- todo: do we want to document every part of the executor here?