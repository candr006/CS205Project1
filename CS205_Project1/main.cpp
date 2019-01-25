#include <iostream>
#include <queue>
#include <set>
using namespace std;
//change this value to make this work for a puzzle of different size
const int puzzle_size=8;
//array dimensions
const int dim=((puzzle_size)/2)-1;

bool checkStatesEqual(int (&a)[dim][dim], int (b)[dim][dim]){
    for( int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            if(a[i][j]!=b[i][j]){
                return false;
            }
        }
    }

    return true;
}

//Node struct has state and path_cost values
struct Node{
    int state[dim][dim];
    int path_cost=0;
    //keep track of where the blank is
    pair<int,int> pos_blank;
    //Add comparator to enable priority queue sorting by path_cost
    bool operator<(const Node& b)const {
        return this->path_cost<b.path_cost;
    }

    bool operator==(const Node& b)const {
        for( int i=0; i<dim; i++){
            for(int j=0; j<dim; j++){
                if(this->state[i][j]!=b.state[i][j]){
                    return false;
                }
            }
        }

        return true;
    }


};

pair<int,int> setStatesEqual(int (&a)[dim][dim], int (b)[dim][dim]){
    pair<int,int> pos_blank;
    for( int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            a[i][j]=b[i][j];

            if(a[i][j]==0){
                pos_blank.first=i;
                pos_blank.second=j;
            }
        }
    }

    return pos_blank;
}


void setNodesEqual(Node &a, Node b){
    setStatesEqual(a.state,b.state);
    a.path_cost=b.path_cost;

    return;
}

void printState(int a[dim][dim], string message){
    string space="";
    cout << message << endl;
    for( int i=0; i<dim; i++){
        space="";
        for(int j=0; j<dim; j++){
            cout << space << a[i][j];
            space= " ";
        }
        cout <<endl;
    }
    cout << endl;
    return;
}

//this function returns the neighbor of the state passed in with the blank moved
Node gen_neighbor(string movement, int (a)[dim][dim], pair<int, int> b_pos, int path_cost){
    int temp[dim][dim];
    Node n;
    pair<int,int> bpos= setStatesEqual(temp,a);
    if(movement=="up"){
       temp[(bpos.first)][bpos.second]=a[(bpos.first)-1][bpos.second];
       temp[(bpos.first)-1][bpos.second]=0;
       bpos= setStatesEqual(n.state,temp);
       n.pos_blank=bpos;
       n.path_cost=path_cost+1;
       return n;

    }
    else if(movement=="down"){
        temp[(bpos.first)][bpos.second]=a[(bpos.first)+1][bpos.second];
        temp[(bpos.first)+1][bpos.second]=0;
        bpos= setStatesEqual(n.state,temp);
        n.pos_blank=bpos;
        n.path_cost=path_cost+1;
        return n;
    }
    else if(movement=="right"){
        temp[(bpos.first)][bpos.second]=a[(bpos.first)][(bpos.second)+1];
        temp[(bpos.first)][(bpos.second)+1]=0;
        bpos= setStatesEqual(n.state,temp);
        n.pos_blank=bpos;
        n.path_cost=path_cost+1;
        return n;
    }
    else if(movement=="left"){
        temp[(bpos.first)][bpos.second]=a[(bpos.first)][(bpos.second)-1];
        temp[(bpos.first)][(bpos.second)-1]=0;
        bpos= setStatesEqual(n.state,temp);
        n.pos_blank=bpos;
        n.path_cost=path_cost+1;
        return n;
    }
    return n;
}


//this function returns all possible neighbors of the node passed in
//only legal moves are considered
vector<Node> getNeighbors(Node n){
    vector<Node> neighbors;

    if(n.pos_blank.first>0) {
        //move blank up possible
        neighbors.push_back(gen_neighbor("up", n.state, n.pos_blank, n.path_cost));
    }
    if(n.pos_blank.first<dim) {
        //move down up possible
        neighbors.push_back(gen_neighbor("down", n.state, n.pos_blank, n.path_cost));
    }

    if(n.pos_blank.second>0) {
        //move left up possible
        neighbors.push_back(gen_neighbor("left", n.state, n.pos_blank, n.path_cost));
    }
    if(n.pos_blank.second<dim) {
        //move right up possible
        neighbors.push_back(gen_neighbor("right", n.state, n.pos_blank, n.path_cost));
    }

        return neighbors;
}

void UniformCostSearch(int (puzzle)[dim][dim], int (goal)[dim][dim]){
    Node initial_state;
    priority_queue<Node> frontier;
    //set<int[dim][dim]> explored;
    set<Node> explored;
    set<Node> front_copy;
    int count=0;

    //set initial Node state to the puzzle
    pair<int,int> pblank = setStatesEqual(initial_state.state,puzzle);
    initial_state.pos_blank=pblank;

    //Add initial_state to frontier priority queue
    frontier.push(initial_state);
    front_copy.insert(initial_state);


    do {
        //Lowest cost Node
        Node n;
        set<Node, std::less<Node>, std::allocator<Node>>::iterator it;
        do {
            n = frontier.top();
            frontier.pop();
            it = front_copy.find(n);
            front_copy.erase(it);
        }while(explored.find(n)!=explored.end());

        //if the state that was popped is equal to the goal state, then we are done
        if(checkStatesEqual(n.state,goal)){
            cout<< "Solution Found" << endl;
            return;
        }
        //otherwise, add the state to those that have been explored
        explored.insert(n);
        if(count<10) {
            printState(n.state, "Expanding state: ");
            count++;
        }

        //find all neighbors of current state, n (max 4)
        //possible actions are move up, down, left, right
        vector<Node>neighbors=getNeighbors(n);
        for(int i=0; i<neighbors.size(); i++){
            it = front_copy.find(neighbors[i]);
            if((explored.find(neighbors[i])==explored.end()) && (it==front_copy.end())){
                frontier.push(neighbors[i]);
                front_copy.insert(neighbors[i]);
            }else if((it!=front_copy.end())){
                Node n=(*it);
                if(n.path_cost>neighbors[i].path_cost){
                    frontier.push(neighbors[i]);
                    front_copy.erase(n);
                    front_copy.insert(neighbors[i]);
                }
            }
        }
    }
    while(!frontier.empty());
    if(frontier.empty()){
        cerr << "FAILURE!" << endl;
        return;
    }




}

void general_search(int puzzle[dim][dim], int alg_choice){
    //goal state
    int count=1;
    int goal[dim][dim];
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            if (count<(puzzle_size+1)){
                goal[i][j]= count;
                count++;
            }
            else
                goal[i][j]= 0;

        }
    }



    switch(alg_choice){
        case 1:
            UniformCostSearch(puzzle,goal);
            break;
    };

    return;
}


int main(){
    int userchoice;
    int puzzleinput[dim][dim];

    cout<<"Welcome to Bertie Woosters 8-puzzle solver.\n"
          "Type “1” to use a default puzzle, or “2” to enter your own puzzle.\n";
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
               "3.\tA* with the Manhattan distance heuristic.\n";
        cin>>alg_choice;


        //general_search call with algorithm choice passed in
        general_search(puzzleinput,alg_choice);


    }

};