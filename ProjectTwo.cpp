//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Cade Bray
// Version     : 1.0
// Copyright   : N/A
// Description : Final project part two. I've implemented a hash table to store
//               ABCU's data. The course code cannot be hashed as a string, so
//               I am sorting through the string and summing the decimal values
//               of the strings characters.
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <regex>
#include <climits>

const unsigned int DEFAULT_SIZE = 179;
using namespace std;

/**
 * @brief This function will display the menu and return the user's choice as a string.
 *
 * This function will display the menu and return the user's choice as a string. The menu will display the options by
 * number and will prompt the user to enter their choice. The function will return the user's choice as a string but
 * will not validate the input which will not matter because this should be used within a switch statement.
 *
 * I recommend setting a default case in the switch statement to handle invalid input.
 *
 * @return The user's choice as a string. (Should be a number in the string)
 */
string menu() {
    /* Display the menu */
    cout << "1. Load file" << endl;
    cout << "2. Print all courses" << endl;
    cout << "3. Search for a course" << endl;
    cout << "9. Quit" << endl;

    //Receive input
    string choice;
    cout << "Enter your choice: " << endl;
    cin >> choice;
    cout << endl << endl;
    return choice;
}

/**
 * @brief This class will hold the course information.
 *
 * This class will hold the course information. The course information will include the course code, course description,
 * and the pre-requisites for the course. The course code is a string that is 4 letters followed by 3 numbers. The course
 * description is a string that is at least two words. The pre-requisites are a vector of strings that are course codes.
 * If this object is incomplete the identifier will be set to 0. Do not use the identifier if you have not set the course
 * code to what you want. The identifier will be reassigned if the course code is changed.
 */
class Course{
    public:

        /**
        * @brief Initialize the course object with values.
        * This object can be used to initialize the course object with values. This is useful when creating a course object
        * for testing. All variables are initialized to the values passed in and the identifier is set to the sum of the
        * decimal values of the course code.
        */
        Course(string t_course_code, string t_course_description, vector<string> t_pre_reqs){
            //Initialize the course object with values. Using std::move to avoid copying the values
            this->course_code = std::move(t_course_code);
            this->course_description = std::move(t_course_description);
            this->pre_reqs = std::move(t_pre_reqs);
            setIdentifier();
        }

        /**
        * @brief Initialize the course object with no values.
        *
        * This object can be used to initialize the course object with no values. This is useful when creating a vector
         * of course objects. All variables are initialized to empty values and the identifier is set to 0.
        */
        Course(){
            //Initialize the course object with no values
            this->course_code = "";
            this->course_description = "";
            this->pre_reqs = {};
            this->identifier = 0;
        }

        /**
        * @brief This is the deconstructor for the course object.
        *
        * Most variables are destroyed when they go out of scope. This deconstructor is used to clean up the pre_reqs
        * vector. The vector is cleared and then shrink_to_fit() is used to free up memory.
        */
        ~Course(){
            //No need to clean up a string. Strings class will handle it in the deconstructor upon leaving scope.

            //Clear the pre_reqs vector for memory management. Shrink_to_fit() is used to free up memory.
            //Vector deconstructor will handle terminating the vector once it's out of scope.
            pre_reqs.clear();
            pre_reqs.shrink_to_fit();
        }

        /**
        * @brief Setter for the course code.
        *
        * This function will set the course code. The course code is a unique identifier for the course object.
        * Since this object is a string it cannot be hashed so a unique identifier is created by summing the decimal
        * values of the course code. I am also Using std::move to avoid copying the values.
        *
        * @param temp_course_code The course code as a string.
        */
        void setCourseCode(string temp_course_code){
            // Set the course code. Using std::move to avoid copying the values.
            this->course_code = std::move(temp_course_code);
            // We've now got the value we use to hash, so we need to convert to an int and hash it.
            setIdentifier();
        }

        /**
        * @brief Getter for the course code.
        *
        * This function will return the course code as a string. Assigned using std::move to avoid copying the values.
        *
        * @return string course code.
        */
        [[nodiscard]] string getCourseCode(){
            // Return the course code.
            return this->course_code;
        }

        /**
        * @brief Setter for the course description.
        *
        * This function will set the course description. The course description is a string that is at least two words.
        * Validation is handled in the data import function. Using std::move to avoid copying the values.
        *
        * @param temp_course_description The course description as a string.
        */
        void setCourseDescription(string temp_course_description){
            // Set the course description. Using std::move to avoid copying the values.
            this->course_description = std::move(temp_course_description);
        }

        /**
        * @brief Getter for the course description.
        *
        * This function will return the course description as a string. The function is marked as [[nodiscard]] to ensure
        * that the return value is used.
        *
        * @return course description as a string.
        */
        [[nodiscard]] string getCourseDescription() const{
            // Return the course description. No discard is used because the string is not being modified.
            return this->course_description;
        }

        /**
        * @brief Setter for the pre-reqs vector.
        *
        * This function will set the pre-reqs vector. The pre-reqs vector is a vector of strings that are course codes.
        * using std::move to avoid copying the values.
        *
        * @param temp_pre_reqs The pre-reqs vector as a vector of strings.
        */
        void setPreReqs(vector<string> temp_pre_reqs){
            // Set the pre-reqs vector. Using std::move to avoid copying the values.
            this->pre_reqs = std::move(temp_pre_reqs);
        }

        /**
        * @brief Getter for the pre-reqs vector.
        *
        * Getter for the pre-reqs vector. This function will return the pre-reqs vector as a vector of strings.
        *
        * @return will return the pre-reqs vector as a vector of strings.
        */
        [[nodiscard]] vector<string> getPreReqs() const{
            // Return the pre-reqs vector. No discard is used because the vector is not being modified.
            return this->pre_reqs;
        }

        /**
        * @brief Will return the identifier for the course object.
        *
        * No setter is used because the identifier is set in the constructor and should not be modified. The identifier
        * is a sum of the decimal values of the course code and description.
        * This will allow it to be hashed. This function will return the identifier as an integer.
        *
        * @return integer sum of the decimal values of the course code.
        */
        [[nodiscard]] int getIdentifier() const{
            // Return the identifier. No discard is used because the integer is not being modified.
            return this->identifier;
        }

    private:
        // Declare the private variables.
        string course_code;
        int identifier{};
        string course_description;
        vector<string> pre_reqs;
        bool CC_included = false;

        /**
        * @brief This function will set the identifier for the course object.
        *
        * The identifier is a sum of the decimal values of the course code and course description.
        * This will allow it to be hashed without conflict. The identifier will become more unique as the course code or
        * description is assigned. I recommend using both the course code and description to increase the uniqueness of
        * the identifier. Using just the course code causes conflicts with similar course codes. If the identifier is 0
        * the course object has not been fully initialized with a course code or description.
        *
        * @return Will return the identifier as an integer.
        */
        void setIdentifier(){

            unsigned char temp_char;
            //Convert the course code to a string of decimal numbers.
            // The time complexity for this you would this is O(n) but since all the course codes are the same length
            // the time complexity would be O(1).
            // We can still generate a unique identifier for the course object by summing the decimal values of the
            // course code. CC_included it a property used for testing that allows a developer to see if the value has
            // been hashed yet.
            if (!this->course_code.empty() and !this->CC_included){
                for (int i = 0; i < this->course_code.length(); ++i) {
                    //Convert the character to an integer. We need to use an unsigned char to avoid overflow.
                    temp_char = this->course_code[i];
                    //Convert the character to an integer.
                    this->identifier += int(temp_char);
                    this->CC_included = true;
                }
            }
            // I wanted to include the course description in the identifier to make it more unique for fewer conflicts but
            // If the user needs to search for a course they would need to include the course description as well.
            // This goes beyond the scope of the project but is a good practice to create more unique identifiers.
        }
    };

/**
 * @brief This class is the hash table for the course objects.
 *
 * This class is the hash table for the course objects. The hash table will hold the course objects in a vector of nodes.
 * The nodes will hold the course object and the key. The key is the identifier for the course object. The key is the
 * integer modulo the table size. The table size is the default size of 179. The hash table will be initialized with the
 * default size of 179. The hash table will have functions to insert, remove, and search for course objects. The hash
 * table will also have functions to print all the course objects in the hash table per the requirements of the project.
 */
 class HashTable{
    private:
        //Declare the private variables and the node struct.

         /**
         * @brief This struct will hold the course object and the key.
         */
         struct Node {
            //Declare the private variables for the node.
             Course course;
             unsigned int key;

             /**
            * @brief This initializer will assign the key to the maximum unsigned integer and the next node to nullptr.
            */
             Node() {
                 key = UINT_MAX;
             }

             /**
            * @brief This initializer will assign the course object to the course variable.
            */
             explicit Node(const Course& aCourse) : Node() {
                 course = aCourse;
             }

             /**
            * @brief This initializer will assign the course object to the course variable and the key to the key variable.
            */
             Node(const Course& aCourse, unsigned int aKey) : Node(aCourse) {
                 key = aKey;
             }
         };

         // These are not a part of the node struct. They are a part of the hash table class private definition.
         vector<Node> nodes;

         unsigned int tableSize = DEFAULT_SIZE;

    public:
         /**
        * @brief This initializer will initialize the hash table with the default size of 179.
        *
        * This initializer will initialize the hash table with the default size of 179. The table size is defined in the
        * private variables.
        */
        HashTable(){
            //Initialize the hash table with a default size.
            nodes.resize(tableSize);
        }

        /**
       * @brief This initializer will initialize the hash table with a custom size.
       *
       * this function will initialize the hash table with a custom size. The function will take in a integer and
       * resize the hash table to the custom size. [[maybe_unused]] is used to suppress the warning for the unused
       * variable.
       *
       * @param size The size of the hash table.
       */
       [[maybe_unused]] explicit HashTable(unsigned int size){
            //Initialize the hash table with a custom size.
            tableSize = size;
            nodes.resize(tableSize);
       }

         /**
          * @brief This deconstruct will delete the hash table.
          *
          * The deconstructor will delete the hash table to free up memory. Since all the nodes are stored in a vector
          * the vector will be cleared and then shrink_to_fit() will be used to free up memory.
          */
        ~HashTable(){
            //Clear the nodes vector to free up memory.
            nodes.clear();
            //Shrink the nodes vector to free up memory.
            nodes.shrink_to_fit();
        }

         /**
         * @brief This function will hash a integer and return it as a the key.
         *
         * The function will take in a integer and hash it to get the key. The key will be returned as an unsigned integer.
         * The key is the integer modulo the table size. The table size is the default size of 179. The key is the
         * remainder of the integer divided by the table size. No discard is used because the integer is not being modified.
         * This function was built with the intention of hashing the course identifier.
         *
         * @param key The integer to be hashed.
         * @return The key as an unsigned integer.
         */
         [[nodiscard]] unsigned int hash(int key) const{
             //Hash the course code to get the index.
             return key % tableSize;
         }

         /**
        * @brief This function will print all the courses in the hash table in no particular order.
        *
        * This function will print all the courses in the hash table in no particular order. The function will iterate
        * through the hash table and print the key, course code, and course description. The function will print the
        * key as an integer, the course code as a string, and the course description as a string. This does not conform
        * to the requirements of the project but is useful for testing. If you want to print in alphanumeric order you
        * can use function printAlphanumeric() which does conform to the requirements of the project.
        */
         void printAll(){
             //Print all the courses in the hash table.
             for (auto &node : nodes) {
                 if (node.key != UINT_MAX) {
                     cout << "Key: " << node.key << " Course Code: " << node.course.getCourseCode() << " Course Description: "
                          << node.course.getCourseDescription() << endl;
                 }
             }
         }

         /**
        * @brief This function will shrink the nodes vector to the max values in the table to conserve memory.
        */
         void shrink(){
             nodes.shrink_to_fit();
         }

         /**
        * @brief This function will print all the courses in the hash table in alphanumeric order.
        *
        * This function will print all the courses in the hash table in alphanumeric order. The function will create a
        * vector of strings to store the course codes. The function will iterate through the hash table and add the
        * course codes to the vector. The function will sort the vector of course codes. The function will iterate
        * through the sorted vector of course codes and print the course code, course description, and pre-reqs.
        * This function conforms to the requirements of the project.
        */
         void printAlphanumeric(){
             //Print all the courses in the hash table in alphanumeric order.
             //Create a vector of strings to store the course codes.
             vector<string> course_codes;
             //Iterate through the hash table and add the course codes to the vector.
             for (auto &node : nodes) {
                 //Check to ensure the key is not empty.
                 if (node.key != UINT_MAX) {
                     // Add the course code to the vector.
                     course_codes.push_back(node.course.getCourseCode());
                 }
             }
             // Sort the vector of course codes.
             sort(course_codes.begin(), course_codes.end());
             // Iterate through the sorted vector of course codes and print the course code and course description.
             for (const auto &course_code : course_codes) {
                 for (auto &node : nodes) {
                     if (node.key != UINT_MAX and node.course.getCourseCode() == course_code) {
                         // Print the course code, course description, and pre-reqs.
                         cout << "Course Code: " << node.course.getCourseCode() << endl << "Course Description: "
                              << node.course.getCourseDescription() << endl << "Pre-Reqs: " << endl;

                         if(node.course.getPreReqs().empty()){
                             // Print no pre-reqs if the pre-reqs vector is empty.
                             cout << "   -No prerequisites" << endl;
                         }

                         // Print the pre-reqs for the course object.
                        for (const auto &pre_req : node.course.getPreReqs()) {
                            // Print the pre-reqs for the course object.
                            cout << "   -" << pre_req << endl;
                        }
                        cout << endl;
                     }
                 }
             }
         }

         /**
        * @brief This function will insert a course object into the hash table.
        *
        * Insertion of a course object into the hash table. The function will take in a course object and hash the course
        * identifier to get the key. The course object will be inserted into the hash table at the key. If the key is
        * already taken the function will find the next available key and insert the course object there. If the hash
        * table is full the function will throw an out_of_range exception.
        *
        * @param temp_course The course object to be inserted into the hash table.
        */
        void insert(const Course& temp_course){
            //Insert the course object into the hash table.
            //Hash the course identifier to be the key.
            unsigned int key = hash(temp_course.getIdentifier());
            unsigned int og_key = key;

            // retrieve node using key
            auto hashed_node = nodes.at(key);

            bool assigned = false;
            while (!assigned){
                if (hashed_node.key == UINT_MAX){
                    // if the key is empty, assign the course to the node
                    nodes.at(key) = Node(temp_course, key);
                    assigned = true;
                } else if (key == nodes.size() - 1) {
                    // Restart the search at zero. We're at the max table index.
                    key = 0;
                    hashed_node = nodes.at(key);
                } else if (key == og_key - 1) {
                    // No space left in this table. Worst case achieved O(n).
                    throw out_of_range("No empty buckets available");
                } else {
                    // Check next bucket
                    key++;
                    //assigned hashed_node to the node for the incremented key.
                    hashed_node = nodes.at(key);
                }
            }
        }

         /**
         * @brief This function will remove a course object from the hash table.
         *
         * Function will remove a course object from the hash table. The function will take in an identifier and search
         * for the course object in the hash table. If the course object is found the course object will be removed from
         * the hash table and return it. If the course object is not found the function will return a blank course object.
         *
         * @param identifier int identifier for the course object. Found as a getter in the course object.
         * @return Will return the item removed if found and will return a blank course if not found.
         */
        Course remove(Course temp_course){
            //Remove the course object from the hash table.
            auto key = hash(temp_course.getIdentifier());
            auto og_key = key;

            while(true){
                if (nodes[key].course.getIdentifier() == temp_course.getIdentifier()){
                    // Check to ensure course codes match as well because it may be the same hashed value but different courses.
                    if (nodes.at(key).course.getCourseCode() == temp_course.getCourseCode()) {
                        //erase node begin and key
                        nodes.erase(nodes.begin() + key);
                        // Resize the nodes vector to free up memory. Important to shrink because if an item is removed from the
                        // vector it can still hold the memory for the removed item and search functions will still evaluate to true
                        // for the removed item at that index. Shrink_to_fit() will free up the memory and prevent this from happening.
                        nodes.shrink_to_fit();
                        return temp_course;
                    }
                    // It was a course with the same hashed value. Keep searching. We're on the verge of worst case O(n).
                    key++;
                } else if (key >= nodes.size() - 1) {
                    // We are at the max index for the table. Restart at zero and keep looking.
                    key = 0;
                } else if (key == og_key - 1) {
                    // The course was not found. Return a blank course to indicate it was not found.
                    return Course();
                } else {
                    // Check next bucket
                    key++;
                }
            }
        }

        /**
        * @brief This function will search for a course object in the hash table.
        *
        * This function receives a course object that it will use to find the course information. If it cannot find
        * the course it will return a blank course object.
        *
        * @param identifier int identifier for the course object. Found as a getter in the course object.
        * @return The course object if found and an empty course object if not found.
        */
        Course search(Course course) {
            // Search for the course object in the hash table.
            // create the key for the given identifier
            auto key = hash(course.getIdentifier());
            auto og_key = key;
            // if entry found for the key. We are straight indexing into the vector instead of using the at() function
            // because we are checking if the key is empty. If the key is empty we will not be able to use the at()
            // because it will throw an out of range exception.
            while (true) {
                if (nodes[key].course.getIdentifier() == course.getIdentifier()) {
                    // Check to ensure that this is the course we needed and not a different course that equivocated to
                    // the same hash value.
                    if (nodes.at(key).course.getCourseCode() == course.getCourseCode()){
                        return nodes.at(key).course;
                    }
                    // It was a course with the same hash value. Most likely assigned before this course was. Increment
                    // and keep looking! We are on the verge of worst case O(n).
                    key++;
                } else if (key >= nodes.size() - 1) {
                    // We are now above or at the index of the end of the vector, so we need to start at 0 to keep searching.
                    key = 0;
                } else if (key == og_key - 1) {
                    // no entry found for the key
                    // return a course object.
                    return Course();
                } else {
                    // Check next bucket
                    key++;
                }
            }
        }
 };

/**
 * @brief This function will do the data import for a file. Returns a vector of course objects.
 *
 * This function will take in a file path and read the file. The file must be a .txt or .csv file. The function will
 * read the file and parse the data into course objects. The course objects will be stored in a vector and returned.
 * FIXME: Need documentation for individual components.
 *
 * @param file_path Constant string of the file path.
 * @return The vector of course type objects.
 */
HashTable receiveFile(const string& file_path){

    vector<Course> courses;
    //Check if the file is a .txt or .csv file
    if (file_path.find(".txt") == string::npos and file_path.find(".csv") == string::npos) {
        cout << "Invalid file type. Please enter a .txt or .csv file." << endl;
        return HashTable();
    }

    // Open the file
    ifstream input_file(file_path);

    //Check if the file is open
    if (!input_file.is_open()) {
        cout << "Unable to open file. Please enter a valid file path." << endl;
        return HashTable();
    }

    // Startup variables
    char c;
    string temp_string;
    int comma_count = 0;
    int line_count = 0;
    Course temp_course;
    vector<string> pre_reqs;

    //Regex for course code and course description
    //Course code is 4 letters followed by 3 numbers
    regex course_code_reg("[A-Z]{4}\\d{3}");
    //Course description is any string with at least on space indicating at least two words.
    regex course_desc_reg("\\s+");

    // This while condition will check to ensure that the character is not the end of the file and will continue to
    // reassign the character to c until the end of the file is reached.
    while ((c = input_file.get()) != EOF) {

        //Found course code
        if(c == ',' and comma_count == 0){

            if (!regex_search(temp_string, course_code_reg)){
                //Check to ensure the course code is in the correct format
                throw invalid_argument("Invalid course code format on line " + to_string(line_count) + ".");
            }
            temp_course.setCourseCode(temp_string);
            temp_string = "";
            comma_count++;
        }

        //Found course description
        else if(c == ',' and comma_count == 1){

            if (!regex_search(temp_string, course_desc_reg)){
                //Check to ensure the course description has content greater than one word (Checks for a space).
                throw invalid_argument("Invalid course code format on line " + to_string(line_count) + ".");
            }
            temp_course.setCourseDescription(temp_string);

            // Reset the temp string and increment the comma count
            temp_string = "";
            comma_count++;
        }

        //Found a pre-req
        else if(c == ',' and comma_count > 1 and !temp_string.empty()) {

            if (!regex_search(temp_string, course_code_reg) and !temp_string.empty()){
                //Check to ensure the pre-req is in the correct format using regex.
                throw invalid_argument("Invalid prerequisite course code format on line "
                + to_string(line_count) + " pre-req #" + to_string(comma_count - 1) + ".");
            }

            pre_reqs.push_back(temp_string);

            // Reset the temp string and increment the comma count
            temp_string = "";
            comma_count++;
        }

        //Found the end of the line
        else if(c == '\n'){

            if(comma_count > 1 and !temp_string.empty()){
                // Item still needing to be appended to prereqs.
                pre_reqs.push_back(temp_string);
            }

            //Set the pre-reqs for the course object
            temp_course.setPreReqs(pre_reqs);
            // Add the course object to the vector
            courses.push_back(temp_course);
            // Reset the temp course object, temp string, and comma count
            temp_course = Course();
            temp_string = "";
            comma_count = 0;
            //Clear the pre-reqs vector for memory management. Shrink_to_fit() is used to free up memory.
            pre_reqs.clear();
            pre_reqs.shrink_to_fit();
            //Increment the line count (Used only for error messages).
            line_count++;
        }

        //Add the character to the temp string
        else{

            if(c != '\r' and c != ',') {
                //Ignore carriage returns and commas
                temp_string += c;
            }
        }
    }
    //Close the file, so it can be used elsewhere if needed.
    input_file.close();

    //Create the hash table with the size of the courses vector.
    HashTable hash_table(courses.size());
    //Insert the course objects into the hash table
    for (const auto &course : courses) {
        hash_table.insert(course);
    }
    cout << "File loaded successfully." << endl << endl;
    return hash_table;
}



int main() {

    // Opening variables
    HashTable hash_table;
    string file_path;
    string chosen_course_code;
    Course temp_course = Course();

    do {
        // Start with getting the selection from the user.
        string choice = menu();
        // Defined here because it cannot be initialized in the switch case
        string remove_choice;
        switch (stoi(choice)) {
            case 1:
                // Load file per requirements.
                cout << "Please enter a file path:" << endl;
                // Using getline() because if the file path or file name has a space it will stop at that point.
                // We want the buffer to give us the entire user provided content.
                cin.ignore();
                getline(cin, file_path);
                //Build hash table with the receivefile importation method.
                hash_table = receiveFile(file_path);
                // Shrinking to size because we defined the hash_table variable in the main function scope this means
                // that the function was initialized with the default capacity of 179. No matter what I tried it will
                // not shrink unless in the same scope as the initial assignment.
                hash_table.shrink();
                break;
            case 2:
                // Print out content alphanumerically per requirements.
                cout << "COURSES:" << endl;
                hash_table.printAlphanumeric();
                break;
            case 3:
                // This function allows for searching of a course per requirements.
                cout << "Please enter the course code: (EX. EXAM123)" << endl;
                cin >> chosen_course_code;

                // Assign to a temp course to build the identifier in the properties
                temp_course = Course(chosen_course_code, "", {});
                // Pass the temp course to the search function, so it can use the identifier and course code properties.
                temp_course = hash_table.search(temp_course);

                //temp_course was returned from the search as a blank course which indicates it was not found.
                if(temp_course.getIdentifier() == 0){
                    cout << "Course not found." << endl << endl;
                    break;
                }
                cout << endl;
                // Course was found. Output information per the requirements.
                cout << "Course Code: " << temp_course.getCourseCode() << endl << "Course Description: "
                << temp_course.getCourseDescription() << endl << "Pre-Reqs: " << endl;
                // Iterate through the pre-reqs
                if (temp_course.getPreReqs().empty()){
                    cout << "   -No prerequisites" << endl;
                }
                for (const auto &pre_req : temp_course.getPreReqs()) {
                    cout << "   -" << pre_req << endl;
                }

                // To make a robust program I added the ability to remove a course. This is not required by the client
                // but is added functionality.
                cout << endl << "Would you like to remove this course? (Y/N)" << endl;
                cin >> remove_choice;
                if (remove_choice == "Y" or remove_choice == "y") {
                    // remove the course. Passes a course object to give course code and identifier
                    temp_course = hash_table.remove(temp_course);

                    // Course was not found. Returned by remove as a blank course indicates it couldn't find the course.
                    if(temp_course.getIdentifier() == 0){
                        cout << "Course not found." << endl << endl;
                        break;
                    }
                    cout << "Course removed." << endl << endl;
                }
                cout << endl << endl;
                break;
            case 9:
                // Exit feature per the clients requirements.
                cout << "Goodbye!" << endl;
                return 0;
            default:
                // Validation of input handling done by a default switch statement.
                cout << "Invalid choice. Please enter a valid choice." << endl;
                break;
        }
    } while (true);
}