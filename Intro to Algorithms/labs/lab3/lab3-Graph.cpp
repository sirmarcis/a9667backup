#include <iostream>
#include <fstream>
#include <limits.h>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <list>
#include <utility>

using namespace std;

int INFINITY = INT_MAX;

class Vertex {
public:
    string           name;   // Vertex name
    vector<Vertex *>  adj;    // Adjacent vertices
    int              dist;   // Cost
    Vertex          *path;   // Previous vertex on shortest path
    bool          visited;   // Keeps track of whether this vertex has been visited or not

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
    Graph( ) { }
    ~Graph( );
    void addEdge( const string & sourceName, const string & destName );
    void printPath( const string & destName ) const;
    void unweighted( const string & startName );

    void dfs();
    void explore(const string & vertexName);

    void reverseEdges();
      
  private:
    Vertex * getVertex( const string & vertexName );
    void printPath( const Vertex & dest ) const;
    void clearAll( );

    vmap vertexMap;
    vector<Vertex *> allVertices;
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

void Graph::printPath( const Vertex & dest ) const
{
    if( dest.path != NULL )
    {
        printPath( *dest.path );
        cout << " to ";
    }
    cout << dest.name;
}

void Graph::clearAll( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        allVertices[ i ]->reset( );
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
        Vertex *v = q.front( );	q.pop_front( );

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

void Graph::explore(const string & vertexName){
    Vertex* currVertex = getVertex(vertexName);
    currVertex->visited = true;
    vector<Vertex*> toVisit = currVertex->adj;
    for(int x = 0; x < toVisit.size(); x++){
        cout << "Vertex [" << vertexName << "] is connected to [" << toVisit[x]->name << "]" << endl;
        if(toVisit[x]->visited == false)
            explore(toVisit[x]->name);
        
    }
}

void Graph::dfs(){
    clearAll();
    for(int x = 0; x < allVertices.size(); x++)
        if(allVertices[x]->visited == false)
            explore(allVertices[x]->name);
    
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

bool processRequest( istream & in, Graph & g )
{
    string startName;
    string destName;

    cout << "Enter start node: ";
    if( !( in >> startName ) )
        return false;
    cout << "Enter destination node: ";
    if( !( in >> destName ) )
        return false;

    g.unweighted( startName );
    g.printPath( destName );

    return true;
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
    g.dfs();
    g.reverseEdges();
    cout << endl;
    g.dfs();
    //while( processRequest( cin, g ) );

    return 0;
}
