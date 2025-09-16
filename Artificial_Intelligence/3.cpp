#include <iostream>
#include <string>
#include <vector>
#include <map>

// Struct to represent a person and their relationships
struct Person {
    std::string name;
    std::string gender;
    std::string father;
    std::string mother;
    std::string spouse;
    std::vector<std::string> children;
};


std::map<std::string, Person> family_tree;

// Function to add a parent-child relationship
void add_parent(const std::string& parent_name, const std::string& child_name) {
    if (family_tree[parent_name].gender == "male") {
        family_tree[child_name].father = parent_name;
    } else if (family_tree[parent_name].gender == "female") {
        family_tree[child_name].mother = parent_name;
    }
    family_tree[parent_name].children.push_back(child_name);
}

// Function to find the father of a person
std::string get_father(const std::string& name) {
    return family_tree.count(name) ? family_tree[name].father : "Not found";
}

int main() {
    // 1. Define all individuals and their genders
    family_tree["adam"].name = "adam"; family_tree["adam"].gender = "male";
    family_tree["eve"].name = "eve"; family_tree["eve"].gender = "female";
    family_tree["ben"].name = "ben"; family_tree["ben"].gender = "male";
    family_tree["carl"].name = "carl"; family_tree["carl"].gender = "male";
    family_tree["linda"].name = "linda"; family_tree["linda"].gender = "female";

    // 2. Establish relationships based on the provided "knowledge base"
    // parent(adam, ben).
    // parent(eve, ben).
    add_parent("adam", "ben");
    add_parent("eve", "ben");
    
    // parent(adam, carl).
    // parent(linda, carl).
    add_parent("adam", "carl");
    add_parent("linda", "carl");
    
    // spouse(adam, eve).
    family_tree["adam"].spouse = "eve";
    family_tree["eve"].spouse = "adam";
    
    // spouse(adam, linda).
    family_tree["adam"].spouse = "linda";
    family_tree["linda"].spouse = "adam";

    // 3. Perform queries
    std::cout << "Adam's spouse is: " << family_tree["adam"].spouse << std::endl;
    std::cout << "Ben's father is: " << get_father("ben") << std::endl;
    std::cout << "Ben's mother is: " << family_tree["ben"].mother << std::endl;
    
    std::cout << "Carl's father is: " << get_father("carl") << std::endl;
    std::cout << "Carl's mother is: " << family_tree["carl"].mother << std::endl;
    
    std::cout << "Adam's children are: ";
    for (const auto& child : family_tree["adam"].children) {
        std::cout << child << " ";
    }
    std::cout << std::endl;
    
    return 0;
}