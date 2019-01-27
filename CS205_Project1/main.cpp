#include <iostream>
#include <queue>
#include <set>
using namespace std;
//change this value to make this work for a puzzle of different size
const int puzzle_size=8;
//array dimensions
const int dim=((puzzle_size)/2)-1;

int debugnode[dim][dim]={
        {1,2,3},
        {5,6,8},
        {4,7,0}
};

bool checkStatesEqual(int (a)[3][3], int (b)[3][3]){
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
        return this->path_cost>b.path_cost;
    }

};
struct NodeCompare
{
    bool operator()(const Node& a, const Node& b)
    {
        int l=a.pos_blank.first+a.pos_blank.second;
        int r=b.pos_blank.first+b.pos_blank.second;
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

bool existsInVect(vector<Node> a, Node n){
    for(int i =0; i<a.size(); i++){
        Node b=a[i];
        if(checkStatesEqual(b.state,n.state)){
            return true;
        }
    }
    return false;
}

Node getNode(vector<Node> a, Node n, bool delete_it=false){

    for(int i =0; i<a.size(); i++){
        Node b=a[i];
        if(checkStatesEqual(b.state,n.state)){
            if(delete_it){
                a.erase(a.begin()+i);
            }
            return b;
        }
    }
    Node t;
    return t;
}

priority_queue<Node> deleteFromQueue(priority_queue<Node> a, Node n){
    priority_queue<Node> b;

    while(!a.empty()){
        Node t=a.top();
        a.pop();
        if(!checkStatesEqual(t.state,n.state)){
            b.push(t);
        }
    }

    return b;
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
    //printState(n.state," Parent:");
    if(n.pos_blank.first>0) {
        //move blank up possible
        Node neighbor=gen_neighbor("up", n.state, n.pos_blank, n.path_cost);
        neighbors.push_back(neighbor);
    }
    if(n.pos_blank.first<(dim-1)) {
        //move down up possible
        Node neighbor=gen_neighbor("down", n.state, n.pos_blank, n.path_cost);
        neighbors.push_back(neighbor);
    }

    if(n.pos_blank.second>0) {
        //move left up possible
        Node neighbor=gen_neighbor("left", n.state, n.pos_blank, n.path_cost);
        neighbors.push_back(neighbor);
    }
    if(n.pos_blank.second<(dim-1)) {
        //move right up possible
        Node neighbor=gen_neighbor("right", n.state, n.pos_blank, n.path_cost);
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

void UniformCostSearch(int (puzzle)[dim][dim], int (goal)[dim][dim]){
    Node initial_state;
    priority_queue<Node> frontier;
    //set<int[dim][dim]> explored;
    vector<Node> explored;
    vector<Node> front_copy;
    int count=0;

    //set initial Node state to the puzzle
    pair<int,int> pblank = setStatesEqual(initial_state.state,puzzle);
    initial_state.pos_blank=pblank;

    //Add initial_state to frontier priority queue
    frontier.push(initial_state);
    front_copy.push_back(initial_state);


    do {
        //Lowest cost Node
        Node n;
        set<Node, std::less<Node>, std::allocator<Node>>::iterator it;
        do {
            n = frontier.top();
            frontier.pop();
            getNode(front_copy,n,true);
        }while(existsInVect(explored,n));

        //if the state that was popped is equal to the goal state, then we are done
        if(checkStatesEqual(n.state,goal)){
            printState(goal,"Solution: ");
            cout<< "Solution Found in " << count<< " steps" << endl;
            return;
        }
        //otherwise, add the state to those that have been explored
        explored.push_back(n);
        count++;
        printState(n.state,"Step "+to_string(count)+" expanding with Path Cost "+to_string(n.path_cost));

        //find all neighbors of current state, n (max 4)
        //possible actions are move up, down, left, right
        vector<Node>neighbors=getNeighbors(n);
        for(int i=0; i<neighbors.size(); i++){

            if((!existsInVect(explored,neighbors[i])) && (!existsInVect(front_copy,neighbors[i]))){
                frontier.push(neighbors[i]);
            }
            if(existsInVect(front_copy,neighbors[i]) && (!existsInVect(explored,neighbors[i]))){
                Node t=getNode(front_copy,neighbors[i]);
                if(t.path_cost>neighbors[i].path_cost){
                    frontier=deleteFromQueue(frontier,t);
                    frontier.push(neighbors[i]);
                    Node t=getNode(front_copy,t,true);
                    front_copy.push_back(neighbors[i]);
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