/**
* @date [12/17/2024]
 * @author [Farhana Sultana]
 */
#include "RecipeBook.hpp"

// Default constructor
RecipeBook::RecipeBook() : BinarySearchTree<Recipe>() {}

// Parameterized constructor
RecipeBook::RecipeBook(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    // Ignore the first line (header)
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, difficulty, description, mastered;

        std::getline(ss, name, ',');
        std::getline(ss, difficulty, ',');
        std::getline(ss, description, ',');
        std::getline(ss, mastered, ',');

        Recipe recipe(name, std::stoi(difficulty), description, mastered == "1");
        this->add(recipe);
    }

    file.close();
}

std::shared_ptr<BinaryNode<Recipe>> findRecipeHelper(std::shared_ptr<BinaryNode<Recipe>> subtree_ptr, const Recipe& target){
    // Uses a binary search
    if (subtree_ptr == nullptr)
        return subtree_ptr; // Not found
    else if ((subtree_ptr->getItem() == target))
        return subtree_ptr; // Found
    else if (subtree_ptr->getItem() > target)
        // Search left subtree
        return findRecipeHelper(subtree_ptr->getLeftChildPtr(), target);
    else
        // Search right subtree
        return findRecipeHelper(subtree_ptr->getRightChildPtr(), target);
}

// Find a recipe by name
std::shared_ptr<BinaryNode<Recipe>> RecipeBook::findRecipe(const std::string& name) const {
    Recipe target;
    target.name_ = name;
    // Call the helper function to search in the tree starting from the root
    std::shared_ptr<BinaryNode<Recipe>> root_ptr = this->getRoot();
    std::shared_ptr<BinaryNode<Recipe>> found_node = findRecipeHelper(root_ptr, target);

    // Return the found recipe if the node is found, otherwise return nullptr
    if (found_node != nullptr) {
        return found_node;  // Return pointer to the Recipe item
    } else {
        return nullptr;  // Recipe not found
    }
}

// Add a recipe
bool RecipeBook::addRecipe(const Recipe& recipe) {
    if (findRecipe(recipe.name_) != nullptr) {
        return false; // Recipe with the same name already exists
    }
    this->add(recipe);
    return true;
}

// Remove a recipe by name
bool RecipeBook::removeRecipe(const std::string& name) {
    Recipe recipe;
    recipe.name_ = name;
    return this->remove(recipe);
}

// Clear all recipes from the tree
void clearSubtree (std::shared_ptr<BinaryNode<Recipe>> node) {
    if (node == nullptr) return;

    clearSubtree(node->getLeftChildPtr());
    clearSubtree(node->getRightChildPtr());
    node.reset();
};

void RecipeBook::clear() {
    auto root = this->getRoot();

    clearSubtree(root);
    this->setRoot(nullptr);
}

void calculateMasteryPointsHelper(std::shared_ptr<BinaryNode<Recipe>> node, const Recipe& target, int& masteryPoints)  {
    if (node == nullptr) return;

    calculateMasteryPointsHelper(node->getLeftChildPtr(), target, masteryPoints);

    const Recipe& currentRecipe = node->getItem();
    if (currentRecipe.difficulty_level_ < target.difficulty_level_ && !currentRecipe.mastered_) {
        masteryPoints++;
    }

    calculateMasteryPointsHelper(node->getRightChildPtr(), target, masteryPoints);
}
// Calculate mastery points
int RecipeBook::calculateMasteryPoints(const std::string& name) const {
    std::shared_ptr<BinaryNode<Recipe>> target = findRecipe(name);
    if (!target) {
        return -1; // Recipe not found
    }

    if (target->getItem().mastered_) {
        return 0; // Recipe is already mastered
    }

    int masteryPoints = 1; // Start with 1 point for the current recipe
    calculateMasteryPointsHelper(this->getRoot(), target->getItem(), masteryPoints);
    return masteryPoints;
}

void collectRecipes(std::shared_ptr<BinaryNode<Recipe>> node, std::vector<Recipe>& recipes)  {
    if (node == nullptr) return;

    collectRecipes(node->getLeftChildPtr(), recipes);
    recipes.push_back(node->getItem());
    collectRecipes(node->getRightChildPtr(), recipes);
}

void RecipeBook::buildBalancedTree(const std::vector<Recipe>& recipes, int start, int end) {
    if (start > end) return;

    int mid = start + (end - start) / 2;
    this->addRecipe(recipes[mid]);

    buildBalancedTree(recipes, start, mid - 1);
    buildBalancedTree(recipes, mid + 1, end);
}

void RecipeBook::balance() {
    std::vector<Recipe> sortedRecipes;
    collectRecipes(this->getRoot(), sortedRecipes);
    this->clear();
    buildBalancedTree(sortedRecipes, 0, sortedRecipes.size() - 1);
}





void displayPreorderHelper(std::shared_ptr<BinaryNode<Recipe>> node){
    if (node == nullptr)
    {
        return;
    }
    std::cout << node->getItem() << std::endl;
    displayPreorderHelper(node->getLeftChildPtr());
    displayPreorderHelper(node->getRightChildPtr());
}

// Display the tree in preorder traversal
void RecipeBook::preorderDisplay() const{
    std::shared_ptr<BinaryNode<Recipe>> root = this->getRoot();
    displayPreorderHelper(root);
}


