/******************
Name:omri halfon
ID:324209402
Assignment:ex1
*******************/
#include "ex6.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128
# define INITIAL_CAPACITY 100
# define ONE_INDEX 1
# define DUPLICATE_CAPACITY 2
# define SWAPPED 1
# define MINIMUM_TO_FIGHT 2

//------------------------------------------------------------------
// 1) Utility: trim white space - Remove leading/trailing whitespace
//-------------------------------------------------------------------
void trimWhitespace(char *str) {
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0) {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int) strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

//---------------------------------------------------------------
// 2) return newly allocated copy of src
//---------------------------------------------------------------
char *myStrdup(const char *src) {
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *) malloc(len + 1);
    if (!dest) {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

// --------------------------------------------------------------
// 3) Safe integer reading
// --------------------------------------------------------------
int readIntSafe(const char *prompt) {
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success) {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0) {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int) strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0') {
            printf("Invalid input.\n");
        } else {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 4) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type) {
    switch (type) {
        case GRASS:
            return "GRASS";
        case FIRE:
            return "FIRE";
        case WATER:
            return "WATER";
        case BUG:
            return "BUG";
        case NORMAL:
            return "NORMAL";
        case POISON:
            return "POISON";
        case ELECTRIC:
            return "ELECTRIC";
        case GROUND:
            return "GROUND";
        case FAIRY:
            return "FAIRY";
        case FIGHTING:
            return "FIGHTING";
        case PSYCHIC:
            return "PSYCHIC";
        case ROCK:
            return "ROCK";
        case GHOST:
            return "GHOST";
        case DRAGON:
            return "DRAGON";
        case ICE:
            return "ICE";
        default:
            return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// 5) Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput() {
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *) malloc(capacity);
    if (!input) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char *temp = (char *) realloc(input, capacity);
            if (!temp) {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char) c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// -----------------------------------------------------------------
// 6) Utility: clearInputBuffer - clear input buffer
// -----------------------------------------------------------------
void clearInputBuffer() {
    int Char;
    while ((Char = getchar()) != '\n' && Char != EOF);
}

//------------------------------------------------------------------
// 7) Utility: add new Owner to list
//------------------------------------------------------------------
void addOwnerToList(char *ownerName, PokemonNode *starter) {
    // Check if the input parameters are valid
    // If either the ownerName or the starter Pokemon is NULL, print an error and return
    if (!ownerName || !starter) {
        printf("Invalid input: ownerName or starter is NULL.\n");
        return;
    }

    // Create a new OwnerNode with the provided owner name and starter Pokemon
    OwnerNode *newOwner = createOwner(ownerName, starter);

    // Add the new owner to the circular linked list
    linkOwnerInCircularList(newOwner);

    // Print a confirmation message with the owner's name and the starter Pokemon's name
    printf("New Pokedex created for %s with starter %s.\n", ownerName, starter->data->name);
}

//------------------------------------------------------------------
// 8) Function to create new Pokemon node
//------------------------------------------------------------------
PokemonNode *createPokemonNode(const PokemonData *data) {
    //malloc new node for new pokemon size of one pokemon
    PokemonNode *newPokemonNode = malloc(sizeof(PokemonNode));
    //If malloc failed print memory allocation error return null
    if (!newPokemonNode) {
        printf("Memory allocation error.\n");
        return NULL;
    }
    //Malloc new data for Pokemon
    newPokemonNode->data = malloc(sizeof(PokemonData));
    //If malloc failed print memory allocation error free the new Pokemon return null
    if (!newPokemonNode->data) {
        printf("Memory allocation error.\n");
        free(newPokemonNode);
        return NULL;
    }
    //Pointer of new data point to consistent pokemon data
    *(newPokemonNode->data) = *data;
    //initialize the left/right "leaves" to null
    newPokemonNode->left = NULL;
    newPokemonNode->right = NULL;
    //return new node
    return newPokemonNode;
}

//------------------------------------------------------------------
// 9) Function to create new owner
//------------------------------------------------------------------
OwnerNode *createOwner(char *ownerName, PokemonNode *starter) {
    //Malloc new owner
    OwnerNode *newOwner = malloc(sizeof(OwnerNode));
    //if malloc failed print memory allocation error exit failure
    if (!newOwner) {
        printf("Memory allocation error\n");
        exit(Failure);
    }
    //Copy owner name into new owner , get the first pointer of Pokedex root
    newOwner->ownerName = myStrdup(ownerName);
    newOwner->pokedexRoot = starter;

    //initialize next and previous owner of the new owner to null
    newOwner->next = NULL;
    newOwner->prev = NULL;

    //return new owner
    return newOwner;
}

//------------------------------------------------------------------
// 10) Function to free specific Pokemon in the list
//------------------------------------------------------------------
void freePokemonNode(PokemonNode *node) {
    //if no Pokemon on the list exit (return)
    if (!node) return;
    //If node data exist free node data change the pointer to null
    if (node->data) {
        free(node->data);
        node->data = NULL;
    }
    //free the whole node change it pointer to null
    free(node);
    node = NULL;
}

//-----------------------------------------------------------------
// 11) Function to free the whole tree of Pokemons of specific owner
//-----------------------------------------------------------------
void freePokemonTree(PokemonNode *root) {
    //If the tree is empty exit the function
    if (!root) return;

    //If left is not empty recursive call to the next left leaf change pointer to null
    if (root->left) {
        freePokemonTree(root->left);
        root->left = NULL;
    }

    //If right is not empty recursive call to the next right leaf change pointer to null
    if (root->right) {
        freePokemonTree(root->right);
        root->right = NULL;
    }

    //if data of the current leaf exist free it change the pointer of the data to null
    if (root->data) {
        free(root->data);
        root->data = NULL;
    }

    //free the whole single Pokemon , change the pointer to null
    free(root);
    root = NULL;
}

//----------------------------------------------------------------
// 12) Function to free specific owner
// ---------------------------------------------------------------
void freeOwnerNode(OwnerNode *owner) {
    //If owner is null exit from the function
    if (!owner) return;

    //If owner has owner name , free the name ,change pointer to null
    if (owner->ownerName) {
        free(owner->ownerName);
        owner->ownerName = NULL;
    }

    //If owner has Pokemones free the whole tree of Pokemones ,change the first pointer to null
    if (owner->pokedexRoot) {
        freePokemonTree(owner->pokedexRoot);
        owner->pokedexRoot = NULL;
    }

    //free the owner change its value to null
    free(owner);
    owner = NULL;
}

//----------------------------------------------------------------
// 13) Function to insert new Pokemon into the tree
//----------------------------------------------------------------
PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode) {
    //If root is null return the new node
    if (!root) {
        return newNode;
    }
    //Sorting the tree by id , insert the new node by the correct order
    if (newNode->data->id < root->data->id) {
        root->left = insertPokemonNode(root->left, newNode);
    } else {
        root->right = insertPokemonNode(root->right, newNode);
    }

    return root;
}

//-----------------------------------------------------------------
// 14) Function to search Pokemon in tree return its pointer
//-----------------------------------------------------------------
PokemonNode *searchPokemonBFS(PokemonNode *root, int id) {
    //pointer change by the loop
    PokemonNode *currentPokemon = root;
    //loop while through all the leaves of the tree
    while (currentPokemon) {
        //if current Pokemon's ID equals the searched ID , return the pointer of the Pokemon
        if (currentPokemon->data->id == id) {
            return currentPokemon;
        }
        //move to the next leaves by changing the pointer of current
        else if (currentPokemon->data->id > id) {
            currentPokemon = currentPokemon->left;
        } else {
            currentPokemon = currentPokemon->right;
        }
    }
    //If the searched ID hasn't found return current(NULL)
    return currentPokemon;
}

//-----------------------------------------------------------------
// 15) Utility: check if specific Pokemon exist in tree
//------------------------------------------------------------------
int isPokemonInTree(PokemonNode *root, int id) {
    //When all the leaves checked and there is no the pokemon in the tree, return False
    if (!root) {
        return False;
    }

    //If the current leaf's ID equals to the searched ID , return True
    if (root->data->id == id) {
        return True;
    }

    //call the function by recursive calls , checking all the leaves
    if (id < root->data->id) {
        return isPokemonInTree(root->left, id);
    } else {
        return isPokemonInTree(root->right, id);
    }
}

//------------------------------------------------------------------
// 16) Function to remove specific Pokemon from tree
//------------------------------------------------------------------
PokemonNode *removeNodeBST(PokemonNode *root, int id) {
    //if root is empty return null
    if (!root) return NULL;

    //call the function by recursive call through all the leaves till finding the searched ID
    if (id < root->data->id) {
        root->left = removeNodeBST(root->left, id);
    } else if (id > root->data->id) {
        root->right = removeNodeBST(root->right, id);
    } else {
        //If the node asked to remove hasn't sons ,free the node ,return null
        if (!root->left && !root->right) {
            freePokemonNode(root);
            return NULL;
        }

        //If the node which asked to remove has single "son"
        else if (!root->left || !root->right) {
            //get the child of the node replace it with the node change the value to null
            PokemonNode *child = root->left ? root->left : root->right;
            root->left = root->right = NULL;
            //free the node which asked to remove, return the single "son"
            freePokemonNode(root);
            return child;
        }
        //If the node which asked to remove has two "sons"
        else {
            //get the right leaf of the node
            PokemonNode *successor = root->right;
            //loop while to get the Pokemon we want to replace with the node
            while (successor->left) {
                successor = successor->left;
            }
            //replace all data , not changing the pointer
            *(root->data) = *(successor->data);
            //remove the node we've replaced with the node that asked to be removed
            root->right = removeNodeBST(root->right, successor->data->id);
        }
    }
    return root;
}

//------------------------------------------------------------------
// 17) Function to remove Pokemon by specific ID from tree
//------------------------------------------------------------------
PokemonNode *removePokemonByID(PokemonNode *root, int id) {
    if (!root) {
        return NULL;
    }

    // Use BFS to confirm the Pokémon exists (optional, but matches your logic)
    if (!searchPokemonBFS(root, id)) {
        printf("No Pokemon with ID %d found.\n", id);
        return root;
    }
    // Call removeNodeBST to handle the actual removal logic
    root = removeNodeBST(root, id);
    printf("Removing Pokemon %s (ID %d).\n", pokedex[id - 1].name, id);
    return root;
}

//------------------------------------------------------------------
// 18) Utility: generic breadth first search
//------------------------------------------------------------------
void BFSGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If the root is NULL, there's nothing to traverse
    if (!root) {
        return;
    }

    // Initial capacity for the queue
    int queueCapacity  = INITIAL_CAPACITY;
    int queueFront  = 0;
    int queueRear = 0;
    PokemonNode **queue = malloc(queueCapacity  * sizeof(PokemonNode **));

    // Allocate memory for the queue to hold pointers to PokemonNode
    if (!queue) {
        printf("Memory allocation error.\n");
        return;
    }

    // Enqueue the root node as the starting point
    queue[queueRear++] = root;

    // Traverse the tree level by level
    while (queueFront  < queueRear) {
        // Dequeue the front node and visit it
        PokemonNode *current = queue[queueFront ++];
        visit(current);

        // If the left child exists, enqueue it
        if (current->left) {
            // If the queue is full, double its capacity
            if (queueRear == queueCapacity ) {
                queueCapacity  *= DUPLICATE_CAPACITY;
                queue = realloc(queue, queueCapacity  * sizeof(PokemonNode **));
                // Exit if reallocation fails
                if (!queue) {
                    printf("Memory allocation error.\n");
                    return;
                }
            }
            queue[queueRear++] = current->left;
        }

        // If the right child exists, enqueue it
        if (current->right) {
            // If the queue is full, double its capacity
            if (queueRear == queueCapacity ) {
                queueCapacity  *= DUPLICATE_CAPACITY;
                queue = realloc(queue, queueCapacity  * sizeof(PokemonNode **));
                // Exit if reallocation fails
                if (!queue) {
                    printf("Memory allocation error.\n");
                    return;
                }
            }
            queue[queueRear++] = current->right;
        }
    }

    // Free the memory allocated for the queue after traversal
    free(queue);
}

//------------------------------------------------------------------
// 19) Utility: A generic pre-order traversal
//------------------------------------------------------------------
void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If the root is NULL, there's nothing to traverse
    if (!root) {
        return;
    }

    // Visit the current node (root)
    visit(root);

    // Recursively traverse the left subtree
    preOrderGeneric(root->left, visit);

    // Recursively traverse the right subtree
    preOrderGeneric(root->right, visit);
}

//------------------------------------------------------------------
// 20) Utility: A generic in-order traversal
//------------------------------------------------------------------
void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If the root is NULL, there's nothing to traverse
    if (!root) {
        return;
    }

    // Recursively traverse the left subtree
    inOrderGeneric(root->left, visit);

    // Visit the current node (root)
    visit(root);

    // Recursively traverse the right subtree
    inOrderGeneric(root->right, visit);
}

//------------------------------------------------------------------
// 21) Utility: A generic post-order traversal (Left-Right-Root)
//------------------------------------------------------------------
void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    // If the root is NULL, there's nothing to traverse
    if (!root) return;

    // Recursively traverse the left subtree
    postOrderGeneric(root->left, visit);

    // Recursively traverse the right subtree
    postOrderGeneric(root->right, visit);

    // Visit the current node (root)
    visit(root);
}

//------------------------------------------------------------------
// 22) Utility: Print one PokemonNode’s data: ID, Name, Type, HP, Attack, Evolve?
//------------------------------------------------------------------
void printPokemonNode(PokemonNode *node) {
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

//---------------------------------------------------------------------
// 23) Utility: Initialize a NodeArray with given capacity.
//---------------------------------------------------------------------
void initNodeArray(NodeArray *na, int cap) {
    // Check if the NodeArray pointer is NULL
    // If it is NULL, print an error message and return
    if (!na) {
        printf("Error: NodeArray pointer is NULL.\n");
        return;
    }

    // Allocate memory for the nodes array within the NodeArray structure
    // The array will hold pointers to PokemonNode, with an initial capacity specified by 'cap'
    na->nodes = malloc(cap * sizeof(PokemonNode **));

    // If memory allocation fails, print an error message and terminate the program
    if (!na->nodes) {
        printf("Memory allocation error.\n");
        exit(Failure);
    }

    // Initialize the size of the NodeArray to 0 (no nodes added yet)
    na->size = 0;

    // Set the capacity of the NodeArray to the value provided by 'cap'
    na->capacity = cap;
}

//-------------------------------------------------------------------
// 24) Function to Add a PokemonNode pointer to NodeArray
//-------------------------------------------------------------------
void addNode(NodeArray *na, PokemonNode *node) {
    // Check if the NodeArray or the PokemonNode is NULL
    // If either is NULL, print an error message and return early
    if (!na || !node) {
        printf("Error: NodeArray or PokemonNode is NULL.\n");
        return;
    }

    // Check if the NodeArray has reached its capacity
    // If the array is full, resize it to accommodate more nodes
    if (na->size == na->capacity) {
        // Double the capacity
        na->capacity *= DUPLICATE_CAPACITY;

        // Reallocate memory for the larger array
        na->nodes = realloc(na->nodes, na->capacity * sizeof(PokemonNode **));

        // If reallocation fails, print an error message and terminate the program
        if (!na->nodes) {
            printf("Memory allocation error during addNode.\n");
            exit(Failure);
        }
    }

    // Add the new PokemonNode to the array at the current position (na->size)
    na->nodes[na->size++] = node;

    // Increment the size of the NodeArray to reflect the addition
}

//-------------------------------------------------------------------
// 25) Utility: Recursively collect all nodes from the BST into a NodeArray
//---------------------------------------------------------------------

void collectAll(PokemonNode *root, NodeArray *na) {
    // Base case: If the root is NULL, there's nothing to collect
    if (!root) return;

    // Add the current node (root) to the NodeArray
    addNode(na, root);

    // Recursively collect nodes from the left subtree
    collectAll(root->left, na);

    // Recursively collect nodes from the right subtree
    collectAll(root->right, na);
}

//---------------------------------------------------------------------
// 26) Utility: Compare function for qsort (alphabetical by node->data->name)
//---------------------------------------------------------------------
int compareByNameNode(const void *a, const void *b) {
    // Cast the void pointers to pointers into PokemonNode pointers
    PokemonNode *nodeA = *(PokemonNode **) a;
    PokemonNode *nodeB = *(PokemonNode **) b;

    // Compare the names of the Pokemon stored in the nodes using strcmp
    return strcmp(nodeA->data->name, nodeB->data->name);
}

//--------------------------------------------------------------------
// 27) Function display all Pokemones by alphabetical order
//--------------------------------------------------------------------
void displayAlphabetical(PokemonNode *root) {
    // If the tree is empty (root is NULL), print a message and return
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }

    // Initialize a NodeArray to store pointers to the nodes
    NodeArray na;

    // Initial capacity is set to 100 , increase it when needed in the function below
    initNodeArray(&na, INITIAL_CAPACITY);

    // Collect all nodes from the tree into the NodeArray using pre-order traversal
    collectAll(root, &na);

    // Sort the nodes in the NodeArray alphabetically by the Pokémon names
    qsort(na.nodes, na.size, sizeof(PokemonNode *), compareByNameNode);

    // Iterate through the sorted NodeArray and print each node
    for (int i = 0; i < na.size; i++) {
        printPokemonNode(na.nodes[i]);
    }

    // Free the dynamically allocated memory for the NodeArray
    free(na.nodes);
}

//--------------------------------------------------------------------
// 28) Function display all Pokemones by BFS order
//--------------------------------------------------------------------
void displayBFS(PokemonNode *root) {
    // Check if the root is NULL (tree is empty)
    // If the tree is empty, print a message and return
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }

    // Perform a Breadth-First Search (BFS) traversal of the tree
    // Use the BFSGeneric function to traverse and print each node
    // The printPokemonNode function is passed as the visitor function
    BFSGeneric(root, printPokemonNode);
}

//--------------------------------------------------------------------
// 29) Function display all Pokemones by pre-order traversal
//-------------------------------------------------------------------
void preOrderTraversal(PokemonNode *root) {
    // Check if the root is NULL (tree is empty)
    // If the tree is empty, print a message and return
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }

    // Perform a Pre-Order Traversal of the tree
    // Use the preOrderGeneric function to traverse the tree
    // The printPokemonNode function is passed as the visitor function
    // This ensures that each node is visited and printed in the order: Root, Left, Right
    preOrderGeneric(root, printPokemonNode);
}

//-------------------------------------------------------------------
// 30) Function display all Pokemones in order traversal
//-------------------------------------------------------------------
void inOrderTraversal(PokemonNode *root) {
    // Check if the root is NULL (tree is empty)
    // If the tree is empty, print a message and return
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }
    // Perform an In-Order Traversal of the tree
    // Use the inOrderGeneric function to traverse the tree
    // The printPokemonNode function is passed as the visitor function
    // This ensures that each node is visited and printed in the order: Left, Root, Right
    inOrderGeneric(root, printPokemonNode);
}

//------------------------------------------------------------------
// 31) Function display all Pokemones by post-order traversal
//------------------------------------------------------------------
void postOrderTraversal(PokemonNode *root) {
    // Check if the root is NULL (tree is empty)
    // If the tree is empty, print a message and return
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }

    // Perform a Post-Order Traversal of the tree
    // Use the postOrderGeneric function to traverse the tree
    // The printPokemonNode function is passed as the visitor function
    // This ensures that each node is visited and printed in the order: Left, Right, Root
    postOrderGeneric(root, printPokemonNode);
}

//------------------------------------------------------------------
// 32) Utility: calculate the total power of specific Pokemon
//------------------------------------------------------------------
double totalPower(int id) {
    // Initialize a variable to hold the total power
    double TotalPower = 0;

    // Add the weighted attack value to the total
    // Attack value is multiplied by 1.5 for its contribution
    TotalPower += ((pokedex[id].attack) * 1.5);

    // Add the weighted HP (health points) value to the total
    // HP value is multiplied by 1.2 for its contribution
    TotalPower += ((pokedex[id].hp) * 1.2);

    //return the total power of Pokemon (double)
    return TotalPower;
}

//-----------------------------------------------------------------
// 33) Function: compare two Pokemones total power
//-----------------------------------------------------------------
void pokemonFight(OwnerNode *owner) {
    //If owner is empty or owner does not have pokemones return to menu
    if (!owner || !owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }

    //get two different IDs from the user
    int firstID = readIntSafe("Enter ID of the first Pokemon: ");
    int secondID = readIntSafe("Enter ID of the second Pokemon: ");

    //In case the pokemon does not exist in the tree return to the menu
    if (!isPokemonInTree(owner->pokedexRoot, firstID) || !isPokemonInTree(owner->pokedexRoot, secondID)) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }

    //decrease the IDs by one to get the index in Pokadex Data array
    firstID--;
    secondID--;

    //print the total score of the battle
    printf("Pokemon 1: %s (Score = %.2f)\nPokemon 2: %s (Score = %.2f)\n",
           pokedex[firstID].name, totalPower(firstID), pokedex[secondID].name, totalPower(secondID));

    //print if the first ID win
    if (totalPower(firstID) > totalPower(secondID)) {
        printf("%s wins!\n", pokedex[firstID].name);
    }

    //print if the second ID win
    else if (totalPower(secondID) > totalPower(firstID)) {
        printf("%s wins!\n", pokedex[secondID].name);
    }

    //print if its tie
    else {
        printf("It's a tie!\n");
    }
}

//-----------------------------------------------------------------
// 34) Function: evolve a Pokemon
//-----------------------------------------------------------------
void evolvePokemon(OwnerNode *owner) {
    // Check if the owner or their Pokedex root is NULL
    // If either is NULL, the Pokedex is considered empty, and evolution cannot proceed
    if (!owner || !owner->pokedexRoot) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }

    // Prompt the user to enter the ID of the Pokémon to evolve
    int ID = readIntSafe("Enter ID of Pokemon to evolve ");

    // Check if the Pokémon with the given ID exists in the Pokedex
    if (!isPokemonInTree(owner->pokedexRoot, ID)) {
        printf("No Pokemon to evolve\n");
        return;
    }

    // Check if the evolved form of the Pokémon (ID + 1) already exists in the Pokedex
    if (isPokemonInTree(owner->pokedexRoot, ID + ONE_INDEX)) {
        // If the evolved form exists, release the original Pokémon
        printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).", ID + 1, pokedex[ID].name,
               pokedex[ID - ONE_INDEX].name, ID);

        // Remove the Pokémon with the given ID from the Pokedex
        owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, ID);
        printf("Pokemon with ID %d has been released.\n", ID);
    }

    // If the evolved form does not exist
    else {
        // update the current Pokémon's data to the evolved form
        printf("Pokemon evolved from %s (ID %d) to %s (ID %d)", pokedex[ID - 1].name, ID, pokedex[ID].name, ID + 1);

        // Search for the current Pokémon node in the Pokedex
        PokemonNode *searchedPokemon = searchPokemonBFS(owner->pokedexRoot, ID);

        // Update the data of the current node to the evolved form
        *searchedPokemon->data = pokedex[ID];
    }
}

//-----------------------------------------------------------------
// 35) Function to add new pokemon
//-----------------------------------------------------------------
void addPokemon(OwnerNode *owner) {
    // Check if the owner is NULL
    // If the owner is invalid, print an error message and exit
    if (!owner) {
        printf("Invalid owner.\n");
        return;
    }

    // Prompt the user to enter the ID of the Pokemon to add
    int pokemonID  = readIntSafe("Enter ID to add: ");

    // Validate the entered ID
    // If the ID is out of the valid range (1-151), print an error and exit
    if (pokemonID  < FirstID || pokemonID  > LastID) {
        printf("Invalid ID.\n");
        return;
    }

    // Check if the Pokemon with the given ID already exists in the owner's Pokedex
    if (isPokemonInTree(owner->pokedexRoot, pokemonID )) {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", pokemonID );
        return;
    }

    // Create a new PokemonNode for the specified Pokemon ID
    PokemonNode *newPokemon = createPokemonNode(&pokedex[pokemonID  - ONE_INDEX]);

    // If node creation fails, print an error and exit
    if (!newPokemon) {
        printf("Failed to create Pokemon node.\n");
        return;
    }

    // Attempt to insert the new Pokemon node into the Pokedex tree
    PokemonNode *updatedPokedexRoot  = insertPokemonNode(owner->pokedexRoot, newPokemon);

    // If insertion fails, print an error and free the memory for the node
    if (!updatedPokedexRoot ) {
        printf("Failed to insert Pokemon node into the Pokedex.\n");
        freePokemonNode(newPokemon);
    }

    // If insertion fails, print an error and free the memory for the node
    else {
        owner->pokedexRoot = updatedPokedexRoot ;
        printf("Pokemon %s (ID %d) added.\n",
               newPokemon->data->name, newPokemon->data->id);
    }
}

//----------------------------------------------------------------
// 36) Function: remove that Pokemon from BST by ID
//----------------------------------------------------------------
void freePokemon(OwnerNode *owner) {
    // Check if the owner is NULL or if the owner's Pokedex is empty
    // If there is no owner or the Pokedex is empty, print a message and return
    if (!owner || !owner->pokedexRoot) {
        if (!owner || !owner->pokedexRoot) {
            printf("No Pokemon to release.\n");
            return;
        }
    }
    // Prompt the user to enter the ID of the Pokemon to release
    int pokemonID  = readIntSafe("Enter Pokemon ID to release: ");

    // Remove the Pokemon with the specified ID from the Pokedex
    // Update the Pokedex root with the result of the removal
    owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, pokemonID );
}

//-----------------------------------------------------------------
// 37) Function: Display Menu
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner) {
    if (!owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice) {
        case DisplayBFS:
            displayBFS(owner->pokedexRoot);
            break;
        case DisplayPreOrder:
            preOrderTraversal(owner->pokedexRoot);
            break;
        case DisplayInOrder:
            inOrderTraversal(owner->pokedexRoot);
            break;
        case DisplayPostOrder:
            postOrderTraversal(owner->pokedexRoot);
            break;
        case DisplayAlphabetical:
            displayAlphabetical(owner->pokedexRoot);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

//-----------------------------------------------------------------
// 38) Function: Sorting Owners (Bubble Sort on Circular List)
//-----------------------------------------------------------------
void sortOwners() {
    // Check if the circular linked list is empty or contains only one owner
    // If so, there's no need to sort
    if (!ownerHead || ownerHead->next == ownerHead) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }

    // Declare a variable to track if any swaps were made during the iteration
    int swapped;

    // Perform a bubble sort on the circular linked list
    do {
        // Reset the swapped flag at the start of each iteration
        swapped = 0;

        // Start sorting from the head of the list
        OwnerNode *currentOwner = ownerHead;

        // Iterate through the list until we reach the starting point
        do {
            // Get the next node in the list
            OwnerNode *nextPokemon = currentOwner->next;

            // Compare the names of the current owner and the next owner
            if (strcmp(currentOwner->ownerName, nextPokemon->ownerName) > 0) {
                // If the current owner's name is greater than the next owner's name, swap their data
                swapOwnerData(currentOwner, nextPokemon);

                // Indicate that a swap was made
                swapped = SWAPPED;
            }

            // Move to the next node in the list
            currentOwner = nextPokemon;

            // Continue until the loop reaches the head again
        } while (currentOwner->next != ownerHead);

        // Repeat until no swaps are made in an iteration
    } while (swapped);

    // Print a message indicating the owners have been sorted
    printf("Owners sorted by name.\n");
}

//----------------------------------------------------------------
// 39) Utility: to swap name and pokedexRoot in two OwnerNode
//----------------------------------------------------------------
void swapOwnerData(OwnerNode *a, OwnerNode *b) {
    // Check if either of the OwnerNode pointers is NULL
    // If one or both are NULL, print an error message and return
    if (!a || !b) {
        printf("Error: One or both OwnerNodes are NULL.\n");
        return;
    }

    // Swap the ownerName strings between the two nodes
    char *tempName = a->ownerName;
    a->ownerName = b->ownerName;
    b->ownerName = tempName;

    // Swap the Pokedex roots between the two nodes
    // This effectively swaps their entire Pokemon trees
    PokemonNode *tempPokedex = a->pokedexRoot;
    a->pokedexRoot = b->pokedexRoot;
    b->pokedexRoot = tempPokedex;
}

//----------------------------------------------------------------
// 40) Utility: Insert a new owner into the circular list
//----------------------------------------------------------------
void linkOwnerInCircularList(OwnerNode *newOwner) {
    // Check if the newOwner is NULL
    // If it is NULL, print an error message and return
    if (!newOwner) {
        printf("Error: Cannot link a NULL owner.\n");
        return;
    }

    // In case the list is empty (ownerHead is NULL)
    if (!ownerHead) {
        // Set newOwner as the head of the circular list
        ownerHead = newOwner;

        // Since this is the only node, it points to itself in both directions
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
    }

    // In case the list already has one or more owners
    else {
        // Find the tail of the circular list (the node before the head)
        OwnerNode *previousOwner = ownerHead->prev;

        // Link the new owner to the tail and head

        // Tail's next points to the new owner
        previousOwner->next = newOwner;

        // New owner's prev points to the tail
        newOwner->prev = previousOwner;

        // New owner's next points to the head
        newOwner->next = ownerHead;

        // Head's prev points to the new owner
        ownerHead->prev = newOwner;
    }
}

//----------------------------------------------------------------
// 41) Utility: Remove a specific OwnerNode from the circular list
//----------------------------------------------------------------
void removeOwnerFromCircularList(OwnerNode *target) {
    // If the target node is NULL, there's nothing to remove
    if (!target) return;

    // Store the previous and next nodes of the target
    OwnerNode *prevOwner = target->prev;
    OwnerNode *nextOwner = target->next;

    // Check if the target node is the head of the list
    if (target == ownerHead) {
        // If the target is the only node in the list, set the head to NULL
        if (target->next == target) {
            ownerHead = NULL;
        }
        // Otherwise, update the head to point to the next node
        else {
            ownerHead = nextOwner;
        }
    }

    // Update the previous node's next pointer to skip the target
    if (prevOwner) prevOwner->next = nextOwner;

    // Update the next node's prev pointer to skip the target
    if (nextOwner) nextOwner->prev = prevOwner;

    // Free the memory associated with the target node
    freeOwnerNode(target);

    // Set the target pointer to NULL to avoid dangling pointers
    target = NULL;
}

//-----------------------------------------------------------------
// 42) Utility: Find an owner by name in the circular list
//-----------------------------------------------------------------
OwnerNode *findOwnerByName(const char *name) {
    // Start searching from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Traverse the circular list until the head is reached again
    do {
        // Compare the current owner's name with the given name
        // If a match is found, return the current node
        if (!strcmp(currentOwner->ownerName, name)) {
            return currentOwner;
        }

        // Move to the next node in the list
        currentOwner = currentOwner->next;

        // Stop when the loop returns to the head
    } while (currentOwner != ownerHead);

    //In case the Pokemon hasn't found, print message ,return NULL
    printf("No Pokedex found.\n");
    return NULL;
}

//------------------------------------------------------------------
// 43) Utility: Check if specific name exist in the list
//------------------------------------------------------------------
int isNameExist(char *name) {
    // Check if the circular linked list is empty
    // If ownerHead is NULL, there are no owners, so return False (name does not exist)
    if (!ownerHead) {
        return False;
    }

    // Start the search from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Traverse the circular linked list
    do {
        // Compare the current owner's name with the input name
        // If a match is found, return True (name exists in the list)
        if (!strcmp(currentOwner->ownerName, name)) {
            return True;
        }

        // Move to the next owner in the list
        currentOwner = currentOwner->next;

        // Stop when the traversal loops back to the head
    } while (currentOwner != ownerHead);
    return False;
}

//------------------------------------------------------------------
// 44) Utility: display all owners in the list
//------------------------------------------------------------------
void displayExistingPokedex() {
    // Check if the circular linked list is empty
    // If ownerHead is NULL, print a message and return
    if (!ownerHead) {
        printf("No owners found.\n");
        return;
    }

    // Start from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Counter to number the owners
    int numOfOwners = ONE_INDEX;

    // Traverse the circular linked list and display each owner's name
    do {
        // Print the owner's name with an index
        printf("%d. %s\n", numOfOwners, currentOwner->ownerName);

        // Move to the next owner in the list
        currentOwner = currentOwner->next;

        // Increment the counter
        numOfOwners++;

        // Stop when the loop returns to the head
    } while (currentOwner != ownerHead);
}

//------------------------------------------------------------------
// 45) Utility: Prompt the user to choose an exist Pokedex from list
//------------------------------------------------------------------
OwnerNode *chooseExistingPokedex() {
    // Display all existing Pokedexes
    printf("\nExisting Pokedexes:\n");
    displayExistingPokedex();

    // Count the number of owners in the circular linked list
    int numOwners = 0;
    OwnerNode *current = ownerHead;
    do {
        // Increment the owner count
        numOwners++;

        // Move to the next node in the list
        current = current->next;

        // Stop when the loop returns to the head
    } while (current != ownerHead);

    // Prompt the user to choose a Pokedex by its number
    int choice;

    do {
        // Read the user's choice
        choice = readIntSafe("Choose a Pokedex by number: ");

        // If the choice is out of range, display an error message
        if (choice < ONE_INDEX || choice > numOwners) {
            printf("Invalid choice. Please choose a number between 1 and %d.\n", numOwners);
        }

        // Repeat until a valid choice is entered
    } while (choice < ONE_INDEX || choice > numOwners);

    // Traverse the list to find the chosen Pokedex
    current = ownerHead;
    for (int i = ONE_INDEX; i < choice; i++) {
        // Move to the next node until the chosen one is reached
        current = current->next;
    }

    // Return the chosen OwnerNode
    return current;
}

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------
void enterExistingPokedexMenu() {
    if (!ownerHead) {
        printf(" No existing Pokedexes.\n");
        return;
    }
    OwnerNode *currentOwner = chooseExistingPokedex();
    printf("\nEntering %s's Pokedex...\n", currentOwner->ownerName);
    int subChoice;
    do {
        printf("\n-- %s's Pokedex Menu --\n", currentOwner->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice) {
            case AddPokemon:
                addPokemon(currentOwner);
            break;
            case DisplayPokemon:
                displayMenu(currentOwner);
            break;
            case ReleasePokemon:
                freePokemon(currentOwner);
            break;
            case PokemonFight:
                pokemonFight(currentOwner);
            break;
            case EvolvePokemon:
                evolvePokemon(currentOwner);
            break;
            case BackToMainMenu:
                printf("Back to Main Menu.\n");
            break;
            default:
                printf("Invalid choice.\n");
        }
    } while (subChoice != BackToMainMenu);
}

//------------------------------------------------------------------
// Existing Pokedex Menu
//------------------------------------------------------------------
void openPokedexMenu() {
    printf("Your name: ");
    char *ownerName = getDynamicInput();
    if (!ownerName) {
        printf("Failed to read owner name.\n");
        return;
    }
    if (isNameExist(ownerName)) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", ownerName);
        free(ownerName);
        return;
    }

    printf("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    int choice = readIntSafe("Your choice: ");
    PokemonNode *starterPokemon = NULL;

    switch (choice) {
        case StarterBulbasaur:
            starterPokemon = createPokemonNode(&pokedex[0]);
            break;
        case StarterCharmander:
            starterPokemon = createPokemonNode(&pokedex[3]);
            break;
        case StarterSquirtle:
            starterPokemon = createPokemonNode(&pokedex[6]);
            break;
        default:
            printf("Invalid choice.\n");

            // Ensure ownerName is freed
            free(ownerName);
            return;
    }

    if (starterPokemon) {
        addOwnerToList(ownerName, starterPokemon);
    }

    // Free memory after use
    free(ownerName);
}

//---------------------------------------------------------------
// 46) Utility: get the number of Pokedex in the list
//---------------------------------------------------------------
int getNumberOfPokedexes() {
    // Start counting from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Initialize a counter to track the number of owners
    int numOfOwners = 0;

    // Traverse the circular linked list
    do {
        // Move to the next owner in the list
        currentOwner = currentOwner->next;

        // Increment the counter
        numOfOwners++;

        // Stop when the loop returns to the head
    } while (currentOwner != ownerHead);

    // Return the total count of owners in the list
    return numOfOwners;
}

//------------------------------------------------------------------
// 47) Function: Delete an entire Pokedex (owner) from the list.
// -----------------------------------------------------------------
void deletePokedex() {
    // Check if there are any existing Pokedexes
    // If the list is empty, print a message and return
    if (!ownerHead) {
        printf("No existing Pokedexes to delete.\n");
        return;
    }

    // Display the list of existing Pokedexes
    printf("\n=== Delete a Pokedex ===\n");
    displayExistingPokedex();

    // Prompt the user to choose a Pokedex to delete by its number
    int choice = readIntSafe("Choose a Pokedex to delete by number: ");

    // Start from the head of the circular list
    OwnerNode *currentOwner = ownerHead;

    // Counter to match the user's choice
    int OwnerIndex = ONE_INDEX;

    // Traverse the circular list
    do {
        // If the current index matches the user's choice, delete the Pokedex
        if (OwnerIndex == choice) {
            printf("Deleting Brook's entire Pokedex...\n");

            // Remove the selected owner from the circular list
            removeOwnerFromCircularList(currentOwner);

            // Confirm the deletion,return to menu
            printf("Pokedex deleted.\n");
            return;
        }

        // Move to the next owner in the list
        currentOwner = currentOwner->next;
        OwnerIndex++;

        // Stop when the loop returns to the head
    } while (currentOwner != ownerHead);

    // If no match was found, print an error message
    printf("No Pokedex to delete.\n");
}

//------------------------------------------------------------------
// 48) Utility: merge two pokedexes
//------------------------------------------------------------------
void merge(PokemonNode **first, PokemonNode *second) {
    // Base case: If the second tree (subtree) is NULL, there is nothing to merge
    if (!second) {
        return;
    }

    // Check if the Pokemon from the second tree is already in the first tree
    // If it is not, create a new PokemonNode and insert it into the first tree
    if (!isPokemonInTree(*first, second->data->id)) {
        // Create a new node using the data from the current node of the second tree
        PokemonNode *newPok = createPokemonNode(second->data);

        // Insert the new node into the first tree
        *first = insertPokemonNode(*first, newPok);
    }

    // Recursively merge the left subtree of the second tree into the first tree
    merge(first, second->left);

    // Recursively merge the right subtree of the second tree into the first tree
    merge(first, second->right);
}

//-----------------------------------------------------------------
// 49) Function: Merge the second owner's Pokedex into the first
//-----------------------------------------------------------------
void mergePokedexMenu() {
    // Check if the circular list is empty
    // If there are no owners, print a message and exit
    if (!ownerHead) {
        printf("Not enough owners to merge.\n");
        return;
    }

    // Check if there are fewer than two owners in the list
    // At least two owners are required to perform a merge
    if (getNumberOfPokedexes() < MINIMUM_TO_FIGHT) {
        printf("Not enough owners to merge.\n");
        return;
    }

    // Prompt the user to enter the names of the two owners to merge
    printf("\n=== Merge Pokedexes ===\n");
    printf("Enter name of first owner: ");

    // Dynamically read the name of the first owner
    char *firstOwnerName = getDynamicInput();

    printf("Enter name of second owner: ");

    // Dynamically read the name of the second owner
    char *secondOwnerName = getDynamicInput();

    // Find the nodes corresponding to the two owners in the circular list
    OwnerNode *first = findOwnerByName(firstOwnerName);
    OwnerNode *second = findOwnerByName(secondOwnerName);

    // Check if either owner could not be found in the list
    if (!first || !second) {
        // If one or both owners are not found, print an error message and abort the merge
        printf("One or both owners not found. Merge aborted.\n");

        // Free dynamically allocated memory for owner names
        free(firstOwnerName);
        free(secondOwnerName);
        return;
    }

    // If both owners are found, proceed with the merge
    printf("Merging %s and %s...\n", firstOwnerName, secondOwnerName);

    // Merge the second owner's Pokedex into the first owner's Pokedex
    merge(&first->pokedexRoot, second->pokedexRoot);

    // Confirm that the merge is complete
    printf("Merge completed.\n");

    // Remove the second owner from the circular list after the merge
    removeOwnerFromCircularList(second);

    // Print a confirmation message
    printf("Owner '%s' has been removed after merging.\n", secondOwnerName);

    // Free the dynamically allocated memory for owner names
    free(firstOwnerName);
    free(secondOwnerName);
}

//-----------------------------------------------------------------
// 50) Utility: print owners from head,  froward traversal
//-----------------------------------------------------------------
void printForward(int numOfPrints) {
    // Start from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Initialize a counter to track the number of prints
    int printCounter = ONE_INDEX;

    // Traverse the list and print the owner names
    do {
        // Print the current owner's name with an index
        printf("[%d] %s\n", printCounter, currentOwner->ownerName);

        // Move to the next node in the list
        currentOwner = currentOwner->next;

        // Increment the counter
        printCounter++;

        // Continue until the specified number of prints is reached
    } while (printCounter <= numOfPrints);
}

//-----------------------------------------------------------------
// 51) Utility : Print owners from head , Backward traversal
//-----------------------------------------------------------------
void printBackward(int numOfPrints) {
    // Start from the head of the circular linked list
    OwnerNode *currentOwner = ownerHead;

    // Initialize a counter to track the number of prints
    int printsCounter = ONE_INDEX;

    // Traverse the list and print the owner names
    do {
        // Print the current owner's name with an index
        printf("[%d] %s\n", printsCounter, currentOwner->ownerName);

        // Move to the previous node in the list
        currentOwner = currentOwner->prev;

        // Increment the counter
        printsCounter++;

        // Continue until the specified number of prints is reached
    } while (printsCounter <= numOfPrints);
}

//--------------------------------------------------------------------------------------------
// 52) Function:  Print owners left or right from head, repeating as many times as user wants
//--------------------------------------------------------------------------------------------
void printOwnersCircular() {
    // Check if the list is empty
    // If there are no owners, print a message and return
    if (!ownerHead) {
        printf("No owners.\n");
        return;
    }

    // Prompt the user to enter a direction (F for forward, B for backward)
    printf("Enter direction (F or B): ");

    // Read the direction input from the user
    char direction = getchar();

    // Clear the input buffer to handle leftover characters
    clearInputBuffer();

    // Validate the direction input
    // If the input is not 'F', 'f', 'B', or 'b', print an error message and return
    if (direction != 'F' && direction != 'f' && direction != 'B' && direction != 'b') {
        printf("Invalid direction.\n");
        return;
    }

    // Prompt the user to enter the number of times to print
    int numOfPrints = readIntSafe("How many prints? ");

    // If the direction is forward (F or f), call the printForward function
    if (direction == 'F' || direction == 'f') {
        printForward(numOfPrints);
    }

    // If the direction is backward (B or b), call the printBackward function
    else if (direction == 'B' || direction == 'b') {
        printBackward(numOfPrints);
    }
}

//--------------------------------------------------------------------------------------
// 53) Function: Frees every remaining owner in the circular list
//--------------------------------------------------------------------------------------
void freeAllOwners() {
    // Check if the circular list is empty
    // If ownerHead is NULL, there are no owners to free, so return
    if (!ownerHead) {
        return;
    }

    // Start from the head of the circular list
    OwnerNode *currentOwner = ownerHead;

    // Traverse the circular list
    do {
        // Save the pointer to the next node before freeing the current node
        OwnerNode *next = currentOwner->next;

        // Free the memory associated with the current owner
        freeOwnerNode(currentOwner);

        // Move to the next node
        currentOwner = next;

        // Stop when the loop returns to the head
    } while (currentOwner != ownerHead);

    // After freeing all nodes, set the head to NULL to indicate the list is empty
    ownerHead = NULL;
}

//---------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu() {
    int choice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice) {
            case NewPokedex:
                openPokedexMenu();
                break;
            case ExistingPokedex:
                enterExistingPokedexMenu();
                break;
            case DeletePokedex:
                deletePokedex();
                break;
            case MergePokedex:
                mergePokedexMenu();
                break;
            case SortOwners:
                sortOwners();
                break;
            case PrintOwners:
                printOwnersCircular();
                break;
            case Exit:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid.\n");
        }
    } while (choice != Exit);
}

int main() {
    mainMenu();
    freeAllOwners();
    return Success;
}
