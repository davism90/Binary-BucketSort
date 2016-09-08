/*Code written by Mariah Davis 09/02/2016*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>

using std::string; using std::cout; using std::cin; using std::ifstream;
using std::getline; using std::ispunct; using std::endl; 
using std::isspace; using std::vector; using std::sort; 
using std::ofstream;

struct Tree
//create a binary tree structure
{
	string word;
	Tree *mother;
	Tree *left;
	Tree *right;
};

struct Tree *new_node (string current_word)
//create a method create new nodes for our binary tree and return them.
//mother nodes represent the nodes above daughter nodes (the nodes
//that branch off from any mother). Linguistically daughters can be
//mothers and so on, but this makes it easy for me.
{
	Tree *node = new Tree;
	node -> word = current_word;
	node -> mother = NULL;
	node -> left = NULL;
	node -> right = NULL;
	
	return node;
}

struct Tree* insert_nodes (Tree *node, string current_word)
//create a method to inset our new nodes into our binary tree and return
//the nodes recursively until we get to the correct position.
{
	Tree *pointer;
	Tree *root_node;
	
	if (node == NULL){
		root_node = new_node(current_word);
		root_node -> mother = pointer;
		return root_node;
	}
	
	if (node->word >= current_word){
		pointer = node;
		node -> left = insert_nodes (node -> left, current_word);
	}
	
	else{
		pointer = node;
		node -> right = insert_nodes (node -> right, current_word);
	}
	
	return node;
}

void print_tree (struct Tree *node, vector<string> &words)
//print our trees to a vector in order to print to a file.
{
	if (node == NULL){
		return;
	}
	
	print_tree(node -> left, words);
	if (node -> word != ""){
		words.push_back(node -> word);
	}
	print_tree(node -> right, words);
	
}

void binary_tree_sort (vector<string> list_of_words)
//create an initial node from the first element in the vector, then
//compare the created nodes and insert them into the tree based on the
//previously defined structure. Then print out the nodes to a file
//using the vector from print_tree method.
{

	Tree* node = new_node(list_of_words[0]);
	for (unsigned int i = 1; i < list_of_words.size(); i++){
		insert_nodes (node, list_of_words[i]);
	}
	vector<string> words;
	ofstream output_file;
	output_file.open("outputfile.txt");
	print_tree(node, words);
	
	for (unsigned int i = 0; i < words.size(); i++){
		output_file << words[i] << endl;
	}
	
	output_file.close();
}

void bucket_sort (vector<string> &list_of_words)
//find the optimized number of buckets based on the total number of
//strings, then sort the words evenly into the buckets based on their
//position in the alphabet and the number of buckets, then sort the 
//words in the bucket and push onto a vector. Print out the vector to
//a file.

{	
	vector< vector<string> > buckets;
	buckets.resize((sqrt(list_of_words.size())) + 1);
	int bucket_tracker = 0;
	
	for (unsigned int i = 0; i < list_of_words.size(); i++){
		while (bucket_tracker < sqrt(list_of_words.size())){
			 if ((list_of_words[i][0] >= (97 + ((24 / 
				sqrt(list_of_words.size())) * (bucket_tracker))))
				&& (list_of_words[i][0] < 97 + ((24 / 
				sqrt(list_of_words.size())) * (bucket_tracker + 1))) && 
				bucket_tracker != 0){
					buckets[bucket_tracker].push_back(list_of_words[i]);
			}
			
			if (((list_of_words[i][0] >= (97))
				&& (list_of_words[i][0] < 97 + ((24 / 
				sqrt(list_of_words.size())) * (bucket_tracker + 1)))) && 
				bucket_tracker == 0){
					buckets[bucket_tracker].push_back(list_of_words[i]);
				}
				
			if ((list_of_words[i][0] <= 57 && list_of_words[i][0] >= 48 && list_of_words[i][0] <= (57/(sqrt(list_of_words.size()) * bucket_tracker))) && 
				bucket_tracker > 0){
					buckets[bucket_tracker].push_back(list_of_words[i]);
			}
			
			if (list_of_words[i][0] <= 57 && list_of_words[i][0] >= 48 && list_of_words[i][0] <= (57/(sqrt(list_of_words.size()) * bucket_tracker+1)) && 
				bucket_tracker == 0){
					buckets[bucket_tracker].push_back(list_of_words[i]);
			}
					
			bucket_tracker += 1;
		}
		bucket_tracker = 0;
	}
	
	list_of_words.clear();
	for (unsigned int i = 0; i < buckets.size(); i++){
		sort(buckets[i].begin(), buckets[i].begin() + buckets[i].size());
		for (unsigned int j = 0; j < buckets[i].size(); j++){
			list_of_words.push_back(buckets[i][j]);
		}
	}
	
	ofstream output_file;
	output_file.open ("outputfile.txt");
	for (unsigned int i = 0; i < list_of_words.size(); i++){
		output_file << list_of_words[i] << endl;
	}
	
	output_file.close();
}

void strip_text (string& line)
//strip the text of punctuation.
{
	for (unsigned int i = 0; i < line.size(); i++){
		line[i] = tolower(line[i]);
		if (ispunct(line[i])){
			line.erase(line.begin() + i);
			i--;
		}
	}
}

void open_file (string file_name, string sort_type)
//open the file, read in the strings. Run them through strip_text method
//and then strip the strings of whitespace. Push them onto a vector, and
//based on the sort type input, do a binary or bucket sort on them.
{
	string line, word = "";
	vector <string> list_of_words;
	
	ifstream file (file_name.c_str());
	while (getline (file, line)){
		strip_text (line);
		for (unsigned int i = 0; i < line.size(); i++){
			if ((isspace(line[i]) || line[i] == '\n') && i != line.size()-1 ){
				list_of_words.push_back(word);
				word = "";
			}
			
			if ((!(isspace(line[i])) || line[i] != '\n') && i == line.size()-1){
				word += line[i];
				list_of_words.push_back(word);
				word = "";
			}
			
			else if (!(isspace(line[i])) && i != line.size()-1 && 
				line[i] != '\n'){
				word += line[i];
			}
		}
		line = "";
	}
	
	if (sort_type == "bucket_sort"){
		bucket_sort(list_of_words);
	}
	
	if (sort_type == "binary_sort"){
		binary_tree_sort(list_of_words);
	}
	
	file.close();
}

int main (int argc, char* argv[])
//calculate the time the program runs, and receive the input from the 
//command line. 
//Give input [ /.cppfile -f filename -s sort_type ], where sort_type can 
//be either binary_sort or bucket_sort, into the terminal.
{
	clock_t start_time, end_time;
	start_time = clock();
	
	if (argc > 5 || argc < 5){
		cout << "Not enough arguments. Please enter" << 
			" your file name and sort type in the format " << 
			" -f filename -s sort_type";
	}
	
	else{
		string file_name, sort_type;
		for (int i = 1; i < argc; i++){
			string arg_string = argv[i];
			if (arg_string == "-f"){
				file_name = argv[i + 1];
			}
			
			else if (arg_string == "-s") {
				sort_type = argv[i + 1];
				open_file (file_name, sort_type);
			}
		}
	}	
	
	end_time = clock();
	
	float time_difference = end_time - start_time;
	cout<<"Your sorted list has been printed to 'outputfile.txt'." << 
		endl<< "Your runtime is: " << time_difference / 
		CLOCKS_PER_SEC << " seconds." << endl;
	
	return 0;
}
