#include <gtest/gtest.h>
#include <wfc/wfc2d.hpp>

// Test case to verify the initialization of WaveFunctionCollapse2D
TEST(WFC2DTest, InitializationTest) {
    wfc2d::WaveFunctionCollapse2D wfc2d;

    const int ROWS = 3;
    const int COLS = 4;

    ASSERT_FALSE(wfc2d.isInitialized());

    wfc2d.initialize(ROWS, COLS);

    ASSERT_EQ(wfc2d.size(), ROWS * COLS);
    ASSERT_TRUE(wfc2d.isInitialized());
}

// Test case to verify the number of rows in the output grid
TEST(WFC2DTest, RowsTest) {
    wfc2d::WaveFunctionCollapse2D wfc2d;

    const std::vector<std::pair<int, int>> testCases = {
        {1, 1},     // Smallest grid
        {3, 4},     // Regular case
        {2, 2},     // Small grid
        {5, 5},     // Square grid
        {10, 5},    // Rectangular grid
    };

    for (const auto& testCase : testCases) {
        const int ROWS = testCase.first;
        const int COLS = testCase.second;

        wfc2d.initialize(ROWS, COLS);

        size_t totalElements = ROWS * COLS;
        size_t elementsModCols = totalElements % COLS;
        size_t calculatedRows = elementsModCols == 0 ? totalElements / COLS : totalElements / COLS + 1;

        EXPECT_EQ(calculatedRows, ROWS);
    }
}

TEST(WFC2DTest, ColumnsTest) {
    wfc2d::WaveFunctionCollapse2D wfc2d;

    const std::vector<std::pair<int, int>> testCases = {
        {1, 1},     // Smallest grid
        {4, 3},     // Regular case
        {2, 2},     // Small grid
        {5, 5},     // Square grid
        {5, 10},    // Rectangular grid
    };

    for (const auto& testCase : testCases) {
        const int ROWS = testCase.first;
        const int COLS = testCase.second;

        wfc2d.initialize(ROWS, COLS);

        size_t totalElements = ROWS * COLS;
        size_t elementsModRows = totalElements % ROWS;
        size_t calculatedCols = elementsModRows == 0 ? totalElements / ROWS : totalElements / ROWS + 1;

        EXPECT_EQ(calculatedCols, COLS);
    }

    wfc2d.run();
}

// Test case to check the correctness of getNeighboringIndices method
TEST(WFC2DTest, NeighboringIndicesTest) {
    wfc2d::WaveFunctionCollapse2D wfc2d;

    const int ROWS = 3;
    const int COLS = 3;

    wfc2d.initialize(ROWS, COLS);

    // Test for an interior cell
    std::vector<size_t> neighbors = wfc2d.getNeighboringIndices(4); // Index of a cell in the middle of the grid
    ASSERT_EQ(neighbors.size(), 4);
    EXPECT_EQ(neighbors[0], 1); // Up
    EXPECT_EQ(neighbors[1], 7); // Down
    EXPECT_EQ(neighbors[2], 3); // Left
    EXPECT_EQ(neighbors[3], 5); // Right

    // Test for a corner cell
    neighbors = wfc2d.getNeighboringIndices(0); // Index of the top-left corner cell
    ASSERT_EQ(neighbors.size(), 2);
    EXPECT_EQ(neighbors[0], 3); // Down
    EXPECT_EQ(neighbors[1], 1); // Right

    // Test for a cell on the top edge
    neighbors = wfc2d.getNeighboringIndices(1); // Index of a cell on the top edge
    ASSERT_EQ(neighbors.size(), 3);
    EXPECT_EQ(neighbors[0], 4); // Down
    EXPECT_EQ(neighbors[1], 0); // Left
    EXPECT_EQ(neighbors[2], 2); // Right
}

TEST(WFC2DTest, ParsesTileOptionsCorrectly) {
    wfc2d::WaveFunctionCollapse2D wfc2d;

    const int ROWS = 3;
    const int COLS = 3;

    wfc2d.initialize(ROWS, COLS);

    // Call the parseRulesFromFile function with the test file path
    std::string testFilePath = "test_tile_options.txt";
    const auto tiles = wfc2d.parseRulesFromFile(testFilePath);

    int counter = 0;
    for (const auto &tile : tiles) {
        std::cout << "[TILE_" << (counter++) << "]\n";
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "Direction " << i << ": " << tile.options[i].to_string() << std::endl;
        }
        std::cout << std::endl;
    }

    
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
