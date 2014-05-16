#include <iostream>
#include <fstream>
#include <limits.h>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <list>
#include <utility>
#include <queue>
#include <algorithm>


using namespace std;

int INFINITY = INT_MAX;

class Vertex {
public:
    string           name;   // Vertex name
    vector<Vertex *>  adj;    // Adjacent vertices
    int              dist;   // Cost
    Vertex          *path;   // Previous vertex on shortest path
    bool          visited;   // Keeps track of whether this vertex has been visited or not
    int             index;   // Number that corresponds to where the vertex is in the map

    Vertex( const string & nm ) : name( nm )
      { reset( ); }

    void reset( )
      { dist = INFINITY; path = NULL; visited = false; }
};


typedef map<string,Vertex *> vmap;
typedef pair<string,Vertex *> vpair;


class Graph
{
  public:
    Graph( ) { clock = 1;}
    ~Graph( );
    void addEdge( const string & sourceName);
    void printPath( const string & destName ) const {}
    void unweighted( const string & startName ) {}
    void dfs();
    int longestWordLadder();
    int explore(const string & vertexName);
    bool wordLadderP(const string& sourceName, const string& destName);
    bool vertexExists(const string& vertexName);
      
  private:
  	bool digForVertex(Vertex* currVertex, const string& destName, vector<Vertex*> wordLadder);
  	bool reverseDigForVertex(const Vertex* currVertex, const Vertex* destVertex, vector<Vertex*> wordLadder);
    Vertex * getVertex( const string & vertexName );
    void printPath( const Vertex & dest ) const {}
    void clearAll( ) {}

    vmap vertexMap;
    vector<Vertex *> allVertices;

    int clock;
};

Graph::~Graph( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        delete allVertices[ i ];
}

int getWeight(const string& str1, const string& str2){
	int weight = 0;
	for(int x = 0; x < str1.size(); x++)
		if(str1.at(x) != str2.at(x))
			weight++;
	return weight;
}

Vertex * Graph::getVertex( const string & vertexName )
{
    vmap::iterator itr = vertexMap.find( vertexName );

    if( itr == vertexMap.end( ) )
    {
        Vertex *newv = new Vertex( vertexName );
        allVertices.push_back( newv );
        vertexMap.insert( vpair( vertexName, newv ) );
        return newv;
    }
    return (*itr).second;
}

void Graph::addEdge(const string& sourceName){
	Vertex* currClosestVertex;
	Vertex* currVertex;
	Vertex* newVertex = getVertex(sourceName);
	vector<Vertex*> adjVerticies;
	int currWeight;
	int smallestWeight = 6; //larger than any weight it can come up with
	for(int x = 0; x < allVertices.size(); x++){
		currVertex = allVertices[x];
		currWeight = getWeight(currVertex->name, newVertex->name);
		if(currWeight == 1){
			currVertex->adj.push_back(newVertex);
			newVertex->adj.push_back(currVertex);
		}
	}
}

void Graph::dfs(){
    clearAll();
    for(int x = 0; x < allVertices.size(); x++)
        if(allVertices[x]->visited == false)
            explore(allVertices[x]->name);       
}

int Graph::longestWordLadder(){
	clearAll();
	int longestPath = 0;
	int currPathLength = 0;
    for(int x = 0; x < allVertices.size(); x++){
        if(allVertices[x]->visited == false){
            currPathLength = explore(allVertices[x]->name);  
            if(currPathLength > longestPath)
            	longestPath = currPathLength;
        }
    }
    return longestPath;
}

int Graph::explore(const string & vertexName){
    Vertex* currVertex = getVertex(vertexName);
    currVertex->visited = true;
    vector<Vertex*> toVisit = currVertex->adj;
    int longestPath = 1;
    int currPathLength;
    for(int x = 0; x < toVisit.size(); x++){
        if(toVisit[x]->visited == false){
            currPathLength = 1 + explore(toVisit[x]->name);
            if(currPathLength > longestPath)
            	longestPath = currPathLength;
        }
        
    }
    return longestPath;
}

bool presentInWordLadder(const Vertex* vertex, vector<Vertex*> wordLadder){
	for(int x=  0; x < wordLadder.size(); x++)
		if(wordLadder[x]->name == vertex->name)
			return true;
	return false;
}

bool Graph::digForVertex(Vertex* currVertex, const string& destName, vector<Vertex*> wordLadder){
	wordLadder.push_back(currVertex);
	for(int x = 0; x < currVertex->adj.size(); x++){
		if(!presentInWordLadder(currVertex->adj[x], wordLadder)){
			if(currVertex->adj[x]->name == destName){
				wordLadder.push_back(currVertex->adj[x]);
				cout << "Found word Ladder!: ";
				for(int y = 0; y < wordLadder.size(); y++)
					cout << wordLadder[y]->name << ", ";
				cout << endl;
				return true;
			}
			else if(digForVertex(currVertex->adj[x], destName, wordLadder))
				return true;
		}
	}
	return false;
}

bool Graph::reverseDigForVertex(const Vertex* currVertex, const Vertex* destVertex, vector<Vertex*> wordLadder){
	if(currVertex->path == destVertex){
		for(int y = 0; y < wordLadder.size(); y++)
			cout << wordLadder[y]->name << ", ";
		cout << destVertex->name << endl;
		return true;
	}
	else if(getWeight(currVertex->path->name, currVertex->name) == 1){
		wordLadder.push_back(currVertex->path);
		return reverseDigForVertex(currVertex->path, destVertex, wordLadder);
	}
	return false;
}

bool Graph::vertexExists(const string& vertexName){
	vmap::iterator itr = vertexMap.find( vertexName );
    if( itr == vertexMap.end( ) )
    	return false;
    return true;
}

bool Graph::wordLadderP(const string& sourceName, const string& destName){	
	vector<Vertex*> wordLadder;
	if(vertexExists(sourceName)){
		Vertex* sourceVertex = getVertex(sourceName);
		return digForVertex(sourceVertex, destName, wordLadder);
	}
	else{
		wordLadder.push_back(new Vertex( sourceName ));
	}
	return false;

}

int main( int argc, char *argv[ ] )
{
    Graph g;

    if( argc != 2 ){
        cerr << "Usage: " << argv[ 0 ] << " graphfile" << endl;
        return 1;
    }

    ifstream inFile( argv[ 1 ] );
    if( !inFile ){
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }

    string source;
    int numEntries = 0;
    // Read the vertices; add them to vertexMap
    while( inFile >> source ){
    	numEntries++;
    	//cout << source <<endl;
        g.addEdge( source );
    }
    //would ->could
    //g.wordLadderP("smoke", "flame"); //orig "flame"
    cout << "longest word ladder: " << g.longestWordLadder() << ", where the total number of entries is: " << numEntries << endl;
    //g.dfs();

    cout << "File read" << endl;

    return 0;
}