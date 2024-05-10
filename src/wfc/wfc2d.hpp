#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <bitset>
#include <array>
#include <random>
#include <limits>
#include <queue>
#include <stack>

namespace wfc2d {

    namespace internal {

        /**
         * @brief Implementation of Wave Function Collapse algorithm in 2D.
         * 
         */
        class WaveFunctionCollapse2DImpl {  

            friend class WaveFunctionCollapse2D;

            enum class EDirections { Up, Down, Left, Right };

        public:
            static constexpr size_t NUM_OPTION_DIRECTIONS = 4; // up, down, left, right

            static constexpr size_t BITSET_SIZE = 8; // Example size for options bitset
            
            using CallbackFn = std::function<void()>;
            
            struct Tile {
                std::bitset<BITSET_SIZE> options[NUM_OPTION_DIRECTIONS];
                size_t entropy;
                bool collapsed{false};
            };

            /**
             * @brief Custom iterator for OutputGrid.
             */
            class iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = size_t;
                using difference_type = std::ptrdiff_t;
                using pointer = size_t*;
                using reference = size_t&;

                /**
                 * @brief Constructs an iterator.
                 * 
                 * @param grid Reference to the output grid.
                 * @param index Index of the iterator.
                 */
                iterator(std::vector<size_t>& grid, size_t index) 
                    : grid(grid), index(index) {}

                reference operator*() const {
                    return grid[index];
                }

                iterator& operator++() {
                    ++index;
                    return *this;
                }

                bool operator==(const iterator& other) const {
                    return index == other.index;
                }

                bool operator!=(const iterator& other) const {
                    return !(*this == other);
                }

            private:
                std::vector<size_t>& grid;
                size_t index;
            };

            // Public methods for user interaction
            WaveFunctionCollapse2DImpl() = default;
            virtual ~WaveFunctionCollapse2DImpl() = default;
            WaveFunctionCollapse2DImpl(const WaveFunctionCollapse2DImpl&) = delete;
            WaveFunctionCollapse2DImpl(WaveFunctionCollapse2DImpl&&) = delete;
            WaveFunctionCollapse2DImpl& operator=(const WaveFunctionCollapse2DImpl&) = delete;
            WaveFunctionCollapse2DImpl& operator=(WaveFunctionCollapse2DImpl&&) = delete;

        public:
            /**
            * @brief Enum class for heuristic type.
            */
            enum class EHeuristic { Entropy };

            /**
             * @brief Initializes the Wave Function Collapse algorithm.
             * 
             * @param rows Number of rows in the grid.
             * @param cols Number of columns in the grid.
             */
            void initialize(size_t rows, size_t cols) {
                assert(rows * cols > 0);

                this->m_wave.resize(rows * cols);

                // Initialize wave grid with maximum entropy
                for (auto& tile : this->m_wave) {
                    for (auto& option : tile.options) {
                        option.set();
                    }

                    tile.entropy = m_ruleset.size();
                    tile.collapsed = false;
                }

                this->m_output.resize(rows * cols);
                std::fill(this->m_output.begin(), this->m_output.end(), std::numeric_limits<std::size_t>::max());

                this->m_gridWidth = cols;
                this->m_gridHeight = rows;

                this->m_initialized = true;
            }

            std::vector<Tile> parseRulesFromFile(const std::string& filepath) {
                std::ifstream inputFile(filepath);
                if (!inputFile.is_open()) {
                    std::cerr << "Error: Unable to open file.\n";
                    return {};
                }

                std::string line;
                while (std::getline(inputFile, line)) {
                    std::istringstream iss(line);
                    std::string key, value;
                    if (line.find("[TILE_") != std::string::npos) {
                        // Extract tile ID from the line
                        Tile currentTile;
                        while (std::getline(inputFile, line)) {
                            if (line.empty() || line[0] == '[') {
                                // If the line is empty or starts with '[', it's the beginning of a new tile
                                break;
                            }

                            std::istringstream iss(line);
                            if (!(std::getline(iss, key, '=') && std::getline(iss, value))) {
                                std::cerr << "Error: Failed to parse line: " << line << std::endl;
                                continue; // Skip to the next line
                            }

                            // Parse option and update currentTile
                            auto parseOption = [&currentTile](const std::string& key, const std::string& value) {
                                size_t index = 0;
                                if(key == "up") index = 0;
                                else if(key == "down") index = 1;
                                else if(key == "left") index = 2;
                                else if(key == "right") index = 3;

                                std::istringstream vss(value);
                                size_t optionTileID;
                                while (vss >> optionTileID) {
                                    currentTile.options[index].set(optionTileID, true);
                                }
                            };
                                                    
                            parseOption(key, value);
                            
                        }

                        this->m_ruleset.push_back(currentTile);
                    }
                }

                inputFile.close();

                return this->m_ruleset;
            }

            /**
             * @brief Runs the Wave Function Collapse algorithm.
             */
            void run() {
                if (!this->m_initialized) {
                    std::cerr << "Error: WaveFunctionCollapse2D not initialized.\n";
                    return;
                }

                // Algorithm logic here...

                // Choose a random tile index
                size_t randomTileIndex = random(0, m_wave.size() - 1);

                // Choose a random direction index
                size_t randomDirectionIndex = random(0, NUM_OPTION_DIRECTIONS - 1);

                // Choose a random option index
                size_t randomOptionIndex = random(0, BITSET_SIZE - 1);

                // Set the random option for the random direction of the random tile
                m_wave[randomTileIndex].options[randomDirectionIndex].set(randomOptionIndex);

                // Print the chosen random option
                std::cout << "Chosen random option for Tile " << randomTileIndex
                        << ", Direction " << randomDirectionIndex
                        << ": " << randomOptionIndex << std::endl;   

                m_output[randomTileIndex] = randomOptionIndex;

                collapse(randomTileIndex);
            }

            /**
             * @brief Collapses the wave function.
             * 
             * This method collapses the wave function in the Wave Function Collapse algorithm.
             * 
             * @return True if the wave function collapse is successful, false otherwise.
             */
            bool collapse(size_t index) { 
                if (m_wave[index].collapsed) {
                    return true;
                }

                m_wave[index].collapsed = true;
                
                return true; 
            }

            /**
             * @brief Applies the superposition principle.
             * 
             * This method applies the superposition principle in the Wave Function Collapse algorithm.
             */
            void propagate() { 
                for (size_t index = 0; index < m_wave.size(); ++index) {
                    size_t row = index / this->m_gridWidth;
                    size_t col = index % this->m_gridWidth;

                    auto neighbors = getNeighboringIndices(index);
                    for (auto neighborIndex : neighbors) {
                        // Propagate logic here...
                    }
                }
            }

            /**
             * @brief Checks if the Wave Function Collapse algorithm is initialized.
             * 
             * @return True if initialized, false otherwise.
             */

            bool isInitialized() const {
                return this->m_initialized;
            }

            /**
             * @brief Gets the size of the output grid.
             * 
             * @return Size of the output grid.
             */
            size_t size() const {
                return this->m_output.size();
            }

            /**
             * @brief Accesses the pattern at the specified index in the output grid using bounds checking.
             * 
             * @param index Index of the pattern to access.
             * @return Reference to the pattern at the specified index.
             * @throws std::out_of_range if index is out of range.
             */
            const size_t& at(size_t index) const {
                return this->m_output.at(index);
            }

            /**
             * @brief Accesses the pattern at the specified index in the output grid without bounds checking.
             * 
             * @param index Index of the pattern to access.
             * @return Reference to the pattern at the specified index.
             * @warning No bounds checking is performed. Accessing an out-of-range index leads to undefined behavior.
             */
            const size_t& operator[](size_t index) const {
                return this->m_output[index];
            }

            /**
             * @brief Returns an iterator to the beginning of the output grid.
             * 
             * @return Iterator to the beginning of the output grid.
             */
            iterator begin() {
                return iterator(this->m_output, 0);
            }

            /**
             * @brief Returns an iterator to the end of the output grid.
             * 
             * @return Iterator to the end of the output grid.
             */
            iterator end() {
                return iterator(this->m_output, this->m_output.size());
            }

            void print() const {
                for (size_t row = 0; row < m_gridHeight; ++row) {
                    for (size_t col = 0; col < m_gridWidth; ++col) {
                        size_t index = row * m_gridWidth + col;
                        std::cout << m_output[index] << " ";
                    }
                    std::cout << std::endl;
                }
            }

            /**
             * @brief Gets the neighboring indices of a given cell.
             * 
             * This method calculates the neighboring indices of a cell in the grid based on its current index.
             * 
             * @param currentIndex The index of the current cell.
             * @return A vector containing the neighboring indices of the current cell: UP, DOWN, LEFT, RIGHT.
             * 
             * The neighboring indices are calculated based on the current index in the grid.
             * The order of neighboring indices in the returned vector is UP, DOWN, LEFT, RIGHT.
             * If a neighboring cell exists in the upward direction, its index is included first.
             * If a neighboring cell exists in the downward direction, its index is included second.
             * If a neighboring cell exists in the leftward direction, its index is included third.
             * If a neighboring cell exists in the rightward direction, its index is included last.
             */
            std::vector<size_t> getNeighboringIndices(size_t currentIndex) {
                std::vector<size_t> neighboringIndices;

                size_t row = currentIndex / this->m_gridWidth;
                size_t col = currentIndex % this->m_gridWidth;

                if (row > 0) {
                    neighboringIndices.push_back(currentIndex - this->m_gridWidth); // Up
                }

                if (row < this->m_gridHeight - 1) {
                    neighboringIndices.push_back(currentIndex + this->m_gridWidth); // Down
                }

                if (col > 0) {
                    neighboringIndices.push_back(currentIndex - 1);                 // Left
                }

                if (col < this->m_gridWidth - 1) {
                    neighboringIndices.push_back(currentIndex + 1);                 // Right
                }

                return neighboringIndices;
            }

            // Utility function to generate a random integer
            size_t random(int min, int max) const {
                std::random_device rd;
                std::default_random_engine generator(rd());
                std::uniform_int_distribution<size_t> distribution(min, max); 
                return distribution(generator);
            }

        private:
            size_t m_gridWidth{ 0 };
            size_t m_gridHeight{ 0 };

            std::vector<Tile> m_wave;
            std::vector<size_t> m_output;
            std::vector<Tile> m_ruleset;
            bool m_initialized{ false }; /**< Flag indicating whether the algorithm is initialized. */
        };

    } // end of namespace internal

    // User-facing interface class
    class WaveFunctionCollapse2D : public internal::WaveFunctionCollapse2DImpl {
    public:
        using internal::WaveFunctionCollapse2DImpl::WaveFunctionCollapse2DImpl;
    };

} // end of namespace wfc2d