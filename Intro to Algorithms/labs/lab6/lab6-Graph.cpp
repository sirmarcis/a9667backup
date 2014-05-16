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
    void addEdge( const string & sourceName, const string & destName );
    void printPath( const string & destName ) const;
    void unweighted( const string & startName );

    void dfs();
    void explore(const string & vertexName);
    void findSCC();

    void bfs(const string& vertexName);
    void find_shortest_path(const string& startVertexName, const string& endVertexName);

    void reverseEdges();
      
  private:
    Vertex * getVertex( const string & vertexName );
    void printPath( const Vertex & dest ) const;
    void clearAll( );

    vmap vertexMap;
    vector<Vertex *> allVertices;

    void previsit(const Vertex * currVertex);
    void postvisit(const Vertex * currVertex);

    vector<int> pre;
    vector<int> post;

    int clock;
};



void Graph::addEdge( const string & sourceName, const string & destName )
{
    Vertex * v = getVertex( sourceName );
    Vertex * w = getVertex( destName );
    v->adj.push_back( w );
}

void Graph::printPath( const string & destName ) const
{
    vmap::const_iterator itr = vertexMap.find( destName );

    if( itr == vertexMap.end( ) )
    {
        cout << "Destination vertex not found" << endl;
        return;
    }

    const Vertex & w = *(*itr).second;
    if( w.dist == INFINITY )
        cout << destName << " is unreachable";
    else
        printPath( w );
    cout << endl;
}

// If vertexName is not present, add it to vertexMap
// In either case, return the Vertex
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

void Graph::printPath( const Vertex & dest ) const{
    if( dest.path != NULL )
    {
        printPath( *dest.path );
        cout << " to ";
    }
    cout << dest.name;
}

void Graph::clearAll( ){
    clock = 1;
    pre.resize(allVertices.size());
    post.resize(allVertices.size());
    for( int i = 0; i < allVertices.size( ); i++ ){
        pre[i] = 0; post[i] = 0;
        if(allVertices[i]->visited == false)
            allVertices[ i ]->index = i;
        allVertices[ i ]->reset( );
    }
}

Graph::~Graph( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        delete allVertices[ i ];
}


void Graph::unweighted( const string & startName )
{
    clearAll( );

    vmap::iterator itr = vertexMap.find( startName );

    if( itr == vertexMap.end( ) )
    {
        cout << startName << " is not a vertex in this graph" << endl;
        return;
    }

    Vertex *start = (*itr).second;
    list<Vertex *> q;
    q.push_back( start ); start->dist = 0;

    while( !q.empty( ) )
    {
        Vertex *v = q.front( ); q.pop_front( );

        for( int i = 0; i < v->adj.size( ); i++ )
        {
            Vertex *w = v->adj[ i ];
            if( w->dist == INFINITY )
            {
                w->dist = v->dist + 1;
                w->path = v;
                q.push_back( w );
            }
        }
    }
}

int largestPostNumber(vector<int>& oldPost){
    int largest = 0;
    int index = 0;
    for(int x = 0; x < oldPost.size(); x++){
        if(oldPost[x] > largest){
            largest = oldPost[x];
            index = x;
        }
    }
    oldPost[index] = 0;
    return index;
}

void Graph::findSCC(){
    reverseEdges();
    dfs();
    vector<int> oldPost = post;
    reverseEdges();
    clearAll();
    cout << endl;
    for(int x = 0; x < oldPost.size(); x++){
        int largestIndex = largestPostNumber(oldPost);
        if(allVertices[largestIndex]->visited == false){
            cout << "SCC{ ";
            explore(allVertices[largestIndex]->name);
            cout << " } " <<endl;
        }
    }

}

void Graph::dfs(){
    clearAll();
    for(int x = 0; x < allVertices.size(); x++)
        if(allVertices[x]->visited == false){
            //cout << "cc { ";
            explore(allVertices[x]->name);
            //cout << "}" << endl;
        }
    //for(int x = 0; x < pre.size(); x++)
    //    cout << "pre for vertex [" << allVertices[x]->name << "] is: [" << pre[x] << "], post is [" << post[x] << "]" <<endl;
}

void Graph::explore(const string & vertexName){
    Vertex* currVertex = getVertex(vertexName);
    currVertex->visited = true;
    vector<Vertex*> toVisit = currVertex->adj;
    previsit(currVertex);
    cout << vertexName << ", ";
    for(int x = 0; x < toVisit.size(); x++){
        //cout << "Vertex [" << vertexName << "] is connected to [" << toVisit[x]->name << "]" << endl;
        if(toVisit[x]->visited == false)
            explore(toVisit[x]->name);
        
    }
    postvisit(currVertex);
}

void Graph::previsit(const Vertex * currVertex){
    pre[currVertex->index] = clock;
    clock++;
}

void Graph::postvisit(const Vertex * currVertex){
    post[currVertex->index] = clock;
    clock++;
}

void reverseSetEdges(Vertex* currVertex, vector<pair<Vertex*, Vertex*> >& newVerticies){
    currVertex->visited = true;
    vector<Vertex*> toVisit = currVertex->adj;
    for(int x = 0; x < toVisit.size(); x++){
        newVerticies.push_back(make_pair(currVertex, toVisit[x]));
        if(toVisit[x]->visited == false)
            reverseSetEdges(toVisit[x], newVerticies);  
    }
}

void Graph::reverseEdges(){
    clearAll();
    vector<pair<Vertex*, Vertex*> > newVerticies; 
    for(int x = 0; x < allVertices.size(); x++){
        if(allVertices[x]->visited == false)
            reverseSetEdges(allVertices[x], newVerticies);
        allVertices[x]->adj.clear(); 
    }
    for(int x = 0; x < newVerticies.size(); x++){
        //cout << "[" << newVerticies[x].first->name <<"] [" << newVerticies[x].second->name << "]" << endl;
        newVerticies[x].second->adj.push_back(newVerticies[x].first);
    }
}

void Graph::bfs(const string& vertexName){
    int infinite = 100000000;
    Vertex* startVertex = getVertex(vertexName);
    for(int x = 0; x < allVertices.size(); x++)
        allVertices[x]->dist = infinite;
    startVertex->dist = 0;
    queue<Vertex*> theQueue;
    theQueue.push(startVertex);
    while(theQueue.size() > 0){
        Vertex* currVertex = theQueue.front();
        theQueue.pop();
        for(int h = 0; h < currVertex->adj.size(); h++){
            if(currVertex->adj[h]->dist == infinite){
                currVertex->adj[h]->dist = currVertex->dist + 1;
                currVertex->adj[h]->path = currVertex;
                theQueue.push(currVertex->adj[h]);
            }
        }
    }
    for(int x = 0; x < allVertices.size(); x++){
        if(allVertices[x]->dist == infinite)
            cout << "distance from [" << vertexName << "] to [" << allVertices[x]->name << "] is [INFINITY]" << endl;
        else
            cout << "distance from [" << vertexName << "] to [" << allVertices[x]->name << "] is [" << allVertices[x]->dist << "]" << endl;
    }
}

void Graph::find_shortest_path(const string& startVertexName, const string& endVertexName){
    Vertex* startVertex = getVertex(startVertexName);
    Vertex* endVertex = getVertex(endVertexName);
    int infinite = 100000000;
    for(int x = 0; x < allVertices.size(); x++)
        allVertices[x]->dist = infinite;
    startVertex->dist = 0;
    startVertex->path = NULL;
    queue<Vertex*> theQueue;
    theQueue.push(startVertex);
    while(theQueue.size() > 0){
        Vertex* currVertex = theQueue.front();
        theQueue.pop();
        for(int h = 0; h < currVertex->adj.size(); h++){
            if(currVertex->adj[h]->dist == infinite){
                currVertex->adj[h]->dist = currVertex->dist + 1;
                currVertex->adj[h]->path = currVertex;
                theQueue.push(currVertex->adj[h]);
            }
            if(currVertex->adj[h] == endVertex){
                while(!theQueue.empty())
                    theQueue.pop();
            }
        }
    }
    Vertex* currVertex = endVertex;
    vector<Vertex*> path;
    while(currVertex != NULL){
        path.push_back(currVertex);
        currVertex = currVertex->path;
    }
    reverse(path.begin(), path.end());
    cout << "Path from [" << startVertexName << "] to [" << endVertexName << "] is {";
    for(int x = 0; x < path.size(); x++)
        cout << path[x]->name << ", ";
    
    cout <<"}" <<endl;
    
}

bool processRequest( istream & in, Graph & g )
{
    string choice;
    string startName;
    string destName;
    cout << "type in 'Quit' to exit" << endl;
    cout << "Bredth first search or find shortest path (BFS, FSP)? ";
    in >> choice;
    if(choice == "BFS"){
        cout << "Enter start node: ";
        if( !( in >> startName ) )
            return false;
        g.bfs(startName);
        return true;
    }
    else if(choice == "FSP"){
        cout <<"Enter start node: ";
        if( !( in >> startName ) )
            return false;
        if(startName == "Quit")
            return false;
        cout << "Enter destination node: ";
        if( !( in >> destName ) )
            return false;
        //g.unweighted( startName );
        //g.printPath( destName );
        g.find_shortest_path(startName, destName);
        return true;
    }
    else
        return false;
}


//
// A simple main that reads the file given by argv[1]
// and then calls processRequest to compute shortest paths.
// Skimpy error checking in order to concentrate on the basics.
//
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

    string source, dest;

    // Read the vertices; add them to vertexMap
    while( inFile >> source >> dest ){
        g.addEdge( source, dest );
    }

    cout << "File read" << endl;

    return 0;
}
