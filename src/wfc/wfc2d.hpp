#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <bitset>
#include <array>
#include <random>

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
            static constexpr size_t BITSET_SIZE = 8;
            
            // Public typedefs for user convenience
            using TileOptions = std::array<std::bitset<BITSET_SIZE>, 4>;
            using CallbackFn = std::function<void()>; /**< Type for callback function. */
            
            struct Tile {
                TileOptions options;
                size_t entropy;
                bool collapsed;
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

                this->m_outputGrid.resize(rows * cols);
                this->m_waveGrid.resize(rows * cols);

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

    std::vector<Tile> tiles;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (line.find("[TILE_") != std::string::npos) {
            std::cout << "New tile detected: " << line << std::endl;
            // Extract tile ID from the line
            Tile currentTile;
            while (std::getline(inputFile, line)) {
                if (line.empty() || line[0] == '[') {
                    // If the line is empty or starts with '[', it's the beginning of a new tile
                    break;
                }
                std::istringstream iss(line);
                if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                    // Parse option and update currentTile
                    auto parseOption = [&currentTile](const std::string& key, const std::string& value) {
                        if (key.find(key) != std::string::npos) {
                            std::istringstream vss(value);
                            size_t optionTileID;
                            size_t index = 0;
                            if(key == "up") index = 0;
                            if(key == "down") index = 1;
                            if(key == "left") index = 2;
                            if(key == "right") index = 3;

                            while (vss >> optionTileID) {
                                currentTile.options[index].set(optionTileID);
                            }
                        }
                    };
                    
                    parseOption(key, value);
                }
            }
            tiles.push_back(currentTile);
        }
    }

    return tiles;
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

                // Seed random number generator
                std::random_device rd;
                std::default_random_engine generator(rd());

                // Choose a random index from the output grid
                std::uniform_int_distribution<size_t> distribution(0, m_outputGrid.size() - 1);
                size_t randomIndex = distribution(generator);

                // Main loop: iterate over each cell in the output grid
                for (size_t i = 0; i < this->m_outputGrid.size(); ++i) {
                    
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
                return this->m_outputGrid.size();
            }

            /**
             * @brief Accesses the pattern at the specified index in the output grid using bounds checking.
             * 
             * @param index Index of the pattern to access.
             * @return Reference to the pattern at the specified index.
             * @throws std::out_of_range if index is out of range.
             */
            const size_t& at(size_t index) const {
                return this->m_outputGrid.at(index);
            }

            /**
             * @brief Accesses the pattern at the specified index in the output grid without bounds checking.
             * 
             * @param index Index of the pattern to access.
             * @return Reference to the pattern at the specified index.
             * @warning No bounds checking is performed. Accessing an out-of-range index leads to undefined behavior.
             */
            const size_t& operator[](size_t index) const {
                return this->m_outputGrid[index];
            }

            /**
             * @brief Returns an iterator to the beginning of the output grid.
             * 
             * @return Iterator to the beginning of the output grid.
             */
            iterator begin() {
                return iterator(this->m_outputGrid, 0);
            }

            /**
             * @brief Returns an iterator to the end of the output grid.
             * 
             * @return Iterator to the end of the output grid.
             */
            iterator end() {
                return iterator(this->m_outputGrid, this->m_outputGrid.size());
            }

            /**
             * @brief Applies the superposition principle.
             * 
             * This method applies the superposition principle in the Wave Function Collapse algorithm.
             */
            void propagate() { }

            /**
             * @brief Collapses the wave function.
             * 
             * This method collapses the wave function in the Wave Function Collapse algorithm.
             * 
             * @return True if the wave function collapse is successful, false otherwise.
             */
            bool collapse() { return true; }

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

        private:
            size_t m_gridWidth{ 0 };
            size_t m_gridHeight{ 0 };

            std::vector<size_t> m_outputGrid; /**< Output grid containing the collapsed patterns. */
            std::vector<Tile> m_waveGrid;

            bool m_initialized{ false }; /**< Flag indicating whether the algorithm is initialized. */
        };

    } // end of namespace internal

    // User-facing interface class
    class WaveFunctionCollapse2D : public internal::WaveFunctionCollapse2DImpl {
    public:
        using internal::WaveFunctionCollapse2DImpl::WaveFunctionCollapse2DImpl;
    };

} // end of namespace wfc2d