#include <iostream>
using namespace std;
//change this value to make this work for a puzzle of different size
const int puzzle_size=8;
//array dimensions
const int dim=((puzzle_size)/2)-1;

//Node struct has state and path_cost values
struct Node{
    int state[dim][dim];
    int path_cost=0;

};


void set_states_equal(int& a[dim][dim], int b[dim][dim]){
    for( int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            a[i][j]=b[i][j];
        }
    }
    return;
}

void UniformCostSearch(int puzzle[dim][dim]){
    Node initial_state;
    set_states_equal(initial_state.state,puzzle);
}

void general_search(int puzzle[dim][dim], int alg_choice){
    switch(alg_choice){
        case 1:
            UniformCostSearch(puzzle);
            break;
    };

    return;
}


int main(){
    int userchoice;
    int puzzleinput[dim][dim];

    cout<<"Welcome to Bertie Woosters 8-puzzle solver.\n"
          "Type “1” to use a default puzzle, or “2” to enter your own puzzle.";
    cin>>userchoice;

    if(userchoice==1){
        //default puzzle

    }else{
        //Enter own puzzle
        int rowinput;
        int alg_choice;

        cout << "Enter your puzzle, use a zero to represent the blank" 	<< endl;
        cout << "Enter the first row, use a space or tabs between numbers" << endl;
        for(int i=0; i<dim; i++){
            cin>>rowinput;
            puzzleinput[0][i]=rowinput;
        }

        cout << "Enter the second row, use a space or tabs between numbers" << endl;
        for(int i=0; i<dim; i++){
            cin>>rowinput;
            puzzleinput[1][i]=rowinput;
        }

        cout << "Enter the third row, use a space or tabs between 	numbers" << endl;
        for(int i=0; i<dim; i++){
            cin>>rowinput;
            puzzleinput[2][i]=rowinput;
        }

        cout <<"Enter your choice of algorithm\n"
               "1.\tUniform Cost Search\n"
               "2.\tA* with the Misplaced Tile heuristic.\n"
               "3.\tA* with the Manhattan distance heuristic.";
        cin>>alg_choice;


        //general_search call with algorithm choice passed in
        general_search(puzzleinput,alg_choice);


    }

};